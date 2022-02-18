
#ifndef MASTER
#define MASTER

#define NBCOUPS 12          //Le nombre de coups maximum
#define NBPIONS 4           //Le nombre de pions à deviner
#define NBCOULEURS 8        //Le nombre de couleurs     

/**
 * Structure contenant un résultat.
 */
typedef struct
{

    char bonneCouleur;      //Nombre de pions ayant la bonne couleur et une mauvaise position
    char bonnePosition;     //Nombre de pions ayant la bonne couleur et la bonne position
    char score;             //Le score actuel : nombre d'essai depuis le début de la partie.

} Resultat;
/**
 * Structure contenant une cmbinaison de couleurs.
 */
typedef struct
{
    char str[NBPIONS + 1];  //Chaîne de caractères contenant la combinaison.
} Combinaison;

/**
 * Variable globale qui contient l'ensemble des couleurs disponibles.
 */
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