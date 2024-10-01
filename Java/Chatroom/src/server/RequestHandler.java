package server;

import il.co.ilrd.chatroom_server.*;
import java.io.ByteArrayInputStream;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.channels.*;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import java.io.ObjectInputStream;

public class RequestHandler {
    private volatile boolean isServerRunning;
    private final ChatroomManager chatroomManager = new ChatroomManager();
    private final MessageHandler messageHandler = new MessageHandler();


    public void startListening(String hostName, int portNumber) {
        isServerRunning = true;
        Set<SocketChannel> tcpClientChannels = new HashSet<>();
        ByteBuffer byteBuffer = ByteBuffer.allocate(2048);


        //create a non-blocking ServerSocketChannel and selector in try-with-resources block
        try (ServerSocketChannel tcpServerSocketChannel = ServerSocketChannel.open();
             Selector selector = Selector.open()) {

            //set up the TCP channels: configure blocking, bind to address, register with selector
            setupTcpChannel(tcpServerSocketChannel, hostName, portNumber, selector);

            while (isServerRunning) {
                //wait for events - blocking until an event is rdy
                int numEvents = selector.select();
                if (numEvents == 0) {
                    System.out.println("num events = 0");
                    continue;
                }
                else {
                    System.out.println("num events = " + numEvents);
                }

                //iterate over all selected keys (events)
                Iterator<SelectionKey> key = selector.selectedKeys().iterator();
                while (key.hasNext()) {
                    SelectionKey selectionKey = key.next();
                    //handle new connection events
                    if (selectionKey.isAcceptable()) {
                        handleConnectionRequest(selectionKey, tcpClientChannels, selector);
                    } else if (selectionKey.isReadable()) {  //handle read events
                        if (selectionKey.channel() instanceof SocketChannel) {
                            handleTcpReadEvent(selectionKey, byteBuffer);
                        }
                    }
                    key.remove();
                }

//                for (SelectionKey key : selector.selectedKeys()) {
//                    //handle new connection events
//                    if (key.isAcceptable()) {
//                        handleConnectionRequest(key, tcpClientChannels, selector);
//                    } else if (key.isReadable()) {  //handle read events
//                        if (key.channel() instanceof SocketChannel) {
//                            handleTcpReadEvent(key, byteBuffer);
//                        }
//                    }
//                }
                //clear the selected keys to prepare for the next set of events
                selector.selectedKeys().clear();
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            //close all client tcpClientChannels when the server shuts down
            for (SocketChannel client : tcpClientChannels) {
                try {
                    client.close();
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

    public void stopRunning() {
        isServerRunning = false;
    }


    private void handleTcpReadEvent(SelectionKey key, ByteBuffer byteBuffer) throws IOException {
        SocketChannel client = (SocketChannel) key.channel();
        Message message = null;
        //receive message from client:
        int bytesRead = client.read(byteBuffer);
        if (bytesRead == -1) {
            client.close();
            return;
        }
        byteBuffer.flip();
        //convert the ByteBuffer into a byte array
        byte[] byteArray = new byte[byteBuffer.remaining()];
        //copies data from buffer to byteArray
        byteBuffer.get(byteArray);
        //create ByteArrayInputStream from the byte array
        ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(byteArray);
        //deserialize the message into a Server.Message instance
        try (ObjectInputStream objectInputStream = new ObjectInputStream(byteArrayInputStream)) {
            message = (Message)objectInputStream.readObject();
            messageHandler.handleMessage(message, client, chatroomManager);

        } catch (IOException | ClassNotFoundException e) {
            System.out.println("de-serialization failed");
            e.printStackTrace();
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
            System.out.println("[Server] client connected. remote address: " + client.getRemoteAddress());
            //register the client channel with the selector, interested in reading from the client
            client.register(selector, SelectionKey.OP_READ);
        }
    }



    private static void setupTcpChannel(ServerSocketChannel tcpServerSocketChannel, String hostName, int port, Selector selector) throws IOException {
        tcpServerSocketChannel.configureBlocking(false);
        //bind the server to a specific port number
        tcpServerSocketChannel.bind(new InetSocketAddress(hostName, port));
        //register the server channel with the selector for "accept" events (new connections)
        tcpServerSocketChannel.register(selector, SelectionKey.OP_ACCEPT);
    }





}
