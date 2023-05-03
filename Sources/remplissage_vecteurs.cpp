/** ...
 * ...
 **/

#include <iostream>
#include <cstdio>
#include <random>
#include <mpi.h>
#include "../Headers/types.h"
#include "../Headers/constantes.h"
#include "../SoA/particule.h"
#include "../Headers/XYZ.h"
#include "../Headers/remplissage_vecteurs.h"

extern u32 N; // NEW
extern u32 nb_iteration; // NEW
extern u32 dt; // NEW




void remplissage_vecteurs(struct Vecteur_3D* pos, struct Vecteur_3D* vit, struct Vecteur_3D* acc) { // Remplit les vecteurs de données
    // Peut être optimisé avec une seule boucle au lieu de 3

    std::string str_N = std::__cxx11::to_string(N);
    lireXYZ("Entree/source"+str_N+".xyz", pos);
    std::cout << "\nBonne lecture du fichier des positions générées aléatoirement par le logiciel Atomsk.\n" << std::endl;

    generation_gaussienne_des_vitesses(vit);
    std::cout << "\nBonne affectation vitesses selon une fonction gaussienne.\n" << std::endl;

    accelerations_initiales_nulles(acc);
    std::cout << "\nBonne affectation des accélérations avec des valeurs nulles.\n" << std::endl;
}




int remplissage_vecteurs_para(int b_z, int rang, int P, std::vector<u64> &ids, struct Vecteur_Para &pos, struct Vecteur_Para &vit, struct Vecteur_Para &acc) {
    int n_local = generation_positions(b_z, rang, P, pos);
    int somme_prev = 0;
    int somme_prev_envoi = 0;

    // Nécessaire pour savoir quels ids donner aux particules locales
    MPI_Request req;
    if (rang == 0) {
        MPI_Isend(&n_local, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &req);
    } else {
        MPI_Status sta;
        MPI_Recv(&somme_prev, 1, MPI_INT, rang-1, 0, MPI_COMM_WORLD, &sta);
        if (rang < P-1) {
            somme_prev_envoi = somme_prev+n_local;
            MPI_Isend(&somme_prev_envoi, 1, MPI_INT, rang+1, 0, MPI_COMM_WORLD, &req);
        }
    }

    remplissage_ids(somme_prev, n_local, ids);
    generation_gaussienne_des_vitesses_para(n_local, vit);
    accelerations_initiales_nulles_para(n_local, acc);
    return n_local;
}

void remplissage_ids(int somme_prev, int n_local, std::vector<u64> &ids) {
    for (int i = somme_prev; i < somme_prev+n_local; ++i) {
        ids.push_back(i);
    }
}

int generation_positions(int b_z, int rang, int P, struct Vecteur_Para &pos) {
    std::string str_N = std::__cxx11::to_string(N);
    return lire_XYZ_Para(b_z, rang, P, "Entree/source"+str_N+".xyz", pos);
}

