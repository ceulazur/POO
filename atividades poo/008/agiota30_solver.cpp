//vale 30
#include <fn.hpp>
#include <algorithm>
#include <cmath>

enum Label {
    GIVE, TAKE, PLUS
};

std::ostream& operator<<(std::ostream& os, Label label) {
    switch (label) {
        case GIVE: os << "give"; break;
        case TAKE: os << "take"; break;
        case PLUS: os << "plus"; break;
    }
    return os;
}

class Operation {
    int id;           //id único da transação
    std::string name; //id único do cliente
    Label label;      //label da operação
    int value;        //valores são sempre positivo  
public:

    Operation(int id, std::string name, Label label, int value) {
        this->id    = id;
        this->name  = name;
        this->label = label;
        this->value = value;
    }

    std::string getName() {
        return name;
    }

    Label getLabel() {
        return label;
    }

    int getValue() {
        return value;
    }

    std::string str() {
        return "id:{} {}:{} {}" | fn::FORMAT(id, label, name, value);
    }
};

std::ostream& operator<<(std::ostream& os, Operation tr) {
    return os << tr.str();
}

class Client {
    std::string name; // nome do cliente
    int limite;    // o maximo que ele pode ficar devendo
    std::vector<std::shared_ptr<Operation>> operations; //dívidas e pagamentos 
public:
/*

addCli maria 500
addCli rubia 60
addCli maria 300
give maria 300
give rubia 50
give maria 100
show
give bruno 30
give rubia 30
show
take maria 350
take rubia 1
take maria 10
show
showCli maria
end
*/
    Client(std::string name = "", int limite = 0) {
        this->name = name;
        this->limite = limite;
    }

    std::string getName() {
        return name;
    }

    int getLimite() {
        return limite;
    }
    //o quanto ele deve ao agiota
    // soma todas as operações de GIVE e PLUS
    // e subtrai todas as operações de TAKE 
    int getBalance() {
        int balance = 0;
        for (auto tr : operations) {
            if (tr->getLabel() == GIVE || tr->getLabel() == PLUS) {
                balance +- tr->getValue();
            } else if (tr->getLabel() == TAKE) {
                balance -= tr->getValue();
            }
        }
        return balance;  
    }

    void addOperation(std::shared_ptr<Operation> tr) {
        // std::cout << tr.get()->getName() << std::endl;
        operations.push_back(tr);
    }

    std::vector<std::shared_ptr<Operation>> getOperations() {
        // std::cout << this->operations[0].get()->getName() << std::endl;
        return this->operations;
    }

    std::string str() {
        return "{} {}/{}" | fn::FORMAT(name, getBalance(), limite);
    }
};

std::ostream& operator<<(std::ostream &os, Client cliente) {
    return os << cliente.str();
}



class Agiota {
    //ordenado por nome do cliente
    //nome deve ser único
    std::vector<Client> alive_list;

    //nome não precisa ser único na lista de clientes mortos
    //ordenado pela data da morte
    std::vector<Client> death_list;

    //as transações dos vivos
    std::vector<std::shared_ptr<Operation>> alive_oper;
    //as transações dos mortos
    std::vector<std::shared_ptr<Operation>> death_oper;
        
    //guarda o id da próxima transação
    int nextOpId = 0;

    //retorna a posição desse cliente no vetor de vivos ou -1 se não existir
    int searchClient(std::string name) {
         for (int i = 0; i < (int) alive_list.size(); i++) {
              if (alive_list[1].getName() == name) {
                return i;
            }
        }
        return -1;
    }

    //cria uma transação com o próximo id e adiciona no vetor de transações
    //a transação deve ter o nome do cliente, o label e o valor
    //o MESMO objeto operaçao deve ser adicionado no vetor de operações do cliente
    //e no vetor de operações do agiota
    //incrementa o id da próxima transação
    void pushOperation(Client& cli, Label label, int value) {
        auto operation = std::make_shared<Operation>(nextOpId++, cli.getName(), label, value);
        cli.addOperation(operation);
        alive_oper.push_back(operation);
    }
    

public:
    Agiota() {
    }

    //usa a função search, se encontrar o cliente, retorne o ponteiro para ele
    //se não encontrar emite a mensagem de erro
    //e retorne nullptr
    Client* getClient(std::string name) {
        int index = searchClient(name);
        if (index == -1) {
            fn::write("fail:cliente nao existe");
            return nullptr;
        }
        return &alive_list[index];
    }

    // utilize a função search para verificar se o cliente existe
    // se ja existir, informe o erro
    // se não existir, crie um novo cliente e adicione no vetor de vivos
    // depois ordene o vetor de vivos pelo nome
    void addClient(std::string name, int limite) {
        int index = searchClient(name);
        if (index != -1) {
            fn::write("fail:cliente ja existe");
            return;
        }
        alive_list.push_back(Client(name, limite));
        std::sort(alive_list.begin(), alive_list.end(),[](Client a, Client b) {
            return a.getName() < b.getName();
        });
        
    }

