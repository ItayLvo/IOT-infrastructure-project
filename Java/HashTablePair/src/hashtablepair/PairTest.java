package hashtablepair;

import java.util.ArrayList;

import static org.junit.jupiter.api.Assertions.*;

class PairTest {

    @org.junit.jupiter.api.Test
    void getKey() {
        Pair<String, String> pair = new Pair<>("a", "b");
        String key = pair.getKey();
        assertEquals("a", key);
    }

    @org.junit.jupiter.api.Test
    void getValue() {
        Pair<String, String> pair = new Pair<>("a", "b");
        String value = pair.getValue();
        assertEquals("b", value);
    }

    @org.junit.jupiter.api.Test
    void setValue() {
        Pair<String, String> pair = new Pair<>("a", "b");
        pair.setValue("c");
        assertEquals("c", pair.getValue());
    }

    @org.junit.jupiter.api.Test
    void testEquals() {
        Pair<String, String> pair1 = new Pair<>("a", "b");
        Pair<String, String> pair2 = new Pair<>("a", "c");
        assertTrue(pair1.equals(pair2));
    }

    @org.junit.jupiter.api.Test
    void of() {
        String key = "a";
        Integer value = 1;
        Pair<String, Integer> newPair = Pair.of(key, value);
        assertEquals(key, newPair.getKey());
        assertEquals(value, newPair.getValue());
    }

    @org.junit.jupiter.api.Test
    void swap() {
        String key = "a";
        Integer value = 1;
        Pair<String, Integer> newPair = new Pair<>(key, value);

        Pair<Integer, String> swapped = Pair.swap(newPair);

        assertEquals(value, swapped.getKey());
        assertEquals(key, swapped.getValue());
    }

    @org.junit.jupiter.api.Test
    void minmax() {
        ArrayList<Integer> arr = new ArrayList<>();
        arr.add(4);
        arr.add(3);
        arr.add(1);
        arr.add(2);
        arr.add(8);
        arr.add(5);
        arr.add(7);
        arr.add(6);
        Pair<Integer, Integer> minmaxtest = Pair.minmax(arr);
        assertEquals(1, minmaxtest.getKey());
        assertEquals(8, minmaxtest.getValue());
    }
}