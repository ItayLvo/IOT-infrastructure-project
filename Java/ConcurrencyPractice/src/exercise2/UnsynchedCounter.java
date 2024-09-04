package exercise2;

public class UnsynchedCounter extends Thread {
    public static volatile int count = 0;

    @Override
    public void run() {
        work();
    }

    private static void work() {
        for (long i = 0; i < 10000000; i++) {
            ++count;
        }
    }
}
