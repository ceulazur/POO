#ifndef DISCIPLINA_HPP
#define DISCIPLINA_HPP

#include <string>
#include <vector>
#include <map>
#include "Aluno.hpp"


class Disciplina {
  private:
    std::string id;
    std::map<std::string, Aluno*> alunos;
  public:
    Disciplina(std::string id);

    std::string getId();

    std::vector<Aluno*> getAlunos();

    std::string toString();

    void addAluno(Aluno* aluno);

    void rmAluno(std::string idAluno);

};

#endif