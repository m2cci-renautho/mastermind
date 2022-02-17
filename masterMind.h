
#ifndef MASTER
#define MASTER

#define NBCOUPS 12
#define NBPIONS 4
#define NBCOULEURS 8
#define NBCOUPS_MAX 12

typedef struct
{

    char bonneCouleur;
    char bonnePosition;
    char score;

} Resultat;

typedef struct
{

    char str[NBPIONS + 1];
} Combinaison;


static const char ENSEMBLE_COULEURS[] = "YBRGOWVF";



void saisirCombinaison(Combinaison *comb);
void comparerCombinaisons(Combinaison comb, Combinaison proposition, Resultat *result);
int estCombinaisonValide(Combinaison comb);
int appartient(const char *chaine, char c);
void creerCombinaisonAleatoire(Combinaison *comb, int distinct);
void saisirCombinaison(Combinaison *comb);

void initColors();
int findColorPair(char c) ;
void afficherCombinaison(WINDOW *win, int noLigne, Combinaison combinaison);
void afficherToutesCouleurs(WINDOW *win);
void printResultat(WINDOW *win, int ligne, Resultat res);
void init_fenetres(WINDOW **propositions, WINDOW **reponses, WINDOW **colors, WINDOW **saisie);
void saisirProposition(WINDOW *win, Combinaison *prop);
#endif