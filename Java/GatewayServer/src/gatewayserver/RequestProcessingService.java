package gatewayserver;

import factory.*;
import pluginservice.DirMonitor;
import pluginservice.DynamicJarLoader;
import threadpool.ThreadPool;

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
    private final PluginService pluginService = new PluginService("/home/itay/git/build/test");

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

    public void executeCommand(Command command) {   //TODO do i need this?
        threadPool.execute(command);
    }

    //private method to set up all factory creation methods
    private void initializeFactory() {
//        commandFactory.add("registerCompany", RegisterCompanyCommand::new); //before adding the addRecipe method. removed row for code-reuse TODO
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


    public class PluginService {
        private final String pluginDirectory;
        private final DirMonitor dirMonitor;
        private final DynamicJarLoader jarLoader = new DynamicJarLoader();


        public PluginService(String pluginDirectory) {
            this.pluginDirectory = pluginDirectory;
            this.dirMonitor = new DirMonitor(pluginDirectory, this);

            //create and run the directory watcher thread
            Thread dirMonitorThread = new Thread(dirMonitor);
            dirMonitorThread.start();
        }


        public void handleJARDetected(Path pathToJAR) throws IOException, ClassNotFoundException, NoSuchMethodException {
            String fullPath = pluginDirectory + "/" + pathToJAR.toString();
            System.out.println(fullPath);
            List<Class<?>> newCommandsList = jarLoader.loadClassesFromJAR(fullPath);
            for (Class<?> newCommand : newCommandsList) {
                    String newCommandName = newCommand.getSimpleName();
                    Constructor<?> commandConstructor = newCommand.getConstructor(String.class);

                    // create a Function<String, Command> using the constructor
                    Function<String, Command> constructorFunction = (dataArgument) -> {
                        try {
                            // instantiate the command using the constructor
                            return (Command) commandConstructor.newInstance(dataArgument);
                        } catch (Exception e) {
                            System.out.println(e.getMessage());
                            throw new RuntimeException("Failed to create command instance", e);
                        }
                    };

                    RequestProcessingService.this.addRecipeToCommandFactory(newCommandName, constructorFunction);
            }
        }


    }

}
