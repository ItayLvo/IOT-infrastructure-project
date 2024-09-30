package client;

import server.MessageType;
import server.Message;

import java.io.*;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Scanner;

public class ChatroomClient {
    private SocketChannel channel;
    private String username;

    public void start(final int portNumber) throws IOException, InterruptedException {
        try (SocketChannel channel = SocketChannel.open()) {
            this.channel = channel;
            ByteBuffer byteBuffer = ByteBuffer.allocate(2048);

            //connect the channel to a specific address and port
            InetSocketAddress serverAddress = new InetSocketAddress("10.1.0.85", portNumber);
            channel.connect(serverAddress);

            //init the listener thread
            Thread listenerThread = new Thread(this::listenToServer);
            listenerThread.start();

            communicateWithChatroom();
        }
    }

    private void communicateWithChatroom() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("write /exit to exit.\nwrite /register [username] to register.\nwrite /unregister to unregister.\nwrite a message to send a message.");
        while (true) {
            String message = scanner.nextLine();
            if (message.equals("/exit")) {
                try {
                    unregister();
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
                break;
            }
            if (message.contains("/register")) {
                String username = message.split(" ")[1];
                try {
                    registerToChatroom(username);
                    this.username = username;
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            if (message.equals("/unregister")) {
                try {
                    unregister();
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
            else {
                Message myMessage = new Message(message, MessageType.SEND_MESSAGE);
                try {
                    sendMessageToServer(myMessage);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        }
    }

    private void unregister() throws IOException {
        Message unregisterMessage = new Message(this.username, MessageType.UNREGISTER);
        sendMessageToServer(unregisterMessage);
    }

    private void sendMessageToServer(Message message) throws IOException {
        //serialize the Message object to a byte array
        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        try (ObjectOutputStream objectOutputStream = new ObjectOutputStream(byteArrayOutputStream)) {
            objectOutputStream.writeObject(message);
            objectOutputStream.flush();  //ensure all data is written
        }

        //get the byte array containing the serialized Server.Message object
        byte[] messageBytes = byteArrayOutputStream.toByteArray();

        //wrap the byte array in a ByteBuffer
        ByteBuffer buffer = ByteBuffer.wrap(messageBytes);

        //write the ByteBuffer to the SocketChannel
        while (buffer.hasRemaining()) {
            channel.write(buffer);
        }
    }

    private void registerToChatroom(String username) throws IOException {
        String myUsername = username;
        Message registerationMessage = new Message(myUsername, MessageType.REGISTER);

        ByteArrayOutputStream byteArrayOutputStream = new ByteArrayOutputStream();
        try (ObjectOutputStream objectOutputStream = new ObjectOutputStream(byteArrayOutputStream)) {
            objectOutputStream.writeObject(registerationMessage);
            objectOutputStream.flush();  //ensure all data is written
        }

        //get the byte array containing the serialized Server.Message object
        byte[] messageBytes = byteArrayOutputStream.toByteArray();

        //wrap the byte array in a ByteBuffer
        ByteBuffer buffer = ByteBuffer.wrap(messageBytes);

        //write the ByteBuffer to the SocketChannel
        channel.write(buffer);
    }


    private void listenToServer() {
        ByteBuffer receiveBuffer = ByteBuffer.allocate(2048);
        Message message = null;
        int bytesRead = 0;
//        try {
////            channel.configureBlocking(true);
//        } catch (IOException e) {
//            throw new RuntimeException(e);
//        }


        while (true) {
            receiveBuffer.clear();
            try {
                bytesRead = channel.read(receiveBuffer);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
            if (bytesRead > 0) {
                receiveBuffer.flip();
                //convert the ByteBuffer into a byte array
                byte[] byteArray = new byte[receiveBuffer.remaining()];
                //copies data from buffer to byteArray
                receiveBuffer.get(byteArray);
                //create ByteArrayInputStream from the byte array
                ByteArrayInputStream byteArrayInputStream = new ByteArrayInputStream(byteArray);
                //deserialize the message into a Server.Message instance
                try (ObjectInputStream objectInputStream = new ObjectInputStream(byteArrayInputStream)) {
                    message = (Message) objectInputStream.readObject();
                    System.out.println("msg from server: " + message.getMessage());
                    System.out.println("type: " + message.getMessageType());
                } catch (IOException | ClassNotFoundException e) {
                    System.out.println("de-serialization failed");
                    e.printStackTrace();
                }
            }
        }
    }



    public static void main(String[] args) throws IOException, InterruptedException {
        ChatroomClient client = new ChatroomClient();
        client.start(9111);
    }
}
