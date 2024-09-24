package udp;

import java.io.IOException;
import java.net.*;
import java.util.Arrays;
import java.util.Scanner;

public class Client {
    public static void main(String[] args) {
        try {
            DatagramSocket socket = new DatagramSocket();
            byte[] buffer = new byte[1024];

            while (true) {
                byte[] msg = "hi it's client!".getBytes();
                DatagramPacket packet = new DatagramPacket(msg, msg.length, InetAddress.getByName("localhost"), 9111);
                socket.send(packet);
                DatagramPacket receivedPacket = new DatagramPacket(buffer, buffer.length);
                socket.receive(receivedPacket);
                System.out.println("msg received from server = " + new String(receivedPacket.getData()));
                Thread.sleep(2000);
            }
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
