package threadpool;

import org.junit.jupiter.api.Test;
import waitingqueue.WaitablePQueue;

import java.util.Comparator;
import java.util.concurrent.Callable;
import java.util.concurrent.TimeUnit;
import static org.junit.jupiter.api.Assertions.*;

class ThreadPoolMyOwnTests {
    @Test
    void testSubmitBasic() {
        ThreadPool pool = new ThreadPool(2);
        assertEquals(2, pool.getCurrentNumberOfThreads());
        Callable<Long> callable1 = getCallable();
        Callable<Long> callable2 = getCallable();
        Callable<Long> callable3 = getCallable();

        pool.submit(callable1);
        pool.submit(callable2);
        pool.submit(callable3);

        pool.shutdown();
        pool.awaitTermination();
        System.out.println("end of main");
        assertEquals(0, pool.getCurrentNumberOfThreads());
    }

    @Test
    void testAwaitTimeout() {
        ThreadPool pool = new ThreadPool(3);
        Callable<Long> callable1 = getCallable();
        Callable<Long> callable2 = getCallable();
        Callable<Long> callable3 = getCallable();
        Callable<Long> callable4 = getCallable();
        Callable<Long> callable5 = getCallable();
        Callable<Long> callable6 = getCallable();

        pool.submit(callable1);
        pool.submit(callable2);
        pool.submit(callable3);
        pool.submit(callable4);
        pool.submit(callable5);
        pool.submit(callable6);

        pool.shutdown();
        boolean waitResult;
        try {
            waitResult = pool.awaitTermination(2, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        assertFalse(waitResult);
    }


    @Test
    void testAwaitTimeoutSuccessful() {
        ThreadPool pool = new ThreadPool(3);
        Callable<Long> callable1 = getCallable();
        Callable<Long> callable2 = getCallable();
        Callable<Long> callable3 = getCallable();
        Callable<Long> callable4 = getCallable();
        Callable<Long> callable5 = getCallable();
        Callable<Long> callable6 = getCallable();

        pool.submit(callable1);
        pool.submit(callable2);
        pool.submit(callable3);
        pool.submit(callable4);
        pool.submit(callable5);
        pool.submit(callable6);

        pool.shutdown();
        boolean waitResult;
        try {
            waitResult = pool.awaitTermination(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        assertTrue(waitResult);
    }


    @Test
    void testPause() {
        ThreadPool pool = new ThreadPool(2);
        Callable<Long> callable1 = getCallableLongSleep();
        Callable<Long> callable2 = getCallableLongSleep();
        Callable<Long> callable3 = getCallableLongSleep();
        Callable<Long> callable4 = getCallableLongSleep();
        Callable<Long> callable5 = getCallableLongSleep();
        Callable<Long> callable6 = getCallableLongSleep();
        Callable<Long> callable7 = getCallableLongSleep();
        Callable<Long> callable8 = getCallableLongSleep();
        Callable<Long> callable9 = getCallableLongSleep();

        pool.submit(callable1);
        pool.submit(callable2);
        pool.submit(callable3);
        pool.submit(callable4);
        pool.submit(callable5);
        pool.submit(callable6);
        pool.submit(callable7);
        pool.submit(callable8);
        pool.submit(callable9);

        //sleep 1 second to wait for threads to start working on 1st task of each thread
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }


        pool.pause();


        boolean waitResult;
        try {
            waitResult = pool.awaitTermination(30, TimeUnit.SECONDS);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        assertFalse(waitResult);
    }

    @Test
    void testResume() {
        ThreadPool pool = new ThreadPool();
        for (int i = 0; i < 20; ++i) {
            Callable<Long> callable1 = getCallableLongSleep();
            pool.submit(callable1);
        }


        //sleep 1 second to wait for threads to start working on 1st task of each thread
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }


        pool.pause();

        try {
            Thread.sleep(20000);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }

        pool.resume();


        pool.shutdown();
        pool.awaitTermination();

        assertEquals(0, pool.getCurrentNumberOfThreads());
    }


    @Test
    void testPriority() {
        ThreadPool pool = new ThreadPool(2);

        Callable<Long> callableHIGH = getCallableHigh();
        Callable<Long> callableMID = getCallableMid();
        Callable<Long> callableLOW = getCallableLow();
        Callable<Long> callableHIGH2 = getCallableHigh();
        Callable<Long> callableMID2 = getCallableMid();
        Callable<Long> callableLOW2 = getCallableLow();


        pool.submit(callableLOW, Priority.LOW);
        pool.submit(callableLOW2, Priority.LOW);
        try {
            Thread.sleep(800);
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
        pool.submit(callableMID2, Priority.MEDIUM);
        pool.submit(callableMID, Priority.MEDIUM);
        pool.submit(callableHIGH, Priority.HIGH);
        pool.submit(callableHIGH2, Priority.HIGH);

        pool.shutdown();
        pool.awaitTermination();

    }

    private Callable<Long> getCallableHigh() {
        return new Callable<Long>() {
            @Override
            public Long call() throws Exception {
                System.out.println("Executing HIGH. " + Thread.currentThread().getName());
                Thread.sleep(2000);
                System.out.println("Finished HIGH. " + Thread.currentThread().getName());
                return Thread.currentThread().getId();
            }
        };
    }

    private Callable<Long> getCallableMid() {
        return new Callable<Long>() {
            @Override
            public Long call() throws Exception {
                System.out.println("Executing MID. " + Thread.currentThread().getName());
                Thread.sleep(2000);
                System.out.println("Finished MID. " + Thread.currentThread().getName());
                return Thread.currentThread().getId();
            }
        };
    }

    private Callable<Long> getCallableLow() {
        return new Callable<Long>() {
            @Override
            public Long call() throws Exception {
                System.out.println("Executing LOW. " + Thread.currentThread().getName());
                Thread.sleep(2000);
                System.out.println("Finished LOW. " + Thread.currentThread().getName());
                return Thread.currentThread().getId();
            }
        };
    }


    private Callable<Long> getCallable() {
        return new Callable<Long>() {
            @Override
            public Long call() throws Exception {
                System.out.println("Executing Callable. " + Thread.currentThread().getName());
                Thread.sleep(2000);
                System.out.println("Finished Callable. " + Thread.currentThread().getName());
                return Thread.currentThread().getId();
            }
        };
    }

    private Callable<Long> getCallableLongSleep() {
        return new Callable<Long>() {
            @Override
            public Long call() throws Exception {
                System.out.println("Executing Callable. " +Thread.currentThread().getName());
                Thread.sleep(5000);
                System.out.println("Finished Callable. " + Thread.currentThread().getName());
                return Thread.currentThread().getId();
            }
        };
    }
}