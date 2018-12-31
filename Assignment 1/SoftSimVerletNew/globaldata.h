
//  globaldata.h
//  Simulation Methods course, 2018
//  First Assignment: Molecular Dynamics (Brownian Dynamics) Simulation

#ifndef globaldata_h
#define globaldata_h

#include <stdio.h>

struct global_struct
{
	double SX, SY;
	double halfSX, halfSY;

	int     N_particles;

	double  *particle_x;                        //particle x position
	double  *particle_y;                        //particle y position
	double  *particle_fx;                       //particle total force x direction
	double  *particle_fy;                       //particle total force y direction
	int     *particle_color;                    //particle color
	double  *particle_direction;                //direction of external force

	double  particle_driving_force;             //external driving force
	double  particle_particle_screening_length;  //inter-partcile force screening length

	double dt;          //simulation step length

	int total_time;     //total running time
	int echo_time;      //echo to screen
	int movie_time;     //write to file
	int time;           //current time step

	/*
		VERLET LIST
	*/
	int N_Verlet;
	int N_Verlet_max; //initial allocation + later, longest allocation
	int *Verletlisti;
	int *Verletlistj;
	int flag_to_rebuild_Verlet;

	double Verlet_cutoff_distance;
	double Verlet_cutoff_distance_squared;
	double Verlet_intershell_squared;

	double *particle_dx_so_far;
	double *particle_dy_so_far;

	//+tabulated
	long precision;
	double *r2_f_values;
	double r2_max;
	double r2_min;
	int use_tabulated;

	FILE *moviefile;
};

extern struct global_struct global;

#endif /* globaldata_h */
