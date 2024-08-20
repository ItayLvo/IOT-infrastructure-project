public class Foo implements Comparable{
    private String name;
    private static int privateConstant = 0;
    public static int publicConstant = 1;


    public Foo(){
    }

    public Foo(String name){
        this.name = name;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Override
    public int compareTo(Object o) {
        return o.toString().compareTo(name);
    }
}