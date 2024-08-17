public class ProductIMP implements Product {

    private final String name;
    private int price;

    public ProductIMP(String name, int price) {
        this.name = name;
        this.price = price;
    }

    public void setPrice(int newPrice) {
        price = newPrice;
    }
    public int getPrice() {
        return price;
    }

    public String getName() {
        return name;
    }
}
