# Bank-Account-Management
A Linux-based Bank Account Management System written in C++.

The project is designed as a student capstone that combines:
- C++ programming and object-oriented design
- Linux system programming (POSIX file operations and file locking)
- Linux device-driver concepts through a simple character device driver
- Git-based development
- Basic software testing and documentation

> **Note:** This is an educational project, not production banking software. PINs and account data are intentionally kept simple so the source is easy to understand.

# Menu / Expected Output

   text
========================================
       BANK ACCOUNT MANAGEMENT SYSTEM
========================================
1. Create New Account
2. Login Account
3. Display Account
4. Deposit Money   
5. Withdraw Money
6. Modify Account
7. Delete Account
8. Display All Accounts
9. Exit
----------------------------------------
Enter your choice:

# Project Structure

   text
bank-account-management-system/
├── README.md
├── Makefile
├── include/
│   └── bank.h
├── src/
│   ├── bank.cpp
│   └── main.cpp
├── driver/
│   ├── bank_device.c
│   └── Makefile
└── docs/
    ├── PRD.md
    └── architecture.md


`accounts.dat` is created automatically when the program first writes account data. It is not kept in the source repository.


# Stage 1 – Project Introduction

# 1.1 Project Idea

The Bank Account Management System is a command-line application for creating and managing simple bank accounts. A user can create an account, log in with an account number and PIN, view account information, deposit or withdraw money, modify account details, delete an account, and display all accounts.

The project is intentionally kept small enough to understand and demonstrate in a viva while still showing Linux and C++ concepts.

# 1.2 Problem to Be Solved

Manual handling of account records can lead to duplicate records, incorrect balance updates, and difficulty finding account information. This project provides a small computer-based system for storing and updating account records.

# 1.3 Scope

# Included
- Account creation
- Account login
- Account display
- Deposit
- Withdrawal
- Account modification
- Account deletion
- Display of all accounts
- Persistent local file storage
- Basic input validation
- Linux file locking
- A simple Linux character-device driver prototype
 # Not Included
- Real banking transactions
- Internet banking
- UPI/card integration
- Database server
- Network authentication
- Real financial security

# 1.4 Expected Outcome

A working Linux command-line program that stores account information in a local data file and demonstrates C++ programming, POSIX system calls, file locking, and basic Linux device-driver concepts.

---

# Stage 2 – Project Requirements & Development Plan

The detailed Project Requirements Document is available in `docs/PRD.md`.

# Functional Requirements

1. Create a new account.
2. Generate a unique account number.
3. Login using account number and PIN.
4. Display account details.
5. Deposit money.
6. Withdraw money only when sufficient balance exists.
7. Modify account name and PIN.
8. Delete an account after confirmation.
9. Display all accounts.
10. Exit without losing saved data.

# Non-Functional Requirements

- Simple command-line interface.
- C++17 compatible code.
- Linux/POSIX environment.
- Clear error messages.
- Local persistent storage.
- Basic file locking to reduce simultaneous-write problems.
- Modular source code.
- Easy compilation and demonstration.

# Development Timeline

| Stage | Work | Evidence |
|---|---|---|
| 1 | Idea, scope, requirements | README + PRD |
| 2 | Modules and development plan | PRD + Git commits |
| 3 | Architecture and UML | architecture.md |
| 4 | Core C++ implementation | working prototype |
| 5 | Testing and debugging | test notes + commits |
| 6 | Final demo and report | final source + presentation |

---

# Stage 3 – System Design & Architecture

The architecture and UML diagrams are documented in `docs/architecture.md`.

# Main Modules

- **Menu Module:** accepts user choices.
- **Account Module:** performs account operations.
- **Storage Module:** reads/writes account records.
- **Linux System Programming Layer:** uses POSIX file APIs and file locking.
- **Device Driver Prototype:** exposes a simple `/dev/bank_device` character device.

# Data Structure

Each account contains:
- account number
- customer name
- PIN
- balance

# Development Environment

Recommended:
- Ubuntu 22.04/24.04 or another Linux distribution
- g++
- make
- gcc
- Linux kernel headers
- git

