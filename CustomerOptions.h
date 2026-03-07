#ifndef CUSTOMEROPTIONS_H
#define CUSTOMEROPTIONS_H

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "Account.h"

std::string getCurrentDate() {
    time_t now = time(0);
    struct tm *t = localtime(&now);
    char buf[11];
    strftime(buf, sizeof(buf), "%m/%d/%Y", t);
    return std::string(buf);
}

void withdrawCash(sql::Connection *con, Account &account) {
    double amount;
    std::cout << "Enter the withdrawal amount: ";
    std::cin >> amount;

    if (amount <= 0) {
        std::cout << "Invalid amount." << std::endl;
        return;
    }

    if (amount > account.getBalance()) {
        std::cout << "Insufficient funds." << std::endl;
        return;
    }

    double newBalance = account.getBalance() - amount;

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "UPDATE accounts SET Balance = ? WHERE AccountNumber = ?");
    pstmt->setDouble(1, newBalance);
    pstmt->setInt(2, account.getAccountNumber());
    pstmt->executeUpdate();
    delete pstmt;

    account.setBalance(newBalance);

    std::cout << "Cash Successfully Withdrawn" << std::endl;
    std::cout << "Account #" << account.getAccountNumber() << std::endl;
    std::cout << "Date: " << getCurrentDate() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Withdrawn: " << amount << std::endl;
    std::cout << "Balance: " << account.getBalance() << std::endl;
}

void depositCash(sql::Connection *con, Account &account) {
    double amount;
    std::cout << "Enter the cash amount to deposit: ";
    std::cin >> amount;

    if (amount <= 0) {
        std::cout << "Invalid amount." << std::endl;
        return;
    }

    double newBalance = account.getBalance() + amount;

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "UPDATE accounts SET Balance = ? WHERE AccountNumber = ?");
    pstmt->setDouble(1, newBalance);
    pstmt->setInt(2, account.getAccountNumber());
    pstmt->executeUpdate();
    delete pstmt;

    account.setBalance(newBalance);

    std::cout << "Cash Deposited Successfully." << std::endl;
    std::cout << "Account #" << account.getAccountNumber() << std::endl;
    std::cout << "Date: " << getCurrentDate() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Deposited: " << amount << std::endl;
    std::cout << "Balance: " << account.getBalance() << std::endl;
}

void displayBalance(Account &account) {
    std::cout << "Account #" << account.getAccountNumber() << std::endl;
    std::cout << "Holder: " << account.getHolderName() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Balance: " << account.getBalance() << std::endl;
}

void customerOptions(sql::Connection *con, Account &account) {
    int choice;

    while (true) {
        std::cout << "\n--- Customer Menu ---" << std::endl;
        std::cout << "1----Withdraw Cash" << std::endl;
        std::cout << "3----Deposit Cash" << std::endl;
        std::cout << "4----Display Balance" << std::endl;
        std::cout << "5----Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                withdrawCash(con, account);
                break;
            case 3:
                depositCash(con, account);
                break;
            case 4:
                displayBalance(account);
                break;
            case 5:
                std::cout << "Logged out." << std::endl;
                return;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
}

#endif
