#ifndef ADMINOPTIONS_H
#define ADMINOPTIONS_H

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include <iostream>
#include <iomanip>
#include <string>
#include "Account.h"

void searchAccount(sql::Connection *con) {
    int accountNumber;
    std::cout << "Enter account number to search: ";
    std::cin >> accountNumber;

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "SELECT * FROM accounts WHERE AccountNumber = ? AND IsAdmin = FALSE");
    pstmt->setInt(1, accountNumber);
    sql::ResultSet *res = pstmt->executeQuery();

    if (res->next()) {
        std::cout << "\n--- Account Found ---" << std::endl;
        std::cout << "Account #" << res->getInt("AccountNumber") << std::endl;
        std::cout << "Login: " << res->getString("Login") << std::endl;
        std::cout << "Name: " << res->getString("HolderName") << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Balance: " << res->getDouble("Balance") << std::endl;
        std::cout << "Status: " << res->getString("Status") << std::endl;
    } else {
        std::cout << "Account not found." << std::endl;
    }

    delete res;
    delete pstmt;
}

void createAccount(sql::Connection *con) {
    std::string login, pin, name, status;
    double balance;

    std::cin.ignore();
    std:: cout << "The following information must be entered for the new account holder"<< std ::endl;
    std::cout << "Login: ";
    std::getline(std::cin, login);

    std::cout << "Pin Code: ";
    std::getline(std::cin, pin);

    bool validPin = (pin.length() == 5);
    if (validPin) {
        for (int i = 0; i < 5; i++) {
            if (!isdigit(pin[i])) {
                validPin = false;
                break;
            }
        }
    }
    if (!validPin) {
        std::cout << "Error: Pin Code must be an integer of length 5." << std::endl;
        return;
    }

    std::cout << "Holders Name: ";
    std::getline(std::cin, name);

    std::cout << "Starting Balance: ";
    std::cin >> balance;

    std::cin.ignore();
    std::cout << "Status: ";
    std::getline(std::cin, status);

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "INSERT INTO accounts (Login, Pin, HolderName, Balance, Status, IsAdmin) "
        "VALUES (?, ?, ?, ?, ?, FALSE)");
    pstmt->setString(1, login);
    pstmt->setString(2, pin);
    pstmt->setString(3, name);
    pstmt->setDouble(4, balance);
    pstmt->setString(5, status);

    try {
        pstmt->executeUpdate();

        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT LAST_INSERT_ID() AS id");
        res->next();
        int newId = res->getInt("id");
        delete res;
        delete stmt;

        std::cout << "Account Successfully Created – the account number assigned is: "
                  << newId << std::endl;
    }
    catch (sql::SQLException &e) {
        if (e.getErrorCode() == 1062) {
            std::cout << "Error: Login '" << login << "' already exists." << std::endl;
        } else {
            std::cout << "Error creating account: " << e.what() << std::endl;
        }
    }

    delete pstmt;
}

void deleteAccount(sql::Connection *con) {
    int accountNumber;
    std::cout << "Enter account number to delete: ";
    std::cin >> accountNumber;

    sql::PreparedStatement *check = con->prepareStatement(
        "SELECT HolderName FROM accounts WHERE AccountNumber = ? AND IsAdmin = FALSE");
    check->setInt(1, accountNumber);
    sql::ResultSet *res = check->executeQuery();

    if (!res->next()) {
        std::cout << "Account not found or cannot delete admin." << std::endl;
        delete res;
        delete check;
        return;
    }

    std::string holderName = res->getString("HolderName");
    delete res;
    delete check;

    std::cout << "You wish to delete the account held by " << holderName << std::endl;
    std::cout << "Re-enter account number to confirm: ";
    int confirmNumber;
    std::cin >> confirmNumber;

    if (confirmNumber != accountNumber) {
        std::cout << "Account numbers do not match. Delete cancelled." << std::endl;
        return;
    }

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "DELETE FROM accounts WHERE AccountNumber = ? AND IsAdmin = FALSE");
    pstmt->setInt(1, accountNumber);
    pstmt->executeUpdate();
    delete pstmt;

    std::cout << "Account #" << accountNumber << " held by "
              << holderName << " has been deleted." << std::endl;
}

