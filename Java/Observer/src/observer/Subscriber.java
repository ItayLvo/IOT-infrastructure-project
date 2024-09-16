package observer;

import java.util.function.Consumer;

public class Subscriber<T> {
    private Callback<T> callback;

    public Subscriber(Consumer<T> consumer, Runnable stopUpdateRunnable) {
        this.callback = new Callback<>(consumer, stopUpdateRunnable);
    }

    public Subscriber(Callback<T> callback) {
        this.callback = callback;
    }

    public Subscriber() {
        //create a default Callback from default Consumer and Runnable
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                System.out.println("stopUpdate was called");
            }
        };

        Consumer<T> consumer = new Consumer<T>() {
            @Override
            public void accept(T t) {
                System.out.println("Update was received: " + t);
            }
        };

        this.callback = new Callback<>(consumer, runnable);
    }

    public void register(Publisher<T> publisher) {
        publisher.register(callback);
    }

    public void unregister() {
        callback.unregister();
    }

    public T getData() {
        return this.callback.getData();
    }
}
