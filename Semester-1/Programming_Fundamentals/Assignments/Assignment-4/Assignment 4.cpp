#include<iostream>
#include<fstream>
#include<string>
using namespace std;

// function prototypes
void countWords();
void copyContents();
void longestWord();
void countLines();
void replacingWord();
void contentReversing();
void sortNumber();
void countVowels();
void findSpecificLine();
void mergeFiles();

int main() // main function
{
	int choice;
	do 
	{
        cout << "\nMenu:" << endl;
        cout << "1. Count Words" << endl;
        cout << "2. Copy Contents" << endl;
        cout << "3. Find Longest Word" << endl;
        cout << "4. Count Lines" << endl;
        cout << "5. Replace Word" << endl;
        cout << "6. Reverse File Contents" << endl;
        cout << "7. Sort Numbers" << endl;
        cout << "8. Count Vowels" << endl;
        cout << "9. Find Specific Line" << endl;
        cout << "10. Merge Files" << endl;
        cout << "0. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) // handling user choice 
		{
            case 1: countWords(); // couting words
			break;
            case 2: copyContents(); // copying contents
			break;
            case 3: longestWord(); // longest word
			break;
            case 4: countLines(); // counting lines
			break;
            case 5: replacingWord(); // replacing word
			break;
            case 6: contentReversing(); // reversing the contents
			break;
            case 7: sortNumber(); // sorting the numbers
			break;
            case 8: countVowels(); // counting vowels
			break;
            case 9: findSpecificLine(); // finding desired line
			break;
            case 10: mergeFiles(); // merging two files
			break;
            case 0: cout << "Exiting..." << endl; // exiting the program
			break;
            default: cout << "Invalid choice. Please try again." << endl; // validating choice 
			break;
        }
    } 
	while (choice != 0); // condition for validating
	
	return 0;
}

void countWords() // function to count words in file
{
	ifstream infile("Assignment_4.txt"); // reding from file
	
	if(!infile) // handling error
	{
		cout<<"An error occured in file opening. "<<endl;
		return;
	}
	
	string word;
	int word_count = 0;
	
	while(infile >> word) // counting words
	{
		word_count++;
	}
	
	// displaying output
	cout<<"\nTotal number of words in file 'Assignment_4' are: "<< word_count << endl;	
	infile.close(); // file closing
}

void copyContents() // function to copy contents
{
	ifstream infile("Assignment_4.txt"); // reading from file
	
	if(!infile) // handle file opening error
	{
		cout<<"An error occured in file opening. "<<endl;
		return;
	}
	
	ofstream outfile ("Copied From Assignment.txt"); // writing to new file
	
	string word;
	
	while(getline(infile,word)) // writing in file
	{
		outfile << word << endl;
	}
	
	// displaying the message
	cout << "\nText copied from 'Assignment_4' to 'Copied From Assignment'. " << endl;
	infile.close();
	outfile.close();	// closing both files
}

void longestWord() // function to find longest word
{
	ifstream infile("Assignment_4.txt"); // reading from file
	
	if(!infile) // handle file opening error
	{
		cout<<"An error occured in file opening. "<<endl;
		return;
	}
	
	string word;
	string longest_word;
	int word_count = 0;
	
	while(infile >> word) // compare the words
	{
		if (word.length() > longest_word.length())
		{
			longest_word = word;
		}
		word_count++;
	}
	
	// display the output
	cout<<"The Longest Word in the File 'Assignment_4' is "<< longest_word << "." << endl;	
	infile.close(); // file closing
}

void countLines() // function to count lines
{
	ifstream infile ("Assignment_4.txt"); // reding from file
	
	if (!infile) // handle file opening error
	{
		cout << "An error occured in file opening. " << endl;
		return;
	}
	
	string line;
	int line_count = 0;
	
	while (getline(infile, line)) // counting lines in file
	{
		line_count ++;
	}
	
	// dislpay output
	cout << "\nTotal Lines in the File 'Assignment_4' are: " << line_count << endl;
	infile.close(); // closing file
}

void replacingWord() // function to replace word
{
	string word_replace, replacement_word; // local variables
	
	cout << "Enter a Word to Replace: "; // taking input from user
	cin >> word_replace;
	cout << "Enter a Replacement Word: ";
	cin >> replacement_word;
	
	ifstream infile ("Assignment_4.txt"); // reading from file
	
	if (!infile) // handle file opening error
	{
		cout << "An error occured in file opening. " << endl;
		return;
	}
	
	string content;
    string line;
    
	while (getline(infile, line)) // replacing the words
	{
        size_t pos = 0;
        while ((pos = line.find(word_replace, pos)) != string::npos) 
		{
            line.replace(pos, word_replace.length(), replacement_word);
            pos += replacement_word.length();
        }
        content += line + "\n";
    }
    infile.close(); // closing file

    ofstream outfile("Assignment_4.txt"); // writing to a file
    outfile << content;
    outfile.close(); // closing file
    
    //display output
    cout << "\nWord replaced successfully in file 'Assignment_4'." << endl;
}

