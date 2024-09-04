package concurrencypractice;

public class ImplementsRunnable implements Runnable {

    @Override
    public void run() {
        for (int i = 0; i < 10; i++) {
            System.out.println(i + "interface ");
            try {
                Thread.sleep(300);
            } catch (InterruptedException e) { }
        }
    }
}
