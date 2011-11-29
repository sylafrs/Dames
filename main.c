#include "SDL_include/SDL.h"
#include "SDL_include/SDL_ttf.h"
#include <stdlib.h>
#include "header.h"
#include "affichage.h"
#include "indicateurs.h"
#include "modificateurs.h"

int main(int argc, char *argv[])
{
    SDL_Surface *ecran = NULL;                      //surface affichée à l'ecran
    SDL_Event event;                                //stock l'evenement

    Case plateau[TAILLE_PLATEAU][TAILLE_PLATEAU];   //le plateau
    Case precedent[TAILLE_PLATEAU][TAILLE_PLATEAU]; //le plateau precedent (pour annuler)

    Joueur joueur;                                  //à qui le tour ?

    bool continuer = true;                          //booléen pour rester dans la boucle
    bool fin = false;                               //booléen declenchant la fin du programme
    bool appuiAbandon = false, appuiAnnuler = false;//les boutons
    bool peutAnnuler;                               //si on peut appuyer sur annuler
    bool selection;                                 //une case est-elle selectionnée
    int cliqueeX = 0, cliqueeY = 0;                 //si oui : quelles sont ces coordonnées ?
    char cause[10];                                 //chaine pour la fin...

    // Démarrage de la SDL, et de la bibliotheque de police.
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    //Ouverture de la fenetre
    #ifdef FULLSCREEN
        ecran = SDL_SetVideoMode(ECRAN_X, ECRAN_Y, NB_COULEURS, SDL_HWSURFACE|SDL_FULLSCREEN);
    #else
        ecran = SDL_SetVideoMode(ECRAN_X, ECRAN_Y, NB_COULEURS, SDL_HWSURFACE);
    #endif

    // On change le titre
    SDL_WM_SetCaption(TITRE_FENETRE, NULL);

    do
    {
        //initialisation des variables
        selection = false; // aucune case ne doit etre cliquee (securite)
        joueur = blanc; // les blancs commencent toujours

        //On initialise le plateau
        reinitialiser(plateau);
        peutAnnuler = false;

        do
        {
            //Affichage
            afficher(plateau, joueur, ecran, selection, cliqueeX, cliqueeY, appuiAbandon, appuiAnnuler);

            //Entree
            SDL_WaitEvent(&event);

            //Gestion (selon le type de l'evenement)

            //appui sur la croix (fenetre)
            if(event.type == SDL_QUIT)
                fin = true; //on s'en va...

            //appui sur une touche
            else if(event.type == SDL_KEYDOWN)
            {
                //la touche etant Echap
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    fin = true; //on s'en va...
            }

            //bouton de la souris relaché
            else if(event.type == SDL_MOUSEBUTTONUP)
            {
                //c'etait un clic gauche
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //on avais cliqué sur annuler
                    if(appuiAnnuler)
                    {
                        //si on peut annuler
                        if(peutAnnuler)
                        {
                            //on ne le peut plus
                            peutAnnuler = false;
                            //le plateau actuel est le plateau d'avant
                            egalPlateau(plateau, precedent);
                            //on redonne la main au joueur precedent
                            joueur = (joueur == noir) ? blanc : noir;
                            //aucune case cliquee
                            selection = false;
                        }

                        //bouton annuler : relaché
                        appuiAnnuler = false;
                    }

                    //on avais cliqué sur abandon
                    else if(appuiAbandon)
                    {
                        //c'est l'autre joueur qui gagne
                        joueur = (joueur == blanc) ? noir : blanc;
                        //bouton abandon : relaché
                        appuiAbandon = false;
                        //fin de la partie
                        continuer = false;
                        //la cause etant l'abandon
                        sprintf(cause, "abandon");
                    }
                }
            }

            //appui sur un bouton de la souris
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                //clic gauche
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //on appuie sur le bouton annuler
                    if(event.button.x > BOUTON_ANNULER_X && event.button.x < BOUTON_ANNULER_X+TAILLE_BOUTON_X && event.button.y > BOUTON_ANNULER_Y && event.button.y < BOUTON_ANNULER_Y+TAILLE_BOUTON_Y)
                        appuiAnnuler = true;

                    //on appuie sur le bouton abandon
                    else if(event.button.x > BOUTON_ABANDON_X && event.button.x < BOUTON_ABANDON_X+TAILLE_BOUTON_X && event.button.y > BOUTON_ABANDON_Y && event.button.y < BOUTON_ABANDON_Y+TAILLE_BOUTON_Y)
                        appuiAbandon = true;

                    //on clique dans la zone de jeu
                    else if(event.button.x > ZDJ_X && event.button.x < ZDJ_X+ZDJ && event.button.y > ZDJ_Y && event.button.y < ZDJ_Y+ZDJ)
                    {
                        //si on avais dejà selectionné quelque chose
                        if(selection)
                        {
                            //si le deplacement est autorise
                            if(possible(plateau, cliqueeX, cliqueeY, ((event.button.x-ZDJ_X)/TAILLE_CASES), ((event.button.y-ZDJ_Y)/TAILLE_CASES)))
                            {
                                //(se deplacer), et si quelque chose a été mangé
                                if(manger(plateau, cliqueeX, cliqueeY, ((event.button.x-ZDJ_X)/TAILLE_CASES), ((event.button.y-ZDJ_Y)/TAILLE_CASES)))
                                {
                                    //le pion est toujours selectionnable, mais à son nouvel
                                    //emplacement
                                    cliqueeX = ((event.button.x-ZDJ_X)/TAILLE_CASES);
                                    cliqueeY = ((event.button.y-ZDJ_Y)/TAILLE_CASES);

                                    //a t-on gagne par victoire totale ?
                                    //si oui, on sort de la partie
                                    continuer = !gagner(plateau, joueur);

                                    if(!continuer)
                                         sprintf(cause, "victoire totale");

                                    //sinon, si on ne peut plus manger --> au joueur suivant !
                                    else if(!peutManger(plateau, cliqueeX, cliqueeY))
                                    {
                                        //on deselectionne le pion
                                        selection = false;
                                        //on change de joueur
                                        joueur = (joueur == blanc) ? noir : blanc;
                                        //on peut revenir en arriere
                                        peutAnnuler = true;

                                        //sommes-nous bloqué ?
                                        continuer = !bloque(plateau, joueur);
                                        if(!continuer)
                                        {
                                            sprintf(cause, "blocage");

                                            //si oui, c'est l'autre joueur,
                                            //celui qui vient de jouer, qui gagne !
                                            joueur = (joueur == blanc) ? noir : blanc;
                                        }

                                    }
                                }

                                //rien a été mangé : au joueur suivant !
                                else
                                {
                                    //on deselectionne le pion
                                    selection = false;
                                    //on change de joueur
                                    joueur = (joueur == blanc) ? noir : blanc;
                                    //on peut revenir en arriere
                                    peutAnnuler = true;
                                }
                            }
                        }

                        //sinon, si on clique sur un pion
                        else if(plateau[(event.button.x-ZDJ_X)/TAILLE_CASES][(event.button.y-ZDJ_Y)/TAILLE_CASES].pion != non)
                        {
                            //coordonnees du pion en question
                            cliqueeY = (event.button.y-ZDJ_Y)/TAILLE_CASES;
                            cliqueeX = (event.button.x-ZDJ_X)/TAILLE_CASES;

                            //si on peut selectionner ce pion
                            if(plateau[cliqueeX][cliqueeY].joueur == joueur && selectionnable(plateau, cliqueeX, cliqueeY))
                            {
                                //le prochain plateau precedent est le plateau actuel
                                egalPlateau(precedent, plateau);
                                //le pion est selectionné
                                selection = true;
                                //on ne peut plus annuler (il faudra d'abord cliquer sur une case)
                                peutAnnuler = false;
                            }
                        }
                    }
                }
            }
        //Gestion terminée

        //repeter ceci tant que la partie nest pas terminée, et que l'on ne veut pas s'en aller
        }while (continuer && !fin);

        //securité...
        appuiAbandon = false;
        appuiAnnuler = false;

        //considerons ici abandon <-> quitter et annuler <-> recommencer

        //tant que l'on a pas choisi...
        while(!continuer && !fin)
        {
            //on affiche le message de fin
            affichageFin(plateau, ecran, appuiAnnuler, appuiAbandon, joueur, cause);

            //si il y a un evenement
            SDL_WaitEvent(&event);

            //de type : "boutton de sourie relaché"
            if(event.type == SDL_MOUSEBUTTONUP)
            {
                //si le bouton en question est le gauche
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //si on avais appuyé sur recommencer
                    if(appuiAnnuler)
                    {
                        //on relache le bouton
                        appuiAnnuler = false;
                        //on recommence une partie
                        continuer = true;
                    }
                    //sinon si on avais appuyé sur arreter
                    else if(appuiAbandon)
                    {
                        //on relache le bouton
                        appuiAbandon = false;
                        //on s'en va
                        fin = true;
                    }
                }
            }

            //de type : "appui sur une bouton de la souris"
            else if(event.type == SDL_MOUSEBUTTONDOWN)
            {
                //clic gauche
                if(event.button.button == SDL_BUTTON_LEFT)
                {
                    //sur le bouton "oui" (recommencer (annuler))
                    if(event.button.x > FIN_BOUTON_OUI_X && event.button.x < FIN_BOUTON_OUI_X+TAILLE_BOUTON_X && event.button.y > FIN_BOUTON_OUI_Y && event.button.y < FIN_BOUTON_OUI_Y+TAILLE_BOUTON_Y)
                        appuiAnnuler = true;

                    //sur le bouton "non" (arreter (abandon))
                    else if(event.button.x > FIN_BOUTON_NON_X && event.button.x < FIN_BOUTON_NON_X+TAILLE_BOUTON_X && event.button.y > FIN_BOUTON_NON_Y && event.button.y < FIN_BOUTON_NON_Y+TAILLE_BOUTON_Y)
                        appuiAbandon = true;
                }
            }

            //de type : "appui sur la croix de la fenetre"
            else if(event.type == SDL_QUIT)
                fin = true;

            //de type : "appui sur une touche"
            else if(event.type == SDL_KEYDOWN)
            {
                //la touche Echap
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    fin = true;
            }
        }

    //tant que l'on ne veut pas s'en aller
    }while(!fin);

    //arret de la SDL et de la biblio des polices
    TTF_Quit();
    SDL_Quit();

    //tout s'est bien passé
    return EXIT_SUCCESS;
}

