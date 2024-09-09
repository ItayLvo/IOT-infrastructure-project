package exercise3;

import java.util.ArrayList;
import java.util.List;

public class MonitorList implements MyMonitor {
    List<Long> list;

    public MonitorList() {
        list = new ArrayList<>();
    }

    public synchronized void add(long i) {
        list.add(i);
        System.out.println(list.size());
        notifyAll();
    }

    public synchronized void remove() throws InterruptedException {
        while (list.isEmpty()) {
            wait();
        }
        list.remove(list.get(0));
        System.out.println(list.size());
    }
}
