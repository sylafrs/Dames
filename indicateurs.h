#ifndef INDICATEURS_H_
#define INDICATEURS_H_

    #include "header.h"

    // Fonction possible(plateau, sourceX, sourceY, caseX, caseY) retourne booleen
    // {dit si la case source peut se deplacer sur la case donnee}
    // Parametres : (D) plateau : Plateau
    //              (D) sourceX, sourceY, caseX, caseY : entiers
    bool possible(Case (*const plateau)[TAILLE_PLATEAU], int sourceX, int sourceY, int caseX, int caseY);

    // Fonction selectionnable(plateau, caseX, caseY) retourne booleen
    // {dit si la case peut etre selectionnee (en fonction du joueur possedant cette derniere)}
    // Parametres : (D) plateau : Plateau
    //              (D) caseX, caseY : entiers
    bool selectionnable(Case (*const plateau)[TAILLE_PLATEAU], int caseX, int caseY);

    // Fonction peutManger(plateau, caseX, caseY) retourne booleen
    // {dit si la case peut manger une autre, Fonction utilisee dans beaucoup d'autres fonctions}
    // Parametres : (D) plateau : Plateau
    //              (D) caseX, caseY : entiers
    bool peutManger(Case (*const plateau)[TAILLE_PLATEAU], int caseX, int caseY);

    // Fonction gagner(plateau) retourne booleen
    // {est vrai en cas de victoire du joueur}
    // Parametres : (D) plateau : Plateau
    //              (D) joueur  : Joueur
    bool gagner(Case (*const plateau)[TAILLE_PLATEAU], Joueur joueur);


    // Fonction bloqie(plateau) retourne booleen
    // {est vrai si le joueur ne peut pas bouger}
    // Parametres : (D) plateau : Plateau
    //              (D) joueur  : Joueur
    bool bloque(Case (*const plateau)[TAILLE_PLATEAU], Joueur joueur);

#endif