void contentReversing() // function to reverse content of file
{
	ifstream infile("Assignment_4.txt"); // reding from file
	if (!infile) // handle file opening error
	{
		cout << "An error occured in file opening. " << endl;
		return;
	}
	
	string content;
	string line;
	
	while(getline(infile, line)) // reading
	{
		content = line + "\n" + content;
	}
	infile.close(); // close file
	
	ofstream outfile ("Reversed_Assignment_4.txt"); // write to file
	if(!outfile) // handle file opening error
	{
		cout << "An error occured in file opening. " << endl;
		return;
	}
	
	outfile << content; // write in file
	outfile.close(); // file close
	
	// display output
	cout << "\nContents of file 'Assignment_4' reversed Successfully in 'Reversed_Assignment_4'. " << endl;
}

void sortNumber() // function to sort numbers
{
	ifstream infile ("Assignment_4Num.txt"); // reding from file
	if(!infile) // handle file opening error
	{
		cout << "An error occured in file opening. " << endl;
		return;
	}
	int numbers[100]; // Assuming a maximum of 100 numbers
    int count = 0;

    while (infile >> numbers[count]) // reading number from file
	{
        count++;
    }
    infile.close(); // file close

    // Simple bubble sort
    for (int i = 0; i < count - 1; i++) 
	{
        for (int j = 0; j < count - i -  1; j++) 
		{
            if (numbers[j] > numbers[j + 1]) 
			{
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j + 1] = temp;
            }
        }
    }

    ofstream outfile("Sorted_Numbers.txt"); // writing to file
    if (!outfile) // handle file opening error
	{
        cout << "An error occurred in file opening." << endl;
        return;
    }

    for (int i = 0; i < count; i++) // storing in file
	{
        outfile << numbers[i] << endl;
    }
    outfile.close(); // file closed
    
    // display output
    cout << "\nNumbers from file 'Assignment_4Num' sorted successfully in 'Sorted_Numbers'." << endl;
}

void countVowels() // function to count vowels
{
    ifstream infile("Assignment_4.txt"); // reading from file
    if (!infile) // handle file opening error
	{
        cout << "An error occurred in file opening." << endl;
        return;
    }

    char vowels[] = {'a', 'e', 'i', 'o', 'u'};
    int vowel_count[5] = {0}; // Count for each vowel

    char ch;
    while (infile.get(ch)) // comparing the words
	{
        ch = tolower(ch); // Convert to lowercase for comparison
        for (int i = 0; i < 5; i++) 
		{
            if (ch == vowels[i]) 
			{
                vowel_count[i]++;
            }
        }
    }
    infile.close(); // file closed
	
	// display output
    cout << "\nFile 'Assignment_4' \nVowel counts:" << endl;
    for (int i = 0; i < 5; i++)  
	{
        cout << vowels[i] << ": " << vowel_count[i] << endl;
    }
}

void findSpecificLine() // function to find specific line
{
    int n; // taking line number from user
    cout << "Enter the line number to retrieve: ";
    cin >> n;

    ifstream infile("Assignment_4.txt"); // reding from file
    if (!infile) // handle file opening error
	{
        cout << "An error occurred in file opening." << endl;
        return;
    }

    string line;
    int line_number = 0;
    
	while (getline(infile, line)) // finding the specific line
	{
        line_number++;
        if (line_number == n) 
		{
            cout << "\nFile 'Assignment_4'. \nLine " << n << ": " << line << endl;
            infile.close(); // file close
            return;
        }
    }
    infile.close(); // file close
    
    // display error
    cout << "\nLine " << n << " does not exist." << endl;
}

void mergeFiles() // function to merge files
{
    ifstream infile1("Assignment_4.txt"); // reding from file
    ifstream infile2("Assignment_4Num.txt");
    ofstream outfile("Merged_File.txt"); // writing to file

    if (!infile1 || !infile2 || !outfile) // handle file opening error
	{
        cout << "An error occurred in file opening." << endl;
        return;
    }

    string line;
    
    // writing to file
	while (getline(infile1, line)) 
	{
        outfile << line << endl;
    }
    while (getline(infile2, line)) 
	{
        outfile << line << endl;
    }
    
	// closing files
    infile1.close();
    infile2.close();
    outfile.close();
    
    // display output
    cout << "\nFiles 'Assignment_4' and 'Assignment_4Num' are merged successfully." << endl;
}
