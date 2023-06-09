#include "header.h"
int cycle=0;
int data_ct = 0;
int progstop = 0;//this is the last line of the program portion of the file. 
struct statistics {
	int itype;
	int rtype;
};

struct statistics mystats;

struct instruction program[100]; 
struct instruction pipeline[5];
int watch_out = 0;


//program counter:
int pc=0;

//data segment:
int data_seg[1000];
int data_red[1000];

//registers:
int gpReg[32];
int reg_ready[32];

//debug flag and forwarding flags:
int debug = -1;
int mem = -1;
int execution_delay = -1;

//jump control flags:
struct instruction wrong_path[5];
int wrong_path_start = 0;

int branch_encountered=0;
int flush =0;
//
int pipeline_head;  //head->always points to the function that's in the status of fetch; NEEDS to be initialized to zero.
int pipeline_tail;  //tail->always points to the function that's in the status of write_back;  NEEDS to be initialized to zero.


void shift(int opcode, int Rs, int Rt, int Rd, int Imm); 

int main(int argc, char* argv[]) {
	for(int i=0;i<argc;i++){
		
		if(! strcmp(argv[i], "bug")){
			debug = 1;
		}
		
		else if(! strcmp(argv[i], "forwarding")){
			execution_delay = 0;		
		}
		
		else{
			debug = 0;
			execution_delay = 1;
		}
	}
	
	initialize_pipe();
	char myString[10];
	FILE *fptr;
	int num[SIZ];
	pipeline_head = 0;
	pipeline_tail = 0;
	int linect = 0;//maintain a count of the lines as we step through the file.
	initialize_reg(gpReg);

	fptr = fopen("LoopyTB.txt", "r"); //("//simpleLDWtb.txt", "r");
    //LoopyTB.txt
    //SubTB2.txt
    //imageAddTB.txt
	//TEST.txt
	//TEST2.txt
	//Test3.txt
	//TEST4.txt -HAs JUMP?
	//addi_sw.txt
	//ld_st.txt
	//load1.txt
	//sw_lw.txt
	//imageAddTB.txt
	//imageAndTB.txt
	//imageMULTBplusSub.txt
	//imageOrTB.txt
	//imageSubTB.txt
	//imageXorTB.txt
	//FIXME line83, pass the name of the file as a variable. i.e. arg1.
	//FIXME PUT This loop into a function, fillprogram

	progstop = process_program(myString, fptr, linect, progstop, num, &program);
	while(fgets(myString, 10, fptr)){

		mem_img2bin(myString, num);
		fill_mem(data_seg, num, linect++);
		data_ct++;
		//printf("datact is: %d\n", data_ct);
	}

		//printf("datact is: %d\n", data_ct);
	print_program(progstop);

	//test_mem(data_seg, linect);
	//printf("linect is: %d\n", linect);
	fclose(fptr);

	//testing/proving:
	//print_reg(gpReg);

	//int encounter_halt=0;
		for(;;){
		//int end_program=0;

		if(check_for_halt()==1)
			break;
		cycle++;
		cleanup_pipe();	
		//write_back: just move it to write back stage from memory.
		printf("Cycle: %d____________________________________________________________\n", cycle);
		printf("PC is %d____________________________________________________________\n", pc);
		write_back();

		printf("flush after write back = %d\n", flush);
		//mem-access, if load or store, there is a memory operation. otherwise just move it forward, probably. 
		memory_access(&pc);
		
		printf("flush after memory acces = %d\n", flush);
		//execute's the program.
		execute_instruction(&pc);

		printf("flush after execute = %d\n", flush);
		//decode an instruction from the program array, unless there is a stall.
		decode_instruction();

		printf("flush after decode = %d\n", flush);
		//fetch an instruction from the program array, unless there's a stall.

		fetch_instruction(&pc);

		printf("flush after fetch = %d\n", flush);
		//decode an instruction from the program array, unless there is a stall.

		//pc++;//increment program counter by one.  (need to adjust to four later).	
	}
	/*	
		for(int i=0;i<progstop;i++){
	//for(int j=0;j<5;j++){
	//change the name of this function assignment->to something else.
	//copy_to_pipe(&program[i], &pipeline[j]);
	display_struct(&pipeline[i]);
	//}			
	//process_code(&program[i], gpReg);
	}
	//print_reg(gpReg);//prints the list of registers.
	*/	
	/*
	   for(int i=0;i<5;i++){
	   display_struct(&pipeline[i]);
	   }
	   */
	
	//printf("datact is: %d\n", data_ct);
	if(debug){
		print_mem2(data_seg, data_ct);
	}
	exit(EXIT_SUCCESS);
}//end of main function.


