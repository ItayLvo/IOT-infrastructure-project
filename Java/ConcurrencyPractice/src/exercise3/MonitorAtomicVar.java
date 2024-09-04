package exercise3;

import java.util.concurrent.atomic.AtomicInteger;

public class MonitorAtomicVar extends MyMonitor {
//    private final Semaphore semaphore = new Semaphore(1);
    private AtomicInteger count = new AtomicInteger(0);

    public void ping() {
        while (true) {
            while (count.get() > 0) { //busywait
            }
            count.incrementAndGet();
            System.out.println(Thread.currentThread().getName() + ": Ping " + count.get());
        }
    }

    public void pong() {
        while (true) {
            while (count.get() == 0) {  //busywait

            }
            count.decrementAndGet();
            System.out.println(Thread.currentThread().getName() + ": Pong " + count.get());
        }
    }

}
