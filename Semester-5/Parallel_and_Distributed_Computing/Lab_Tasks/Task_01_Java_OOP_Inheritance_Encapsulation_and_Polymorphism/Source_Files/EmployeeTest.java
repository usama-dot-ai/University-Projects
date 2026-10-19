class Employee {
    // Private instance variables
    private String firstName;
    private String lastName;
    private int monthlySalary;

    // Parameterized Constructor
    public Employee(String firstName, String lastName, int monthlySalary) {
        this.firstName = firstName;
        this.lastName = lastName;
        // Apply validation directly or route via setter
        setMonthlySalary(monthlySalary);
    }

    // Getters and Setters
    public String getFirstName() {
        return firstName;
    }

    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }

    public String getLastName() {
        return lastName;
    }

    public void setLastName(String lastName) {
        this.lastName = lastName;
    }

    public int getMonthlySalary() {
        return monthlySalary;
    }

    // Mutator with non-negative validation rule
    public void setMonthlySalary(int monthlySalary) {
        if (monthlySalary > 0) {
            this.monthlySalary = monthlySalary;
        } else {
            this.monthlySalary = 0;
        }
    }

    // Helper method to compute annual salary
    public int getAnnualSalary() {
        return this.monthlySalary * 12;
    }
}

public class EmployeeTest {
    public static void main(String[] args) {
        // Instantiate two Employee objects
        Employee emp1 = new Employee("Asad", "Farooq", 5000);
        Employee emp2 = new Employee("Ibtehaj", "Ali", -1200); // Invalid salary, should default to 0

        // Display initial yearly salaries
        System.out.println("--- Initial Yearly Salaries ---");
        System.out.println(emp1.getFirstName() + " " + emp1.getLastName() +
                ": $" + emp1.getAnnualSalary());
        System.out.println(emp2.getFirstName() + " " + emp2.getLastName() +
                ": $" + emp2.getAnnualSalary());

        // Apply a 10% raise to both employees' monthly salaries
        emp1.setMonthlySalary((int) (emp1.getMonthlySalary() * 1.10));
        emp2.setMonthlySalary((int) (emp2.getMonthlySalary() * 1.10));

        // Display updated yearly salaries
        System.out.println("\n--- Yearly Salaries After 10% Raise ---");
        System.out.println(emp1.getFirstName() + " " + emp1.getLastName() +
                ": $" + emp1.getAnnualSalary());
        System.out.println(emp2.getFirstName() + " " + emp2.getLastName() +
                ": $" + emp2.getAnnualSalary());
    }
}