#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include "fn.hpp"

using namespace fn;

std::vector<int> get_men(std::vector<int> vet) {
    std::vector<int> unique;
    for (auto elem : vet){
        if (elem >  0)
            unique.push_back(elem);
    }
    return unique;
}

std::vector<int> get_calm_women(std::vector<int> vet) {
    std::vector<int> unique;
    for (auto elem : vet){
        if (elem < 0 && elem > -10)
            unique.push_back(elem);
    }
    
    return unique;
}

std::vector<int> sort(std::vector<int> vet) {
    std::sort(vet.begin(), vet.end());
    return vet;
    }

std::vector<int> sort_stress(std::vector<int> vet) {
    std::sort(vet.begin(), vet.end(), [] (int a, int b) {     
      
      if(abs (a) < abs (b)) {
        return true;
      }

      return false;
    });
    return vet;
}

std::vector<int> reverse(std::vector<int> vet) {
    std::reverse(vet.begin(), vet.end());
    return vet;
}

void reverse_inplace(std::vector<int> &vet) {
    std::reverse(vet.begin(), vet.end());
}

std::vector<int> unique(std::vector<int> vet) {
    std::vector<int> unicos; 
    for (int i = 0; i < vet.size(); i++ ) {
        bool ja_existe = false;
       for(int j = 0; j < unicos.size(); j++) {
        if(vet[i] == unicos[j]){
            ja_existe = true;
            break;
        }
       }
        if (!ja_existe) {
            unicos.push_back(vet[i]);

        }
    } 

    // std::unordered_set<int> v;
    

    // for(int i: vet) { 
    //     v.insert(i);
    // }

    // std::vector<int> ans;

    // for(int i:v) { 
    //     ans.push_back(i);
    // }

    // return ans;

    return unicos;
}

std::vector<int> repeated(std::vector<int> vet) {
    // std::vector<int> repetidos;
 
    // for (int i = 0; i < vet.size(); i++ ) {
    //    for(int j = 0; j < i; j++) {
    //         if(vet[j] == vet[i]) {
    //             repetidos.push_back(vet[i]);
    //             break;
    //         }
    //    }
    // } 

    // return repetidos;
    
    std::vector<int> unicos, repetidos;
 
    for (int i = 0; i < vet.size(); i++ ) {
        bool ja_existe = false;
       
       for(int j = 0; j < unicos.size(); j++) {
        if(vet[i] == unicos[j]){
            ja_existe = true;
            break;
        }
       }
        if (ja_existe) {
            repetidos.push_back(vet[i]);
        } else {
            unicos.push_back(vet[i]);
        }
    } 

    return repetidos;
}


int main() {
    auto str2vet = [](auto s) { return s | SLICE(1, -1) | JOIN() | SPLIT(',') | MAP(FNT(x, (int)+x)); };

    while(true) {
        std::string line = input();
        auto args = split(line, ' ');
        write('$' + line);

        if     (args[0] == "get_men"        ) { write(get_men(str2vet(args[1])));        }
        else if(args[0] == "get_calm_women" ) { write(get_calm_women(str2vet(args[1]))); }
        else if(args[0] == "sort"           ) { write(sort(str2vet(args[1])));           }
        else if(args[0] == "nl"             ) { write("\n-------------\n");              }
        else if(args[0] == "sort_stress"    ) { write(sort_stress(str2vet(args[1])));    }
        else if(args[0] == "reverse"        ) { write(reverse(str2vet(args[1])));        }
        else if(args[0] == "reverse_inplace") { 
            auto vet = str2vet(args[1]);
            reverse_inplace(vet);
            write(vet);
        }
        else if(args[0] == "unique"         ) { write(unique(str2vet(args[1])));         }
        else if(args[0] == "repeated"       ) { write(repeated(str2vet(args[1])));       }
        else if(args[0] == "end"            ) { break;                                   }
    }
}


