/*
   Funciton of this simulator 
   Performance and function simulation 
Input: hardware configuration file (hw.cfg) and instruction trace file (inst.trc)
Output: Performance file (perf_results.txt) and memory/register status dump (mem_reg.dump)

Execution flow: 
1. parse configuration file 
2. open the insturction file and start the performance simulation 
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
//#include <sys/time.h>

#include "main.h"
#include "sim.h"
#include "machine.h"
#include "inst_process.h"

MEMORY Inst_MEM[2048]; 	/* at most 2048 words --> 2^11*4=8K B size at most  */
MEMORY Data_MEM[2048];	/* at most 2048 words */

/* print out the results 
   1. CPU time 
   2. Datapath type 
   3. Performance results (in number of cycles) 
   4. contents of the instruction memory 
   5. contents of the data memory 
   6. contents of the registers 
 */
void print_results(FILE	*output_file){
	int	i, j, k; 

	/* CPU cycle time*/
	fprintf(output_file, "CPU cycle time:	%ld ps \n ", CPU_CYCLE_TIME);

	/* simulation type */
	switch(DATAPATH_TYPE){
		case SINGLE:
			fprintf(output_file, "Datapath type:	Single-Cycle Datapath \n"); 
			break; 

		case MULTI:
			fprintf(output_file, "Datapath type:	Multi-Cycle Datapath \n");
			break;  

		case PIPELINE:
			fprintf(output_file, "Datapath type:	Pipeline \n");
			break;  
	}
	/* total number of simulated cycles */
	fprintf(output_file, "Total # of CPU cycles:	%ld \n", sim_cycles);

	/*  print out the contents of the instruction memory to the output file */
	fprintf(output_file, "Instruction Memory Contents: \nMem_Addr\tLabel\tInstruction \n");
	for(i = 0; i < num_inst_mem_word; i++)
	{
		if(strcmp(inst_memory[i * 4].data, "UNUSE") == 0)
			continue;

		if(strcmp(inst_memory[i * 4].label, "NULL") == 0)
			fprintf(output_file, "%d\t\t\t%s\n", i * 4, inst_memory[i * 4].data);
		else
			fprintf(output_file, "%d\t\t%s\t%s\n", i * 4, inst_memory[i * 4].label, inst_memory[i * 4].data);
	}

	/*  print out the contents of the data memory to the output file */
	fprintf(output_file, "Data Memory Contents: \nMem_Addr\tData \n");
	for(i = 0; i < num_data_mem_word; i++)
	{
		if(data_memory[i * 4].mem_data != ~0)
		{
			fprintf(output_file, "%d\t\t%ld\n", i * 4, data_memory[i * 4].mem_data);
		}
	}

	/*  printout the contents of the register file */
	fprintf(output_file, "Register file contents: \nReg_ID\tData \n");
	for(i = 0; i < num_reg_mem_word; i++)
	{
		if(reg_memory[i * 4].mem_data != ~0)
		{
			fprintf(output_file, "%d\t%ld\n", i, reg_memory[i * 4].mem_data);
		}
	}
}

int main(int argc, char* argv[]){
	int	i, j, k; 
	char	input_string[64]; 
	char	output_file[128];
	FILE	*output; 

	if(argc != 5){
		printf("usage: NCNU_CPU_SIM HW_Config.cfg init_data_placement.txt assembely_code.txt perf_results.txt \n");
		exit(1);
	}

	/* setup the simulation mode: trace driven or execution driven */

	/* process the input hardware configuration, defined in machine.h*/
	setup_hardware_cfg(argv[1]);

	/* set up the cycle time, defined in machine.h*/
	init_CPU_cycle_time();  

	/* defined in machine.h
	   setup the initial data placement of the data memory */
	setup_data_memory(argv[2]);

	/* defined in machine.h
	   setup the instruction memory  */
	setup_inst_memory(argv[3]);

	/* init program counter, defined in machine.h */
	init_program_counter(); 

	/* set output file name*/
	strcpy(output_file, argv[4]);

	/* initialize the number of elapsed CPU cycle */
	sim_cycles = 0; 

	/* start the performance and functional simulation 
	   utilize a loop to go through the whole trace file 
	 */
	while(strcmp(inst_memory[PC].label, "Exit") != 0 ){
		inst_func_simulation(inst_memory[PC].data);
		/*  update the program counter accordingly */
	}

	output = fopen(output_file ,"w");
	print_results(output);
	fclose(output);
}

