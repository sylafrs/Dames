#include "indicateurs.h"
#include <stdio.h>

//ICI : le tableau de cases n'est pas modifié !

//fonction qui repond à la question : "mon pion ou ma dame peut-il aller là ?"
bool possible(Case (*const plateau)[TAILLE_PLATEAU], int sourceX, int sourceY, int caseX, int caseY)
{
    bool possible = false;
    int i;

    if(caseX < 0 || caseY < 0 || sourceX < 0 || sourceY < 0 || caseX >= TAILLE_PLATEAU || caseY >= TAILLE_PLATEAU || sourceX >= TAILLE_PLATEAU || sourceY >= TAILLE_PLATEAU )
    {
        return false;
    }

    //si en diagonale et case d'arrivee vide...
    if(plateau[caseX][caseY].pion == non && abs(caseX-sourceX) == abs(caseY-sourceY))
    {
        //pion normal
        if(plateau[sourceX][sourceY].pion == normal)
        {
            //si on mange
            if(peutManger(plateau, sourceX, sourceY))
            {
                //si la case se trouve a deux d'intervalle et qu'entre il y ait un pion adverse
                //caseX-sourceX : ecart sur X
                //si l'ecart est de 2 : sourceX+((caseX - sourceX)/2) est l'abscisse de la case entre !
                if(abs(caseX-sourceX) == 2)
                    possible = plateau[sourceX+((caseX - sourceX)/2)][sourceY+((caseY - sourceY)/2)].joueur != plateau[sourceX][sourceY].joueur && plateau[sourceX+((caseX - sourceX)/2)][sourceY+((caseY - sourceY)/2)].pion != non;
            }

            //on ne mange pas
            else if(abs(caseX-sourceX) == 1)
            {
                //les blancs : vers le haut
                if(plateau[sourceX][sourceY].joueur == blanc)
                    possible = (caseY-sourceY == -1);

                //les noirs : vers le bas
                else
                    possible = (caseY-sourceY == +1);
            }
        }

        //les reines
        else
        {
            //ne peux pas faire de cannibalisme ou sauter un groupe adverse.
            possible = true;
            i = 1;

            //en bas à droite
            if(caseX-sourceX > 0 && caseY-sourceY > 0)
            {
                //continue jusqu'au prochain obstacle
                while(possible && sourceX+i < caseX)
                {
                    possible = (plateau[sourceX+i][sourceY+i].pion == non);
                    i++;
                }
            }

            //en bas à gauche
            else if(caseX-sourceX > 0 && caseY-sourceY < 0)
            {
                //continue jusqu'au prochain obstacle
                while(possible && sourceX+i < caseX)
                {
                    possible = (plateau[sourceX+i][sourceY-i].pion == non);
                    i++;
                }
            }

            //en haut à gauche
            else if(caseX-sourceX < 0 && caseY-sourceY < 0)
            {
                //continue jusqu'au prochain obstacle
                while(possible && sourceX-i > caseX)
                {
                    possible = (plateau[sourceX-i][sourceY-i].pion == non);
                    i++;
                }
            }

            //en haut à droite
            else if(caseX-sourceX < 0 && caseY-sourceY > 0)
            {
                //continue jusqu'au prochain obstacle
                while(possible && sourceX-i > caseX)
                {
                    possible = (plateau[sourceX-i][sourceY+i].pion == non);
                    i++;
                }
            }

            //si on peut manger : on doit manger !
            //sinon, possible garde sa valeur (on ne mange pas, donc pas d'obstacle)
            if(peutManger(plateau, sourceX, sourceY))
            {
                //obligé de manger !!!
                possible = !possible;

                //i = premiere case rencontree

                //en bas à droite, si l'obstacle rencontré est le joueur
                if(caseX-sourceX > 0 && caseY-sourceY > 0 && plateau[sourceX+i-1][sourceY+i-1].joueur != plateau[sourceX][sourceY].joueur)
                {
                    //toutes les cases derriere le premier obstacle sont possibles jusqu'au deuxieme obstacle
                    while(possible && sourceX+i < caseX)
                    {
                        possible = (plateau[sourceX+i][sourceY+i].pion == non);
                        i++;
                    }
                }

                //en bas à gauche
                else if(caseX-sourceX > 0 && caseY-sourceY < 0 && plateau[sourceX+i-1][sourceY-(i-1)].joueur != plateau[sourceX][sourceY].joueur)
                {
                    //toutes les cases derriere le premier obstacle sont possibles jusqu'au deuxieme obstacle
                    while(possible && sourceX+i < caseX)
                    {
                        possible = (plateau[sourceX+i][sourceY-i].pion == non);
                        i++;
                    }
                }

                //en haut à gauche
                else if(caseX-sourceX < 0 && caseY-sourceY < 0 && plateau[sourceX-(i-1)][sourceY-(i-1)].joueur != plateau[sourceX][sourceY].joueur)
                {
                    //toutes les cases derriere le premier obstacle sont possibles jusqu'au deuxieme obstacle
                    while(possible && sourceX-i > caseX)
                    {
                        possible = (plateau[sourceX-i][sourceY-i].pion == non);
                        i++;
                    }
                }

                //en haut à droite
                else if(caseX-sourceX < 0 && caseY-sourceY > 0 && plateau[sourceX-(i-1)][sourceY+i-1].joueur != plateau[sourceX][sourceY].joueur)
                {
                    //toutes les cases derriere le premier obstacle sont possibles jusqu'au deuxieme obstacle
                    while(possible && sourceX-i > caseX)
                    {
                        possible = (plateau[sourceX-i][sourceY+i].pion == non);
                        i++;
                    }
                }

                //on ne peux pas manger, mais on peux manger ? oO
                else
                    possible = false;
            }
        }
    }

    return possible;
}

