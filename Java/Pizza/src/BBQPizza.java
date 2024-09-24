public class BBQPizza extends PizzaTopping{
    String description = "BBQ";
    int price = 5;

    public BBQPizza(Pizza pizza) {
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
