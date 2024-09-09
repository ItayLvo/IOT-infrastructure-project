package exercise3;

import java.util.concurrent.atomic.AtomicInteger;

public class MonitorAtomicVar implements MyMonitor {
    private AtomicInteger count = new AtomicInteger(0);

    public void ping() throws InterruptedException {
        while (true) {
            while (count.get() != 0) { //busywait
            }
            System.out.println(Thread.currentThread().getName() + ": Ping " + count.get());
            count.incrementAndGet();
        }
    }

    public void pong() throws InterruptedException {
        while (true) {
            while (count.get() == 0) {  //busywait
            }
            System.out.println(Thread.currentThread().getName() + ": Pong " + count.get());
            count.decrementAndGet();
        }
    }

}
