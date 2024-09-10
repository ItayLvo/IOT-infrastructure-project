package waitingqueue;

import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.Semaphore;
import java.util.concurrent.TimeUnit;

public class WaitablePQueue<E> {
    private Queue<E> queue;
    private Semaphore semaphore;
    private Timer timer;

    //constructor with Comparator
    public WaitablePQueue(Comparator<E> comparator) {
        queue = new PriorityQueue<E>(comparator);
        semaphore = new Semaphore(0);
    }

    //Constructor without Comparator, E should be Comparable
    public WaitablePQueue() {
        queue = new PriorityQueue<E>();
        semaphore = new Semaphore(0);
    }

    public void enqueue(E e) {
        synchronized (queue) {
            queue.add(e);
        }
        semaphore.release();
    }

    public E dequeue() {
        try {
            semaphore.acquire();
        } catch (InterruptedException ex) {
            throw new RuntimeException(ex);
        }

        synchronized (queue) {
            return queue.remove();
        }
    }


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

        synchronized (queue) {
            isRemoved = queue.remove(o);
        }

        if (isRemoved == true) {
            semaphore.release();
        }

        return isRemoved;
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
