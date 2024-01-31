#include <C:\Users\User\source\repos\Lorcana\Lorcana\Lorcana.h>

void DisplayMenu()
{
    cout << "===== Menu =====" << std::endl;
    cout << "1. Look at my picture" << std::endl;
    cout << "2. ADMIN ONLY --> Upload new hashes" << std::endl;
    cout << "3. Option 3" << std::endl;
    cout << "4. Exit" << std::endl;
    cout << "================" << std::endl;
}

void ExecuteChoice(int choice)
{
    switch (choice) {
    case 1:
        //Run Image manipulation
        TakeImage();
        break;
    case 2:
        PerceptHashForDB();
        AddtoDB();
        break;
    }
}