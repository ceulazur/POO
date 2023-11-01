#include <iostream> 
#include "fn.hpp"


class Pet{
private:
    int energyMax, hungryMax, cleanMax;
    int energy, hungry, clean;
    int diamonds = 0;
    int age = 0;
    bool alive = true;

    bool testAlive() {
        if (alive)
            return true;
        fn::write("fail: pet esta morto");
        return false;
    }

    void setHungry(int value) {
        if (value <= 0) {
            hungry = 0;
            fn::write("fail: pet morreu de fome");
            alive = false;
        } else if (value > hungryMax) {
            hungry = hungryMax;
        } else {
            hungry = value;
        }
    }
    void setEnergy(int value) {
        if (value <= 0) {
            energy = 0;
            fn::write("fail: pet morreu de fraqueza");
            alive = false;
        } else if (value > energyMax) {
            energy = energyMax;
        } else {
            energy = value;
        }
    }

    void setClean(int value) {
        if (value <= 0) {
            clean = 0;
            fn::write("fail: pet morreu de sujeira");
            alive = false;
        } else if (value > cleanMax) {
            clean = cleanMax;
        } else {
            clean = value;
        }
    }


public:
    Pet(int energy = 0, int hungry = 0, int clean = 0) { 
    // nop int shower, sleep, eat, play;
    energyMax = energy;
    hungryMax = hungry;
    cleanMax = clean;
    this->energy = energyMax;
    this->hungry = hungryMax;
    this->clean = cleanMax;
    }

    void play() {

        setEnergy(energy- 2);
        setHungry(hungry- 1);
        setClean(clean- 3);
        diamonds += 1;
        age += 1;

        if (!testAlive())
            return;

    }

    void shower() {
        if (!testAlive())
            return;
        setEnergy(energy - 3);
        setHungry(hungry - 1);
        setClean(cleanMax);
        diamonds += 0;
        age += 2;
    }

    void eat() {
        if (!testAlive())
            return;
        setEnergy(energy - 1);
        setHungry(hungry + 4);
        setClean(clean - 2);
        diamonds += 0;
        age += 1;
    }

    void sleep() {
        if (!testAlive())
            return;
            if (energyMax - energy >= 5) {
            int turnsSlept = energyMax - energy;
             setEnergy(energyMax);
             setHungry(hungry - 1);
             setClean(clean - 0);
             diamonds += 0;
             age += turnsSlept;
        } else {
            fn::write("fail: nao esta com sono");
        }
    }



    int getClean() {
        return clean;
    }
    int getHungry() {
        return hungry;
    }
    int getEnergy() {
        return energy;
    }
    int getEnergyMax() {
        return energyMax;
    }
    int getCleanMax() {
        return cleanMax;
    }
    int getHungryMax() {
        return hungryMax;
    }

    std::string toString() {
        return fn::format("E:{}/{}, S:{}/{}, L:{}/{}, D:{}, I:{}", 
            energy, energyMax, hungry, hungryMax, clean, cleanMax, diamonds, age);
    }

};


int main() {
    Pet pet;
    while (true) {
        auto line = fn::input();
        auto args = fn::split(line, ' ');

        fn::write("$" + line);

        if (args[0] == "end") {
            break;
        } else if (args[0] == "init") {
            pet = Pet(+args[1], +args[2], +args[3]);
        } else if (args[0] == "show") {
            fn::write(pet.toString());
        } else if (args[0] == "play") {
            pet.play();
        } else if (args[0] == "eat") {
            pet.eat();
        } else if (args[0] == "sleep") {
            pet.sleep();
        } else if (args[0] == "shower") {
            pet.shower();
        } else {
            fn::write("fail: comando invalido");
        }
    }
}
