
//  main.c
//  ising_model


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define L_max 100

int L_spin; //N = L x L side of the lattice
int N_spin; // number of spins
int spin[L_max][L_max];

int t; //time
double T; //Temperature
long int N_steps_per_T; // this many MC steps for each Temperature

long int step;
double M_avg, M2_avg, E_avg, E2_avg;
double mTotalAvg = 0.0;
double m2TotalAvg = 0.0;
double eTotalAvg = 0.0;
double e2TotalAvg = 0.0;
long int N_rej; //how many rejections happened

FILE *outfile;
FILE *resultfile;
FILE *moviefile;

void statistics_initialization();
void spin_initialization();
void write_cmovie();
void write_out_result();

void statistics_initialization()
{
	M_avg = 0.0;
	M2_avg = 0.0;
	E_avg = 0.0;
	E2_avg = 0.0;
	N_rej = 0;
}

void spin_initialization()
{
	int i, j;
	int pos, neg;

	pos = 0;
	neg = 0;

	for (i = 0; i < L_spin; i++)
		for (j = 0; j < L_spin; j++)
		{
			//all spins pointing in random directions
			//(good for high temperatures)

			if ((rand() / (RAND_MAX + 1.0)) < 0.5)
			{
				spin[i][j] = 1;
				pos++;
			}
			else
			{
				spin[i][j] = -1;
				neg++;
			}

			/*uniform initialization
			all soins pointing in one direction
			(good for low temperatures)*/
			if (T <=2) {
				spin[i][j] = +1;
			}
				
		}
	printf("Initialization complete\n");
	printf("pos = %d neg = %d\n", pos, neg);

	/*
	if (T==0.1)
	for(i=0;i<L_spin;i++)
	for(j=0;j<L_spin;j++)
	if (spin[i][j]==1) fprintf(outfile,"%d %d\n",i,j);
	*/
}

//how much the energy would change if I flipped this spin
int Energy_function_for_one_spin(int fi, int fj)
{
	int ni, nj; //neighbor,we have 4 neighbors in total
	int i;

	int delta = 0;

	for (i = 0; i < 4; i++)
	{

		switch (i)
		{
		case 0: {ni = fi - 1; nj = fj; break; } // left
		case 1: {ni = fi + 1; nj = fj; break; } // right
		case 2: {ni = fi;     nj = fj + 1; break; } //up
		case 3: {ni = fi;     nj = fj - 1; break; } //down
		default: {
			ni = fi; nj = fj;
			printf("You should never get to this line!!!\n");
		}
		}

		//PBC periodic bopundary conditions fold-back
		if (ni < 0) ni = ni + L_spin;
		if (ni >= L_spin) ni = ni - L_spin;
		if (nj < 0) nj = nj + L_spin;
		if (nj >= L_spin) nj = nj - L_spin;

		//change in the energy
		delta = delta - spin[fi][fj] * spin[ni][nj];

	}
	//delta is how much the energy would chage if I flipped spin
	//on the position fi,fj in the lattice

	return delta;
}

int delta_E(int fi, int fj)
{
	int dE;

	dE = -2 * Energy_function_for_one_spin(fi, fj);

	return dE;
}


//flips spin (on the position: to_be_flipped_i,to_be_flipped_j)
void flip_spin(to_be_flipped_i, to_be_flipped_j)
{
	spin[to_be_flipped_i][to_be_flipped_j] = -spin[to_be_flipped_i][to_be_flipped_j];
}

void calculate_statistics()
{

	double M, M2, E, E2;
	int i, j;

	M = 0.0;//magnetization
	E = 0.0;//energy

	for (i = 0; i < L_spin; i++)
		for (j = 0; j < L_spin; j++)
		{
			M = M + (double)spin[i][j];
			E = E + (double)Energy_function_for_one_spin(i, j);
		}

	//square of M and E
	M2 = M * M;
	E2 = E * E;

	//we have done a number of steps
	//echo to the user and to the file
	if (step % 1000 == 0)
	{
		printf("t= %d M = %lf\n", t, M / N_spin);
		//fprintf(outfile, "%d %lf\n", t, M / (double)N_spin);
	}

	if (step < 500000) {
		return;
	}

	int newstep = step - 500000;

	M_avg = M_avg * ((double)(newstep) / (double)(newstep + 1.0)) + M / (double)(newstep + 1.0);
	//M_avg += M / (double)N_steps_per_T;

	M2_avg = M2_avg * ((double)(newstep) / (double)(newstep + 1.0)) + M2 / (double)(newstep + 1.0);
	E_avg = E_avg * ((double)(newstep) / (double)(newstep + 1.0)) + E / (double)(newstep + 1.0);
	E2_avg = E2_avg * ((double)(newstep) / (double)(newstep + 1.0)) + E2 / (double)(newstep + 1.0);

}

void write_cmovie_text(void);

