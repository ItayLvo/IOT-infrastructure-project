package dirmonitorwatchservice;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class DirMonitor {

    public static void main(String[] args) throws IOException, InterruptedException {
        WatchService watchService = FileSystems.getDefault().newWatchService();

        Path pathToMonitor = Paths.get("/home/itay/git/build/test");
        Path destinationPath = Paths.get("/home/itay/git/build/watcher_testing.txt");

//        if (!Files.exists(destinationPath)) {
//            Files.createFile(destinationPath);
//        }

        pathToMonitor.register(watchService,
                StandardWatchEventKinds.ENTRY_MODIFY,
                StandardWatchEventKinds.ENTRY_DELETE,
                StandardWatchEventKinds.ENTRY_CREATE);

        WatchKey key;
        while ((key = watchService.take()) != null) {
            for (WatchEvent<?> event : key.pollEvents()) {
                String logEntry = "Event kind: " + event.kind() + ". File affected: " + event.context() + ". Timestamp: " + LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_DATE_TIME) + "\n";
                Files.write(destinationPath, logEntry.getBytes(), StandardOpenOption.APPEND, StandardOpenOption.CREATE);
            }
            key.reset();

        }
    }
}
