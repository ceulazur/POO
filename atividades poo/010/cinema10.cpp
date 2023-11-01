// o que fez: fiz tudo, menos um ou outro toString e std::string str por não entender direito como ou que colocar neles
//com quem fiz: primeiro sozinha, depois vendo videos seus e depois com o meu amigo kairo
//quanto tempo levou: pelo menos 5 horas para o moodle não me deixar compilar, sinceramente!
//o que aprendi: o uso de this, os retornos adequados novamente, realmemte como usar ->,   Cliente(std::string id = "", std::string fone = "") : id(id), fone(fone)  essa parte aqui, que é passar o valor de id e fone ( com esse : depois da parte anterior, legal demais), a usar o ponteiro inteligente shared_ptr, a criar objetos e fuções necessárias,
#include <fn.hpp>
#include <memory>
//struct 
class Cliente {
    std::string id;
    std::string fone;
   // std::string nome;
public:
    Cliente(std::string id = "", std::string fone = "") : id(id), fone(fone) {    
    }
   
   
    std::string getFone() {
        return fone;
    }
    std::string getId() {
        return id;
    }
    void setFone(std::string fone) {
       this->fone = fone;

    }
    void setId(std::string id) {
        this->id = id;
    }

  std::string str() { 
        std::ostringstream oss;
        oss << id << ":" << fone;
        return oss.str(); 
  }
};

/*  Inicializando.
  - Iniciar a sala de cinema.
    - Ao iniciar, você deve informar quantos assentos existem na sua sala.
  - Mostrar o estado da sala, escrevendo um - para cada cadeira vazia.
  - Se uma nova sala for iniciada, apague todas as informações da sala antiga. */
class Sala{
    std::vector<std::shared_ptr<Cliente>> cadeiras;
    /*  
  ' procura em todas as cadeiras não nulas
  ' pelo nome do cliente e se encontrar
  ' retorna o índice ou então -1 
    */
   public:
    int procurar(std::string nome) {
        for (int i = 0; i < cadeiras.size(); i++) {
            if (cadeiras[i]->getId() == nome) {
                return i;
            }
        }
         return -1; 
    }
    //' verifica se o indice da cadeira é válido
    bool verificarIndice(int indice) {
        if (indice >= 0 && indice < cadeiras.size()) {
            return true;
        }
        return false;
    }

//' inicia todas as cadeiras como null
    Sala(int capacidade = 0) {
        for (int i = 0; i < capacidade; i++) {
            Cliente *aux = new Cliente();
            std::shared_ptr<Cliente> clienteAux(aux);
            cadeiras.push_back(clienteAux);
        }
    }

/* 
 ' verifica se o índice é válido (use o método verificarIndice)
  ' verifica se a cadeira está ocupada
  ' verifica se o cliente já está na sala (use o método procurar)
  ' cria o cliente e coloca na cadeira
 */
    void reservar(std::string id, std::string fone, int ind) {
        if (!verificarIndice(ind)) {
            std::cout << "fail: cadeira nao existe\n";
            return;
        }
        if (cadeiras[ind]->getId() != "") {
            std::cout << "fail: cadeira ja esta ocupada\n";
            return;
        }
        if (procurar(id) != -1) {
            std::cout << "fail: cliente ja esta no cinema\n";
            return;
        }
        //criando um objeto:
        Cliente *cliente = new Cliente(id, fone);
        std::shared_ptr<Cliente> clienteAux(cliente);
        cadeiras[ind] = clienteAux;
    }
    /* 
     ' procura o cliente pelo nome (use o método procurar)
     ' remover o cliente da cadeira
     */
    void cancelar(std::string id) { 
        int valor = procurar(id);
        if (valor == -1) {
            std::cout << "fail: cliente nao esta no cinema\n";
            return;
        } 
        //magnifico:
       // cadeiras.erase(cadeiras.begin() + valor);
       cadeiras[valor]->setId("");
    
    }
 
   std::string str() {

    std::string result = "[";

        for(int i = 0; i < cadeiras.size(); i++) {
            if(cadeiras[i]->getId() == "" ) {
                result += "-";
            } else {
                result += cadeiras[i]->str();
            } 
            if (i < cadeiras.size() - 1) {
                result += " ";
            }
        }
        result += "]";
        return result;
   }
    // std::vector<std::shared_ptr<Cliente>> getCadeiras;
};


// std::ostream& operator<<(std::ostream& os, Cliente c) {t return os << c.str(); }
std::ostream& operator<<(std::ostream& os, Sala    c) { return os << c.str(); }

int main() {
    Sala sala;
    while (true) {
        auto line = fn::input();
        auto args = fn::split(line, ' ');
        fn::write("$" + line);

        if (args[0] == "end") {
            break;
        } else if (args[0] == "show") {
            fn::write(sala);
        } else if (args[0] == "init") {
            sala = Sala(+args[1]);
        } else if (args[0] == "reservar") {
            sala.reservar(args[1], args[2], (int) +args[3]);
        } else if (args[0] == "cancelar") {
            sala.cancelar(args[1]);
        } else {
            fn::write("fail: comando invalido");
        }
    }
}