#include <stdlib.h>

#include "SDL_include/SDL.h"
#include "SDL_include/SDL_ttf.h"

#include "affichage.h"
#include "indicateurs.h"

//ICI : le tableau de cases n'est pas modifié !

void afficher(Case (*const plateau)[TAILLE_PLATEAU], Joueur joueur, SDL_Surface *ecran, bool selection, int cliqueeX, int cliqueeY, bool abandon, bool annuler)
{
    SDL_Surface *fond = NULL;
    SDL_Rect position;

    //fond d'ecran
    #ifdef IMAGE_EN_FOND
        //image
        fond = SDL_LoadBMP(FOND_ECRAN);
        position.x = 0;
        position.y = 0;
        SDL_BlitSurface(fond, NULL, ecran, &position);
    #else
        //couleur
        SDL_FillRect(ecran, NULL, FOND_ECRAN);
    #endif

    //dessine ce qu'il y a a dessiner
    dessinerPlateau(plateau, ecran, joueur, selection, cliqueeX, cliqueeY);
    dessinerCadre(joueur, ecran);
    dessinerScore(plateau, ecran);
    dessinerBouton(ecran, BOUTON_ANNULER_X, BOUTON_ANNULER_Y, "annuler", annuler);
    dessinerBouton(ecran, BOUTON_ABANDON_X, BOUTON_ABANDON_Y, "abandon", abandon);

    //upgrade
    SDL_Flip(ecran);
    SDL_FreeSurface(fond);
}

void dessinerBouton(SDL_Surface *ecran, int posX, int posY, char* texte, bool appuye)
{
    TTF_Font *police = TTF_OpenFont(POLICE_BOUTTON, TAILLE_POLICE_BOUTTON);
    SDL_Surface *boutton, *surfaceTexte;
    SDL_Color couleurpolice = COULEUR_POLICE_BOUTTON;
    SDL_Rect position;

    //l'image du bouton suivant si celui-ci est appuyé ou non
    if(!appuye)
        boutton = SDL_LoadBMP(IMG_BOUTON);
    else
        boutton = SDL_LoadBMP(IMG_BOUTON_APPUYE);

    //ou se situe le bouton ?
    position.x = posX;
    position.y = posY;
    SDL_BlitSurface(boutton, NULL, ecran, &position);

    //le texte dans le bouton
    //surfaceTexte : le texte affiché, texte : la chaine de caracteres.

    surfaceTexte = TTF_RenderText_Blended(police, texte, couleurpolice);

    //l'endroit ou se trouve le texte sur l'ecran
    position.x = posX+(TAILLE_BOUTON_X-surfaceTexte->w)/2;
    position.y = posY+(TAILLE_BOUTON_Y-surfaceTexte->h)/2;
    SDL_BlitSurface(surfaceTexte, NULL, ecran, &position);

    //libere la mémoire
    SDL_FreeSurface(boutton);
    SDL_FreeSurface(surfaceTexte);
    TTF_CloseFont(police);
}

