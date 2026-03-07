# MidTermExam — ATM Console Application

I will be documenting each steps during this process to keep track of my work

## Overview
A C++ console-based ATM system connected to a MySQL database running in Docker. Supports two user types: **Customers** and **Administrators**.

## Project Structure
| File | Purpose |
|------|---------|
| `main.cpp` | Entry point — DB connection, login loop, role selection, menu routing |
| `Account.h` | Account class with private members, getters/setters, input validation helpers |
| `Login.h` | Login function using prepared statements |
| `CustomerOptions.h` | Customer menu — Withdraw, Deposit, Display Balance |
| `AdminOptions.h` | Admin menu — Create, Delete, Update, Search accounts |
| `schema.sql` | Database table creation and seed data |
| `.devcontainer/docker-compose.yml` | Docker setup for app + MySQL containers |
| `.devcontainer/Dockerfile` | Dev container with C++ and MySQL libraries |

## Setup
1. Open the project in VS Code with the Dev Containers extension
2. Reopen in container (it uses `docker-compose.yml`)
3. Run `schema.sql` against the MySQL database to create the `accounts` table
4. Compile and run:
   ```bash
   g++ -o atm_app main.cpp -lmysqlcppconn && ./atm_app
   ```

## Database Connection
- **Host:** `db` (Docker service name, used inside the dev container)
- **Port:** `3306`
- **User:** `root`
- **Password:** `p`
- **Database:** `ATM`

For the VS Code SQL extension (connecting from host machine), use `127.0.0.1:3307`.

## Features

### Login System
- Single login screen for both customers and admins
- Validates login and 5-digit pin against the database
- After login, user selects role (Customer or Administrator)
- Admin access is restricted to accounts with `IsAdmin = TRUE`

### Customer Options
- **1----Withdraw Cash** — Validates amount (positive, sufficient funds), updates DB, shows receipt with account #, date, amount, and balance
- **3----Deposit Cash** — Validates amount, updates DB, shows receipt
- **4----Display Balance** — Shows account #, date, and current balance
- **5----Exit** — Returns to login screen

### Admin Options
- **1----Create New Account** — Collects login, pin (validated as 5-digit integer), name, balance, status. Shows assigned account number
- **2----Delete Existing Account** — Looks up account holder, asks admin to re-enter account number to confirm deletion
- **3----Update Account Information** — Displays current account info, allows updating any field (press Enter to skip)
- **4----Search for Account** — Displays full account details by account number
- **6----Exit** — Returns to login screen

### Robustness
- Input validation helpers (`getValidInt`, `getValidDouble`) prevent crashes from non-numeric input
- Pin code validation ensures exactly 5 digits
- Duplicate login detection on account creation
- Admin accounts protected from deletion
- Encapsulation via private class members with getters/setters

## Progress

- [x] Docker environment setup (app + MySQL containers)
- [x] Database schema (`accounts` table)
- [x] Account class with encapsulation (private members, getters/setters)
- [x] Login system with prepared statements
- [x] Role selection after login (Customer / Administrator)
- [x] Customer menu (Withdraw, Deposit, Display Balance, Exit)
- [x] Admin menu (Create, Delete, Update, Search, Exit)
- [x] Input validation and error handling
- [x] Output format matching project requirements
