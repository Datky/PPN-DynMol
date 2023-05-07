/** @fichier
 * Algorithmes
 **/

#include <cmath>
#include <vector>
#include "../Headers/interaction.h"
//#include "rayonverlet.cpp"
//#include <vector>

//std::vector<int> NN, NL;
extern u32 N;
extern u32 nb_iteration;
extern u32 dt;
extern u32 b_x; // NEW
extern u32 b_y; // NEW
extern u32 b_z; // NEW

const int MN = 10000;

int trouver_ind(int valeur, std::vector<u32> &vec) {
      for (u32 i = 0; i < vec.size(); ++i) {
            if (vec[i] == (u32)valeur) {
                  return i;
            }
      }
      return -1;
}
          

void majPositionsetCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules &at, f64 const& r_cut_carre, Frontiere const& frontiere_type) {

      f64 F_x, F_y, F_z ;
      F_x = F_y = F_z = 0; // ????

      
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

          // Taille des cellules
          f64 tc_x = b_x / c_x;
          f64 tc_y = b_y / c_y;
          f64 tc_z = b_z / c_z;

      for (u32 i = 0; i < N; ++i) {


            int old_z = at.pos->Z[i] / tc_x;
            int old_y = at.pos->Y[i] / tc_y;
            int old_x = at.pos->X[i] / tc_z;


            // Calcul des positions
            at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
            at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
            at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

            // Mise en place d'une frontière
            unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut_carre);


            // Calcul de la nouvelle cellule de la particule
            int new_z = at.pos->Z[i] / tc_z;
            int new_y = at.pos->Y[i] / tc_y;
            int new_x = at.pos->X[i] / tc_x;



            if (old_x != new_x || old_y != new_y || old_z != new_z) { // La particule change de cellule
                  // Cherche la position de l'élément pour le supprimer
                  for (u32 id = 0; id < vec[old_z+1][old_y+1][old_x+1].size(); ++id) {
                        if (vec[old_z+1][old_y+1][old_x+1][id] == i) {
                              vec[old_z+1][old_y+1][old_x+1].erase(vec[old_z+1][old_y+1][old_x+1].begin()+id, vec[old_z+1][old_y+1][old_x+1].begin()+id+1);
                              break;
                        }
                  }
                  // Réinsertion dans la bonne cellule
                  vec[new_z+1][new_y+1][new_x+1].push_back(i);
            }
      }
}



