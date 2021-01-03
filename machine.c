#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "machine.h"

#define	MAX_LINE_LENGTH	1024

/*  Function of this file 
	1. initialize the hardware configuration, including 
	a. the size and amount of each resource 
	b. the access latency of each resource 
	c. the cycle time of the target datapath 
 */


void setup_hardware_cfg(char *HWCfg){
	FILE *fp = fopen(HWCfg, "r"); 
	char line[MAX_LINE_LENGTH]; 
	char *token; 
	const char delimit[4] = "\t@"; 
	char attr_datapath_type[] = "-datapath_type"; 
	char attr_data_mem_size[] = "-data_mem_size";
	char attr_inst_mem_size[] = "-inst_mem_size"; 
	char attr_reg_size[] = "-reg_size"; 
	char attr_op_mem_lat[] = "-op_mem_lat";
	char attr_op_ALU_lat[] = "-op_ALU_lat";
	char attr_op_reg_lat[] = "-op_reg_lat";  
	int	i, j, k; 

	/* initialize inst/data memory (if any) and register file*/

	fprintf(stderr, "opening file %s\n", HWCfg);

	if(!fp){
		fprintf(stderr, "can't open file %s\n", HWCfg);
		exit(0);
		return; 
	}
	else {
		/* main loop for parsing the HW configuration file */
		while(fgets(line, MAX_LINE_LENGTH, fp)){
			if(line[0] == '#'){	/* it is a comment, no need to process */
				continue; 
			}
			else if(line[0] == '-'){	/* setting hardware configurations accoding to their attributes */
				token = strtok(line, "\t");
				if(strcmp(line, attr_datapath_type) == 0 ) { /* setting datapath type */
					token = strtok(NULL, "\t");
					if(strcmp(token, "SINGLE") == 0)
						DATAPATH_TYPE = SINGLE; 
					else if(strcmp(token, "MULTI") == 0)
						{
							DATAPATH_TYPE = MULTI; 
						}
					else if(strcmp(token, "PIPELINE") == 0)
						{
							token = strtok(NULL, "\t");
							pipe_stages = atoi(token);
							printf("pipe stages: %d\n", pipe_stages);
							DATAPATH_TYPE = PIPELINE; 
						}
					else{
						printf("Unidenfitied datapath type, please check \n");
						exit(0);
					}
					continue; 
				}
				/* finish the parsing of the hw.cfg file and setup the hardware configuration related parameters */
				else if(strcmp(line, attr_data_mem_size) == 0){ /* setting data memory size */
					token = strtok(NULL, "\t");
					i = atoi(token);	/* get data memory size (in bytes) */
					if(i==0){
						printf("Data memory size cannot be zero! \n");
						exit(0);
					}
					else if((i%4)!=0) {
						printf("Data memory size must be the multiple of the size of a word (32-bit, 4 bytes) \n");
						exit(0);
					}
					else { /* setup the size of */
						num_data_mem_word = i / 4; 
						printf("data mem word: %ld\n", num_data_mem_word);
						for(j = 0; j < i; j++)
						{
							data_memory[j].mem_data = ~0;
						}
					}
					continue; 
				}
				else if(strcmp(line, attr_inst_mem_size) == 0){ /* setting inst memory size */
					token = strtok(NULL, "\t");
					i = atoi(token);	/* get inst memory size (in bytes) */
					if(i==0){
						printf("Inst memory size cannot be zero! \n");
						exit(0);
					}
					else if((i%4)!=0) {
						printf("Inst memory size must be the multiple of the size of a word (32-bit, 4 bytes) \n");
						exit(0);
					}
					else { /* setup the size of */
						num_inst_mem_word = i / 4; 
						printf("inst mem word: %ld\n", num_inst_mem_word);
						for(j = 0; j < i; j++)
						{
							strcpy(inst_memory[j].data, "UNUSE");
							strcpy(inst_memory[j].label, "NULL");
						}
					}
					continue; 
				}
				else if(strcmp(line, attr_reg_size) == 0){	/* setting register size*/
					token = strtok(NULL, "\t");
					i = atoi(token);

					num_reg_mem_word = i;
					printf("reg mem word: %ld\n", num_reg_mem_word);
					for(j = 0; j < i * 4; j++)
					{
						reg_memory[j].mem_data = ~0;
					}
					continue; 
				}
				else if(strcmp(line, attr_op_mem_lat) == 0){	/* setting the memory access latency */
					token = strtok(NULL, "\t");
					i = atoi(token);

					MEMORY_ACC_TIME = i;
					
					printf("mem acc time: %ld\n", MEMORY_ACC_TIME);
					continue; 
				}
				else if(strcmp(line, attr_op_ALU_lat) ==0){	/* setting the ALU access latency */
					token = strtok(NULL, "\t");
					i = atoi(token);

					ALU_TIME = i;
					
					printf("alu time: %ld\n", ALU_TIME);
					continue; 
				}
				else if(strcmp(line, attr_op_reg_lat) == 0){	/* setting the register file access latency*/
					token = strtok(NULL, "\t");
					i = atoi(token);

					REGISTER_ACC_TIME = i;
					
					printf("reg acc time: %ld\n", REGISTER_ACC_TIME);
					continue; 
				}
				else{
					printf("Unidenfied hardware configuration attribute, please check \n");
					exit(0);
				}
			}
			else{
				printf("UNDEFINED HARDWARE CONFIGURATON FORMAT! \n");
				exit(0); 
			}
		} 
	}
}

