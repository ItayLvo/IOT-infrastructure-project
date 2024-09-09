package exercise3;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Semaphore;

public class MonitorListSemaphore implements MyMonitor {
    final List<Long> list;
    Semaphore sem;

    public MonitorListSemaphore() {
        list = new ArrayList<>();
        sem = new Semaphore(0);
    }

    public void add(long i) {
        synchronized (list) {
            list.add(i);
            System.out.println(list.size());
        }
        sem.release();
    }

    public void remove() throws InterruptedException {
        sem.acquire();
        synchronized (list) {
            list.remove(list.get(0));
            System.out.println(list.size());
        }
    }
}
