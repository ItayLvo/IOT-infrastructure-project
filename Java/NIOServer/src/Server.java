import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.HashSet;
import java.util.Set;

public class Server {

    public void start(final int portNumber) throws IOException {
        Set<SocketChannel> tcpClientChannels = new HashSet<>();
        ByteBuffer byteBuffer = ByteBuffer.allocate(1024);

        //create a non-blocking ServerSocketChannel, DatagramChannel and selector in try-with-resources block
        try (ServerSocketChannel tcpServerSocketChannel = ServerSocketChannel.open();
             DatagramChannel udpChannel = DatagramChannel.open();
             Selector selector = Selector.open()) {

            //set up the UDP and TCP channels: configure blocking, bind to address, register with selector
            setupTcpChannel(tcpServerSocketChannel, portNumber, selector);
            setupUdpChannel(udpChannel, portNumber, selector);


            while (true) {
                //wait for events - blocking until an event is rdy
                selector.select();
                //iterate over all selected keys (events)
                for (SelectionKey key : selector.selectedKeys()) {
                    //handle new connection events
                    if (key.isAcceptable()) {
                        handleConnectionRequest(key, tcpClientChannels, selector);
                    //handle read events
                    } else if (key.isReadable()) {
                        if (key.channel() instanceof SocketChannel) {
                            handleTcpReadEvent(key, byteBuffer);
                        }
                        else if (key.channel() instanceof DatagramChannel) {
                            handleUdpReadEvent(key, byteBuffer);
                        }
                    }
                }
                //clear the selected keys to prepare for the next set of events
                selector.selectedKeys().clear();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            //close all client tcpClientChannels when the server shuts down
            for (SocketChannel client : tcpClientChannels) {
                client.close();
            }
        }
    }


    private static void handleUdpReadEvent(SelectionKey key, ByteBuffer byteBuffer) throws IOException {
        DatagramChannel udpClientChannel = (DatagramChannel) key.channel();
        byteBuffer.clear();
        //handle message from client:
        InetSocketAddress clientAddress = (InetSocketAddress) udpClientChannel.receive(byteBuffer);
        byteBuffer.flip();
        byte[] message = new byte[byteBuffer.remaining()];
        byteBuffer.get(message);
        System.out.println("The message from the client is: " + new String(message));

        //respond to UDP client:
        byteBuffer.clear();
        byteBuffer.put(("Server received: " + new String(message)).getBytes());
        byteBuffer.flip();
        udpClientChannel.send(byteBuffer, clientAddress);
        byteBuffer.clear();
    }


    private static void handleTcpReadEvent(SelectionKey key, ByteBuffer byteBuffer) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
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


    private static void handleConnectionRequest(SelectionKey key, Set<SocketChannel> tcpClientChannels, Selector selector) throws IOException {
        if (key.channel() instanceof ServerSocketChannel) {
            //accept the new connection and create a SocketChannel for the client
            ServerSocketChannel channel = (ServerSocketChannel) key.channel();
            SocketChannel client = channel.accept();
            tcpClientChannels.add(client);   //add all client sockets to collection that will be all closed when server dies
            client.configureBlocking(false);
            Socket socket = client.socket();
            System.out.println("client remote address: " + client.getRemoteAddress());
            System.out.println("client's socket inet address: " + socket.getInetAddress());
            //register the client channel with the selector, interested in reading from the client
            client.register(selector, SelectionKey.OP_READ);
        }
    }


    private static void setupUdpChannel(DatagramChannel udpChannel, int port, Selector selector) throws IOException {
        udpChannel.configureBlocking(false);
        //bind the DatagramChannel to the local address for listening to inbound UDP packets
        udpChannel.bind(new InetSocketAddress(port));
        //register the channel with the selector for reading events
        udpChannel.register(selector, SelectionKey.OP_READ);
    }


    private static void setupTcpChannel(ServerSocketChannel tcpServerSocketChannel, int port, Selector selector) throws IOException {
        tcpServerSocketChannel.configureBlocking(false);
        //bind the server to a specific port number
        tcpServerSocketChannel.bind(new InetSocketAddress(port));
        //register the server channel with the selector for "accept" events (new connections)
        tcpServerSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
    }

}