void initialize_array(struct instruction *an_instruction[]){

	for(int i=0;i<5;i++){
		pipeline[i].pipe_stage=-1;
	}
}


int bin1dec_2sComp(int array[], int len) {

	// Decimal value to be returned
	int dec = 0;

	for (int i=0; i< len; i++) {
		if (array[i] == 1) {
			dec = (dec + pow(2, (len-1-i)));
		}
	}

	if (array[0] == 1) {
		dec = (dec - pow(2, (len)));
	}
	//return decimal value;  
	return dec; 
}//end of bin2dec_2sComp

int bin2dec(int array[], int len) {

	// Decimal value to be returned
	int dec = 0;


	for (int i=0; i< len; i++) {
		if (array[i] == 1) {
			dec = (dec + pow(2, (len-1-i)));
		}
	}
	//return decimal value;  
	return dec;  
}//end of bin2dec
 //

int extract_opcode(int bin_inst[], int opLen){

	int opcode[6];
	int decOp = 0;

	for (int i=0; i<6; i++){
		opcode[i] = bin_inst[(i)];
	}   

	decOp = bin2dec(opcode, opLen);
	return decOp;
}

int extract_mem(int bin_inst[]){
	int mem[32];
	int dec_mem=0;

	for(int i=0;i<32;i++){
		mem[i]=bin_inst[(i)];
	}
	dec_mem=bin2dec(mem, 32);
	return dec_mem;
}

const char* extract_opcode_str(int opcode){
	int op=opcode;
	//char inst_str[5];
	if(op==0)
		//strcpy(inst_str, "ADD");
		return "ADD";

	if(op==1)
		//strcpy(inst_str, "ADDI");
		return "ADDI";
	if(op==2)
		//strcpy(inst_str, "SUB");
		return "SUB";
	if(op==3)
		return "SUBI";
	if(op==4)
		return "MUL";
	if(op==5)
		return "MULI";
	if(op==6)
		return "OR";
	if(op==7)
		return "ORI";
	if(op==8)
		return "AND";
	if(op==9)
		return "ANDI";
	if(op==10)
		return "XOR";
	if(op==11)
		return "XORI";
	if(op==12)
		return "LDW";
	if(op==13)
		return "STW";
	if(op==14)
		return "BZ";
	if(op==15)
		return "BEQ";
	if(op==16)
		return "JR";
	if(op==17)
		return "HALT";
	else
		return "XXXX";
}

int extract_rs(int bin_inst[]){

	int rs[5];
	int dec_rs = 0;

	for (int i=6, j=0; i<11;i++){
		rs[j++]=bin_inst[(i)];
	}

	dec_rs = bin2dec(rs, 5);
	//printf("RS is: %d ", dec_rs);
	return dec_rs;
}

int extract_rt(int bin_inst[]){

	int rt[5];
	int dec_rt = 0;

	for (int i=11, j=0; i<16;i++){
		rt[j++]=bin_inst[(i)];
	}

	dec_rt = bin2dec(rt, 5);
	//printf("RT is: %d ", dec_rt);
	return dec_rt;
}

int extract_rd(int bin_inst[]){

	int rd[5];
	int dec_rd = 0;

	for (int i=16, j=0; i<21;i++){
		rd[j++]=bin_inst[(i)];
	}

	dec_rd = bin2dec(rd, 5);
	//printf("RD is: %d\n", dec_rd);
	return dec_rd;
}

int extract_type(int bin_inst[]){

	int decimal_opcode = extract_opcode(bin_inst, 6);

	if(decimal_opcode == 0 || decimal_opcode == 2 || decimal_opcode ==4 || decimal_opcode==6 || decimal_opcode==8 || decimal_opcode==10){
		//0 = ADD; 2=SUB; 4=MUL; 6= 8=AND 10=XOR.
		return 0;
	}
	else
		return 1;
}

int extract_immediate(int bin_inst[]){

	int imm[16];
	int dec_imm = 0;

	for(int i=16, j=0; i<32;i++){
		imm[j++]=bin_inst[(i)];
	}
	dec_imm=bin1dec_2sComp(imm, 16);

	return dec_imm;
}

