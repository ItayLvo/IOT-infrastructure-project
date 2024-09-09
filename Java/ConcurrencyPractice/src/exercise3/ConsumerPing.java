package exercise3;

public class ConsumerPing extends Thread {
    MyMonitor monitorAtomicVar;

    public ConsumerPing(MyMonitor monitorAtomicVar) {
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
