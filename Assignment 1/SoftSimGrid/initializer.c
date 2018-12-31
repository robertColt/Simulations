//
//  initializer.c
//  softsim
//
//  Created by András Libál on 7/18/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

#include "initializer.h"
#include "globaldata.h"
#include <math.h>

//reads the param,eter file
//adjusts the parameters accordingly
void read_init_file(char *filename)
{
FILE *infile;
char instring[255];
int linenum;

infile = fopen(filename,"rt");
if (infile==NULL)
    {
    printf("Failed to open parameter file %s\n",filename);
    fflush(stdout);
    exit(1);
    }
printf("Initializing data from the parameterfile\n");
printf("Opened parameter file %s\n",filename);
linenum = 1;
while (!feof(infile))
    {
    fgets(instring,255,infile);
    //printf("%d:%s\n",linenum,instring);
    linenum++;
    //tokenize, recognize tokens, act upon them

    }
}

//init the common variables in the simulation
void init_simulation()
{
printf("Post Initializing data\n");

//timestep length needs to be set
global.dt = 0.001;
printf("Timestep (dt) = %lf\n",global.dt);

//the dirving force for the pinning sites needs to be set
global.pinning_driving_force = 0.5;
printf("Pinning site driving force = %lf\n",global.pinning_driving_force);

//radius of pinning sites
global.pinningsite_setradius = 0.5;
printf("Pinning site radius set to = %.2lf\n",global.pinningsite_setradius);

//the dirving force for the pinning sites needs to be set
global.particle_driving_force = 0.5;
printf("Particle driving force = %lf\n",global.pinning_driving_force);

global.partile_particle_screening_length = 4.0;
global.partile_particle_screening_wavevector =  1.0/global.partile_particle_screening_length;
printf("Particle particle screening lenght = %lf\n",global.partile_particle_screening_length);
printf("Particle particle screening wavevector = %lf\n",global.partile_particle_screening_wavevector);

global.Verlet_cutoff_distance = 1.5 * global.partile_particle_screening_length;
global.Verlet_cutoff_distance_squared = global.Verlet_cutoff_distance * global.Verlet_cutoff_distance;
global.Verlet_intershell_squared = global.Verlet_cutoff_distance - global.partile_particle_screening_length;
global.Verlet_intershell_squared = global.Verlet_intershell_squared / 2.0;
global.Verlet_intershell_squared *= global.Verlet_intershell_squared;

printf("Verlet cutoff distance = %.2lf\n",global.Verlet_cutoff_distance);
printf("Verlet cutoff distance squared = %.2lf\n",global.Verlet_cutoff_distance_squared);
printf("Half of Verlet intershell distance = %.2lf\n",sqrt(global.Verlet_intershell_squared));
printf("Half of Verlet intershell distance squared = %.2lf\n",global.Verlet_intershell_squared);

//zero everything so rebuild Verlet can find this the first time
global.Verletlisti = NULL;
global.Verletlistj = NULL;
global.N_Verlet = 0;
global.N_Verlet_max = 0;

//zero everythiong so rebuild_pinning_grid can find this the first time
global.pinningsite_grid = NULL;
global.Nx_pinningsite_grid = 0;
global.Ny_pinningsite_grid = 0;

//initialize the grid
}

void init_grid() {
	
	double c = global.SX / global.Verlet_cutoff_distance;
	int d = (int)c;
	global.N_grid_cells_x = (int)(global.SX / global.Verlet_cutoff_distance);
	global.N_grid_cells_y = (int)(global.SY / global.Verlet_cutoff_distance);

	printf("SX : %f, SY:%f, cutoff: %f, gx:%d, gy:%d\n",
		global.SX, global.SY, global.Verlet_cutoff_distance,
		global.N_grid_cells_x, d);

	global.grid= malloc(global.N_grid_cells_x * sizeof(node*));

	for (int i = 0; i < global.N_grid_cells_x; i++)
	{
		global.grid[i] = (node*)malloc(global.N_grid_cells_y * sizeof(node));
		//printf("Node.i = %dn", global.grid[i]);
	}

	for (int i = 0; i < global.N_grid_cells_x; i++)
	{
		for (int j = 0; j < global.N_grid_cells_y; j++)
		{
			global.grid[i][j].i = -1;
			global.grid[i][j].next = NULL;
			//_Bool isnull = (global.grid[i][j].next == NULL);
			//printf("%d,%d,%d,%d\n",i,j, (&global.grid[i][j])->i, isnull);
		}
	}

	flag.grid_built = 1;
	/*for (int i = 0; i < N; i++)
	{
		int* currentIntPtr = ptr[i];
		free(currentIntPtr);
	}*/
}


