#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

	#include "header.h"

	#define BLANC SDL_MapRGB(ecran->format, 255, 255, 255)
    #define NOIR  SDL_MapRGB(ecran->format, 000, 000, 000)
    #define BLEU  SDL_MapRGB(ecran->format, 000, 000, 255)
    #define VERT  SDL_MapRGB(ecran->format, 000, 150, 000)
    #define ROUGE SDL_MapRGB(ecran->format, 255, 000, 000)


	// Procedure afficher(plateau, ecran, selection, cliqueeX, cliqueeY, appuiAbandon, appuiAnnuler)
    // {affiche la zone de jeu (le plateau), le score, la selection et les endroit ou elle peut aller}
    // Parametres : (D) plateau                    : Plateau
    //              (R) ecran                      : Surface SDL
    //              (D) selection                  : booleen
    //              (D) cliqueeX, cliqueeY         : entiers
    //              (D) appuiAbandon, appuiAnnuler : booleens
    void afficher(Case (*const plateau)[TAILLE_PLATEAU], Joueur, SDL_Surface *ecran, bool selection, int cliqueeX, int cliqueeY, bool appuiAbandon, bool appuiAnnuler);

    // Procedure afficher(plateau, ecran, Appcontinuer, Appfin, gagnant, cause)
    // {affiche le message de fin de jeu, pour une eventuelle nouvelle partie}
    // Parametres : (D) plateau              : Plateau
    //              (R) ecran                : Surface SDL
    //              (D) Appcontinuer, Appfin : booleens
    //              (D) gagnant              : Joueur
    //              (D) cause                : chaine de caracteres
    void affichageFin(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran, bool Appcontinuer, bool Appfin, Joueur gagnant, const char cause[10]);

    // Procedure dessinerBouton(ecran, posX, posY, texte, appuye)
    // {dessine sur l'ecran un bouton (appuye ou non) aux coordonées (posX,posY) avec du texte}
    // Parametres : (R) ecran      : Surface SDL
    //              (D) posX, posY : entiers
    //              (D) texte      : chaine de caracteres
    //              (D) appuye     : booleen
    void dessinerBouton(SDL_Surface *ecran, int posX, int posY, char* texte, bool appuye);

    // Procedure dessinerPlateau(plateau, ecran, joueur, selection, cliqueeX, cliqueeY)
    // {dessine le plateau en y deposant les pions. la couleur de la case depend des paramètres}
    // Paramètres : (D) plateau            : Plateau
    //              (R) ecran              : Surface SDL
    //              (D) joueur             : Joueur
    //              (D) selection          : booleen
    //              (D) cliqueeX, cliqueeY : entiers
    void dessinerPlateau(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran, Joueur joueur, bool selection, int cliqueeX, int cliqueeY);

    // Procedure dessineCadre(joueur, ecran)
    // {dessine les deux cadres}
    // Paramètres : (D) joueur : Joueur
    //              (R) ecran  : Surface SDL
    void dessinerCadre(Joueur joueur, SDL_Surface *ecran);

    // Procedure dessinerScore(plateau, ecran)
    // dessine le score (dans le cadre)}
    // Paramètres : (D) plateau : Plateau
    //              (R) ecran   : Surface SDL
    void dessinerScore(Case (*const plateau)[TAILLE_PLATEAU], SDL_Surface *ecran);

#endif
