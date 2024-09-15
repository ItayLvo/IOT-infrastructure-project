package waitingqueueconditionalvar;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Timer;
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

    public E dequeue(long timeout, TimeUnit unit) {
        long remainingTimeToWait = unit.toNanos(timeout);
        long deadline = System.nanoTime() + remainingTimeToWait;

        try {
            if (!lock.tryLock(remainingTimeToWait, TimeUnit.NANOSECONDS)) {
                return null;
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        }

        //update remaining time to wait after waiting for lock
        remainingTimeToWait = deadline - System.nanoTime();
        try {
            while (queue.isEmpty()) {
                if (remainingTimeToWait <= 0) {
                    return null; // timeout while waiting for an item
                }
                remainingTimeToWait = condition.awaitNanos(remainingTimeToWait);
            }
            return queue.remove();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        } finally {
            lock.unlock();
        }
    }


    public boolean remove(Object o) {
        lock.lock();
        try {
            boolean isRemoved = queue.remove(o);
            if (isRemoved) {
                condition.signal();
            }
            return isRemoved;
        } finally {
            lock.unlock();
        }
    }

    public E peek() {
        lock.lock();
        try {
            return queue.peek();
        } finally {
            lock.unlock();
        }
    }

    public int size() {
        lock.lock();
        try {
            return queue.size();
        } finally {
            lock.unlock();
        }

    }

    public boolean isEmpty() {
        lock.lock();
        try {
            return queue.isEmpty();
        } finally {
            lock.unlock();
        }
    }

}
