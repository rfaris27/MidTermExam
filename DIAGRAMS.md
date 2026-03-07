# ATM System Diagrams

## 1. Use Case Diagram

### Actors
- **Customer** — A user with a bank account who can perform transactions
- **Administrator** — A privileged user who manages customer accounts

```mermaid
graph TD
    subgraph ATM System
        UC1[Login]
        UC2[Withdraw Cash]
        UC3[Deposit Cash]
        UC4[Display Balance]
        UC5[Create New Account]
        UC6[Delete Existing Account]
        UC7[Update Account Information]
        UC8[Search for Account]
        UC9[Select Role]
        UC10[Logout / Exit]
    end

    Customer((Customer))
    Admin((Administrator))

    Customer --> UC1
    Customer --> UC9
    Customer --> UC2
    Customer --> UC3
    Customer --> UC4
    Customer --> UC10

    Admin --> UC1
    Admin --> UC9
    Admin --> UC5
    Admin --> UC6
    Admin --> UC7
    Admin --> UC8
    Admin --> UC10
```

### Use Case Descriptions

| Use Case | Actor(s) | Description |
|----------|----------|-------------|
| Login | Customer, Admin | User enters login and 5-digit pin. System verifies credentials against the database. |
| Select Role | Customer, Admin | After login, user selects Customer or Administrator role. Access is validated. |
| Withdraw Cash | Customer | Customer enters amount to withdraw. System validates (positive, sufficient funds), updates balance in DB, displays receipt. |
| Deposit Cash | Customer | Customer enters amount to deposit. System validates (positive), updates balance in DB, displays receipt. |
| Display Balance | Customer | System displays account number, current date, and balance. |
| Create New Account | Admin | Admin enters login, pin, name, balance, status. System validates pin (5 digits), checks for duplicate login, creates account. |
| Delete Existing Account | Admin | Admin enters account number. System shows holder name, asks for re-confirmation, deletes account. |
| Update Account Information | Admin | Admin enters account number. System shows current info. Admin can update holder, balance, status, login, or pin. |
| Search for Account | Admin | Admin enters account number. System displays full account details. |
| Logout / Exit | Customer, Admin | User exits their menu and returns to the login screen. |

---

## 2. Application Flow Diagram

```mermaid
flowchart TD
    A[Start Application] --> B[Connect to MySQL Database]
    B --> C[Display Login Screen]
    C --> D[Enter Login & Pin]
    D --> E{Credentials Valid?}
    E -- No --> F[Display Error]
    F --> C
    E -- Yes --> G[Select Role]
    G --> H{Role Choice}
    
    H -- Customer --> I[Customer Menu]
    I --> I1{Choice}
    I1 -- 1 --> I2[Withdraw Cash]
    I1 -- 3 --> I3[Deposit Cash]
    I1 -- 4 --> I4[Display Balance]
    I1 -- 5 --> C
    I2 --> I
    I3 --> I
    I4 --> I

    H -- Administrator --> J{Is Admin?}
    J -- No --> K[Access Denied]
    K --> C
    J -- Yes --> L[Admin Menu]
    L --> L1{Choice}
    L1 -- 1 --> L2[Create Account]
    L1 -- 2 --> L3[Delete Account]
    L1 -- 3 --> L4[Update Account]
    L1 -- 4 --> L5[Search Account]
    L1 -- 6 --> C
    L2 --> L
    L3 --> L
    L4 --> L
    L5 --> L
```

---

## 3. Component Diagram

```mermaid
graph TB
    subgraph "C++ Application"
        MAIN["main.cpp<br/>(Entry Point)"]
        ACC["Account.h<br/>(Account Class + Input Validators)"]
        LOGIN["Login.h<br/>(Authentication)"]
        CUST["CustomerOptions.h<br/>(Withdraw, Deposit, Balance)"]
        ADMIN["AdminOptions.h<br/>(Create, Delete, Update, Search)"]
    end

    subgraph "MySQL Database"
        DB[(ATM Database)]
        TBL["accounts table<br/>AccountNumber, Login, Pin,<br/>HolderName, Balance, Status, IsAdmin"]
    end

    MAIN --> ACC
    MAIN --> LOGIN
    MAIN --> CUST
    MAIN --> ADMIN
    LOGIN --> ACC
    CUST --> ACC
    ADMIN --> ACC
    LOGIN --> DB
    CUST --> DB
    ADMIN --> DB
    DB --> TBL
```

### Component Responsibilities

| Component | File | Responsibility |
|-----------|------|----------------|
| Entry Point | `main.cpp` | DB connection, login loop, role selection, menu routing |
| Account Model | `Account.h` | Data encapsulation (private members, getters/setters), input validation helpers |
| Authentication | `Login.h` | Prepared statement login query, populates Account object |
| Customer Options | `CustomerOptions.h` | Withdraw, deposit, display balance with DB updates |
| Admin Options | `AdminOptions.h` | CRUD operations on customer accounts |
| Database Schema | `schema.sql` | Table definition and seed data |

---

## 4. Deployment Diagram

```mermaid
graph TB
    subgraph "Docker Environment"
        subgraph "App Container (devcontainer)"
            APP["C++ ATM Application<br/>(atm_app binary)"]
            LIBS["libmysqlcppconn<br/>MySQL C++ Connector"]
            GCC["g++ Compiler<br/>Build Tools"]
        end

        subgraph "DB Container (MidTermProject-db)"
            MYSQL["MySQL 8.0 Server"]
            DATA["db-data Volume<br/>(Persistent Storage)"]
        end

        NET["Docker Network<br/>(devcontainer_default)"]
    end

    DEV["Developer Machine<br/>(VS Code + Dev Containers Extension)"]
    SQL_EXT["VS Code SQL Extension<br/>(127.0.0.1:3307)"]

    DEV --> APP
    SQL_EXT --> MYSQL
    APP -- "tcp://db:3306" --> MYSQL
    MYSQL --> DATA
    APP --- NET
    MYSQL --- NET
```

### Deployment Details

| Component | Container | Port | Notes |
|-----------|-----------|------|-------|
| C++ App | `devcontainer-app` | N/A | Runs inside dev container, connects to DB via service name `db` |
| MySQL 8.0 | `MidTermProject-db` | 3306 (internal), 3307 (host) | Persistent data via `db-data` Docker volume |
| Docker Network | `devcontainer_default` | — | Enables container-to-container communication via DNS |

---

## 5. Database Schema Diagram

```mermaid
erDiagram
    accounts {
        INT AccountNumber PK "AUTO_INCREMENT, starts at 100"
        VARCHAR_50 Login UK "UNIQUE, NOT NULL"
        VARCHAR_5 Pin "NOT NULL, must be 5 digits"
        VARCHAR_100 HolderName "NOT NULL"
        DECIMAL_10_2 Balance "DEFAULT 0.00"
        VARCHAR_20 Status "DEFAULT Active"
        BOOLEAN IsAdmin "DEFAULT FALSE"
    }
```