void dessinerPlateau(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran, Joueur joueur, bool selection, int cliqueeX, int cliqueeY)
{
    int i, j;
    SDL_Rect position;

    //Les cases et les pions..
    SDL_Surface *uneCase      = NULL,
                *reineNoire   = SDL_LoadBMP(REINE_NOIRE  ),
                *reineBlanche = SDL_LoadBMP(REINE_BLANCHE),
                *pionNoir     = SDL_LoadBMP(PION_NOIR    ),
                *pionBlanc    = SDL_LoadBMP(PION_BLANC   );

    //..sans leur couleur de fond
    SDL_SetColorKey(  reineNoire, SDL_SRCCOLORKEY, FOND_IMG);
    SDL_SetColorKey(reineBlanche, SDL_SRCCOLORKEY, FOND_IMG);
    SDL_SetColorKey(    pionNoir, SDL_SRCCOLORKEY, FOND_IMG);
    SDL_SetColorKey(   pionBlanc, SDL_SRCCOLORKEY, FOND_IMG);

    uneCase = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CASES, TAILLE_CASES, NB_COULEURS, 0, 0, 0, 0);

    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            //la position de la case
            position.x = (i*TAILLE_CASES)+ZDJ_X;
            position.y = (j*TAILLE_CASES)+ZDJ_Y;

            //Fond de la case
            //si il s'agit d'une case cliquee
            if(selection && (cliqueeX == i) && (cliqueeY == j))
                SDL_FillRect(uneCase, NULL, VERT);

            //si il s'agit d'une case sur laquelle peut aller le pion
            else if(selection && possible(plateau, cliqueeX, cliqueeY, i, j))
                SDL_FillRect(uneCase, NULL, ROUGE);

            //ici : les pions selectionnables.
            else if(!selection && plateau[i][j].joueur == joueur && selectionnable(plateau,i,j))
                SDL_FillRect(uneCase, NULL, BLEU);

            //sinon le fond est blanc s'il s'agit d'une case 'paire'
            else if((i+j)%2 == 0)
                SDL_FillRect(uneCase, NULL, BLANC);

            //sinon : noir
            else
                SDL_FillRect(uneCase, NULL, NOIR);

            //on "colle" la case à l'ecran
            SDL_BlitSurface(uneCase, NULL, ecran, &position);

            //colle par dessus le pion de la case (s'il y en a)
            if(plateau[i][j].pion == reine)
            {
                if(plateau[i][j].joueur == blanc)
                        SDL_BlitSurface(reineBlanche, NULL, ecran, &position);
                else
                    SDL_BlitSurface(reineNoire, NULL, ecran, &position);
            }
            else if(plateau[i][j].pion == normal)
            {
                if(plateau[i][j].joueur == blanc)
                        SDL_BlitSurface(pionBlanc, NULL, ecran, &position);
                else
                    SDL_BlitSurface(pionNoir, NULL, ecran, &position);
            }
        }
    }

    //libere la memoire allouee
    SDL_FreeSurface(uneCase);
    SDL_FreeSurface(pionBlanc);
    SDL_FreeSurface(pionNoir);
    SDL_FreeSurface(reineBlanche);
    SDL_FreeSurface(reineNoire);
}

