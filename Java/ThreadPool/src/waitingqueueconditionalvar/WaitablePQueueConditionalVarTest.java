package waitingqueueconditionalvar;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class WaitablePQueueConditionalVarTest {

    @Test
    void enqueue() {
        WaitablePQueue<Integer> wpq = new WaitablePQueue<>();
        wpq.enqueue(5);
        wpq.enqueue(10);
        wpq.enqueue(3);
        assertEquals(3, wpq.dequeue());
        assertEquals(5, wpq.dequeue());
        assertEquals(1, wpq.size());
        assertEquals(10, wpq.dequeue());
    }

    @Test
    void dequeue() {
    }

    @Test
    void remove() {
    }

    @Test
    void peek() {
    }

    @Test
    void size() {
    }

    @Test
    void isEmpty() {
    }
}