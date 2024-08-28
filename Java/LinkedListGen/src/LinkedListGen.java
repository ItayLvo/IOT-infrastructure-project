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
        ++modCount;

        T t = head.data;
        head = head.next;

        if (head == null) {
            tail = null;
        }

        return t;
    }

    public int size() {
        int count = 0;

        for(T item : this) {
            ++count;
        }

        return count;
    }

    public boolean isEmpty() {
        return (head == null);
    }


    public Iterator<T> find(T data) {
        ListIterator<T> iter = new ListIterator<T>(this.head);
        Node<T> resultNode = null;

        while (iter.hasNext()) {
            //save current node before advancing it with next()
            resultNode = iter.node;
            T currentItem = iter.next();

            if (currentItem.equals(data)) {
                return new ListIterator<T>(resultNode);
            }
        }
        // if didn't find the item, return null
        return null;
    }

    /* alternative implementation using node, not "fail-fast safe":
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
        list1.head = null;
        list1.tail = null;
        list2.head = null;
        list2.tail = null;

        return mergedLists;
    }



    public static <T> LinkedListGen<T> newReverse(LinkedListGen<T> list) {
        LinkedListGen<T> reversedList = new LinkedListGen<>();

        for (T item : list) {
            reversedList.pushFront(item);
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
        return (Iterator<T>)new ListIterator<>(head);
    }


    // nested Node<e> class:
    private static class Node<E> {
        private E data;
        private Node<E> next;

        public Node(E t, Node<E> next) {
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
