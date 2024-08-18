import java.util.Timer;
import java.util.TimerTask;

public class VendingMachine {

    private VendingMachineState state;
    private int credit;
    private Monitor monitor;
    private VendingSlot[] slots;
    private Product selectedProduct;
    private VendingSlot selectedSlot;
    private static final int DEFAULT_NUM_PRODUCTS_PER_SLOT = 10;
    private static final int TIMEOUT_SECONDS = 5;
    private Timer timer;

    public VendingMachine(Monitor monitor, Product[] productList) {
        this.monitor = monitor;
        this.credit = 0;
        this.state = VendingMachineState.OFF;

        // create slots array with 1 slot per product
        // fill each slot with respective product
        this.slots = new VendingSlot[productList.length];
        for (int i = 0; i < productList.length; ++i) {
            slots[i] = new VendingSlot(productList[i], DEFAULT_NUM_PRODUCTS_PER_SLOT, i);
        }
    }

    public void cancel() {
        state.cancel(this);
    }

    public void insertCoin(Coin coin) {
        state.insertCoin(this, coin);
        resetTimeout();
    }

    public void selectProduct(int slotIndex) {
        if (slotIndex < 0 || slotIndex >= slots.length || slots[slotIndex].amount == 0) {
            monitor.display("Invalid slot selection");
            return;
        }

        resetTimeout();
        selectedSlot = slots[slotIndex];
        state.selectProduct(this, slots[slotIndex].product);
    }

    public void turnOn() {
        state.turnOn(this);
    }

    public void turnOff() {
        state.turnOff(this);
        cancelTimeout();
    }

    private void dispenseProduct() {
        monitor.display("dispensing product: " + selectedProduct.getName() + ", price: " + selectedProduct.getPrice());
        selectedSlot.decrementAmount();
        credit -= selectedProduct.getPrice();
        monitor.display("credits after purchase: " + credit);
        //reset machine after purchase
        selectedSlot = null;
        selectedProduct = null;
        cancelTimeout();
    }

    private boolean hasEnoughCredits() {
        return credit >= selectedProduct.getPrice();
    }

    /*********************** nested Class: Coin ***********************/
    static public enum Coin {
        TEN_AGOROT(10),
        HEZI_SHEKEL(50),
        SHEKEL(100),
        SHNEKEL(200),
        FIVE_SHEKEL(500),
        TEN_SHEKEL(1000);

        private final int value;

        Coin(int value) {
            this.value = value;
        }

        public int getValue() {
            return value;
        }
    }

    /*********************** nested Enum: VendingMachineState ***********************/
    private enum VendingMachineState {
        OFF {
            @Override
            public void cancel(VendingMachine vm) {
                return;
            } //do nothing

            @Override
            public void insertCoin(VendingMachine vm, Coin coin) {
                return;
            } //do nothing

            @Override
            public void selectProduct(VendingMachine vm, Product product) {
                return;
            } //do nothing

            @Override
            public void turnOff(VendingMachine vm) {
                return;
            } //do nothing

            @Override
            public void turnOn(VendingMachine vm) {
                vm.monitor.display("turning on");
                vm.state = VendingMachineState.WAITING_FOR_SELECTION;
            }
        },
        WAITING_FOR_SELECTION {
            @Override
            public void cancel(VendingMachine vm) {
                vm.credit = 0;
            }

            @Override
            public void insertCoin(VendingMachine vm, Coin coin) {
                vm.credit += coin.getValue();
                vm.monitor.display("credits after insertion = " + vm.credit);
            }

            @Override
            public void selectProduct(VendingMachine vm, Product product) {
                vm.selectedProduct = product;
                vm.monitor.display("product selected: " + product.getName());
                if (vm.hasEnoughCredits()) {
                    vm.dispenseProduct();
                    vm.state = VendingMachineState.WAITING_FOR_SELECTION;
                }
                else {
                    vm.monitor.display("Enter more credits. Current amount is " + vm.credit + ", price is " + vm.selectedProduct.getPrice());
                    vm.state = VendingMachineState.WAITING_FOR_MONEY;
                }
            }

            @Override
            public void turnOff(VendingMachine vm) {
                vm.credit = 0;
                vm.state = VendingMachineState.OFF;
            }

            @Override
            public void turnOn(VendingMachine vm) {
                return;
            } //do nothing
        },
        WAITING_FOR_MONEY {
            @Override
            public void cancel(VendingMachine vm) {
                vm.monitor.display("purchase canceled");
                vm.credit = 0;
            }

            @Override
            public void insertCoin(VendingMachine vm, Coin coin) {
                vm.credit += coin.getValue();
                vm.monitor.display("credits after insertion = " + vm.credit);
                this.selectProduct(vm, vm.selectedProduct);
            }

            @Override
            public void selectProduct(VendingMachine vm, Product product) {
                vm.selectedProduct = product;
                vm.monitor.display("product selected: " + product.getName());
                if (vm.hasEnoughCredits()) {
                    vm.dispenseProduct();
                    vm.state = VendingMachineState.WAITING_FOR_SELECTION;
                }
                else {
                    vm.monitor.display("Enter more credits. Current amount is " + vm.credit + ", price is " + vm.selectedProduct.getPrice());
                }
            }

            @Override
            public void turnOff(VendingMachine vm) {
                vm.credit = 0;
                vm.state = VendingMachineState.OFF;
            }

            @Override
            public void turnOn(VendingMachine vm) {
                return;
            } //do nothing
        };


        public abstract void cancel(VendingMachine vm);

        public abstract void insertCoin(VendingMachine vm, Coin coin);

        public abstract void selectProduct(VendingMachine vm, Product product);

        public abstract void turnOff(VendingMachine vm);

        public abstract void turnOn(VendingMachine vm);


    }

/*********************** nested Class: VendingSlot ***********************/
    static private class VendingSlot {
        private Product product;
        private int amount;
        private int code;

        public VendingSlot(Product product, int amount, int code) {
            this.product = product;
            this.amount = amount;
            this.code = code;
        }

        public int getAmount() {
            return amount;
        }

        public Product getProduct() {
            return product;
        }

        public void setProduct(Product product) {
            this.product = product;
        }

        public int getCode() {
            return code;
        }

        public void setCode(int code) {
            this.code = code;
        }

        public void decrementAmount() {
            --amount;
        }
    }

    /*********************** timeout methods ***********************/
    private void resetTimeout() {
        if (timer != null) {
            timer.cancel();
        }

        timer = new Timer();

        // schedule a new task that will call resetMachine() after the timeout period
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                resetMachine(); // This will be called if the timeout period expires
            }
        }, TIMEOUT_SECONDS * 1000); // Schedule the task to run after TIMEOUT_SECONDS
    }

    private void cancelTimeout() {
        // Cancel the timer to prevent the reset task from running
        if (timer != null) {
            timer.cancel();
        }
    }

    private void resetMachine() {
        monitor.display("Timeout. refunding credit: " + credit);
        credit = 0;
        selectedProduct = null;
        state = VendingMachineState.WAITING_FOR_SELECTION;
    }



}
