package exercise3;

public class Consumer extends Thread {
    MyMonitor monitor;

    public Consumer(MyMonitor m) {
        this.monitor = m;
    }

    public void run() {
        consume();
    }

    private void consume() {
        while (true) {
            try {
                monitor.remove();
            } catch (InterruptedException e) {
                System.out.println("Consumer interrupted");
            }
        }
    }
}
