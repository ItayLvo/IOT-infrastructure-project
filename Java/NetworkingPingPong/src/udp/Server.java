package udp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Server {
    public static void main(String[] args) {
        try {
            DatagramSocket socket = new DatagramSocket(9111);
            byte[] buffer = new byte[1024];
            while (true) {
                DatagramPacket packet = new DatagramPacket(buffer, buffer.length);
                socket.receive(packet);
                InetAddress senderAddress = packet.getAddress();
                int port = packet.getPort();
                String addressAsString = new String(senderAddress.getAddress());
                System.out.println("address of sender = " + addressAsString + ", port of sender = " + port);
                String messageFromClient = new String(packet.getData(), 0, packet.getLength());
                System.out.println("message from client = " + messageFromClient);
                String response = ("hi its server and i received your msg: " + messageFromClient);
                packet = new DatagramPacket(response.getBytes(), response.getBytes().length, senderAddress, port);
                packet.setData("message from server to client".getBytes());
                socket.send(packet);
                Thread.sleep(2000);
            }
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
