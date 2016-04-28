//
//  main.c
//  projet_info
//
//  Created by Baptiste Boudier on 21/04/2016.
//  Copyright © 2016 Baptiste Boudier. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "liste.h"

/////////////////////////////////// liste.c //////////////////////////////////////////////////////////

void afficher(elem * pelt){
    printf("sommet d'arrivée : %ld coût de l'arc : %lf\n",pelt->sommet_arrivee,pelt->cout);
}

Liste creer_liste(){
    return NULL;
}

int est_vide(Liste l){
    return !l;
}

void visualiser_liste(Liste l){
    Liste p = l;
    while(!est_vide(p)){
        afficher(&(p->val));
        p=p->suiv;
    }
}

Liste ajout_tete(elem c, Liste l){
    Liste nl;
    nl=calloc(1,sizeof(*nl));
    if(nl==NULL)
        return NULL;
    nl->suiv=l;
    nl->val=c;
    return nl;
}

Liste supprimer_tete(Liste l){
    if(l==NULL)
        return NULL;
    Liste temp;
    temp = (l->suiv);
    free(l);
    return temp;
}

Liste ajout_queue(elem c, Liste l){
    Liste nl,p;
    p=l;
    nl = calloc(1,sizeof(*nl));
    while((p->suiv) != NULL){
        p = (p->suiv);
    }
    (p->suiv)=nl;
    (nl->val)=c;
    return l;
}

Liste supprimen(int n, Liste l){
    Liste p,temp;
    maillon * ad;
    p=l;
    int i;
    for(i=0;i<n-1;i++){
        p = (p->suiv);
    }
    temp = p;
    p = (p->suiv);
    (temp->suiv)=(p->suiv);
    free(p);
    return l;
}

Liste concat(Liste l1,Liste l2){
    while(l2!=NULL){
        ajout_queue(l2->val,l1);
        l2=l2->suiv;
    }
    return l1;
}

////////////////////////////////////////////// liste_hash.c /////////////////////////////////////////
typedef long elem_hash; //dans elem_hash on va stocher le num de station correspondant au nom de la station
struct cellule_hash {
    elem_hash val; 
    struct cellule_hash * suiv;
};
typedef struct cellule_hash maillon_hash;
typedef maillon_hash * Liste_hash;

Liste_hash creer_liste_hash(){
    return NULL;
}

int est_vide_hash(Liste_hash l){
    return !l;
}

Liste_hash ajout_tete_hash(elem_hash e, Liste_hash l){
    Liste_hash nl;
    nl=calloc(1,sizeof(*nl));
    if(nl==NULL) 
        return NULL;
    nl->suiv=l;
    nl->val=e;
    return nl;
}

Liste_hash supprimer_tete_hash(Liste_hash l){
    if(l==NULL) 
        return NULL;
    Liste_hash temp;
    temp = (l->suiv);
    free(l);
    return temp;
}

Liste_hash ajout_queue_hash(elem_hash c, Liste_hash l){
    Liste_hash nl,p;
    p=l;
    nl = calloc(1,sizeof(*nl));
    while((p->suiv) != NULL){
        p = (p->suiv);
    }
    (p->suiv)=nl;
    (nl->val)=c;
    return l;
}

