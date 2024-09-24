public class FarmHouse implements Pizza{
    int price = 11;
    String description = "Farm House";

    @Override
    public String getDescription() {
        return description;
    }

    @Override
    public int getPrice() {
        return price;
    }
}
