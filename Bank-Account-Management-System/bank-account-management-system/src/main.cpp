#include "bank.h"
#include <iostream>
#include <limits>

void showMenu() {
    std::cout << "\n========================================\n";
    std::cout << "       BANK ACCOUNT MANAGEMENT SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "1. Create New Account\n";
    std::cout << "2. Login Account\n";
    std::cout << "3. Display Account\n";
    std::cout << "4. Deposit Money\n";
    std::cout << "5. Withdraw Money\n";
    std::cout << "6. Modify Account\n";
    std::cout << "7. Delete Account\n";
    std::cout << "8. Display All Accounts\n";
    std::cout << "9. Exit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "Enter your choice: ";
}

int main() {
    BankSystem bank;
    long loggedInAccount = -1;
    bool loggedIn = false;

    while (true) {
        showMenu();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number from 1 to 9.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                bank.createAccount();
                break;

            case 2:
                loggedIn = bank.login(loggedInAccount);
                break;

            case 3:
                if (loggedIn)
                    bank.displayAccount(loggedInAccount);
                else
                    std::cout << "Please login first.\n";
                break;

            case 4:
                if (loggedIn)
                    bank.deposit(loggedInAccount);
                else
                    std::cout << "Please login first.\n";
                break;

            case 5:
                if (loggedIn)
                    bank.withdraw(loggedInAccount);
                else
                    std::cout << "Please login first.\n";
                break;

            case 6:
                if (loggedIn)
                    bank.modifyAccount(loggedInAccount);
                else
                    std::cout << "Please login first.\n";
                break;

            case 7:
                if (loggedIn) {
                    bank.deleteAccount(loggedInAccount);
                    loggedIn = false;
                    loggedInAccount = -1;
                } else {
                    std::cout << "Please login first.\n";
                }
                break;

            case 8:
                bank.displayAll();
                break;

            case 9:
                std::cout << "Thank you for using the Bank Account Management System.\n";
                return 0;

            default:
                std::cout << "Invalid choice. Select 1 to 9.\n";
        }
    }
}
