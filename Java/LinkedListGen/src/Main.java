import java.util.Iterator;

public class Main {
    public static void main(String[] args) {
        LinkedListGen<String> list = new LinkedListGen<>();
        list.pushFront("four");
        list.pushFront("three");
        list.pushFront("two");
        list.pushFront("one");
        list.popFront();

        for (String s : list) {
            System.out.println(s);
        }
    }


}