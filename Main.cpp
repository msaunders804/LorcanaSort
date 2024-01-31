#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

int main(int argc, char* argv[])
{
	int choice;
	do {
		DisplayMenu();
		cout << "Enter Choice Number: ";
		cin >> choice;
		while(choice > 4)
		{
			cout << "Please Enter A Valid Choice:  ";
			cin >> choice;
		}
		ExecuteChoice(choice);
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Pause to display output and wait for user input
		cout << "Press Enter to continue...";
		cin.get();
	} while (choice != 4);



}