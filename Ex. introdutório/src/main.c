#include <stdio.h>
#include "arquivos.h"

int main(){
    
    int funcionalidade;
    scanf("%d", &funcionalidade);

    switch (funcionalidade)
    {
    case 1:{
        char arquivoEntrada[20];
        scanf("%s", arquivoEntrada);
        funcionalidade1(arquivoEntrada);
        break;
    }
    default:
        break;
    }


    return 0;
}