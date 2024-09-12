package composite;

import java.io.FileNotFoundException;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        Tree tree = null;
        try {
            tree = new Tree("/home/itay/git");
            tree.print();
        } catch (FileNotFoundException e) {
            System.out.println("File not found!");
        }

    }
}