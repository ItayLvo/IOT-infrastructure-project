package threadpool;

import waitingqueue.WaitablePQueue;

import java.util.*;
import java.util.concurrent.*;

public class ThreadPool implements Executor {
    private final WaitablePQueue<Task<?>> taskQueue = new WaitablePQueue<>();
    private volatile int currentNumberOfThreads;
    private boolean isShutDown = false;
    private final Object poolPauseLock = new Object();
    private volatile boolean isPaused = false;

    //TODO: dont check condition in thread run loop, just use sleeping pill. use tasks for pause, kill, resume, etc
    //TODO: fix sleeping pill priority to highest-1         DONE
    //TODO: not supposed to set num thead while paused      DONE
    //TODO: fix shutdown, cant send sleeping pill because it overrides all existing tasks. needs to be lowest priority, and use flag to stop queue enqueuing    DONE
    //TODO: await: you can use get() to block until last poison pill

    //set number of threads in the pool according to number of CPU cores
    public ThreadPool() {
        this(Runtime.getRuntime().availableProcessors());
    }

    public ThreadPool(int nThreads) {
        currentNumberOfThreads = nThreads;
        for (int i = 0; i < nThreads; ++i) {
            Worker worker = new Worker();
            worker.start();
        }
    }

    private final class Worker extends Thread {

