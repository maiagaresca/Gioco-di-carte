#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

static struct Carta *mazzo1 = NULL;
struct Carta *ultimaCarta1 = NULL;
static struct Carta *mano1[6];
static struct Carta *campo1[4];
static struct Carta *mazzo2 = NULL;
struct Carta *ultimaCarta2 = NULL;
static struct Carta *mano2[6];
static struct Carta *campo2[4];

static int probabilitaTipo();
static int puntiVita(int i);
static void creaMazzoTenebre(struct Carta **mazzo, int n, struct Carta *ultimaCarta);
static void creaMazzoVita(struct Carta **mazzo, int n, struct Carta *ultimaCarta);
static void creaMazzoLuce(struct Carta **mazzo, int n, struct Carta *ultimaCarta);
static void stampaMazzo(struct Carta **mazzo);
static void cancellaCarta(struct Carta **mazzo, struct Carta *ultimaCarta);
static void stampaTipo(int valore);
static void creaMano(struct Carta **mazzo, struct Carta mano[], struct Carta *ultimaCarta); //andrà poi richiamata nella funzione 'imposta_gioco()'
static void stampaMano(struct Carta mano[]);
static void pesca(struct Carta **mazzo, struct Carta mano[], struct Carta *ultimaCarta);
static void giocaCarta(struct Carta campo[], struct Carta mano[]);
static void giocaCreatura(struct Carta campo[], struct Carta mano[], struct Carta *sceltaCarta);
static void stampaCampo(struct Carta campo[]);
static void attacca();
static void passa();

static int probabilitaTipo(){

  int n = 0;
  n = rand() % 99 + 1;

  if(n <= 40){
    return 0;
  }else if(n > 40 && n <= 65){
    return 1;
  }else if(n > 65 && n <= 85){
    return 2;
  }else{
    return 3;
  }
}

static int puntiVita(int i){

  /*In questo modo andrò a sezionare il mazzo in 4 parti e scegliere con un rand
    se la carta varrà 1 o 2 punti nella prima parte del mazzo, 3 o 4 punti nella
    seconda parte del mazzo e così via*/
  if(i/4 == 0){
    return rand() % 2 +7;
  }else if(i/4 == 1){
    return rand() % 2 +5;
  }else if(i/4 == 2){
    return rand() % 2 +3;
  }else{
    return rand() % 2 +1;
  }
}

static void creaMazzoTenebre(struct Carta **mazzo, int n, struct Carta *ultimaCarta){

  //RICORDA!! IL CONTENUTO E LA LOCAZIONE DI MEMORIA
  for(int i = 0; i < n; i++){
    //punto ad una locazione appena creata che non contiene nulla ma che avrà
    //come "campi" quelli di struct Carta
    struct Carta *nuovaCarta =(struct Carta*) malloc(sizeof(struct Carta));
      nuovaCarta->tipo = probabilitaTipo();  //verranno generati numeri da 0 a 3
      if(nuovaCarta->tipo == 2){
        nuovaCarta->punti_vita = 2*puntiVita(i);
      }
      nuovaCarta->punti_vita = puntiVita(i);
      nuovaCarta->successivo = NULL;

      if(*mazzo == NULL){
          //inserisco in mazzo la locazione di memoria appena creata di nuova carta
          *mazzo = nuovaCarta;
          ultimaCarta = nuovaCarta;
      }else{
        ultimaCarta->successivo = nuovaCarta;
        ultimaCarta = nuovaCarta;
      }
    }
}

static void creaMazzoVita(struct Carta **mazzo, int n, struct Carta *ultimaCarta){

  for(int i = 0; i < n; i++){
    struct Carta *nuovaCarta =(struct Carta*) malloc(sizeof(struct Carta));
      nuovaCarta->tipo = probabilitaTipo();  //verranno generati numeri da 0 a 3
      if(nuovaCarta->tipo == 0){
        nuovaCarta->punti_vita = 1.5*puntiVita(i);
      }
      nuovaCarta->punti_vita = puntiVita(i);
      nuovaCarta->successivo = NULL;

      if(*mazzo == NULL){
          *mazzo = nuovaCarta;
          ultimaCarta = nuovaCarta;
      }else{
        ultimaCarta->successivo = nuovaCarta;
        ultimaCarta = nuovaCarta;
      }
  }
}