void print_program(int linect){
	//printf("linecount is %d\n", linect);
	for(int i=0;i<linect;i++){
		if(program[i].TYPE==0) //THIS IS R-TYPE
			printf("%s RD:R%d, RS:R%d RT:R%d \n", program[i].opcode_name, program[i].rd, program[i].rs, program[i].rt);

		if(program[i].TYPE==1) //THIS IS i-TYPE
			printf("%s RT:R%d, RS:R%d IMM:%d \n", program[i].opcode_name, program[i].rt, program[i].rs, program[i].imm);
	}
}

struct instruction fill_instruction(int bin_inst[], int len){

	struct instruction my_instruct;
	my_instruct.opcode = extract_opcode(bin_inst, 6);  //bin2dec(opcode, len);
	my_instruct.rs = extract_rs(bin_inst);
	my_instruct.rt = extract_rt(bin_inst);
	my_instruct.rd = extract_rd(bin_inst);
	my_instruct.TYPE = extract_type(bin_inst);

	my_instruct.ready = 0;
	return my_instruct;
}

void flush_pipeline(){
	for(int i=0;i<5;i++){
		if(wrong_path[i].opcode != -1){
			for(int j=0;j<5;j++){
				if(wrong_path[i].serial == pipeline[j].serial && pipeline[j].opcode != -1){
					pipeline[j].opcode = -1;
					pipeline[j].pipe_stage = -1;
					wrong_path[i].opcode = -1;
					wrong_path[i].pipe_stage = -1;
					printf("serial getting flushed: %d\n", wrong_path[i].serial);
				}

			}	
		}
	}
}


int process_program(char myString[], FILE *fptr, int linect, int progstop,int num[], struct instruction *an_instruction[]){
	//TODO:Need to test this function, only compiles, has not been tested.
	while(fgets(myString, 10, fptr)){
		mem_img2bin(myString, num);//converts hex to bin, saves in num.
					   //prt32(num);//prints all 32 bits of the binary number (TODO for testing)
		int serial = linect;
		
		parse(&program[linect++], num, serial);
		if(program[linect-1].opcode==17){//this is a halt instruction
						 //continue filling the memory, stop filling the program.	
			progstop=linect;
			return progstop;
			//break;	
		}
	}
	return progstop;
}

int  pipe_empty() {

	int i=-3;
	for(i=0;i<5;i++)
		if(pipeline[i].pipe_stage==-1)
			break;
	return i;

}
void fetch_instruction(int *pc){
	
	int fetched_inst_stage = stagebusy(0);
	if ((fetched_inst_stage == -1 || *pc == 1) && flush !=1) {
			
		int empty_pipe_slot=pipe_empty();
			
		copy_to_pipe(&program[*pc], &pipeline[empty_pipe_slot]);
		pipeline[empty_pipe_slot].pipe_stage=0; 
		//branch needs different handling.
		(*pc)++;
			
		printf("\nFetching inst.");
		display_struct(&pipeline[empty_pipe_slot]);

		if(watch_out == 1){
			
			copy_to_pipe(&pipeline[empty_pipe_slot], &wrong_path[wrong_path_start++]);
			//for(int i =0;i<5;i++){
				//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@wrong path pipe stage is: %d\n", wrong_path[i].pipe_stage);
				//printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@wrong path serial is: %d\n", wrong_path[i].serial);
				//display_struct(&wrong_path[i]);
			//}
		}
		
	}
	if(flush==1){
		flush=0;
		watch_out=0;
		wrong_path_start=0;
		for(int i=0;i<5;i++){
			
			wrong_path[i].opcode=-1;
			wrong_path[i].pipe_stage=-1;
		}
	}

	printf("exiting fetch.\n");
}



