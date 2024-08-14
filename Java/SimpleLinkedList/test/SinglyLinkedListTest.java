import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;



class SinglyLinkedListTest {

    @Test
    void testListSizeShouldBeTwo() {
        SinglyLinkedList list = new SinglyLinkedList();
        Integer obj1 = new Integer(1000);
        list.pushFront(obj1);
        obj1 = new Integer(1000);
        list.pushFront(obj1);
        obj1 = new Integer(2000);
        list.pushFront(obj1);
        obj1 = new Integer(3000);
        list.popFront();

        int sizeOfList = list.size();
        Assertions.assertEquals(2, sizeOfList);
    }


    @Test
    void testPopEmptyListShouldThrowNullPointerException() {
        SinglyLinkedList list = new SinglyLinkedList();
        Assertions.assertThrows(NullPointerException.class, () -> list.popFront());
    }


    @Test
    void testListIsEmpty() {
        SinglyLinkedList list = new SinglyLinkedList();
        Assertions.assertTrue(list.isEmpty());

        Integer obj1 = new Integer(1000);
        list.pushFront(obj1);

        Assertions.assertFalse(list.isEmpty());
    }

    @Test
    void testFindShouldFindElement() {
        SinglyLinkedList list = new SinglyLinkedList();

        Integer obj1 = new Integer(1000);
        list.pushFront(obj1);

        Integer obj2 = new Integer(2000);
        list.pushFront(obj2);

        Integer obj3 = new Integer(3000);
        list.pushFront(obj3);

        ListIterator findResultIter = list.find(obj2);
        Assertions.assertEquals(obj2, findResultIter.next());
    }

    @Test
    void testFindShouldReturnNull() {
        SinglyLinkedList list = new SinglyLinkedList();

        Integer obj1 = 1000;
        list.pushFront(obj1);

        Integer obj2 = 2000;
        list.pushFront(obj2);

        Integer obj3 = 3000;
        list.pushFront(obj3);

        String differentObj = "random object";

        ListIterator findResultIter = list.find(differentObj);
        Assertions.assertNull(findResultIter);
    }
}