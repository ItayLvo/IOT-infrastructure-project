package hashtablepair;

import java.util.ArrayList;
import java.util.Map;
import java.util.Objects;

public class Pair<K,V> implements Map.Entry<K,V> {
    private K key;
    private V value;

    public Pair(K key, V value) {
        this.key = key;
        this.value = value;
    }

    @Override
    public K getKey() {
        return key;
    }

    @Override
    public V getValue() {
        return value;
    }

    @Override
    public V setValue(V v) {
        V oldValue = this.value;
        this.value = v;
        return oldValue;
    }

    @Override
    public String toString() {
        return "[" + key + ", " + value + "]";
    }

    @Override
    public int hashCode() {
        return Objects.hash(key, value);
    }

    @SuppressWarnings("unchecked")
    @Override
    public boolean equals(Object obj) {
        if (this == obj) {
            return true;
        }
        if (!(obj instanceof Pair)) {
            return false;
        }

        Pair<K,V> other = (Pair<K,V>) obj;
        return Objects.equals(key, other.key) && Objects.equals(value, other.value);
    }

    public static <K,V> Pair<K,V> of(K key, V value) {
        return new Pair<K,V>(key, value);
    }

    public static <K,V> Pair<V,K> swap(Pair<K,V> pair) {
        return new Pair<V, K>(pair.getValue(), pair.getKey());
    }


    public static <T extends Comparable<T>> Pair<T, T> minmax(ArrayList<T> arr) {
        T min = arr.get(0);
        T max = arr.get(0);

        for (T t: arr) {
            if (t.compareTo(min) < 0) {
                min = t;
            }
            if (t.compareTo(max) > 0) {
                max = t;
            }
        }

        return new Pair<T, T>(min, max);
    }
}
