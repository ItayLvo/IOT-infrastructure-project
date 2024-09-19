package dynamicjarloader;

import java.io.IOException;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        DynamicJarLoader loader;
        try {
            loader = new DynamicJarLoader("java.io.Serializable", "/home/itay/git/Java/DynamicJarLoader/out/artifacts/DynamicJarLoader_jar/DynamicJarLoader.jar");
        } catch (ClassNotFoundException | IOException e) {
            System.out.println(e.getMessage() + " something failed");
            throw new RuntimeException(e);
        }

        List<Class<?>> classList = loader.getClassList();
        for (Class<?> clazz : classList) {
            System.out.println(clazz.getName());
        }
    }
}