void VerletCellulesPara(int rang, int P, int n_local, int cellules_locales, std::vector<std::vector<std::vector<std::vector<Particule_Cellule>>>> &vec, 
      Particules_Para &part, f64 const& r_cut_carre, Frontiere const& frontiere_type, MPI_Datatype* types, 
      u32** comms) {




      // Initialisation des communications
      int ind = 1;
      for (int j = 0; j < c_y; ++j) {
            for (int k = 0; k < c_x; ++k) {
                  // Up
                  comms[0][0] += vec[1][j+1][k+1].size();
                  comms[0][ind] = vec[1][j+1][k+1].size();


                  // Down
                  comms[1][0] += vec[cellules_locales][j+1][k+1].size();
                  comms[1][ind] = vec[cellules_locales][j+1][k+1].size();


                  ind++;
            }
      }

      Particule_Cellule* up = (Particule_Cellule*)malloc(sizeof(Particule_Cellule)*comms[0][0]);
      Particule_Cellule* down = (Particule_Cellule*)malloc(sizeof(Particule_Cellule)*comms[1][0]);
      Particule_Cellule* recv_up;
      Particule_Cellule* recv_down;


      
      int index_min_up = 0;
      int index_min_down = 0;
      for (int j = 0; j < c_y; ++j) {
            for (int k = 0; k < c_x; ++k) {

                  // Stockage dans les buffer d'envoi
                  // Up
                  for (u32 i = 0; i < vec[1][j+1][k+1].size(); ++i) {
                        up[index_min_up+i] = vec[1][j+1][k+1][i];      
                  }
                  index_min_up += vec[1][j+1][k+1].size();

                  // Down
                  for (u32 i = 0; i < vec[cellules_locales][j+1][k+1].size(); ++i) {
                        down[index_min_down+i] = vec[cellules_locales][j+1][k+1][i];          
                  }
                  index_min_down += vec[cellules_locales][j+1][k+1].size();
            }
      }



      MPI_Status sta;
      MPI_Request req;

      int voisin_haut = rang-1;
      int voisin_bas = rang+1;
      if (voisin_haut < 0) voisin_haut = P-1;
      if (voisin_bas == P) voisin_bas = 0;


      // Envoi et réception des cellules fantômes
      int count = (c_y*c_x)+1;

      // Envoie au voisin du bas
      MPI_Isend(&comms[1][0], count, MPI_UNSIGNED, voisin_bas, rang, MPI_COMM_WORLD, &req);
      // Envoie au voisin du haut
      MPI_Isend(&comms[0][0], count, MPI_UNSIGNED, voisin_haut, rang, MPI_COMM_WORLD, &req);


      // Reçoit du voisin du haut
      MPI_Recv(&comms[2][0], count, MPI_UNSIGNED, voisin_haut, voisin_haut, MPI_COMM_WORLD, &sta);
      // Reçoit du voisin du bas
      MPI_Recv(&comms[3][0], count, MPI_UNSIGNED, voisin_bas, voisin_bas, MPI_COMM_WORLD, &sta);




      recv_up = (Particule_Cellule*)malloc(sizeof(Particule_Cellule)*comms[2][0]);
      recv_down = (Particule_Cellule*)malloc(sizeof(Particule_Cellule)*comms[3][0]);


      // Envoi des cellules fantômes
      MPI_Isend(down, comms[1][0], types[0], voisin_bas, rang, MPI_COMM_WORLD, &req);
      MPI_Isend(up, comms[0][0], types[0], voisin_haut, rang, MPI_COMM_WORLD, &req);


      // Recouvrement des communications
      MPI_Request req_recv_down, req_recv_up;
      int flag_down, flag_up;
      MPI_Irecv(recv_down, comms[3][0], types[0], voisin_bas, voisin_bas, MPI_COMM_WORLD, &req_recv_down);
      MPI_Irecv(recv_up, comms[2][0], types[0], voisin_haut, voisin_haut, MPI_COMM_WORLD, &req_recv_up);




      // Périodicité sur les côtés
      for (int i = 0; i < cellules_locales; ++i) {
            for (int j = 1; j <= c_y; ++j) {
                  vec[i+1][j][0].erase(vec[i+1][j][0].begin(), vec[i+1][j][0].end());
                  vec[i+1][j][c_x+1].erase(vec[i+1][j][c_x+1].begin(), vec[i+1][j][c_x+1].end());

                  for (u32 ind = 0; ind < vec[i+1][j][1].size(); ++ind) {
                        vec[i+1][j][c_x+1].push_back(vec[i+1][j][1][ind]);
                  }
                  for (u32 ind = 0; ind < vec[i+1][j][c_x].size(); ++ind) {
                        vec[i+1][j][0].push_back(vec[i+1][j][c_x][ind]);
                  }
            }
            for (int k = 1; k <= c_x; ++k) {
                  vec[i+1][0][k].erase(vec[i+1][0][k].begin(), vec[i+1][0][k].end());
                  vec[i+1][c_y+1][k].erase(vec[i+1][c_y+1][k].begin(), vec[i+1][c_y+1][k].end());

                  for (u32 ind = 0; ind < vec[i+1][1][k].size(); ++ind) {
                        vec[i+1][c_y+1][k].push_back(vec[i+1][1][k][ind]);
                  }
                  for (u32 ind = 0; ind < vec[i+1][c_y][k].size(); ++ind) {
                        vec[i+1][0][k].push_back(vec[i+1][c_y][k][ind]);
                  }
            }
      }



      f64 F_x, F_y, F_z;
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

      // Taille des cellules
      f64 tc_x = (f64)b_x / c_x;
      f64 tc_y = (f64)b_y / c_y;
      f64 tc_z = (f64)b_z / c_z;

      int locales = c_z / P;


      Vecteur_3D forces;
      forces.X = (f64*)malloc(sizeof(f64)*n_local);
      forces.Y = (f64*)malloc(sizeof(f64)*n_local);
      forces.Z = (f64*)malloc(sizeof(f64)*n_local);
      int forces_index = 0;

      // Boucle dans les cellules

      for (int i = 1; i < cellules_locales+1; ++i) {
            for (int j = 1; j < c_y+1; ++j) {
                  for (int k = 1; k < c_x+1; ++k) {

                        
                        // Verlet
                        for (u32 ind = 0; ind < vec[i][j][k].size(); ++ind) {
                              // Mise à zéro de la force
                              F_x = F_y = F_z = 0;

                              int vrai_id = trouver_ind(vec[i][j][k][ind].id, part.ids); // Les particules ne sont pas rangées dans le même ordre

                              
                              // 1er calcul des vitesses : v_i(t+dt/2)
                              part.vit.X[vrai_id] += 0.5*part.acc.X[vrai_id]*dt;
                              part.vit.Y[vrai_id] += 0.5*part.acc.Y[vrai_id]*dt;
                              part.vit.Z[vrai_id] += 0.5*part.acc.Z[vrai_id]*dt;



                              // Calcul de la force : F_i(t+dt) et a_i(t+dt)
                              for (int ii = i-1; ii <= i+1; ++ii) {
                                    for (int jj = j-1; jj <= j+1; ++jj) {
                                          for (int kk = k-1; kk <= k+1; ++kk) {
                                                for (u32 vois = 0; vois < vec[ii][jj][kk].size(); ++vois) {

                                                      f64 r_x = unique_limites->calculDistance(part.pos.X[vrai_id], vec[ii][jj][kk][vois].X, b_x, r_cut_carre);
                                                      f64 r_y = unique_limites->calculDistance(part.pos.Y[vrai_id], vec[ii][jj][kk][vois].Y, b_y, r_cut_carre);
                                                      f64 r_z = unique_limites->calculDistance(part.pos.Z[vrai_id], vec[ii][jj][kk][vois].Z, b_z, r_cut_carre);
                                                      f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                                                      if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                                                      F_x += F_Lennard_Jones(r_global_carre)*r_x;
                                                      F_y += F_Lennard_Jones(r_global_carre)*r_y;
                                                      F_z += F_Lennard_Jones(r_global_carre)*r_z;

                                                      }
                                                }
                                          }
                                    }
                              }

                              forces.X[forces_index] = F_x;
                              forces.Y[forces_index] = F_y;
                              forces.Z[forces_index] = F_y;
                              forces_index++;



                              
                        } 
                  }
            }
      }



      
      // Limite de recouvrement des communications
      MPI_Test(&req_recv_down, &flag_down, MPI_STATUS_IGNORE);
      MPI_Test(&req_recv_up, &flag_up, MPI_STATUS_IGNORE);
      while (!flag_down || !flag_up) {
            MPI_Test(&req_recv_down, &flag_down, MPI_STATUS_IGNORE);
            MPI_Test(&req_recv_up, &flag_up, MPI_STATUS_IGNORE); // truncated
      }

      

      forces_index = 0;
      // Verlet avec les cellules fantômes
      for (int i = 0; i < cellules_locales; ++i) {
            for (int j = 0; j < c_y; ++j) {
                  for (int k = 0; k < c_x; ++k) {
                        // Verlet
                        for (u32 ind = 0; ind < vec[i+1][j+1][k+1].size(); ++ind) {

                              int vrai_id = trouver_ind(vec[i+1][j+1][k+1][ind].id, part.ids);
                              
                              // Cellules fantômes du haut
                              if (rang != 0) {
                                    if (i == 0) {
                                          for (int jj = j-1; jj <= j+1; ++jj) {
                                                for (int kk = k-1; kk <= k+1; ++kk) {
                                                      if ((jj >= 0 && jj < c_y) && (kk >= 0 && kk < c_x)) {
                                                            int comms_ind = (jj*c_y) + kk;
                                                            int somme = 0;
                                                            for (int iter = 1; iter < comms_ind+1; ++iter) {
                                                                  somme += comms[2][iter];
                                                            }
                                                            for (u32 ghost = somme; ghost < somme+comms[2][comms_ind+1]; ++ghost) {
                                                                  f64 r_x = unique_limites->calculDistance(part.pos.X[vrai_id], recv_up[ghost].X, b_x, r_cut_carre);
                                                                  f64 r_y = unique_limites->calculDistance(part.pos.Y[vrai_id], recv_up[ghost].Y, b_y, r_cut_carre);
                                                                  f64 r_z = unique_limites->calculDistance(part.pos.Z[vrai_id], recv_up[ghost].Z, b_z, r_cut_carre);
                                                                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                                                                  if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                                                                        forces.X[forces_index] += F_Lennard_Jones(r_global_carre)*r_x;
                                                                        forces.Y[forces_index] += F_Lennard_Jones(r_global_carre)*r_y;
                                                                        forces.Z[forces_index] += F_Lennard_Jones(r_global_carre)*r_z;

                                                                  }
                                                            }
                                                      }
                                                }
                                          }   
                                    }
                              }

                              // Cellules fantômes du bas
                              if (rang != P-1) {
                                    if (i == cellules_locales-1) {
                                          for (int jj = j-1; jj <= j+1; ++jj) {
                                                for (int kk = k-1; kk <= k+1; ++kk) {
                                                      if ((jj >= 0 && jj < c_y) && (kk >= 0 && kk < c_x)) {
                                                            int comms_ind = (jj*c_y) + kk;
                                                            int somme = 0;
                                                            for (int iter = 1; iter < comms_ind+1; ++iter) {
                                                                  somme += comms[3][iter];
                                                            }
                                                            for (u32 ghost = somme; ghost < somme+comms[3][comms_ind+1]; ++ghost) {
                                                                  f64 r_x = unique_limites->calculDistance(part.pos.X[vrai_id], recv_down[ghost].X, b_x, r_cut_carre);
                                                                  f64 r_y = unique_limites->calculDistance(part.pos.Y[vrai_id], recv_down[ghost].Y, b_y, r_cut_carre);
                                                                  f64 r_z = unique_limites->calculDistance(part.pos.Z[vrai_id], recv_down[ghost].Z, b_z, r_cut_carre);
                                                                  f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                                                                  if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                                                                        forces.X[forces_index] += F_Lennard_Jones(r_global_carre)*r_x;
                                                                        forces.Y[forces_index] += F_Lennard_Jones(r_global_carre)*r_y;
                                                                        forces.Z[forces_index] += F_Lennard_Jones(r_global_carre)*r_z;

                                                                  }
                                                            }
                                                      }
                                                }
                                          }   
                                    }
                              }
                              
                              // Calcul des accélérations : a_i(t+dt)
                              part.acc.X[vrai_id] = forces.X[forces_index]/m;
                              part.acc.Y[vrai_id] = forces.Y[forces_index]/m;
                              part.acc.Z[vrai_id] = forces.Z[forces_index]/m;

                              // 2ième calcul des vitesses : v_i(t+dt)
                              part.vit.X[vrai_id] += 0.5*part.acc.X[vrai_id]*dt;
                              part.vit.Y[vrai_id] += 0.5*part.acc.Y[vrai_id]*dt;
                              part.vit.Z[vrai_id] += 0.5*part.acc.Z[vrai_id]*dt;

                              forces_index++;
                        }
                        
                  }
            }
      }

      /*
      free(recv_up);
      free(recv_down);
      free(up);
      free(down);
      */

      std::vector<std::vector<Particule_Comm>> send_vecs;
      std::vector<u32> change_vecs;
      for (int i = 0; i < P; ++i) {
            std::vector<Particule_Comm> tmp_vec;
            send_vecs.push_back(tmp_vec);
      }

      Particule_Comm part_curr;

      printf("RANG %d ERREUR APRES CE POINT\n", rang);
      // Mise à jour des positions
      for (int i = 0; i < cellules_locales; ++i) {
            for (int j = 0; j < c_y; ++j) {
                  for (int k = 0; k < c_x; ++k) {
                        for (u32 ind = 0; ind < vec[i+1][j+1][k+1].size(); ++ind) {


                              
                              int vrai_id = trouver_ind(vec[i+1][j+1][k+1][ind].id, part.ids);

                              // Vieilles positions
                              int old_z = (part.pos.Z[vrai_id] / tc_z) - (locales*rang);
                              int old_y = (part.pos.Y[vrai_id] / tc_y);
                              int old_x = (part.pos.X[vrai_id] / tc_x);


                              
                              // Calcul des nouvelles positions
                              part.pos.X[vrai_id] += part.vit.X[vrai_id]*dt + 0.5*part.acc.X[vrai_id]*pow(dt,2.0);
                              part.pos.Y[vrai_id] += part.vit.Y[vrai_id]*dt + 0.5*part.acc.Y[vrai_id]*pow(dt,2.0);
                              part.pos.Z[vrai_id] += part.vit.Z[vrai_id]*dt + 0.5*part.acc.Z[vrai_id]*pow(dt,2.0);
                              
                              
                              // Mise en place d'une frontière périodique
                              part.pos.X[vrai_id] = std::abs(std::fmod(part.pos.X[vrai_id], (f64)b_x));
                              part.pos.Y[vrai_id] = std::abs(std::fmod(part.pos.Y[vrai_id], (f64)b_y));
                              part.pos.Z[vrai_id] = std::abs(std::fmod(part.pos.Z[vrai_id], (f64)b_z));
                                    


                              // Nouvelles positions
                              f64 z_calcul = (part.pos.Z[vrai_id] / tc_z);

                              int new_z = z_calcul - (locales*rang);
                              f64 precise_z = z_calcul - (f64)(locales*rang);

                              int new_y = (part.pos.Y[vrai_id] / tc_y);
                              int new_x = (part.pos.X[vrai_id] / tc_x);



                              if (new_x != old_x || new_y != old_y || new_z != old_z) {
                                    if (precise_z < 0.0 || precise_z > cellules_locales*tc_z) { // Changement de processus

                                          // Calcul du nouveau processus
                                          int new_process = (int)std::fmod(z_calcul, c_z);


                                          part_curr.id = vec[i+1][j+1][k+1][ind].id;
                                          part_curr.posX = part.pos.X[vrai_id];
                                          part_curr.posY = part.pos.Y[vrai_id];
                                          part_curr.posZ = part.pos.Z[vrai_id];

                                          part_curr.vitX = part.vit.X[vrai_id];
                                          part_curr.vitY = part.vit.Y[vrai_id];
                                          part_curr.vitZ = part.vit.Z[vrai_id];

                                          part_curr.accX = part.acc.X[vrai_id];
                                          part_curr.accY = part.acc.Y[vrai_id];
                                          part_curr.accZ = part.acc.Z[vrai_id];
                                          send_vecs[new_process].push_back(part_curr);


                                          continue;

                                    } else { // Changement simple de cellule
                                          change_vecs.push_back(vec[i+1][j+1][k+1][ind].id);
                                          continue;
                                    }
                              }

                              if (precise_z < 0.0 || precise_z > cellules_locales*tc_z) { // Changement de processus

                                    // Calcul du nouveau processus
                                    int new_process = (int)std::fmod(z_calcul, c_z);


                                    part_curr.id = vec[i+1][j+1][k+1][ind].id;
                                    part_curr.posX = part.pos.X[vrai_id];
                                    part_curr.posY = part.pos.Y[vrai_id];
                                    part_curr.posZ = part.pos.Z[vrai_id];

                                    part_curr.vitX = part.vit.X[vrai_id];
                                    part_curr.vitY = part.vit.Y[vrai_id];
                                    part_curr.vitZ = part.vit.Z[vrai_id];

                                    part_curr.accX = part.acc.X[vrai_id];
                                    part_curr.accY = part.acc.Y[vrai_id];
                                    part_curr.accZ = part.acc.Z[vrai_id];
                                    send_vecs[new_process].push_back(part_curr);

                                    }
                        }    
                  }
            }
      }

      
      // Préparation des communications
      int* counts = (int*)malloc(sizeof(int)*P); // Nombre de particules à recevoir
      int* flags = (int*)malloc(sizeof(int)*P);
      int* flags_recv = (int*)malloc(sizeof(int)*P);


      MPI_Request* count_recv_reqs = (MPI_Request*)malloc(sizeof(MPI_Request)*P);
      MPI_Request* send_buffer_reqs = (MPI_Request*)malloc(sizeof(MPI_Request)*P);
      MPI_Request* recv_buffer_reqs = (MPI_Request*)malloc(sizeof(MPI_Request)*P);
      MPI_Request req_send_count;



      for (int i = 0; i < P; ++i) {
            if (i != rang) {
                  int count = send_vecs[i].size();
                  MPI_Isend(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &req_send_count);
                  MPI_Irecv(&counts[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &count_recv_reqs[i]);
            }
      }

      
      // Attente de toutes les infos
      int somme_flags = 0;
      while (somme_flags != P-1) {
            somme_flags = 0;
            for (int i = 0; i < P; ++i) {
                  if (i != rang) {
                        MPI_Test(&count_recv_reqs[i], &flags[i], MPI_STATUS_IGNORE);
                        somme_flags += flags[i];
                  }
            }
      }
      

      // Réception des nouvelles particules dans le processus courant
      Particule_Comm** part_recv = (Particule_Comm**)malloc(sizeof(Particule_Comm*)*P);
      for (int i = 0; i < P; ++i) {
            part_recv[i] = (Particule_Comm*)malloc(sizeof(Particule_Comm)*counts[i]);
            if (i != rang) {
                  MPI_Isend(&send_vecs[i][0], send_vecs[i].size(), types[1], i, 0, MPI_COMM_WORLD, &send_buffer_reqs[i]);
                  MPI_Irecv(&part_recv[i][0], counts[i], types[1], i, 0, MPI_COMM_WORLD, &recv_buffer_reqs[i]);
            }
      }



      // Recouvrement des communications
      Particule_Cellule part_cour;
      for (u32 part_cell = 0; part_cell < change_vecs.size(); ++part_cell) { // Changement de cellule

            bool found = false;
            // Trouver la cellule courante
            for (int i = 1; i < cellules_locales+1; ++i) {
                  for (int j = 1; j < c_y+1; ++j) {
                        for (int k = 1; k < c_x+1; ++k) {
                              for (u32 ind = 0; ind < vec[i][j][k].size(); ++ind) {
                                    if (change_vecs[i] == vec[i][j][k][ind].id) {
                                          part_cour = vec[i][j][k][ind];
                                          vec[i][j][k].erase(vec[i][j][k].begin() + ind);
                                          found = true;
                                          break;
                                    }
                                    if (found) break;
                              }
                              if (found) break;
                        }
                        if (found) break;
                  }
                  if (found) break;
            }
            int cell_x = part_cour.X / tc_x;
            int cell_y = part_cour.Y / tc_y;
            int cell_z = part_cour.Z / tc_z - (locales*rang);
            vec[cell_z+1][cell_y+1][cell_x+1].push_back(part_cour);
      }


      for (int process = 0; process < P; ++process) {
            for (u32 part_cell = 0; part_cell < send_vecs[process].size(); ++part_cell) { // Changement de cellule
                  bool found = false;
                  // Trouver la cellule courante
                  for (int i = 1; i < cellules_locales+1; ++i) {
                        for (int j = 1; j < c_y+1; ++j) {
                              for (int k = 1; k < c_x+1; ++k) {
                                    for (u32 ind = 0; ind < vec[i][j][k].size(); ++ind) {
                                          if (send_vecs[process][i].id == vec[i][j][k][ind].id) {
                                                vec[i][j][k].erase(vec[i][j][k].begin() + ind);
                                                found = true;
                                                break;
                                          }
                                          if (found) break;
                                    }
                                    if (found) break;
                              }
                              if (found) break;
                        }
                        if (found) break;
                  }
            }
      }




      // Suppression des particules qui ont changées de processus

      for (int i = 0; i < P; ++i) {
            for (u32 j = 0; j < send_vecs[i].size(); ++j) {
                  int vrai_id = trouver_ind(send_vecs[i][j].id, part.ids);
                  part.ids.erase(part.ids.begin()+vrai_id);
                  part.pos.X.erase(part.pos.X.begin()+vrai_id);
                  part.pos.Y.erase(part.pos.Y.begin()+vrai_id);
                  part.pos.Z.erase(part.pos.Z.begin()+vrai_id);

                  part.vit.X.erase(part.vit.X.begin()+vrai_id);
                  part.vit.Y.erase(part.vit.Y.begin()+vrai_id);
                  part.vit.Z.erase(part.vit.Z.begin()+vrai_id);

                  part.acc.X.erase(part.acc.X.begin()+vrai_id);
                  part.acc.Y.erase(part.acc.Y.begin()+vrai_id);
                  part.acc.Z.erase(part.acc.Z.begin()+vrai_id);
            }
      }


      
      // Attente de toutes les communications
      int somme_flags_recv = 0;
      while (somme_flags_recv != P-1) {
            somme_flags_recv = 0;
            for (int i = 0; i < P; ++i) {
                  if (i != rang) {
                        MPI_Test(&recv_buffer_reqs[i], &flags_recv[i], MPI_STATUS_IGNORE);
                        somme_flags_recv += flags_recv[i];
                  }
            }
      }
      


      Particule_Cellule curr;
      // Réception des nouvelles particules
      for (int i = 0; i < P; ++i) {
            if (i != rang) {
                  for (int j = 0; j < counts[i]; ++j) {
                        part.pos.X.push_back(part_recv[i][j].posX);
                        part.pos.Y.push_back(part_recv[i][j].posY);
                        part.pos.Z.push_back(part_recv[i][j].posZ);

                        part.vit.X.push_back(part_recv[i][j].vitX);
                        part.vit.Y.push_back(part_recv[i][j].vitY);
                        part.vit.Z.push_back(part_recv[i][j].vitZ);

                        part.acc.X.push_back(part_recv[i][j].accX);
                        part.acc.Y.push_back(part_recv[i][j].accY);
                        part.acc.Z.push_back(part_recv[i][j].accZ);
                        part.ids.push_back(part_recv[i][j].id);


                        // Màj des cellules
                        int ind_z = (part_recv[i][j].posZ / tc_z) - (locales*rang);
                        int ind_y = (part_recv[i][j].posY / tc_y);
                        int ind_x = (part_recv[i][j].posX / tc_x);
                          
                        curr.id = part.ids[i];
                        curr.X = part.pos.X[i];
                        curr.Y = part.pos.Y[i];
                        curr.Z = part.pos.Z[i];
                          

                        // Attention aux ghost cells   
                        vec[ind_z+1][ind_y+1][ind_x+1].push_back(curr);
                  }
            }
      }




}



