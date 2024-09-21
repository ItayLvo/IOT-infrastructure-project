package dynamicjarloader;

import java.io.File;
import java.io.IOException;
import java.net.URL;
import java.net.URLClassLoader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;
import java.util.jar.JarEntry;
import java.util.jar.JarFile;

public class DynamicJarLoader {


    public List<Class<?>> loadClassesFromJAR(String interfaceName, String pathOfJARFile) throws IOException, ClassNotFoundException {
        List<Class<?>> classList = new ArrayList<>();
        File jarFile = new File(pathOfJARFile);
        JarFile jar = new JarFile(jarFile);

        URL[] jarURL = {jarFile.toURI().toURL()};
        URLClassLoader classLoader = new URLClassLoader(jarURL);

        Enumeration<JarEntry> jarEntries = jar.entries();
        while (jarEntries.hasMoreElements()) {
            JarEntry jarEntry = jarEntries.nextElement();
            String className = jarEntry.getName();
            if (className.endsWith(".class")) {
                String fullyQualifiedClassName = className.replace('/', '.').substring(0, className.length() - ".class".length());
                    Class<?> clazz = classLoader.loadClass(fullyQualifiedClassName);
//                    System.out.println("Current class = " + clazz.getName());
                    Class<?>[] currentInterfaces = clazz.getInterfaces();
                    for (Class<?> inter : currentInterfaces) {
//                    System.out.println("Interface = " + inter.getName());
                        if (inter.getName().equals(interfaceName)) {
//                            System.out.println("Found interface " + inter.getName() + " in class " + clazz.getName());
                            classList.add(clazz);
                        }
                    }
            }
        }
        return classList;
    }


}
