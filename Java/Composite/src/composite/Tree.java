package composite;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class Tree implements AbstractFile {
    private List<AbstractFile> tree = new ArrayList<>();

    public Tree(String absolutePathOfRootDirectory) {
        File file = new File(absolutePathOfRootDirectory);
        tree.add(new PrimitiveFile(file));

        if (file.isDirectory()) {
            File[] subDirectories = file.listFiles();
            for (File subFile : subDirectories) {
                if (subFile.isDirectory()) {
                    tree.add(new Tree(subFile.getAbsolutePath()));
                }
                else {
                    tree.add(new PrimitiveFile(subFile));
                }
            }
        }
    }

    @Override
    public void print() {
        for (AbstractFile file : tree) {
            file.print();
        }
    }
}