static void creaMazzoLuce(struct Carta **mazzo, int n, struct Carta *ultimaCarta){

  for(int i = 0; i < n; i++){
    struct Carta *nuovaCarta =(struct Carta*) malloc(sizeof(struct Carta));
      nuovaCarta->tipo = probabilitaTipo(); //verranno generati numeri da 0 a 3
      if(nuovaCarta->tipo == 3){
        nuovaCarta->punti_vita = 3.5*puntiVita(i);
      }
      nuovaCarta->punti_vita = puntiVita(i);
      nuovaCarta->successivo = NULL;

      if(*mazzo == NULL){
          *mazzo = nuovaCarta;
          ultimaCarta = nuovaCarta;
      }else{
        ultimaCarta->successivo = nuovaCarta;
        ultimaCarta = nuovaCarta;
      }
  }
}

static void stampaMazzo(struct Carta **mazzo){

  if(*mazzo == NULL){
    printf("Non ci sono carte nel mazzo\n");
  }else{
    struct Carta *sentinella = *mazzo;

    do{
      printf("Tipo: ");
      stampaTipo(sentinella->tipo);
      printf("  Punti vita: %d\n", sentinella->punti_vita);
      sentinella = sentinella->successivo;
    }while(sentinella != NULL);
  }
}

static void cancellaCarta(struct Carta **mazzo, struct Carta *ultimaCarta){

  if(*mazzo == NULL){
    printf("Non ci sono carte nel mazzo\n");
  }else{
    struct Carta *cartaPrev = NULL;
    struct Carta *cartaScan = *mazzo;

    while((cartaScan->successivo) != ultimaCarta){
      cartaScan = cartaScan->successivo;
    }

    free(cartaScan->successivo);
    cartaScan->successivo = NULL;
    ultimaCarta = cartaScan;
    }
}

static void stampaTipo(int valore){
  enum Tipo_carta tipo;

  tipo = valore;

  switch (tipo) {
    case 0:
      printf("creatura\n");
      break;

    case 1:
      printf("rimuovi creatura\n");
      break;

    case 2:
      printf("infliggi danno\n");
      break;

    case 3:
      printf("cura danno\n");
      break;
  }
}

static void creaMano(struct Carta **mazzo, struct Carta mano[], struct Carta *ultimaCarta){
  for(int i = 0; i < 6; i++){
    struct Carta *sentinella = *mazzo;
    while((sentinella->successivo) != NULL){
      sentinella = sentinella->successivo;
      ultimaCarta = sentinella;
    }

    mano[i].tipo = sentinella->tipo;
    mano[i].punti_vita = sentinella->punti_vita;

    cancellaCarta(mazzo, ultimaCarta);
  }
}

static void stampaMano(struct Carta mano[]){

  for(int i = 0; i < 6; i++){
    printf("Tipo: ");
    stampaTipo(mano[i].tipo);
    printf("  Punti vita: %d\n", mano[i].punti_vita);
  }
}

/*static void pesca(struct Carta **mazzo, struct Carta mano[], struct Carta *ultimaCarta){

  for(int i = 0; i < 6; i++){
    if(mano[i] == NULL){ //controllo con NULL perché una volta che gioco la
                        //carta, poi lo slot libero lo imposto a 'NULL'
      struct Carta *sentinella = *mazzo;
      while((sentinella->successivo) != NULL){
        sentinella = sentinella->successivo;
        ultimaCarta = sentinella;
      }

      mano[i].tipo = sentinella->tipo;
      mano[i].punti_vita = sentinella->punti_vita;

      cancellaCarta(mazzo, ultimaCarta);
      break;
    }
  }
}*/

static void giocaCarta(struct Carta campo[], struct Carta mano[]){
  /*in questo caso il giocatore sceglie una delle proprie carte e sceglie un
  bersaglio su cui giocarla: la carta di tipo rimuovi_creatura può avere come
  bersaglio una delle creature del nemico sul campo, mentre infliggi_danno può
  essere giocata contro una creatura del nemico sul campo o direttamente
  contro il mago nemico , guarisci_danno può essere giocata a favore di sé stessi
  oppure su una delle proprie creature. Una creatura può essere invece giocata
  inserendola sul proprio campo. Una carta infliggi_danno infligge una danno
  pari al numeri dei suoi punti_vita ad una creatura o al mago nemico, la
  guarisci_danno incrementa i PV del mago o i punti_vita della creatura bersaglio,
  la rimuovi_creatura distrugge la creatura in campo (free). Le carte giocate
  (ad eccezione delle creature giocate) possono essere subito dopo deallocate
  (free). Ovviamente anche le creature, quando raggiungono 0 punti_vita, sono
  eliminate dal campo (free).*/
  int n = 0;


    printf("Scegliere le carte da mettere in campo\n");
    printf("Gli indici delle carte in mano vanno da 0 a 5\nSelezionare la posizione della carta nella mano: %d\n");
    scanf("%d\n", &n);

    struct Carta *sceltaCarta = mano[n];

    switch (mano[n].tipo) {
      case 0: //creatura
        giocaCreatura(campo, mano, sceltaCarta);
        printf("carta tolta dalla mano %p\n", mano[n]);
        break;

      case 1: //rimuovi_creatura
        //rimuovi;
        break;

      case 2: //infliggi_danno
        //infliggi;
        break;

      case 3: //cura_danno
        //guarisci;
        break;
    }
}

