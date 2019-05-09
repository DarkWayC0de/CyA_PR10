

#include <fstream>
#include <iostream>
#include <iterator>
#include "DFA.h"
namespace CyA{
    DFA::DFA() {

    }
    DFA::~DFA() {

    }
    void DFA::cargar(char *nombrefichero) {
        if(!DFA_.empty()){
            DFA_.clear();
            Alfabeto_.clear();
            nestados_=0;
            arranque_=0;
        }
        build(nombrefichero);  //rev
    }
    void DFA::build(char *nombrefichero){
        std::ifstream archivo;
        archivo.open(nombrefichero);
        if(!archivo.fail()){
          std::cout<<"Apertura de archivo correcta";
           archivo>>nestados_;
           archivo>>arranque_;
           for (int i = 0; i < nestados_; ++i) {
              Estado aux;
              archivo>>aux.estado_>>aux.aceptacion_>>aux.transiciones_;
              for (int j = 0; j <aux.transiciones_; ++j) {
                  std::pair<char, unsigned> auxp;
                  archivo>>auxp.first>>auxp.second;
                  Alfabeto_.insert(auxp.first);
                  aux.transicion_.push_back(auxp);
              }
              DFA_.insert(aux);
           }
           analizar(archivo);

           archivo.close();
        }else{
            std::cout<<"Error en la apertura del archivo apertura";
        }
    }
    void DFA::analizar(std::ifstream &a) {
        bool erro=false;
        for (std::set<Estado>::iterator i= DFA_.begin();i!=DFA_.end();i++) {
            for (int j = 0; j <i->transiciones_ ; ++j) {
                for (int k = 0; k <i->transiciones_ ; ++k) {
                    if(j!=k&&(i->transicion_[j].first==i->transicion_[k].first)){
                        if(i->transicion_[j].second!=i->transicion_[k].second) {
                            erro = true;
                        }
                    }
                }
            }
        }
        if(nestados_!=DFA_.size()){
            erro=true;
        }
        if(erro){
            std::cout << "\nError definicion de automata";
            DFA_.clear();
            Alfabeto_.clear();
        } else{
            std::cout <<"\nCarga correcta";
        }
    }
    bool DFA::vasio() {
        return DFA_.empty();
    }
    std::ostream& DFA::write(std::ostream &os) {
        os<<"DFA\n"
            <<nestados_<<"\n"
            <<arranque_<<"\n";
        for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
            os<<i->estado_<<" "<<i->aceptacion_<<" "<<i->transiciones_<<" ";
            for (int j = 0; j <i->transiciones_ ; ++j) {
                os<<i->transicion_[j].first<<" "<<i->transicion_[j].second<<" ";
            }
            os<<"\n";
        }


    }
    void DFA::muerte() {
        int muerto;
        bool vivo=true;
        std::cout<<"Estados de muerte: ";
        for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
            muerto=0;
            for (int j = 0; j <i->transiciones_ ; ++j) {
                if(i->transicion_[j].second==i->estado_){
                    muerto++;
                }
            }
            if(muerto==(i->transiciones_)){
                vivo=false;
                std::cout<<"\nEl estado "<<i->estado_<<" es un estado de muerte porque todas sun transiciones apuntan a si mismo como vemos aqui: ";
                for (int j = 0; j <i->transiciones_ ; ++j) {
                    std::cout<<i->transicion_[j].first<<" "<<i->transicion_[j].second<<" ";
                }
                std::cout<<"";
            }
        }
        if(vivo){
            std::cout<<"\nEste DFA no tiene estados de muerte";
        }
    }
    void DFA::cadena() {
        char cadena[100];
        bool aceptado=true;
        unsigned a;
        std::cout<<"Introduce la cadena a Analizar: ";
        std::cin>>cadena;
        a=siguiente(arranque_,cadena[0],aceptado);
        std::cout<<"Cadena de entrada:"<<cadena
            <<"\nEstado actual\tEntrada\t\tSiguiente estado\n"
            <<arranque_<<"\t\t"<<cadena[0]<<"\t\t"<<a<<"\n";

        for (int i = 1; cadena[i]!='\0'&&aceptado; ++i) {
            std::cout<<a<<"\t\t"<<cadena[i]<<"\t\t"<<siguiente(a,cadena[i], aceptado)<<"\n";
            a=siguiente(a,cadena[i], aceptado);
        }

        if (aceptado&&aceptacion(a)){
            std::cout<<"Cadena de entrada ACEPTADA";
        } else{
            std::cout<<"Cadena de entrada No ACEPTADA";
        }
    }
    unsigned DFA::siguiente(unsigned origen, char entrada, bool &aceptado) {
        for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
            if(i->estado_==origen){
                for (int j = 0; j <i->transiciones_ ; ++j) {
                    if(i->transicion_[j].first==entrada){
                        return i->transicion_[j].second;
                    }
                }
            }
        }
        aceptado=false;
        return origen;
    }
    bool DFA::aceptacion(unsigned a) {
        for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
            if(i->estado_==a){
                return i->aceptacion_;
            }
        }
        return false;
    }
    void DFA::minimize() {
        contwrite=0;
        std::set<Estado> aceptacion_,noaceptacion_;
        std::set<std::set<Estado>> particion,viejaparticion;
        for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
            if(i->aceptacion_){
                aceptacion_.insert(i.operator*());
            }else{
                noaceptacion_.insert(i.operator*());
            }
        }

        particion.insert(aceptacion_);
        particion.insert(noaceptacion_);
        writeconj(std::cout,particion);
        do{
            viejaparticion=particion;
            particion=partir(viejaparticion);

        }while(particion!=viejaparticion);
        std::cout<<"\nEl DFA minimo tiene "<<particion.size()<<" estados\n";
        //construir automata
        construir(particion);
    }
    std::set<std::set<Estado>>  DFA::partir(std::set<std::set<CyA::Estado>> apartir) {
        std::set<std::set<Estado>> resultadoparticion;
        resultadoparticion.clear();
        for (std::set<std::set<Estado>>::iterator conjunto=apartir.begin();conjunto!=apartir.end();conjunto++) {
            std::set<std::set<Estado>> conjuntoaux;
            conjuntoaux = desarrollo(conjunto.operator*(),apartir);
            resultadoparticion.insert(conjuntoaux.begin(),conjuntoaux.end());
        }
        return resultadoparticion;
    }
    std::set<std::set<Estado>> DFA::desarrollo(std::set<CyA::Estado> conjunto, std::set<std::set<CyA::Estado>> apartir) {
        std::set<std::set<Estado>> rparcial;
        rparcial.insert(conjunto);

        for(std::set<char>::iterator simbolo=Alfabeto_.begin();simbolo!=Alfabeto_.end();simbolo++){
            std::set<std::set<Estado>> cnjacumulador;
            cnjacumulador.clear();
            for(std::set<std::set<Estado>>::iterator itrparcial=rparcial.begin();itrparcial!=rparcial.end();itrparcial++){
                std::set<std::set<Estado>> auxrparcial;
                auxrparcial=divi(itrparcial.operator*(),simbolo.operator*(),apartir);
                cnjacumulador.insert(auxrparcial.begin(),auxrparcial.end());
            }
            rparcial=cnjacumulador;
            if(conjunto!=rparcial.begin().operator*()){
                std::set<std::set<Estado>> aux,aux2;
                aux2.insert(conjunto);
                std::cout<<"\nCon el simbolo '"<<simbolo.operator*()<<"' :";
                std::set_difference(apartir.begin(),apartir.end(),aux2.begin(),aux2.end(),inserter(aux,aux.begin()));
                aux.insert(rparcial.begin(),rparcial.end());
                writeconj(std::cout,aux);
            }
        }
        return rparcial;
    }
    std::set<std::set<Estado>> DFA::divi(std::set<Estado> cnj,char simbolo, std::set<std::set<Estado>> apartir){
        std::set<std::set<Estado>> actual;
        std::set<Estado> actual2,miactual, actual3;
        unsigned cont =0;
        for(std::set<std::set<Estado>>::iterator i =apartir.begin();i!=apartir.end();i++){
            for (std::set<Estado>::iterator j=i->begin();j!=i->end();j++){
                for (std::set<Estado>::iterator estado= cnj.begin();estado!=cnj.end();estado++){
                    for (int k = 0; k <estado->transiciones_ ; ++k) {
                        if(estado->transicion_[k].first==simbolo){
                            if(j->estado_==estado->transicion_[k].second) {
                                cont++;
                                actual3.insert(estado.operator*());
                            }
                        }
                    }
                }
            }
            if(cont!=cnj.size()&&cont!=0||cont==cnj.size()){
                actual.insert(actual3);
                actual3.clear();
            }
            cont=0;
        }
        return actual;
    }
    std::ostream& DFA::writeconj(std::ostream &os, std::set<std::set<Estado>> conj) {

        os<<"\nP"<<contwrite++<<" = {";
        for (std::set<std::set<Estado>>::iterator i=conj.begin();i!=conj.end();i++) {
            os << "{";
            for (std::set<Estado>::iterator j = i->begin(); j != i->end(); j++) {
                os << j->estado_ <<" ";
            }
            os<<"} ";

        }
        os<<"}";

    }
    void DFA::construir(std::set<std::set<CyA::Estado>> conj) {
        std::set<Estado> newdfa;
        unsigned contestados=0;
        for (std::set<std::set<Estado>>::iterator i=conj.begin();i!=conj.end();i++){
            Estado aux;

            aux.estado_=contestados++;
            aux.transiciones_=Alfabeto_.size();
            for(std::set<Estado>::iterator j=i->begin();j!=i->end();j++){
                aux.aceptacion_=false;
                if(j->aceptacion_){
                    aux.aceptacion_=true;
                }
                for(std::set<char>::iterator l=Alfabeto_.begin();l!=Alfabeto_.end();l++){
                    std::pair<char,unsigned> pairaux;
                    for (int k = 0; k <j->transiciones_; ++k) {
                        if(l.operator*()==j->transicion_[k].first){
                            pairaux.second=destino(conj,j->transicion_[k].second);
                            pairaux.first=l.operator*();
                        }
                    }
                    aux.transicion_.push_back(pairaux);
                }
            }

            newdfa.insert(aux);
        }
        this->nestados_=contestados;
        this->arranque_=destino(conj,arranque_);
        DFA_=newdfa;
    }
    unsigned DFA::destino(std::set<std::set<CyA::Estado>> cnj, unsigned nbus) {
        unsigned cont=0;
        for(std::set<std::set<Estado>>::iterator i=cnj.begin();i!=cnj.end();i++){
            for (std::set<Estado>::iterator j=i->begin();j!=i->end();j++) {
                if(j->estado_==nbus){
                    return cont;
                }
            }
            cont++;
        }
    }
    void DFA::Exportar() {
        char nombrefichero[50];
        std::cout<<"Introduzca el nombre del fichero a crear:";
        std::cin>>nombrefichero;
        std::streambuf *psbuf, *backup;
        std::ofstream filestr;
        filestr.open (nombrefichero);

        backup = std::cout.rdbuf();

        psbuf = filestr.rdbuf();
        std::cout.rdbuf(psbuf);
        std::cout <<nestados_<<"\n"
                <<arranque_<<"\n";
                for(std::set<Estado>::iterator i=DFA_.begin();i!=DFA_.end();i++){
                    std::cout<<i->estado_<<" "<<i->aceptacion_;
                    for (int j = 0; j <i->transiciones_; ++j) {
                        std::cout<<" "<<i->transicion_[j].first<<" "<<i->transicion_[j].second;
                    }
                    std::cout<<"\n";
                }
        std::cout.rdbuf(backup);
        filestr.close();
    }
    std::set<char>& DFA::getAlfabeto_(){
        return Alfabeto_;
    }
    unsigned& DFA::getarranque_(){
        return arranque_;
    }
    std::set<Estado>& DFA::getDFA_(){
        return DFA_;
    }
    unsigned& DFA::getnestados_(){
        return nestados_;
    }

}

