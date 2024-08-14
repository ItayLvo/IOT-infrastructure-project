import static org.junit.Assert.*;
import org.junit.Test;

public class ComplexNumberTest {
    public static ComplexNumber num = null;

    @Test
    public void testGetterReal() {
        num = new ComplexNumber();

        assertEquals(0, num.getReal(), 0.001);

        num = new ComplexNumber(54.123, 32);

        assertEquals(54.123, num.getReal(), 0.001);
    }

    @Test
    public void testSetterReal() {
        num = new ComplexNumber();

        num.setReal(123.1);
        assertEquals(123.1, num.getReal() , 0.001);

        num.setReal(0);
        assertEquals(0, num.getReal() , 0.001);

        num.setReal(-2432.32);
        assertEquals(-2432.32, num.getReal() , 0.001);
    }

    @Test
    public void testGetterImaginary() {
        num = new ComplexNumber();

        assertEquals(0, num.getImaginary(), 0.001);

        num = new ComplexNumber(54.123, 32);

        assertEquals(32, num.getImaginary(), 0.001);
    }

    @Test
    public void testSetterImaginary() {
        num = new ComplexNumber();

        num.setImaginary(123.1);
        assertEquals(123.1, num.getImaginary() , 0.001);

        num.setImaginary(0);
        assertEquals(0, num.getImaginary() , 0.001);

        num.setImaginary(-2432.32);
        assertEquals(-2432.32, num.getImaginary() , 0.001);
    }

    @Test
    public void testSetValue() {
        num = new ComplexNumber();

        num.setValue(234.23, 32.2);
        assertEquals(234.23, num.getReal() , 0.001);
        assertEquals(32.2, num.getImaginary() , 0.001);


        num.setValue(0, 0);
        assertEquals(0, num.getReal() , 0.001);
        assertEquals(0, num.getImaginary() , 0.001);

        num.setValue(-31.53, -3112);
        assertEquals(-31.53, num.getReal() , 0.001);
        assertEquals(-3112, num.getImaginary() , 0.001);
    }

    @Test
    public void testIsReal() {
        num = new ComplexNumber();

        assertTrue(num.isReal());

        num.setImaginary(1);
        assertFalse(num.isReal());
    }

    @Test
    public void testIsImaginary() {
        num = new ComplexNumber();

        assertFalse(num.isImaginary());

        num.setImaginary(1);
        assertTrue(num.isImaginary());
    }

    @Test
    public void testAdd() {
        num = new ComplexNumber();

        num = num.add(new ComplexNumber(5, 21));
        assertEquals(5, num.getReal(), 0.001);
        assertEquals(21, num.getImaginary(), 0.001);

        num = num.add(new ComplexNumber());
        assertEquals(5, num.getReal(), 0.001);
        assertEquals(21, num.getImaginary(), 0.001);

        num = num.add(new ComplexNumber(-20.5, -2));
        assertEquals(-15.5, num.getReal(), 0.001);
        assertEquals(19, num.getImaginary(), 0.001);
    }

    @Test
    public void testSubtract() {
        num = new ComplexNumber();

        num = num.subtract(new ComplexNumber(5, 21));
        assertEquals(-5, num.getReal(), 0.001);
        assertEquals(-21, num.getImaginary(), 0.001);

        num = num.subtract(new ComplexNumber());
        assertEquals(-5, num.getReal(), 0.001);
        assertEquals(-21, num.getImaginary(), 0.001);

        num = num.subtract(new ComplexNumber(-20.5, -2));
        assertEquals(15.5, num.getReal(), 0.001);
        assertEquals(-19, num.getImaginary(), 0.001);
    }

    @Test
    public void testMultiply() {
        num = new ComplexNumber(2, 5);

        num = num.multiplyWith(new ComplexNumber(41, 21));
        assertEquals(-23, num.getReal(), 0.001);
        assertEquals(247, num.getImaginary(), 0.001);

        num = num.multiplyWith(new ComplexNumber(0, 0));
        assertEquals(0, num.getReal(), 0.001);
        assertEquals(0, num.getImaginary(), 0.001);
    }

    @Test
    public void testDivideBy() {
        num = new ComplexNumber(2, 3);

        num = num.divideBy(new ComplexNumber(4, 5));
        assertEquals(23f / 41f, num.getReal(), 0.001);
        assertEquals(2f / 41f, num.getImaginary(), 0.001);

    }

    @Test
    public void testEquals() {
        num = new ComplexNumber();

        assertFalse(num.equals(new Integer(1)));
        assertEquals(num, new ComplexNumber());

        num.setValue(3, 5);

        assertEquals(num, new ComplexNumber(3, 5));
    }

    @Test
    public void testHashCode() {
        num = new ComplexNumber(15, 13);
        ComplexNumber num2 = new ComplexNumber(15, 13);

        assertEquals(num.hashCode(), num2.hashCode());

        num2.setValue(1, 1);

        assertNotEquals(num.hashCode(), num2.hashCode());

    }

    @Test
    public void testCompareTo() {
        num = new ComplexNumber();
        ComplexNumber num2 = new ComplexNumber();

        assertEquals(0, num.compareTo(num2));

        num.setValue(3, 5);
        num2.setValue(5, 3);

        assertEquals(0, num.compareTo(num2));

        num.setReal(5);

        assertEquals(1, num.compareTo(num2));
        assertEquals(-1, num2.compareTo(num));
    }

    @Test
    public void testToString() {
        num = new ComplexNumber(3, 5);

        assertEquals("3.0 + 5.0i", num.toString());

        num.setValue(12.54, 564.454476);

        assertEquals("12.54 + 564.454476i", num.toString());

        num = new ComplexNumber();

        assertEquals("0.0 + 0.0i", num.toString());
    }

    @Test
    public void testParse() {
        num = ComplexNumber.parse("4.54165 + 5145.541i");
        assertEquals(4.54165, num.getReal(), 0.001);
        assertEquals(5145.541, num.getImaginary(), 0.001);

        num = ComplexNumber.parse("0 + 0i");

        assertEquals(0, num.getReal(), 0.001);
        assertEquals(0, num.getImaginary(), 0.001);
    }


}
