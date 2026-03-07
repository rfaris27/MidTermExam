# MidTermExam — ATM Console Application 

I will be documenting each steps during this process to keep track of my work 

## Overview
A C++ console-based ATM system connected to a MySQL database running in Docker. Supports two user types: Customers and Administrators.

Project Structure
| File | Purpose |
|------|---------|
| `main.cpp` | Entry point — DB connection, login loop, menu routing |
| `Account.h` | Account class with private members, getters/setters |
| `Login.h` | Login function using prepared statements |
| `schema.sql` | Database table creation and seed data |
| `.devcontainer/docker-compose.yml` | Docker setup for app + MySQL containers |
| `.devcontainer/Dockerfile` | Dev container with C++ and MySQL libraries |

Setup
1. Open the project in VS Code with the Dev Containers extension
2. Reopen in container (it uses `docker-compose.yml`)
3. Run `schema.sql` against the MySQL database to create the `accounts` table
4. Compile and run:
   ```bash
   g++ -o atm_app main.cpp -lmysqlcppconn && ./atm_app
   ```

Database Connection
- **Host:** `db` (Docker service name, used inside the dev container)
- **Port:** `3306`
- **User:** `root`
- **Password:** `p`
- **Database:** `ATM`

For the VS Code SQL extension (connecting from host machine), use `127.0.0.1:3307`.

 Progress

- [x] Docker environment setup (app + MySQL containers)
- [x] Database schema (`accounts` table)
- [x] Account class with encapsulation (private members, getters/setters)
- [x] Login system with prepared statements (SQL injection safe)
- [ ] Customer menu (Withdraw, Deposit, Display Balance)
- [ ] Admin menu (View, Create, Delete, Update accounts)