//init the simulation box
//this can be later modified
void init_simulation_box()
{
printf("Initializing the simulation box\n");
global.SX = 60.0;
global.SY = 60.0;
global.halfSX = global.SX/2.0;
global.halfSY = global.SY/2.0;

printf("SX = %lf SY = %lf\n",global.SX,global.SY);
}

void init_pinningsites()
{
global.N_pinningsites = 576;
global.pinningsite_x = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_y = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_fx = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_fy = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_color = (int *)malloc(global.N_pinningsites*sizeof(int));
global.pinningsite_direction_x = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_direction_y = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_dx_so_far = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_dy_so_far = (double *)malloc(global.N_pinningsites*sizeof(double));
global.pinningsite_R = (double *)malloc(global.N_pinningsites*sizeof(double));

//init_pinningsites_square_lattice();
init_pinningsites_triangular_lattice();
global.pinning_direction_change = 1;
}

void init_pinningsites_square_lattice()
{
int i,j,k;
int N_rows,N_columns;
double lattice_const;

N_rows = (int)sqrt((double)global.N_pinningsites);
N_columns = N_rows;

lattice_const = global.SX/(double)N_rows;
global.pinning_lattice_constant = lattice_const;

k=0;

for(i=0;i<N_rows;i++)
    for(j=0;j<N_columns;j++)
        {
        global.pinningsite_x[k] = (i + 0.5) * lattice_const;
        global.pinningsite_y[k] = (j + 0.5) * lattice_const;
        global.pinningsite_dx_so_far[k] = 0.0;
        global.pinningsite_dy_so_far[k] = 0.0;
        global.pinningsite_fx[k] = 0.0;
        global.pinningsite_fy[k] = 0.0;
        global.pinningsite_R[k] = global.pinningsite_setradius;
        k++;
        }

printf("pinningsites initialized\n");
printf("N_pinningsites = %d\n",k);
printf("Square lattice\nRows = %d\nColumns = %d\nLattice constant = %.2lf\n",N_rows,N_columns, lattice_const);
}

void init_pinningsites_triangular_lattice()
{
int i,j,k;
int N_rows,N_columns;
double lattice_const;

N_columns = (int)sqrt((double)global.N_pinningsites);
lattice_const = global.SX/(double)N_columns;
global.pinning_lattice_constant = lattice_const;

//keeping same number of rows and columns
N_rows = N_columns;
//if (N_rows%2==1) N_rows++;

/*
//rescaling rows rto nearest number for SY
N_rows = (int) floor(global.SY / (lattice_const * (sqrt(3)/2.0)));
printf("Rescaled row number to = %d\n",N_rows);
*/

global.SY = N_rows * lattice_const * (sqrt(3)/2.0);
printf("Rescaled SY to = %.2lf\n",global.SY);

k=0;

for(i=0;i<N_rows;i++)
    for(j=0;j<N_columns;j++)
        {
        global.pinningsite_x[k] = (i + 0.25 + 0.5*(j%2) ) * lattice_const;
        global.pinningsite_y[k] = (j + 0.25) * lattice_const * (sqrt(3)/2.0);
        global.pinningsite_dx_so_far[k] = 0.0;
        global.pinningsite_dy_so_far[k] = 0.0;
        global.pinningsite_fx[k] = 0.0;
        global.pinningsite_fy[k] = 0.0;
        global.pinningsite_R[k] = global.pinningsite_setradius;
        
        //set the color and direction of the pinningsite
        
        if (j%2==0) global.pinningsite_color[k] = 2 + i % 3;
        else global.pinningsite_color[k] = 2 + (i+2) % 3;
        
        //printf("%d %d %d\n",i,j,global.pinningsite_color[k]);
        
        //driection based on color
        switch (global.pinningsite_color[k]-2)
            {
            case 0: {
                    global.pinningsite_direction_x[k] = - 0.5;
                    global.pinningsite_direction_y[k] = - sqrt(3)/2.0;
                    break;
                    }
            case 1: {
                    global.pinningsite_direction_x[k] = 1.0;
                    global.pinningsite_direction_y[k] = 0.0;
                    break;
                    }
            case 2: {
                    global.pinningsite_direction_x[k] = - 0.5;
                    global.pinningsite_direction_y[k] = + sqrt(3)/2.0;
                    break;
                    }
            }
        k++;
        }

printf("Pinning sites initialized\n");
printf("N_pinningsites = %d\n",k);
printf("Triangular lattice\nRows = %d\nColumns = %d\nLattice constant = %.2lf\n",N_rows,N_columns, lattice_const);
}

