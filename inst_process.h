/* define the data structure and function utilized in the funcational simulation of each instruction */

#ifndef	INST_PROCESS_H
#define	INST_PROCESS_H

/* 10001 ~ represents the instructions */

/* arithematic and logic operations */
#define	ADD	10001
#define	SUB	10002
#define	ADDI	10003
#define	SUBI	10004
#define	OR	10005
#define	AND	10006
#define	SLL	10007
#define	SRL	10008

/* load store operations */
#define	LW	11001
#define	SW	11002
#define	LBU	11003
#define	SBU	11004

/* control instructions */
#define	J	12001
#define	BEQ	12002
#define BNE	12003

/* functional simulation of each instruction */
void inst_func_simulation(char* inst); 

#endif 