# System Design & Architecture

# 1. High-Level Architecture

flowchart TD
    U[User / Terminal] --> M[Menu Layer]
    M --> A[Account Manager]
    A --> S[Storage Layer]
    S --> F[(accounts.dat)]
    A --> P[POSIX System Calls]
    P --> F
    U --> D[/dev/bank_device/]
    D --> K[Linux Character Driver]
    K --> L[Linux Kernel]


The application and the device-driver demonstration are separate components. The application uses POSIX file APIs for persistent account storage. The driver demonstrates the Linux device-driver interface.

# 2. Responsibilities

| Component | Responsibility |
|---|---|
| `main.cpp` | menu and program entry point |
| `bank.h` | account structure and class declarations |
| `bank.cpp` | account operations and file handling |
| `accounts.dat` | persistent local account records |
| `bank_device.c` | Linux character-device driver |
| `Makefile` | compile the application |
| `driver/Makefile` | compile the kernel module |

# 3. Class Diagram

classDiagram
    class Account {
        +long accountNumber
        +char name[60]
        +char pin[5]
        +double balance
    }

    class BankSystem {
        -string dataFile
        +createAccount()
        +login()
        +displayAccount()
        +deposit()
        +withdraw()
        +modifyAccount()
        +deleteAccount()
        +displayAll()
    }

    BankSystem --> Account


# 4. Sequence Diagram – Deposit

sequenceDiagram
    actor User
    participant Menu
    participant BankSystem
    participant File as accounts.dat

    User->>Menu: Select Deposit
    Menu->>BankSystem: deposit(account, amount)
    BankSystem->>File: lock + read records
    File-->>BankSystem: account record
    BankSystem->>BankSystem: balance += amount
    BankSystem->>File: write updated record
    BankSystem->>File: unlock
    BankSystem-->>Menu: success
    Menu-->>User: show new balance


# 5. State Machine

stateDiagram-v2
    [*] --> MainMenu
    MainMenu --> CreateAccount
    MainMenu --> Login
    MainMenu --> DisplayAll
    MainMenu --> [*]: Exit

    Login --> LoggedIn: valid PIN
    Login --> MainMenu: invalid PIN

    LoggedIn --> DisplayAccount
    LoggedIn --> Deposit
    LoggedIn --> Withdraw
    LoggedIn --> Modify
    LoggedIn --> Delete

    DisplayAccount --> LoggedIn
    Deposit --> LoggedIn
    Withdraw --> LoggedIn
    Modify --> LoggedIn
    Delete --> MainMenu

# 6. Implementation Plan

# Step 1
Create the account structure and menu.

# Step 2
Implement file-based persistence using POSIX APIs.

# Step 3
Implement create/login/display/deposit/withdraw.

# Step 4
Implement modify/delete/display-all.

# Step 5
Add input validation and file locking.

# Step 6
Build and test the Linux character driver.

# Step 7
Perform integration testing and document results.

# 7. Linux System Programming Concepts

The C++ application demonstrates:
- `open()`
- `read()`
- `write()`
- `close()`
- `lseek()`
- `flock()`

The driver demonstrates:
- module initialization/cleanup
- character device registration
- file operations
- kernel/user-space boundary

# 8. Git Strategy

```text
main
  |
  +-- dev
       |
       +-- feature/account-management
       +-- feature/linux-driver
       +-- feature/testing

Use small meaningful commits and merge completed features into `dev` before the final `main` branch.
