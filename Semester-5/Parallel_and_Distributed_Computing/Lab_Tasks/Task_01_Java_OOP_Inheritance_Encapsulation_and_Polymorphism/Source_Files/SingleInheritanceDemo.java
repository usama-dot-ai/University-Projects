// Superclass
class Animal {
    // Instance variable accessible to subclasses
    String name;

    // Superclass method
    public void eat() {
        System.out.println(name + " is eating food.");
    }
}

// Subclass inheriting from Animal
class Dog extends Animal {
    // Subclass method utilizing the inherited field
    public void display() {
        System.out.println("Dog's Name: " + name);
    }
}

public class SingleInheritanceDemo {
    public static void main(String[] args) {
        // Instantiate an object of subclass Dog
        Dog myDog = new Dog();

        // Assign a value to the inherited instance variable
        myDog.name = "Tommy";

        // Invoke superclass method
        myDog.eat();

        // Invoke subclass method
        myDog.display();
    }
}