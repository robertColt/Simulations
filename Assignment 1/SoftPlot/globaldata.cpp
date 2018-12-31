//
//  globaldata.c
//  softplot
//
//  Created by András Libál on 8/7/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

#include "globaldata.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct global_struct global;

void initialize_global_data()
{

    global.Windowsize_x = 1280;
    global.Windowsize_y = 800;

    global.SX = 60.0;
    global.SY = 60.0;
    
    global.zoom_x0 = 0.0;
    global.zoom_x1 = global.SX;
    global.zoom_deltax = global.zoom_x1 - global.zoom_x0;
    
    global.zoom_y0 = 0.0;
    global.zoom_y1 = global.SY;
    global.zoom_deltay = global.zoom_y1 - global.zoom_y0;
    
    global.radius_vertex = 0.01;
    global.radius_particle = 0.005;
    
    strcpy(global.moviefilename,"test.mvi");
    
    global.N_frames = 0;
    global.current_frame = 0;
    
    global.trajectories_on = 0;
    global.trace_length = 10;
}

//try to open the movie file, exit if not found with an error message

void open_movie_file()
{

global.moviefile = fopen(global.moviefilename,"rb");
if (global.moviefile == NULL)
    {
    printf("Cannot find/open movie file: %s\n",global.moviefilename);
    exit(1);
    }
    
}

void dummy_read_cmovie_frame()
{
int intholder;
float floatholder;
int i;

//read one frame but don't store the values (dummy read)
fread(&intholder,sizeof(int),1,global.moviefile);
if (!feof(global.moviefile))
    {
    global.N_objects = intholder;
    //printf("N_particles = %d\n",N_particles);
    fread(&intholder,sizeof(int),1,global.moviefile);
    //printf("Frame = %d\n",intholder);

    for(i=0;i<global.N_objects;i++)
        {
        //intholder = global_variables.particles[i].color;
        fread(&intholder,sizeof(int),1,global.moviefile);
        //intholder = i;//ID
        fread(&intholder,sizeof(int),1,global.moviefile);
        //floatholder = (float)global_variables.particles[i].x;
        fread(&floatholder,sizeof(float),1,global.moviefile);
        //floatholder = (float)global_variables.particles[i].y;
        fread(&floatholder,sizeof(float),1,global.moviefile);
        //floatholder = 1.0;//cum_disp, cmovie format
        fread(&floatholder,sizeof(float),1,global.moviefile);
        }
    }

}

void read_cmovie_frame()
{
int intholder;
float floatholder;
int i;

fread(&intholder,sizeof(int),1,global.moviefile);
if (!feof(global.moviefile))
    {
    global.N_objects = intholder;
    //printf("N_objects = %d\n",N_objects);
    fread(&intholder,sizeof(int),1,global.moviefile);
    //printf("Frame = %d\n",intholder);

    for(i=0;i<global.N_objects;i++)
        {
        //read in the color
        fread(&intholder,sizeof(int),1,global.moviefile);
        global.objects[global.N_frames][i].color = intholder;
        //read in the ID
        fread(&intholder,sizeof(int),1,global.moviefile);
        //read in the x coordinate
        fread(&floatholder,sizeof(float),1, global.moviefile);
        global.objects[global.N_frames][i].x = floatholder;
        //read in the y coordinate
        fread(&floatholder,sizeof(float),1, global.moviefile);
        global.objects[global.N_frames][i].y = floatholder;
        //read in extra data that is unused (legacy)
        fread(&floatholder,sizeof(float),1,global.moviefile);
        global.objects[global.N_frames][i].R = floatholder;
        }
    }
}

void read_moviefile_data()
{
int i;

//open the file, if it cannot be found, exit with error
open_movie_file();

//pre-scan the file to find out how many frames/partciles we have
//this could be modified to be capable of finding the frames that are complete
printf("Pre-scanning movie file %s\n",global.moviefilename);
global.N_frames = 0;
while (!feof(global.moviefile))
    {
    dummy_read_cmovie_frame();
    if (!feof(global.moviefile)) global.N_frames++;
    }

printf("Movie has %d frames\n",global.N_frames);
printf("Movie has %d partciles in a frame\n",global.N_objects);

//allocate space for the data

global.objects = (struct object_struct **) malloc(global.N_frames*sizeof(struct object_struct *));
for(i=0;i<global.N_frames;i++)
    global.objects[i] = (struct object_struct *)malloc(global.N_objects*sizeof(struct object_struct));



//re-open the movie file
fclose(global.moviefile);
global.moviefile = fopen(global.moviefilename,"rb");

//read the actual data
//reads in all the data from the movie file
//this usually fits in the memory - if not this needs a rewrite
global.N_frames = 0;
while (!feof(global.moviefile))
    {
    read_cmovie_frame();
    if (!feof(global.moviefile)) global.N_frames++;
    }

}


void write_frame_data_to_file()
{
FILE *outfile[6];
int i;
int file_i;
int color;
int frame;

frame = 35;


outfile[0] = fopen("hex80k_f35_verttype0.txt","wt");
outfile[1] = fopen("hex80k_f35_verttype1.txt","wt");
outfile[2] = fopen("hex80k_f35_verttype2.txt","wt");
outfile[3] = fopen("hex80k_f35_verttype3.txt","wt");
outfile[4] = fopen("hex80k_f35_verttype4.txt","wt");
outfile[5] = fopen("hex80k_f35_verttypegs.txt","wt");

for(i=0;i<global.N_objects;i++)
    {
    color = global.objects[frame][i].color;
    
    if (color==10) color = 9;
    if ((color>=4)&&(color<=9))
        {
        file_i = color - 4;
        fprintf(outfile[file_i],"%f %f\n",global.objects[frame][i].x,global.objects[frame][i].y);
        }
    }

for(i=0;i<=5;i++)
    fclose(outfile[i]);

}





