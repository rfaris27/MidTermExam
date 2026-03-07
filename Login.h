#ifndef LOGIN_H
#define LOGIN_H

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include <iostream>
#include <string>
#include "Account.h"

bool loginUser(sql::Connection *con, Account &account) {
    std::string login, pin;

    std::cout << "Enter login: ";
    std::cin >> login;
    std::cout << "Enter Pin code: ";
    std::cin >> pin;

    sql::PreparedStatement *pstmt = con->prepareStatement(
        "SELECT * FROM accounts WHERE Login = ? AND Pin = ?");
    pstmt->setString(1, login);
    pstmt->setString(2, pin);

    sql::ResultSet *res = pstmt->executeQuery();

    bool found = false;
    if (res->next()) {
        account.setAccountNumber(res->getInt("AccountNumber"));
        account.setLogin(res->getString("Login"));
        account.setPin(res->getString("Pin"));
        account.setHolderName(res->getString("HolderName"));
        account.setBalance(res->getDouble("Balance"));
        account.setStatus(res->getString("Status"));
        account.setAdmin(res->getBoolean("IsAdmin"));
        found = true;
    }

    delete res;
    delete pstmt;
    return found;
}

#endif
