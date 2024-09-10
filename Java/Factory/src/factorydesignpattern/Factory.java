package factorydesignpattern;

import java.util.HashMap;
import java.util.Map;
import java.util.function.Function;

public class Factory<K, T, D> {

    private final Map<K, Function<D, ? extends T>> creationFunctions = new HashMap<>();

    T create(K key, D data) {
        Function<D, ? extends T> createFunction = creationFunctions.get(key);
        if (createFunction == null) {
            throw new RuntimeException("Create function not found for key: " + key);
        }

        return createFunction.apply(data);
    }

    void add(K key, Function<D, ? extends T> createFunction) {
        creationFunctions.put(key, createFunction);
    }

    public Main.Book instanceCreateFantasyBook(String title) {
        return new Main.FantasyBook(title);
    }
}
