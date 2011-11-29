#ifndef MODIFICATEURS_H
#define MODIFICATEURS_H

    #include "header.h"

    // Procedure reinitialiser(plateau)
    // {replace les pions}
    // Parametres : (R) plateau : Plateau
    void reinitialiser(Case plateau[TAILLE_PLATEAU][TAILLE_PLATEAU]);

    // Fonction manger(plateau, sourceX, sourceY, caseX, caseY) retourne booleen
    // {deplace en diagonale (si en diagonale) et mange tout ce qu'il y a sur le chemin,
    //  (cannibale ou pas) retourne vrai si le pion a mange}
    // Parametres : (D/R) plateau : Plateau
    //              (D) sourceX, sourceY, caseX, caseY : entiers
    bool manger(Case plateau[TAILLE_PLATEAU][TAILLE_PLATEAU], int sourceX, int sourceY, int caseX, int caseY);

    // Procedure egal(aChanger, aSauver)
    // {met le contenu de aSauver dans aChanger}
    // Parametres : (R) aChanger : Plateau
    //              (D) aSauver  : Plateau
    void egalPlateau(Case aChanger[TAILLE_PLATEAU][TAILLE_PLATEAU], Case (*const aSauver)[TAILLE_PLATEAU]);

#endif