void decode_instruction() {

	int fetched_inst_index=stagebusy(0);

	if(stagebusy(1)==-1 && fetched_inst_index != -1 && flush !=1) {
	
		//FIXME: on Branch a stall might happen
		//pipeline[fetched_inst_index].pipe_stage=1;

		if(pipeline[fetched_inst_index].opcode==12 && cycle>reg_ready[pipeline[fetched_inst_index].rs]) /*&& no store with same address is pending*/ {//type 1.
					
			reg_ready[pipeline[fetched_inst_index].rt]=cycle+execution_delay;
			pipeline[fetched_inst_index].pipe_stage=1;
			printf("\nDecoding inst.");
			display_struct(&pipeline[fetched_inst_index]);
		}
		else if(pipeline[fetched_inst_index].opcode==13 && cycle>reg_ready[pipeline[fetched_inst_index].rs]){
				
			int effective_address=gpReg[pipeline[fetched_inst_index].rs]+pipeline[fetched_inst_index].imm;
			//FIXME address conflicts need to be resolved.
			data_red[effective_address]=reg_ready[pipeline[fetched_inst_index].rs]+execution_delay;
			pipeline[fetched_inst_index].pipe_stage=1;
			printf("\nDecoding inst.");
			display_struct(&pipeline[fetched_inst_index]);
		}
		else if(pipeline[fetched_inst_index].opcode==16){//if opcode is jump. FIXME: if opcode is jump AND target addres !=PC.
			
			watch_out = 1;
			printf("setting watchout to 1");
			pipeline[fetched_inst_index].pipe_stage=1;
			printf("\nDecoding inst.");
			display_struct(&pipeline[fetched_inst_index]);
		}
		//same as above, but for branch BZ:
		else if(pipeline[fetched_inst_index].opcode==14){//if opcode is jump. FIXME: if opcode is jump AND target addres !=PC.
			
			watch_out = 1;
			printf("setting branch BZ watchout to 1\n");
			
			if(reg_ready[pipeline[fetched_inst_index].rs]<cycle){
				pipeline[fetched_inst_index].pipe_stage=1;
				printf("\n********************Decoding BZ inst.");
				display_struct(&pipeline[fetched_inst_index]);
			}

		}
		else if(pipeline[fetched_inst_index].opcode==15){
		
			watch_out = 1;
			printf("setting watchout to 1 from BEQ\n");

			if(reg_ready[pipeline[fetched_inst_index].rs]<cycle && reg_ready[pipeline[fetched_inst_index].rt]<cycle){
				pipeline[fetched_inst_index].pipe_stage=1;
				printf("\nDecoding BEQ inst. ");
				display_struct(&pipeline[fetched_inst_index]);
			}

		}
		else{

			pipeline[fetched_inst_index].pipe_stage=1;
			printf("\nDecoding inst.");
			display_struct(&pipeline[fetched_inst_index]);

		}
		if(debug){
		print_reg(gpReg);
		}
		//if(debug){
		//	print_reg(reg_ready);	
		//}

	}
	
	printf("exiting decode.\n");
}

void execute_instruction(int *pc){
		
		//execute the actual code...
		int decoded_inst_index=stagebusy(1);
		if(stagebusy(2)==-1 && decoded_inst_index != -1 && flush !=1){

				if(pipeline[decoded_inst_index].TYPE==0) {

					if(cycle>reg_ready[pipeline[decoded_inst_index].rs] && cycle>reg_ready[pipeline[decoded_inst_index].rt]){

						pipeline[decoded_inst_index].pipe_stage=2;
						reg_ready[pipeline[decoded_inst_index].rd]=cycle+execution_delay;
						
						printf("execute inst.");
						//TODO FIXME: Need to test this!  THIS: is the function that computes math results.
						if(pipeline[decoded_inst_index].opcode<12){
						process_code(&pipeline[decoded_inst_index], gpReg);
						}
						/*	if(debug){
							print_reg(gpReg);
							}	*/
						display_struct(&pipeline[decoded_inst_index]);

					}		
					else printf("STALL:execute inst. \n");
				}

				if(pipeline[decoded_inst_index].TYPE==1) {

						if(cycle>reg_ready[pipeline[decoded_inst_index].rs]) {

							if(pipeline[decoded_inst_index].opcode != 12 && pipeline[decoded_inst_index].opcode !=13){
								
								reg_ready[pipeline[decoded_inst_index].rt]=cycle+execution_delay;
							}
	
							pipeline[decoded_inst_index].pipe_stage=2;//update execution stage of instruction in pipe.
						
							printf("execute inst.");
							//TODO FIXME: Need to test this code! //THIS: is the function that computes math results.
						
							if(pipeline[decoded_inst_index].opcode<12){
								
								process_code(&pipeline[decoded_inst_index], gpReg);
							}
					/*	
							if(debug){
								print_reg(gpReg);
							}		       */

							display_struct(&pipeline[decoded_inst_index]);

						}

						
						else printf("STALL:execute inst. \n");
				}
			
		}
		
		printf("exiting execute.\n");
	}//closes execute 



