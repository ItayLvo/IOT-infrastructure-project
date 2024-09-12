package threadpool;

import waitingqueue.WaitablePQueue;

import java.util.*;
import java.util.concurrent.*;

public class ThreadPool implements Executor {
    private final Set<Thread> threadPool = new HashSet<>();   //TODO final?
    private final WaitablePQueue<Task<?>> taskQueue = new WaitablePQueue<>();
    private final Map<Task<?>, Thread> taskMap = new HashMap<>();
    private int maxNumberOfThreads;
    private boolean isShutDown = false;


    //set number of threads in the pool according to number of CPU cores
    public ThreadPool() {
        this(Runtime.getRuntime().availableProcessors());
    }

    public ThreadPool(int nThreads) {
        maxNumberOfThreads = nThreads;
        for (int i = 0; i < nThreads; ++i) {
            Worker worker = new Worker();
            threadPool.add(worker); //TODO is this a memory leak? do i need to keep track of the workers and have a reference to them?
            worker.start();
        }
    }

    private final class Worker extends Thread {     //TODO do i need this wrapper class or are the thread methods enough?
        private volatile boolean killThread = false;

        @Override
        public void run() {
            while (!killThread) {
                //try to dequeue a task from the queue
                Task<?> task = taskQueue.dequeue();
                //run the current task
                task.executeTask();
            }
        }

        public void stopWorker() {
            killThread = true;
        }

        public void startWorker() {
            killThread = false;
            this.start();
        }

    }   //end of Worker class


    @Override
    public void execute(Runnable runnable) {

    }


    public Future<?> submit(Runnable command) {
        Callable<Object> callableWrapper = Executors.callable(command);
        return submit(callableWrapper, Priority.MEDIUM);
//        Task<Object> task = new Task<>(callableWrapper, Priority.MEDIUM.ordinal());
//        taskQueue.enqueue(task);
//        return task.getFuture();
    }

    public Future<?> submit(Runnable command, Priority p) { //TODO check what happens when you don't update result and leave it null!
        Callable<Object> callableWrapper = Executors.callable(command);
        return submit(callableWrapper, p);
//        Task<Object> task = new Task<>(callableWrapper, p.ordinal());
//        taskQueue.enqueue(task);
//        return task.getFuture();
    }

    public <T> Future<T> submit(Runnable command, Priority p, T value) {
        Callable<T> callableWrapper = Executors.callable(command, value);
        return submit(callableWrapper, p);
//        Task<T> task = new Task<>(callableWrapper, p.ordinal());
//        taskQueue.enqueue(task);
//        return task.getFuture();
    }

    public <T> Future<T> submit(Callable<T> command) {
        return submit(command, Priority.MEDIUM);
//        Task<T> task = new Task<>(command, Priority.MEDIUM.ordinal());
//        taskQueue.enqueue(task);
//        return task.getFuture();
    }

    public <T> Future<T> submit(Callable<T> command, Priority p) {
        if (!isShutDown) {
            Task<T> task = new Task<>(command, p.ordinal());
            taskQueue.enqueue(task);
            return task.getFuture();
        }
        //if shutdown() was called before submit
        throw new RejectedExecutionException("ThreadPool is shut down");
    }


    void setNumOfThreads(int nThreads) {
        if (nThreads > maxNumberOfThreads) {
            //increase number of threads in the pool
            for (int i = 0; i < nThreads - maxNumberOfThreads; ++i) {
                Worker worker = new Worker();
                worker.start();
            }
        } else {
            //decrease number of threads in the pool
            for (int i = 0; i < maxNumberOfThreads - nThreads; ++i) {
                Callable<Void> killThreadRunnable = new Callable() {
                    @Override
                    public Void call() {
                        Thread.currentThread().interrupt();
                        return null;
                    }
                };

                Task<Void> killThreadTask = new Task<>(killThreadRunnable, Integer.MAX_VALUE);
                taskQueue.enqueue(killThreadTask);
            }
        }
        maxNumberOfThreads = nThreads;
    }

    void pause() {

    }

    void resume() {

    }

    void shutdown() {
        setNumOfThreads(0); //all threads will terminate after they are done with their current task
        isShutDown = true;
    }

    void awaitTermination() {

    }

    boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {

    }


    public int getMaxNumberOfThreads() {
        return maxNumberOfThreads;
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
                isDoneSemaphore.release();  //release semaphore for Future.get() method
            } catch (Exception e) {
                this.exception = e; //if call() threw an exception, catch it and save it for Future.get(), then continue to next task
            }
        }


        @Override
        public int compareTo(Task<T> other) {
            return this.priority - other.priority;
        }


        private Future<T> getFuture() {
            return future;
        }


        //FutureIMP<T> is an inner class of Task<T>. each Future is tied to an instance of Task
        private class TaskFuture implements Future<T> {

            @Override
            public boolean cancel(boolean b) {
                boolean removed = ThreadPool.this.taskQueue.remove(Task.this);
                if (removed && !isDone) {
                    isCancelled = true;
                }

                return removed;
            }
            //TODO advanced cancel with true/false argument


            @Override
            public boolean isCancelled() {  //TODO
                return isCancelled;
            }

            @Override
            public boolean isDone() {
                return isDone;
            }

            //TODO check if cancelled flag is on, so you don't get stuck in the semaphore
            @Override
            public T get() throws InterruptedException, ExecutionException {
                if (Task.this.exception != null) {
                    throw new ExecutionException(exception);
                }

                if (Task.this.isDone) {
                    return Task.this.result;
                } else {
                    isDoneSemaphore.acquire();
                    return Task.this.result;
                }
            }

            @Override
            public T get(long l, TimeUnit timeUnit) throws InterruptedException, ExecutionException, TimeoutException {
                if (Task.this.exception != null) {
                    throw new ExecutionException(exception);
                }

                if (Task.this.isDone) {
                    return Task.this.result;
                } else {
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
