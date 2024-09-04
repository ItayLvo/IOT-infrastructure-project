package exercise2;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        UnsynchedCounter t1 = new UnsynchedCounter();
        UnsynchedCounter t2 = new UnsynchedCounter();

        long startTime = System.nanoTime();

        t1.start();
        t2.start();
        try {
            t1.join();
            t2.join();
        } catch (InterruptedException e) {}

        long totalTime = System.nanoTime() - startTime;
        System.out.println("counter = " +UnsynchedCounter.count);
        System.out.println("un-synchronized: " + totalTime + " nano seconds, " + (totalTime / 1000000) + " milliseconds" + "\n");

        /*************************************************/

        SynchronizedMethodCounter t3 = new SynchronizedMethodCounter();
        SynchronizedMethodCounter t4 = new SynchronizedMethodCounter();

        startTime = System.nanoTime();
        t3.start();
        t4.start();

        t3.join();
        t4.join();

        totalTime = System.nanoTime() - startTime;
        System.out.println("counter = " +SynchronizedMethodCounter.count);
        System.out.println("synchronized method: " + totalTime + " nano seconds, " + (totalTime / 1000000) + " milliseconds" + "\n");

        /*************************************************/

        AtomicCounter t5 = new AtomicCounter();
        AtomicCounter t6 = new AtomicCounter();

        startTime = System.nanoTime();
        t5.start();
        t6.start();

        t5.join();
        t6.join();

        totalTime = System.nanoTime() - startTime;
        System.out.println("counter = " +AtomicCounter.count);
        System.out.println("atomics method: " + totalTime + " nano seconds, " + (totalTime / 1000000) + " milliseconds" + "\n");

        /*************************************************/

        LockCounter t7 = new LockCounter();
        LockCounter t8 = new LockCounter();

        startTime = System.nanoTime();
        t7.start();
        t8.start();

        t7.join();
        t8.join();

        totalTime = System.nanoTime() - startTime;
        System.out.println("counter = " +LockCounter.count);
        System.out.println("lock method: " + totalTime + " nano seconds, " + (totalTime / 1000000) + " milliseconds" + "\n");

        /*************************************************/

        SyncBlockCounter t9 = new SyncBlockCounter();
        SyncBlockCounter t10 = new SyncBlockCounter();

        startTime = System.nanoTime();
        t9.start();
        t10.start();

        t9.join();
        t10.join();

        totalTime = System.nanoTime() - startTime;
        System.out.println("counter = " +SyncBlockCounter.count);
        System.out.println("synchronized block method: " + totalTime + " nano seconds, " + (totalTime / 1000000) + " milliseconds" + "\n");
    }
}
