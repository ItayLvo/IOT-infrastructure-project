package commandplugins;

import factory.Command;

public class RegisterProductCommand implements Command {
    private final String data;

    public RegisterProductCommand(String data) {
        this.data = data;
    }

    @Override
    public String getData() {
        return data;
    }

    @Override
    public void execute() {
        System.out.println("REGISTER PRODUCT command!\tThis class is: " + this.getClass().getSimpleName() + ", data: " + data);
    }
}
