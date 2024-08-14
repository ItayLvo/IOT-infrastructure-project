public class Main {
    public static void main(String[] args) {

        SinglyLinkedList list = new SinglyLinkedList();
        Integer obj1 = new Integer(1);
        Integer obj2 = new Integer(2);
        Integer obj3 = new Integer(3);
        Integer obj4 = new Integer(4);

        System.out.println("is the list empty? " +list.isEmpty());
        System.out.println("size of list = " +list.size());

        System.out.println("pushing 1 item...");
        list.pushFront(obj1);

        System.out.println("is the list empty? " +list.isEmpty());
        System.out.println("size of list = " +list.size());

        System.out.println("pushing more items...");
        list.pushFront(obj2);
        list.pushFront(obj3);
        list.pushFront(obj4);

        System.out.println("size of list = " +list.size());

        Object obj5 = list.popFront();
        System.out.println("popped from list. return value is = " + obj5);
        obj5 = list.popFront();
        System.out.println("popped from list again. return value is = " + obj5);
        System.out.println("size of list = " +list.size());

        ListIterator iter = list.begin();
        System.out.println("does list.begin has next? " +iter.hasNext());

        iter = list.find(obj2);
        System.out.println("result of list.find(obj2) = " + iter.next());

        list.popFront();
        list.popFront();
        list.popFront();    // list is empty at this point - return null
        System.out.println("size of list = " +list.size());

        list.pushFront(obj1);
        iter = list.find(obj1);
        System.out.println("result of list.find(obj1) = " + iter.next());

    }
}