void updateAccount(sql::Connection *con) {
    int accountNumber;
    std::cout << "Enter account number to update: ";
    std::cin >> accountNumber;

    sql::PreparedStatement *check = con->prepareStatement(
        "SELECT * FROM accounts WHERE AccountNumber = ? AND IsAdmin = FALSE");
    check->setInt(1, accountNumber);
    sql::ResultSet *res = check->executeQuery();

    if (!res->next()) {
        std::cout << "Account not found or cannot update admin." << std::endl;
        delete res;
        delete check;
        return;
    }

    std::cout << "\nAccount # " << res->getInt("AccountNumber") << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Holder: " << res->getString("HolderName") << std::endl;
    std::cout << "Balance: " << res->getDouble("Balance") << std::endl;
    std::cout << "Status: " << res->getString("Status") << std::endl;
    std::cout << "Login: " << res->getString("Login") << std::endl;
    std::cout << "Pin Code: " << res->getString("Pin") << std::endl;

    delete res;
    delete check;

    std::cout << "\nEnter new values (press Enter to keep current and move to next):" << std::endl;

    std::string holder, pin, status, login, balanceStr;
    std::cin.ignore();

    std::cout << "Holder: ";
    std::getline(std::cin, holder);
    std::cout << "Balance: ";
    std::getline(std::cin, balanceStr);
    std::cout << "Status: ";
    std::getline(std::cin, status);
    std::cout << "Login: ";
    std::getline(std::cin, login);
    std::cout << "Pin Code: ";
    std::getline(std::cin, pin);

    if (!pin.empty()) {
        bool validPin = (pin.length() == 5);
        if (validPin) {
            for (int i = 0; i < 5; i++) {
                if (!isdigit(pin[i])) {
                    validPin = false;
                    break;
                }
            }
        }
        if (!validPin) {
            std::cout << "Error: Pin Code must be an integer of length 5. Update cancelled." << std::endl;
            return;
        }
    }

    sql::PreparedStatement *pstmt;

    if (!holder.empty()) {
        pstmt = con->prepareStatement(
            "UPDATE accounts SET HolderName = ? WHERE AccountNumber = ?");
        pstmt->setString(1, holder);
        pstmt->setInt(2, accountNumber);
        pstmt->executeUpdate();
        delete pstmt;
    }
    if (!balanceStr.empty()) {
        double balance = std::stod(balanceStr);
        pstmt = con->prepareStatement(
            "UPDATE accounts SET Balance = ? WHERE AccountNumber = ?");
        pstmt->setDouble(1, balance);
        pstmt->setInt(2, accountNumber);
        pstmt->executeUpdate();
        delete pstmt;
    }
    if (!status.empty()) {
        pstmt = con->prepareStatement(
            "UPDATE accounts SET Status = ? WHERE AccountNumber = ?");
        pstmt->setString(1, status);
        pstmt->setInt(2, accountNumber);
        pstmt->executeUpdate();
        delete pstmt;
    }
    if (!login.empty()) {
        pstmt = con->prepareStatement(
            "UPDATE accounts SET Login = ? WHERE AccountNumber = ?");
        pstmt->setString(1, login);
        pstmt->setInt(2, accountNumber);
        pstmt->executeUpdate();
        delete pstmt;
    }
    if (!pin.empty()) {
        pstmt = con->prepareStatement(
            "UPDATE accounts SET Pin = ? WHERE AccountNumber = ?");
        pstmt->setString(1, pin);
        pstmt->setInt(2, accountNumber);
        pstmt->executeUpdate();
        delete pstmt;
    }

    std::cout << "Account updated successfully." << std::endl;
}

void adminOptions(sql::Connection *con, Account &account) {
    int choice;

    while (true) {
        std::cout << "\n--- Admin Options ---" << std::endl;
        std::cout << "Welcome, " << account.getHolderName() << std::endl;
        std::cout << "1----Create New Account" << std::endl;
        std::cout << "2----Delete Existing Account" << std::endl;
        std::cout << "3----Update Account Information" << std::endl;
        std::cout << "4----Search for Account" << std::endl;
        std::cout << "6----Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                createAccount(con);
                break;
            case 2:
                deleteAccount(con);
                break;
            case 3:
                updateAccount(con);
                break;
            case 4:
                searchAccount(con);
                break;
            case 6:
                std::cout << "Logged out." << std::endl;
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
}

#endif