        @Override
        public void run() {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    //try to dequeue a task from the queue
                    System.out.println(Thread.currentThread().getName() + " is waiting for tasks");
                    Task<?> task = taskQueue.dequeue();
                    //run the current task
                    System.out.println(Thread.currentThread().getName() + " has dequeued a task and calling execute() on " + task.toString());
                    task.executeTask();
                }
            } finally {
                //thread was killed: decrease number of current threads
                synchronized (poolPauseLock) {
                    System.out.println(Thread.currentThread().getName() + " is killed. updated number of threads = " + (currentNumberOfThreads - 1));
                    --currentNumberOfThreads;
                    //notify any threads waiting for awaitTermination
                    if (currentNumberOfThreads == 0) {
                        poolPauseLock.notifyAll();
                    }
                }
            }
        }
    }   //end of Worker class


    @Override
    public void execute(Runnable runnable) {

    }


    public Future<?> submit(Runnable command) {
        Callable<Object> callableWrapper = Executors.callable(command);
        return submit(callableWrapper, Priority.MEDIUM);

    }

    public Future<?> submit(Runnable command, Priority p) {
        Callable<Object> callableWrapper = Executors.callable(command);
        return submit(callableWrapper, p);
    }

    public <T> Future<T> submit(Runnable command, Priority p, T value) {
        Callable<T> callableWrapper = Executors.callable(command, value);
        return submit(callableWrapper, p);
    }

    public <T> Future<T> submit(Callable<T> command) {
        return submit(command, Priority.MEDIUM);
    }

    public <T> Future<T> submit(Callable<T> command, Priority p) {
        if (!isShutDown) {
            Task<T> task = new Task<>(command, p.getValue());
            taskQueue.enqueue(task);
            return task.getFuture();
        }

        //if shutdown() was called before submit
        throw new RejectedExecutionException("ThreadPool is shut down");
    }


    void setNumOfThreads(int nThreads) {
        if (nThreads > currentNumberOfThreads) {
            //increase number of threads in the pool
            for (int i = 0; i < (nThreads - currentNumberOfThreads); ++i) {
                Worker worker = new Worker();
                worker.start();
            }
            currentNumberOfThreads = nThreads;  //update current count of threads after creating the new workers
        } else {
            //decrease number of threads in the pool
            for (int i = 0; i < (currentNumberOfThreads - nThreads); ++i) {
                //create a "poison pill" callable and wrap it in a new Task with max priority, then enqueue it
                Task<Void> killThreadTask = createPoisonPillTask(100);
                taskQueue.enqueue(killThreadTask);
            }
        }
    }


    private Task<Void> createPoisonPillTask(Integer priority) {
        Callable<Void> killThreadCallable = new Callable() {
            @Override
            public Void call() {
                Thread.currentThread().interrupt();
                return null;
            }
        };
        Task<Void> killThreadTask = new Task<>(killThreadCallable, priority);
        return killThreadTask;
    }


    void pause() {
        //set flag to true to stop client from inserting new tasks
        isShutDown = true;

        //set isPaused flag that threads check in their sleeping pill task
        isPaused = true;

        for (int i = 0; i < currentNumberOfThreads; ++i) {
            System.out.println("sent " + (i+1) + "th sleeping pill");
            //the sleeping pill task will have the highest priority (outside the killing pill)
            Task<Void> pauseThreadTask = createSleepingPillTask(99);
            taskQueue.enqueue(pauseThreadTask);
        }
    }


    private Task<Void> createSleepingPillTask(Integer priority) {
        Callable<Void> pauseThreadCallable = new Callable() {
            @Override
            public Void call() {
                synchronized (poolPauseLock) {
                    while (isPaused) {
                        try {
                            System.out.println("waiting! " + Thread.currentThread().getName());
                            poolPauseLock.wait();
                            System.out.println("woke up! " + Thread.currentThread().getName());
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
                return null;
            }
        };

        Task<Void> sleepingPillTask = new Task<>(pauseThreadCallable, priority);
        return sleepingPillTask;
    }


    void resume() {
        isShutDown = false; //allow users to insert new tasks

        synchronized (poolPauseLock) {
            isPaused = false;
            poolPauseLock.notifyAll();
        }
    }


    void shutdown() {
        //set shutdown flag to true so client can't add any tasks to the task queue
        isShutDown = true;

        //create and enqueue a poison pill task for each thread
        for (int i = 0; i < (currentNumberOfThreads); ++i) {
            //create a "poison pill" callable and wrap it in a new Task with the lowest priority, then enqueue it
            Task<Void> killThreadTask = createPoisonPillTask(0);
            taskQueue.enqueue(killThreadTask);
        }
    }

    void awaitTermination() {
        try {
            awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }


    boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        //calculate timeout deadline and remaining time to wait
        long remainingTimeToWait = unit.toMillis(timeout);
        long deadline = System.currentTimeMillis() + remainingTimeToWait;

        synchronized (poolPauseLock) {
            while (currentNumberOfThreads > 0) {
                System.out.println("number of alive threads = " + currentNumberOfThreads);
                remainingTimeToWait = deadline - System.currentTimeMillis();
                if (remainingTimeToWait <= 0) {
                    return false;   //timed out
                }
                poolPauseLock.wait(remainingTimeToWait);
            }
        }

        return true;
    }


    public int getCurrentNumberOfThreads() {
        return currentNumberOfThreads;
    }


    private class Task<T> implements Comparable<Task<T>> {
        private final Callable<T> callable;
        private final Future<T> future;
        private final Semaphore isDoneSemaphore = new Semaphore(0);
        private T result = null;
        private boolean isDone = false;
        private boolean isCancelled = false;
        private final int priority;
        private Exception exception = null;


        public Task(Callable<T> callable, int priority) {
            this.callable = callable;
            this.priority = priority;
            this.future = new TaskFuture();
        }


        private void executeTask() {
            try {
                result = callable.call();   //execute the command
                isDone = true;  //set isDone flag, which is checked in Future.get() method
            } catch (Exception e) {
                this.exception = e; //if call() threw an exception, catch it and save it for Future.get(), then continue to next task
                if (e instanceof InterruptedException) {    //if thread was interrupted
                    Thread.currentThread().interrupt(); //preserve interrupted status for isInterrupted() check
                }
            } finally {
                isDoneSemaphore.release();  //release semaphore for Future.get() method
            }
        }


        @Override
        public int compareTo(Task<T> other) {
            return other.priority - this.priority;
        }


        private Future<T> getFuture() {
            return future;
        }


        private class TaskFuture implements Future<T> {

            @Override
            public boolean cancel(boolean b) {
                //check if task is already complete
                if (isDone) {
                    return false;
                }
                //try to remove task from queue
                boolean removed = ThreadPool.this.taskQueue.remove(Task.this);
                //set isCancelled to "true" only if it was actually removed
                if (removed) {
                    isCancelled = true;
                }

                return removed;
            }


            @Override
            public boolean isCancelled() {
                return isCancelled;
            }

            @Override
            public boolean isDone() {
                return isDone;
            }


            @Override
            public T get() throws InterruptedException, ExecutionException {
                //re-use the get() method which receives time limit arguments, allow endless time limit
                try {
                    return get(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
                } catch (TimeoutException e) { }
                return null;
            }


            @Override
            public T get(long l, TimeUnit timeUnit) throws InterruptedException, ExecutionException, TimeoutException {
                //if Task.executeTask() resulted in an exception
                if (Task.this.exception != null) {
                    throw new ExecutionException(exception);
                }

                //if the task was cancelled using Future.cancel()
                if (isCancelled) {
                    throw new CancellationException();
                }

                //if the task is done, return the result
                if (Task.this.isDone) {
                    return Task.this.result;
                } else {
                    //if the task is not done yet, wait for the semaphore token (executeTask() releases it when done)
                    if (isDoneSemaphore.tryAcquire(l, timeUnit) == true) {
                        return Task.this.result;
                    } else {
                        //if timed out
                        throw new TimeoutException();
                    }
                }
            }
        }   //end of TaskFuture class
    }
}
