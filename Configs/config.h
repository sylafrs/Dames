/*
        Configuration : Fond vert
*/

#ifndef CONFIG_H_
#define CONFIG_H_

    /*Modes*/
    #define FULLSCREEN      //declenche le plein ecran

    /*Ecran/fenetre*/
    #define ECRAN_X         1024//640     //la longueur de la fenetre en pixel
    #define ECRAN_Y         600//480      //la hauteur de la fenetre en pixel
    #define NB_COULEURS     32            //nb de couleurs
    #define TITRE_FENETRE   "Jeu de Dames"//titre de la fenetre
    #define TITRE_X (ECRAN_X-texte->w)/2  //et.. ..dans la fenetre (abscisse en pixels)
    #define TITRE_Y (ZDJ_Y-texte->h)/2    //ordonnee en pixels
    //#define IMAGE_EN_FOND               //fond d'ecran : image ou pas ? (ou couleur ?)
    #define FOND_ECRAN      VERT          //fond d'ecran


    /*Police*/
    #define POLICE          "Polices\\anime.ttf"    //La police utilisee
    #define TAILLE_POLICE   20               //La taille de la police (faites un test sous word !)
    #define COULEUR_POLICE  {0, 0, 0}               //La couleur de la police

    /*Zone de jeu*/
    #define ZDJ_X           ZDJ_Y              //l'endroit (abscisse) ou commence la ZDJ
    #define ZDJ_Y           ((ECRAN_Y-ZDJ)/2)  //l'endroit (ordonnee) ou commence la ZDJ
    #define TAILLE_PLATEAU  10                 //la longueur et la largeur du plateau en cases
    #define TAILLE_CASES    46                 //la longueur et la largeur d'une case en pixel
    #define LIGNE_DEPART    4                  //la ligne de front des joueurs au depart (numero)

    /*Pions*/
    #define REINE_NOIRE     "Images\\reine_noire.bmp"        //Image de la reine noire
    #define REINE_BLANCHE   "Images\\reine_blanche.bmp"      //Image de la reine blanche
    #define PION_NOIR       "Images\\pion_noir.bmp"          //Image du pion noir
    #define PION_BLANC      "Images\\pion_blanc.bmp"         //Image du pion blanc
    #define FOND_IMG        ROUGE                            //Couleur des images a enlever (le fond)

    /*Cadre*/
    #define TAILLE_CADRE_X  450                         //largeur d'un Cadre de stats (en pixels)
    #define TAILLE_CADRE_Y  TAILLE_CASES*LIGNE_DEPART   //hauteur d'un Cadre de stats (en pixels)
    #define CADRE_A_X       ZDJ+ZDJ_X+10                //Cadre des noirs (abscisse en pixels)
    #define CADRE_A_Y       ZDJ_Y                       //Cadre des noirs (ordonnee en pixels)
    #define CADRE_B_X       CADRE_A_X                   //Cadre des blancs (abscisse en pixels)
    #define CADRE_B_Y       ECRAN_Y-ZDJ_Y-TAILLE_CADRE_Y//Cadre des blancs (ordonnee en pixels)
    #define EPAISSEUR_CADRE 5                           //Epaisseur du cadre (en pixels)

    /*Cadre - Interieur*/
    //#define FOND_CADRE_IMG_NORMALE "uneimage.bmp"          //pour que le fond soit une image
    //#define FOND_CADRE_IMG_FOND                            //si fond cadre = fond ecran (image)
    //#define TAUX_TRANSPARENCE   128                          //taux de transparence entre 0 et 255
    //#define COULEUR_FOND_CADRE    VERT                     //pour que le fond soit une couleur
    #define COULEUR_FOND_CADRE    FOND_ECRAN                 //pour que le fond soit une couleur
    #define NOM_CADRE_X     10                               //Titre du cadre  (abcisse, en pixels)
    #define NOM_CADRE_Y     10                               //Titre du cadre  (ordonee, en pixels)
    #define SCORE_RESTE_CADRE_X   (TAILLE_CADRE_X-texte->w)/2//Nombre de pions (abcisse, en pixels)
    #define SCORE_RESTE_CADRE_Y   50                         //Nombre de pions (ordonee, en pixels)
    #define SCORE_REINE_CADRE_X   (TAILLE_CADRE_X-texte->w)/2//Nombre de dames (abcisse, en pixels)
    #define SCORE_REINE_CADRE_Y   80                         //Nombre de dames (ordonee, en pixels)

    /*Boutons*/
    #define POLICE_BOUTTON          "Polices\\anime.ttf"        //Police du bouton
    #define TAILLE_POLICE_BOUTTON   10                          //Taille de la police du bouton
    #define COULEUR_POLICE_BOUTTON  {0, 0, 0}                   //Couleur de la police du bouton
    #define TAILLE_BOUTON_X         100                         //largeur d'un bouton
    #define TAILLE_BOUTON_Y         30                          //hauteur d'un bouton
    #define IMG_BOUTON              "Images\\boutton.bmp"       //Image du bouton
    #define IMG_BOUTON_APPUYE       "Images\\bouttonAppuye.bmp" //Image du bouton appuye

    /*Boutons crees*/
    #define BOUTON_ANNULER_X   CADRE_A_X+TAILLE_CADRE_X/2-TAILLE_BOUTON_X-5 //abscisse de 'Annuler'
    #define BOUTON_ANNULER_Y   CADRE_A_Y+TAILLE_CADRE_Y+30                  //ordonee (en pixel)
    #define BOUTON_ABANDON_X   BOUTON_ANNULER_X+TAILLE_BOUTON_X+10          //abscisse de 'Abandon'
    #define BOUTON_ABANDON_Y   BOUTON_ANNULER_Y                             //ordonee (en pixel)

    /*Message de fin*/
    #define FIN_VAINQUEUR_X        TITRE_X                    //message du vainqueur (abscisse, en px)
    #define FIN_VAINQUEUR_Y        TITRE_Y+100                // (ordonnee en pixel)
    #define FIN_STATS_X            TITRE_X                    //stats du vainqueur (abscisse en pixel)
    #define FIN_STATS_Y            FIN_VAINQUEUR_Y+30         // (ordonnee en pixel)
    #define FIN_MESS_REVANCHE_X    TITRE_X                    //position du message de revanche (absci)
    #define FIN_MESS_REVANCHE_Y    (ECRAN_Y-texte->h)/2       // (ordonnee en pixel)
    #define FIN_BOUTON_OUI_X       (ECRAN_X-TAILLE_BOUTON_X)/2//position du boutton 'oui' (abscisse)
    #define FIN_BOUTON_OUI_Y       (ECRAN_Y+50)/2             // (ordonnee en pixel)
    #define FIN_BOUTON_NON_X       FIN_BOUTON_OUI_X           //position du boutton 'non' (abscisse)
    #define FIN_BOUTON_NON_Y       FIN_BOUTON_OUI_Y+TAILLE_BOUTON_Y+5 // (ordonnee en pixel)

#endif//CONFIG_H_
