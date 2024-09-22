package factory;

public interface Command extends Runnable {

    String getData();

    @Override
    void run();
}
