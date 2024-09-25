import java.io.IOException;

public class MainNioServerTest {
    public static void main(String[] args) throws IOException, InterruptedException {
        Server server = new Server("10.1.0.85", 9111);
        server.listen();
        Thread.sleep(10000);
        server.stop();
    }
}