void memory_access(int *pc){

	int executed_inst_index=stagebusy(2);
	if(stagebusy(3) == -1 && executed_inst_index != -1){

			if(pipeline[executed_inst_index].opcode==12 /*&& no store with same address is pending*/ ){//type 1.
					
				reg_ready[pipeline[executed_inst_index].rt]=cycle;//FIXME: how will we handle forwarding/no-forawrding in a load instance?
				//printf("********************Load REG_Ready: %d\n", pipeline[executed_inst_index].rt);
				printf("mem inst. ld ");	

				int effective_address=gpReg[pipeline[executed_inst_index].rs]+pipeline[executed_inst_index].imm;
				LDW(pipeline[executed_inst_index].rt, effective_address, gpReg, progstop);
				display_struct(&pipeline[executed_inst_index]);
				
				pipeline[executed_inst_index].pipe_stage=3;

			}
			else if(pipeline[executed_inst_index].opcode==13 ){
					//printf("gp_register value: %d\n", pipeline[executed_inst_index].rs);
					//printf("reg_ready value: %d\n", reg_ready[pipeline[executed_inst_index].rs]);
					//printf("^^^^^^^^^^^^^^^^^");
					//display_struct(&pipeline[executed_inst_index]);
				if(cycle>reg_ready[pipeline[executed_inst_index].rt]){
						
					int effective_address=gpReg[pipeline[executed_inst_index].rs]+pipeline[executed_inst_index].imm;
					//FIXME address conflicts need to be resolved.
					data_red[effective_address]=reg_ready[pipeline[executed_inst_index].rs]+1; //FIXME: how will we handle forwarding/no-forawrding in a store instance?
					//TODO:call my function that handles writing writing to memory/ STW()...whatever.
					//calls actual stw write t o memory.
					STW(pipeline[executed_inst_index].rt, effective_address, gpReg);
					pipeline[executed_inst_index].pipe_stage=3;
					
					printf("mem inst./st ");	
					display_struct(&pipeline[executed_inst_index]);

				}
			}
			else if(pipeline[executed_inst_index].opcode == 16){//opcode 16 = jump.
				//printf("************************************executing the jump in type1");
				int target_pc = (gpReg[pipeline[executed_inst_index].rs]) /4;//PC is an index, thus we divide by 4."

				//flushing pipeline with all wrong path instructions:
				//if(target_pc != (*pc)){
					for(int i=0;i<5;i++){
						printf("wrong path[%d] is: %d\n", i, wrong_path[i].serial);
					}
						
					flush_pipeline();
					flush = 1;
					wrong_path_start = 0;
					clear_wrong_path_buffer();
					(*pc)= target_pc;
					printf("Flushing from jump:\n");
										
				//}
				
				pipeline[executed_inst_index].pipe_stage=3;
				printf("mem inst./j ");
				display_struct(&pipeline[executed_inst_index]);

			}
			//same code as else if from above being modified to handle BZ branches instead of jumps.
			else if(pipeline[executed_inst_index].opcode == 14) {
				printf("************************************executing the BZ Rs x BRANCH in type1");
				
				int target_pc =((pipeline[executed_inst_index].imm)+(pipeline[executed_inst_index].serial));
				//flushing pipeline with all wrong path instructions:
				if(gpReg[pipeline[executed_inst_index].rs] == 0){//FIXME:if target_PC==PC we should not flush.
					for(int i=0;i<5;i++){
						printf("wrong path[%d] is: %d\n", i, wrong_path[i].serial);
					}
							
					flush_pipeline();
					flush = 1;
					wrong_path_start = 0;
					clear_wrong_path_buffer();
					printf("\nBBBB target_pc is: %d\n", target_pc);
					printf("current pc is: %d\n", (*pc));
					(*pc)= target_pc;
					printf("Flushing from BZ Rs x:\n");
										
				}	
				else{
					watch_out=0;
					clear_wrong_path_buffer();
				}
				pipeline[executed_inst_index].pipe_stage=3;
				printf("mem inst./BZ ");
				display_struct(&pipeline[executed_inst_index]);

			}
			//same code as else if from above, being modified to handle BEQ branches instead of BZ branches.	
			else if(pipeline[executed_inst_index].opcode == 15){//15 is BEQ's opcode.

				printf("************************************executing the BEQ Rs x BRANCH in type1");
				int target_pc =((pipeline[executed_inst_index].imm)+(pipeline[executed_inst_index].serial));
				printf("imm is %d/n", pipeline[executed_inst_index].imm);
				//flushing pipeline with all wrong path instructions:
				if(gpReg[pipeline[executed_inst_index].rs]==gpReg[pipeline[executed_inst_index].rt]){
				
					for(int i=0;i<5;i++){
						printf("wrong path[%d] is: %d\n", i, wrong_path[i].serial);
					}
			
					flush_pipeline();
					flush = 1;
					wrong_path_start = 0;
					clear_wrong_path_buffer();
					printf("\nAAAA target_pc is: %d\n", target_pc);
					(*pc)= target_pc;
					printf("Flushing from BEQ Rs x:\n");
										
				
				}	
				else{
					watch_out=0;
					clear_wrong_path_buffer();
				}

				pipeline[executed_inst_index].pipe_stage=3;
				printf("mem inst./BEQ ");
				display_struct(&pipeline[executed_inst_index]);

			}



			else {
				pipeline[executed_inst_index].pipe_stage=3;
						
				printf("mem inst. Not ld/st ");
                                display_struct(&pipeline[executed_inst_index]);
			}
			//execution will have calculated address.
			//here, you will update the shadow array of memory to be the cycle+1;
			//need to calculate an address, is all instructions move into this stage.
			//grab the executed one, make it memory, whatever we did before, put in memory,
			//check if load or store (do a bunch of stuff) 
			//move the index returned to us to be memory, after that we'll know if it's load or store,
			//we'll do a bunch of stuff, then we exit.  
			//execute memory access function.
					}

		printf("exiting mem-access.\n");
	}


	void write_back(){

		int mem_inst_index=stagebusy(3);
		if(stagebusy(4)== -1 && mem_inst_index != -1){

			pipeline[mem_inst_index].pipe_stage=4;
			printf("write_Back inst.");
			display_struct(&pipeline[mem_inst_index]);
		}

		printf("exiting WB.\n");
		//display_struct(&pipeline[mem_inst_index]);
		
	}


