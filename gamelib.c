#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"

static struct Mago giocatore1;
static struct Mago giocatore2;
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
static void creaMano(struct Carta **mazzo, struct Carta *mano[], struct Carta *ultimaCarta); //andrà poi richiamata nella funzione 'imposta_gioco()'
static void stampaMano(struct Carta *mano[]);
static void pesca(struct Carta **mazzo, struct Carta *mano[], struct Carta *ultimaCarta);
static void giocaCarta(struct Mago *giocatore, struct Mago *nemico, struct Carta *campoGiocatore[], struct Carta *manoGiocatore[], struct Carta *campoNemico[]);
static void giocaCreatura(struct Carta *campo[], struct Carta *mano[], int n);
static void rimuoviCreatura(struct Carta *campo[]);
static void infliggiDanno(struct Carta *campo[], struct Mago *giocatore, int puntiVita);
static void guarisciDanno(struct Carta *campo[], struct Mago *giocatore, int puntiVita);
static void stampaCampo(struct Carta *campo[]);
static void sceltaCarta(struct Carta *campo[], int utilizzo[]);
static void controCreatura(struct Carta *campoGiocatore[], struct Carta *campoNemico[], int scelta, int utilizzo[]);
static void controMago(struct Mago *nemico, struct Carta *campo[], int scelta, int utilizzo[]);
static void attacca(struct Mago *nemico, struct Carta *campoGiocatore[], struct Carta *campoNemico[]);
static void controlloCarte(struct Mago *giocatore, struct Mago *nemico);
static void passa();

void termina_gioco(){

  while(mazzo1 != NULL){
    struct Carta *sentinella = (*mazzo1).successivo;
    free(mazzo1);
    mazzo1 = sentinella;
  }

  while(mazzo2 != NULL){
    struct Carta *sentinella = (*mazzo2).successivo;
    free(mazzo2);
    mazzo2 = sentinella;
  }
  exit(0);
}

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
    stampaMazzo(mazzo);
    struct Carta *cartaPrev = NULL;
    struct Carta *cartaScan = *mazzo;

    while((cartaScan->successivo) != ultimaCarta){
      cartaScan = cartaScan->successivo;
    }

    //free(cartaScan->successivo);
    cartaScan->successivo = NULL;
    ultimaCarta = cartaScan;

    //se rimane una sola carta nel mazzo, posso cancellarla e quindi avrò che nel mazzo non ci sono più carte
    if((*mazzo)->successivo == NULL){
      ultimaCarta = NULL;
      *mazzo = NULL;
    }
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

static void creaMano(struct Carta **mazzo, struct Carta *mano[], struct Carta *ultimaCarta){

  for(int i = 0; i < 6; i++){
    struct Carta *sentinella = *mazzo;

    while((sentinella->successivo) != NULL){
      sentinella = sentinella->successivo;
      ultimaCarta = sentinella;
    }

    mano[i] = sentinella;

    cancellaCarta(mazzo, ultimaCarta);

  }
}

static void stampaMano(struct Carta *mano[]){

  for(int i = 0; i < 6; i++){
    if(mano[i] != NULL){
      printf("%d", i);
      printf(" Tipo: ");
      stampaTipo(mano[i]->tipo);
      printf("    Punti vita: %d\n", mano[i]->punti_vita);
    }else{
      printf("%d", i);
      printf(" ----\n");
    }
  }
}

static void pesca(struct Carta **mazzo, struct Carta *mano[], struct Carta *ultimaCarta){

  int pesca = 0;

  if(*mazzo == NULL){
    printf("Il mazzo ha terminato le carte\n");
    controlloCarte(&giocatore1, &giocatore2);
  }else{
    for(int i = 0; i < 6; i++){
      if(mano[i] == NULL){ //controllo con NULL perché una volta che gioco la
                          //carta, poi lo slot libero lo imposto a 'NULL'
        struct Carta *sentinella = *mazzo;

        while((sentinella->successivo) != NULL){
          sentinella = sentinella->successivo;
          ultimaCarta = sentinella;
        }

        mano[i]= sentinella;

        cancellaCarta(mazzo, ultimaCarta);
        pesca = 1;
        break;
      }
    }

    if(pesca == 0){
      printf("Hai la mano piena, non puoi pescare altre carte\n");
    }
  }
}

