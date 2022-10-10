#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main(){

    int scelta = 0, turno = 0;

  do{

    printf("\nScegliere la funzione del gioco:\n 1) Imposta il gioco\n 2) Combatti \n 3) Termina gioco\nLa tua scelta e: ");
    scanf("%d", &scelta);

    switch (scelta) {
      case 1:
        imposta_gioco();
        break;

      case 2:
        combatti();
        break;

      case 3:
        //termina_gioco();
        return 0;
        break;

      default:
        printf("Non esiste questa possibile scelta. ERRORE!!\n");
        break;
    }
  }while(1);
}
