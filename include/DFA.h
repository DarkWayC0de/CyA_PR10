//
// Created by DarkWayC0de_pc on 17/11/2018.
//

#ifndef PR6_DFA_H
#define PR6_DFA_H

#include <iostream>
#include <set>
#include "Estado.h"
#include <algorithm>

namespace CyA {
  class DFA {
  private:
    std::set<Estado> DFA_;
    std::set<char>Alfabeto_;
    unsigned nestados_,arranque_,contwrite;

    void build(char nombrefichero[]);
    void analizar(std::ifstream &a);
    bool aceptacion(unsigned a);
    std::set<std::set<Estado>> partir(std::set<std::set<Estado>>);
    std::set<std::set<Estado>> desarrollo(std::set<Estado> ,std::set<std::set<Estado>>);
    std::set<std::set<Estado>> divi(std::set<Estado> estado,char simbolo,std::set<std::set<Estado>> L);
    std::ostream& writeconj(std::ostream &os,std::set<std::set<Estado>> );
    void construir(std::set<std::set<Estado>>);
    unsigned destino(std::set<std::set<Estado>>, unsigned );

  public:
    DFA();
    ~DFA();
    void cargar(char nombrefichero[]);
    bool vasio();
    std::ostream& write(std::ostream &os);
    void muerte(void);
    void cadena(void);
    unsigned siguiente(unsigned origen,char entrada,bool &aceptado);
    void minimize();
    void Exportar();
    std::set<Estado>& getDFA_();
    std::set<char>& getAlfabeto_();
    unsigned& getnestados_();
    unsigned& getarranque_();
  };
}

#endif //PR6_DFA_H