int stagebusy(int execution_stage){//this function is very confusing.

		//if the stage is busy return the index of that entry else return -1
		int stage_busy=-1;
		for(int i=0;i<5;i++){

			if(pipeline[i].pipe_stage==-1)//if pipeline is initialized to all -1s, the else if never runs.
				continue;
			else if(pipeline[i].pipe_stage==execution_stage){
				return i;
			}
		}	
		return stage_busy;//this will execute only for -1

}

void cleanup_pipe(){
		//loop over the pipeline:
		for(int i=0;i<5;i++){
			if(pipeline[i].pipe_stage==4) {
				printf("inside the cleanup\n");
				pipeline[i].pipe_stage=-1;

			}
		}
	}

int check_for_halt() {

		for(int i=0;i<5;i++){
			if(pipeline[i].pipe_stage==4 && pipeline[i].opcode==17)
				return 1;
		}
		return 0;

	}


void initialize_pipe(){

	for(int i=0;i<5;i++){
		pipeline[i].opcode = -1;
			pipeline[i].rs= -1;
			pipeline[i].rt= -1;
			pipeline[i].rd= -1;
			pipeline[i].imm= -1;
			pipeline[i].ready= -1;
			pipeline[i].pipe_stage = -1;
			pipeline[i].serial = -1;

			wrong_path[i].opcode = -1;
			wrong_path[i].rs= -1;
			wrong_path[i].rt= -1;
			wrong_path[i].rd= -1;
			wrong_path[i].imm= -1;
			wrong_path[i].ready= -1;
			wrong_path[i].pipe_stage = -1;
			wrong_path[i].serial = -1;


	}
}

void clear_wrong_path_buffer(){
	for(int i=0;i<5;i++){
		wrong_path[i].opcode = -1;
		wrong_path[i].rs= -1;
		wrong_path[i].rt= -1;
		wrong_path[i].rd= -1;
		wrong_path[i].imm= -1;
		wrong_path[i].ready= -1;
		wrong_path[i].pipe_stage = -1;
		wrong_path[i].serial = -1;
	}
	wrong_path_start=0;
}


