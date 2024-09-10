package waitingqueueconditionalvar;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class WaitablePQueue<E> {
    private Queue<E> queue;
    private Lock lock = new ReentrantLock();
    private Condition condition = lock.newCondition();
    private Timer timer;
    private int capacity;
    private static final int DEFAULT_CAPACITY = 16;

    //constructor with Comparator
    public WaitablePQueue(int capacity, Comparator<E> comparator) {
        queue = new PriorityQueue<E>(comparator);
        this.capacity = capacity;
    }

    //Constructor without Comparator, E should be Comparable
    public WaitablePQueue(int capacity) {
        queue = new PriorityQueue<E>();
        this.capacity = capacity;
    }

    public WaitablePQueue() {
       this(DEFAULT_CAPACITY);
    }

    public void enqueue(E e) {
        lock.lock();

        try {
            // wait until the is space in the queue
            while (queue.size() == capacity) {
                condition.await();
            }
            queue.add(e);
            condition.signal();
        } catch (InterruptedException ex) {
            throw new RuntimeException(ex);
        } finally {
            lock.unlock();
        }
    }

    public E dequeue() {
        E item;
        lock.lock();
        try {
            while (queue.isEmpty()) {
                condition.await();
            }
            item = queue.remove();
            condition.signal();
            return item;
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } finally {
            lock.unlock();
        }
    }

    //TODO
    public E dequeue(long timeout, TimeUnit unit) {
        return null;
    }


    /*
    private void resetTimeout() {
        if (timer != null) {
            timer.cancel();
        }

        timer = new Timer();

        // schedule a new task that will call resetMachine() after the timeout period
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                resetMachine(); // This will be called if the timeout period expires
            }
        }, TIMEOUT_SECONDS * 1000); // Schedule the task to run after TIMEOUT_SECONDS
    }

    private void cancelTimeout() {
        // Cancel the timer to prevent the reset task from running
        if (timer != null) {
            timer.cancel();
        }
    }

    private void resetMachine() {
        monitor.display("Timeout. refunding credit: " + credit);
        credit = 0;
        selectedProduct = null;
        state = VendingMachineState.WAITING_FOR_SELECTION;
    }
    */


    public boolean remove(Object o) {
        boolean isRemoved;

        lock.lock();
        isRemoved = queue.remove(o);
        condition.signal();
        lock.unlock();
        return isRemoved;
    }

    public E peek() {
        lock.lock();
        E e = queue.peek();
        lock.unlock();
        return e;
    }

    public int size() {
        lock.lock();
        int size = queue.size();
        lock.unlock();
        return size;
    }

    public boolean isEmpty() {
        lock.lock();
        boolean isEmpty = queue.isEmpty();
        lock.unlock();
        return isEmpty;
    }
}
