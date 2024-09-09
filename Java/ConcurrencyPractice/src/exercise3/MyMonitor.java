package exercise3;

public interface MyMonitor {

    default public void ping() throws InterruptedException {
    }

    default public void pong() throws InterruptedException {
    }

    default public void remove() throws InterruptedException {
    }

    default public void add(long i) throws InterruptedException {
    }

}
