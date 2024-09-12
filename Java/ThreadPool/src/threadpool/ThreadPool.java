package threadpool;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.Executor;
import java.util.concurrent.Future;

public class ThreadPool implements Executor {
    List<Thread> threadPool;


    public ThreadPool(int nThreads) {
        threadPool = new ArrayList<>(nThreads);
        for (Thread t : threadPool) {
            t = new Thread();
        }
    }

    Future submit(Runnable r, Priority p) {

    }

    Future submit(Runnable command, Priority p, T value) {

    }

    Future submit(Callable c) {

    }

    Future submit(Callable c, Priority p) {

    }

    void setNumOfThreads(int nThreads) {

    }

    void pause() {

    }

    void resume() {

    }

    void shutdown () {

    }

    void awaitTermination() {

    }

    @Override
    public void execute(Runnable runnable) {

    }
}
