package composite;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.List;

public class Tree implements PrintableFile {
    private List<PrintableFile> tree = new ArrayList<>();

    public Tree(String absolutePathOfRootDirectory) throws FileNotFoundException {
        File file;

        //try to open file with given path
        try {
        file = new File(absolutePathOfRootDirectory); }
        catch (NullPointerException e) {
            throw new FileNotFoundException(absolutePathOfRootDirectory);
        }

        //add "myself" to the tree
        tree.add(new PrimitiveFile(absolutePathOfRootDirectory));

        //handle my sub-directories
        if (file.isDirectory()) {
            File[] subDirectories = file.listFiles();
            for (File subFile : subDirectories) {
                if (subFile.isDirectory()) {
                    tree.add(new Tree(subFile.getAbsolutePath()));
                }
                else {
                    tree.add(new PrimitiveFile(subFile.getAbsolutePath()));
                }
            }
        }
        //if root is not a directory
        else {
            throw new IllegalArgumentException();
        }
    }

    @Override
    public void print() {
        for (PrintableFile file : tree) {
            file.print();
        }
    }

    //nested PrimitiveFile class
    public static class PrimitiveFile implements PrintableFile {
        private File file;

        public PrimitiveFile(String path) {
            this.file = new File(path);
        }

        @Override
        public void print() {
            System.out.println(file);
        }
    }
}
