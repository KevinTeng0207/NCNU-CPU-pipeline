// define the data structure utilized in the simulator 

#ifndef MACHINE_H
#define MACHINE_H

#define SINGLE		1
#define	MULTI		2
#define	PIPELINE	3

/*
Data structures for 
1. Instruction trace: 
   With 2 fields to record (a) memory address, and (b) the instruction
2. Hardware architecture 
   (a) Memory 
   (b) Register 
3. Restrictions of this architecture 
   (a) instruction and data are placed at different memories. 
   (b) instruction and data memories share the smae memory address space 
       0 ~ instruction memory size: for instruction memories 
       inst_mem_size ~ (inst_mem_size + data_mem_size): for data memories 
   (c) inst and data memory has the same memory access latency  
*/

int	DATAPATH_TYPE; 		// indicate the type of the datapath 
long	CPU_CYCLE_TIME; 	// in pico second
long	MEMORY_ACC_TIME;	// memory access time in pico seconds
long	REGISTER_ACC_TIME;	// register access time in pico seconds  
long	ALU_TIME; 		// ALU execution time in pico seconds
long	PC;			// program counter  
long	num_inst_mem_word;	// number of words in the instruction memory 
long	num_data_mem_word;	// number of words in the data memory 
long	num_reg_mem_word;

int add_cycles;
int sub_cycles;
int addi_cycles;
int subi_cycles;
int or_cycles;
int and_cycles;
int sll_cycles;
int srl_cycles;
int lw_cycles;
int sw_cycles;
int lbu_cycles;
int sbu_cycles;
int j_cycles;
int beq_cycles;
int bne_cycles;
int pipe_stages;


typedef struct memory{
  long	mem_addr; /* the memory address, in every 4 bytes */
  char	data[128]; 	  /* data stored in the correspondent memory address space */
  char	label[32];/* only be useful for insturciton memory */ 
}MEMORY; 

int max_inst;
int max_data;
int max_reg;

typedef struct file_memory{
  long mem_addr;
  long mem_data;
  char data[128];
}FILE_MEMORY;

MEMORY inst_memory[2048];    //只有指令用MEMORY放
FILE_MEMORY data_memory[2048];   //DATA和REG都用FILE_MEMORY
FILE_MEMORY reg_memory[2048];

long	Register[64];	/* maximum number of the register is set to 64*/

void setup_hardware_cfg(char *HWCfg);

void init_CPU_cycle_time(void); 

void setup_data_memory(char *data_mem_content_file); 

void setup_inst_memory(char *inst_mem_content_file); 

void init_program_counter(void);

#endif 
