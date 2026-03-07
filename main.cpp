#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <iostream>
#include "Account.h"
#include "Login.h"

void customerMenu(sql::Connection *con, Account &account) {
    std::cout << "Welcome, " << account.getHolderName() << "!" << std::endl;
    std::cout << "Customer menu coming soon..." << std::endl;
}

void adminMenu(sql::Connection *con, Account &account) {
    std::cout << "Welcome " << account.getHolderName() << "!" << std::endl;
    std::cout << "Admin Options coming soon..." << std::endl;
}

int main() {
    try {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://db:3306", "root", "p");
        con->setSchema("ATM");

        std::cout << "--- ATM System ---" << std::endl;

        while (true) {
            Account account;

            if (loginUser(con, account)) {
                if (account.isAdmin()) {
                    adminMenu(con, account);
                } else {
                    customerMenu(con, account);
                }
            } else {
                std::cout << "Error: Invalid login or pin." << std::endl;
            }
        }

        delete con;
    }
    catch (sql::SQLException &e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        return 1;
    }

    return 0;
}
