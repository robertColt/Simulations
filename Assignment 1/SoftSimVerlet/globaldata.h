//
//  globaldata.h
//  softsim
//
//  Created by András Libál on 7/19/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

#ifndef globaldata_h
#define globaldata_h

#include <stdio.h>

struct global_struct
    {
    double SX, SY;
    double halfSX,halfSY;
    
    int N_particles;
    double *particle_x;
    double *particle_y;
    double *particle_fx;
    double *particle_fy;
    int *particle_color;
    double *particle_direction_x;
    double *particle_direction_y;
    
    double *particle_dx_so_far;
    double *particle_dy_so_far;

    double particle_driving_force;
    double partile_particle_screening_length;
    double partile_particle_screening_wavevector;
    
    int N_Verlet;
    int N_Verlet_max; //initial allocation + later, longest allocation
    int *Verletlisti;
    int *Verletlistj;
    int flag_to_rebuild_Verlet;

    double Verlet_cutoff_distance;
    double Verlet_cutoff_distance_squared;
    double Verlet_intershell_squared;
    
    double dt;
    
    int total_time;
    int echo_time;
    int movie_time;
    int time;
    
   
    
    FILE *moviefile;
    };

struct flag_struct
    {
    short int system_size_SX_set;
    short int system_size_SY_set;
    
    };



extern struct global_struct global;
extern struct flag_struct flag;
#endif /* globaldata_h */
