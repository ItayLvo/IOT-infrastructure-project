import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class TcpClient {

    public void start(final int portNumber) throws IOException, InterruptedException {
        try (SocketChannel channel = SocketChannel.open()) {

            //connect the channel to a specific address and port
            InetSocketAddress serverAddress = new InetSocketAddress("localhost", portNumber);
            channel.connect(serverAddress);


            String message = "hi from TCP client";
            ByteBuffer byteBuffer = ByteBuffer.allocate(512);

            while (true) {
                //prepare a message for the server:
                byteBuffer.clear();
                byteBuffer.put(message.getBytes());
                byteBuffer.flip(); //flip buffer to prepare for writing to the channel
                //write the message to the server:
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
        TcpClient client = new TcpClient();
        client.start(9111);
    }
}