static void giocaCarta(struct Mago *giocatore, struct Mago *nemico, struct Carta *campoGiocatore[], struct Carta *manoGiocatore[], struct Carta *campoNemico[]){

  int n = 0;

  printf("\nScegliere le carte da giocare\n");
  stampaMano(manoGiocatore);
  printf("Gli indici delle carte in mano vanno da 0 a 5\nSelezionare la posizione della carta nella mano: ");
  scanf("%d", &n);

  if(manoGiocatore[n] == NULL){
    printf("La carta scelta non è presente in mano\n");
  }else{
    switch (manoGiocatore[n]->tipo){
      case 0:
        giocaCreatura(campoGiocatore, manoGiocatore, n);
        stampaCampo(campoGiocatore);
        break;

      case 1:
        if(campoNemico != NULL){
          rimuoviCreatura(campoNemico);
        }else{
          printf("Non ci sono creature nel campo nemico\n");
        }
        break;

      case 2:
        infliggiDanno(campoNemico, nemico, manoGiocatore[n]->punti_vita);
        break;

      case 3:
        guarisciDanno(campoGiocatore, giocatore, manoGiocatore[n]->punti_vita);
        break;
    }

    manoGiocatore[n] = NULL;
  }
}

static void giocaCreatura(struct Carta *campo[], struct Carta *mano[], int n){

  for(int i = 0; i < 4; i++){
    if(campo[i] == NULL){
      campo[i] = mano[n];

      break;
    }
  }
}

static void rimuoviCreatura(struct Carta *campo[]){
  int rimuovi = 0;

  struct Carta *rimuovicreatura = NULL;

  printf("Gli indici delle carte vanno da 0 a 3\n");
  printf("Si scelga quale carta si vuole rimuovere dal campo nemico\n");
  scanf("%d", &rimuovi);

  if(campo[rimuovi] == NULL){
    printf("La posizione della carta scelta non è sul campo\n");
  }else{
    campo[rimuovi] = rimuovicreatura;

    free(rimuovicreatura);
    campo[rimuovi] = NULL; //per indicare che quello slot è libero
  }
}

static void infliggiDanno(struct Carta *campo[], struct Mago *giocatore, int puntiVita){
  int infliggi = 0, scelta = 0;

  printf("Si scelga se:\n");
  printf("'1' Si vuole infliggere danno ai punti vita del mago\n ");
  printf("'2' Si vuole infliggere danno alle creature\n");
  printf("Scelta: ");
  scanf("%d", &infliggi);

  switch (infliggi) {
    case 1:
      (*giocatore).PV -= puntiVita;
      if((*giocatore).PV <= 0){
        controlloCarte(&giocatore1, &giocatore2);
      }
      break;

    case 2:
    printf("Stampa in corso del campo nemico\n");
    stampaCampo(campo);

    printf("Gli indici delle carte vanno da 0 a 3\n");
    printf("Si scelga la carta del campo nemico alla quale si vuole infliggere danno\n");
    scanf("%d", &scelta);

    campo[scelta]->punti_vita -= puntiVita;

    if(campo[scelta]->punti_vita <= 0){
      free(campo[scelta]);
      //per intendere che la carta nello slot non ha più punti ed è stata tolta dal campo
      campo[scelta] = NULL;
    }
    break;

    default:
      printf("La scelta selezionata è errata\n");
      break;
  }
}

static void guarisciDanno(struct Carta *campo[], struct Mago *giocatore, int puntiVita){
  int guarisci = 0, scelta = 0;

  if(campo == NULL){
    printf("Non ci sono carte in campo da poter curare\n");
    printf("Si curerà il mago\n");

    (*giocatore).PV += puntiVita;
  }else{
    printf("Si scelga se:\n");
    printf("'1' Si vuole guarire il danno sui punti vita del mago\n ");
    printf("'2' se si vuole guarire il danno sulle creature\n");
    printf("Scelta: ");
    scanf("%d", &guarisci);

    switch (guarisci) {
      case 1:
        (*giocatore).PV += puntiVita;
        break;

      case 2:
      printf("Stampa in corso del proprio campo\n");
      stampaCampo(campo);

      printf("Gli indici delle carte vanno da 0 a 3\n");
      printf("Si scelga la carta del proprio campo da curare\n");
      scanf("%d", &scelta);

      campo[scelta]->punti_vita += puntiVita;
      break;
    }
  }
}

static void stampaCampo(struct Carta *campo[]){
  for(int j = 0; j < 4; j++){
    if(campo[j] != NULL){
      printf("%d", j);
      printf("  Tipo: ");
      stampaTipo(campo[j]->tipo);
      printf("    Punti vita: %d\n",  campo[j]->punti_vita);
    }else{
      //metto uno spazio in più per far capire che manca una carta nello slot
      //che sto controllando
      printf("\n----\n");
    }
  }
}

