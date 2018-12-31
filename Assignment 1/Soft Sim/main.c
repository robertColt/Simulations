//
//  main.c
//  softsim
//
//  Created by András Libál on 7/18/18.
//  Copyright © 2018 András Libál. All rights reserved.
//
// first problem: tim,e crystal simulation

#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

#include <stdio.h>
#include "initializer.h"
#include "running.h"
#include <time.h>

int main(int argc, const char * argv[]) {
    
    printf("Soft Matter Simulator\n");
   
    init_simulation();
    init_simulation_box();
    init_particles();
    init_files();
    
	clock_t timeElapsed;
	timeElapsed = clock();

    run_simulation();

	timeElapsed = clock() - timeElapsed;
	double time_taken = ((double)timeElapsed) / CLOCKS_PER_SEC; //time taken by the simulation

	printf("Simulation took %f seconds !\n", time_taken); //takes about 437s 7.3 min  for total_time=50000

	WINPAUSE;
    
    return 0;
}
