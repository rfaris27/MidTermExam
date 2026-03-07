USE ATM;

CREATE TABLE accounts (
    AccountNumber INT AUTO_INCREMENT PRIMARY KEY,
    Login         VARCHAR(50)    UNIQUE NOT NULL,
    Pin           VARCHAR(5)     NOT NULL,
    HolderName    VARCHAR(100)   NOT NULL,
    Balance       DECIMAL(10,2)  DEFAULT 0.00,
    Status        VARCHAR(20)    DEFAULT 'Active',
    IsAdmin       BOOLEAN        DEFAULT FALSE
) AUTO_INCREMENT = 100;

-- Create an Admin user
INSERT INTO accounts (Login, Pin, HolderName, Balance, Status, IsAdmin)
VALUES ('Adnan123', '12345', 'Adnan', 0.00, 'Active', TRUE)
ON DUPLICATE KEY UPDATE Login = Login;


