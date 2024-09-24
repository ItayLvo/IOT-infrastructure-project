import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class Client {

    public void start(final int portNumber) throws IOException, InterruptedException {
        try (SocketChannel channel = SocketChannel.open()) {
            channel.connect(new InetSocketAddress(portNumber));

            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

            while (true) {
                String message = System.lineSeparator() + "hello, i am client!";
                byteBuffer.clear();
                byteBuffer.put(message.getBytes());
                byteBuffer.flip(); //flip buffer to prepare for writing to the channel
                while (byteBuffer.hasRemaining()) {
                    channel.write(byteBuffer);
                }

                //read response from server:
                byteBuffer.clear();
                int bytesRead = channel.read(byteBuffer);
                if (bytesRead > 0) {
                    byteBuffer.flip();  //prepare buffer for reading
                    byte[] response = new byte[bytesRead];
                    byteBuffer.get(response);
                    System.out.println("The response from the server is: "  + new String(response));
                }

                Thread.sleep(3000);
            }
        }
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        Client client = new Client();
        client.start(9111);
    }
}


