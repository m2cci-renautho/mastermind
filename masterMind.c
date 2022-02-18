
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <regex.h>
#include <ncurses.h>

#include "masterMind.h"

/**
 * @brief Crée dans @param comb une combinaison aléatoire.
 *
 * @param comb La combinaison créée
 * @param distinct Booléen : si vrai, les couleurs choisies seront toutes distinctes.
 */
void creerCombinaisonAleatoire(Combinaison *comb, int distinct)
{

    srand(time(NULL));
    int id_couleur;
    int dejaVus[NBCOULEURS] = {0};

    for (int position = 0; position < NBPIONS; position++)
    {
        do
        {
            id_couleur = rand() % NBCOULEURS;
        } while (distinct && dejaVus[id_couleur] == 1);

        dejaVus[id_couleur] = 1;
        comb->str[position] = ENSEMBLE_COULEURS[id_couleur];
    }
    comb->str[NBPIONS] = '\0';
}
/**
 * @brief Renvoie vrai si et seulement si le caractère @param c est dans la chaine @param chaine;
 *
 * @param chaine La chaine de caractère
 * @param c Le caractère à tester
 * @return int
 */
int appartient(const char *chaine, char c)
{
        
    int i = 0;
    int longueur = strlen(chaine);
    while (i != longueur+1 && chaine[i++] != c)
        ;
    return (i != longueur+1);
}
/**
 * @brief Renvoie vrai si et seulement si la combinaison de couleur est valide, c'est à dire que les couleurs sont bien dans ENSEMBLE_COULEURS.
 *
 * @param comb La combinaison à tester.
 * @return True (1) ou False (0).
 */
int estCombinaisonValide(Combinaison comb)
{

    regex_t regex;
    int retour;
    // Création de la regex
    char regString[100];
    snprintf(regString, sizeof(regString), "[%s]{%d}", ENSEMBLE_COULEURS, NBPIONS);
    // Compilation
    retour = regcomp(&regex, regString, REG_EXTENDED);
    if (retour)
        printf("no compile\n");

    // On execute la ragaex
    retour = regexec(&regex, comb.str, 0, NULL, 0);

    if (!retour)
    {
        // S'il y a match
        regfree(&regex);
        return 1;
    }
    else if (retour == REG_NOMATCH)
    {
        // S'il n'y a pas match
        regfree(&regex);
        return 0;
    }
    else
    {
        // Si ça bug
        regerror(retour, &regex, comb.str, sizeof(comb.str));
        fprintf(stderr, "Regex match failed: %s\n", comb.str);
        exit(1);
    }
}
/**
 * @brief Ecrit dans @param result le résultat de la comparaison entre une combinaison et une proposition, c'est-à-dire donne le nombre de bâtons blancs (bonnes couleurs mal placées) et de bâtons rouges (bonnes couleurs bien placées)
 * 
 * @param comb La combinaison de référence
 * @param proposition La combinaison proposée à comparer.
 * @param result La structure Résultat qui va contenir le résultat de la comparaison.
 * 
 * Pré-condition : Les deux combinaisons sont valides.
 */
void comparerCombinaisons(Combinaison comb, Combinaison proposition, Resultat *result)
{

    result->bonnePosition = 0;
    result->bonneCouleur = 0;
    char dejaVu[NBPIONS] = {0};
   
    for (int i = 0; i < NBPIONS; i++)
    {
        if (comb.str[i] == proposition.str[i]){

            result->bonnePosition++;
            dejaVu[i] = comb.str[i];
        }
        else
        {
            if(appartient(comb.str, proposition.str[i]) && !appartient(dejaVu, proposition.str[i])){
                dejaVu[i] = proposition.str[i];
                result->bonneCouleur++;
            }   
        }
    }
}
/**
 * @brief Initialise les couleurs (pour Ncurse) associées aux couleurs de ENSEMBLE_COULEURS. 
 * Remarque : Les couleurs ont étés fixéees "à la main" en affichant à l'écran toutes les couleurs et leur indice afin de regarder quelles étaient les plus pertinentes.
 * 
 */
void initColors()
{
    start_color();

    for (int i = 0; i < NBCOULEURS; i++)
    {
        switch (ENSEMBLE_COULEURS[i])
        {
        case 'Y':
            init_pair(i + 1, COLOR_BLACK, 226);
            break;
        case 'B':
            init_pair(i + 1, 15, 21);
            break;
        case 'R':
            init_pair(i + 1, COLOR_BLACK, 196);
            break;
        case 'G':
            init_pair(i + 1, COLOR_BLACK, 40);
            break;
        case 'O':
            init_pair(i + 1, COLOR_BLACK, 208);
            break;
        case 'W':
            init_pair(i + 1, COLOR_BLACK, 15);
            break;
        case 'V':
            init_pair(i + 1, 15, 54);
            break;
        case 'F':
            init_pair(i + 1, 15, 201);
            break;
        }
    }
}
/**
 * @brief Initialise les différentes fenêtres ncurses pour l'interface du jeu.
 * 
 * @param propositions Le plateau de jeu où apparaissent les propositions. 
 * @param reponses La zone où apparaissent les réponses et les résultats des comparaisons.  
 * @param colors Un rappel de toutes les couleurs disponibles et les caractères à saisir.
 * @param saisie La zone de saisie des couleurs.
 */
