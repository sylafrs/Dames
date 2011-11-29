#ifndef HEADER_H_
#define HEADER_H_

    #include <stdlib.h>

    //Configuration utilisee
    #include "Configs/configC.h"

    //defines dependants des parametres
    #define ZDJ TAILLE_PLATEAU*TAILLE_CASES //taille de l'ecran en pixel

    typedef enum bool { // un booleen est un type de variable qui peut prendre deux valeurs :
        true = 1,       // - vrai
        false = 0       // - faux
    } bool ;

    typedef enum Joueur { // deux joueurs :
        blanc = 0,        // - blanc
        noir = 1          // - noir
    } Joueur ;

    typedef enum Pion { // un pion peut etre :
        non = 0,        // - pas là
        normal = 1,     // - normal
        reine = 2       // - une reine
    } Pion ;

    //Structure d'une case
    typedef struct Case {
        Pion pion;          // y a t-il un pion, une reine ou rien ?
        Joueur joueur;      // a qui appartient la case ?
    } Case ;

#endif//HEADER_H_
