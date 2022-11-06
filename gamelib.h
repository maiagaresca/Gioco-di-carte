#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void imposta_gioco();
void combatti();
void termina_gioco();

enum Classe_mago{
  tenebre,
  vita,
  luce
};

enum Tipo_carta{
  creatura,
  rimuovi_creatura,
  infliggi_danno,
  cura_danno
};

struct Carta{
  enum Tipo_carta tipo;
  int punti_vita;
  struct Carta *successivo;
};

struct Mago{
  char nome[20];
  int PV; //Punti vita
  enum Classe_mago classe;
  struct Carta *mazzo;
  struct Carta *mano[6];
  struct Carta *campo;
};