void generation_gaussienne_des_vitesses_para(int n_local, struct Vecteur_Para &vit) {


    std::random_device rd; // Génération d'une graine.
    std::mt19937 gen(rd()); // Génération par "gen" d'un entier non signé par l'algo de Mersenne en fonction de la graine.
    std::normal_distribution<> d_v_x{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_y{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_z{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.

    f64 qdm_systeme_x = 0 ;
    f64 qdm_systeme_y = 0 ;
    f64 qdm_systeme_z = 0 ;

    // Pour chaque appel de d_v_x(gen), d_v_y(gen) et d_v_z(gen) : transformation de l'entier généré par gen en un nouveau double aléatoire (gaussien) autour de 0.
    // Calcul de la composante selon x, y et z du vecteur quantité de mouvement total du système.
    for(int i = 0; i < n_local; i++) {
        vit.X.push_back(d_v_x(gen));
        vit.Y.push_back(d_v_y(gen));
        vit.Z.push_back(d_v_z(gen));
        qdm_systeme_x = qdm_systeme_x + m*(vit.X[i]);
        qdm_systeme_y = qdm_systeme_y + m*(vit.Y[i]);
        qdm_systeme_z = qdm_systeme_z + m*(vit.Z[i]);
    }


    /*
    std::cout << "Quantité de mouvement du système selon x avant ajustement: " << qdm_systeme_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y avant ajustement: " << qdm_systeme_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z avant ajustement: " << qdm_systeme_z << "A/s" << std::endl;
    */

// Ajustement de qdm_systeme à 0, ie m*vit->X[i] = m*vit->X[i] - qdm_systeme_x, ETC.
    f64 qdm_systeme_x_sur_N_sur_m = qdm_systeme_x/N/m;

    for(int i = 0; i < n_local; i++){
        vit.X[i] -= qdm_systeme_x_sur_N_sur_m;
        vit.Y[i] -= qdm_systeme_x_sur_N_sur_m;
        vit.Z[i] -= qdm_systeme_x_sur_N_sur_m;
//        std::cout << &vit->X[i] << ": vit_Gauss_après_qdm_syst_nulle_X = " <<vit->X[i] << std::endl;
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_qdm_syst_nulle_Y = " <<vit->Y[i] << std::endl;
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_qdm_syst_nulle_Z = " <<vit->Z[i] << std::endl;
    }

// Qdm totale après ajustement
    f64 qdm_systeme_après_x = 0 ;
    f64 qdm_systeme_après_y = 0 ;
    f64 qdm_systeme_après_z = 0 ;
    for(int i = 0; i < n_local; i++){
        qdm_systeme_après_x += m*(vit.X[i]);
        qdm_systeme_après_y += m*(vit.Y[i]);
        qdm_systeme_après_z += m*(vit.Z[i]);
    }

    /*
    std::cout << "Quantité de mouvement du système selon x après ajustement: " << qdm_systeme_après_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y après ajustement: " << qdm_systeme_après_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z après ajustement: " << qdm_systeme_après_z << "A/s" << std::endl;
    */

// Calcul de la température en fonction de la norme du vecteur vitesse.
    f64 temperature = 0;
    f64 inverse_trois_k_b_N_m = 1/(3*k_b*N)*m;
    
    for(int i = 0; i < n_local; i++) {
        temperature = temperature + inverse_trois_k_b_N_m*(pow(vit.X[i],2.0)+pow(vit.Y[i],2.0)+pow(vit.Z[i],2.0));
    }

// Calcul de la vitesse selon initialisation à temperature_cible;
    f64 sqrt_temperature_cible_sur_temperature = sqrt(temperature_cible/temperature);
    for(int i = 0; i < n_local; i++){
        vit.X[i] = (vit.X[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->X[i] << ": vit_Gauss_après_ajustement_Tcible_X = " <<vit->X[i] << std::endl;
        vit.Y[i] = (vit.Y[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_ajustement_Tcible_Y = " <<vit->Y[i] << std::endl;
        vit.Z[i] = (vit.Z[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_ajustement_Tcible_Z = " <<vit->Z[i] << std::endl;
    }
}

void accelerations_initiales_nulles_para(int n_local, struct Vecteur_Para &acc) {
    for (int i = 0; i < n_local; ++i) {
        acc.X.push_back(0.0);
        acc.Y.push_back(0.0);
        acc.Z.push_back(0.0);
    }
}


void generation_gaussienne_des_vitesses(struct Vecteur_3D* vit){

    std::random_device rd; // Génération d'une graine.
    std::mt19937 gen(rd()); // Génération par "gen" d'un entier non signé par l'algo de Mersenne en fonction de la graine.
    std::normal_distribution<> d_v_x{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_y{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.
    std::normal_distribution<> d_v_z{0.0,1.0}; // Moyenne des vitesses : 0 ; écart-type (moyenne quadratique des écarts à la moyenne) : 1 Å/s.

    f64 qdm_systeme_x = 0 ;
    f64 qdm_systeme_y = 0 ;
    f64 qdm_systeme_z = 0 ;

// Pour chaque appel de d_v_x(gen), d_v_y(gen) et d_v_z(gen) : transformation de l'entier généré par gen en un nouveau double aléatoire (gaussien) autour de 0.
// Calcul de la composante selon x, y et z du vecteur quantité de mouvement total du système.
    for(u64 i = 0; i < N; i++){
        vit->X[i] = d_v_x(gen);
        qdm_systeme_x = qdm_systeme_x + m*(vit->X[i]);
        vit->Y[i] = d_v_y(gen);
        qdm_systeme_y = qdm_systeme_y + m*(vit->Y[i]);
        vit->Z[i] = d_v_z(gen);
        qdm_systeme_z = qdm_systeme_z + m*(vit->Z[i]);
    }

    std::cout << "Quantité de mouvement du système selon x avant ajustement: " << qdm_systeme_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y avant ajustement: " << qdm_systeme_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z avant ajustement: " << qdm_systeme_z << "A/s" << std::endl;

// Ajustement de qdm_systeme à 0, ie m*vit->X[i] = m*vit->X[i] - qdm_systeme_x, ETC.
    f64 qdm_systeme_x_sur_N_sur_m = qdm_systeme_x/N/m;

    for(u64 i = 0; i < N; i++){
        vit->X[i] -= qdm_systeme_x_sur_N_sur_m;
        vit->Y[i] -= qdm_systeme_x_sur_N_sur_m;
        vit->Z[i] -= qdm_systeme_x_sur_N_sur_m;
//        std::cout << &vit->X[i] << ": vit_Gauss_après_qdm_syst_nulle_X = " <<vit->X[i] << std::endl;
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_qdm_syst_nulle_Y = " <<vit->Y[i] << std::endl;
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_qdm_syst_nulle_Z = " <<vit->Z[i] << std::endl;
    }

// Qdm totale après ajustement
    f64 qdm_systeme_après_x = 0 ;
    f64 qdm_systeme_après_y = 0 ;
    f64 qdm_systeme_après_z = 0 ;
    for(u64 i = 0; i < N; i++){
        qdm_systeme_après_x += m*(vit->X[i]);
        qdm_systeme_après_y += m*(vit->Y[i]);
        qdm_systeme_après_z += m*(vit->Z[i]);
    }

    std::cout << "Quantité de mouvement du système selon x après ajustement: " << qdm_systeme_après_x << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon y après ajustement: " << qdm_systeme_après_y << "A/s" << std::endl;
    std::cout << "Quantité de mouvement du système selon z après ajustement: " << qdm_systeme_après_z << "A/s" << std::endl;

// Calcul de la température en fonction de la norme du vecteur vitesse.
    f64 temperature = 0;
    f64 inverse_trois_k_b_N_m = 1/(3*k_b*N)*m;
    
    for(u64 i = 0; i < N; i++){
        temperature = temperature + inverse_trois_k_b_N_m*(pow(vit->X[i],2.0)+pow(vit->Y[i],2.0)+pow(vit->Z[i],2.0));
    }

// Calcul de la vitesse selon initialisation à temperature_cible;
    f64 sqrt_temperature_cible_sur_temperature = sqrt(temperature_cible/temperature);
    for(u64 i = 0; i < N; i++){
        vit->X[i] = (vit->X[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->X[i] << ": vit_Gauss_après_ajustement_Tcible_X = " <<vit->X[i] << std::endl;
        vit->Y[i] = (vit->Y[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->Y[i] << ": vit_Gauss_après_ajustement_Tcible_Y = " <<vit->Y[i] << std::endl;
        vit->Z[i] = (vit->Z[i])*sqrt_temperature_cible_sur_temperature;
//        std::cout << &vit->Z[i] << ": vit_Gauss_après_ajustement_Tcible_Z = " <<vit->Z[i] << std::endl;
    }
}

void accelerations_initiales_nulles(struct Vecteur_3D* acc){
    for(u64 i = 0; i < N; i++){
        acc->X[i] = 0.0;
//      std::cout << &acc->X[i] << ": accX = " <<acc->X[i] << std::endl;
        acc->Y[i] = 0.0;
//      std::cout << &acc->Y[i] << ": accY = " <<acc->Y[i] << std::endl;
        acc->Z[i] = 0.0;
//      std::cout << &acc->Z[i] << ": accZ = " <<acc->Z[i] << std::endl;
    }

}