//initializes space for the particles, direction, position, color etc.
void init_particles()
{
global.N_particles = 800;


global.particle_x = (double *)malloc(global.N_particles*sizeof(double));
global.particle_y = (double *)malloc(global.N_particles*sizeof(double));
global.particle_fx = (double *)malloc(global.N_particles*sizeof(double));
global.particle_fy = (double *)malloc(global.N_particles*sizeof(double));
global.particle_color = (int *)malloc(global.N_particles*sizeof(int));
global.particle_direction_x = (double *)malloc(global.N_particles*sizeof(double));
global.particle_direction_y = (double *)malloc(global.N_particles*sizeof(double));
global.particle_dx_so_far = (double *)malloc(global.N_particles*sizeof(double));
global.particle_dy_so_far = (double *)malloc(global.N_particles*sizeof(double));

//init_particles_square_lattice();
//init_particles_triangular_lattice();
init_particles_randomly();

printf("Particles initialized\n");
printf("N_particles = %d\n",global.N_particles);
}

//calculates the shortest distance between 2 points in a PBC configuration
//only used in the random deposition chekc which happens once at initialization
//so this is not so time crucial left the square root inside
double distance_folded_PBC(double x0,double y0,double x1,double y1)
{
double r;
double dx,dy;

dx = x1 - x0;
dy = y1 - y0;

//PBC fold back
//if any distance is lrger than half the box
//the copy in the neighboring box is closer
if (dx > global.halfSX) dx -=global.SX;
if (dx <= -global.halfSX) dx +=global.SX;
if (dy > global.halfSY) dx -=global.SY;
if (dy <= -global.halfSY) dx +=global.SY;

r = sqrt( dx*dx + dy*dy );

return r;
}

//places particles 
void init_particles_randomly()
{
int i,j;
double x_try,y_try;
double r_min;
double dr;
int overlap;
int N_trials;

r_min = 0.2;

for(i=0;i<global.N_particles;i++)
    {
    x_try = 0.0;
    y_try = 0.0;
    
    //check overlap with previous particles
    //assume there is overlap to get into the cycle
    overlap = 1;
    N_trials = 0;
    
    while ((overlap==1)&&(N_trials<global.N_particles))
        {
   
        //attempt to place the particle
        x_try = global.SX * rand()/(RAND_MAX+1.0);
        y_try = global.SY * rand()/(RAND_MAX+1.0);
 
        //assume this was good
        overlap = 0;
        
        for(j=0;j<i;j++)
            {
            //calculate distance
            dr = distance_folded_PBC(x_try, y_try, global.particle_x[j], global.particle_y[j]);
            if (dr < r_min)
                {
                overlap = 1; //found overlap
                N_trials++;
                break; //no need to check with other particles
                }
            }
        }
        
    if (N_trials==global.N_particles)
        {
        printf("Can't place particles randomly, (system too dense) quitting\n");
        exit(1);
        }
      
	global.particle_x[i] =  x_try;
	global.particle_y[i] =  y_try;
    global.particle_dx_so_far[i] = 0.0;
    global.particle_dy_so_far[i] = 0.0;
    global.particle_fx[i] = 0.0;
    global.particle_fy[i] = 0.0;
    
    if (rand()/(RAND_MAX+1.0)<0.5)
        {
        global.particle_direction_x[i] = - 1.0;
        global.particle_direction_y[i] = 0.0;
        global.particle_color[i] = 2;
        }
    else
        {
        global.particle_direction_x[i] = + 1.0;
        global.particle_direction_y[i] = 0.0;
        global.particle_color[i] = 3;
        }
    
    }

printf("Random arrangement of particles initialized\n");
printf("N_particles = %d\n",global.N_particles);

}

