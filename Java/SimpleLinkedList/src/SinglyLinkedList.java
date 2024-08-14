//import java.util.LinkedList;
//import java.util.ListIterator;


public class SinglyLinkedList {
    private Node head;

    public SinglyLinkedList() {
        this.head = null;
    }

    public void pushFront(Object data) {
        Node newNode = new Node(data, null);
        if (head == null) {
            this.head = newNode;
        }
        else {
            newNode.next = this.head;
            this.head = newNode;
        }
    }

    public Object popFront() {

        //save current head and return it to the user
        Node temp = this.head;

        //set the new head to be head->next
        Node newHead = this.head.next;
        this.head = newHead;

        return temp.data;
    }


    public int size() {
        int count = 0;
        Node runner = this.head;

        while (runner != null) {
            ++count;
            runner = runner.next;
        }

        return count;
    }


    public boolean isEmpty() {
        return (this.head == null);
    }

    public ListIterator begin() {
        ListIteratorIMP iter = new ListIteratorIMP(this.head);

        return iter;

    }

    public ListIterator find(Object key) {
        Node runner = this.head;
        ListIteratorIMP iter = null;

        while (runner != null) {
            if (key.equals(runner.data)) {
                iter = new ListIteratorIMP(runner);
                break;
            }
            runner = runner.next;
        }

        return iter;
    }


    //    -------- inner class Node --------
    private static class Node {
        private Object data;
        private Node next;


        public Node(Object data, Node next) {
            this.data = data;
            this.next = next;
        }
    }

    //    -------- inner class LisIteratorIMP --------
    private static class ListIteratorIMP implements ListIterator {
        private Node node;

        public ListIteratorIMP(Node node) {
            this.node = node;
        }

        @Override
        public boolean hasNext() {
            return (this.node != null);
        }

        @Override
        public Object next() {
            Object toReturn = this.node.data;
            this.node = this.node.next;
            return toReturn;
        }
    }
}



