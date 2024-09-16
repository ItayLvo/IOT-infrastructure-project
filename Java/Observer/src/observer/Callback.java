package observer;

import java.util.function.Consumer;

public class Callback<T> {
    private Dispatcher<T> dispatcher;
    private final Consumer<T> consumer;
    private final Runnable stopUpdateRunnable;

    public Callback(Consumer<T> consumer, Runnable stopUpdateRunnable) {
        this.consumer = consumer;
        this.stopUpdateRunnable = stopUpdateRunnable;
    }

    public void update(T data) {
        consumer.accept(data);
    }

    public void stopUpdate() {
        stopUpdateRunnable.run();
    }

    public void setDispatcher(Dispatcher<T> dispatcher) {
        this.dispatcher = dispatcher;
    }

    public void unregister() {
        if (dispatcher == null) {
            throw new IllegalStateException("dispatcher is null");
        }

        dispatcher.unregister(this);
    }

}
