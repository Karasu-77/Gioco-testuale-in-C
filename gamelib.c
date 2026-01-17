#include "gamelib.h"

//per generare numeri casuali
static int numero_casuale(int range){ 

    int numero = rand() % range;
    return numero;
}

//variabili di controllo
static bool UndiciVirgolaCinque_bool=false;
static bool demotorzone_bool=false;
static bool mappa_completa=false;
static int numero_zone=0;
static int giocatori=0;
static bool fuga=false;
static int giocatori_vivi=0;
static int vincitori=0;

const char *stampa_oggetto(enum Tipo_oggetto oggetto);
const char *stampa_nome_zona(enum Tipo_zona zona);
const char *stampa_nemico(enum Tipo_nemico nemico);
const char *stampa_mondo(enum Tipo_mondo mondo);

static char *nomi_giocatori[4] = {NULL};
char *nomi_vincitori[3] = {NULL};
static struct Giocatore *lista_giocatori[4] = {NULL};
static struct Zona_mondoreale *prima_zona_mondoreale = NULL;
//static struct Zona_soprasotto *prima_zona_soprasotto = NULL;

//giocatori
static void imposta_giocatori(void);
static void crea_giocatori(struct Giocatore *giocatore);
static void stampa_giocatore(struct Giocatore *giocatore);
//mappa
static void imposta_mappa(void);
static void genera_mappa(struct Zona_mondoreale **prima_zona_mondoreale);
static Zona_mondoreale *genera_zona(void);
static void stampa_zona_reale(struct Zona_mondoreale *prima_zona_mondoreale);
static void stampa_zona_sottopra(struct Zona_soprasotto *prima_zona_soprasotto);
static void stampa_zona(Zona_mondoreale *prima_zona_mondoreale, struct Zona_soprasotto *prima_zona_soprasotto);
static void stampa_mappa(struct Zona_mondoreale *prima_zona_mondoreale);
static void modifica_mappa(void);
static void inserisci_zona(struct Zona_mondoreale **prima_zona_mondoreale);
static void cancella_zona(struct Zona_mondoreale **prima_zona_mondoreale);
static void chiudi_mappa(void);
//gioco
static void avanza(Giocatore **lista_giocatori, int i);
static void indietreggia(Giocatore **lista_giocatori, int i);
static void cambia_mondo(Giocatore **lista_giocatori, int i);
static void combatti(Giocatore **lista_giocatori, int i);
static void raccogli_oggetto(struct Giocatore **lista_giocatori, int i);
static void utilizza_oggetto(Giocatore **lista_giocatori, int i);

//funzioni del main
void imposta_gioco(void);
void gioca(void);
void termina_gioco(void);
void crediti(void);

//varie funzioni con cui nominalizzare gli enum
const char *stampa_mondo(enum Tipo_mondo mondo){

    switch (mondo){
    case mondo_reale: return "Mondo Reale";
    case soprasotto: return "Mondo Soprasotto";
    default: puts("boh");}

}

const char *stampa_oggetto(enum Tipo_oggetto oggetto){

    switch (oggetto){
    case nessun_oggetto: return "Vuoto";
    case bicicletta: return "Bicicletta";
    case maglietta_fuocoinferno: return "Maglietta Fuocoinferno";
    case bussola: return "Bussola";
    case schitarrata_metallica: return "Schitarrata Metallica";
    default: puts("boh");}

}

static void stampa_giocatore(struct Giocatore *giocatore){
     
    printf("+=================================================+\n");
    printf("| %-47.47s |\n", giocatore->nome);
    printf("+=================================================+\n");
    printf("| Ti trovi nel   : %-30s |\n", stampa_mondo(giocatore->mondo));
    printf("| Att. pischico  : %-30d |\n", giocatore->attacco_pischico);
    printf("| Dif. pischica  : %-30d |\n", giocatore->difesa_pischica);
    printf("| Fortuna        : %-30d |\n", giocatore->fortuna);
    printf("| Zaino          : %-30s |\n", stampa_oggetto(giocatore->zaino[0]));
    printf("|                  %-30s |\n", stampa_oggetto(giocatore->zaino[1]));
    printf("|                  %-30s |\n", stampa_oggetto(giocatore->zaino[2]));
    printf("+=================================================+\n");

}

const char *stampa_nome_zona(enum Tipo_zona zona){

    switch (zona){
        case bosco: return "Bosco";
        case scuola: return "Scuola";
        case laboratorio: return "Laboratorio";
        case caverna: return "Caverna";
        case strada: return "Strada";
        case giardino: return "Giardino";
        case supermercato: return "Supermercato";
        case centrale_elettrica: return "Centrale elettrica";
        case deposito_abbandonato: return "Deposito abbandonato";
        case stazione_polizia: return "Stazione di Polizia";
        default: puts("Boh");}


    }


const char *stampa_nemico(enum Tipo_nemico nemico){

    switch (nemico){
        case nessun_nemico: return "Nessuno";
        case democane: return "Democane";
        case billi: return "Billi";
        case demotorzone: return "Demotorzone";
        default: puts("Boh");
    
    }
}

//stampa solo la zona dei mondo reale
static void stampa_zona_reale(Zona_mondoreale *prima_zona_mondoreale){

    printf("---------------------------------------------------+\n");
    printf("|                    MONDO REALE                   |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Zona    : %-38.38s |\n", stampa_nome_zona(prima_zona_mondoreale->zona));        
    printf("| Oggetto : %-38.38s |\n", stampa_oggetto(prima_zona_mondoreale->oggetto));
    printf("| Nemico  : %-38.38s |\n", stampa_nemico(prima_zona_mondoreale->nemico));
    printf("+--------------------------------------------------+\n");

}

