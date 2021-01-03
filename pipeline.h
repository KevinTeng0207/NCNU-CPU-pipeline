#ifndef	PIPELINE_H
#define	PIPELINE_H

/* R type */
#define	ADD	1
#define	SUB	1
#define	OR	1
#define	AND	1
/* I types */
#define	SLL	2
#define	SRL	2
#define	ADDI	2
#define	SUBI	2
#define	LW	2
#define	SW	2
#define	LBU	2
#define	SBU	2
#define	BEQ	2
#define BNE	2
/* J type */
#define	J	3
typedef struct pipeline_memory{
  int rs;              /*  */
  int rt;
  int rd;
  int imm;
  char	op[128]; 	  /*  */
  char	inst[128];    /*  */ 
  int temp;
}PIPELINE_MEMORY; 

PIPELINE_MEMORY IFID;
PIPELINE_MEMORY IDEX;
PIPELINE_MEMORY EXMEM;
PIPELINE_MEMORY MEMWB;

void inst_fetch(char* inst);
void inst_decode();
void inst_execute();
void reg_update();
void mem_writeback();
void flush_pipeline();
bool if_hazard();
bool pipeline_null();
bool pipeline_next_stage_null(PIPELINE_MEMORY temp);
void print_pipeline_register_content();
void clear_pipeline_register_content(PIPELINE_MEMORY *temp);
void pass_register_content(PIPELINE_MEMORY *source,PIPELINE_MEMORY *destination);

#endif 