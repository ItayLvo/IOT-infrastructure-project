package server;

import il.co.ilrd.chatroom_server.*;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectOutputStream;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;
import java.util.Map;


public class MessageHandler {

    public void handleMessage(Message message, SocketChannel client, ChatroomManager chatroomManager) throws IOException {
        MessageType type = message.getMessageType();
        String username;

        switch (type) {
            case REGISTER:
                System.out.println("REGISTER");
                username = message.getMessage();
                if (chatroomManager.isUsernameAvailable(username)) {
                    //register the user to the room (saves in collection)
                    chatroomManager.userRegistration(client, message.getMessage());
                    //send reg approved
                    Message regApprovedMsg = new Message(username, MessageType.REGISTER_APPROVED);
                    sendMessageToClient(regApprovedMsg, client);
                    //send a "joined" message to all registered users
                    notifyUserJoinedOrLeft(username, chatroomManager, "joined");
                }
                else {  //if the username is already taken
                    Message registerDeclined = new Message(message.getMessage(), MessageType.REGISTER_DECLINED);
                    sendMessageToClient(registerDeclined, client);
                }
                break;
            case UNREGISTER:
                System.out.println("UNREGISTER");
                username = message.getMessage();
                //check if username is registered
                if (!chatroomManager.isUsernameAvailable(username)) {
                    //un-register the user to the room (removes from collection)
                    chatroomManager.removeUser(client);
                    //send a "left" message to all registered users
                    notifyUserJoinedOrLeft(username, chatroomManager, "left");
                }
                else {  //if the username is not registered
                    Message unregisterDeclined = new Message(message.getMessage(), MessageType.UNREGISTER_DECLINED);
                    sendMessageToClient(unregisterDeclined, client);
                }
                break;
            case SEND_MESSAGE:
                System.out.println("SEND_MESSAGE");
                String messageToSend = message.getMessage();
                //check if username is registered
                if (chatroomManager.isClientRegistered(client)) {
                    //send the message to all registered users
                    String sendingUsername = chatroomManager.getUsername(client);
                    Message broadcastMessage = new Message("[" + sendingUsername + "] " + message.getMessage(), MessageType.BROADCAST_MESSAGE);
                    sendMessageToAllUsers(broadcastMessage, chatroomManager);
                }
                else {  //if the client is not registered
                    Message messageDeclined = new Message(message.getMessage(), MessageType.MESSAGE_DECLINED);
                    sendMessageToClient(messageDeclined, client);
                }
                break;
            case REGISTER_APPROVED:
                System.out.println(message.getMessage() + ": REGISTER_APPROVED");
                break;
            case REGISTER_DECLINED:
                System.out.println(message.getMessage() + ": REGISTER_DECLINED");
                break;
            case UNREGISTER_APPROVED:
                System.out.println(message.getMessage() + ": UNREGISTER_APPROVED");
                break;
            case UNREGISTER_DECLINED:
                System.out.println(message.getMessage() + ": UNREGISTER_DECLINED");
                break;
            case MESSAGE_DECLINED:
                System.out.println(message.getMessage() + ": MESSAGE_DECLINED");
                break;
            case BROADCAST_MESSAGE:
                System.out.println(message.getMessage() + ": BROADCAST_MESSAGE");

                break;
        }
    }

    private void sendMessageToAllUsers(Message broadcastMessage, ChatroomManager chatroomManager) throws IOException {
        Map<SocketChannel, String> users = chatroomManager.getUsers();
        for (SocketChannel user : users.keySet()) {
            sendMessageToClient(broadcastMessage, user);
        }
    }


    private void sendMessageToClient(Message message, SocketChannel clientChannel) throws IOException {
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
            clientChannel.write(buffer);
        }
    }


    private void notifyUserJoinedOrLeft(String username, ChatroomManager chatroomManager, String action) throws IOException {
        Message newUserJoinedMessage = new Message("[Server] " + username + " " + action + " the chatroom", MessageType.BROADCAST_MESSAGE);
        Map<SocketChannel, String> users = chatroomManager.getUsers();
        for (SocketChannel user : users.keySet()) {
            sendMessageToClient(newUserJoinedMessage, user);
        }
    }
}
