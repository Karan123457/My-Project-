#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

class Account {
private:
    int accountNumber;
    float balance;
    string pin;
    vector<string> transactionHistory;
    int incorrectAttempts;
    bool locked;

public:
    Account(int number, float initialBalance, string accountPin) 
        : accountNumber(number), balance(initialBalance), pin(accountPin), incorrectAttempts(0), locked(false) {}

    bool verifyPin(const string& enteredPin) {
        if (locked) {
            cout << "Account is locked due to multiple incorrect PIN attempts." << endl;
            return false;
        }

        if (pin == enteredPin) {
            incorrectAttempts = 0; // Reset incorrect attempts on successful login
            return true;
        } else {
            incorrectAttempts++;
            if (incorrectAttempts >= 3) {
                locked = true;
                cout << "Account locked after 3 incorrect PIN attempts." << endl;
            } else {
                cout << "Incorrect PIN. Attempts remaining: " << 3 - incorrectAttempts << endl;
            }
            return false;
        }
    }

    void deposit(float amount) {
        if (amount > 0) {
            balance += amount;
            string transaction = "Deposited $" + to_string(amount);
            transactionHistory.push_back(transaction);
            cout << transaction << ". New balance: $" << balance << endl;
        } else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(float amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            string transaction = "Withdrew $" + to_string(amount);
            transactionHistory.push_back(transaction);
            cout << transaction << ". New balance: $" << balance << endl;
        } else {
            cout << "Invalid withdrawal amount or insufficient funds." << endl;
        }
    }

    void displayBalance() const {
        cout << "Current balance: $" << balance << endl;
    }

    void displayTransactionHistory() const {
        cout << "\nTransaction History for Account " << accountNumber << ":\n";
        if (transactionHistory.empty()) {
            cout << "No transactions available.\n";
        } else {
            for (const string& transaction : transactionHistory) {
                cout << "- " << transaction << endl;
            }
        }
    }

    void calculateInterest(float rate) {
        float interest = balance * (rate / 100);
        balance += interest;
        cout << "Interest added at " << rate << "% rate. New balance: $" << balance << endl;
        transactionHistory.push_back("Interest added: $" + to_string(interest));
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    float getBalance() const {
        return balance;
    }

    void transfer(Account& target, float amount) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            target.deposit(amount);
            string transaction = "Transferred $" + to_string(amount) + " to account " + to_string(target.getAccountNumber());
            transactionHistory.push_back(transaction);
            cout << transaction << ". New balance: $" << balance << endl;
        } else {
            cout << "Invalid transfer amount or insufficient funds." << endl;
        }
    }
};

class ATM {
private:
    vector<Account> accounts;

public:
    void addAccount(const Account& account) {
        accounts.push_back(account);
    }

    Account* authenticate(int accountNumber, const string& pin) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber && account.verifyPin(pin)) {
                return &account;
            }
        }
        cout << "Authentication failed. Incorrect account number or PIN." << endl;
        return nullptr;
    }

    void run() {
        int accountNumber;
        string pin;
        cout << "Enter account number: ";
        cin >> accountNumber;
        cout << "Enter PIN: ";
        cin >> pin;

        Account* account = authenticate(accountNumber, pin);
        if (account) {
            int choice;
            do {
                cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. View Transaction History\n5. Transfer to Another Account\n6. Calculate Interest\n7. Exit\nEnter your choice: ";
                cin >> choice;
                switch (choice) {
                    case 1:
                        account->displayBalance();
                        break;
                    case 2: {
                        float amount;
                        cout << "Enter amount to deposit: ";
                        cin >> amount;
                        account->deposit(amount);
                        break;
                    }
                    case 3: {
                        float amount;
                        cout << "Enter amount to withdraw: ";
                        cin >> amount;
                        account->withdraw(amount);
                        break;
                    }
                    case 4:
                        account->displayTransactionHistory();
                        break;
                    case 5: {
                        int targetAccountNumber;
                        float amount;
                        cout << "Enter target account number: ";
                        cin >> targetAccountNumber;
                        cout << "Enter amount to transfer: ";
                        cin >> amount;
                        Account* targetAccount = findAccount(targetAccountNumber);
                        if (targetAccount) {
                            account->transfer(*targetAccount, amount);
                        } else {
                            cout << "Target account not found." << endl;
                        }
                        break;
                    }
                    case 6: {
                        float rate;
                        cout << "Enter interest rate (%): ";
                        cin >> rate;
                        account->calculateInterest(rate);
                        break;
                    }
                    case 7:
                        cout << "Exiting the system." << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            } while (choice != 7);
        }
    }

    Account* findAccount(int accountNumber) {
        for (auto& account : accounts) {
            if (account.getAccountNumber() == accountNumber) {
                return &account;
            }
        }
        return nullptr;
    }
};

int main() {
    ATM atm;
    atm.addAccount(Account(123456, 500.0, "1234")); // Account number 123456 with PIN 1234
    atm.addAccount(Account(654321, 1000.0, "5678")); // Account number 654321 with PIN 5678

    atm.run();

    return 0;
}