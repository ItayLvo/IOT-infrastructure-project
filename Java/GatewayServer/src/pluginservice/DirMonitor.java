package pluginservice;

import java.io.IOException;
import java.nio.file.*;
import java.util.function.Consumer;

public class DirMonitor implements Runnable {
    private final String pathOfDirToMonitor;
    private final Consumer<Path> actionOnJarDetected;

    public DirMonitor(String pathOfDirToMonitor, Consumer<Path> actionOnJarDetected) {
        this.pathOfDirToMonitor = pathOfDirToMonitor;
        this.actionOnJarDetected = actionOnJarDetected;
    }


    @Override
    public void run() {
        monitorDirectory();
    }


    public void monitorDirectory() {
        WatchService watchService;
        try {
            watchService = FileSystems.getDefault().newWatchService();
        } catch (IOException e) {
            throw new RuntimeException("Failed to initiate Watch Service", e);
        }

        Path pathToMonitor = Paths.get(this.pathOfDirToMonitor);

        try {
            pathToMonitor.register(watchService,
                    StandardWatchEventKinds.ENTRY_MODIFY,
                    StandardWatchEventKinds.ENTRY_CREATE);
        } catch (IOException e) {
            throw new RuntimeException("Failed to register new Watch Service events", e);
        }
        // we don't handle JARs being deleted, only added (so no ENTRY_DELETE)

        WatchKey key;
        try {
            while ((key = watchService.take()) != null) {
                for (WatchEvent<?> event : key.pollEvents()) {
//                String logEntry = "Event kind: " + event.kind() + ". File affected: " + event.context() + ". Timestamp: " + LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_DATE_TIME) + "\n";
//                Files.write(destinationPath, logEntry.getBytes(), StandardOpenOption.APPEND, StandardOpenOption.CREATE);
//                System.out.println("LogEntry = " + logEntry);
//                System.out.println("event.context() = " + event.context()); //TODO

//                this.pluginService.handleJARDetected((Path)event.context());  //THIS WAS LAST WORKING VERSION! using public PluginService stuff
                    actionOnJarDetected.accept(pathToMonitor.resolve((Path) event.context()));
                }
                key.reset();

            }
        } catch (InterruptedException e) {
            throw new RuntimeException("Interrupted during WatchService.take()", e);
        }
        try {
            watchService.close();
        } catch (IOException e) {
            throw new RuntimeException("Error closing the WatcherService", e);
        }

    }
}
