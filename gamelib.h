#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <stdbool.h>



enum Tipo_zona {bosco, scuola, laboratorio, caverna, strada, giardino, supermercato, centrale_elettrica, deposito_abbandonato, stazione_polizia};
enum Tipo_nemico{nessun_nemico, democane, billi, demotorzone};
enum Tipo_oggetto{nessun_oggetto, bicicletta, maglietta_fuocoinferno, bussola, schitarrata_metallica};
enum Tipo_mondo{mondo_reale, soprasotto};



typedef struct Giocatore {
    
    char nome[100];
    enum Tipo_mondo mondo;
    enum Tipo_oggetto zaino[3];
    struct Zona_mondoreale *pos_mondoreale;
    struct Zona_soprasotto *pos_soprasotto;
    int attacco_pischico;
    int difesa_pischica;
    int fortuna;

} Giocatore;

typedef struct Zona_mondoreale {
    
    enum Tipo_zona zona;
    enum Tipo_nemico nemico;
    enum Tipo_nemico nemico_originale; //per rigenerare eventualmente il nemico nella zona dopo essere avanzati
    enum Tipo_oggetto oggetto;
    struct Zona_mondoreale *avanti;
    struct Zona_mondoreale *indietro;
    struct Zona_soprasotto *link_soprasotto;

} Zona_mondoreale;

typedef struct Zona_soprasotto {

    enum Tipo_zona zona;
    enum Tipo_nemico nemico;
    enum Tipo_nemico nemico_originale;
    struct Zona_soprasotto *avanti;
    struct Zona_soprasotto *indietro;
    struct Zona_mondoreale *link_mondoreale;
   
} Zona_soprasotto;


void imposta_gioco(void);
void gioca(void);
void termina_gioco(void);
void crediti(void);


