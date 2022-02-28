---
title: "Réseaux  -  Compte-rendu TP4"
author: "Thomas Renaud & Jonathan Delhomme"
geometry: margin=2cm
output: pdf_document
---

# Cahier des charges du Mastermind.

## Choix du serveur et des protocoles
L'objectif est de créer un serveur itératif, de le lancer, puis de se faire connecter un client. La communication sera faite grâce aux protocoles TCP/IP. Le choix dans un premier temps d'un serveur itératif se justifie par le fait qu'il n'y aura que peu de connections simultanées au serveur. 

## Algorithme du serveur

L'algorithme du serveur est le suivant :
```
socket
bind
listen
faire
    accept
    shutdown
    read/write /*traitement*/
    close
tant que(vrai)
close
```

## Algorithme du client
L'algorithme du client est le suivant :

```
socket
connect
read/write /*traitement, boucle du jeu*/
close
```

# Algorithme du jeu

TODO: Gérer les erreurs à chaque étape.
TODO: Gérer l'enchainement de plusieurs parties
TODO: Vérifier les zombies