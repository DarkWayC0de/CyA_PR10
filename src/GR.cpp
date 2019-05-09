#include "GR.h"

namespace CyA{
    GR::GR() {

    }
    GR::~GR() {

    }
    void GR::cargar(char *nombrefichero) {
        std::ifstream archivo;
        archivo.open(nombrefichero);
        if(!archivo.fail()){
            std::cout<<"Apertura de archivo correcta";
            unsigned a;
            char b;
            archivo>>a;
            for (int i = 0; i <a; ++i) {
                archivo >>b;
                simbolosterminales_.push_back(b);
            }
            archivo>>a;
            for (int j = 0; j <a ; ++j) {
                archivo>>b;
                simbolosnoterminales_.push_back(b);
            }
            for (int k = 0; k <simbolosnoterminales_.size(); ++k) {
                archivo>>b;
                regla c;
                c.simbolo_=b;
                archivo>>b;
                archivo>>b;
                char cadena[50];
                archivo.getline(cadena,50,'\n');
                std::string micadena;
                for (int i = 0; cadena[i]!='\0' ; ++i) {
                    if(cadena[i]!=' ') {
                        if (cadena[i] != '|') {
                            micadena.push_back(cadena[i]);
                        } else {
                            c.regla_.push_back(micadena);
                            micadena.clear();
                        }
                    }
                }
                c.regla_.push_back(micadena);
                reglas_.push_back(c);
            }
            archivo.close();
        }else{
            std::cout<<"Error en la apertura del archivo apertura";
        }

    }
    void GR::cargardesdeDFA(CyA::DFA A) {
        for(std::set<char>::iterator i=A.getAlfabeto_().begin(); i!=A.getAlfabeto_().end();i++){
            simbolosterminales_.push_back(i.operator*());
        }
        for(std::set<Estado>::iterator i=A.getDFA_().begin();i!=A.getDFA_().end();i++){
            regla aux;

                aux.simbolo_=calculartermino(A.getarranque_(),i->estado_);
                for (int j = 0; j <i->transiciones_; ++j) {
                    std::string aux2;
                    aux2.push_back(i->transicion_[j].first);
                    aux2.push_back(calculartermino(A.getarranque_(),i->transicion_[j].second));
                    aux.regla_.push_back(aux2);
                }

            simbolosnoterminales_.push_back(calculartermino(A.getarranque_(),i->estado_));

            if(i->aceptacion_){
                std::string aux2="~";
                aux.regla_.push_back(aux2);
            }
            reglas_.push_back(aux);
        }
        if(simbolosnoterminales_[0]!='S'){
            int cont=0;
            while(simbolosnoterminales_[cont]!='S'){
                cont++;
            }
            char temp= simbolosnoterminales_[cont];
            for (int i = 0; i <cont+1 ; ++i) {
                char temp2=simbolosnoterminales_[i];
                simbolosnoterminales_[i]=temp;
                temp=temp2;
            }
            regla temp3= reglas_[cont];
            for (int i = 0; i <cont+1 ; ++i) {
                regla temp4=reglas_[i];
                reglas_[i]=temp3;
                temp3=temp4;
            }
        }
    }
    bool GR::pertenece(char a) {
        for (int i = 0; i <reglas_.size() ; ++i) {
            if(reglas_[i].simbolo_==a){
                return true;
            }
        }
        return false;
    }
    char GR::calculartermino(unsigned arranque, unsigned actual){
        static int b= 0;
        if(arranque==actual){
            return 'S';
        }else{
            if(actual>arranque){
                actual--;
            }
            char a='A';
            for (int i = 0; i <actual+b; ++i) {
                a++;
            }
            if(a=='S'){
                a++;
                b++;
            }
            return a;
        }

    }
    void GR::exportar(char *nombrefichero) {
        std::streambuf *psbuf, *backup;
        std::ofstream filestr;
        filestr.open (nombrefichero);

        backup = std::cout.rdbuf();

        psbuf = filestr.rdbuf();
        std::cout.rdbuf(psbuf);
        write(std::cout);
        std::cout.rdbuf(backup);
        filestr.close();
    }
    std::ostream& GR::write(std::ostream &os) {
        os<<simbolosterminales_.size()<<"\n";
        for (int i = 0; i <simbolosterminales_.size(); ++i) {
            os<<simbolosterminales_[i]<<" ";
        }
         os<<"\n"<<simbolosnoterminales_.size()<<"\n";
        for (int j = 0; j <simbolosnoterminales_.size() ; ++j) {
            os<<simbolosnoterminales_[j]<<" ";
        }
        os<<"\n";
        for (int k = 0; k <reglas_.size(); ++k) {
            os<<reglas_[k].simbolo_<<" -> ";
            for (int i = 0; i <reglas_[k].regla_.size()-1 ; ++i) {
                os<<reglas_[k].regla_[i]<<" | ";
            }
            os<< reglas_[k].regla_[reglas_[k].regla_.size()-1]<<"\n";
        }
        return os;
    }
    GR GR::eliminarprvacias() {
        GR A;
        std::vector<char> contiene_vacio;
        regla aux;
        A.simbolosterminales_ = simbolosterminales_;
        for (int i = 0; i < reglas_.size(); ++i) {
            aux.simbolo_ = reglas_[i].simbolo_;
            for (int j = 0; j < reglas_[i].regla_.size(); ++j) {
                if (reglas_[i].regla_[j].size() == 1) {
                    if (reglas_[i].regla_[j].compare("~") == 0) {
                        if (reglas_[i].regla_[j].size() == 1) {
                            contiene_vacio.push_back(reglas_[i].simbolo_);
                        }
                    } else {
                        aux.regla_.push_back(reglas_[i].regla_[j]);

                    }
                } else {
                    aux.regla_.push_back(reglas_[i].regla_[j]);

                }
            }
            if (!aux.regla_.empty()) {
                A.reglas_.push_back(aux);
            }
            aux.regla_.clear();

        }
        std::vector<char> anulable,auxaunl;
        anulable = contiene_vacio;
        bool bucle;
        do{
            bucle=false;
            A.desarollo_eliminacion_anulables(anulable);
            auxaunl = A.reglas_anulables(anulable);
            if(auxaunl!=anulable){
                bucle=true;
                anulable=auxaunl;
            }
        }while(bucle);

        if(A!=*this){
            return A.eliminarprvacias();
        }
        for (int m = 0; m <A.reglas_.size() ; ++m) {
            A.simbolosnoterminales_.push_back(A.reglas_[m].simbolo_);
        }

        return A;
    }
  int GR::operator!=(const CyA::GR &A) const {
    if(this->simbolosterminales_!=A.simbolosterminales_)return 1;
    if(this->simbolosnoterminales_!=A.simbolosnoterminales_)return 1;
    if(this->reglas_!=A.reglas_)return 1;
      return 0;
    }
GR GR::eliminarprunitarias() {
        GR A;
    std::vector<char> contiene_unitarias;
    regla aux;
    A.simbolosterminales_ = simbolosterminales_;
    for (int i = 0; i < reglas_.size(); ++i) {
        aux.simbolo_ = reglas_[i].simbolo_;
        for (int j = 0; j < reglas_[i].regla_.size(); ++j) {
            if (reglas_[i].regla_[j].size() == 1) {
                bool push=false;
                for (int k = 0; k <simbolosnoterminales_.size(); ++k) {
                    std::string a;
                    a=simbolosnoterminales_[k];
                    if (reglas_[i].regla_[j].compare(a) == 0) {
                        if (reglas_[i].regla_[j].size() == 1) {
                            contiene_unitarias.push_back(reglas_[i].simbolo_);
                            push=true;
                        }
                    }
                }
                if(!push){
                    aux.regla_.push_back(reglas_[i].regla_[j]);

                }
            } else {
                aux.regla_.push_back(reglas_[i].regla_[j]);

            }
        }
        if (!aux.regla_.empty()) {
            A.reglas_.push_back(aux);
        }
        aux.regla_.clear();
    }

    desarollo_eliminacion_unitaria(contiene_unitarias,A);
    if(A!=*this){
        return A.eliminarprvacias();
    }
    for (int m = 0; m <A.reglas_.size() ; ++m) {
        A.simbolosnoterminales_.push_back(A.reglas_[m].simbolo_);
    }

    return A;
    }
    std::vector<char> GR::reglas_anulables(std::vector<char> &B) {
        std::vector<char> a,candidato;
        a=B;
        for (int i = 0; i <reglas_.size() ; ++i) {
            for (int j = 0; j <reglas_[i].regla_.size() ; ++j) {
                for (int k = 0; k <B.size() ; ++k) {
                    std::string F;
                    F.push_back(B[k]);
                    if(reglas_[i].regla_[j].compare(F)==0){
                        candidato.push_back(reglas_[i].simbolo_);
                    }
                }
            }
        }
        for (int m = 0; m <candidato.size() ; ++m) {
            bool repetido=false;
            for (int l = 0; l < a.size(); ++l) {
                if(a[l]==candidato[m]){
                    repetido= true;
                }
            }
            if(!repetido){
                a.push_back(candidato[m]);
            }
        }
        return  a;
    }
    void GR::desarollo_eliminacion_anulables(std::vector<char> anulable) {
        bool elimina=false;
        for (int x = 0; x < anulable.size(); ++x) {
            if (this->pertenece(anulable[x])) {
                for (int i = 0; i < this->reglas_.size(); ++i) {
                    for (int j = 0; j < this->reglas_[i].regla_.size(); ++j) {
                        for (int k = 0; k < this->reglas_[i].regla_[j].size(); ++k) {
                            std::string a;
                            if (this->reglas_[i].regla_[j][k] == anulable[x]) {
                                for (int l = 0; l < this->reglas_[i].regla_[j].size(); ++l) {
                                    if (l != k) {
                                        a.push_back(this->reglas_[i].regla_[j][l]);
                                    }
                                }
                                if (!this->reglas_[i].regla_duplicada(a) && !a.empty()) {
                                    this->reglas_[i].regla_.push_back(a);
                                }
                            }
                        }
                    }
                }

            } else {
                for (int j = 0; j < this->reglas_.size(); ++j) {
                    for (int i = 0; i < this->reglas_[j].regla_.size(); ++i) {
                        std::string nuevo;
                        for (int k = 0; k < this->reglas_[j].regla_[i].size(); k++) {
                            if (this->reglas_[j].regla_[i][k] == anulable[x]) {
                                elimina = true;
                            }
                            if (!elimina) {
                                nuevo.push_back(this->reglas_[j].regla_[i][k]);
                            }
                            elimina = false;
                        }
                        if ((this->reglas_[j].regla_[i].size() == 1) && (nuevo.empty())) {
                            nuevo = "~";
                        }
                        this->reglas_[j].regla_[i] = nuevo;
                    }
                }
            }
        }

    }
    void GR::desarollo_eliminacion_unitaria(std::vector<char> unitaria, GR &A) {
        GR B;
        B=A;
        for (int x = 0; x < unitaria.size(); ++x) {
            if (A.pertenece(unitaria[x])) {
                for (int i = 0; i <reglas_.size(); ++i) {
                    if( reglas_[i].simbolo_==unitaria[x]) {
                        for (int j = 0; j < reglas_[i].regla_.size(); ++j) {
                            for (int k = 0; k <simbolosnoterminales_.size(); ++k) {
                                std::string h;h=simbolosnoterminales_[k];
                                if(A.pertenece(simbolosnoterminales_[k])) {
                                    if (reglas_[i].regla_[j].size() == 1 && h == reglas_[i].regla_[j]){
                                        std::vector<std::string> f = A.buscar_reglas(simbolosnoterminales_[k]);
                                        int d = A.buscar_simbolo(unitaria[x]);
                                        for (int l = 0; l < f.size(); ++l) {
                                            if (!A.reglas_[d].regla_duplicada(f[l])) {
                                                A.reglas_[d].regla_.push_back(f[l]);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }else{/*
                GR C;
                C.simbolosterminales_=A.simbolosterminales_;
                C.simbolosnoterminales_=A.simbolosnoterminales_;
                std::vector<std::string> a;
                for (int j = 0; j < reglas_.size(); ++j) {
                    if(reglas_[j].simbolo_==unitaria[x]){
                       a =reglas_[j].regla_;
                    }
                }
                for (int i = 0; i <A.reglas_.size() ; ++i) {
                    regla p;
                    p.simbolo_=A.reglas_[i].simbolo_;
                    for (int j = 0; j <A.reglas_[i].regla_.size() ; ++j) {
                        bool contiene=false;
                        for (int k = 0; k <A.reglas_[i].regla_[j].size() ; ++k) {
                            if (reglas_[i].regla_[j][k]==unitaria[x]){
                                contiene=true;
                                std::vector<std::string> cadena;
                                int cont=0;
                                for (int m = 0; m <a.size() ; ++m) {
                                    for (int l = 0; l < reglas_[i].regla_[j].size(); ++l) {
                                        if (l = k) {
                                            if(cadena.empty()){
                                                cadena.push_back("");
                                            }
                                            cadena[cont].push_back(reglas_[i].regla_[j][l]);
                                        } else {
                                            for (int n = 0; n <a[m].size() ; ++n) {
                                                if(cadena.empty()){
                                                    cadena.push_back("");
                                                }
                                                cadena[cont].push_back(a[m][n]);
                                            }
                                        }
                                    }
                                    cont++;
                                }
                                for (int i1 = 0; i1 <cont ; ++i1) {
                                    p.regla_.push_back(cadena[i1]);
                                }
                            }
                        }
                        if(!contiene){
                            p.regla_.push_back(A.reglas_[i].regla_[j]);
                        }
                    }
                    C.reglas_.push_back(p);
                }
                A=C;*/
            }
        }
        if(A!=B){
            desarollo_eliminacion_unitaria(unitaria,A);
        }
    }
    std::vector<std::string> GR::buscar_reglas(char a){
        for (int i = 0; i <reglas_.size(); ++i) {
            if(reglas_[i].simbolo_==a){
                return reglas_[i].regla_;
            }
        }
    }
    int GR::buscar_simbolo(char a) {
        for (int i = 0; i <reglas_.size(); ++i) {
            if(reglas_[i].simbolo_==a){
                return i;
            }
        }
        return -1;
    }
    int GR::operator==(const CyA::GR &a) const {
        if(this->simbolosnoterminales_!=a.simbolosnoterminales_)return 0;
        if(this->simbolosterminales_!=a.simbolosterminales_)return 0;
        if(this->reglas_!=a.reglas_)return 0;
        return 1;
    }
}

