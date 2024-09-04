package exercise2;

import java.util.concurrent.atomic.AtomicInteger;

public class AtomicCounter extends Thread {
    public static AtomicInteger count = new AtomicInteger(0);

    @Override
    public void run() {
        work();
    }

    private static void work() {
        for (int i = 0; i < 10000000; i++) {
            count.incrementAndGet();
        }
    }
}
