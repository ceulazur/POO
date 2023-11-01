#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Aluno.hpp"
#include "Disciplina.hpp"

class Sistema {
  private:
    std::map<std::string, Disciplina*> discps;
    std::map<std::string, Aluno*> alunos;
  public:
    Aluno(std::string idAluno);
    Disciplina(std::string idDiscp);
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