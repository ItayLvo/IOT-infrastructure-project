package gatewayserver;

import factory.Command;

import java.util.Map;

public class GatewayServer {
    RequestProcessingService RPS;

    public GatewayServer() {
        Parser parser = new CommandParser();
        this.RPS = new RequestProcessingService(parser);
    }

    public void handleRequest(String request) {
        RPS.handleRequest(request);

        System.out.println("done...");
    }


}
