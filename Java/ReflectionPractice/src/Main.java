import java.lang.reflect.InvocationTargetException;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        try {
            ObjectAnalyzer.Analyze();
        } catch (ClassNotFoundException e) {
            throw new RuntimeException(e);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        } catch (InstantiationException e) {
            throw new RuntimeException(e);
        } catch (IllegalAccessException e) {
            throw new RuntimeException(e);
        } catch (InvocationTargetException e) {
            throw new RuntimeException(e);
        }
    }
}