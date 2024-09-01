import java.util.*;
import java.util.stream.Collectors;

public class Main {
    public static void main(String[] args) {
        //ex1:
        int[] arr = {5,2,3,4,1,7};
        List<Integer> list = new ArrayList<>();
        for (int i = 0; i < arr.length; ++i) {
            list.add(arr[i]);
        }
        System.out.println(Arrays.stream(arr).boxed().collect(Collectors.toList()));
        Arrays.sort(arr);
        System.out.println(Arrays.toString(arr) + "\n");
        System.out.println(list);
        list.sort(Comparator.naturalOrder());
//        Collections.sort(list);
        System.out.println(list);

        //ex2a:
        Map<String, Integer> weekMap = new LinkedHashMap<>();
        weekMap.put("Sunday", 1);
        weekMap.put("Monday", 2);
        weekMap.put("Tuesday", 3);
        weekMap.put("Wednesday", 4);
        weekMap.put("Thursday", 5);
        weekMap.put("Friday", 6);
        weekMap.put("Saturday", 7);

        Set<Map.Entry<String, Integer>> entries = weekMap.entrySet();
        for (Map.Entry<String, Integer> entry : entries) {
            System.out.println("key: " + entry.getKey() + " " + "value: "  + entry.getValue());
        }

        //ex2b:
        Collection<Integer> values = weekMap.values();
        Set<String> days = weekMap.keySet();
        for (Integer i : values)
            System.out.println(i);

        //ex3a:
        DataObject obj1 = new DataObject("First", 1);
        DataObject obj4 = new DataObject("First", 4);
        DataObject obj10 = new DataObject("First", 10);

        DataObject obj2 = new DataObject("Second", 2);
        DataObject obj5 = new DataObject("Second", 5);
        DataObject obj7 = new DataObject("Second", 7);
        DataObject obj9 = new DataObject("Second", 9);

        DataObject obj6 = new DataObject("Third", 6);
        DataObject obj8 = new DataObject("Third", 8);
        DataObject obj3 = new DataObject("Third", 3);

        DataObject[] dataObjectArr = new DataObject[]{obj1, obj2, obj3, obj4, obj5, obj6, obj7, obj8, obj9, obj10};

        Map<String, Integer> objMap = new HashMap<>();

        for (DataObject obj : dataObjectArr) {
            Integer currValue = objMap.get(obj.getCode());
            if (currValue != null) {
                objMap.put(obj.getCode(), currValue + obj.getValue());
            }
            else {
                objMap.put(obj.getCode(), obj.getValue());
            }
        }

        for (Map.Entry<String, Integer> entry : objMap.entrySet()) {
            System.out.println("key: " + entry.getKey() + " " + "value: " + entry.getValue());
        }

    }
}