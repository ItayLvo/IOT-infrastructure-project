package exercise3;

import java.util.concurrent.Semaphore;

public class MonitorBarrier implements MyMonitor {
    private volatile String message;
    Semaphore sem;
    private volatile int countConsumed = 0;
    private final int COUNT_CONSUMERS = 4;
    private final Object lock;

    public MonitorBarrier() {
        sem = new Semaphore(0);
        lock = new Object();
    }

    @Override
    public void add(long tmp) throws InterruptedException {
        synchronized (lock) {
            while (countConsumed < COUNT_CONSUMERS) {
                lock.wait();
            }

            message = "random message " + Math.random();
            System.out.println(Thread.currentThread().getName() + ": " + message);
            countConsumed = 0;
            sem.release(COUNT_CONSUMERS);
//            lock.notifyAll();
        }

    }

    @Override
    public void remove() throws InterruptedException {
        sem.acquire();

        synchronized (lock) {
            ++countConsumed;
            System.out.println(Thread.currentThread().getName() + ": " + "consumed " + message);

            if (countConsumed == COUNT_CONSUMERS) {
                lock.notifyAll();
            }
        }
    }
}
