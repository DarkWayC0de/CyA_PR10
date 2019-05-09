#include <iostream>
//#include "DFA.h"
#include "GR.h"

int main(int _argc,char *_argv[]){
   if(_argc==3){
        CyA::GR B;
        B.cargar(_argv[1]);
        CyA::GR C;
        C=B.eliminarprvacias();
        //C.exportar(_argv[2]);
        C=C.eliminarprunitarias();
        C.exportar(_argv[2]);
    }else{
        return 1;
    }
    return 0;
}
