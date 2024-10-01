package server;

public class ChatroomServer {

    public static void main(String[] args) {
        ChatroomServer server = new ChatroomServer("10.10.1.105", 9111);
        server.start();
    }

    RequestHandler requestHandler = new RequestHandler();
    private final String hostName;
    private final int portNumber;


    public ChatroomServer(String hostName, int portNumber) {
        this.hostName = hostName;
        this.portNumber = portNumber;
    }

    public void start() {
        Thread serverThread = new Thread(this::listen);
        serverThread.start();
    }

    public void stop() {
        requestHandler.stopRunning();
    }

    private void listen() {
        requestHandler.startListening(hostName, portNumber);
    }

}