long hachage(char * mot, long N){
    int a = 1;
    char * p = mot;
    int h = 0;
    while(p!='\0'){
        h += ((*p)*a)%N;
        a = (a*31)%N;
        p++;
    }
    return h;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
struct cellule1 {
    double poids;
    long pere;
};

typedef struct cellule1 sommet;

void inverse(long * tab, long taille) {
    long i = 0, tmp;
    for(i=0 ; i < taille/2 ; i++) {
        tmp = tab[i];
        tab[i] = tab[taille-i-1];
        tab[taille-i-1] = tmp;
    }
}

//lecture retourne la matrice d'incidence a partir d'un .txt et passe par pointeur le nb de sommet et d'arcs


Liste * lecture(char * nom_fichier, long * nb_sommet, long * nb_arc, Liste_hash * table_hash){
    int taille_maximale = 512;
    long i;
    long indice,successeur;
    double cout;
    elem element_liste;
    Liste * Matrice_incidence;
    char tampon[taille_maximale];

    Liste_hash * table;
    table = calloc(*nb_sommet,sizeof(*table)); //on prend une taile de table de hachage aussi longue que le nb de station comme ça on a une bijection entre les noms et l'indice de hachage
    long numero_station;
    double coord_x, coord_y;
    char ligne[taille_maximale];
    char nom_station[taille_maximale];
    long indice_hash;
    
    FILE * fichier = NULL;
    fichier = fopen(nom_fichier , "r");
    if(!fichier) {
        perror("fopen");
        exit(1);
    }
    
    else{
        fscanf(fichier, "%ld %ld", nb_sommet, nb_arc);
        printf("nb de som : %ld nb d'arc : %ld",*nb_sommet,*nb_arc);
        Matrice_incidence = calloc(*nb_sommet,sizeof(* Matrice_incidence));
        if(Matrice_incidence == NULL) return NULL;
        fgets(tampon, taille_maximale, fichier);
        printf("%s", tampon);
        fgets(tampon, taille_maximale, fichier);
        printf("%s", tampon);
        for(i=0;i<*nb_sommet;i++){  
            fscanf(fichier, "%ld %lf %lf %s %s\n", &numero_station, &coord_x, &coord_y, ligne, nom_station); 
            printf("num station : %ld ",numero_station);
            indice_hash = hachage(nom_station, * nb_sommet);    
            table[indice_hash]= ajout_tete_hash(numero_station,table[indice_hash]);
            //fgets(tampon, taille_maximale, fichier); //permet d'aller jusqu'au debur de la description des arcs
            //printf("%s", tampon);
        }
        table_hash = table;

        fgets(tampon, taille_maximale, fichier);
        printf("%s", tampon);
        
        for(i=0;i<*nb_arc;i++){
            fscanf(fichier, "%ld %ld %lf", &indice, &successeur, &cout);    //fscanf(fichier, "%ld  %ld %lf", &indice, &element_liste.sommet_arrivee, &element_liste.cout);
            element_liste.sommet_arrivee = successeur;                         // permet de ne pas definir successeur et cout et supprimme deux lignes de code
            element_liste.cout = cout;
            Matrice_incidence[indice] = ajout_tete(element_liste, Matrice_incidence[indice]);
        }
        fclose(fichier);
        return Matrice_incidence;
    }
    
}

//cette fcn alloue graph et l'initialise
sommet * initialisation_graph(long nb_sommet, long sommet_depart){
    sommet * graph;
    graph = calloc(nb_sommet,sizeof(* graph));
    long i;
    for(i=0;i<nb_sommet;i++){
        (&graph[i])->poids = -1; //-1=infini
        (&graph[i])->pere = -1;
    }
    (&graph[sommet_depart])->poids = 0;
    (&graph[sommet_depart])->pere = sommet_depart;
    return graph;
    
}


//cette fonction retournne le plus court chemin sous forme de tableau de long sous le format : depart som1 som2 arrivee -1
//-1 indique la fin du tableau, ce qu'il y a avant est le trajet
//si -1 est le premier element du tableau cela signifie qu'il n'existe pas de trajet
long * recherche_plus_court_chemin(Liste * Matrice_incidence, long nb_sommet, long sommet_depart, long sommet_arrivee){
    sommet * graph = initialisation_graph(nb_sommet,sommet_depart);
    long i,u,v;
    double test_poids;
    Liste liste;
    //calcul du graph
    // for(i=1;i<nb_sommet;i++){
        for(u=0;u<nb_sommet;u++){         //on ne fais pas un for(v=0;v<nb_sommet,v++) car grace a la matrice on connait les arcs ->temps gagné
            liste = Matrice_incidence[u];
            while(liste){ //tant qu'on reste dan sla liste
                v = liste->val.sommet_arrivee;
                if((&graph[v])->poids != -1 && (&graph[u])->poids != -1){ //graph[liste->val.sommet_arrivee]->poids  c'est le poids du sommet d'arrivé , poids de v dans l'enoncé
                    if(((&graph[u])->poids + liste->val.cout) < (&graph[v])->poids){
                        test_poids=((&graph [u])->poids + liste->val.cout);
                        (&graph[v])->poids = ((&graph[u])->poids + liste->val.cout); //poids(v) = poids(u) + poids(arc(u,v))
                        (&graph[v])->pere = u; //mise à jour pere
                    }
                }
                if((&graph[v])->poids == -1 && (&graph[u])->poids != -1){
                    test_poids=((&graph [u])->poids + liste->val.cout);
                    (&graph[v])->poids = ((&graph [u])->poids + liste->val.cout); //poids(v) = poids(u) + poids(arc(u,v))
                    (&graph[v])->pere = u;
                    }
                liste = (liste->suiv);
            }
        }
   // }
    //calcul plus court chemin
    long * plus_court_chemin;
    plus_court_chemin = calloc(nb_sommet,sizeof(* plus_court_chemin)); //on est sur que ca sera assez long, la taille du chemin < nb sommet
    long indice = sommet_arrivee;
    long k = 0;
    long stock_pere;
    do{                                  //on remonte le graph pour trouver le plus court chemin
        plus_court_chemin[k] = indice;
        stock_pere = (&graph[indice])->pere;
        indice = stock_pere;
        k++;
    } while(indice != sommet_depart && stock_pere != -1); //si un des pere vaut -1 alors on arrete car chemin impossible mais il peut y avoir le cas d'une boucle infini peut etre donc peut etre mettre la condition sur k ie on arrete quand k>nombre de sommet car le trajet ne peut pas exceder le nb de sommet ->  && k<=nb_sommet
    if(stock_pere != -1){                    //if(k<=nb_sommet)
        plus_court_chemin[k]= sommet_depart;
        plus_court_chemin[k+1]= -1;      // convention prise on aura donc en fin de chemin le plus court la valeur -1, utile pour la suite quand on voudrat savoir que le trajet est fini
        inverse(plus_court_chemin,k+1); //on met le trajet dan sle bon sens
        return plus_court_chemin;
    }
    else{
        *plus_court_chemin = -1; // si le trajet est impossible on a -1 comme premier elem du tableau
        return plus_court_chemin;
    }
}

int main(){
    Liste_hash table_hash[512];
    long sommet;
    long arc;
    Liste * mat;
    mat = lecture("graphe1.txt",&sommet,&arc,table_hash);
    printf("%ld",sommet);
}

/*
int main(){
    Liste * Matrice_incidence;
    elem element_liste;
    Matrice_incidence = calloc(8,sizeof(* Matrice_incidence));
    element_liste.sommet_arrivee = 1;
    element_liste.cout = 1698;
    Matrice_incidence[0]=ajout_tete(element_liste, Matrice_incidence[0]);
    element_liste.sommet_arrivee = 2;
    element_liste.cout = 2370;
    Matrice_incidence[0]=ajout_tete(element_liste, Matrice_incidence[0]);
    element_liste.sommet_arrivee = 3;
    element_liste.cout = 1047;
    Matrice_incidence[0]=ajout_tete(element_liste, Matrice_incidence[0]);
    element_liste.sommet_arrivee = 2;
    element_liste.cout = 1819;
    Matrice_incidence[1]=ajout_tete(element_liste, Matrice_incidence[1]);
    element_liste.sommet_arrivee = 4;
    element_liste.cout = 978;
    Matrice_incidence[1]=ajout_tete(element_liste, Matrice_incidence[1]);
    element_liste.sommet_arrivee = 5;
    element_liste.cout = 548;
    Matrice_incidence[1]=ajout_tete(element_liste, Matrice_incidence[1]);
    element_liste.sommet_arrivee = 3;
    element_liste.cout = 1678;
    Matrice_incidence[2]=ajout_tete(element_liste, Matrice_incidence[2]);
    element_liste.sommet_arrivee = 5;
    element_liste.cout = 2134;
    Matrice_incidence[2]=ajout_tete(element_liste, Matrice_incidence[2]);
    element_liste.sommet_arrivee = 5;
    element_liste.cout = 468;
    Matrice_incidence[4]=ajout_tete(element_liste, Matrice_incidence[4]);
    element_liste.sommet_arrivee = 7;
    element_liste.cout = 621;
    Matrice_incidence[4]=ajout_tete(element_liste, Matrice_incidence[4]);
    element_liste.sommet_arrivee = 5;
    element_liste.cout = 1498;
    Matrice_incidence[6]=ajout_tete(element_liste, Matrice_incidence[6]);
    element_liste.sommet_arrivee = 5;
    element_liste.cout = 913;
    Matrice_incidence[7]=ajout_tete(element_liste, Matrice_incidence[7]);
 
    long nb_sommet = 8;
    long sommet_depart = 7;
    long sommet_arrivee = 4;
    long * tab;
    tab = recherche_plus_court_chemin(Matrice_incidence,nb_sommet,sommet_depart,sommet_arrivee);
    int i;
    for(i=0;i<10;i++){
 printf("%ld ", tab[i]);
    }
 }*/
 

//test initialisation_graph
/*int main(){
    long nbsom = 8;
    long somdep = 3;
    sommet * graph;
    int i;
    graph = initialisation_graph(nbsom, somdep);
    for(i=0;i<nbsom;i++){
        printf("poids : %lf pere : %ld \n",(&graph[i])->poids,(&graph[i])->pere);
    }
}*/