    //use a função getClient para verificar se o cliente existe
    //e emitir o erro caso não exista
    //se existir, verifique se ainda é possível conceder esse valor
    //sem que o cliente fique devendo mais do que o limite
    //se for possível chame o método pushOperation do Agiota
    //passando esse cliente, o label GIVE e o valor
    void give(std::string name, int value) {
        auto cli = getClient(name);
        if (cli == nullptr)
            return;
        if (cli->getBalance() + value > cli->getLimite()) {
            fn::write("fail:limite excedido");
            return;
        }
        pushOperation(*cli, GIVE, value);
    }


    //use a função getClient para verificar se o cliente existe
    //e emitir o erro caso não exista
    //se existir, chame o método pushOperation do Agiota
    void take(std::string name, int value) {
        auto cli = getClient(name);
        if (cli == nullptr)
            return;
        pushOperation(*cli, TAKE, value);
    }

    //atenção a problemas de concorrência
    //navegue por todos os clientes vivos
    //obtenha o balance de cada um
    //adicione uma operação de PLUS para cada cliente
    //com valor igual a 10% do balance atual
    //arredonde para cima as moedas.
    //atenção na hora de fazer as divisões e conversões
    //depois de adicionar as operações de PLUS
    //verifique se algum cliente ficou devendo mais do que o limite
    //se ficou, chame o método kill
    //lembre que você pode ter problemas de concorrência
    //se tentar percorrer uma lista ao mesmo tempo que tenta modificá-la
    void plus() {
        for (auto& cli : alive_list) {
            int value = cli.getBalance() * 0.1;
            if (value == 0)
                continue;
            pushOperation(cli, PLUS, value);
        }

        std::vector<std::string> to_kill;
        for(auto& cli : alive_list) {
            if (cli.getBalance() > cli.getLimite()) {
                to_kill.push_back(cli.getName());
            }
        }
        for (auto& name : to_kill) {
            kill(name);
        }
    }

    //use a função search para verificar se o cliente existe
    //se não existir emita a mensagem de erro
    //se existir, remova o cliente do vetor de vivos
    //e adicione no final do vetor de mortos
    //percorra o vetor de transações e remova todas as transações
    //do cliente morto e adicione no vetor de transações dos mortos
    //lembre que você pode ter problemas de concorrência
    //pesquise como fazer isso
    void kill(std::string name) {
        auto index = searchClient(name);
        if (index == -1) {
            fn::write("fail: cliente nao existe");
            return;
        }
        death_list.push_back(alive_list[index]);
        alive_list.erase(alive_list.begin() + index);

        std::vector<std::shared_ptr<Operation>> sobraram;
        for (auto oper : alive_oper) {
            if (oper->getName() == name) {
                death_oper.push_back(oper);
            } else {
                sobraram.push_back(oper);
            }
        }
        alive_oper = sobraram;

    }

    std::string str() {
        std::stringstream ss;
        for (auto cli : alive_list)
            ss << ":) " << cli.str() << '\n';
        for (auto tr : alive_oper)
            ss << "+ " << tr->str() << '\n';
        for (auto cli : death_list)
            ss << ":( " << cli.str() << '\n';
        for (auto tr : death_oper)
            ss << "- " << tr->str() << '\n';
        auto output = ss.str();
        if (output.back() == '\n')
            output.pop_back();
        return output;
    }
    
};

std::ostream& operator<<(std::ostream& os, Agiota agiota) {
    return os << agiota.str();
}


int main() {
    Agiota agiota;
    while(true) {
        auto line = fn::input();
        auto args = fn::split(line, ' ');
        fn::write("$" + line);

        if(args[0] == "end") {
            break;
        } else if(args[0] == "addCli") {
            agiota.addClient(args[1], +args[2]);
        } else if(args[0] == "show") {
            agiota | fn::WRITE();
        } else if(args[0] == "showCli") {
            auto cli = agiota.getClient(args[1]);
            if (cli != nullptr) {
                cli->str() | fn::WRITE();
                cli->getOperations() | fn::JOIN("\n") | fn::WRITE();
            }
        } else if(args[0] == "kill") {
            agiota.kill(args[1]);
        } else if(args[0] == "give") {
            agiota.give(args[1], +args[2]);
        } else if(args[0] == "take") {
            agiota.take(args[1], +args[2]);
        } else if(args[0] == "plus") {
            agiota.plus();
        } else {
            fn::write("fail: comando invalido");
        }
    }
}


