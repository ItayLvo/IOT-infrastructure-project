package server;

import java.nio.channels.SocketChannel;
import java.util.HashMap;
import java.util.Map;

public class ChatroomManager {
    private final Map<SocketChannel, String> users = new HashMap<>();


    public Map<SocketChannel, String> getUsers() {
        return users;
    }

    public void userRegistration(SocketChannel socketChannel, String username) {
        users.put(socketChannel, username);
    }

    public void removeUser(SocketChannel socketChannel) {
        users.remove(socketChannel);
    }

    public boolean isUsernameAvailable(String userName) {
        return !(users.containsValue(userName));
    }

    public boolean isClientRegistered(SocketChannel client) {
        return users.containsKey(client);
    }

    public String getUsername(SocketChannel client) {
        return users.get(client);
    }

}



