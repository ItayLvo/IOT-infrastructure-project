import java.util.ConcurrentModificationException;
import java.util.Iterator;
//import java.util.LinkedList;

public class LinkedListGen<T> implements Iterable<T> {
    private Node<T> head;
    private Node<T> tail;
    //fail-fast counter
    private volatile int modCount = 0;

    //constructor:
    public LinkedListGen() {
    }

    // public functions
    public void pushFront(T t) {
        ++modCount;
        Node<T> newHead = new Node<>(t, head);
        head = newHead;

        if (tail == null) {
            tail = newHead;
        }
    }

    public T popFront() {
        if (head == null) {
            return null;
        }

        ++modCount;

        T t = head.data;
        head = head.next;

        if (head == null) {
            tail = null;
        }

        return t;
    }

    public int size() {
        Node<T> n = head;
        int count = 0;
        while (n != null) {
            ++count;
            n = n.next;
        }

        return count;
    }

    public boolean isEmpty() {
        return (size() == 0);
    }

    public Iterator<T> find(T data) {
        Node<T> n = head;

        while (n != null) {
            if (n.data.equals(data)) {
                return new ListIterator<T>(n);
            }
            n = n.next;
        }

        return null;
    }

    /* alternative implementation using iterator:
    public Iterator<T> find(T data) {
        ListIterator<T> iter = new ListIterator<T>(this.head);

        while (iter.hasNext()) {
            T t = iter.next();
            if (t.equals(data)) {
                return new ListIterator<T>(new Node<T>(t, iter.node));
            }
        }

        return iter;
    }
    */

    //removes all nodes from both lists and returns a new one
    public static <T> LinkedListGen<T> mergeLists(LinkedListGen<T> list1, LinkedListGen<T> list2) {
        ++list1.modCount;
        ++list2.modCount;

        LinkedListGen<T> mergedLists = new LinkedListGen<T>();

        //connect the lists
        list1.tail.next = list2.head;

        //set head and tail of the new list
        mergedLists.head = list1.head;
        mergedLists.tail = list2.tail;

        //nullify old lists for garbage collection
        list1 = null;
        list2 = null;

        return mergedLists;
    }


    public static <T> LinkedListGen<T> newReverse(LinkedListGen<T> list) {
        LinkedListGen<T> reversedList = new LinkedListGen<>();
        Node<T> runner = new Node<>(list.head.data, list.head.next);

        while (runner != null) {
            reversedList.pushFront(runner.data);
            runner = runner.next;
        }

        return reversedList;
    }


    public static <T> void printList(LinkedListGen<T> list) {
        for (T t : list) {
            System.out.print(t + " ");
        }
    }

    @Override
    public Iterator<T> iterator() {
        return new ListIterator<>(head);
    }


    // nested Node<T> class:
    private static class Node<T> {
        private T data;
        private Node<T> next;

        public Node(T t, Node<T> next) {
            this.data = t;
            this.next = next;
        }
    }


    // nested ListIterator<T> class:
    private class ListIterator<T> implements Iterator<T> {
        private Node<T> node;
        int expectedModCount;

        //constructor:
        public ListIterator(Node<T> node) {
            this.node = node;
            expectedModCount = LinkedListGen.this.modCount;
        }

        @Override
        public T next() {
            if (expectedModCount != LinkedListGen.this.modCount) {
                throw new ConcurrentModificationException();
            }

            T t = node.data;
            node = node.next;
            return t;
        }

        @Override
        public boolean hasNext() {
            if (expectedModCount != LinkedListGen.this.modCount) {
                throw new ConcurrentModificationException();
            }

            return node != null;
        }
    }
}
