public class PizzaTopping implements Pizza {
    int price = 0;
    String description = "abstract topping";
    Pizza pizza;

    public PizzaTopping(Pizza pizza) {
        this.pizza = pizza;
    }

    @Override
    public String getDescription() {
        return pizza.getDescription();
    }

    @Override
    public int getPrice() {
        return pizza.getPrice();
    }
}