void init_fenetres(WINDOW **propositions, WINDOW **reponses, WINDOW **colors, WINDOW **saisie)
{
    initscr();
    initColors();

    int col = 0;
    int ligne = 0;

    *propositions = subwin(stdscr, 2 * NBCOUPS_MAX + 1, 2 * NBPIONS + 3, col, ligne);
    ligne += 2 * NBPIONS + 3;
    *reponses = subwin(stdscr, 2 * NBCOUPS_MAX + 1, NBPIONS + 2, 0, ligne);
    col += 2 * NBCOUPS_MAX + 1;
    *colors = subwin(stdscr, 3, 2 * NBCOULEURS + 1, col, 0);
    col += 3;
    *saisie = subwin(stdscr, 3, 17, col, 0);

    box(*propositions, ACS_VLINE, ACS_HLINE);
    box(*reponses, ACS_VLINE, ACS_HLINE);
    box(*colors, ACS_VLINE, ACS_HLINE);
    box(*saisie, ACS_VLINE, ACS_HLINE);
}
/**
 * @brief Affiche la combinaison à l'écran.
 * 
 * @param win Fenetre ncurses dans laquelle l'afficher.
 * @param noLigne Numéro de la ligne de la fenêtre.
 * @param comb La combinaison à afficher.
 */
void afficherCombinaison(WINDOW *win, int noLigne, Combinaison comb)
{

    int pair;
    for (int i = 0; i < NBPIONS; i++)
    {
        pair = findColorPair(comb.str[i]);
        wattron(win, COLOR_PAIR(pair));
        mvwaddch(win, noLigne, 2 * i + 2, comb.str[i]);
        wattroff(win, COLOR_PAIR(pair));
    }
    wrefresh(win);
}
/**
 * @brief Permet d'entrer au clavier une proposition de combinaison.
 * Ecrit dans @param prop la proposition saisie.
 * 
 * @param win La fenêtre de saisie.
 * @param prop La proposition saisie.
 */
void saisirProposition(WINDOW *win, Combinaison *prop)
{
    do
    {
        wclear(win);
        box(win, ACS_VLINE, ACS_HLINE);
        wrefresh(win);
        wmove(win, 1, 1);
        wgetnstr(win, prop->str, NBPIONS);
    } while (!estCombinaisonValide(*prop));
}
/**
 * @brief Permet de retrouver l'indice de la paire de couleur correspondant au caractère @param c.
 * 
 * @param c Le caractère correspondant à la couleur
 * @return L'indice de la paire.
 * 
 * Pré-condition : @param c appartient à ENSEMBLE_COULEURS.
 */
int findColorPair(char c)
{

    int i = 0;
    while (i != NBCOULEURS + 1 && c != ENSEMBLE_COULEURS[i++])
        ;

    return (i <= NBCOULEURS) ? (i) : -1;
}
/**
 * @brief Affiche toutes les couleurs disponibles pour le jeu.
 * 
 * @param win Fenêtre d'affichage des couleurs.
 */
void afficherToutesCouleurs(WINDOW *win)
{

    for (int i = 0; i < NBCOULEURS; i++)
    {
        wattron(win, COLOR_PAIR(i + 1));
        mvwaddch(win, 1, 2 * i + 1, ENSEMBLE_COULEURS[i]);
        wattroff(win, COLOR_PAIR(i + 1));
    }
    wrefresh(win);
}
/**
 * @brief Affiche un résultat à l'écran.
 * 
 * @param win Fenêtre d'affichage du résultat
 * @param ligne Ligne à laquelle afficher.
 * @param res Le résultat à afficher.
 */
void printResultat(WINDOW *win, int ligne, Resultat res)
{

    short blanc = NBCOULEURS + 1;
    short rouge = NBCOULEURS + 2;

    init_pair(rouge, 196, COLOR_BLACK);
    init_pair(blanc, 15, COLOR_BLACK);
    char col = 1;

    for (char i = 0; i < res.bonneCouleur; i++)
    {
        wattron(win, COLOR_PAIR(blanc));
        mvwaddch(win, ligne, col, '|');
        wattroff(win, COLOR_PAIR(blanc));
        col++;
    }
    for (char i = 0; i < res.bonnePosition; i++)
    {
        wattron(win, COLOR_PAIR(rouge));
        mvwaddch(win, ligne, col, '|');
        wattroff(win, COLOR_PAIR(rouge));
        col++;
    }
    wrefresh(win);
}