//stampa solo la zona del soprasotto
static void stampa_zona_sottopra(Zona_soprasotto *prima_zona_soprasotto){

    printf("+--------------------------------------------------+\n");
    printf("|                MONDO SOPRASOTTO                  |\n");
    printf("+--------------------------------------------------+\n");
    printf("| Zona    : %-38.38s |\n", stampa_nome_zona(prima_zona_soprasotto->zona));
    printf("| Nemico  : %-38.38s |\n", stampa_nemico(prima_zona_soprasotto->nemico));
    printf("+--------------------------------------------------+\n"); 

}

//stampa una zona completa dei due mondi
static void stampa_zona(Zona_mondoreale *prima_zona_mondoreale, Zona_soprasotto *prima_zona_soprasotto){

    stampa_zona_reale(prima_zona_mondoreale);
    stampa_zona_sottopra(prima_zona_soprasotto);
    
 }


//funzione per creare i giocatori con le loro statistiche randomiche
static void crea_giocatori(Giocatore *giocatore){

      do{
        printf("Scegli il nome del tuo personaggio:\n");
        fgets(giocatore->nome, 100, stdin);
        giocatore->nome[strcspn(giocatore->nome, "\n")] = '\0';
        if (strlen(giocatore->nome)==0) {
        system("clear");
        puts("(Il nome non può essere vuoto.)\n");}

        }while(strlen(giocatore->nome)==0);
    
      giocatore->mondo=mondo_reale;
      giocatore->pos_mondoreale = NULL;
      giocatore->pos_soprasotto = NULL;
      for (int i=0; i<3; i++){giocatore->zaino[i] = nessun_oggetto;}
      giocatore->attacco_pischico = numero_casuale(20)+1;
      giocatore->difesa_pischica = numero_casuale(20)+1;
      giocatore->fortuna = numero_casuale(20)+1;


    stampa_giocatore(giocatore);
    
    
    int scelta=0; 
    
    puts("\n\n\nVuoi cambiare le tue statistiche?\nAggiungi 3 punti all'Attacco ma ne togli 3 alla Difesa o viceversa.\n");
    puts("1) Aumenta attacco\n2) Aumenta difesa\n3) Non modificare niente");
    if(!UndiciVirgolaCinque_bool){//mi assicuro che si possa scegliere l'opzione 4 solo una volta
    puts("4) Diventa UndiciVirgolaCinque (+4 sia in attacco che in difesa, -7 in fortuna).\n\n");}
    

    do{

        int c=0; //pulizia buffer
        do{
            if(scanf("%d", &scelta)!=1){
                puts("Input non valido.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;}
                break;
        }while(1);
        
        
        if(scelta==4 && !UndiciVirgolaCinque_bool){   
            strcpy(giocatore->nome, "UndiciVirgolaCinque");
             giocatore->attacco_pischico+=4;
             giocatore->difesa_pischica+=4;
             giocatore->fortuna-=7;
             puts("\nModifica effettuata.\n");
             UndiciVirgolaCinque_bool=true;
             break;
         } 

    else{
        switch(scelta){

            case 1: giocatore->attacco_pischico+=3;
                    giocatore->difesa_pischica-=3;
                    puts("\nModifica effettuata.\n");
                    break;

            case 2: giocatore->attacco_pischico-=3;
                    giocatore->difesa_pischica+=3;
                    puts("\nModifica effettuata.\n");
                    break;
            

            case 3: puts("Nessuna modifica effettuata.\n");
                    break;
            

            default: puts("\nErrore, scelta non consentita.\n");
            
        }
         
    }
    } while(scelta<1 || scelta>3); 


//in caso le modifiche portino ad avere statistiche inferiori a 0 o maggiori di 20
    int att = giocatore->attacco_pischico;
    int dif = giocatore->difesa_pischica;
    int fort = giocatore->fortuna;

    if(att < 1) att = 1;
    if(att > 20) att = 20;
    giocatore->attacco_pischico=att;

    if(dif<1) dif=1;
    if(dif>20) dif=20;
    giocatore->difesa_pischica=dif;

    if(fort<1) fort=1;
    if(fort>20) fort=20;
    giocatore->fortuna=fort;


    stampa_giocatore(giocatore);

}

//qui si sceglie il numero dei giocatori e si richiama crea_giocatori per crearli
static void imposta_giocatori(void){

    if(lista_giocatori[giocatori]!=NULL){
        for(int i=0; i<giocatori; i++){
         free(lista_giocatori[i]);}
        UndiciVirgolaCinque_bool=false;}

    puts("\nCome prima cosa scegli il numero di partecipanti per un massimo di 4.\n\n");  
    
    do{
    int c;
    
    do{
        if(scanf("%d", &giocatori)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
    

    if(giocatori<1 || giocatori>4){
        puts("Errore, digita un numero compreso tra 1 e 4.\n");}

    }while(giocatori<1 || giocatori>4);

    printf("\n\nSono stati selezionati %d giocatori.\n", giocatori);

    puts("\nOra passiamo alla crazione.\n");

    for (int i = 0; i < giocatori; i++){

       //creo un array che ospita le struct dei giocatori
        lista_giocatori[i] = malloc(sizeof(struct Giocatore));

        crea_giocatori(lista_giocatori[i]);

    }
   

}

//creazione di una singola zona del mondo reale e del soprasotto collegate tra loro
static struct Zona_mondoreale *genera_zona(void){
    
    struct Zona_mondoreale *prima_zona_mondoreale = malloc(sizeof(struct Zona_mondoreale));
    struct Zona_soprasotto *prima_zona_soprasotto = malloc(sizeof(struct Zona_soprasotto));

    prima_zona_mondoreale->zona=numero_casuale(10);
    prima_zona_mondoreale->nemico=numero_casuale(3);
    prima_zona_mondoreale->nemico_originale=prima_zona_mondoreale->nemico;
    prima_zona_mondoreale->oggetto=numero_casuale(5);
    prima_zona_mondoreale->avanti=NULL;
    prima_zona_mondoreale->indietro=NULL;
    prima_zona_mondoreale->link_soprasotto=prima_zona_soprasotto;

    prima_zona_soprasotto->zona=prima_zona_mondoreale->zona;
    prima_zona_soprasotto->avanti=NULL;
    prima_zona_soprasotto->indietro=NULL;
    prima_zona_soprasotto->link_mondoreale=prima_zona_mondoreale;

    int c=numero_casuale(3);

    switch(c){

        case 0: prima_zona_soprasotto->nemico=nessun_nemico;
                prima_zona_soprasotto->nemico_originale=prima_zona_soprasotto->nemico;
                break;
        
        case 1: prima_zona_soprasotto->nemico=democane;
                prima_zona_soprasotto->nemico_originale=prima_zona_soprasotto->nemico;
                break;
            
        case 2: if(!demotorzone_bool){//mi assicuro che ci sia solo un demotorzone per quando andrò a richimare questa funzione
                prima_zona_soprasotto->nemico=demotorzone; //per la generazione della mappa completa
                demotorzone_bool=true;}
                else{int d=numero_casuale(2);
                    if(d==0){prima_zona_soprasotto->nemico=nessun_nemico;
                        prima_zona_soprasotto->nemico_originale=prima_zona_soprasotto->nemico;}
                    if(d==1){prima_zona_soprasotto->nemico=democane;
                        prima_zona_soprasotto->nemico_originale=prima_zona_soprasotto->nemico;}
                }
                break;

        default: break;

    }

    return prima_zona_mondoreale;

}

//generazione di 15 zone del mondo reale e 15 del soprasotto
static void genera_mappa(struct Zona_mondoreale **prima_zona_mondoreale){

    demotorzone_bool=false; //per sicurezza se eventualmente questa funzione venisse richiamata più volte
    numero_zone=0;

    Zona_mondoreale *temp1 = *prima_zona_mondoreale;
    while (temp1 != NULL) {

        Zona_mondoreale *temp2=temp1->avanti;

        free(temp1->link_soprasotto); //eventualmente per liberare una mappa precedentemente creata
        free(temp1);

        temp1=temp2;
    }
   
    *prima_zona_mondoreale = NULL;

    for (int k=0; k<15; k++) {
        numero_zone++;
        Zona_mondoreale *nuova_zona = genera_zona(); //genero 15 zone e le collego tra di loro

        if (*prima_zona_mondoreale == NULL) {
            *prima_zona_mondoreale = nuova_zona;
        } else {
            Zona_mondoreale *temp3 = *prima_zona_mondoreale;
            while (temp3->avanti != NULL)
            temp3 = temp3->avanti;

            temp3->avanti = nuova_zona;
            nuova_zona->indietro = temp3;

            temp3->link_soprasotto->avanti = nuova_zona->link_soprasotto;
            nuova_zona->link_soprasotto->indietro = temp3->link_soprasotto;
        }
    } 
}

//per stampare solo la mappa composta solo dalle zone del mondo reale
static void stampa_mappa(Zona_mondoreale *prima_zona_mondoreale){
    
    int numero=1; //per numerarle
    struct Zona_mondoreale *temp = prima_zona_mondoreale;
    while (temp != NULL){
    printf("\n[%d]\n", numero);
    numero++;
    stampa_zona_reale(temp);
    temp = temp->avanti;}
}   


static void inserisci_zona(Zona_mondoreale **prima_zona_mondoreale){
    
    int h, c, x, y, z;
    system("clear");
    stampa_mappa(*prima_zona_mondoreale);
    puts("\nDigita il numero della zona dove vuoi aggiungere quella nuova.\n");
    puts("(Se ad esempio digiti 2, la nuova zona comparirà dopo di essa)\n");
    
    do{
        if(scanf("%d", &h)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
    

    if (h<1) {
        puts("Numero non valido.");
        return;
    }

    Zona_mondoreale *temp = *prima_zona_mondoreale;
    int i=1;

    while (temp!=NULL && i<h){ //per iterare fino alla zona numero h a cui attaccare una nuova zona, se esiste
        temp = temp->avanti;
        i++;
    }

    if (temp == NULL) {
        puts("\nZona inesistente.\n"); 
        return;
    }
    
    Zona_mondoreale *nuova_zona = genera_zona();

    nuova_zona->avanti = temp->avanti;
    nuova_zona->indietro = temp;

    if (temp->avanti!=NULL) {
        temp->avanti->indietro = nuova_zona;
    }

    temp->avanti = nuova_zona;
    numero_zone++; //le zone devono essere esattamente 15
    puts("\nZona aggiunta.\n\n");
    //viene creata una nuova zona con tutte le caratteristiche ma l'utente può scegliere cosa mettere nella zona
    puts("Puoi inoltre scegliere cosa e chi inserire nella zona.\n"); 
    puts("Scegli l'oggetto: 1) Nessun oggetto 2) Bicicletta 3) Maglietta fuocoinferno 4) Bussola 5) Schitarrata metallica Random) Esci\n");


    do{
        if(scanf("%d", &x) != 1){
            puts("Input non valido.");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
    
    
    switch (x){

    case 1: nuova_zona->oggetto=nessun_oggetto;
            puts("\nModifica avvenuta.");
        break;
    
    case 2: nuova_zona->oggetto=bicicletta;
            puts("\nModifica avvenuta.");
        break;
    
    case 3: nuova_zona->oggetto=maglietta_fuocoinferno;
            puts("\nModifica avvenuta.");
        break;
     
    case 4: nuova_zona->oggetto=bussola;
            puts("\nModifica avvenuta.");
        break;    
    
    case 5: nuova_zona->oggetto=schitarrata_metallica;
            puts("\nModifica avvenuta.");
        break;   
    
    default: puts("\nNessuna modifica effettuata.");
        break;
    }
    

    puts("Scegli il nemico del Mondo Reale: 1) Nessun nemico 2) Democane 3) Billi Qualsiasi) Esci\n");



    do{
        if(scanf("%d", &y)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
    

    switch (y){

    case 1: nuova_zona->nemico=nessun_nemico;
            nuova_zona->nemico_originale=nessun_nemico;
            puts("\nModifica avvenuta.");
        break;
    
    case 2: nuova_zona->nemico=democane;
            nuova_zona->nemico_originale=democane;
            puts("\nModifica avvenuta.");
        break;
    
    case 3: nuova_zona->nemico=billi;
            nuova_zona->nemico_originale=billi;
            puts("\nModifica avvenuta.");
        break;
    
    default: puts("\nNessuna modifica effettuata.");
        break;
    }

    puts("Scegli il nemico del Soprasotto: 1) Nessun nemico 2) Democane 3) Demotorzone Qualsiasi) Esci\n");
    puts("Attenzione: può esserci solo un Demotorzone.\n");
    


    do{
        if(scanf("%d", &z)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
    
    
    switch (z){

    case 1: nuova_zona->link_soprasotto->nemico=nessun_nemico;
            nuova_zona->link_soprasotto->nemico_originale=nessun_nemico;
            puts("\nModifica avvenuta.");
        break;
    
    case 2: nuova_zona->link_soprasotto->nemico=democane;
            nuova_zona->link_soprasotto->nemico_originale=democane;
            puts("\nModifica avvenuta.");
        break;
    
    case 3: if(!demotorzone_bool){nuova_zona->link_soprasotto->nemico=demotorzone; puts("\nModifica avvenuta.");}
            else{puts("\nEsiste già un Demotorzone altrove (o magari questa funzione l'ha appena creato).");
                int c=numero_casuale(2);
                if(c==0){nuova_zona->link_soprasotto->nemico=nessun_nemico;}
                else{nuova_zona->link_soprasotto->nemico=democane;}
            }
        break;
    
        default: puts("\nNessuna modifica effettuata.");
        break;
    }

    stampa_mappa(*prima_zona_mondoreale);

    
}

//cancella una zona in posizione h
static void cancella_zona(Zona_mondoreale **prima_zona_mondoreale) {

    int h, c;
    system("clear");
    sleep(1);
    stampa_mappa(*prima_zona_mondoreale);
    puts("\nDigita il numero della zona che vuoi eliminare.\n");

    do{
        if(scanf("%d", &h)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);

    if (h < 1 || *prima_zona_mondoreale == NULL) {
        puts("Zona inesistente.");
        return;
    }

    Zona_mondoreale *temp = *prima_zona_mondoreale;
    int i=1;

    while (temp != NULL && i < h) { //per iterare fino alla posizione h nella mappa, se esite
        temp = temp->avanti;
        i++;
    }

    if (temp == NULL) {
        puts("Zona inesistente.");
        return;
    }
    if(temp->link_soprasotto->nemico==demotorzone){demotorzone_bool=false;} //vengono eliminate sia la zona del mondo reale che quella del soprasotto
    Zona_mondoreale *prima = temp->indietro; //quindi mi assicuro che se ci fosse il demotorzone nella zona eliminata ciò verrà segnalato
    Zona_mondoreale *dopo = temp->avanti;

    if (prima != NULL) {prima->avanti=dopo;}
    else {*prima_zona_mondoreale=dopo;}
    
    if (dopo != NULL){dopo->indietro = prima;}

    free(temp->link_soprasotto);
    free(temp);

    numero_zone--; //le zone devono essere esattamente 15

    puts("\nZona eliminata.\n");
    sleep(1);
    stampa_mappa(*prima_zona_mondoreale);
}


//raccoglie le funzioni per aggiungere o cancellare una zona dalla mappa
static void modifica_mappa(void){
    
    int c, scelta;
      system("clear");
      puts("Vuoi aggiungere o eliminare una nuova zona?");
      do{
      puts("\n1) Aggiungi\n2) Elimina\n3) Indietro");
      
      do{
        if(scanf("%d", &scelta)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);

      switch(scelta){
        
        case 1: inserisci_zona(&prima_zona_mondoreale);
                break;

        case 2: cancella_zona(&prima_zona_mondoreale);
                break;    
         
        case 3: break;

        default: puts("\nErrore, digita 1 o 2 o 3.\n");
      }

      }while(scelta!=3);
}

//mi assicuro che dopo eventuali modifiche le zone siano effettivamente 15 e che ci sia un unico demotorzone
static void chiudi_mappa(void){

    if(numero_zone==15){mappa_completa=true;}
    
        if (!demotorzone_bool || !mappa_completa) {

            if(!demotorzone_bool){puts("Attenzione: per giocare serve un Demotorzone nel Soprasotto.");}

            else{puts("Attenzione: per giocare le zone devono essere esattamente 15.");}

        } else {
            puts("Creazione della mappa terminata, ora inizia il gioco!");
        }
}
    
//tutte le funzioni con cui creare la mappa e modificarla
static void imposta_mappa(void){

    sleep(3);
    system("clear");
    puts("Ora passiamo a creare la mappa di gioco.\n");
    puts("È generata randomicamente ed è composta da 15 zone.\n");
    puts("Ogni zona è formata da due diversi mondi specchiati, il mondo Reale ed il mondo Soprasotto...\n");
    
    int c=0, scelta;
    do{puts("\n\n1) Genera una mappa\n2) Visualizza la mappa\n3) Modifica la mappa\n4) Visualizza una zona\n5) Chiudi e gioca\n");
       
    do{
        if(scanf("%d", &scelta)!=1){
            puts("Input non valido.\n");
            while ((c = getchar()) != '\n' && c != EOF);
            continue;}
            break;
    }while(1);
        
    

    switch(scelta){

        case 1: genera_mappa(&prima_zona_mondoreale);
                puts("\nLa nuova mappa di gioco è stata creata.\n");
                break;
        
        case 2: if(numero_zone==0){puts("\nMappa ancora da creare.\n");}
                else{
                stampa_mappa(prima_zona_mondoreale);}
                break;

        case 3: if(numero_zone==0){puts("\nMappa ancora da creare.\n");}
                else{
                modifica_mappa();}
                break;

        case 4: if(prima_zona_mondoreale!=NULL){
                puts("\nScegli quale zona stampare.\n"); //la mappa stampa solo il mondo reale
                struct Zona_mondoreale  *temp1 = prima_zona_mondoreale; //ma con questo richiamo si stampa la zona di entrambi i mondi
                struct Zona_soprasotto *temp2 = prima_zona_mondoreale->link_soprasotto;
                int a, i=1;
                scanf("%d", &a);
                while ((c = getchar()) != EOF && c != '\n');
                if(a<0){puts("\nErrore.\n");}
                else{
                while (temp1 != NULL && temp2 != NULL && i < a) {
                       temp1 = temp1->avanti;
                       temp2 = temp2->avanti;
                       i++;}
                if (temp1 == NULL || temp2 == NULL) {puts("Zona inesistente.\n");}
                stampa_zona(temp1, temp2);}
                }else{puts("Mappa ancora da creare.\n");}
                break;

        case 5: chiudi_mappa();
                break;
    
        default: puts("\nErrore, puoi digitare solo 1, 2, 3, o 4.\n");
                
    }

    }while(!(scelta == 5 && mappa_completa && demotorzone_bool));

}

static void avanza(Giocatore **lista_giocatori, int i){

    int vivo_morto=numero_casuale(2); 

    switch(lista_giocatori[i]->mondo){

    case mondo_reale:
    
    if (lista_giocatori[i]->pos_mondoreale == NULL || lista_giocatori[i]->pos_mondoreale->avanti == NULL) {
        puts("\nNon puoi avanzare oltre.\n");
        return;
    }

    switch(lista_giocatori[i]->pos_mondoreale->nemico){

        case nessun_nemico: {Zona_mondoreale *temp = lista_giocatori[i]->pos_mondoreale;
                            lista_giocatori[i]->pos_mondoreale=lista_giocatori[i]->pos_mondoreale->avanti;
                            lista_giocatori[i]->pos_soprasotto=lista_giocatori[i]->pos_mondoreale->link_soprasotto;

                            if(temp->nemico_originale==democane && vivo_morto==0){temp->nemico=democane;}
                            //50% di probabilità di rigenerare il nemico sconfitto
                            if(temp->nemico_originale==billi && vivo_morto==0){temp->nemico=billi;}

                            puts("\nOra ti trovi in: \n");
                            stampa_zona_reale(lista_giocatori[i]->pos_mondoreale);}
                            break;

        case democane: puts("\nPer avanzare devi prima sconfiggere il Democane!\n");
                       break;

        case billi: puts("\nPer avanzare devi prima sconfiggere Billi!\n");
                    break;
        
        default: puts("Boh");

    }
        break;

    case soprasotto:

        if (lista_giocatori[i]->pos_soprasotto == NULL || lista_giocatori[i]->pos_soprasotto->avanti == NULL){
            puts("\nNon puoi avanzare oltre.\n");
            return;
        }
    
        switch(lista_giocatori[i]->pos_soprasotto->nemico){
    
            case nessun_nemico: {Zona_soprasotto *temp = lista_giocatori[i]->pos_soprasotto;
                                lista_giocatori[i]->pos_soprasotto = lista_giocatori[i]->pos_soprasotto->avanti;
                                lista_giocatori[i]->pos_mondoreale = lista_giocatori[i]->pos_soprasotto->link_mondoreale;

                                if(temp->nemico_originale==democane && vivo_morto==0){temp->nemico=democane;}
                                
                                puts("\nOra ti trovi in: \n");
                                stampa_zona_sottopra(lista_giocatori[i]->pos_soprasotto);}
                                break;
    
            case democane: puts("\nPer avanzare devi prima sconfiggere il Democane!\n");
                           break;
    
            case demotorzone: puts("\nPer avanzare devi prima sconfiggere Demotorzone!\n");
                        break;
            
            default: puts("Boh");
    
        }
            break;
            
    default: puts("Boh");

    }

}
        
        
static void indietreggia(Giocatore** lista_giocatori, int i){

    switch(lista_giocatori[i]->mondo){

        case mondo_reale:
        
        if (lista_giocatori[i]->pos_mondoreale == NULL || lista_giocatori[i]->pos_mondoreale->indietro == NULL) {
            puts("\nNon puoi indietreggiare oltre.\n");
            return;
        }
    
        switch(lista_giocatori[i]->pos_mondoreale->nemico){
    
            case nessun_nemico: lista_giocatori[i]->pos_mondoreale = lista_giocatori[i]->pos_mondoreale->indietro;
                                lista_giocatori[i]->pos_soprasotto=lista_giocatori[i]->pos_mondoreale->link_soprasotto;
                                puts("\nOra ti trovi in: \n");
                                stampa_zona_reale(lista_giocatori[i]->pos_mondoreale);
                                break;
    
            case democane: puts("\nPer indietreggiare devi prima sconfiggere il Democane!\n");
                           break;
    
            case billi: puts("\nPer indietreggiare devi prima sconfiggere Billi!\n");
                        break;
            
            default: puts("Boh");
    
        }
            break;
    
        case soprasotto:

        if (lista_giocatori[i]->pos_soprasotto == NULL || lista_giocatori[i]->pos_soprasotto->indietro == NULL) {
            puts("\nNon puoi indietreggiare oltre.\n");
            return;
        }
    
        switch(lista_giocatori[i]->pos_soprasotto->nemico){
    
            case nessun_nemico: lista_giocatori[i]->pos_soprasotto = lista_giocatori[i]->pos_soprasotto->indietro;
                                lista_giocatori[i]->pos_mondoreale = lista_giocatori[i]->pos_soprasotto->link_mondoreale;
                                puts("\nOra ti trovi in: \n");
                                stampa_zona_sottopra(lista_giocatori[i]->pos_soprasotto);
                                break;
    
            case democane: puts("\nPer indietreggiare devi prima sconfiggere il Democane!\n");
                           break;
    
            case demotorzone: puts("\nPer indietreggiare devi prima sconfiggere Demotorzone!\n");
                        break;
            
            default: puts("Boh");
    
        }
            
            default: puts("Boh");
    
        }
        
}      

static void cambia_mondo(Giocatore **lista_giocatori, int i){

    switch(lista_giocatori[i]->mondo){

        case mondo_reale: if(lista_giocatori[i]->pos_mondoreale->nemico==nessun_nemico){
                          lista_giocatori[i]->mondo=soprasotto;
                          lista_giocatori[i]->pos_soprasotto=lista_giocatori[i]->pos_mondoreale->link_soprasotto;
                          puts("\nOra ti trovi in: \n");
                          stampa_zona_sottopra(lista_giocatori[i]->pos_soprasotto);}

                          else{puts("Per cambiare mondo devi prima sconfiggere il nemico.\n");}
                          break;

        case soprasotto:  if(lista_giocatori[i]->pos_soprasotto->nemico==nessun_nemico){
                          lista_giocatori[i]->mondo=mondo_reale;
                          lista_giocatori[i]->pos_mondoreale=lista_giocatori[i]->pos_soprasotto->link_mondoreale;
                          puts("\nOra ti trovi in: \n");
                          stampa_zona_reale(lista_giocatori[i]->pos_mondoreale);}

                          else{//per tentare la fuga solo una volta per turno
                          if(fuga){puts("\nHai già tentato la fuga e non ha funzionato, resti qui... \n");}
                          else{
                          puts("\nÈ stato tirato un dado da 20, se il risultato è inferiore alla tua fortuna scappi, altrimenti resti qui.\n");
                          int z =numero_casuale(20)+1;
                          printf("\nÈ uscito: %d\n", z);
                          if(z<lista_giocatori[i]->fortuna){
                          lista_giocatori[i]->mondo=mondo_reale;
                          lista_giocatori[i]->pos_mondoreale=lista_giocatori[i]->pos_soprasotto->link_mondoreale;
                          puts("\nSei scappato e ora ti trovi in: \n");
                          stampa_zona_reale(lista_giocatori[i]->pos_mondoreale);}
                          else{puts("\nRimani qui... \n"); fuga=true;}
                          }
                          }
                          break;
    
        default: puts("Boh");

    }

}

static void combatti(Giocatore **lista_giocatori, int i){

    struct Giocatore nemico; //per creare localmente un nemico da combattere
    
    switch(lista_giocatori[i]->mondo){

    case mondo_reale:

    switch(lista_giocatori[i]->pos_mondoreale->nemico){

        case democane:
        nemico.attacco_pischico=3;
        nemico.difesa_pischica=25;
        puts("\nSei in presenza di un Democane...\n");
        break;

        case billi:
        nemico.attacco_pischico=2;
        nemico.difesa_pischica=25;
        puts("\nSei in presenza di Billi...\n");
        break;

        default: puts("\nNessuno.\n");
        break;
       
    } break;

    case soprasotto:
    
    switch(lista_giocatori[i]->pos_soprasotto->nemico){

        case democane:
        nemico.attacco_pischico=3;
        nemico.difesa_pischica=25;
        puts("\nSei in presenza di un Democane...\n");
        break;

        case demotorzone:
        nemico.attacco_pischico=5;
        nemico.difesa_pischica=50;
        puts("\nSei in presenza del Demetorzone!\n");
        break;

        default: puts("\nNessuno.\n");
        break;
       
    } break;

    default: puts("\nBoh.\n");
    break;

    }
    
    
    if(lista_giocatori[i]->mondo==mondo_reale){
    
    if(lista_giocatori[i]->pos_mondoreale->nemico!=nessun_nemico){

    puts("\nOra inizia il combattimento!.\n");
    puts("Si lancia un dado da 6 ad ogni interazione attacco/difesa;\n");
    puts("Se il risultato è inferiore alla propria fortuna l'attacco o la difesa vanno a buon fine.\n");
    int dado;

    do{

    sleep(2);
    printf("Nemico: %s", stampa_nemico(lista_giocatori[i]->pos_mondoreale->nemico));
    printf("\nAtt. pischico: %d | Dif. pischica: %d\n\n", nemico.attacco_pischico, nemico.difesa_pischica);
    
    printf("%s sta attaccando...\n", lista_giocatori[i]->nome);
    dado=numero_casuale(6)+1;

    if(dado<lista_giocatori[i]->fortuna){nemico.difesa_pischica-=lista_giocatori[i]->attacco_pischico;
         sleep(2); puts("Colpito!\n");
        if(nemico.difesa_pischica<=0){break;}} //break per impedire ad un ormai già morto di finire il turno
    else{puts("Mancato...\n");}

    printf("Il nemico sta attaccando...\n");
    dado=numero_casuale(6)+1;

    if(dado<lista_giocatori[i]->fortuna){puts("Mancato!\n");}
    else{lista_giocatori[i]->difesa_pischica-=nemico.attacco_pischico;
         puts("Sei stato colpito...!\n");
        if(lista_giocatori[i]->difesa_pischica<=0){
            sleep(3);
            printf("Oh no! %s è morto in battaglia!\n", lista_giocatori[i]->nome);
            break;}}

    
    }while(nemico.difesa_pischica>0 && lista_giocatori[i]->difesa_pischica>0);

    }else{puts("\nNon c'è nessuno da combattere qui.\n");}

}else{

    if(lista_giocatori[i]->pos_soprasotto->nemico!=nessun_nemico){

        puts("\nOra inizia il combattimento!.\n");
        puts("Si lancia un dado da 6 ad ogni interazione attacco/difesa;\n");
        puts("Se il risultato è inferiore alla propria fortuna l'attacco o la difesa vanno a buon fine.\n");
        int dado;
    
    do{
    
    sleep(2);
    printf("Nemico: %s", stampa_nemico(lista_giocatori[i]->pos_soprasotto->nemico));
    printf("\nAtt. pischico: %d | Dif. pischica: %d\n\n", nemico.attacco_pischico, nemico.difesa_pischica);
        
    printf("%s sta attaccando...\n", lista_giocatori[i]->nome);
    dado=numero_casuale(6)+1;
    
    if(dado<lista_giocatori[i]->fortuna){nemico.difesa_pischica-=lista_giocatori[i]->attacco_pischico;
    sleep(2); puts("Colpito!\n");
    if(nemico.difesa_pischica<=0){break;}} //break per impedire ad un ormai già morto di finire il turno
    else{puts("Mancato...\n");}
    
    printf("Il nemico sta attaccando...\n");
    dado=numero_casuale(6)+1;
    
    if(dado<lista_giocatori[i]->fortuna){puts("Mancato!\n");}
    else{lista_giocatori[i]->difesa_pischica-=nemico.attacco_pischico;
        puts("Sei stato colpito...!\n");
        if(lista_giocatori[i]->difesa_pischica<=0){
            sleep(3);
            printf("Oh no! %s è morto in battaglia!\n", lista_giocatori[i]->nome);
            break;}}
        
    }while(nemico.difesa_pischica>0 && lista_giocatori[i]->difesa_pischica>0);
    
        
    }else{puts("\nNon c'è nessuno da combattere qui.\n");}

}

    if(lista_giocatori[i]->mondo==soprasotto && lista_giocatori[i]->pos_soprasotto->nemico==demotorzone && nemico.difesa_pischica<=0){
    lista_giocatori[i]->pos_soprasotto->nemico=nessun_nemico;
    demotorzone_bool=false;
    if(vincitori<3){nomi_vincitori[vincitori] = strdup(lista_giocatori[i]->nome);
    vincitori++;}
    printf("Demotorzone sconfitto! %s vince!\n", lista_giocatori[i]->nome);
    return;}

    
    if(nemico.difesa_pischica<=0){ //rimuovo il nemico dalla zona
         
        if(lista_giocatori[i]->mondo==mondo_reale){lista_giocatori[i]->pos_mondoreale->nemico=nessun_nemico;}

        if(lista_giocatori[i]->mondo==soprasotto){lista_giocatori[i]->pos_soprasotto->nemico=nessun_nemico;}

        puts("Nemico sconfitto!\n");}

}

static void raccogli_oggetto(Giocatore **lista_giocatori, int i){

    if(lista_giocatori[i]->mondo==mondo_reale){

    if(lista_giocatori[i]->pos_mondoreale->nemico==nessun_nemico){

    int inventario=-1;

    for (int j=0; j<3; j++) { //per controllare se nello zaino ho posti liberi
        if (lista_giocatori[i]->zaino[j]==nessun_oggetto) {
            inventario = j;
            break;}
    }

    if(inventario==-1){puts("\nNon puoi raccogliere l'oggetto, hai lo zaino pieno.\n"); return;}

    else{

    switch(lista_giocatori[i]->pos_mondoreale->oggetto){

        case bicicletta: lista_giocatori[i]->zaino[inventario]=bicicletta;
                         lista_giocatori[i]->pos_mondoreale->oggetto=nessun_oggetto;
                         puts("\nOggetto raccolto.\n");
        break;
                    
        case maglietta_fuocoinferno: lista_giocatori[i]->zaino[inventario]=maglietta_fuocoinferno;
                                     lista_giocatori[i]->pos_mondoreale->oggetto=nessun_oggetto;
                                     puts("\nOggetto raccolto.\n");
        break;
                        
        case bussola: lista_giocatori[i]->zaino[inventario]=bussola;
                      lista_giocatori[i]->pos_mondoreale->oggetto=nessun_oggetto;
                      puts("\nOggetto raccolto.\n");
        break;

        case schitarrata_metallica: lista_giocatori[i]->zaino[inventario]=schitarrata_metallica;
                                    lista_giocatori[i]->pos_mondoreale->oggetto=nessun_oggetto;
                                    puts("\nOggetto raccolto.\n");
        break;
                
        default: puts("\nNon c'è nulla da raccogliere qui.\n");
        break;

    }

    }

    }else{puts("\nNon puoi raccogliere l'oggetto se non hai prima sconfitto il nemico.\n");}

    }else{puts("\nNessun oggetto da raccogliere in questo mondo.\n");}

}


static void utilizza_oggetto(Giocatore **lista_giocatori, int i) {

    int inventario, c;

    do {

        puts("Scegli l'oggetto nel tuo zaino da utilizzare (0-2):"); //l'array zaino va da 0 a 2
        do{
            if(scanf("%d", &inventario)!=1){
                puts("Input non valido.\n");
                while ((c = getchar()) != '\n' && c != EOF);
                continue;}
                break;
        }while(1);

    } while (inventario<0 || inventario>2);

    enum Tipo_oggetto oggetto = lista_giocatori[i]->zaino[inventario];

    if(oggetto==nessun_oggetto){puts("\nNessun oggetto utilizzabile.\n");return;}

    switch(oggetto){

        case maglietta_fuocoinferno:
            lista_giocatori[i]->difesa_pischica+=10;
            puts("\nLa tua difesa pischica aumenta di 10!\n");
            break;

        case schitarrata_metallica:
            lista_giocatori[i]->attacco_pischico += 10;
            puts("\nIl tuo attacco pischico aumenta di 10!\n");
            break;

        case bussola:
            lista_giocatori[i]->fortuna += 10;
            puts("\nLa tua fortuna aumenta di 10!\n");
            break;

        case bicicletta: //permette di saltare ccompletamente una zona o di scappare da un combattimento
            if (lista_giocatori[i]->mondo==mondo_reale){if(lista_giocatori[i]->pos_mondoreale->avanti!=NULL){
                lista_giocatori[i]->pos_mondoreale=lista_giocatori[i]->pos_mondoreale->avanti;}
            }else{
                if(lista_giocatori[i]->pos_soprasotto->avanti!=NULL){
                   lista_giocatori[i]->pos_soprasotto=lista_giocatori[i]->pos_soprasotto->avanti;}
            }
            puts("\nSei andato in avanti di due zone!\n");
            break;

        default:
            puts("\nNessun oggetto utilizzabile.\n");
            return;
    }

    lista_giocatori[i]->zaino[inventario]=nessun_oggetto; //lo slot diventa vuoto una volta utilizzato l'oggetto
}




void imposta_gioco(void){

   srand(time(NULL));
   imposta_giocatori();
   imposta_mappa();


}

void gioca(void) {
    
    system("clear");
    puts("Ora inizia il gioco!\n");

    int opzione, c;

    for (int j = 0; j < giocatori; j++) {
        lista_giocatori[j]->pos_mondoreale = prima_zona_mondoreale;
        lista_giocatori[j]->pos_soprasotto = prima_zona_mondoreale->link_soprasotto;
        nomi_giocatori[j] = strdup(lista_giocatori[j]->nome); //salvo i nomi dei giocatori a parte per i saluti finali
    }


    giocatori_vivi = giocatori;

    do{

        for (int i = giocatori - 1; i > 0; i--){ //per randomizzare i turni
            int j = rand() % (i + 1);
            Giocatore *temp = lista_giocatori[i];
            lista_giocatori[i] = lista_giocatori[j];
            lista_giocatori[j] = temp;
        }

        for (int i=0; i<giocatori; i++){
            if(!demotorzone_bool){break;} //interrompo il gioco se il demotorzone è stato sconfitto
            if(lista_giocatori[i] == NULL){continue;} 
            bool movimento=false; //per regolare una singola avanzata per turno
            fuga=false; //si può evitare di combattere nel soprasotto una volta per turno
            do{
            if(lista_giocatori[i]->difesa_pischica<=0){ //se il giocatore muore viene liberato dalla memoria dinamica
            free(lista_giocatori[i]);
            lista_giocatori[i]=NULL;
            giocatori_vivi--;
            break;}
            printf("È il turno di: %s\n\n", lista_giocatori[i]->nome);
            puts("1) Avanza");
            puts("2) Indietreggia");
            puts("3) Cambia mondo");
            puts("4) Combatti");
            puts("5) Stampa giocatore");
            puts("6) Stampa zona");
            puts("7) Raccogli oggetto");
            puts("8) Utilizza oggetto");
            puts("9) Passa il turno");
            puts("10) Esci dal gioco");

            
            do{
                if(scanf("%d", &opzione)!=1){
                    puts("Input non valido.\n");
                    while ((c = getchar()) != '\n' && c != EOF);
                    continue;}
                    break;
            }while(1);
            
            system("clear");

            if(opzione==10){return;}

            switch(opzione) {

                case 1: if(!movimento){avanza(lista_giocatori, i); movimento=true;}
                        else{puts("Non puoi avanzare se hai cambiato mondo o se sei avanzato già una volta.\n");}
                        break;

                case 2: indietreggia(lista_giocatori, i);
                        break;

                case 3: if(lista_giocatori[i]->mondo==mondo_reale){
                        if(!movimento){cambia_mondo(lista_giocatori, i); movimento=true;}
                        else{puts("Non puoi cambiare mondo se sei avanzato già una volta o hai già cambiato.\n");}}
                        else{cambia_mondo(lista_giocatori, i);}
                        break;

                case 4: combatti(lista_giocatori, i);
                        break;

                case 5: stampa_giocatore(lista_giocatori[i]);
                        break;

                case 6: if(lista_giocatori[i]->mondo==mondo_reale){stampa_zona_reale(lista_giocatori[i]->pos_mondoreale);}
                        else{stampa_zona_sottopra(lista_giocatori[i]->pos_soprasotto);}
                        break;

                case 7: raccogli_oggetto(lista_giocatori, i);
                        break;

                case 8: utilizza_oggetto(lista_giocatori, i); 
                        break;

                case 9:
                        break;

                default: puts("Scelta non valida");
                         break;
            }

            
            }while(lista_giocatori[i]->difesa_pischica>0 && opzione!=9 && demotorzone_bool);

        if(giocatori_vivi==0){puts("\nE non ne rimase più nessuno...\n");}

    }
    
    }while (giocatori_vivi>0 && demotorzone_bool); //il gioco continua finché c'è qualcuno in vita e il demotorzone è ancora vivo

    puts("Fine del gioco!");
    UndiciVirgolaCinque_bool=false;
}


void termina_gioco(void){

    puts("\n\nGrazie a: \n");
    
    for(int i=0; i<giocatori; i++){
        printf("%s\n", nomi_giocatori[i]);
    }

    puts("\nper aver giocato!\n");

    for (int j = 0; j < giocatori; j++) { //libero i giocatori sopravissuti
        if(lista_giocatori[j] != NULL) free(lista_giocatori[j]);
        if(nomi_giocatori[j] != NULL) {
            free(nomi_giocatori[j]);
            nomi_giocatori[j] = NULL;} 
    }    

    Zona_mondoreale *temp1 = prima_zona_mondoreale;

    while (temp1 != NULL) { //libero la mappa
    Zona_mondoreale *temp2 = temp1->avanti;
        if(temp1->link_soprasotto != NULL) free(temp1->link_soprasotto);
        free(temp1);
        temp1 = temp2;
    }
}

void crediti(void){

    puts("Game Designer: Alsen Xhani\nGame Writer: Alsen Xhani \nGame Director: Alsen Xhani \nGame Producer: Alsen Xhani");

    puts("\nI vincitori delle scorse 3 partite:\n");

for(int i = 0; i < vincitori; i++) {
    if(nomi_vincitori[i] != NULL) {
        printf("%s\n\n\n\n\n\n", nomi_vincitori[i]); //salvo a parte i nomi dei vincitori
    }
}

}