void run_for_fixed_T()
{
	int to_be_flipped_i, to_be_flipped_j;
	int dE;
	double p, r;

	for (step = 0; step < N_steps_per_T; step++)
	{
		//pick the spin at random
		to_be_flipped_i = (int)floor((double)L_spin * rand() / (RAND_MAX + 1.0));
		to_be_flipped_j = (int)floor((double)L_spin * rand() / (RAND_MAX + 1.0));

		//calculate the delta E that this spin flip will cause
		dE = delta_E(to_be_flipped_i, to_be_flipped_j);

		//printf("dE = %d ",dE);

		//Metropolis MC rejection
		if (dE <= 0) {

			//if the energy is lowered then ALWAYS make the step
			flip_spin(to_be_flipped_i, to_be_flipped_j);
			//printf("sure\n");
		}

		else {
			//the energy is actually raised by this flip
			p = exp(-(double)dE / (double)T); //between (0..1)

			r = rand() / (RAND_MAX + 1.0); //random number from 0..1

			if (r < p) flip_spin(to_be_flipped_i, to_be_flipped_j);
			//this actually does the flip
			else N_rej++;//otherwise I reject the move
		}

		//take the SAME OLD state again if there was no spin flip
		calculate_statistics();
		if (t % 10000 == 0) write_cmovie_text();
		t++;

	}

}

void write_out_result()
{
	M_avg = M_avg / N_spin;

	printf("T = %lf M = %lf Rej = %ld\n", T, M_avg, N_rej);

}

void write_cmovie_text() {
	int i, j;
	for (i = 0; i < L_spin; i++)
	{
		for (j = 0; j < L_spin; j++)
		{
			if (spin[i][j] == -1) {
				fprintf(moviefile, "bo ");
			}
			else {
				fprintf(moviefile, "ro ");
			}
		}
	}
	fprintf(moviefile, "\n");
}

void write_cmovie()
{
	int i, j;
	float floatholder;
	int intholder;

	intholder = N_spin;
	fwrite(&intholder, sizeof(int), 1, moviefile);

	intholder = t;
	fwrite(&intholder, sizeof(int), 1, moviefile);

	for (i = 0; i < L_spin; i++)
		for (j = 0; j < L_spin; j++)
		{
			if (spin[i][j] == -1) intholder = 2; //color of spin
			else intholder = 3;
			fwrite(&intholder, sizeof(int), 1, moviefile);

			intholder = i*L_spin + j;            //spin ID
			fwrite(&intholder, sizeof(int), 1, moviefile);

			floatholder = (float)i;
			fwrite(&floatholder, sizeof(float), 1, moviefile);

			floatholder = (float)j;
			fwrite(&floatholder, sizeof(float), 1, moviefile);

			floatholder = 1.0;//cum_disp, cmovie format
			fwrite(&floatholder, sizeof(float), 1, moviefile);
		}
}


#ifdef _WIN32
#define WINPAUSE system("pause")
#endif

int main(int argc, const char * argv[])
{
	printf("Ising model simulation\n");

	outfile = fopen("ising_data_T21.dat", "wt");
	moviefile = fopen("ising_movie_tc", "wt");

	L_spin = 20;
	N_spin = L_spin * L_spin;

	N_steps_per_T = 1000000;

	fprintf(moviefile, "%d\n", L_spin);
	fprintf(moviefile, "%d\n", (N_steps_per_T / 10000));

	T = 2.11; // y fixed temperature 

	resultfile = fopen("ising_data", "at");
	//I just stay at a fixed temperature
	//while(T<=1.0)
	//{

	int runs = 1;
	for (int i = 0; i < runs; i++) {
		srand((int)time(NULL));
		t = 0;

		spin_initialization();
		statistics_initialization();
		run_for_fixed_T();
		write_out_result();

		mTotalAvg += M_avg;
		m2TotalAvg += M2_avg;
		eTotalAvg += E_avg;
		e2TotalAvg += E2_avg;
	}

	mTotalAvg /= runs;
	m2TotalAvg /= runs;
	eTotalAvg /= runs;
	e2TotalAvg /= runs;

	double cv = eTotalAvg * eTotalAvg - e2TotalAvg;
	double lambda = mTotalAvg * mTotalAvg - m2TotalAvg;

	printf("\nL(%d) : T(%f) : M(%f) : M2(%f) : E(%f) : E2(%f) : CV(%f) : LAMBDA(%f)",
		L_spin, T, mTotalAvg, m2TotalAvg, eTotalAvg, e2TotalAvg, cv, lambda);

	fprintf(resultfile, "\nL(%d) : T(%f) : M(%f) : M2(%f) : E(%f) : E2(%f) : CV(%f) : LAMBDA(%f)",
		L_spin, T, mTotalAvg, m2TotalAvg, eTotalAvg, e2TotalAvg, cv, lambda);

	//T = T + 0.1;
	//}

	fclose(outfile);
	fclose(moviefile);

	WINPAUSE;
	return 0;
}