static void sceltaCarta(struct Carta *campo[], int utilizzo[]){

  for(int i = 0; i < 4; i++){
    if(campo[i] != NULL){
      if(utilizzo[i] == 0){
        printf("%d", i);
        printf("  Tipo: ");
        stampaTipo(campo[i]->tipo);
        printf("    Punti vita: %d\n",  campo[i]->punti_vita);
      }else{
        printf("%d", i);
        printf("  La creatura ha già attaccato\n");
      }
    }else{
      utilizzo[i] = 0;
      printf("\n----\n");
    }
  }
}

static void controCreatura(struct Carta *campoGiocatore[], struct Carta *campoNemico[], int scelta, int utilizzo[]){
  int attacca = 0;

    printf("Quale carta si vuole attaccare nel campo nemico? \n");
    stampaCampo(campoNemico);
    printf("Carta nemica scelta:");
    scanf("%d", &attacca);

    campoNemico[attacca]->punti_vita -= campoGiocatore[scelta]->punti_vita;
    utilizzo[scelta] = 1;

    if(campoNemico[attacca]->punti_vita <= 0){
      campoNemico[attacca] = NULL;
    }
}

static void controMago(struct Mago *nemico, struct Carta *campo[], int scelta, int utilizzo[]){

    (*nemico).PV -= campo[scelta]->punti_vita;
    utilizzo[scelta] = 1;

    if((*nemico).PV <= 0){
      controlloCarte(&giocatore1, &giocatore2);
    }
}

static void attacca(struct Mago *nemico, struct Carta *campoGiocatore[], struct Carta *campoNemico[]){
  int count = 0, scelta = 0, noncarta;
  int utilizzo[4] = {0};
  char risposta = 'y';

  while(count < 4){
    noncarta = 0;
    //scelgo con quale creatura voglio attaccare il mio nemico
    printf("Con quale creatura si vuole attaccare? \n");
    sceltaCarta(campoGiocatore, utilizzo);
    printf("Carta scelta: ");
    scanf("%d", &scelta);

    //attacco la creatura nemica con la mia e controllo se non l'ho uccisa, altimenti la "dealloco" dal campo
    for(int i = 0; i < 4; i++){
      if(campoNemico[i] != NULL){
        controCreatura(campoGiocatore, campoNemico, scelta, utilizzo);
        break;
      }else{
        noncarta++;
      }
    }

    //non essendoci carte in campo decido di poter far attacare la creatura al mago
    if(noncarta == 4){
      printf("Il nemico non ha creature in campo\n");
      printf("Verrà attaccato il mago nemico\n");

      controMago(nemico, campoGiocatore, scelta, utilizzo);
    }

    //se ho già attaccato con tutte le carte, allora passo alla giocata successiva
    bool uso = false;
    for(int i = 0; i < 4; i++){
      uso = false;
      if(utilizzo[i] == 0){
        uso = true;
      }else{
        break;
      }
    }

    if(uso == true){
      printf("Sono già state usate tutte le carte per poter attaccare il nemico\n");
    }
    //nell'attacca posso giocare al massimo tanto quante carte ho a disposizione sul campo
    printf("Vuoi attaccare ancora?(y/n)");
    scanf(" %c", &risposta);

    if(risposta == 'n'){
      break;
    }else if(risposta == 'y'){
      continue;
    }else{
      printf("Hai inserito una risposta errata\n");
      printf("L'attacco sarà concluso\n");
      break;
    }
    count++;
  }
}

static void controlloCarte(struct Mago *giocatore, struct Mago *nemico){

  if(giocatore->PV <= 0){
    printf("Congratulazioni %s! Hai vinto la partita!!\n", nemico->nome);
  }else if(nemico->PV <= 0){
    printf("Congratulazioni %s! Hai vinto la partita!!\n", giocatore->nome);
  }else if(giocatore->PV > nemico->PV){
    printf("Congratulazioni %s! Hai vinto la partita!!\n", giocatore->nome);
  }else if(giocatore->PV < nemico->PV){
    printf("Congratulazioni %s! Hai vinto la partita!!\n", nemico->nome);
  }else{
    printf("I giocatori hanno stesso punteggio, quindi la partita si conclude con un pareggio!\n");
  }

  termina_gioco();
}

static void passa(){}

