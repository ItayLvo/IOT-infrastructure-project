package tcp;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class ClientTest {
    public static void main(String[] args) throws IOException {
        try {
            InetAddress amitAddress = InetAddress.getByName("10.10.2.79");
            Socket socket = new Socket(amitAddress.getHostName(), 59595);
            //if i needed to read from console:
//            BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);  //true for auto-flush buffer
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            while (true) {
                out.println("ping");
                Thread.sleep(1000);
                String message = in.readLine();
                System.out.println(message);
            }
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }
    }
}