void dessinerCadre(Joueur joueur, SDL_Surface *ecran)
{
    //cadre
    SDL_Surface *cadre = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CADRE_X, TAILLE_CADRE_Y, NB_COULEURS, 0, 0, 0, 0);
    SDL_Surface *intcadre = NULL;

    //positions
    SDL_Rect position;
    #ifdef FOND_CADRE_IMG_FOND
        SDL_Rect interieur;
    #endif

    //cadre (bordure) du joueur noir
    if(joueur == noir)
        SDL_FillRect(cadre, NULL, ROUGE);
    else
        SDL_FillRect(cadre, NULL, NOIR);
    position.x = CADRE_A_X;
    position.y = CADRE_A_Y;
    SDL_BlitSurface(cadre, NULL, ecran, &position);

    //cadre (bordure) du joueur blanc
    if(joueur == blanc)
       SDL_FillRect(cadre, NULL, ROUGE);
    else
       SDL_FillRect(cadre, NULL, NOIR);
    position.x = CADRE_B_X;
    position.y = CADRE_B_Y;
    SDL_BlitSurface(cadre, NULL, ecran, &position);

    //interieur du cadre (position)
    position.x = CADRE_A_X+EPAISSEUR_CADRE;
    position.y = CADRE_A_Y+EPAISSEUR_CADRE;

    #ifdef FOND_CADRE_IMG_NORMALE
        //si le fond est une image (normale)
        intcadre = SDL_LoadBMP(FOND_CADRE_IMG_NORMALE);
        #ifdef FOND_TRANSPARENT
            //de la transparence ?
            SDL_SetAlpha(intcadre, SDL_SRCALPHA, FOND_TRANSPARENT);
        #endif
        //on colle le tout!
        SDL_BlitSurface(intcadre, NULL, ecran, &position);
     #else
        //sinon
        #ifdef FOND_CADRE_IMG_FOND
            //si c'est l'image mise en fond d'ecran
            intcadre = SDL_LoadBMP(FOND_ECRAN);

            //on prend la partie qui devrait se situer dans le cadre
            interieur.x = position.x;
            interieur.y = position.y;
            interieur.w = TAILLE_CADRE_X-2*EPAISSEUR_CADRE;
            interieur.h = TAILLE_CADRE_Y-2*EPAISSEUR_CADRE;
            #ifdef FOND_TRANSPARENT
                //de la transparence ?
                SDL_SetAlpha(intcadre, SDL_SRCALPHA, FOND_TRANSPARENT);
            #endif
            //on colle le tout
            SDL_BlitSurface(intcadre, &interieur, ecran, &position);
        #else
             //sinon : c'est une couleur unie
             intcadre = SDL_CreateRGBSurface(SDL_HWSURFACE, TAILLE_CADRE_X-EPAISSEUR_CADRE*2, TAILLE_CADRE_Y-EPAISSEUR_CADRE*2, NB_COULEURS, 0, 0, 0, 0);
             SDL_FillRect(intcadre, NULL, COULEUR_FOND_CADRE);

             SDL_BlitSurface(intcadre, NULL, ecran, &position);
        #endif
    #endif

     //position de l'interieur de l'autre cadre
     position.x = CADRE_B_X+EPAISSEUR_CADRE;
     position.y = CADRE_B_Y+EPAISSEUR_CADRE;

    #ifdef FOND_CADRE_IMG_FOND
        //la taille du cadre etant la meme, seule la position de l'endroit de l'image
        //a prendre est changee
        interieur.x = position.x;
        interieur.y = position.y;

        //on colle le tout
        SDL_BlitSurface(intcadre, &interieur, ecran, &position);
    #else
        // cas de l'image ou du fond uni : rien a changer, juste coller
        SDL_BlitSurface(intcadre, NULL, ecran, &position);
    #endif

    //on libere les surfaces utilisees
    SDL_FreeSurface(cadre);
    SDL_FreeSurface(intcadre);
}

void dessinerScore(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran)
{
    //compteurs
    int i, j;
    int resteNoir = 0, resteBlanc = 0;
    int nbReineNoir = 0, nbReineBlanc = 0;

    //chaine
    char chaine[100];

    //texte, police, couleur et position
    SDL_Surface *texte;
    TTF_Font *police = TTF_OpenFont(POLICE, TAILLE_POLICE);
    SDL_Color couleurpolice = COULEUR_POLICE;
    SDL_Rect position;

    //compte le nombre de reines et de pions
    for(i = 0; i < TAILLE_PLATEAU; i++)
    {
        for(j = 0; j < TAILLE_PLATEAU; j++)
        {
            if(plateau[i][j].pion != non)
            {
                if(plateau[i][j].joueur == noir)
                {
                    resteNoir++;
                    if(plateau[i][j].pion == reine)
                        nbReineNoir++;
                }

                else
                {
                    resteBlanc++;
                    if(plateau[i][j].pion == reine)
                        nbReineBlanc++;
                }
            }
        }
    }

    //titre de la fenetre (rien à voir, mais bon)
    texte = TTF_RenderText_Blended(police, TITRE_FENETRE , couleurpolice);
    position.x = TITRE_X;
    position.y = TITRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Les Noirs"
    texte = TTF_RenderText_Blended(police, "Les Noirs" , couleurpolice);
    position.x = CADRE_A_X+NOM_CADRE_X;
    position.y = CADRE_A_Y+NOM_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Pièces restantes : XX" (pour les noirs)
    sprintf(chaine, "Pièces restantes : %d", resteNoir);
    texte = TTF_RenderText_Blended(police, chaine, couleurpolice);
    position.x = CADRE_A_X+SCORE_RESTE_CADRE_X;
    position.y = CADRE_A_Y+SCORE_RESTE_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Nombre de dames : XX" (pour les noirs)
    sprintf(chaine, "Nombre de dames : %d", nbReineNoir);
    texte = TTF_RenderText_Blended(police, chaine, couleurpolice);
    position.x = CADRE_A_X+SCORE_REINE_CADRE_X;
    position.y = CADRE_A_Y+SCORE_REINE_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Les Blancs"
    texte = TTF_RenderText_Blended(police, "Les Blancs" , couleurpolice);
    position.x = CADRE_B_X+NOM_CADRE_X;
    position.y = CADRE_B_Y+NOM_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Pieces restantes : XX" (pour les blancs)
    sprintf(chaine, "Pièces restantes : %d", resteBlanc);
    texte = TTF_RenderText_Blended(police, chaine, couleurpolice);
    position.x = CADRE_B_X+SCORE_RESTE_CADRE_X;
    position.y = CADRE_B_Y+SCORE_RESTE_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Nombre de dames : XX" (pour les blancs)
    sprintf(chaine, "Nombre de dames : %d", nbReineBlanc);
    texte = TTF_RenderText_Blended(police, chaine, couleurpolice);
    position.x = CADRE_B_X+SCORE_REINE_CADRE_X;
    position.y = CADRE_B_Y+SCORE_REINE_CADRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //on libère la mémoire
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
}