void imposta_gioco(){

  giocatore1.PV = 20;
  giocatore2.PV = 20;
  int n;

  printf("\nSi scelga la dimensione del mazzo con cui giocare.\n Tot. carte: ");
  scanf("%d", &n);

  printf("\nSetting per il PRIMO giocatore\n");
  printf("Il primo giocatore scelga il proprio nome.\n Nome: ");
  scanf("%s", giocatore1.nome);
  printf("\nSi scelga la classe del mago.\n Tra cui:\n '0' per tenebre\n '1' per vita\n '2' per luce.\n Scelta: ");
  scanf("%d", &giocatore1.classe);

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

    default:
      printf("Non esiste questa possibile scelta. ERRORE!!\n");
      break;
  }

  printf("\nSetting per il SECONDO giocatore\n");
  printf("Il secondo giocatore scelga il proprio nome.\n Nome: ");
  scanf("%s", giocatore2.nome);
  printf("\nSi scelga la classe del mago.\n Tra cui:\n '0' per tenebre\n '1' per vita\n '2' per luce.\n Scelta: ");
  scanf("%d", &giocatore2.classe);

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

    default:
      printf("Non esiste questa possibile scelta. ERRORE!!\n");
      break;
  }

//Da decidere se far vedere il mazzo oppure solamente la mano di ciascun giocatore
  /*printf("\nNome del giocatore: %s\n", giocatore1.nome);
  stampaMazzo(&mazzo1);
  printf("\nNome del giocatore: %s\n", giocatore2.nome);
  stampaMazzo(&mazzo2);*/

  creaMano(&mazzo1, mano1, ultimaCarta1);
  creaMano(&mazzo2, mano2, ultimaCarta2);

  /*printf("\nStampa della mano del primo giocatore\n");
  stampaMano(mano1);

  printf("\nStampa della mano del secondo giocatore\n");
  stampaMano(mano2);*/
}

void combatti(){
  int turno = 0, scelta = 0;
  bool giocata;

  while(1){
    int utilizzo[3] = {0};
    giocata = true;

    if(turno%2 == 0){
      printf("\nTURNO GIOCATORE: %s", giocatore1.nome);
      printf("    PUNTI VITA: %d\n", giocatore1.PV);

      printf("Stampa del campo del GIOCATORE\n");
      stampaCampo(campo1);
      printf("Stampa del campo del NEMICO\n");
      stampaCampo(campo2);

      while(giocata){
        printf("\nCosa vuoi fare?\n '1' Per pescare una carta dal mazzo\n '2' Per giocare una carta\n");
        printf(" '3' Per attaccare\n '4' Per passare la tua mano\n La tua scelta: ");
        scanf("%d", &scelta);

        switch (scelta) {
          case 1:
            if(utilizzo[scelta-1] == 0){
              pesca(&mazzo1, mano1, ultimaCarta1);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 2:
            if(utilizzo[scelta-1] == 0){
              giocaCarta(&giocatore1, &giocatore2, campo1, mano1, campo2);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 3:
            if(utilizzo[scelta-1] == 0){
              attacca(&giocatore2, campo1, campo2);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 4:
            passa();
            giocata = false;
            break;

          default:
            printf("La scelta inserita non è nella lista\n");
        }
      }
    }else{
      printf("\nTURNO GIOCATORE: %s", giocatore2.nome);
      printf("    PUNTI VITA: %d\n", giocatore2.PV);

      printf("Stampa del campo del GIOCATORE\n");
      stampaCampo(campo2);
      printf("Stampa del campo del NEMICO\n");
      stampaCampo(campo1);

      while(giocata){
        printf("\nCosa vuoi fare?\n '1' Per pescare una carta dal mazzo\n '2' Per giocare una carta\n");
        printf(" '3' Per attaccare\n '4' Per passare la tua mano\n La tua scelta: ");
        scanf("%d", &scelta);

        switch (scelta) {
          case 1:
            if(utilizzo[scelta-1] == 0){
              pesca(&mazzo2, mano2, ultimaCarta2);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 2:
            if(utilizzo[scelta-1] == 0){
              giocaCarta(&giocatore2, &giocatore1, campo2, mano2, campo1);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 3:
            if(utilizzo[scelta-1] == 0){
              attacca(&giocatore1, campo2, campo1);
              utilizzo[scelta-1] = 1;
            }else{
              printf("funzione già effettuata\n");
            }
            break;

          case 4:
            passa();
            giocata = false;
            break;

          default:
            printf("La scelta inserita non è nella lista\n");
        }
      }
    }
    turno++;
  }
}
