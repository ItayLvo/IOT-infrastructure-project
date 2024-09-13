package threadpool;

import waitingqueue.WaitablePQueue;

import java.util.*;
import java.util.concurrent.*;

public class ThreadPool implements Executor {
    private final Set<Thread> threadPool = new HashSet<>();   //TODO final?
    private final WaitablePQueue<Task<?>> taskQueue = new WaitablePQueue<>();
    private volatile int currentNumberOfThreads;
    private boolean isShutDown = false;
    private final Object poolPauseLock = new Object();
    private volatile boolean isPaused = false;


    //set number of threads in the pool according to number of CPU cores
    public ThreadPool() {
        this(Runtime.getRuntime().availableProcessors());
    }

    public ThreadPool(int nThreads) {
        currentNumberOfThreads = nThreads;
        for (int i = 0; i < nThreads; ++i) {
            Worker worker = new Worker();
            threadPool.add(worker); //TODO is this a memory leak? do i need to keep track of the workers and have a reference to them?
            worker.start();
        }
    }

    private final class Worker extends Thread {     //TODO do i need this wrapper class or are the Thread methods enough?
//      private volatile boolean killThread = false;
//      private volatile boolean isPaused = false;
//      private final Object workerPauseLock = new Object();

        @Override
        public void run() {
            try {
                while (!Thread.currentThread().isInterrupted()) {
                    //try to dequeue a task from the queue
                    Task<?> task = taskQueue.dequeue();
                    //run the current task
                    task.executeTask();
                }
            } finally {
                //thread was killed: decrease number of current threads
                synchronized (poolPauseLock) {
                    --currentNumberOfThreads;
                    //notify any threads waiting for awaitTermination
                    if (currentNumberOfThreads == 0) {
                        poolPauseLock.notifyAll();
                    }
                }
            }
        }
    }   //end of Worker class

    //TODO remove this and fields when done
//old run() after adding kill/pause/resume features:
//       @Override
//       public void run() {
//           while (!killThread) {
//               synchronized (workerPauseLock) {
//                   if (killThread) {   //may have changed while waiting to synchronize on pauseLock
//                       break;
//                   }
//                   if (isPaused) {
//                       try {
//                           workerPauseLock.wait(); //block until another thread calls pauseLock.notifyAll()
//                       } catch (InterruptedException ex) {
//                           break;
//                       }
//                       if (killThread) {   //killThread status might have changed while we waited
//                           break;
//                       }
//                   }
//               }
//               //try to dequeue a task from the queue
//               Task<?> task = taskQueue.dequeue();
//               //run the current task
//               task.executeTask();
//           }   //end of while loop
//           --currentNumberOfThreads;
//       }
//
//
//       public void killWorker() {
//           killThread = true;
//           resumeWorker(); //to unblock
//       }
//
//       public void pauseWorker() {
//           isPaused = true;
//       }
//
//       public void resumeWorker() {
//           synchronized (workerPauseLock) {
//               isPaused = false;
//               workerPauseLock.notifyAll();    //unblock waiting threads
//           }
//       }




    @Override
    public void execute(Runnable runnable) {

    }


    public Future<?> submit(Runnable command) {
        Callable<Object> callableWrapper = Executors.callable(command);
        return submit(callableWrapper, Priority.MEDIUM);

    }

    public Future<?> submit(Runnable command, Priority p) { //TODO check what happens when you don't update result and leave it null!
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
            Task<T> task = new Task<>(command, p.ordinal());
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
        } else {
            //decrease number of threads in the pool
            for (int i = 0; i < (currentNumberOfThreads - nThreads); ++i) {
                //create a "poison pill" callable and wrap it in a new Task with max priority, then enqueue it
                Task<Void> killThreadTask = createPoisonPillTask();
                taskQueue.enqueue(killThreadTask);
            }
        }
        currentNumberOfThreads = nThreads;
    }

    private Task<Void> createPoisonPillTask() {
        Callable<Void> killThreadCallable = new Callable() {
            @Override
            public Void call() {
                Thread.currentThread().interrupt();
                return null;
            }
        };
        Task<Void> killThreadTask = new Task<>(killThreadCallable, Integer.MIN_VALUE);
        return killThreadTask;
    }


    void pause() {
        for (int i = 0; i < currentNumberOfThreads; ++i) {
            Task<Void> pauseThreadTask = createSleepingPillTask();
            taskQueue.enqueue(pauseThreadTask);
        }
    }


    private Task<Void> createSleepingPillTask() {
        Callable<Void> pauseThreadCallable = new Callable() {
            @Override
            public Void call() {
                synchronized (poolPauseLock) {
                    while (isPaused) {
                        try {
                            poolPauseLock.wait();
                        } catch (InterruptedException e) {
                            throw new RuntimeException(e);
                        }
                    }
                }
                return null;
            }
        };
        //the sleeping pill task will have the lowest possible priority, so it's pushed to the end of the queue
        Task<Void> sleepingPillTask = new Task<>(pauseThreadCallable, Integer.MAX_VALUE);
        return sleepingPillTask;
    }


    void resume() {
        synchronized (poolPauseLock) {
            isPaused = false;
            poolPauseLock.notifyAll();
        }
    }


    void shutdown() {
        setNumOfThreads(0); //all threads will terminate after they are done with their current task
        isShutDown = true;
    }

    void awaitTermination() {
        try {
            awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }


    boolean awaitTermination(long timeout, TimeUnit unit) throws InterruptedException {
        shutdown();

        //calculate timeout deadline and remaining time to wait
        long remainingTimeToWait = unit.toMillis(timeout);
        long deadline = System.currentTimeMillis() + remainingTimeToWait;

        synchronized (poolPauseLock) {
            while (currentNumberOfThreads > 0) {
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
            return this.priority - other.priority;
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