void init_particles_square_lattice()
{
int i,j,k;
int N_rows,N_columns;
double lattice_const;

N_rows = (int)sqrt((double)global.N_particles);
N_columns = N_rows;

lattice_const = global.SX/(double)N_rows;
global.pinning_lattice_constant = lattice_const;

k=0;

for(i=0;i<N_rows;i++)
    for(j=0;j<N_columns;j++)
        {
        global.particle_x[k] = (i + 0.5) * lattice_const;
        global.particle_y[k] = (j + 0.5) * lattice_const;
        global.particle_dx_so_far[k] = 0.0;
        global.particle_dy_so_far[k] = 0.0;
        global.particle_fx[k] = 0.0;
        global.particle_fy[k] = 0.0;
        k++;
        }
 
printf("Square lattice of particles initialized\n");
printf("N_particles = %d\n",k);
printf("Square lattice\nRows = %d\nColumns = %d\nLattice constant = %.2lf\n",N_rows,N_columns, lattice_const);
}

void init_particles_triangular_lattice()
{
int i,j,k;
int N_rows,N_columns;
double lattice_const;

N_columns = (int)sqrt((double)global.N_particles);
lattice_const = global.SX/(double)N_columns;
global.pinning_lattice_constant = lattice_const;

//keeping same number of rows and columns
N_rows = N_columns;
//if (N_rows%2==1) N_rows++;

/*
//rescaling rows rto nearest number for SY
N_rows = (int) floor(global.SY / (lattice_const * (sqrt(3)/2.0)));
printf("Rescaled row number to = %d\n",N_rows);
*/

global.SY = N_rows * lattice_const * (sqrt(3)/2.0);
global.halfSY = global.SY/2.0;
printf("Rescaled SY to = %.2lf\n",global.SY);

k=0;

for(i=0;i<N_rows;i++)
    for(j=0;j<N_columns;j++)
        {
        global.particle_x[k] = (i + 0.25 + 0.5*(j%2) ) * lattice_const;
        global.particle_y[k] = (j + 0.25) * lattice_const * (sqrt(3)/2.0);
        global.particle_dx_so_far[k] = 0.0;
        global.particle_dy_so_far[k] = 0.0;
        global.particle_fx[k] = 0.0;
        global.particle_fy[k] = 0.0;
        
        //set the color and direction of the particle
        
        if (j%2==0) global.particle_color[k] = 2 + i % 3;
        else global.particle_color[k] = 2 + (i+2) % 3;
        
        //printf("%d %d %d\n",i,j,global.particle_color[k]);
        
        //driection based on color
        switch (global.particle_color[k]-2)
            {
            case 0: {
                    global.particle_direction_x[k] = - 0.5;
                    global.particle_direction_y[k] = - sqrt(3)/2.0;
                    break;
                    }
            case 1: {
                    global.particle_direction_x[k] = 1.0;
                    global.particle_direction_y[k] = 0.0;
                    break;
                    }
            case 2: {
                    global.particle_direction_x[k] = - 0.5;
                    global.particle_direction_y[k] = + sqrt(3)/2.0;
                    break;
                    }
            }
        k++;
        }

printf("Particles initialized\n");
printf("N_particles = %d\n",k);
printf("Triangular lattice\nRows = %d\nColumns = %d\nLattice constant = %.2lf\n",N_rows,N_columns, lattice_const);

FILE *test;

test = fopen("testfile1.txt","wt");
for(i=0;i<global.N_particles;i++)
    if (global.particle_color[i]==0) fprintf(test,"%lf %lf\n",global.particle_x[i],global.particle_y[i]);
fclose(test);
test = fopen("testfile2.txt","wt");
for(i=0;i<global.N_particles;i++)
    if (global.particle_color[i]==1) fprintf(test,"%lf %lf\n",global.particle_x[i],global.particle_y[i]);
fclose(test);
test = fopen("testfile3.txt","wt");
for(i=0;i<global.N_particles;i++)
    if (global.particle_color[i]==2) fprintf(test,"%lf %lf\n",global.particle_x[i],global.particle_y[i]);
fclose(test);
}

void init_files()
{
global.moviefile = fopen("test.mvi","wb");
if (global.moviefile == NULL)
    {
    printf("Could not create/open movie file\n");
    exit(2);
    }
}


