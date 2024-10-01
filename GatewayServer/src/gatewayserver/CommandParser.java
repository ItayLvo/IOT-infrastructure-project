package gatewayserver;

import java.util.AbstractMap;
import java.util.Map;

public class CommandParser implements Parser {
    Map.Entry<String, String> parseResult;

    @Override
    public Map.Entry<String, String> parse(String input) {
        // Check if the input string starts with '@' and contains at least two '@'
        if (input == null || input.length() < 3 || input.indexOf('@') == -1) {
            throw new IllegalArgumentException("Invalid format: Input must be in the format '@key@data'");
        }

        // Find the first and second '@' positions
        int firstAt = input.indexOf('@');
        int secondAt = input.indexOf('@', firstAt + 1);

        // Validate if the second '@' exists
        if (secondAt == -1) {
            throw new IllegalArgumentException("Invalid format: Missing second '@'");
        }

        // Extract key (between the two '@' symbols)
        String key = input.substring(firstAt + 1, secondAt);

        // Extract data (after the second '@')
        String data = input.substring(secondAt + 1);

        // Return a Map.Entry with the key and data
        return new AbstractMap.SimpleEntry<>(key, data);
    }
}
