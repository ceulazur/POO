//vale 10
#include <fn.hpp>
#include <utility>
#include <string>
#include <vector>
HO

class Coin {
    std::string label;
    double value;
    int volume;

    Coin(std::string label, double value, int volume) :
        label(label), value(value), volume(volume) {
    }
public:
    const static Coin C10;
    const static Coin C25;
    const static Coin C50;
    const static Coin C100;

    double getValue() const { 
        return value; 
    }
    int getVolume() const { 
        return volume; 
    }
    std::string getLabel() const { 
        return label; 
    }

    std::string str() const {
        return fn::format("{%.2f}:{}", value, volume);
    }
};

const Coin Coin::C10  { "C10", 0.10, 1};
const Coin Coin::C25  { "C25", 0.25, 2};
const Coin Coin::C50  { "C50", 0.50, 3};
const Coin Coin::C100 {"C100", 1.00, 4};

std::ostream& operator<<(std::ostream& os, const Coin& coin) {
    return os << coin.str();
}

class Item {
    std::string label;
    int volume;
public:
    Item(std::string label, int volume): label(label), volume(volume) { //todo
    }
    int getVolume() const {
        return volume; // todo
    }
    std::string getLabel() const {
        return label; // todo
    }
    void setVolume(int volume) {
        this->volume = volume;
    }
    void setLabel(std::string label) {
        this->label = label;
    }
    std::string str() const {
        return label + ":" + std::to_string(volume); // todo
    }
};

std::ostream& operator<<(std::ostream& os, const Item& item) {
    return os << item.str();
}

class Pig {
    std::vector<Item> itens;
    std::vector<Coin> coins;

    int volumeMax {0};
    bool broken {false};
    int volume {0};

public:
    Pig(int volumeMax = 0) : volumeMax(volumeMax) {

    }
    
    bool addCoin(Coin coin) {
       if (broken) {
           std::cout<<"fail: the pig is broken\n";
            return false; 
       } else if (getVolume() + coin.getVolume() > volumeMax) {
            std::cout<<"fail: the pig is full\n";
            return false;
        }
       volume += coin.getVolume();
       coins.push_back(coin);
       return true;
        // todo
    }

    bool addItem(Item item) {
        if (broken) {
            std::cout<<"fail: the pig is broken\n";
            return false;
        } else if (getVolume() + item.getVolume() > volumeMax) {
            std::cout<<"fail: the pig is full\n";
            return false;
        }
        volume += item.getVolume();
        itens.push_back(item);

        return true;
    }

    bool breakPig() {
        if (broken) {
            return false;
        }
        volume = 0;
        broken = true;
        return true;
        // todo
    }

    std::vector<Coin> extractCoins() {
        if(!broken) {
            std::cout << "fail: you must break the pig first\n";

            return {}; 
        } else {
            std::vector<Coin> extractedCoins(coins);
            coins.clear();

            return extractedCoins;
        }
    }
// fail:·the pig is full
    std::vector<Item> extractItems() {
        if(!broken) {
            std::cout << "fail: you must break the pig first\n";

            return {}; 
        } else {
            std::vector<Item> extractedItems(itens);
            itens.clear();

            return extractedItems;
        }
    }

    double getValue() const {
        double totalValue = 0.0;
        for (const auto& coin : coins) {
            totalValue += coin.getValue();
        }
        return totalValue; // todo
    }

    int getVolume() const {
       return volume;
    }
    
    std::string str() const {
        std::string pigString = "state=";

        pigString += this->broken ? "broken" : "intact";

        pigString += " : coins=[";

        for(int i = 0; i < this->coins.size(); ++ i) {
            if(i == 0) {
                pigString += this->coins[i].str();
            } else {
                pigString += ", " + this->coins[i].str();
            }
        }

        pigString += "]";

        pigString += " : items=[";

        for(int i = 0; i < this->itens.size(); ++ i) {
            if(i == 0) {
                pigString += this->itens[i].str();
            } else {
                pigString += ", " + this->itens[i].str();
            }
        }

        pigString += "] : value=";

        pigString += fn::format("{%.2f }", this->getValue());

        pigString += ": volume=";
        pigString += std::to_string(this->getVolume());
        pigString += "/" + std::to_string(this->volumeMax);

        return pigString;
    }
};

std::ostream& operator<<(std::ostream& os, const Pig& pig) {
       
    return os << pig.str();
}


int main() {

    Pig pig;
    while (true) {
        auto line = fn::input();
        fn::write("$" + line);
        auto args = fn::split(line);

        if      (args[0] == "end"    ) { break; }
        else if (args[0] == "addCoin") { 
            if      (args[1] == "10" ) pig.addCoin(Coin::C10);
            else if (args[1] == "25" ) pig.addCoin(Coin::C25);
            else if (args[1] == "50" ) pig.addCoin(Coin::C50);
            else if (args[1] == "100") pig.addCoin(Coin::C100);
        }
        else if (args[0] == "init"        ) { pig = Pig( (int) +(args[1]));                 }
        else if (args[0] == "addItem"     ) { pig.addItem(Item(args[1], (int) +(args[2]))); }
        else if (args[0] == "break"       ) { pig.breakPig();                               }
        else if (args[0] == "extractCoins") { fn::write(pig.extractCoins());                }
        else if (args[0] == "extractItems") { fn::write(pig.extractItems());                }
        else if (args[0] == "show"        ) { fn::write(pig.str());                               }
        else                                { fn::write("fail: invalid command");           }
    }
}
