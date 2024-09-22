package gatewayserver;

import factory.*;
import pluginservice.DirMonitor;
import pluginservice.DynamicJarLoader;
import threadpool.ThreadPool;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Constructor;
import java.nio.file.Path;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Future;
import java.util.function.Function;

public class RequestProcessingService {
    private final Parser parser;
    private final Factory<String, Command, String> commandFactory = new Factory<>();
    private final ThreadPool threadPool = new ThreadPool(3);    //TODO change this argument later
    private final PluginService pluginService = new PluginService();

    public RequestProcessingService(Parser parser) {
        this.parser = parser;
        initializeFactory();
    }

    public Map.Entry<String, String> parseRequest(String request) {
        return parser.parse(request);
    }

    public Command createCommand(String key, String data) {
        return commandFactory.create(key, data);
    }


    //private method to set up all factory creation methods
    private void initializeFactory() {
        addRecipeToCommandFactory("registerCompany", RegisterCompanyCommand::new);
    }

    private void addRecipeToCommandFactory(String key, Function<String, Command> newCommandConstructor) {
        System.out.println("***adding to factory: " + key);
        System.out.println("***adding to factory: " + newCommandConstructor);
        this.commandFactory.add(key, newCommandConstructor);
    }

    public Future<?> handleRequest(String request) {
        //create a Runnable that parses the request, creates a command using the Command Factory, and then executes it.
        //then submit that Runnable to the thread pool
        Runnable requestRunnable = new Runnable() {
            @Override
            public void run() {
                Map.Entry<String, String> keyDataPair = parseRequest(request);
                Command command = createCommand(keyDataPair.getKey(), keyDataPair.getValue());
                command.run();
            }
        };
        return threadPool.submit(requestRunnable);
    }


    private class PluginService {
        private static final String pluginDirectory = "/home/itay/git/build/test";

        public PluginService() {
            loadInitialJARsFromDir();

            //create and run the directory watcher thread
            DirMonitor dirMonitor = new DirMonitor(pluginDirectory, this::handleJARDetected);
            Thread dirMonitorThread = new Thread(dirMonitor);
            dirMonitorThread.start();
        }


        private void handleJARDetected(Path pathToJAR) {
            String fullPath = pluginDirectory + "/" + pathToJAR.toString();
            System.out.println(fullPath);
            List<Class<?>> newCommandsList;
            try {
                newCommandsList = DynamicJarLoader.loadClassesFromJAR(fullPath);
            } catch (IOException | ClassNotFoundException e) {
                throw new RuntimeException("Couldn't load classes from JAR file", e);
            }

            for (Class<?> newCommand : newCommandsList) {
                    String newCommandName = newCommand.getSimpleName();
                Constructor<?> commandConstructor;
                try {
                    commandConstructor = newCommand.getConstructor(String.class);
                } catch (NoSuchMethodException e) {
                    throw new RuntimeException("Couldn't get a constructor which accepts String as an argument", e);
                }

                // create a Function<String, Command> using the constructor
                Constructor<?> finalCommandConstructor = commandConstructor;
                Function<String, Command> constructorFunction = (dataArgument) -> {
                        try {
                            // instantiate the command using the constructor
                            return (Command) finalCommandConstructor.newInstance(dataArgument);
                        } catch (Exception e) {
                            System.out.println(e.getMessage());
                            throw new RuntimeException("Failed to create command instance", e);
                        }
                    };

                    RequestProcessingService.this.addRecipeToCommandFactory(newCommandName, constructorFunction);
            }
        }


        private void loadInitialJARsFromDir() {
            File JarDirectory = new File(pluginDirectory);
            if (!JarDirectory.exists() || !JarDirectory.isDirectory()) {
                return;
            }

            for (File file : JarDirectory.listFiles()) {
                if (file.getName().endsWith(".jar")) {
                    handleJARDetected(file.toPath());
                }
            }
        }



    }   //end of PluginService inner class

}
