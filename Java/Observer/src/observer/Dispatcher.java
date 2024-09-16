package observer;

import java.util.ArrayList;
import java.util.List;

public class Dispatcher<T> {
    private final List<Callback<T>> subscribers;

    public Dispatcher() {
        subscribers = new ArrayList<>();
    }

    public void register(Callback<T>  subscriber) {
        subscribers.add(subscriber);
        subscriber.setDispatcher(this);
    }

    public void unregister(Callback<T> subscriber) {
        subscribers.remove(subscriber);
        subscriber.setDispatcher(null);
    }

    public void updateAll(T data) {
        for (Callback<T> subscriber : subscribers) {
            subscriber.update(data);
        }
    }

    public void stopService() {
        for (Callback<T> subscriber : subscribers) {
            subscriber.stopUpdate();
            subscriber.setDispatcher(null);
        }
    }


}
