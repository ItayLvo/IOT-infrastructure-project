package hashtablepair;

import org.junit.jupiter.api.Test;

import java.util.*;

import static org.junit.jupiter.api.Assertions.*;

class HashMapTest {

    @Test
    void size() {
        HashMap<Integer, String> map = new HashMap<>();
        assertEquals(0, map.size());
        map.put(1, "one");
        assertEquals(1, map.size());
        map.put(2, "two");
        assertEquals(2, map.size());
    }

    @Test
    void isEmpty() {
        HashMap<Integer, String> map = new HashMap<>();
        assertTrue(map.isEmpty());
        map.put(1, "one");
        assertFalse(map.isEmpty());
    }

    @Test
    void containsKey() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        assertTrue(map.containsKey(1));
        assertFalse(map.containsKey(2));
    }

    @Test
    void containsValue() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        assertTrue(map.containsValue("one"));
        assertFalse(map.containsValue("two"));
    }

    @Test
    void get() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        assertEquals("one", map.get(1));
    }

    @Test
    void getNotFoundReturnsNull() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        assertNull(map.get(5));
    }

    @Test
    void put() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        assertEquals("one", map.get(1));
    }

    @Test
    void putOverrideOldPair() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        map.put(1, "two");
        assertFalse(map.containsValue("one"));
        assertNotEquals("one", map.get(1));
        assertEquals("two", map.get(1));
    }

    @Test
    void remove() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        map.put(2, "two");
        map.remove(2);
        assertEquals(1, map.size());
        assertNull(map.get(2));
        assertEquals("one", map.get(1));
    }

    @Test
    void putAll() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        map.put(2, "two");
        map.put(3, "three");

        Map<Integer, String> newMap = new LinkedHashMap<>();
        newMap.put(3, "new three");
        newMap.put(4, "four");
        newMap.put(5, "five");

        map.putAll(newMap);

        assertEquals(5, map.size());
        assertFalse(map.containsValue("three"));
        assertTrue(map.containsValue("new three"));

        //should print 1: one 2: two 3: new three 4: four 5: five
        Set<Map.Entry<Integer, String>> entries = map.entrySet();
        for (Map.Entry<Integer, String> entry : entries) {
            System.out.println(entry.getKey() + ": " + entry.getValue());
        }

    }

    @Test
    void clear() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1, "one");
        map.put(2, "two");
        map.put(3, "three");
        map.put(50000, "many");
        map.clear();
        assertEquals(0, map.size());

        map.put(1, "one");
        map.put(2, "two");
        map.put(3, "three");
        map.put(50000, "many");
        assertEquals(4, map.size());
    }

    @Test
    void keySet() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1000, "one");
        map.put(2000, "two");
        map.put(3000, "three");
        map.put(4000, "four");
        Set<Integer> keySet = map.keySet();
        assertTrue(keySet.contains(1000));
        assertTrue(keySet.contains(2000));
        assertTrue(keySet.contains(3000));
        assertTrue(keySet.contains(4000));
        assertEquals(4, keySet.size());
        for (Integer key : keySet)
            System.out.println(key);
    }

    @Test
    void values() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1000, "one");
        map.put(2000, "two");
        map.put(3000, "three");
        map.put(4000, "four");
        Collection<String> valueCol = map.values();
        assertTrue(valueCol.contains("one"));
        assertTrue(valueCol.contains("two"));
        assertTrue(valueCol.contains("three"));
        assertTrue(valueCol.contains("four"));
        assertEquals(4, valueCol.size());
        for (String value : valueCol)
            System.out.println(value);
    }

    @Test
    void entrySet() {
        HashMap<Integer, String> map = new HashMap<>();
        map.put(1000, "one");
        map.put(2000, "two");
        map.put(3000, "three");
        map.put(4000, "four");
        Set<Map.Entry<Integer, String>> entrySet = map.entrySet();

        assertEquals(4, entrySet.size());
        assertTrue(entrySet.contains(new Pair<>(1000, "one")));

        assertFalse(entrySet.contains(new Pair<>(33000, "12312312")));

        for (Map.Entry<Integer, String> ent : map.entrySet()) {
            System.out.println(ent.getKey() + ": " + ent.getValue());
        }
    }

    @Test
    void testFailFast() {
        ConcurrentModificationException e = assertThrows(ConcurrentModificationException.class, () ->
        {
            HashMap<Integer, String> map = new HashMap<>();
            map.put(1000, "one");
            map.put(2000, "two");
            map.put(3000, "three");
            Set<Map.Entry<Integer, String>> entrySet = map.entrySet();
            Iterator<Map.Entry<Integer, String>> iterator = entrySet.iterator();
            iterator.next();
            map.remove(2);
            iterator.next();
        });
    }
}