VS Code can be used as the editor.

### WSL note

The C++ application can be developed and run in WSL. Loading a custom kernel module depends on the WSL kernel configuration and is not guaranteed. For the actual driver demonstration, use a native Linux installation or a Linux virtual machine if WSL does not allow module loading.

---

# Stage 4 – Initial Implementation & Prototype

## Build the C++ application

```bash
make
```

Run:

```bash
./bank_app
```

The first run creates the local `accounts.dat` file after an account is saved.

## Clean the build

```bash
make clean
```

## Example Demonstration

```text
1. Create New Account
Enter customer name: Rahul
Enter 4 digit PIN: 1234
Enter opening balance: 5000

Account created successfully.
Account Number: 1001
```

Then use option 2 to login.

---

# Linux Device Driver Prototype

The driver is a small character device used only to demonstrate the Linux device-driver part of the project. It accepts text written by a user program and returns the stored text when read.

Build it from the `driver` directory:

```bash
cd driver
make
```

Load it on a Linux system that permits external modules:

```bash
sudo insmod bank_device.ko
dmesg | tail
```

Create the device node using the major number printed by `dmesg`:

```bash
sudo mknod /dev/bank_device c <MAJOR_NUMBER> 0
sudo chmod 666 /dev/bank_device
```

Test:

```bash
echo "Bank driver test" > /dev/bank_device
cat /dev/bank_device
```

Remove it:

```bash
sudo rm /dev/bank_device
sudo rmmod bank_device
```

> Do not load an unfamiliar kernel module on a system you depend on. This driver is intentionally tiny and educational.

---

# Stage 5 – Testing, Integration & Improvement

# Basic Test Cases

| Test | Input | Expected Result |
|---|---|---|
| Create account | Valid name/PIN/balance | Account created |
| Duplicate login | Wrong PIN | Login rejected |
| Deposit | Positive amount | Balance increases |
| Withdraw | Amount <= balance | Balance decreases |
| Withdraw | Amount > balance | Transaction rejected |
| Modify | New name/PIN | Details updated |
| Delete | Existing account | Account removed |
| Display all | Option 8 | All saved accounts shown |
| Invalid menu | 0/10/etc. | Error message |
| Driver read/write | Text to device | Same text can be read |

# Improvement Work

During this stage:
- fix input and file errors
- test each menu option
- test multiple accounts
- check invalid amounts and PINs
- verify data remains after restarting the program
- test the driver separately
- update Git commits and documentation

---

# Stage 6 – Final Implementation & Presentation

The final demonstration should show:

1. Project objective
2. Folder structure
3. System architecture
4. C++ classes/functions
5. Linux system calls used
6. Account creation
7. Login
8. Deposit and withdrawal
9. Modify/delete
10. Persistent data after restarting
11. Device-driver demonstration
12. Test results
13. Limitations
14. Future improvements

## Final Deliverables

- Source code
- README
- PRD
- Architecture/UML documentation
- Git repository
- Test evidence
- Project report
- Final demonstration

## Limitations

- Local file storage is not a real banking database.
- PIN storage is only for learning and is not production-secure.
- No network communication.
- No real bank/payment integration.
- The driver is a demonstration driver, not a real banking hardware driver.

## Future Improvements

- SQLite/PostgreSQL database
- Password/PIN hashing
- Transaction history
- Admin role
- Better audit logging
- REST API
- GUI/web interface
- Unit-test framework
- Better driver-to-application communication

---

# Progress Evidence and Git Plan

Make commits after meaningful work instead of one huge final commit.

Example:

```bash
git init
git add README.md docs/PRD.md
git commit -m "stage 1 add project introduction and requirements"

git add include src
git commit -m "stage 4 implement core account management"

git add driver
git commit -m "stage 4 add linux character device prototype"

git add docs
git commit -m "stage 5 add testing and architecture documentation"
```

Suggested branch names:

   ``text
main
dev
feature/account-management
feature/linux-driver


# Project Rule

Keep the implementation understandable. During the presentation, be able to explain every important function, system call, data field, and driver operation in your own words.
