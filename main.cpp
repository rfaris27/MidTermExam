#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/metadata.h>
#include <cppconn/exception.h>
#include <iostream>
#include <string>

int main()
{
    try
    {
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        driver = sql::mysql::get_mysql_driver_instance();

        // 1. Connect using the service name 'db' from your docker-compose
        // 2. Use the database 'ATM'
        // Change the IP to "db" or "MidTermProject-db"
        con = driver->connect("tcp://db:3306", "root", "p");
        con->setSchema("ATM");

        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res;
        sql::ResultSetMetaData *res_meta;

        std::cout << "--- Connection Successful! ---" << std::endl;

        // --- SECTION: Verify Account Table ---
        // This pulls 1 row to prove the connection and table structure exist
        std::cout << "--- Checking Accounts Table ---" << std::endl;
        
        res = stmt->executeQuery("SELECT * FROM accounts LIMIT 1");
        res_meta = res->getMetaData();
        int col_count = res_meta->getColumnCount();

        if (!res->next()) {
            std::cout << "Connected, but the 'accounts' table is currently empty." << std::endl;
        } else {
            // Loop through columns using the Metadata style you prefer
            for (int i = 1; i <= col_count; i++)
            {
                std::cout << res_meta->getColumnName(i) << ": " << res->getString(i);
                if (i < col_count)
                    std::cout << " | ";
            }
            std::cout << std::endl;
        }

        // Clean up
        delete res;
        delete stmt;
        delete con;
    }
    catch (sql::SQLException &e)
    {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        std::cerr << "Error Code: " << e.getErrorCode() << std::endl;
        return 1;
    }

    return 0;
}