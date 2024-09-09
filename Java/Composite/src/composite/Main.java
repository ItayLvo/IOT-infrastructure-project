package composite;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        Tree tree = new Tree("/home/itay/git/projects");
        tree.print();
    }
}