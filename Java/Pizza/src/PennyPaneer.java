public class PennyPaneer implements Pizza {
    int price = 10;
    String description = "Penny Paneer";

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public int getPrice() {
        return price;
    }
}
