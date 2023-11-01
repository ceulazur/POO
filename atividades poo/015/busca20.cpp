#include <fn.hpp>
#include <algorithm>


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


class Agenda {
    std::vector<Contact> contatos;

    int findContact(std::string id) { 
        for(int i = 0; i < contatos.size(); i++) {
           if (contatos[i].getName() == id) {
            return i;
           }
        }
        return -1;
    }

public:
    //add contato que não existe ou adicionar fones ao contato que existe
    void addContato(std::string name, std::vector<Fone> fones) { 
       int pos = findContact(name);
       if (pos != -1) {
        
        for (int i = 0; i < fones.size(); i++) {
            contatos[pos].addFone(fones[i].getId(), fones[i].getNumber());
        }
       } else {
        Contact *newContact = new Contact(name);
        for (int i = 0; i < fones.size(); i++) {
            newContact->addFone(fones[i].getId(), fones[i].getNumber());
        }
        contatos.push_back(*newContact);
       }
    }

    void rmContact(std::string name) { 
        int pos = findContact(name);
        if (pos != -1) {
            contatos.erase(contatos.begin() + pos);
        }
    }

    Contact * getContact(std::string id) { 
         int pos = findContact(id);
        if (pos != -1) {
            return &contatos[pos];
        }
        return nullptr; 
    }

    std::vector<Contact> getContatos() { 
        return contatos; 
    }

    std::vector<Contact> search(std::string pattern) { 
        std::vector<Contact> result;
        for (int i = 0; i < contatos.size(); i++) {
            if (contatos[i].getName().find(pattern) != std::string::npos) {
                result.push_back(contatos[i]);

            } else {
                for (int j = 0; j < contatos[i].getFones().size(); j++) {
                    if (contatos[i].getFones()[j].getNumber().find(pattern) != std::string::npos || contatos[j].getFones()[j].getId().find(pattern) != std::string::npos) {
                        result.push_back(contatos[i]);
                        break;
                    }
                }
            }
        } 
        std::sort(result.begin(), result.end(), [](Contact a, Contact b) {
            return a.getName() < b.getName();
        });
        return result; 
    }

    std::vector<Contact> getFavorited() { 
        std::vector<Contact> result;
        for (int i = 0; i < contatos.size(); i++) {
            if (contatos[i].isFavorited()) {
                result.push_back(contatos[i]);

            }

        }
        return result; 
    }

    std::string str() { 
        std::string result = "";
        std::sort(contatos.begin(), contatos.end(), [](Contact a, Contact b) {
            return a.getName() < b.getName();
        });
        for (int i = 0; i < contatos.size(); i++) {
            if (i == 0) {
               result += contatos[i].str();
            } else {
                 result += "\n" + contatos[i].str();
            }
           
        }
        return result; 
    }
};


std::ostream& operator<<(std::ostream& out, Fone fone      ) { return out << fone.str();    }
std::ostream& operator<<(std::ostream& out, Contact contato) { return out << contato.str(); }
std::ostream& operator<<(std::ostream& out, Agenda agenda  ) { return out << agenda.str();  }

int main() {
    Agenda agenda;
    while(true) {
        auto line = fn::input();
        fn::write("$" + line);
        auto args = fn::split(line, ' ');
        if(args[0] == "end") {
            break;
        }
        else if(args[0] == "add") {//name id:fone id:fone ...
            std::vector<Fone> fones;
            for (int i = 2; i < (int) args.size(); i++) {
                auto parts = fn::split(args[i], ':');
                fones.push_back(Fone(parts[0], parts[1]));
            }
            agenda.addContato(args[1], fones);
        }
        else if(args[0] == "show") {
            fn::write(agenda);
        }
        else if(args[0]== "rmFone") {//id index
            Contact * contato = agenda.getContact(args[1]);
            if(contato != nullptr)
                contato->rmFone((int) +args[2]);
        }
        else if(args[0] == "rm") {//id
            agenda.rmContact(args[1]);
        }
        else if(args[0] == "tfav") {//id 1 ou 0
            Contact * contato = agenda.getContact(args[1]);
            if(contato != nullptr)
                contato->toogleFavorited();
        }
        else if(args[0] == "favs") {
            agenda.getFavorited() | fn::JOIN("\n") | fn::WRITE();
        }
        else if(args[0] == "search") {//pattern
            agenda.search(args[1]) | fn::JOIN("\n") | fn::WRITE();
        }
        else{
            fn::write("fail: comando invalido");
        }
    }
}
