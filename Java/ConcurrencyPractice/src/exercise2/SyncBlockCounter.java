package exercise2;

public class SyncBlockCounter extends Thread {
    public static int count = 0;

    @Override
    public void run() {
        work();
    }


    private static void work() {
        for (long i = 0; i < 10000000; i++) {
            synchronized (SyncBlockCounter.class) {
                ++count;
            }
        }

    }
}