package composite;

import java.io.File;

public class PrimitiveFile implements AbstractFile {
    private File file;
    
    public PrimitiveFile(File file) {
        this.file = file;
    }
    
    @Override
    public void print() {
        System.out.println(file);
    }
}
