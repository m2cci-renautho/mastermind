/******************************************************************************/
/*			Application: ...					*/
/******************************************************************************/
/*									      */
/*			 programme  CLIENT				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs : ... 					*/
/*									      */
/******************************************************************************/

#include <stdio.h>
#include <curses.h> /* Primitives de gestion d'ecran */
#include <sys/signal.h>
#include <sys/wait.h>
#include <stdlib.h>

#include "fon.h"  /* primitives de la boite a outils */

#include "masterMind.h" /*Primitives du masterMind*/


#define SERVICE_DEFAUT "2111"
#define SERVEUR_DEFAUT "127.0.0.1"


int client_appli(char *serveur, char *service);
void sendProposition(int socket, Combinaison proposition);
void readResultat(int socket, Resultat *res);
void jouerUnMasterMind(int socket);

/*****************************************************************************/
/*--------------- programme client -----------------------*/

int main(int argc, char *argv[])
{

	char *serveur = SERVEUR_DEFAUT; /* serveur par defaut */
	char *service = SERVICE_DEFAUT; /* numero de service par defaut (no de port) */
	int socket;

	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
	{
	case 1: /* arguments par defaut */
		printf("serveur par defaut: %s\n", serveur);
		printf("service par defaut: %s\n", service);
		break;
	case 2: /* serveur renseigne  */
		serveur = argv[1];
		printf("service par defaut: %s\n", service);
		break;
	case 3: /* serveur, service renseignes */
		serveur = argv[1];
		service = argv[2];
		break;
	default:
		printf("Usage:client serveur(nom ou @IP)  service (nom ou port) \n");
		exit(1);
	}

	/* serveur est le nom (ou l'adresse IP) auquel le client va acceder */
	/* service le numero de port sur le serveur correspondant au  */
	/* service desire par le client */
	// TODO: Insérer ici le menu pour la partie
	socket = client_appli(serveur, service);
	jouerUnMasterMind(socket);
	h_close(socket);
}

/*****************************************************************************/
int client_appli(char *serveur, char *service)
{
	// socket()
	struct sockaddr_in *adresseServeur;

	int socket = h_socket(AF_INET, SOCK_STREAM);
	adr_socket(service, serveur, SOCK_STREAM, &adresseServeur);
	// connect()
	h_connect(socket, adresseServeur);
	return socket;
}

/*****************************************************************************/

void jouerUnMasterMind(int socket)
{
	
	// initialisation de la fenetre
	WINDOW *propositions, *reponses, *colors, *saisie = NULL;
	init_fenetres(&propositions, &reponses, &colors, &saisie);
	afficherToutesCouleurs(colors);
	wrefresh(reponses);
	wrefresh(propositions);
	// Début du jeu
	Combinaison proposition;
	
	Resultat res;
	res.score = 0;

	do
	{
		// Saisie de la proposition par le joueur
		saisirProposition(saisie, &proposition);
		// Affichage de la proposition
		afficherCombinaison(propositions, 2 * res.score + 1, proposition);
		//Envoi au serveur
		sendProposition(socket, proposition);
		// Réponse
		readResultat(socket, &res);
		//Affichage du résultat
		printResultat(reponses, 2 * res.score - 1, res);
	} while (res.score < NBCOUPS && res.bonnePosition != NBPIONS);
	endwin();
	if (res.score < NBCOUPS)
	{
		printf("Félicitations, vous avez gagné en %d coups !\n", (int) res.score);
	}
	else
	{
		printf("Dommage, vous avez perdu !\n");
	}
	delwin(propositions);
	delwin(reponses);
	delwin(colors);
	delwin(saisie);
	delscreen(NULL);	
}
void sendProposition(int socket, Combinaison proposition)
{
	h_writes(socket, proposition.str, NBPIONS);
}

void readResultat(int socket, Resultat *res)
{

	char buffer[3];
	h_reads(socket, buffer, 3);
	res->bonneCouleur = buffer[0];
	res->bonnePosition = buffer[1];
	res->score = buffer[2];

}
