# PPN-DynMol
Dynamique Moléculaire

On invite l'utilisateur à lancer l'exécution de "launch.sh" par la commande "./launch.sh"qui implique entre autres :
- Génération des fichiers .xyz de positions initiales.
- Suppression automatique des fichiers intermédiaires (.xyz, .cfg, .o)
- Création d'un répertoire Simulation/ dans lequel sont stockés les fichiers .xyz représentant les positions des particules avec 1 fichier par itération de temps.

Nettoyage :
- $ make clean_Simulation : Suppression du répertoire Simulation/ dans lequel sont stockés les fichiers .xyz.

- ($ make clean_o : Suppression des .o (automatique))
- ($ make clean : Suppression des .o et du répertoire Simulation/ dans lequel sont stockés les fichiers .xyz)
