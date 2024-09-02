package hashtablepair;

import java.util.*;

public class HashMap<K, V> implements Map<K, V> {
    List<List<Entry<K, V>>> buckets;
    public static final int DEFAULT_INITIAL_CAPACITY = 16;
    private final float capacity;
    private int size = 0;


    public HashMap() {
        this(DEFAULT_INITIAL_CAPACITY);
    }


    public HashMap(int capacity) {
        this.capacity = capacity;
        buckets = new ArrayList<>(capacity);

        for (int i = 0; i < capacity; ++i) {
            buckets.add(new ArrayList<>());
        }
    }


    @Override
    public int size() {
        return size;
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public boolean containsKey(Object o) {
        if (o == null)
            return false;

        int bucket = (int) (o.hashCode() % capacity);
        for (Entry<K, V> entry : buckets.get(bucket)) {
            if (entry.getKey().equals(o)) {
                return true;
            }
        }

        return false;
    }

    @Override
    public boolean containsValue(Object o) {
        Collection<V> values = values();
        for (V v : values) {
            if (v.equals(o)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public V get(Object o) {
        int bucket = (int) (o.hashCode() % capacity);
        for (Entry<K, V> entry : buckets.get(bucket)) {
            if (entry.getKey().equals(o)) {
                return entry.getValue();
            }
        }
        return null;
    }

    @Override
    public V put(K k, V v) {
        // remove the existing k entry, if exists
        remove(k);

        //insert the new entry
        Map.Entry<K, V> entry = new Pair<>(k, v);
        int bucket = (int) (k.hashCode() % capacity);
        buckets.get(bucket).add(entry);
        ++size;

        return v;
    }

    @Override
    public V remove(Object o) {
        int bucket = (int) (o.hashCode() % capacity);
        for (Entry<K, V> entry : buckets.get(bucket)) {
            if (entry.getKey().equals(o)) {
                V v = entry.getValue();
                buckets.get(bucket).remove(entry);
                --size;
                return v;
            }
        }

        return null;
    }

    @Override
    public void putAll(Map<? extends K, ? extends V> map) {
        for (Map.Entry<? extends K, ? extends V> entry : map.entrySet()) {
            put(entry.getKey(), entry.getValue());
        }
    }

    @Override
    public void clear() {
        // clear every list inside the buckets, without changing the buckets arraylist itself
        for (List<Entry<K, V>> list : buckets) {
            list.clear();
        }
        size = 0;
    }

    @Override //O(1) space
    public Set<K> keySet() {
        return new SetOfKeys();
    }

    @Override  //O(1) space
    public Collection<V> values() {
        return new CollectionOfValues();
    }

    @Override  //O(1) space
    public Set<Entry<K, V>> entrySet() {
        return new SetOfEntries();
    }


    private class SetOfEntries extends AbstractSet<Map.Entry<K, V>> {
        @Override
        public Iterator<Entry<K, V>> iterator() {
            return new EntriesIterator();
        }

        @Override
        public int size() {
            return size;
        }

        private class EntriesIterator implements Iterator<Entry<K, V>> {
            int currentBucket = 0;
            int currentIndex = 0;
            Map.Entry<K, V> currentEntry = null;


            public EntriesIterator() {
                //set currentEntry at the first valid entry, or null if couldn't find any
                advanceToNextEntry();
            }

            private void advanceToNextEntry() {
                // loop until we find a non-empty bucket
                while (currentBucket < buckets.size()) {
                    List<Entry<K, V>> bucket = buckets.get(currentBucket);

                    if (currentIndex < bucket.size()) {
                        currentEntry = bucket.get(currentIndex);
                        ++currentIndex;
                        return;
                    } else {
                        // move to the next bucket
                        ++currentBucket;
                        currentIndex = 0;
                    }
                }

                // no more entries found
                currentEntry = null;
            }

            @Override
            public boolean hasNext() {
                return currentEntry != null;
            }

            @Override
            public Entry<K, V> next() {
                if (currentEntry == null) {
                    throw new NoSuchElementException();
                }

                Entry<K, V> entryToReturn = currentEntry;
                advanceToNextEntry();

                return entryToReturn;
            }
        }
    }

    private class SetOfKeys extends AbstractSet<K> {
        @Override
        public Iterator<K> iterator() {
            return new KeysIterator();
        }

        @Override
        public int size() {
            return size;
        }

        private class KeysIterator implements Iterator<K> {
            private final SetOfEntries.EntriesIterator entriesIterator;

            public KeysIterator() {
                this.entriesIterator = new SetOfEntries().new EntriesIterator();
            }

            @Override
            public boolean hasNext() {
                return entriesIterator.hasNext();
            }

            @Override
            public K next() {
                return entriesIterator.next().getKey();
            }
        }
    }

    private class CollectionOfValues extends AbstractCollection<V> {

        @Override
        public Iterator<V> iterator() {
            return new ValueMapIterator();
        }

        @Override
        public int size() {
            return size;
        }

        private class ValueMapIterator implements Iterator<V> {
            private final SetOfEntries.EntriesIterator entriesIterator;

            public ValueMapIterator() {
                this.entriesIterator = new SetOfEntries().new EntriesIterator();
            }

            @Override
            public boolean hasNext() {
                return entriesIterator.hasNext();
            }

            @Override
            public V next() {
                return entriesIterator.next().getValue();
            }
        }
    }
}

