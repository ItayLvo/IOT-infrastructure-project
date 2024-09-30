import java.net.SocketAddress;

public class User {
    private final SocketAddress address;
    private final String username;

    public User(SocketAddress address, String username) {
        this.username = username;
        this.address = address;
    }

    public SocketAddress getAddress() {
        return address;
    }

    public String getUsername() {
        return username;
    }
}
