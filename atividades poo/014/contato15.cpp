#include <fn.hpp>


class Fone{
    std::string id;
    std::string number;
public:
    Fone(std::string id = "", std::string number= "") : id(id), number(number) {
       
    }
    
    std::string getId() { 
        return id;
    }
    
    std::string getNumber() {
         return number; 
    }

    bool isValid() { 
        for (int i = 0; i < number.size(); i++) {
            if(!((number[i] >= '0' and number[i] <= '9') or number[i] == '(' or number[i] == ')' or number[i] == '.')) {
                return false;
            }
        }
        return true; 
    }

    std::string str() { 
        return fn::format("{%s}:{%s}", id, number);
    }   
};

class Contact{
    std::string name;
    bool favorited;
    std::vector<Fone> fones;
public:
    Contact(std::string name = "") : name(name), favorited(false) {

    }

    std::string getName() { 
        return name; 
    }

    std::vector<Fone> getFones() { 
        return fones; 
    }

    void addFone(std::string id, std::string number) { 
        Fone *newFone = new Fone(id,number);
        if (newFone->isValid()) {
            fones.push_back(*newFone);
        } else {
            std::cout << "fail: invalid number\n";
        }
    }

    void rmFone(int index) { 
        fones.erase(fones.begin() + index);
        return; 
    }

    void toogleFavorited() { 
        favorited =! favorited;
        return; 
    }

    bool isFavorited() { 
        return favorited; 
    }
//david [oi:123, tim:9081, claro:5431]
    std::string str() { 
        std::string result = (favorited ? "@ " : "- ");
        result += name + " [";

        for (int i = 0; i < fones.size(); i++) {
            if (i == 0) {
                result += fones[i].str();

            } else {
                result += ", " + fones[i].str();
            }
        }
        result += "]";
       return result;
    }
};

std::ostream& operator<<(std::ostream& out, Contact contato) { return out << contato.str(); }
std::ostream& operator<<(std::ostream& out, Fone fone      ) { return out << fone.str();    }


int main() {
    Contact contato;
    while(true) {
        auto line = fn::input();
        fn::write("$" + line);
        auto args = fn::split(line, ' ');

        if(args[0] == "end") {
            break;
        }
        else if(args[0] == "init") {//name
            contato = Contact(args[1]);
        }
        else if(args[0] == "add") {//id fone
            contato.addFone(args[1], args[2]);
        }
        else if(args[0] == "rm") {//id
            contato.rmFone(+args[1]);
        }
        else if(args[0] == "tfav") {
            contato.toogleFavorited();
        }
        else if(args[0] == "show") {
            fn::write(contato);
        }
        else{
            fn::write("fail: comando invalido");
        }
    }
}