#include <iostream>
#include <map>
#include <vector>
#include <fn.hpp>

class Disciplina;
class Aluno;
class Sistema;

class Sistema {
  private:
    std::map<std::string, Disciplina *> discps;
    std::map<std::string, Aluno *> alunos;
  public:
    Sistema();
    ~Sistema();
    std::string toString();
    void addAluno(std::string idAluno);
    void addDiscp(std::string idDiscp);
    std::vector<Aluno*> getAlunos();
    std::vector<Disciplina*> getDiscps();
    void matricular(std::string idAluno, std::string idDiscp);
    void desmatricular(std::string idAluno, std::string idDiscp);
    void removerAluno(std::string idAluno);
    void removerDiscp(std::string idDiscp);
};

class Disciplina {
  private:
    std::string id;
    std::map<std::string, Aluno*> alunos;
  public:
    Disciplina(std::string id);
    ~Disciplina();

    std::string getId();

    std::vector<Aluno*> getAlunos();

    std::string toString();

    void addAluno(Aluno* aluno);

    void rmAluno(std::string idAluno);

};

class Aluno {
  private:
    std::string id;
    std::map<std::string, Disciplina*> discps;
  public:
    Aluno(std::string id);
    ~Aluno();
    std::string getId();
    std::vector<Disciplina*> getDiscps();
    std::string toString();
    void addDiscp(Disciplina* discp);
    void rmDiscp(std::string idDiscp);
};

Aluno::Aluno(std::string id) : id(id) {};

Aluno::~Aluno() {
  for(std::pair<std::string, Disciplina *> discp: this->discps) {
    discp.second->rmAluno(this->id);
  }
};

std::string Aluno::getId(){
  return this->id;
};

std::vector<Disciplina*> Aluno::getDiscps() {
  
  std::vector<Disciplina*> v_discps;
  
  for(std::pair<std::string, Disciplina *> discp: this->discps) {
    v_discps.push_back(discp.second);
  }

  return v_discps;
}

std::string Aluno::toString(){
    std::vector<Disciplina*> discps =  getDiscps();
    std::string result  = id + " ["; 
    for (int i = 0; i < discps.size(); i++) {
        if (i == 0) {
            result += discps[i]->getId();

        } else {
            result += ", " + discps[i]->getId();
        }
    }
    result += "]";
    return result;
};

void Aluno::addDiscp(Disciplina* discp){
    this->discps[discp->getId()] = discp;  
};

void Aluno::rmDiscp(std::string idDiscp){
    this->discps.erase(idDiscp); 
};

Disciplina::Disciplina(std::string id) : id(id) {};

Disciplina::~Disciplina() {
  for(std::pair<std::string, Aluno *> aluno: this->alunos) {
    aluno.second->rmDiscp(this->id);
  }
};
    
std::string Disciplina::getId(){
  return this->id;
};

std::vector<Aluno*> Disciplina::getAlunos() {
    std::vector<Aluno*> v_alu;
  
  for(std::pair<std::string, Aluno *> discp: this->alunos) {
    v_alu.push_back(discp.second);
  }

  return v_alu;
};

std::string Disciplina::toString(){
    std::vector<Aluno*> alu =  getAlunos();
    std::string result  = id + " ["; 
    for (int i = 0; i < alunos.size(); i++) {
        if (i == 0) {
            result += alu[i]->getId();

        } else {
            result += ", " + alu[i]->getId();
        }
    }
    result += "]";
    return result;
};

void Disciplina::addAluno(Aluno* aluno){
    this->alunos[aluno->getId()] = aluno;  
};

void Disciplina::rmAluno(std::string idAluno){
    this->alunos.erase(idAluno);  
};

Sistema::Sistema() {
  this->discps = std::map<std::string, Disciplina*>();
  this->alunos = std::map<std::string, Aluno*>();
}

Sistema::~Sistema() {
  for(std::pair<std::string, Disciplina *> discp: this->discps) {
    delete discp.second;
  }

  for(std::pair<std::string, Aluno *> aluno: this->alunos) {
    delete aluno.second;
  }
}

std::string Sistema::toString(){
    std::string result = "- alunos\n";
    for(std::pair<std::string, Aluno *>  aluno: alunos) {
        result += aluno.second->toString() + "\n";
    }

    result += "- discps\n";
    for(std::pair<std::string, Disciplina *>  discp:discps) {
        result += discp.second->toString() + "\n";

    }

    result = result.substr(0, result.size() - 1);
    
    return result;
};

void Sistema::addAluno(std::string idAluno) {
  Aluno *aluno = new Aluno(idAluno);

  this->alunos[idAluno] = aluno;
};

void Sistema::addDiscp(std::string idDiscp){
  Disciplina *discp = new Disciplina(idDiscp);

  this->discps[idDiscp] = discp;
};

std::vector<Aluno*> Sistema::getAlunos(){
  std::vector<Aluno*> v_alunos;
  
  for(std::pair<std::string, Aluno *> aluno: this->alunos) {
    v_alunos.push_back(aluno.second);
  }

  return v_alunos;
};

std::vector<Disciplina*> Sistema::getDiscps() {
  std::vector<Disciplina*> v_discps;
  
  for(std::pair<std::string, Disciplina *> discp: this->discps) {
    v_discps.push_back(discp.second);
  }

  return v_discps;
}

void Sistema::matricular(std::string idAluno, std::string idDiscp){
  this->alunos[idAluno]->addDiscp(this->discps[idDiscp]);
  this->discps[idDiscp]->addAluno(this->alunos[idAluno]);
};

void Sistema::desmatricular(std::string idAluno, std::string idDiscp){
  this->alunos[idAluno]->rmDiscp(idDiscp);
  this->discps[idDiscp]->rmAluno(idAluno);
};

void Sistema::removerAluno(std::string idAluno){
  this->alunos.erase(idAluno);
  
  for(auto discp: this->discps) {
    discp.second->rmAluno(idAluno);
  }
};

void Sistema::removerDiscp(std::string idDiscp){
  this->discps.erase(idDiscp);   
};

int main() {
  Sistema sis = Sistema();
  std::string op = "";

  while (true) {
      auto line = fn::input();
      fn::write("$" + line);

      auto args = fn::split(line);
      
      if      (args[0] == "end"    )
        break;
      else if (args[0] == "nwalu"  ) { 
        for(int i = 1; i < args.size(); i++)
          sis.addAluno(args[i]);
      } 
      else if (args[0] == "nwdis"  ) { 
        for(int i = 1; i < args.size(); i++)
          sis.addDiscp(args[i]);
      }
      else if (args[0] == "show"   ) 
        fn::write(sis.toString());
      else if (args[0] == "tie"    ) { 
        for(int i = 2; i < args.size(); i++)
          sis.matricular(args[1], args[i]);
      }
      else if (args[0] == "untie"  ) { 
        for(int i = 2; i < args.size(); i++)
          sis.desmatricular(args[1], args[i]);
      }
      else if (args[0] == "rmalu"  ) { 
        sis.removerAluno(args[1]);
      }
      else if (args[0] == "rmdis"  ) { 
        sis.removerDiscp(args[1]);
      }
      else {
        fn::write("fail: invalid command");
      }
  }


  return 0;
}