package exercise3;

public class Consumer extends Thread {
    MyMonitor monitorAtomicVar;

    public Consumer(MyMonitor monitorAtomicVar) {
        this.monitorAtomicVar = monitorAtomicVar;
    }

    @Override
    public void run() {
        while (true) {
            try {
                monitorAtomicVar.ping();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
