package pluginservice;

import gatewayserver.RequestProcessingService;

import java.io.IOException;
import java.nio.file.*;

public class DirMonitor implements Runnable {
    private final String pathOfDirToMonitor;
    private final RequestProcessingService.PluginService pluginService;


    public DirMonitor(String pathOfDirToMonitor, RequestProcessingService.PluginService pluginService) {
        this.pathOfDirToMonitor = pathOfDirToMonitor;
        this.pluginService = pluginService;
    }


    @Override
    public void run() {
        try {
            monitorDirectory();
        } catch (IOException | NoSuchMethodException | ClassNotFoundException | InterruptedException e) {
            System.out.println(e.getMessage() + " *** exception in run() of DirMonitor");
            throw new RuntimeException(e);
        }
    }


    public void monitorDirectory() throws IOException, InterruptedException, ClassNotFoundException, NoSuchMethodException {
        WatchService watchService = FileSystems.getDefault().newWatchService();

        Path pathToMonitor = Paths.get(this.pathOfDirToMonitor); // "/home/itay/git/build/test"
//        Path destinationPath = Paths.get("/home/itay/git/build/watcher_testing.txt");

//        if (!Files.exists(destinationPath)) {
//            Files.createFile(destinationPath);
//        }

        pathToMonitor.register(watchService,
                StandardWatchEventKinds.ENTRY_MODIFY,
                StandardWatchEventKinds.ENTRY_CREATE);
                // we don't handle JARs being deleted, only added (so no ENTRY_DELETE)

        WatchKey key;
        while ((key = watchService.take()) != null) {
            for (WatchEvent<?> event : key.pollEvents()) {
//                String logEntry = "Event kind: " + event.kind() + ". File affected: " + event.context() + ". Timestamp: " + LocalDateTime.now().format(DateTimeFormatter.ISO_LOCAL_DATE_TIME) + "\n";
//                Files.write(destinationPath, logEntry.getBytes(), StandardOpenOption.APPEND, StandardOpenOption.CREATE);
//                System.out.println("LogEntry = " + logEntry);
//                System.out.println("event.context() = " + event.context()); //TODO
                this.pluginService.handleJARDetected((Path)event.context());
            }
            key.reset();

        }
    }
}