void VerletCellules(std::vector<std::vector<std::vector<std::vector<u32>>>> &vec, Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type) {

      f64 F_x, F_y, F_z ;
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);



      // Boucle dans les cellules en évitant les ghost cell
      for (int i = 1; i < c_z+1; ++i) {
            for (int j = 1; j < c_y+1; ++j) {
                  for (int k = 1; k < c_x+1; ++k) {

                        
                        // Verlet
                        for (u32 ind = 0; ind < vec[i][j][k].size(); ++ind) {
                              // Mise à zéro de la force
                              F_x = F_y = F_z = 0; 

                              int particule = vec[i][j][k][ind];



                              // 1er calcul des vitesses : v_i(t+dt/2)
                              at.vit->X[particule] += 0.5*at.acc->X[particule]*dt;
                              at.vit->Y[particule] += 0.5*at.acc->Y[particule]*dt;
                              at.vit->Z[particule] += 0.5*at.acc->Z[particule]*dt;



                              // Calcul de la force : F_i(t+dt) et a_i(t+dt)
                              for (int ii = i-1; ii <= i+1; ++ii) {
                                    for (int jj = j-1; jj <= j+1; ++jj) {
                                          for (int kk = k-1; kk <= k+1; ++kk) {
                                                for (u32 vois = 0; vois < vec[ii][jj][kk].size(); ++vois) {
                                                      //f64 r_x = at.pos->X[particule] - at.pos->X[vec[ii][jj][kk][vois]];
                                                      //f64 r_y = at.pos->Y[particule] - at.pos->Y[vec[ii][jj][kk][vois]];
                                                      //f64 r_z = at.pos->Z[particule] - at.pos->Z[vec[ii][jj][kk][vois]];
                                                      f64 r_x = unique_limites->calculDistance(at.pos->X[particule], at.pos->X[vec[ii][jj][kk][vois]], b_x, r_cut_carre);
                                                      f64 r_y = unique_limites->calculDistance(at.pos->Y[particule], at.pos->Y[vec[ii][jj][kk][vois]], b_y, r_cut_carre);
                                                      f64 r_z = unique_limites->calculDistance(at.pos->Z[particule], at.pos->Z[vec[ii][jj][kk][vois]], b_z, r_cut_carre);
                                                      f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);

                                                      if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                                                      F_x += F_Lennard_Jones(r_global_carre)*r_x;
                                                      F_y += F_Lennard_Jones(r_global_carre)*r_y;
                                                      F_z += F_Lennard_Jones(r_global_carre)*r_z;

                                                      }
                                                }
                                          }
                                    }
                              }

                              // Calcul des accélérations : a_i(t+dt)
                              at.acc->X[particule] = F_x/m;
                              at.acc->Y[particule] = F_y/m;
                              at.acc->Z[particule] = F_z/m;

                              // 2ième calcul des vitesses : v_i(t+dt)
                              at.vit->X[particule] += 0.5*at.acc->X[particule]*dt;
                              at.vit->Y[particule] += 0.5*at.acc->Y[particule]*dt;
                              at.vit->Z[particule] += 0.5*at.acc->Z[particule]*dt;



                        }
                  }
            }
      }
}

