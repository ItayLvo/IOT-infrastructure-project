import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.HashSet;
import java.util.Set;

public class Server {

    public void start(final int portNumber) throws IOException {
        int port = portNumber;
        Set<SocketChannel> channels = new HashSet<>();

        //create a non-blocking ServerSocketChannel and selector
        try (ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
             Selector selector = Selector.open()) {

            serverSocketChannel.configureBlocking(false);

            //bind the server to a specific port number
            serverSocketChannel.bind(new InetSocketAddress(port));

            //register the server channel with the selector for "accept" events (new connections)
            serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

            while (true) {
                // wait for events - blocking until an event is rdy
                selector.select();

                //iterate over all selected keys (events)
                for (SelectionKey key : selector.selectedKeys()) {
                    //handle new connection events
                    if (key.isAcceptable()) {
                        if (key.channel() instanceof ServerSocketChannel) {
                            //accept the new connection and create a SocketChannel for the client
                            ServerSocketChannel channel = (ServerSocketChannel) key.channel();
                            SocketChannel client = channel.accept();
                            channels.add(client);   //add all client sockets to collection that will be all closed when server dies
                            client.configureBlocking(false);
                            Socket socket = client.socket();
                            System.out.println("client remote address: " + client.getRemoteAddress());
                            System.out.println("client's socket inet address: " + socket.getInetAddress());
                            //register the client channel with the selector, interested in reading from the client
                            client.register(selector, SelectionKey.OP_READ);
                        }
                    //handle read events (data from clients)
                    } else if (key.isReadable()) {
                        if (key.channel() instanceof SocketChannel) {
                            SocketChannel client = (SocketChannel) key.channel();
                            channels.add(client);

                            //receive message from client:
                            int bytesRead = client.read(byteBuffer);
                            byteBuffer.flip();
                            byte[] message = new byte[byteBuffer.remaining()];
                            byteBuffer.get(message);
                            System.out.println("The message from the client is: " + new String(message));

                            //prepare server response to client:
                            byteBuffer.clear();
                            String response = "Server received: " + new String(message);
                            byteBuffer.put(response.getBytes());
                            byteBuffer.flip();

                            //write response back to client
                            while (byteBuffer.hasRemaining()) {
                                client.write(byteBuffer);
                            }
                            byteBuffer.clear();
                        }
                    }
                }
                //clear the selected keys to prepare for the next set of events
                selector.selectedKeys().clear();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            //close all client channels when the server shuts down
            for (SocketChannel client : channels) {
                client.close();
            }
        }
    }
}
