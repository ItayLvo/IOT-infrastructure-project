package exercise3;

public class ProducerPong extends Thread {
    MyMonitor monitorAtomicVar;

    public ProducerPong(MyMonitor monitorAtomicVar) {
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
