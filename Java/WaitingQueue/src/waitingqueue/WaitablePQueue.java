package waitingqueue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class WaitablePQueue<E> {
    private Queue<E> queue;
    private Semaphore semaphore = new Semaphore(0);
    private Lock lock = new ReentrantLock();

    //constructor with Comparator
    public WaitablePQueue(Comparator<E> comparator) {
        queue = new PriorityQueue<E>(comparator);
    }

    //Constructor without Comparator, E should be Comparable
    public WaitablePQueue() {
        queue = new PriorityQueue<E>();
    }

    public void enqueue(E e) {
        lock.lock();
        try {
            queue.add(e);
            semaphore.release();
        } finally {
            lock.unlock();
        }
    }

    public E dequeue() {
        try {
            semaphore.acquire();
        } catch (InterruptedException ex) {
            throw new RuntimeException(ex);
        }

        lock.lock();
        try {
            return queue.remove();
        } finally {
            lock.unlock();
        }
    }


    public E dequeue(long timeout, TimeUnit unit) {
        E item = null;

        //calculate timeout deadline and remaining time to wait
        long remainingTimeToWait = unit.toNanos(timeout);
        long deadline = System.nanoTime() + remainingTimeToWait;

        //try to acquire semaphore before timeout
        try {
            if (!semaphore.tryAcquire(remainingTimeToWait, TimeUnit.NANOSECONDS)) {
                return null;
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        }

        //update remaining time to wait after waiting for semaphore
        remainingTimeToWait = deadline - System.nanoTime();

        boolean acquiredLock = false;
        try {
            if (!lock.tryLock(remainingTimeToWait, TimeUnit.NANOSECONDS)) {
                return null;    //if timed out while waiting for lock - return null
            }
            acquiredLock = true;
            return queue.remove();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            return null;
        } finally {
            if (acquiredLock) {
                lock.unlock();
            } else {
                //if timed out while waiting for lock - release semaphore
                semaphore.release();
            }
        }
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
        lock.lock();
        try {
            boolean isRemoved = queue.remove(o);
            if (isRemoved) {
                semaphore.release();
            }
            return isRemoved;
        } finally {
            lock.unlock();
        }
    }

    public E peek() {
        synchronized (queue) {
            return queue.peek();
        }
    }


    public int size() {
        synchronized (queue) {
            return queue.size();
        }
    }


    public boolean isEmpty() {
        synchronized (queue) {
            return queue.isEmpty();
        }
    }
}