bool selectionnable(Case (*const plateau)[TAILLE_PLATEAU], int caseX, int caseY)
{
    int i,j = 0;
    bool selectionnable = false;
    bool souffle;

    if(caseX < 0 || caseY < 0 || caseX >= TAILLE_PLATEAU || caseY >= TAILLE_PLATEAU)
    {
        fprintf(stderr, "Erreur : coordonnees erronnees dans selectionnable");
        exit(EXIT_FAILURE);
    }

    //on ne selectionne pas une case vide...
    if(plateau[caseX][caseY].pion != non)
    {
        //si on peut manger : on peut selectionner !
        selectionnable = peutManger(plateau, caseX, caseY);

        //sinon : il faut qu'aucune case ne puisse manger et que le deplacement soit possible !
        if(!selectionnable)
        {
            //regarde si aucun pion ne peut manger
            souffle = false;
            while(!souffle && j < TAILLE_PLATEAU)
            {
                i = 0;
                while(!souffle && i < TAILLE_PLATEAU)
                {
                    if(plateau[i][j].pion != non && plateau[i][j].joueur == plateau[caseX][caseY].joueur)
                        souffle = peutManger(plateau, i, j);
                    i++;
                }
                j++;
            }

            if(!souffle)
            {
                //si on peut bouger

                //cas de la reine
                if(plateau[caseX][caseY].pion == reine)
                    selectionnable = possible(plateau, caseX, caseY, caseX-1, caseY+1) || possible(plateau, caseX, caseY, caseX+1, caseY+1) || possible(plateau, caseX, caseY, caseX-1, caseY-1) || possible(plateau, caseX, caseY, caseX+1, caseY-1);

                //cas du pion noir
                else if(plateau[caseX][caseY].joueur == noir)
                    selectionnable = possible(plateau, caseX, caseY, caseX-1, caseY+1) || possible(plateau, caseX, caseY, caseX+1, caseY+1);

                //cas du pion blanc
                else
                    selectionnable = possible(plateau, caseX, caseY, caseX-1, caseY-1) || possible(plateau, caseX, caseY, caseX+1, caseY-1);
            }

            //(else selectionnable = false;)
        }
    }

    return selectionnable;
}

