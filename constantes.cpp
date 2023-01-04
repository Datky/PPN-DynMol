#include "constantes.h"
#include <iostream>

u32 parametres_N::parametres() {
    std::cout << std::endl << "Merci d'entrer le nombre de particules N (5000 ou 10000 recommandés ou 15000): ";
    std::cin >> N;
    std::cout << std::endl;
    return N;
}
parametres_N objet_N {};

u32 parametres_nb_iteration::parametres() {
    std::cout << std::endl << "Merci d'entrer nb_iteration (exemple : 20): ";
    std::cin >> nb_iteration;
    std::cout << std::endl;
    return nb_iteration;
}
parametres_nb_iteration objet_nb_iteration {};

u32 parametres_dt::parametres() {
    std::cout << std::endl << "Merci d'entrer dt(en fs ; exemple : 10 fs): ";
    std::cin >> dt;
    std::cout << std::endl;
    return dt;
}
parametres_dt objet_dt {};

const u32 N = objet_N.parametres();
const u32 nb_iteration = objet_nb_iteration.parametres();
const u32 dt = objet_dt.parametres();


