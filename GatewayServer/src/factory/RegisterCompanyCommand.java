package factory;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class RegisterCompanyCommand implements Command {
    private final String data;

    public RegisterCompanyCommand(String data) {
        this.data = data;
    }

    @Override
    public String getData() {
        return data;
    }

    @Override
    public void execute() {
        System.out.println("registering company command!\tThis class is: " + this.getClass().getSimpleName() + ", data: " + data);
    }

    public static void main(String[] args) throws InvocationTargetException, InstantiationException, IllegalAccessException {
        Constructor<?> ctor;
        try {
            ctor = RegisterCompanyCommand.class.getConstructor(String.class);
        } catch (NoSuchMethodException e) {
            throw new RuntimeException(e);
        }

        Command command = (Command) ctor.newInstance("dd");
        System.out.println(command.getData());
    }
}
