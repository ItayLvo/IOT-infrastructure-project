package concurrencypractice;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello world!");
        ExtendsThread extendsThread = new ExtendsThread();
        extendsThread.start();

        ImplementsRunnable imp = new ImplementsRunnable();
        Thread t1 = new Thread(imp);
        t1.start();
    }
}