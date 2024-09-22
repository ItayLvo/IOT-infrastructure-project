package gatewayserver;

import java.util.Map;

public interface Parser {
    Map.Entry<String, String> parse(String input);
}
