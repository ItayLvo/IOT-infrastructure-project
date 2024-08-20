import java.lang.reflect.*;
import java.util.Arrays;

public class ObjectAnalyzer {

    public static void Analyze() throws ClassNotFoundException, NoSuchMethodException, InstantiationException, IllegalAccessException, InvocationTargetException {
        Class<Foo> fooClass = (Class<Foo>) Class.forName("Foo");
        //Class fooClass = Class.forName("Foo");  <-- this also works

        //print Foo's ancestor
        System.out.println(fooClass.getSuperclass().getName());

        //print Foo's implemented interfaces using Arrays.toString
        System.out.println(Arrays.toString(fooClass.getInterfaces()));

        //print Foo's implemented interfaces using ForEach
        Class<?>[] interfaceArr = fooClass.getInterfaces();
        for (Class<?> interfaceInstance : interfaceArr) {
            System.out.println(interfaceInstance.getName());
        }

        //fetching and printing Foo's fields
        Field[] fields = fooClass.getDeclaredFields();
        System.out.println("There are " + fields.length + " fields");
        for (Field field : fields) {
//            field.setAccessible(true);
            System.out.println(field.getName());
        }


        Constructor<Foo> ct = fooClass.getConstructor(String.class);
        Object arglist[] = new Object[1];
        arglist[0] = new String("hi from reflection");
        Foo newFoo = (Foo) ct.newInstance(arglist);
        System.out.println(newFoo.getName());


        Foo newFoo2 = (Foo) fooClass.newInstance();
        System.out.println(newFoo2.getName());
    }
}
