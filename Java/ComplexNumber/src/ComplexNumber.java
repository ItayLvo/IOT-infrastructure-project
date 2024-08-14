import java.util.Objects;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class ComplexNumber implements Comparable<ComplexNumber> {

    private double real;
    private double imaginary;

    // Constructors

    // default ctor: initial the real and imaginary to 0
    public ComplexNumber() {
        this.real = 0.0;
        this.imaginary = 0.0;
    }


    public ComplexNumber(double real, double imaginary) {
        this.real = real;
        this.imaginary = imaginary;
    }

    // Getters and setters
    public double getReal() {
        return this.real;
    }

    public void setReal(double real) {
        this.real = real;
    }

    public double getImaginary() {
        return this.imaginary;
    }

    public void setImaginary(double imaginary) {
        this.imaginary = imaginary;
    }

    // Set the complex number
    public void setValue(double real, double imaginary) {
        this.real = real;
        this.imaginary = imaginary;
    }

    // Check if the object is real
    public boolean isReal() {
        Double thisImaginaryAutoBox = this.imaginary;
        return thisImaginaryAutoBox.equals(0.0);
    }

    // Check if the object is imaginary
    public boolean isImaginary() {
        return (!this.isReal());
    }

    // Add between 2 complex numbers
    public ComplexNumber add(ComplexNumber complexNum) {
        double newReal = this.real + complexNum.real;
        double newImaginary = this.imaginary + complexNum.imaginary;
        return new ComplexNumber(newReal, newImaginary);
    }

    // Subtract between 2 complex numbers
    public ComplexNumber subtract(ComplexNumber complexNumb) {
        double newReal = this.real - complexNumb.real;
        double newImaginary = this.imaginary - complexNumb.imaginary;
        return new ComplexNumber(newReal, newImaginary);
    }

    // Multiply two complex number
    public ComplexNumber multiplyWith(ComplexNumber complexNum) {
        double newReal = this.real * complexNum.real - this.imaginary * complexNum.imaginary;
        double newImaginary = this.real * complexNum.imaginary + this.imaginary * complexNum.real;
        return new ComplexNumber(newReal, newImaginary);
    }

    // Divide two complex number - if divide by 0 is undefined
    public ComplexNumber divideBy(ComplexNumber complexNum) {
        double denominator = complexNum.real * complexNum.real + complexNum.imaginary * complexNum.imaginary;
        double newReal = (this.real * complexNum.real + this.imaginary * complexNum.imaginary) / denominator;
        double newImaginary = (this.imaginary * complexNum.real - this.real * complexNum.imaginary) / denominator;
        return new ComplexNumber(newReal, newImaginary);
    }

    // Parse string to complex number. pattern: x + yi
    public static ComplexNumber parse(String expression) {

        //remove whitespaces from start and end of string
        expression = expression.trim();

        // regex pattern for "x+yi" format
        String regex = "(-?\\d+\\.?\\d*) [+] ([+-]?\\d+\\.?\\d*)i";
        Pattern pattern = Pattern.compile(regex);
        Matcher matcher = pattern.matcher(expression);

        if (matcher.matches()) {
            String realPart = matcher.group(1);
            String imaginaryPart = matcher.group(2);

            double real = Double.parseDouble(realPart);
            double imaginary = Double.parseDouble(imaginaryPart);

            return new ComplexNumber(real, imaginary);
        }
        else {
            System.out.println("error");
        }
        return null;
    }

    // Override equals to compare between 2 complex numbers
    @Override
    public boolean equals(Object o) {

        if (this == o) {
            return true;
        }

        if (!(o instanceof ComplexNumber)) {
            return false;
        }

        ComplexNumber other = (ComplexNumber) o;
        Double thisRealAutoBox = this.real;
        Double thisImaginaryAutoBox = this.imaginary;
        return (thisRealAutoBox.equals(other.real) && thisImaginaryAutoBox.equals(other.imaginary));
    }



    // Override hashCode to comply with our new equals implementation
    @Override
    public int hashCode() {
        return Objects.hash(real, imaginary);
    }

    //previous implementation using Object.hasCode():
    //    @Override
//    public int hashCode() {
//        Double thisRealAutoBox = this.real;
//        Double thisImaginaryAutoBox = this.imaginary;
//        int result = 31;
//        result = 31 * result + thisRealAutoBox.hashCode();
//        result = 31 * result + thisImaginaryAutoBox.hashCode();
//        return result;
//    }


    @Override
    public int compareTo(ComplexNumber num) {
        double thisDistance = Math.sqrt(this.real * this.real + this.imaginary * this.imaginary);
        double otherDistance = Math.sqrt(num.real * num.real + num.imaginary * num.imaginary);

        return Double.compare(thisDistance, otherDistance);
    }

    @Override
    public String toString() {
        return this.real + " + " + this.imaginary + "i";
    }
}
