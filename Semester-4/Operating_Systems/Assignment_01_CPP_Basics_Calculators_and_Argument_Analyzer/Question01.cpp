#include <iostream>
#include <string>
#include <cstdlib> // For atoi (string to integer conversion)

int main(int argc, char *argv[])
{
    // Part 1: Validate argument count (Program name + 3 inputs = 4)
    if (argc != 4)
    {
        std::cout << "Usage: ./calc num1 operator num2" << std::endl;
        std::cout << "Example: ./calc 10 + 5" << std::endl;
        return 1; // Exit with error code
    }

    // Part 2: Convert arguments to usable types
    // argv[0] is the program name, so we start at argv[1]
    int num1 = std::atoi(argv[1]);
    std::string op = argv[2];
    int num2 = std::atoi(argv[3]);

    // Part 3: Perform calculation based on operator
    if (op == "+")
    {
        std::cout << "Result: " << (num1 + num2) << std::endl;
    }
    else if (op == "-")
    {
        std::cout << "Result: " << (num1 - num2) << std::endl;
    }
    else if (op == "x" || op == "*")
    {
        // Note: '*' is a wildcard in Linux, so 'x' is safer for testing
        std::cout << "Result: " << (num1 * num2) << std::endl;
    }
    else if (op == "/")
    {
        // Part 4: Handle division by zero
        if (num2 == 0)
        {
            std::cout << "Error: Division by zero!" << std::endl;
        }
        else
        {
            std::cout << "Result: " << (num1 / num2) << std::endl;
        }
    }
    else
    {
        std::cout << "Error: Invalid operator. Use +, -, x, /" << std::endl;
    }

    return 0;
}