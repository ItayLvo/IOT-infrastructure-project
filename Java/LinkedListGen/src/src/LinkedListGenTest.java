package src;

import org.junit.jupiter.api.Test;
import java.util.ConcurrentModificationException;
import java.util.Iterator;
import static org.junit.jupiter.api.Assertions.*;

class LinkedListGenTest {

    @org.junit.jupiter.api.Test
    void pushFront() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(1);
        list.pushFront(2);
        assertEquals(2, list.popFront());
    }

    @org.junit.jupiter.api.Test
    void popFront() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(2);
        list.pushFront(1);
        list.pushFront(3);
        assertEquals(3, list.popFront());
    }

    @org.junit.jupiter.api.Test
    void size() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(1);
        list.pushFront(2);
        list.popFront();
        assertEquals(1, list.size());
    }

    @org.junit.jupiter.api.Test
    void isEmpty() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        assertTrue(list.isEmpty());
        list.pushFront(1);
        assertFalse(list.isEmpty());
    }

    @org.junit.jupiter.api.Test
    void find() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(1);
        list.pushFront(2);
        list.pushFront(3);
        Iterator<Integer> iter = list.find(2);
        Integer res = iter.next();
        assertEquals(2, res);
    }


    @org.junit.jupiter.api.Test
    void findShouldNotFindAndReturnNullIterator() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(1);
        list.pushFront(2);
        list.pushFront(3);
        Iterator<Integer> iter = list.find(444);
        assertFalse(iter.hasNext());
    }

    @org.junit.jupiter.api.Test
    void mergeLists() {
        LinkedListGen<Integer> list1 = new LinkedListGen<>();
        LinkedListGen<Integer> list2 = new LinkedListGen<>();
        list1.pushFront(3);
        list1.pushFront(2);
        list1.pushFront(1);
        list2.pushFront(6);
        list2.pushFront(5);
        list2.pushFront(4);

        LinkedListGen<Integer> mergedList = LinkedListGen.mergeLists(list1, list2);

        int runner = 1;
        for (Integer i : mergedList) {
            assertEquals(runner, i);
            ++runner;
        }

    }

    @org.junit.jupiter.api.Test
    void newReverse() {
        LinkedListGen<Integer> list = new LinkedListGen<>();
        list.pushFront(1);
        list.pushFront(2);
        list.pushFront(3);

        //current list = 3 -> 2 -> 1

        LinkedListGen<Integer> reversed = LinkedListGen.newReverse(list);

        //list after reverse = 1 -> 2 -> 3
        assertEquals(1, reversed.popFront());
        assertEquals(2, reversed.popFront());
        assertEquals(3, reversed.popFront());
    }

    @org.junit.jupiter.api.Test
    void printList() {
        LinkedListGen<String> list = new LinkedListGen<>();
        list.pushFront("three");
        list.pushFront("two");
        list.pushFront("one");

        LinkedListGen.printList(list);
    }

    @Test
    void testFailFast() {
        ConcurrentModificationException e = assertThrows(ConcurrentModificationException.class, () ->
        {
            LinkedListGen<Integer> list = new LinkedListGen<>();
            list.pushFront(1);
            list.pushFront(2);

            Iterator<Integer> iterator = list.iterator();
            // move to the first element
            System.out.println(iterator.next()); // should print 2

            // modify the list (this will increment modCount)
            list.pushFront(3);

            //this should throw an exception
            Integer i = iterator.next();
        });
    }
}