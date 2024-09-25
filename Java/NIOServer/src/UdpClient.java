import java.io.IOException;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.DatagramChannel;

public class UdpClient {

    public static void main(String[] args) throws IOException, InterruptedException {
        UdpClient client = new UdpClient();
        client.start(9111);
    }
    public void start(final int portNumber) throws IOException, InterruptedException {
        try (DatagramChannel channel = DatagramChannel.open()) {
            InetSocketAddress socketAddress = new InetSocketAddress(InetAddress.getByName("10.1.0.85"), portNumber);

            ByteBuffer byteBuffer = ByteBuffer.allocate(512);

            while (true) {
                byteBuffer.clear();
                byte[] msg = "hi from UDP client".getBytes();
                byteBuffer.put(msg);
                byteBuffer.flip();
//                DatagramPacket datagramPacket = new DatagramPacket(msg, msg.length, InetAddress.getByName("localhost"), portNumber);  //dont need this??
                channel.send(byteBuffer, socketAddress);

                //receive response from server:
                byteBuffer.clear();
                InetSocketAddress senderAddress = (InetSocketAddress) channel.receive(byteBuffer);
                byteBuffer.flip();
                System.out.println("response from server is: " + new String(byteBuffer.array()).trim());

                Thread.sleep(2000);
            }
        }
    }
}
