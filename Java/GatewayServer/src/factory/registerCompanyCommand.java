package factory;

public class registerCompanyCommand extends Command {

    public registerCompanyCommand(String data) {
        super(data);
    }

    @Override
    public void run() {
        super.run();
        System.out.println("registering company command!");
    }
}
