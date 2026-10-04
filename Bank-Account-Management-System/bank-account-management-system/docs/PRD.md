# Project Requirements Document (PRD)

# 1. Project Name
Bank Account Management System

# 2. Objective
Build a simple Linux command-line bank account application using C++ while demonstrating POSIX system programming and a basic Linux character device driver.

# 3. Users
- Customer/user: manages an account through the menu.
- Developer/student: demonstrates implementation and Linux concepts.

# 4. Functional Requirements

# FR-01 Create Account
The system shall create an account with a unique account number, customer name, PIN and opening balance.

# FR-02 Login
The system shall verify account number and PIN before allowing account operations.

# FR-03 Display Account
The system shall display the logged-in account information.

# FR-04 Deposit
The system shall add a positive deposit amount to the account balance.

# FR-05 Withdraw
The system shall allow withdrawal only when the amount is positive and does not exceed the balance.

# FR-06 Modify
The system shall allow the customer name and PIN to be changed after successful login.

# FR-07 Delete
The system shall delete an account after successful login and confirmation.

# FR-08 Display All
The system shall display all stored accounts for demonstration purposes.

# FR-09 Persistence
Account records shall remain available after the program is restarted.

# FR-10 Device Driver
The project shall include a small Linux character device driver that demonstrates open/read/write/release operations.

# 5. Non-Functional Requirements

- Language: C++17 for the application, C for the Linux module.
- Platform: Linux.
- Build tools: g++, gcc, make.
- Interface: terminal/CLI.
- Storage: local binary file.
- Code should be modular and readable.
- Errors should be handled without crashing during normal invalid input.
- File operations should use a basic advisory lock.

# 6. Modules

1. Menu and input
2. Account management
3. File storage
4. POSIX system-programming layer
5. Linux device driver
6. Testing/documentation

# 7. Deliverables

- C++ source
- Header
- Makefile
- Linux driver source and Makefile
- README
- PRD
- Architecture/UML notes
- Git history
- Test evidence
- Final report

# 8. Acceptance Criteria

The project is considered working when:
- the application compiles on Linux;
- all nine menu options work;
- account data persists after restart;
- invalid deposits/withdrawals are rejected;
- login rejects an incorrect PIN;
- the driver can be built and, on a suitable Linux environment, loaded and tested;
- documentation explains design and limitations.
