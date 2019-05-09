//
// Created by DarkWayC0de_pc on 16/12/2018.
//

#include "regla.h"
namespace CyA{
  regla::regla() {

  }
  regla::~regla() {

  }
  int regla::operator==(const CyA::regla &a) const {
    if(this->simbolo_!=a.simbolo_)return 0;
    if(this->regla_!=a.regla_)return 0;
    return 1;
  }
  bool regla::regla_duplicada(std::string a) {
      for (int i = 0; i <regla_.size() ; ++i) {
          if(regla_[i].compare(a)==0){
              return true;
          }
      }
      return false;
  }

}

