package server;

public class ChatroomServer {
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


    private void listen() {
        requestHandler.startListening(hostName, portNumber);
    }

}
