#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
private:
    int accountNumber;
    std::string login;
    std::string pin;
    std::string holderName;
    double balance;
    std::string status;
    bool admin;

public:
    int getAccountNumber() const { return accountNumber; }
    std::string getLogin() const { return login; }
    std::string getPin() const { return pin; }
    std::string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }
    std::string getStatus() const { return status; }
    bool isAdmin() const { return admin; }

    void setAccountNumber(int num) { accountNumber = num; }
    void setLogin(const std::string &l) { login = l; }
    void setPin(const std::string &p) { pin = p; }
    void setHolderName(const std::string &name) { holderName = name; }
    void setBalance(double b) { balance = b; }
    void setStatus(const std::string &s) { status = s; }
    void setAdmin(bool a) { admin = a; }
};

#endif
