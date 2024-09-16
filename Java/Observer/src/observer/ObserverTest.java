package observer;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class ObserverPatternTest {

    @Test
    void testPublisherSubscriberInteraction() {
        Publisher<String> publisher = new Publisher<>();
        Subscriber<String> subscriber = new Subscriber<>();

        subscriber.register(publisher);
        publisher.produce("Hello");

        assertEquals("Hello", subscriber.getData());
    }

    @Test
    void testMultipleSubscribers() {
        Publisher<Integer> publisher = new Publisher<>();
        Subscriber<Integer> subscriber1 = new Subscriber<>();
        Subscriber<Integer> subscriber2 = new Subscriber<>();

        subscriber1.register(publisher);
        subscriber2.register(publisher);

        publisher.produce(42);

        assertEquals(42, subscriber1.getData());
        assertEquals(42, subscriber2.getData());
    }

    @Test
    void testUnregister() {
        Publisher<String> publisher = new Publisher<>();
        Subscriber<String> subscriber = new Subscriber<>();

        subscriber.register(publisher);
        publisher.produce("First message");
        assertEquals("First message", subscriber.getData());

        subscriber.unregister();
        publisher.produce("Second message");
        assertEquals("First message", subscriber.getData()); // should still have the old data
    }

    @Test
    void testCustomCallback() {
        Publisher<Integer> publisher = new Publisher<>();
        StringBuilder result = new StringBuilder();

        Subscriber<Integer> subscriber = new Subscriber<>(
                data -> result.append("Received: ").append(data),
                () -> result.append(" | Stopped")
        );

        subscriber.register(publisher);
        publisher.produce(100);
        publisher.close();

        assertEquals("Received: 100 | Stopped", result.toString());
    }

    @Test
    void testPublisherClose() {
        Publisher<String> publisher = new Publisher<>();
        StringBuilder result = new StringBuilder();

        Subscriber<String> subscriber = new Subscriber<>(
                data -> result.append("Received: ").append(data),
                () -> result.append(" | Stopped")
        );

        subscriber.register(publisher);
        publisher.produce("Test");
        publisher.close();

        assertEquals("Received: Test | Stopped", result.toString());
    }

    @Test
    void testIllegalUnregister() {
        Subscriber<String> subscriber = new Subscriber<>();
        assertThrows(IllegalStateException.class, subscriber::unregister);
    }
}