#include <iostream>
#include <sstream>
#include <vector>

struct GRAFITE {
  float calibre; 
  int tamanho;   
  std::string dureza;

  GRAFITE(float calibre = 0.0f, int tamanho = 0, std::string dureza = "NULL") : calibre{calibre}, tamanho{tamanho}, dureza{dureza} {
  }

  int desgastePorFolha() {
    if (dureza == "HB"){
      return 1;
    } else if (dureza == "2B") {
      return 2;
    } else if (dureza == "4B") {
      return 4;
    } else if (dureza == "6B") {
      return 6;
    }

    return 0;
  }

  std::string toString(int cont) {
    std::string aux = "Grafite " + std::to_string(cont) + ": [" + std::to_string(this->calibre) + " : " + dureza + " : " + std::to_string(this->tamanho) + "]";
    return aux;
  }
};

struct LAPISEIRA {
  float calibre; // Em mm.
  std::vector<GRAFITE> grafites;

  LAPISEIRA(float calibre = 0.0f) : calibre{calibre} {
  }

  bool inserir(GRAFITE *grafite) {
    int soma{0};

    if (grafite->calibre != this->calibre) {
      std::cout << "ERROR! Calibre incompativel\n";
      return false;
    }
    for (int i{0}; i < (int)this->grafites.size(); i++) {
      soma += this->grafites[i].tamanho;
    }
    if ((soma + grafite->tamanho) > 150) {
      int falta = 150 - soma;
      std::cout << "ERROR! Lapiseira cheia. Nao coloque um grafite maior que " << falta << std::endl;
      return false;
    }
    grafites.push_back(*grafite);
    return true;
  }

  GRAFITE *remover() {
    GRAFITE *cis = &this->grafites[(int)this->grafites.size() - 1];
    this->grafites.pop_back();
    return cis;
  }

  bool grafiteInutil(int tamanho) {
    if (tamanho <= 6) {
      return true;
    }
    return false;
  }

  bool write(int folhas) {
    int follhasEscritas{0};

    if ((int)this->grafites.size() == 0) {
      std::cout << "ERROR! Sem grafite na lapiseira\n";
      return false;
    }

    while (folhas > 0) {
      if (this->grafites[0].tamanho == 0 || grafiteInutil(this->grafites[0].tamanho)) {
        if ((int)this->grafites.size() == 1) {
          std::cout << "\nTexto incompleto!\n";
          std::cout << follhasEscritas << " folhas foram escritas\n";

          std::cout << "\nCabou o grafite :/\n";
        }

        this->grafites.erase(this->grafites.begin());
        if ((int)this->grafites.size() == 0) {
          return false;
        }
      }
      if (!grafiteInutil(this->grafites[0].tamanho)) {
        int desgaste = this->grafites[0].desgastePorFolha();
        this->grafites[0].tamanho -= desgaste;
        follhasEscritas++;
        folhas--;
      }
    }

    if (folhas == 0 && this->grafites[0].tamanho == 0) {
      this->grafites.erase(this->grafites.begin());
      if ((int)this->grafites.size() == 0) {
        std::cout << "\nCabou o grafite :/\n";
      }
      return false;
    }

    return true;
  }

  void caiu() {
    if ((int)this->grafites.size() == 0) {
      std::cout << "Man, tua lapiseira caiu mas ainda bem que nao tinha grafite\n";
      return;
    }

    std::vector<GRAFITE> aux;
    for (int i{0}; i < (int)this->grafites.size(); i++) {
      if (this->grafites[i].tamanho > 1) {
        int tamanho = this->grafites[i].tamanho;
        int div{2};
        while (true) {
          if (tamanho % div == 0) {
            break;
          }
          div++;
        }

        this->grafites[i].tamanho = tamanho / div;
        for (int j{0}; j < div; j++) {
          aux.push_back(this->grafites[i]);
        }
      } else if (this->grafites[i].tamanho == 1) {
        aux.push_back(this->grafites[i]);
      }
    }
    this->grafites.clear();
    this->grafites = aux;
    return;
  }

  std::string toString() {
    std::string aux = "Calibre: " + std::to_string(this->calibre) + ", ";
    return aux;
  }
};

int main()
{
  LAPISEIRA bic;

  while (true) {
    std::cout << "$ ";

    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    std::string comando;
    ss >> comando;

    if (comando == "end") {
      break;
    } else if (comando == "init") {
      float calibre{0};
      ss >> calibre;
      bic.calibre = calibre;
    } else if (comando == "show") {
      std::cout << bic.toString();
      if ((int)bic.grafites.size() == 0) {
        std::cout << "Grafite: null\n";
      } else {
        for (int i{0}; i < (int)bic.grafites.size(); i++) {
          std::cout << bic.grafites[i].toString(i + 1) << std::endl;
        }
      }
    } else if (comando == "inserir") {
      float calibre{0};
      std::string dureza;
      int tamanho;

      ss >> calibre >> dureza >> tamanho;
      GRAFITE *grafite = new GRAFITE(calibre, tamanho, dureza);

      if (!bic.inserir(grafite)) {
        delete grafite;
      }
    }
    else if (comando == "remover") {
      GRAFITE *grafite = bic.remover();
      if (grafite != nullptr) {
        delete grafite;
      } else {
        std::cout << "Tem grafite na lapiseira nao man!\n";
      }
    } else if (comando == "write") {
      int folhas{0};

      ss >> folhas;
      bic.write(folhas);
    }
    else if (comando == "caiu") {
      bic.caiu();
    } else {
      std::cout << "Ei po, tu nao digitou o comando da forma certa.\n";
    }
  }

  return 0;
}