static void giocaCreatura(struct Carta campo[], struct Carta mano[], struct Carta *sceltaCarta){

  for(int j = 0; j < 4; j++){
    if(campo[j] == 0){
      campo[j].tipo = sceltaCarta->tipo;
      campo[j].punti_vita = sceltaCarta->punti_vita;

      free(sceltaCarta);
      sceltaCarta = NULL;

      break;
    }
  }
}

static void stampaCampo(struct Carta campo[]){
  for(int j = 0; j < 4; j++){
    if(campo[j] != 0){
      printf("Tipo: ");
      stampaTipo(campo[j].tipo);
      printf("  Punti vita: %d\n",  campo[j].punti_vita);
    }else{
      //metto uno spazio in più per far capire che manca una carta nello slot
      //che sto controllando
      printf("\n----\n");
    }
  }
}


static void attacca(){

}

static void passa(){

}

void imposta_gioco(){
  static struct Mago giocatore1;
  giocatore1.PV = 20;
  static struct Mago giocatore2;
  giocatore2.PV = 20;
  int n;

  printf("\nSetting per il PRIMO giocatore\n");
  printf("Il primo giocatore scelga il proprio nome.\n Nome: ");
  scanf("%s", giocatore1.nome);
  printf("\nSi scelga la classe del mago.\n Tra cui:\n '0' per tenebre\n '1' per vita\n '2' per luce.\n Scelta: ");
  scanf("%d", &giocatore1.classe);
  printf("\nSetting per il SECONDO giocatore\n");
  printf("Il secondo giocatore scelga il proprio nome.\n Nome: ");
  scanf("%s", giocatore2.nome);
  printf("\nSi scelga la classe del mago.\n Tra cui:\n '0' per tenebre\n '1' per vita\n '2' per luce.\n Scelta: ");
  scanf("%d", &giocatore2.classe);
  printf("\nSi scelga la dimensione del mazzo con cui giocare.\n Tot. carte: ");
  scanf("%d", &n);

  switch (giocatore1.classe) {
    case 0:
      creaMazzoTenebre(&mazzo1, n, ultimaCarta1);
      break;

    case 1:
      creaMazzoVita(&mazzo1, n, ultimaCarta1);
      break;

    case 2:
      creaMazzoLuce(&mazzo1, n, ultimaCarta1);
      break;
  }

  switch (giocatore2.classe) {
    case 0:
      creaMazzoTenebre(&mazzo2, n, ultimaCarta2);
      break;

    case 1:
      creaMazzoVita(&mazzo2, n, ultimaCarta2);
      break;

    case 2:
      creaMazzoLuce(&mazzo2, n, ultimaCarta2);
      break;
  }

//Da decidere se far vedere il mazzo oppure solamente la mano di ciascun giocatore
  /*printf("\nNome del giocatore: %s\n", giocatore1.nome);
  stampaMazzo(&mazzo1);
  printf("\nNome del giocatore: %s\n", giocatore2.nome);
  stampaMazzo(&mazzo2);*/

  creaMano(&mazzo1, mano1, ultimaCarta1);
  creaMano(&mazzo2, mano2, ultimaCarta2);

  printf("\nStampa della mano del primo giocatore\n");
  stampaMano(mano1);

  printf("\nStampa della mano del secondo giocatore\n");
  stampaMano(mano2);
  //creaCampo(campo1);
  //creaCampo(campo2);
}

void combatti(){
  int scelta = 0;

  printf("Cosa vuoi fare?\n '1' Per pescare una carta dal mazzo\n '2' Per giocare una carta\n");
  printf("'3' Per attaccare\n '4' Per passare la tua mano\n La tua scelta: %d");
  scanf("%d\n", &scelta);

  switch (scelta) {
    case 1:
      //pesca();
      break;

    case 2:
      giocaCarta(campo1, mazzo1);
      break;

    case 3:
      //attaccare();
      break;

    case 4:
      //passa();
      break;

    default:
      printf("La scelta inserita non è nella lista\n");
  }
}