bool peutManger(Case (*const plateau)[TAILLE_PLATEAU], int caseX, int caseY)
{
    bool resultat = false, stop;
    int i;

    if(caseX < 0 || caseY < 0 || caseX >= TAILLE_PLATEAU || caseY >= TAILLE_PLATEAU)
    {
        fprintf(stderr, "Erreur : coordonnees erronnees dans peutManger");
        exit(EXIT_FAILURE);
    }

    if(plateau[caseX][caseY].pion == normal)
    {
        //si on n'est pas au bord :
            //si ya un pion adverse adjacent et qu'il y a de la place derriere

        if(caseX < TAILLE_PLATEAU-2 && caseY < TAILLE_PLATEAU-2)
            resultat = plateau[caseX+2][caseY+2].pion == non && plateau[caseX+1][caseY+1].pion != non && plateau[caseX+1][caseY+1].joueur != plateau[caseX][caseY].joueur;

        if(!resultat && caseX < TAILLE_PLATEAU-2 && caseY > 1)
            resultat = plateau[caseX+2][caseY-2].pion == non && plateau[caseX+1][caseY-1].pion != non && plateau[caseX+1][caseY-1].joueur != plateau[caseX][caseY].joueur;

        if(!resultat && caseX > 1 && caseY > 1)
            resultat = plateau[caseX-2][caseY-2].pion == non && plateau[caseX-1][caseY-1].pion != non && plateau[caseX-1][caseY-1].joueur != plateau[caseX][caseY].joueur;

        if(!resultat && caseX > 1 && caseY < TAILLE_PLATEAU-2)
            resultat = plateau[caseX-2][caseY+2].pion == non && plateau[caseX-1][caseY+1].pion != non && plateau[caseX-1][caseY+1].joueur != plateau[caseX][caseY].joueur;
    }
    else if(plateau[caseX][caseY].pion == reine)
    {
        //en gros il faut qu'il y ait sur au moins une de ses diagonales, un pion adverse puis un espace
        //(mais pas de pion de notre couleur sur le chemin)

        resultat = false;

        //sur la diagonale bas-droite
        stop = false;
        i = 1;
        while(!stop && !resultat && caseX+i < TAILLE_PLATEAU-1 && caseY+i < TAILLE_PLATEAU-1)
        {
            stop = plateau[caseX+i][caseY+i].pion != non;
            resultat = stop && plateau[caseX+i][caseY+i].joueur != plateau[caseX][caseY].joueur && plateau[caseX+i+1][caseY+i+1].pion == non;
            i++;
        }

        //sur la diagonale bas-gauche
        stop = false;
        i = 1;
        while(!stop && !resultat && caseX+i < TAILLE_PLATEAU-1 && caseY-i > 0)
        {
            stop = plateau[caseX+i][caseY-i].pion != non;
            resultat = stop && plateau[caseX+i][caseY-i].joueur != plateau[caseX][caseY].joueur && plateau[caseX+i+1][caseY-(i+1)].pion == non;
            i++;
        }

        //sur la diagonale haut-gauche
        stop = false;
        i = 1;
        while(!stop && !resultat && caseX-i > 0 && caseY-i > 0)
        {
            stop = plateau[caseX-i][caseY-i].pion != non;
            resultat = stop && plateau[caseX-i][caseY-i].joueur != plateau[caseX][caseY].joueur && plateau[caseX-(i+1)][caseY-(i+1)].pion == non;
            i++;
        }

        //sur la diagonale haut-droite
        stop = false;
        i = 1;
        while(!stop && !resultat && caseX-i > 0 && caseY+i < TAILLE_PLATEAU-1)
        {
            stop = plateau[caseX-i][caseY+i].pion != non;
            resultat = stop && plateau[caseX-i][caseY+i].joueur != plateau[caseX][caseY].joueur && plateau[caseX-(i+1)][caseY+i+1].pion == non;
            i++;
        }
    }

    return resultat;
}

bool gagner(Case (*const plateau)[TAILLE_PLATEAU], Joueur joueur)
{
    bool victoire = true;
    int i = 0, j;

    //victoire totale si le joueur adverse n'a plus de pion
    //soit : si tous les pions presents sont les notres...
    while(i < TAILLE_PLATEAU && victoire)
    {
        j = 0;
        while(j < TAILLE_PLATEAU && victoire)
        {
            if(plateau[i][j].pion != non)
                victoire = (plateau[i][j].joueur == joueur);

            j++;
        }
        i++;
    }
    return victoire;
}

bool bloque(Case (*const plateau)[TAILLE_PLATEAU], Joueur joueur)
{
    int i = 0, j;
    bool pasBloque = false;

    //est bloqué un joueur qui ne peut rien selectionner...

    while(i < TAILLE_PLATEAU && !pasBloque)
    {
        j = 0;
        while(j < TAILLE_PLATEAU && !pasBloque)
        {
            if(plateau[i][j].pion != non && plateau[i][j].joueur == joueur)
                pasBloque = selectionnable(plateau, i, j);
            j++;
        }
        i++;
    }

    return !pasBloque;
}
