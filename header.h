//ECE586.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "instruction.h"

#define SIZ 32
#define OPLEN 6
#define REGLEN 5
#define IMMLEN 16 
#define TRUE 1
#define FALSE 0


void initialize_wrong_path();
void initialize_array(struct instruction *an_instruction[]);
int process_program(char myString[], FILE *fptr, int linect, int progstop,int num[], struct instruction *an_instruction[]);
void print_inst(struct instruction);
struct instruction fill_instruction(int bin_inst[], int len);
int hex2bin(char myString[10], int num[SIZ]);
int bin2dec(int array[], int len);
int bin2dec_2sComp(int array[], int len);
int mem_img2bin(char myString[10], int num[SIZ]);
void opSwitch(int decOp);
void prt32(int num[SIZ]);
int extract_opcode(int bin_inst[], int opLen);
int extract_rs(int bin_inst[]);
int extract_rt(int bin_inst[]);
int extract_rd(int bin_inst[]);
int extract_immediate(int bin_inst[]);
int extract_type(int bin_inst[]);
int extract_mem(int bin_inst[]);
void print_program(int linect);
const char *extract_opcode_str(int opcode);
void parse(struct instruction *an_instruction, int num[SIZ], int serial);
void fill_mem(int data_seg[], int num[SIZ], int datact);
void print_mem(int data_seg[], int linect);

void initialize_reg(int gpReg[32]);
int process_code(struct instruction *an_instruction, int gpReg[]);
int ADD(int rs, int rt);
int ADDI(int rs, int imm);
int SUB(int rs, int rt);
int SUBI(int rs, int imm);
int MUL(int rs, int rt);
int MULI(int rs, int imm);
int OR(int rs, int rt);
int ORI(int rs, int imm);
int AND(int rs, int rt);
int ANDI(int rs, int imm);
int XOR(int rs, int rt);
int XORI(int rs, int imm);


int LDW(int rt, int effective_address, int gpReg[], int progstop);
int STW(int rt, int effective_address, int gpReg[]);
void print_reg(int gpReg[]);

void flush_pipeline();

void copy_to_pipe(struct instruction *an_instruction, struct instruction *other_instruction);
void display_struct(struct instruction *an_instruction);

void fetch_instruction(int *pc);
void decode_instruction();
void execute_instruction(int *pc);
void memory_access(int *pc);
void write_back();


int stagebusy(int execution_stage);
void cleanup_pipe();
void clear_wrong_path_buffer();

int check_for_halt();
void initialize_pipe();

