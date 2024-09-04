package exercise3;


import java.util.concurrent.Semaphore;

public class MonitorTwoSemaphores extends MyMonitor{
    private final Semaphore semaphoreEmpty = new Semaphore(1);
    private final Semaphore semaphoreFull = new Semaphore(0);

    public void ping() throws InterruptedException {
        while (true) {
            semaphoreFull.acquire();
            System.out.println(Thread.currentThread().getName() + ": ping semaphoreFull");
            semaphoreEmpty.release();
        }
    }

    public void pong() throws InterruptedException {
        while (true) {
            semaphoreEmpty.acquire();
            System.out.println(Thread.currentThread().getName() + ": pong semaphoreEmpty");
            semaphoreFull.release();
        }
    }

}
