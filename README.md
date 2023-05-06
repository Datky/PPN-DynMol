# PPN-DynMol
Dynamique Moléculaire

On invite l'utilisateur à lancer l'exécution de "launch.sh" par la commande "./launch.sh" qui implique entre autres :
- Génération des fichiers .xyz de positions initiales.
- Suppression automatique des fichiers intermédiaires (.xyz, .cfg, .o)
- Création d'un répertoire Entree/ dans lequel sont stockés les fichiers .xyz représentant les positions initiales des particules.
- Création d'un répertoire Sortie/ dans lequel sont stockés les fichiers .xyz représentant les positions calculées des particules avec 1 fichier par itération de temps.

Nettoyage :
- $ make clean : Règle pour suppression de tous les fichiers créés.
- ($ make clean_o : Suppression des .o et exécutables (automatique))

