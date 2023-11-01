#ifndef ALUNO_H
#define ALUNO_H

#include <string>
#include <vector>
#include <map>
#include "Disciplina.hpp"

class Aluno {
  private:
    std::string id;
    std::map<std::string, Disciplina*> discps;
  public:
    Aluno(std::string id);
    std::string getId();
    std::vector<Disciplina*> getDiscps();
    std::string toString();
    void addDiscp(Disciplina* discp);
    void rmDiscp(std::string idDiscp);
};

#endif