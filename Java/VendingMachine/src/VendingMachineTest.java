import org.junit.*;
import org.junit.jupiter.api.AfterEach;

import static org.junit.Assert.*;

public class VendingMachineTest {

    private VendingMachine vendingMachine;

    @Before
    public void setUp() {
        Product p1 = new ProductIMP("p1", 100);
        Product p2 = new ProductIMP("p2", 200);
        Product p3 = new ProductIMP("p3", 350);

        Monitor display = new Display();

        Product[] products = {p1, p2, p3};
        vendingMachine = new VendingMachine(display, products);

        vendingMachine.turnOn();
    }

    @After
    public void release(){
        vendingMachine.turnOff();
    }

    @AfterEach
    public void cleanup(){
        vendingMachine.cancel();
        System.out.println();
        System.out.println();
    }


    @Test
    public void testInsertCoin() {
        vendingMachine.insertCoin(VendingMachine.Coin.HEZI_SHEKEL);
        vendingMachine.insertCoin(VendingMachine.Coin.TEN_AGOROT);
    }

    @Test
    public void testSelectProduct() {
        vendingMachine.selectProduct(0);
        vendingMachine.insertCoin(VendingMachine.Coin.HEZI_SHEKEL);
        vendingMachine.insertCoin(VendingMachine.Coin.TEN_AGOROT);
        vendingMachine.insertCoin(VendingMachine.Coin.TEN_SHEKEL);
    }

    @Test
    public void testTimeout() throws InterruptedException {
        vendingMachine.selectProduct(0);
        vendingMachine.insertCoin(VendingMachine.Coin.FIVE_SHEKEL);
        for (int i = 0; i < 6; ++i) {
            Thread.sleep(1000);
        }
    }

}