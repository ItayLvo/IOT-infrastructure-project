package gatewayserver;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public class Main {
    public static void main(String[] args) {
        //TODO notice: wrong key should throw RuntimeException, but doesn't because
        GatewayServer server = new GatewayServer();
        Future<?> result1 = server.handleRequest("@registerCompany@Amazon");


        try {
            Thread.sleep(7000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        Future<?> result2 = server.handleRequest("@RegisterProductCommand@*******************");

        try {
            System.out.println(result1.get());
            System.out.println(result2.get());
        } catch (InterruptedException | ExecutionException e) {
            System.out.println(e.getMessage());
        }

    }
}