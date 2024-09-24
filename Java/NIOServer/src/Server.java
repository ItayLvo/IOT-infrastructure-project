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

        //try with resources block to create server socket channel and selector
        try (ServerSocketChannel serverSocketChannel = ServerSocketChannel.open();
             Selector selector = Selector.open()) {

            serverSocketChannel.configureBlocking(false);
            serverSocketChannel.bind(new InetSocketAddress(port));
            //register the server channel with the selector for "accept" events
            serverSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
            ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

            while (true) {
                // wait for events - blocking until an event is rdy
                selector.select();
                for (SelectionKey key : selector.selectedKeys()) {
                    if (key.isAcceptable()) {
                        if (key.channel() instanceof ServerSocketChannel) {
                            ServerSocketChannel channel = (ServerSocketChannel) key.channel();
                            SocketChannel client = channel.accept();
                            channels.add(client);   //add all client sockets to collection that will be all closed when server dies
                            client.configureBlocking(false);
                            Socket socket = client.socket();
                            System.out.println("client remote address: " + client.getRemoteAddress());
                            System.out.println("client's socket inet address: " + socket.getInetAddress());
                            client.register(selector, SelectionKey.OP_READ);
                        }
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
                selector.selectedKeys().clear();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            for (SocketChannel client : channels) {
                client.close();
            }
        }
    }
}
