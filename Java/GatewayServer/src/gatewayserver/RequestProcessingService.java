package gatewayserver;

import factory.*;
import threadpool.ThreadPool;

import java.util.Map;
import java.util.concurrent.Callable;

public class RequestProcessingService {
    private final Parser parser;
    private final Factory<String, Command, String> commandFactory = new Factory<>();
    private final ThreadPool threadPool = new ThreadPool(3);


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
        commandFactory.add("registerCompany", registerCompanyCommand::new);
    }

    public void handleRequest(String request) {
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
        threadPool.execute(requestRunnable);
    }

}