void affichageFin(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran, bool Appcontinuer, bool Appfin, Joueur gagnant, const char cause[10])
{
    TTF_Font *police = TTF_OpenFont(POLICE, TAILLE_POLICE);
    SDL_Surface *fond = NULL;
    SDL_Surface *texte = NULL;
    SDL_Color couleurpolice = COULEUR_POLICE;
    SDL_Rect position;
    char chaine[100];
    int i,j,nb,dames;

    //fond d'ecran
    #ifdef IMAGE_EN_FOND
        //image
        fond = SDL_LoadBMP(FOND_ECRAN);
        position.x = 0;
        position.y = 0;
        SDL_BlitSurface(fond, NULL, ecran, &position);
    #else
        //couleur
        SDL_FillRect(ecran, NULL, FOND_ECRAN);
    #endif

    //affiche le titre de la fenetre
    texte = TTF_RenderText_Blended(police, TITRE_FENETRE , couleurpolice);
    position.x = TITRE_X;
    position.y = TITRE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //qui a gagné ?
    if(gagnant == blanc)
        sprintf(chaine, "Le joueur blanc a gagné par %s,", cause);
    else
        sprintf(chaine, "Le joueur noir a gagné par %s,", cause);

    texte = TTF_RenderText_Blended(police, chaine, couleurpolice);
    position.x = FIN_VAINQUEUR_X;
    position.y = FIN_VAINQUEUR_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //compte le score du vainqueur
    nb = 0;
    dames = 0;
    for(i=0; i < TAILLE_PLATEAU; i++)
    {
        for(j=0; j < TAILLE_PLATEAU; j++)
        {
            if(plateau[i][j].pion != non && plateau[i][j].joueur == gagnant)
            {
                nb++;
                if(plateau[i][j].pion == reine)
                    dames++;
            }
        }
    }

    //l'affiche
    sprintf(chaine, "il lui restait %d pièces dont %d dames.", nb, dames);
    texte = TTF_RenderText_Blended(police, chaine , couleurpolice);
    position.x = FIN_STATS_X;
    position.y = FIN_STATS_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //"Une petite revanche ?"
    texte = TTF_RenderText_Blended(police, "Une petite revanche ?" , couleurpolice);
    position.x = FIN_MESS_REVANCHE_X;
    position.y = FIN_MESS_REVANCHE_Y;
    SDL_BlitSurface(texte, NULL, ecran, &position);

    //bouton "oui"
    dessinerBouton(ecran, FIN_BOUTON_OUI_X, FIN_BOUTON_OUI_Y, "oui", Appcontinuer);
    //bouton "non"
    dessinerBouton(ecran, FIN_BOUTON_NON_X, FIN_BOUTON_NON_Y, "non", Appfin);

    //mise à jour de l'ecran
    SDL_Flip(ecran);

    //libere l'espace memoire
    SDL_FreeSurface(texte);
    TTF_CloseFont(police);
}
