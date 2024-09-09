package exercise3;

public class Producer extends Thread {
    MyMonitor monitor;

    public Producer(MyMonitor m) {
        this.monitor = m;
    }

    public void run() {
        try {
            produce();
        } catch (InterruptedException e) { }
    }

    private void produce() throws InterruptedException {
        long i = 0;
        while (true) {
            monitor.add(i);
            ++i;
        }
    }
}