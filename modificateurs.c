#include "modificateurs.h"
#include <stdio.h>

//ICI : le tableau de cases (le plateau) est retourné implicitement (pas de const)

void reinitialiser(Case plateau[TAILLE_PLATEAU][TAILLE_PLATEAU])
{
    long i, j;

    // les LIGNE_DEPART lignes de TAILLE_PLATEAU cases remplies de pions du joueur 1
    for(j = 0; j < LIGNE_DEPART; j++)
    {
        for(i = 0; i < TAILLE_PLATEAU; i++)
        {
            if((i+j)%2 == 1)
            {
                plateau[i][j].pion = normal;
                plateau[i][j].joueur = noir;
            }
            else
            {
                plateau[i][j].pion = non;
            }
        }
    }

    //le no man's land
    for(j = LIGNE_DEPART; j < TAILLE_PLATEAU-LIGNE_DEPART; j++)
    {
        for(i = 0; i < TAILLE_PLATEAU; i++)
        {
            plateau[i][j].pion = non;
        }
    }

    // les lignes du joueur 2
    for(j = TAILLE_PLATEAU-LIGNE_DEPART; j < TAILLE_PLATEAU; j++)
    {
        for(i = 0; i < TAILLE_PLATEAU; i++)
        {
            if((i+j)%2 == 1)
            {
                plateau[i][j].pion = normal;
                plateau[i][j].joueur = blanc;
            }
            else
            {
                plateau[i][j].pion = non;
            }
        }
    }
}


bool manger(Case (*const plateau)[TAILLE_PLATEAU], int sourceX, int sourceY, int caseX, int caseY)
{
    bool aMange = false;    //true si on a mangé
    long i = 0;

    if(caseX < 0 || caseY < 0 || sourceX < 0 || sourceY < 0 || caseX >= TAILLE_PLATEAU || caseY >= TAILLE_PLATEAU || sourceX >= TAILLE_PLATEAU || sourceY >= TAILLE_PLATEAU )
    {
        fprintf(stderr, "Erreur : coordonnees erronnees dans manger");
        exit(EXIT_FAILURE);
    }

    //sur la diagonale
    if(abs(caseX-sourceX) == abs(caseY-sourceY))
    {
        //le pion va a sa destination
        plateau[caseX][caseY] = plateau[sourceX][sourceY];
        plateau[sourceX][sourceY].pion = non;

        //si c'est un blanc et qu'il arrive en haut, ou si c'est un noir en bas
        if((caseY == 0 && plateau[sourceX][sourceY].joueur == blanc) || (caseY == TAILLE_PLATEAU-1 && plateau[sourceX][sourceY].joueur == noir))
            plateau[caseX][caseY].pion = reine;

        //en mangeant tout sur son passage :
        //on part du principe que toutes les verifications ont été faites avant (bonne source et bonne destination)

        //pour chaque case sur la destination
        for(i = 1; i < abs(caseX-sourceX); i++)
        {
            //diagonale bas-droite (si il y a quelque chose)
            if(caseX-sourceX > 0 && caseY-sourceY > 0 && plateau[sourceX+i][sourceY+i].pion != non)
            {
                //alors on le mange
                aMange = true;
                plateau[sourceX+i][sourceY+i].pion = non;
            }

            //diagonale bas-gauche (si il y a quelque chose)
            else if(caseX-sourceX > 0 && caseY-sourceY < 0 && plateau[sourceX+i][sourceY-i].pion != non)
            {
                //alors on le mange
                aMange = true;
                plateau[sourceX+i][sourceY-i].pion = non;
            }

            //diagonale haut-droite (si il y a quelque chose)
            else if(caseX-sourceX < 0 && caseY-sourceY > 0 && plateau[sourceX-i][sourceY+i].pion != non)
            {
                //alors on le mange
                aMange = true;
                plateau[sourceX-i][sourceY+i].pion = non;
            }

            //diagonale haut-gauche (si il y a quelque chose)
            else if(caseX-sourceX < 0 && caseY-sourceY < 0 && plateau[sourceX-i][sourceY-i].pion != non)
            {
                //alors on le mange
                aMange = true;
                plateau[sourceX-i][sourceY-i].pion = non;
            }
        }
    }

    return aMange;
}

void egalPlateau(Case aChanger[TAILLE_PLATEAU][TAILLE_PLATEAU], Case (*const aSauver)[TAILLE_PLATEAU])
{
    int i, j;

    //pour chaque case
    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            //même contenu.
            aChanger[i][j].pion = aSauver[i][j].pion;
            aChanger[i][j].joueur = aSauver[i][j].joueur;
        }
    }
}
