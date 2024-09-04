package exercise3;

public class Producer extends Thread {
    MyMonitor monitorAtomicVar;

    public Producer(MyMonitor monitorAtomicVar) {
        this.monitorAtomicVar = monitorAtomicVar;
    }

    @Override
    public void run() {
        while (true) {
            try {
                monitorAtomicVar.pong();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }
    }
}
