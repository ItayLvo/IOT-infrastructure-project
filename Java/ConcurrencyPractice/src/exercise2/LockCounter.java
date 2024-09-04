package exercise2;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class LockCounter extends Thread {
    public static int count = 0;
    static Lock lock = new ReentrantLock();

    @Override
    public void run() {
        work();
    }

    private static void work() {
        for (int i = 0; i < 10000000; i++) {
            lock.lock();
            ++count;
            lock.unlock();
        }
    }
}
