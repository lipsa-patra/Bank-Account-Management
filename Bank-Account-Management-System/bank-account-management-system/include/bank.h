#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>

struct Account {
    long accountNumber;
    char name[60];
    char pin[5];
    double balance;
};

class BankSystem {
private:
    std::string dataFile;

    std::vector<Account> readAccounts();
    bool writeAccounts(const std::vector<Account>& accounts);
    long nextAccountNumber(const std::vector<Account>& accounts) const;

public:
    explicit BankSystem(const std::string& file = "accounts.dat");

    void createAccount();
    bool login(long& accountNumber);
    void displayAccount(long accountNumber);
    void deposit(long accountNumber);
    void withdraw(long accountNumber);
    void modifyAccount(long accountNumber);
    void deleteAccount(long accountNumber);
    void displayAll();
};

#endif
