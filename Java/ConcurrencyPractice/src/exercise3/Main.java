package exercise3;

public class Main {
    public static void main(String[] args) {
        MyMonitor mAtomicVar = new MonitorAtomicVar();
        MyMonitor mSemaphores = new MonitorTwoSemaphores();

        Producer p1 = new Producer(mSemaphores);
        Consumer c1 = new Consumer(mSemaphores);


        p1.start();
        c1.start();

    }
}
