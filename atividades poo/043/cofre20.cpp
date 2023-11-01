#include <fn.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

class Valuable {
public:
    virtual std::string getLabel()  const = 0;
    virtual double      getValue()  const = 0;
    virtual int         getVolume() const = 0;
    virtual std::string str()       const = 0;
    virtual ~Valuable() {}
};

std::ostream& operator<<(std::ostream& os, const Valuable& valuable) {
    return os << valuable.str();
}

class Coin : public Valuable {
    Coin(const std::string& label, double value, int volume) : label(label), value(value), volume(volume) {}
public:

    static const Coin C10;
    static const Coin C25;
    static const Coin C50;
    static const Coin C100;

    std::string getLabel() const override {
        return label;
    }

    int getVolume() const override {
        return volume;
    }

    double getValue() const override {
        return value;
    }

    std::string str() const override {
        return fn::format("{}:{%.2f}:{}", label, value, volume);
    }

private:
    std::string label;
    double value;
    int volume;
};

const Coin Coin::C10  { "M10", 0.10, 1};
const Coin Coin::C25  { "M25", 0.25, 2};
const Coin Coin::C50  { "M50", 0.50, 3};
const Coin Coin::C100 {"M100", 1.00, 4};


class Item : public Valuable {
public:
    Item(const std::string& label, double value, int volume): label(label), value(value), volume(volume) { }

    std::string getLabel() const override { 
        return label;
    }

    int getVolume() const override { 
        return volume; 
    }

    double getValue() const override {
        return value;
    }

    std::string str() const override { 
        return fn::format("{}:{%.2f}:{}", label, value, volume);
    }

private:
    std::string label;
    double value;
    int volume;
};

class Pig {
    int volumeMax;
    bool broken;
    std::vector<std::shared_ptr<Valuable>> valuables;
public:

    Pig(int volumeMax) : volumeMax(volumeMax), broken(false) {};

    void addValuable(const std::shared_ptr<Valuable>& val) { 
        if(this->getVolume() + val.get()->getVolume() > this->volumeMax) {
            std::cout<< "fail: the pig is full\n";
            return;
        } else if(broken) {
            std::cout<< "fail: the pig is broken\n";
            return;
        }
            
       valuables.push_back(val); 
    }

    void breakPig() {
        broken = true;
    }

    std::vector<std::shared_ptr<Coin>> extractCoins() {

        if (broken) {
            std::vector<std::shared_ptr<Coin>> varAux;

            for (int i = 0; i < (int) valuables.size(); i++){
                if (
                    valuables[i].get()->getLabel() == "M10" || 
                    valuables[i].get()->getLabel() == "M25" || 
                    valuables[i].get()->getLabel() == "M50" || 
                    valuables[i].get()->getLabel() == "M100"
                ){
                    varAux.push_back(std::dynamic_pointer_cast<Coin>(valuables[i]));
                }
            }

            for (int i = 0; i < (int) valuables.size(); i++){
                if (
                    valuables[i].get()->getLabel() == "M10" || 
                    valuables[i].get()->getLabel() == "M25" || 
                    valuables[i].get()->getLabel() == "M50" || 
                    valuables[i].get()->getLabel() == "M100"
                ) {
                    valuables.erase(valuables.begin() + i);

                    i = std::max(-1, i - 1);
                }
            }

            return varAux;
        } else {
            std::cout << "fail: you must break the pig first\n";
        }

        return {};
    }

    std::vector<std::shared_ptr<Item>> extractItems() {
        if (broken) {
            std::vector<std::shared_ptr<Item>> varAux;
            
            for (int i = 0; i < (int) valuables.size(); i++){
                if (
                    valuables[i].get()->getLabel() != "M10" && 
                    valuables[i].get()->getLabel() != "M25" && 
                    valuables[i].get()->getLabel() != "M50" && 
                    valuables[i].get()->getLabel() != "M100"
                ){
                    varAux.push_back(std::dynamic_pointer_cast<Item>(valuables[i]));
                }
            }

            for (int i = 0; i < (int) valuables.size(); i++){
                if (valuables[i].get()->getLabel() != "M10" && 
                    valuables[i].get()->getLabel() != "M25" && 
                    valuables[i].get()->getLabel() != "M50" && 
                    valuables[i].get()->getLabel() != "M100") {
                    
                    valuables.erase(valuables.begin() + i);

                    i = std::max(-1, i - 1);
                }
            }

            return varAux;
        } else {
            std::cout << "fail: you must break the pig first\n";
        }

        return {};
    }

    int getVolume() const {
        int volume = 0; 
        for (int i = 0; i < (int) valuables.size(); i++){
           volume += valuables[i].get()->getVolume();
        }
        return (broken ? 0 : volume);
    }

    double getValue() const { 
        double value = 0; 
        for (int i = 0; i < (int) valuables.size(); i++){
           value += valuables[i].get()->getValue();
        }
        return value; 
    }

    std::string str() const {
        std::string varAux = "[";
        for (int i = 0; i < (int) valuables.size(); i++){
            if(!i) {
                varAux += valuables[i].get()->str();
            } else {
                varAux += ", " + valuables[i].get()->str();
            }
        }

        varAux += "] : " + fn::format("{%.2f}$ : {}/{} : {}", getValue(), getVolume(), volumeMax, (broken ? "broken" : "intact"));
        return varAux;
    }
};


std::ostream& operator<<(std::ostream& os, const Pig& pig) {
    return os << pig.str();
}

int main() {
    Pig pig(0);
    while (true) {
        try {
            auto line = fn::input();
            auto args = fn::split(line, ' ');
            fn::write("$" + line);

            if      (args[0] == "end" ) { break; } 
            else if (args[0] == "init") { pig = Pig(+args[1]); }
            else if (args[0] == "addCoin") {
                switch((int) +args[1]) {
                    case  10: pig.addValuable(std::make_shared<Coin>(Coin::C10 )); break;
                    case  25: pig.addValuable(std::make_shared<Coin>(Coin::C25 )); break;
                    case  50: pig.addValuable(std::make_shared<Coin>(Coin::C50 )); break;
                    case 100: pig.addValuable(std::make_shared<Coin>(Coin::C100)); break;
                }
            }
            else if (args[0] == "addItem") {
                pig.addValuable(std::make_shared<Item>(args[1], +args[2], (int) +args[3]));
            }
            else if (args[0] == "break"       ) { pig.breakPig();                               }
            else if (args[0] == "extractCoins") {
                std::vector<std::shared_ptr<Coin>> res = pig.extractCoins();
                
                if (res.size() > 0) {
                    std::string varAux = "[";
                    for (int i = 0; i < (int) res.size(); i++){
                        if(!i) {
                            varAux += res[i].get()->str();
                        } else {
                            varAux += ", " + res[i].get()->str();
                        }
                    }

                    varAux += "]";
                    fn::write(varAux);
                }  
            }
            else if (args[0] == "extractItems") { 
                std::vector<std::shared_ptr<Item>> res = pig.extractItems();

                if(res.size() > 0) {

                    std::string varAux = "[";
                    for (int i = 0; i < (int) res.size(); i++){
                        if(!i) {
                            varAux += res[i].get()->str();
                        } else {
                            varAux += ", " + res[i].get()->str();
                        }
                    }

                    varAux += "]";
                    fn::write(varAux);
                }
            }
            else if (args[0] == "show"        ) { fn::write(pig);                               }
            else                                { fn::write("fail: invalid command");           }
        } catch (std::string s) {
            fn::write(s);
        }
    }

}