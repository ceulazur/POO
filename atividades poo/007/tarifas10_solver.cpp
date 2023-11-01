//vale 10
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace fn;
enum class Label {
    Deposit,
    Fee,
    Opening,
    Reverse,
    Withdraw
};

class Operation {
private:
    int index;
    Label label;
    int value;
    int balance;

public:
    Operation(int index, Label label, int value, int balance) : index(index), label(label), value(value), balance(balance) {}

    int getBalance() const { 
        return balance; 
    }
    int getIndex() const { 
        return index; 
    }
    Label getLabel() const { 
        return label; 
    }
    int getValue() const { 
        return value; 
    }

    friend std::ostream& operator<<(std::ostream& os, const Operation& operation) {
        os << operation.index << ": ";
         switch (operation.label) {
            case Label::Deposit:
                os << "deposit:  ";
                break;
            case Label::Fee:
                os << "fee:      ";
                break;
            case Label::Opening:
                os << "opening:  ";
                break;
            case Label::Reverse:
                os << "reverse:  ";
                break;
            case Label::Withdraw:
                os << "withdraw: ";
                break;
        }
        os << std::setw(4) << operation.value << ": " << std::setw(4) << operation.balance;
        return os;
    }
};


class BalanceManager {
private:
    int balance;
    std::vector<Operation> extract;
    int nextId;

public:
    BalanceManager() : balance(0), nextId(0) {
        addOperation(Label::Opening, 0);
    }

    void addOperation(Label label, int value) {
    Operation operation(nextId, label, value, balance);
    extract.push_back(operation);
    nextId++;
    if (label == Label::Deposit || label == Label::Withdraw) {
        balance += value;
    }
}

    int getBalance() const { 
        return balance; 
    }

    std::vector<Operation> getExtract(int qtdOp = 0) const {
        if (qtdOp == 0) {
            return extract;
        }
        int startIndex = extract.size() - qtdOp;
        if (startIndex < 0) {
            startIndex = 0;
        }
        std::vector<Operation> result(extract.begin() + startIndex, extract.end());
        return result;
    }
};

class Account {
private:
    int id;
    BalanceManager balanceManager;

public:
    Account(int id) : id(id) {}

    void deposit(int value) {
        if (value < 0) {
            std::cout << "fail: invalid value" << std::endl;
            return;
        }
        balanceManager.addOperation(Label::Deposit, value);
    }

    void fee(int value) { 
        balanceManager.addOperation(Label::Fee, -value); 
    }

    void reverse(int index) {
        std::vector<Operation> extract = balanceManager.getExtract();
        if (index >= extract.size() || extract[index].getLabel() != Label::Fee) {
            std::cout << "fail: invalid index" << std::endl;
            return;
        }
        int feeValue = extract[index].getValue();
        balanceManager.addOperation(Label::Reverse, feeValue);
    }

    void withdraw(int value) {
        if (value < 0) {
            std::cout << "fail: invalid value" << std::endl;
            return;
        }
        if (value > balanceManager.getBalance()) {
            std::cout << "fail: insufficient balance" << std::endl;
            return;
        }
        balanceManager.addOperation(Label::Withdraw, -value);
    }

    friend std::ostream& operator<<(std::ostream& os, const Account& account) {
        os << "account:" << account.id << " balance:" << account.balanceManager.getBalance();
        return os;
    }

    const BalanceManager& getBalanceManager() const { 
        return balanceManager; 
    }
};

int main() {
    Account account(0);

    while(true) {
        auto line = input();
        auto cmd = split(line, ' ');

        write("$" + line);

        if(cmd[0] == "init") {
            account = Account(+cmd[1]);
        } else if(cmd[0] == "deposit") {
            account.deposit(+cmd[1]);
        } else if(cmd[0] == "fee") {
            account.fee(+cmd[1]);
        } else if(cmd[0] == "reverse") {
            account.reverse(+cmd[1]);
        } else if(cmd[0] == "withdraw") {
            account.withdraw(+cmd[1]);
        } else if(cmd[0] == "show") {
            write(account);
        } else if(cmd[0] == "extract") {
            int qtdOp = (cmd.size() > 1) ? +cmd[1]) : 0;
            auto extract = account.getBalanceManager().getExtract(qtdOp);
            for(auto operation : extract) {
                write(operation);
            }
        } else if(cmd[0] == "end") {
            break;
        }
    }
}