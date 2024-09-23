package tcp;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class Server {

    public static void main(String[] args) {
        ServerSocket serverSocket;

        try {
            System.out.println("Started server main");
            serverSocket = new ServerSocket(59595);
            System.out.println("waiting for connection");
            Socket socket = serverSocket.accept();
            System.out.println("connection accepted!");
            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
//            BufferedWriter out = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            while (true) {
                String line = in.readLine();
                Thread.sleep(1000);
                if (line.equals("ping")) {
                    System.out.println("ping");
                    out.println("pong");
                }
                else if (line.equals("pong")) {
                    System.out.println("pong");
                    out.println("ping");
                }
            }
        } catch (IOException | InterruptedException e) {
            throw new RuntimeException(e);
        }

    }
}
