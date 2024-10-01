package il.co.ilrd.chatroom_server;

import java.io.Serializable;

public class Message implements Serializable {
    private static final long serialVersionUID = 1L;

    private final MessageType messageType;
    private final String message;

    public Message(String message, MessageType messageType) {
        this.message = message;
        this.messageType = messageType;
    }

    public String getMessage() {
        return message;
    }

    public MessageType getMessageType() {
        return messageType;
    }
}

