#include <cmath>
#include <fstream>
#include "Headers/types.h"
#include "Headers/constantes.h"
#include "SoA/particule.h"
#include "Headers/interaction.h"
#include "Headers/XYZ.h"
#include "Headers/remplissage_vecteurs.h"
#include "Headers/potentiel.h"
#include "Headers/cellules.h"
#define ASSERT_TRUE(test) if (!(test)) std::cout << "Test failed in file " << __FILE__ \
                                       << " line " << __LINE__ << ": " #test << std::endl 
    
u32 N = 20; 
u32 nb_iteration = 1; 
u32 dt = 1; 
u32 b_x = 700; 
u32 b_y = 700; 
u32 b_z = 700;

void Test_XYZ(){
    
    struct Vecteur_3D *__restrict pos_S = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
    pos_S->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    pos_S->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    pos_S->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    struct Vecteur_3D *__restrict  pos_C= static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
    pos_C->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    pos_C->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    pos_C->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

    lireXYZ("Test/TEST_source.xyz", pos_S);
    ecrireXYZ(pos_S, "Test/TEST_cible.xyz");
    lireXYZ("Test/TEST_cible.xyz", pos_C);

    for(u64 i = 0; i < N; i++){
        ASSERT_TRUE(pos_S->X[i] == pos_C->X[i]);
        ASSERT_TRUE(pos_S->Y[i] == pos_C->Y[i]);
        ASSERT_TRUE(pos_S->Z[i] == pos_C->Z[i]);
    }    
}

void Test_accelerations_initiales_nulles(){    
    struct Vecteur_3D *__restrict acc = static_cast<Vecteur_3D*>(std::aligned_alloc(sizeof(Vecteur_3D), sizeof(Vecteur_3D)));
    acc->X = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    acc->Y = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));
    acc->Z = static_cast<f64*>(std::aligned_alloc(sizeof(f64), sizeof(f64)*N));

    accelerations_initiales_nulles(acc);
    for(u64 i = 0; i < N; i++){
        ASSERT_TRUE(acc->X[i] == 0.0);
        ASSERT_TRUE(acc->Y[i] == 0.0);
        ASSERT_TRUE(acc->Z[i] == 0.0);
    }
}

void Test_F_Lennard_Jones(){ 
    ASSERT_TRUE(F_Lennard_Jones(0.1)>0);
    ASSERT_TRUE(F_Lennard_Jones(1000)<0);
    f64 x = d*sqrt(cbrt(2));     //d*2^(1/6)=3.82198
    f64 F_x = F_Lennard_Jones(x*x);
    ASSERT_TRUE(F_x<1e-9 && F_x>-1e-9);
    f64 r_cut = d*2.5;          //d*2.5=8.5125
    f64 F_r_cut = F_Lennard_Jones(r_cut*r_cut);
    ASSERT_TRUE(F_r_cut<1e-5 && F_r_cut>-1e-5);
}

void Graphique_F_Lennard_Jones(){ 
      std::ofstream fichier;
      fichier.open("Test/Data_force_Lennard_Jones.dat");

      fichier << "r F" << std::endl;

      for (f64 r = 3.5; r < 9; r+=0.01) {
            f64 F = F_Lennard_Jones(r*r);
            fichier << r << " "<< F << std::endl;
      }
      
      fichier.close();
}

void Test_creeLimites(){    

    f64 X, Y, Z, F_x, F_y, F_z ;
    f64 r_cut_carre = 1;

    //Periodiques
    X = 2+b_x;
    Y = 155.5-2*b_y;
    Z = 200.5+2*b_z;

    auto frontiere_P = Frontiere::Periodiques; 
    auto P_limites = LimitesFabric::create(frontiere_P);
    P_limites->creeLimites(X, Y, Z, F_x, F_y, F_z, r_cut_carre);

    ASSERT_TRUE(X == 2);
    ASSERT_TRUE(Y == 155.5);
    ASSERT_TRUE(Z == 200.5);

    //Murs
    F_x = F_y = F_z = 0;
    X = b_x-0.55;
    Y = b_y-0.90;
    Z = b_z-0.2;

    auto frontiere_M = Frontiere::Murs;
    auto M_limites = LimitesFabric::create(frontiere_M);
    M_limites->creeLimites(X,Y, Z, F_x, F_y, F_z, r_cut_carre);

    ASSERT_TRUE(X<b_x && X>0);
    ASSERT_TRUE(Y<b_y && Y>0);
    ASSERT_TRUE(Z<b_z && Z>0);

    ASSERT_TRUE(F_x!=0);
    ASSERT_TRUE(F_y!=0);
    ASSERT_TRUE(F_z!=0);
}

void Test_calculDistance(){
    
    f64 r, i, j;
    f64 r_cut_carre = 3*3;
    f64 b = 10;

    //Periodiques
    auto frontiere_P = Frontiere::Periodiques; 
    auto P_limites = LimitesFabric::create(frontiere_P);

    //Murs
    auto frontiere_M = Frontiere::Murs;
    auto M_limites = LimitesFabric::create(frontiere_M);

    i = 7;
    j = 5;
    r = P_limites->calculDistance(i, j, b, r_cut_carre); //Periodiques
    ASSERT_TRUE(r == 2);
    r = M_limites->calculDistance(i, j, b, r_cut_carre); //Murs
    ASSERT_TRUE(r == 2);

    i = 5;
    j = 7;
    r = P_limites->calculDistance(i, j, b, r_cut_carre); //Periodiques
    ASSERT_TRUE(r == -2);
    r = M_limites->calculDistance(i, j, b, r_cut_carre); //Murs
    ASSERT_TRUE(r == -2);

    i = 9;
    j = 1;
    r = P_limites->calculDistance(i, j, b, r_cut_carre); //Periodiques
    ASSERT_TRUE(r == -2);
    r = M_limites->calculDistance(i, j, b, r_cut_carre); //Murs
    ASSERT_TRUE(r == 8);

    i = 1;
    j = 9;
    r = P_limites->calculDistance(i, j, b, r_cut_carre); //Periodiques
    ASSERT_TRUE(r == 2);
    r = M_limites->calculDistance(i, j, b, r_cut_carre); //Murs
    ASSERT_TRUE(r == -8);
}


int main() {
    
    Test_XYZ();
    Test_accelerations_initiales_nulles();
    Test_F_Lennard_Jones();
    Graphique_F_Lennard_Jones();
    Test_creeLimites();
    Test_calculDistance();

    return 0;
}