// Algorithme de Verlet
void Verlet(Particules & at, f64 const& r_cut_carre, Frontiere const& frontiere_type){

      f64 F_x, F_y, F_z ;
      
      // Mise en place d'une frontière : création du pointeur
      auto unique_limites = LimitesFabric::create(frontiere_type);

      for(u32 i=0; i<N; ++i){

            // Mise à zéro de la force
            F_x = F_y = F_z = 0;  

             // Calcul des positions : p_i(t+dt)
             at.pos->X[i] += at.vit->X[i]*dt + 0.5*at.acc->X[i]*pow(dt,2.0);
             at.pos->Y[i] += at.vit->Y[i]*dt + 0.5*at.acc->Y[i]*pow(dt,2.0);
             at.pos->Z[i] += at.vit->Z[i]*dt + 0.5*at.acc->Z[i]*pow(dt,2.0);

             // Mise en place d'une frontière
             unique_limites->creeLimites(at.pos->X[i], at.pos->Y[i], at.pos->Z[i], F_x, F_y, F_z, r_cut_carre);

             // 1er calcul des vitesses : v_i(t+dt/2)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;

             // Calcul de la force : F_i(t+dt) et a_i(t+dt)
             for(u32 j=0; j<N; ++j){ 

/*                   
                   //Ne parcourir que la liste de voisin//
                   for (int jj = 0; jj < NN[i]; ++jj){
                         const int j = NL[i * MN + jj];// Calcul de la distance entre les atomes : r_i(t+dt)

                         f64 r_x = at.pos->X[i] - at.pos->X[j];
                         f64 r_y = at.pos->Y[i] - at.pos->Y[j];
                         f64 r_z = at.pos->Z[i] - at.pos->Z[j];
*/
//                         rayonverlet(b_x, b_y, b_z, r_x, r_y, r_z);
                         // f64 r_global = sqrt(pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0)); !AVANT!


                         f64 r_x = unique_limites->calculDistance(at.pos->X[i], at.pos->X[j], b_x, r_cut_carre);
                         f64 r_y = unique_limites->calculDistance(at.pos->Y[i], at.pos->Y[j], b_y, r_cut_carre);
                         f64 r_z = unique_limites->calculDistance(at.pos->Z[i], at.pos->Z[j], b_z, r_cut_carre);

                         
                         f64 r_global_carre = pow(r_x,2.0) + pow(r_y,2.0) + pow(r_z,2.0);
                         // Calcul de la force si la distance inter-atomique globale est inférieure au rayon de coupure :

                         // if (r_global<r_cut && r_global!=0) { !AVANT!                 
                         if (r_global_carre<r_cut_carre && r_global_carre!=0) {
                               F_x += F_Lennard_Jones(r_global_carre)*r_x;
                               F_y += F_Lennard_Jones(r_global_carre)*r_y;
                               F_z += F_Lennard_Jones(r_global_carre)*r_z;

                                // Peut être pas nécessaire
                        // Stocker dans la liste de voisin
               
                        // at.liste[i].append(j)
                     //}    
                   }
             }

             // Calcul des accélérations : a_i(t+dt)
             at.acc->X[i] = F_x/m;
             at.acc->Y[i] = F_y/m;
             at.acc->Z[i] = F_z/m;

             // 2ième calcul des vitesses : v_i(t+dt)
             at.vit->X[i] += 0.5*at.acc->X[i]*dt;
             at.vit->Y[i] += 0.5*at.acc->Y[i]*dt;
             at.vit->Z[i] += 0.5*at.acc->Z[i]*dt;
      }   
}

/*Fabrication****************************************************************************************************/
std::unique_ptr<Limites> LimitesFabric::create(Frontiere const& frontiere_type) {
      std::unique_ptr<Limites> unique_limites;

      switch (frontiere_type) {
            case Frontiere::Periodiques:
                  unique_limites = std::make_unique<LimitesPeriodiques>();
                  break;

            case Frontiere::Murs:
                  unique_limites = std::make_unique<LimitesMurs>();
                  break;
      }
      return unique_limites;
}

