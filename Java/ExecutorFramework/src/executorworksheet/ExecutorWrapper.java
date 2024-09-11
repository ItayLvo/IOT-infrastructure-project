package executorworksheet;

import java.util.concurrent.*;

public class ExecutorWrapper {
    public static void main(String[] args) {
        //fixed thread pool implementation
        ExecutorService executor1 = Executors.newFixedThreadPool(3);
        executor1.execute(new WorkerRunnable("first"));
        Future<Integer> future1 = executor1.submit(new WorkerCallable(2));
        executor1.execute(new WorkerRunnable("third"));
        Future<Integer> future2 = executor1.submit(new WorkerCallable(4));
        executor1.execute(new WorkerRunnable("fifth"));
        executor1.shutdown();
        try {
            executor1.awaitTermination(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        System.out.println("\n\n---------\n\n\n");

        //single thread pool implementation
        ExecutorService executor2 = Executors.newSingleThreadExecutor();
        executor2.execute(new WorkerRunnable("first"));
        Future<Integer> future3 = executor2.submit(new WorkerCallable(2));
        executor2.execute(new WorkerRunnable("third"));
        Future<Integer> future4 = executor2.submit(new WorkerCallable(4));
        executor2.execute(new WorkerRunnable("fifth"));

        System.out.println("finished adding threads");

        try {
            System.out.println("***get :" + future4.get());
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        } catch (ExecutionException e) {
            throw new RuntimeException(e);
        }


        executor2.shutdown();
        try {
            executor2.awaitTermination(5, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        System.out.println("\n\n---------\n\n\n");



        ExecutorService executor3 = Executors.newCachedThreadPool();
        executor3.execute(new WorkerRunnable("first"));
        Future<Integer> future5 = executor3.submit(new WorkerCallable(2));
        executor3.execute(new WorkerRunnable("third"));
        Future<Integer> future6 = executor3.submit(new WorkerCallable(4));
        executor3.execute(new WorkerRunnable("fifth"));
        System.out.println("finished adding threads");
        executor3.shutdown();
        System.out.println("\n\n---------\n\n\n");



        ScheduledThreadPoolExecutor executor4 = (ScheduledThreadPoolExecutor) Executors.newScheduledThreadPool(3);
        executor4.scheduleAtFixedRate(new WorkerRunnable("first"), 3, 3, TimeUnit.SECONDS);
    }


    public static class WorkerRunnable implements Runnable {
        private String name;

        public WorkerRunnable(String name) {
            this.name = name;
        }

        @Override
        public void run() {
            System.out.println("Runnable starting " + name);
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
                throw new RuntimeException(e);
            }
            System.out.println("Runnable finished " + name);
        }
    }


    public static class WorkerCallable implements Callable<Integer> {
        private Integer id;

        public WorkerCallable(Integer id) {
            this.id = id;
        }

        @Override
        public Integer call() throws Exception {
            System.out.println("Callable starting " + id);
            id *= 10;
            Thread.sleep(1000);
            System.out.println("Callable finished " + id);
            return id;
        }

    }
}
