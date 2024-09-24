import java.io.IOException;

public class MainNioServerTest {
    public static void main(String[] args) throws IOException, InterruptedException {
        Server server = new Server();
        server.start(9111);
    }
}
