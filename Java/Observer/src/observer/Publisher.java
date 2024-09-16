package observer;

public class Publisher<T> {
    Dispatcher<T> dispatcher = new Dispatcher<>();

    public void register(Callback<T> callback) {
        dispatcher.register(callback);
    }

    public void produce(T data) {
        dispatcher.updateAll(data);
    }

    public void close() {
        dispatcher.stopService();
    }
}
