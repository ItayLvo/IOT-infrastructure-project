public class Jalapeno extends PizzaTopping {
    String description = "Jalapeno";
    int price = 4;

    public Jalapeno(Pizza pizza) {
        super(pizza);
    }
    @Override
    public String getDescription() {
        return pizza.getDescription() + "with " + description;
    }
    @Override
    public int getPrice() {
        return pizza.getPrice() + price;
    }
}
