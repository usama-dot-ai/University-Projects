class StringReversalStack {
    private char[] stackArray;
    private int top;

    public StringReversalStack(int capacity) {
        stackArray = new char[capacity];
        top = -1;
    }

    public void push(char c) {
        stackArray[++top] = c;
    }

    public char pop() {
        return stackArray[top--];
    }

    public boolean isEmpty() {
        return top == -1;
    }
}

public class ProtocolSerializer {
    public static void main(String[] args) {
        String identifier = "USER_SESSION_849";
        int len = identifier.length();

        StringReversalStack stack = new StringReversalStack(len);

        // 1. Push all characters of the identifier onto the stack
        for (int i = 0; i < len; i++) {
            stack.push(identifier.charAt(i));
        }

        // 2. Pop all characters off the stack to form the reversed string
        String reversedIdentifier = "";
        while (!stack.isEmpty()) {
            reversedIdentifier += stack.pop();
        }

        // 3. Display original and reversed payload
        System.out.println("Original Identifier : " + identifier);
        System.out.println("Reversed for Serial : " + reversedIdentifier);
    }
}