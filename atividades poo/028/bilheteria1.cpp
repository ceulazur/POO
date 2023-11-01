#include<fn.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <sstream>
#include <string>

//[forro:50.00,·piano:15.00,·samba:70.00]↵ (expected)
//[[forro:50],·[piano:15],·[samba:70]]↵    (received)

class Pessoa;
class Evento;
class Venda;
class Bilheteria;

class Venda {
  private:
    std::shared_ptr<Pessoa> pessoa;
    std::shared_ptr<Evento> evento;
    float valor;
  public:
    Venda(std::shared_ptr<Pessoa> pessoa, std::shared_ptr<Evento> evento);
    std::shared_ptr<Pessoa> getPessoa();
    std::shared_ptr<Evento> getEvento();
    float getValor();
    std::string toString();
};

class Bilheteria {
  private:
    std::vector<Venda> repVendas;
    std::map<std::string, std::shared_ptr<Pessoa>> repPessoas;
    std::map<std::string, std::shared_ptr<Evento>> repEventos;
  public:
    Bilheteria();
    std::vector<Venda> getVendas();
    std::vector<Pessoa> getPessoas();
    std::vector<Evento> getEventos();
    std::shared_ptr<Pessoa> getPessoa(std::string nome);
    std::shared_ptr<Evento> getEvento(std::string nome);
    void addPessoa(std::string nome, bool meia);
    void addEvento(std::string nome, float preco);
    void vender(std::string nome_pessoa, std::string nome_evento);
};

class Pessoa {
  private:
    std::string nome;
    bool meia;
  public:
    Pessoa(std::string nome, bool meia);
    std::string getNome();
    bool getMeia();
    std::string toString();
};

class Evento {
  private:
    std::string nome;
    float preco;
  public:
    Evento(std::string nome, float preco);
    std::string getNome();
    float getPreco();
    std::string toString();
};

Bilheteria::Bilheteria() {}

std::vector<Venda> Bilheteria::getVendas() {
  return repVendas;
}

std::vector<Pessoa> Bilheteria::getPessoas() {
  std::vector<Pessoa> result;
  
  for(auto& par : repPessoas) {
    result.push_back(*par.second.get());
  }

  return result;
}

std::vector<Evento> Bilheteria::getEventos() {
  std::vector<Evento> result;

  for(auto& par : repEventos) {
    result.push_back(*par.second.get());
  }

  return result;
}

std::shared_ptr<Pessoa> Bilheteria::getPessoa(std::string nome) {
  return repPessoas[nome];
}

std::shared_ptr<Evento> Bilheteria::getEvento(std::string nome) {
    return repEventos[nome];
}
//$addPessoa chico meia
//fail: pessoa chico ja existe
void Bilheteria::addPessoa(std::string nome, bool meia) {
  if(repPessoas.find(nome) != repPessoas.end()) {
    std::cout << "fail: pessoa " << nome << " ja existe" << std::endl;
    return;
  }

  repPessoas[nome] = std::make_shared<Pessoa>(nome, meia);
}

void Bilheteria::addEvento(std::string nome, float preco) {
  if(repEventos.find(nome) != repEventos.end()) {
    std::cout << "fail: evento " << nome << " ja existe" << std::endl;
    return;
  }

  repEventos[nome] = std::make_shared<Evento>(nome, preco);
}

void Bilheteria::vender(std::string nome_pessoa, std::string nome_evento) {
  if(repPessoas.find(nome_pessoa) == repPessoas.end()) {
    std::cout << "fail: pessoa " << nome_pessoa << " nao existe" << std::endl;
    return;
  } else if(repEventos.find(nome_evento) == repEventos.end()) {
    std::cout << "fail: evento " << nome_evento << " nao existe" << std::endl;
    return;
  }
  
  auto pessoa = getPessoa(nome_pessoa);
  auto evento = getEvento(nome_evento);

  repVendas.push_back(Venda(pessoa, evento));    
}

bool Pessoa::getMeia() {
  return meia;
}

std::string Pessoa::toString(){
  return fn::format("{}:{}", nome, meia ? "meia" : "inteira");
}

Pessoa::Pessoa(std::string nome, bool meia) : nome(nome), meia(meia) {}

std::string Pessoa::getNome() {
  return nome;
}

Evento::Evento(std::string nome, float preco) : nome(nome), preco(preco) {};

std::string Evento::getNome() {
  return nome;
}

float Evento::getPreco() {
  return preco;
}

std::string Evento::toString() {
  return fn::format(
    "{}:{%.2f}", 
    nome, preco
  );
}

Venda::Venda(std::shared_ptr<Pessoa> pessoa, std::shared_ptr<Evento> evento) : pessoa(pessoa), evento(evento) {
  valor = evento->getPreco();

  if(pessoa->getMeia()) {
    valor /= 2.0;
  }
}

std::shared_ptr<Pessoa> Venda::getPessoa() {
  return pessoa;
}

std::shared_ptr<Evento> Venda::getEvento() {
  return evento;
}

float Venda::getValor() {
  return valor;
}

std::string Venda::toString() {
  return fn::format(
    "{}:{}:{%.2f}", 
    pessoa->getNome(), evento->getNome(), valor
  );
}

std::ostream& operator<<(std::ostream& out,  Pessoa   pessoa   ) { return out << pessoa.toString();    }
std::ostream& operator<<(std::ostream& out, Venda venda) { return out << venda.toString(); }
std::ostream& operator<<(std::ostream& out, Evento evento) { return out << evento.toString(); }



int main() {
  Bilheteria * bilheteria = new Bilheteria();

  while(true) {
    auto line = fn::input();
    fn::write("$" + line);
    auto args = fn::split(line, ' ');

    if(args[0] == "end") {
      break;
    }
    else if(args[0] == "addPessoa") {
      bilheteria->addPessoa(args[1], args[2] == "meia");
    }
    else if(args[0] == "addEvento") {
      bilheteria->addEvento(args[1], std::stof(args[2]));
    }
    else if(args[0] == "vender") {
      bilheteria->vender(args[1], args[2]);
    }

    else if(args[0] == "pessoas") {
      fn::write(bilheteria->getPessoas());
    }
    else if(args[0] == "eventos") {
      fn::write(bilheteria->getEventos());
    }
    else if(args[0] == "vendas") {
      fn::write(bilheteria->getVendas());
    }
    else{
      fn::write("fail: comando invalido");
    }
  }
}