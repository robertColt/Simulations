//
//  globaldata.h
//  softplot
//
//  Created by András Libál on 8/7/18.
//  Copyright © 2018 András Libál. All rights reserved.
//

#ifndef globaldata_h
#define globaldata_h

#include <stdio.h>

struct object_struct
    {
    int color;
    float x;
    float y;
    float R;
    };

struct global_struct
    {
    
    //OpenGl window size
    int Windowsize_x;           //window size in pixels x direction
    int Windowsize_y;           //window size in pixels y direction
    
    //System Size
    double SX;
    double SY;
    
    //Sytem Zoom in
    float zoom_x0,zoom_y0;         //lower left corner
    float zoom_x1,zoom_y1;         //upper right corner
    float zoom_deltax,zoom_deltay; //size of the zoomed area
    
    float radius_particle;
    float radius_vertex;
    
    char moviefilename[100];
    FILE *moviefile;
    
    int N_frames;
    int current_frame;
    
    int trajectories_on;
    int trace_length;

    int N_objects;
    
    //all objects in the movie file
    struct object_struct **objects;

    };

extern struct global_struct global;

void initialize_global_data(void);

void open_movie_file(void);
void dummy_read_cmovie_frame(void);
void read_moviefile_data(void);

void write_frame_data_to_file();

#endif /* globaldata_h */
