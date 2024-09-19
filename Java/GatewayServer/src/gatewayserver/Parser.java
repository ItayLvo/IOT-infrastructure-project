package gatewayserver;

import java.util.Map;

public interface Parser {
    public Map.Entry<String, String> parse(String input);
}
