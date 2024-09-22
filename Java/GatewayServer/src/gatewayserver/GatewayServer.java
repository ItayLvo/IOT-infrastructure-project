package gatewayserver;

import java.util.concurrent.Future;

public class GatewayServer {
    private final RequestProcessingService RPS;

    public GatewayServer() {
        Parser parser = new CommandParser();
        this.RPS = new RequestProcessingService(parser);
    }

    public Future<?> handleRequest(String request) {
        return RPS.handleRequest(request);

    }


}
