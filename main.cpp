#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <iostream>
#include "Account.h"
#include "Login.h"
#include "CustomerOptions.h"
#include "AdminOptions.h"

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
                    adminOptions(con, account);
                } else {
                    customerOptions(con, account);
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