void init_CPU_cycle_time(void){
	int	i, j, k; 
	
	/* HOMEWORK: finish the CPU cycle time calculation for multi-cycle datapath */
	switch(DATAPATH_TYPE){
		case SINGLE://
			CPU_CYCLE_TIME = MEMORwq	Y_ACC_TIME * 2 + REGISTER_ACC_TIME * 2 + ALU_TIME;  
			break; 
		case MULTI: //�H5�Ӹ̳̪�������
			if (MEMORY_ACC_TIME > REGISTER_ACC_TIME)
			{
				if (MEMORY_ACC_TIME > ALU_TIME)
					CPU_CYCLE_TIME = MEMORY_ACC_TIME;
				else
					CPU_CYCLE_TIME = ALU_TIME;
			}
			else
			{
				if (REGISTER_ACC_TIME > ALU_TIME)
					CPU_CYCLE_TIME = REGISTER_ACC_TIME;
				else
					CPU_CYCLE_TIME = ALU_TIME;
			}
			break;
		case PIPELINE://5stage pipeline

			break; 
	}
}

void setup_data_memory(char *data_mem_content_file){
	FILE* pf_dm = fopen(data_mem_content_file, "r");
	char buffer[128];
	while(fscanf(pf_dm, "%s", buffer) != EOF)
	{
		if(strcmp(buffer, "-data_memory") == 0)
		{
			long mem_addr, mem_data;
			while(fscanf(pf_dm, "%ld%ld", &mem_addr, &mem_data) == 2)
			{
				printf("mem addr: %ld data: %ld\n", mem_addr, mem_data);
				data_memory[mem_addr].mem_addr = mem_addr;
				data_memory[mem_addr].mem_data = mem_data;
			}
		}

	}
	fclose(pf_dm);

	pf_dm = fopen(data_mem_content_file, "r");
	while(fscanf(pf_dm, "%s", buffer) != EOF)
	{
		if(strcmp(buffer, "-register_file") == 0)
		{
			char s_reg_num[8];
			long reg_data;
			while(fscanf(pf_dm, "%s%ld", s_reg_num, &reg_data) == 2)
			{
				long reg_num = atoi(s_reg_num + 1);
				printf("reg num: %ld data: %ld\n", reg_num, reg_data);
				reg_memory[reg_num * 4].mem_addr = reg_num * 4;
				reg_memory[reg_num * 4].mem_data = reg_data;
			}
		}
	}
	fclose(pf_dm);
}

void setup_inst_memory(char *inst_mem_content_file){
	FILE* pf_im = fopen(inst_mem_content_file, "r");
	char buffer[128];
	while(fgets(buffer, 128, pf_im))
	{
		if('0' <= buffer[0] && buffer[0] <= '9')
		{
			char* s_mem_addr = strtok(buffer, "\t");
			long mem_addr = atoi(s_mem_addr);
			printf("mem addr: %ld ", mem_addr);

			char* label;
			char* inst;
			if(strchr(buffer + strlen(s_mem_addr) + 1, ':'))
			{
				label = strtok(NULL, ":");
				printf("label: %s ", label);
				inst = buffer + strlen(s_mem_addr) + 1 + strlen(label) + 1;
				strcpy(inst_memory[mem_addr].label, label);
			}
			else
			{
				inst = buffer + strlen(s_mem_addr) + 1;
			}
			printf("inst: %s\n", inst);

			inst_memory[mem_addr].mem_addr = mem_addr;
			strcpy(inst_memory[mem_addr].data, inst);
            max_inst++; 
		}
	}
}

void init_program_counter(void){
	long i;
	for(i = 0; i < num_inst_mem_word; i++)
	{
		if(strcmp(inst_memory[i * 4].label, "Start") == 0)
		{
			PC = i * 4;
			printf("PC: %ld\n", PC);
			break;
		}
	}
	return;
}
