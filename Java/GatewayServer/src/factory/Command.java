package factory;

public abstract class Command implements Runnable {
    String data;

    public Command(String data) {
        this.data = data;
    }

    public String getData() {
        return data;
    }

    @Override
    public void run() {
        System.out.println("Command is: " + Class.class.getName() + ", data: " + data);
    }
}
