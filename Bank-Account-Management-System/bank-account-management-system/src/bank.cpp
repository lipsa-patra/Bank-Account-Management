#include "bank.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace {
bool validPin(const std::string& pin) {
    if (pin.size() != 4) return false;
    for (char c : pin) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

bool validName(const std::string& name) {
    return !name.empty() && name.size() < 60;
}

double readPositiveAmount(const std::string& prompt) {
    double amount;
    while (true) {
        std::cout << prompt;
        if (std::cin >> amount && amount > 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return amount;
        }
        std::cout << "Please enter a positive number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}
}

BankSystem::BankSystem(const std::string& file) : dataFile(file) {}

std::vector<Account> BankSystem::readAccounts() {
    std::vector<Account> accounts;

    int fd = open(dataFile.c_str(), O_RDONLY | O_CREAT, 0600);
    if (fd == -1) {
        std::perror("open");
        return accounts;
    }


    Account account{};
    ssize_t bytesRead;
    while ((bytesRead = read(fd, &account, sizeof(Account))) == sizeof(Account)) {
        accounts.push_back(account);
    }

    close(fd);
    return accounts;
}

bool BankSystem::writeAccounts(const std::vector<Account>& accounts) {
    int fd = open(dataFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        std::perror("open");
        return false;
    }


    for (const Account& account : accounts) {
        const char* data = reinterpret_cast<const char*>(&account);
        size_t remaining = sizeof(Account);
        while (remaining > 0) {
            ssize_t written = write(fd, data, remaining);
            if (written <= 0) {
                std::perror("write");
                close(fd);
                return false;
            }
            data += written;
            remaining -= static_cast<size_t>(written);
        }
    }


    close(fd);
    return true;
}

long BankSystem::nextAccountNumber(const std::vector<Account>& accounts) const {
    long number = 1000;
    for (const auto& account : accounts) {
        if (account.accountNumber >= number) {
            number = account.accountNumber + 1;
        }
    }
    return number;
}

void BankSystem::createAccount() {
    auto accounts = readAccounts();

    std::string name = readLine("Enter customer name: ");
    while (!validName(name)) {
        std::cout << "Name cannot be empty and must be shorter than 60 characters.\n";
        name = readLine("Enter customer name: ");
    }

    std::string pin = readLine("Enter 4 digit PIN: ");
    while (!validPin(pin)) {
        std::cout << "PIN must contain exactly 4 digits.\n";
        pin = readLine("Enter 4 digit PIN: ");
    }

    double balance = readPositiveAmount("Enter opening balance: ");

    Account account{};
    account.accountNumber = nextAccountNumber(accounts);
    std::strncpy(account.name, name.c_str(), sizeof(account.name) - 1);
    std::strncpy(account.pin, pin.c_str(), sizeof(account.pin) - 1);
    account.balance = balance;

    accounts.push_back(account);

    if (writeAccounts(accounts)) {
        std::cout << "\nAccount created successfully.\n";
        std::cout << "Account Number: " << account.accountNumber << "\n";
    } else {
        std::cout << "Could not save the account.\n";
    }
}

bool BankSystem::login(long& accountNumber) {
    std::cout << "\n--- Login ---\n";
    std::cout << "Account number: ";

    if (!(std::cin >> accountNumber)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid account number.\n";
        return false;
    }

    std::string pin;
    std::cout << "PIN: ";
    std::cin >> pin;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    auto accounts = readAccounts();
    for (const auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            if (pin == account.pin) {
                std::cout << "Login successful.\n";
                return true;
            }
            break;
        }
    }

    std::cout << "Invalid account number or PIN.\n";
    return false;
}

void BankSystem::displayAccount(long accountNumber) {
    auto accounts = readAccounts();

    for (const auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            std::cout << "\n--- Account Details ---\n";
            std::cout << "Account Number : " << account.accountNumber << "\n";
            std::cout << "Name           : " << account.name << "\n";
            std::cout << "Balance        : Rs. " << std::fixed << std::setprecision(2)
                      << account.balance << "\n";
            return;
        }
    }
    std::cout << "Account not found.\n";
}

void BankSystem::deposit(long accountNumber) {
    auto accounts = readAccounts();
    double amount = readPositiveAmount("Enter deposit amount: ");

    for (auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            account.balance += amount;
            if (writeAccounts(accounts)) {
                std::cout << "Deposit successful. New balance: Rs. "
                          << std::fixed << std::setprecision(2)
                          << account.balance << "\n";
            }
            return;
        }
    }
    std::cout << "Account not found.\n";
}

void BankSystem::withdraw(long accountNumber) {
    auto accounts = readAccounts();
    double amount = readPositiveAmount("Enter withdrawal amount: ");

    for (auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            if (amount > account.balance) {
                std::cout << "Insufficient balance.\n";
                return;
            }

            account.balance -= amount;
            if (writeAccounts(accounts)) {
                std::cout << "Withdrawal successful. New balance: Rs. "
                          << std::fixed << std::setprecision(2)
                          << account.balance << "\n";
            }
            return;
        }
    }
    std::cout << "Account not found.\n";
}

void BankSystem::modifyAccount(long accountNumber) {
    auto accounts = readAccounts();

    for (auto& account : accounts) {
        if (account.accountNumber == accountNumber) {
            std::string name = readLine("Enter new name: ");
            while (!validName(name)) {
                std::cout << "Invalid name.\n";
                name = readLine("Enter new name: ");
            }

            std::string pin = readLine("Enter new 4 digit PIN: ");
            while (!validPin(pin)) {
                std::cout << "PIN must contain exactly 4 digits.\n";
                pin = readLine("Enter new 4 digit PIN: ");
            }

            std::strncpy(account.name, name.c_str(), sizeof(account.name) - 1);
            account.name[sizeof(account.name) - 1] = '\0';
            std::strncpy(account.pin, pin.c_str(), sizeof(account.pin) - 1);
            account.pin[sizeof(account.pin) - 1] = '\0';

            if (writeAccounts(accounts)) {
                std::cout << "Account modified successfully.\n";
            }
            return;
        }
    }

    std::cout << "Account not found.\n";
}

void BankSystem::deleteAccount(long accountNumber) {
    auto accounts = readAccounts();

    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->accountNumber == accountNumber) {
            std::string confirm = readLine("Delete this account? (y/n): ");
            if (confirm == "y" || confirm == "Y") {
                accounts.erase(it);
                if (writeAccounts(accounts)) {
                    std::cout << "Account deleted successfully.\n";
                }
            } else {
                std::cout << "Delete cancelled.\n";
            }
            return;
        }
    }

    std::cout << "Account not found.\n";
}

void BankSystem::displayAll() {
    auto accounts = readAccounts();

    if (accounts.empty()) {
        std::cout << "No accounts found.\n";
        return;
    }

    std::cout << "\n"
              << std::left << std::setw(15) << "Account"
              << std::setw(25) << "Name"
              << "Balance\n";
    std::cout << "--------------------------------------------------------\n";

    for (const auto& account : accounts) {
        std::cout << std::left << std::setw(15) << account.accountNumber
                  << std::setw(25) << account.name
                  << "Rs. " << std::fixed << std::setprecision(2)
                  << account.balance << "\n";
    }
}
