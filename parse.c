#include "header.h"


//extern struct instruction program[3000]; 
extern int data_seg[1000];
extern int progstop;


int mem_img2bin(char myString[10], int num[SIZ]){
	
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 0;

	for(int i=0;i<8;++i){
		if(i==0){
			a=0;
			b=1;
			c=2;
			d=3;
		}
		if(i==1){
			a=4;
			b=5;
			c=6;
			d=7;
		}
		if(i==2){
			a=8;
			b=9;
			c=10;
			d=11;
		}
		if(i==3){
			a=12;
			b=13;
			c=14;
			d=15;
		}
		if(i==4){
			a=16;
			b=17;
			c=18;
			d=19;
		}
		if(i==5){
			a=20;
			b=21;
			c=22;
			d=23;
		}
		if(i==6){
			a=24;
			b=25;
			c=26;
			d=27;
		}
		if(i==7){
			a=28;
			b=29;
			c=30;
			d=31;
		}

		if(myString[i]=='0'){
			num[a]=0;
			num[b]=0;
			num[c]=0;
			num[d]=0;
		}
		if(myString[i]=='1'){
			num[a]=0;
			num[b]=0;
			num[c]=0;
			num[d]=1;
		}
		if(myString[i]=='2'){
			num[a]=0;
			num[b]=0;
			num[c]=1;
			num[d]=0;
		}
		if(myString[i]=='3'){
			num[a]=0;
			num[b]=0;
			num[c]=1;
			num[d]=1;
		}
		if(myString[i]=='4'){
			num[a]=0;
			num[b]=1;
			num[c]=0;
			num[d]=0;
		}
		if(myString[i]=='5'){
			num[a]=0;
			num[b]=1;
			num[c]=0;
			num[d]=1;
		}
		if(myString[i]=='6'){
			num[a]=0;
			num[b]=1;
			num[c]=1;
			num[d]=0;
		}
		if(myString[i]=='7'){
			num[a]=0;
			num[b]=1;
			num[c]=1;
			num[d]=1;
		}
		if(myString[i]=='8'){
			num[a]=1;
			num[b]=0;
			num[c]=0;
			num[d]=0;
		}
		if(myString[i]=='9'){
			num[a]=1;
			num[b]=0;
			num[c]=0;
			num[d]=1;
		}
		if(myString[i]=='A'){
			num[a]=1;
			num[b]=0;
			num[c]=1;
			num[d]=0;
		}
		if(myString[i]=='B'){
			num[a]=1;
			num[b]=0;
			num[c]=1;
			num[d]=1;
		}
		if(myString[i]=='C'){
			num[a]=1;
			num[b]=1;
			num[c]=0;
			num[d]=0;
		}
		if(myString[i]=='D'){
			num[a]=1;
			num[b]=1;
			num[c]=0;
			num[d]=1;
		}
		if(myString[i]=='E'){
			num[a]=1;
			num[b]=1;
			num[c]=1;
			num[d]=0;
		}
		if(myString[i]=='F'){
			num[a]=1;
			num[b]=1;
			num[c]=1;
			num[d]=1;
		}
	}//close forloop
	//prt32(num); //TODO: KEEP THIS HERE FOR FUTURE DEBUGGING.
	return 0;
}//close mem_img2bin function


void parse(struct instruction *an_instruction, int num[SIZ], int serial){

	an_instruction->opcode=extract_opcode(num, OPLEN);
	//printf("opcode= %d\n", an_instruction->opcode);
	strcpy(an_instruction->opcode_name,extract_opcode_str(an_instruction->opcode));
	
	an_instruction->rs=extract_rs(num);
	//printf("RS is: %d\n", an_instruction->rs);
	an_instruction->rt=extract_rt(num);
	//printf("rt is: %d\n", an_instruction->rt);
	an_instruction->rd=extract_rd(num);
	//printf("rd is: %d\n", an_instruction->rd);
	an_instruction->imm=extract_immediate(num);
	//printf("imm is: %d\n", an_instruction->imm);
	an_instruction->TYPE=extract_type(num);
	//printf("type is %d\n", an_instruction->TYPE);
	//printf("type is: %d\n", an_instruction.TYPE);
	an_instruction->ready=0;
	an_instruction->serial=serial;
}

void fill_mem(int data_seg[], int num[SIZ], int linect){
	//printf("linect is: %d\n", linect);//TODO KEEP FOR DEBUGGING?
	//printf("num is: \n");//TODO: KEEP FOR DEBUGGING?
	//prt32(num);//TODO:KEEP FOR DEBUGGING?
	data_seg[linect]=extract_mem(num);	
	
	//printf("              data_seg is: %d\n", data_seg[linect]);//TODO:KEEP FOR DEBUGGING?
}

void print_mem(int data_seg[], int datact){
	for(int i=0;i<datact;i++){
		int j=((i+1)*4);
		if(data_seg[i] !=0){
			printf("Address: %d;  contents: %d\n", j, data_seg[i]);
		}
	}
}

int get_mem(int data_seg[], int value){
	int line=value*4;
	printf("line is: %d\n", line);

	return line;
}
void prt32(int num[SIZ]){//TODO: THIS IS FOR DEBUGGING, mostly unused.

	int count = 0;
	for(int i=0;i<32;++i){
		if(count!=3){
			printf("%d", num[i]);
			++count;
		}
		else{
			printf("%d ", num[i]);
			count=0;
		}
	}

		printf("\n");
}

int LDW(int rt, int effective_address, int gpReg[], int progstop){
	printf("RAW effective address is: %d\n", effective_address);
	gpReg[rt]=data_seg[((effective_address-100)/4)];
	//gpReg[rt]=data_seg[(((effective_address-25)/4))];
	printf("CCCCgpReg[rt] is: %d\n", gpReg[rt]);

	printf("effective_address is: %d\n", effective_address);
	printf("data at effective address: %d\n", data_seg[((effective_address+100)/4)]);
	return rt;
}

int STW(int rt, int effective_address, int gpReg[]){	

	int effective_index=((effective_address)/4);
	data_seg[effective_index-1]=gpReg[rt];
	//printf("CCCCgpReg[rt] is: %d\n", gpReg[rt]);
	printf("effective_address is: %d\n", effective_address);
	printf("progstop is: %d\n", progstop);
	
       	return 0;
}


int process_code(struct instruction *an_instruction, int gpReg[])
{
	int result=0;

	if(an_instruction->opcode==0){
		//printf("**********PROCESS_CODE: rs is: %d\n rt is: %d\n", an_instruction->rs, an_instruction->rt);
		result = ADD(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		//printf("**********PROCESS_CODE: result is: %d\n", result);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==1){
		//printf("**********PROCESS_CODE:  rs is: %d\n imm is: %d\n", an_instruction->rs, an_instruction->imm);
		result=ADDI(gpReg[an_instruction->rs], an_instruction->imm);
		//printf("**********PROCESS_CODE:  result is: %d\n", result);

		gpReg[an_instruction->rt]=result;
		return 0;
	}

	if(an_instruction->opcode==2){
		result=SUB(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==3){
		result=SUBI(gpReg[an_instruction->rs], an_instruction->imm);
		gpReg[an_instruction->rt]=result;
	}
	if(an_instruction->opcode==4){
		result=MUL(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==5){
		result=MULI(gpReg[an_instruction->rs], an_instruction->imm);
		gpReg[an_instruction->rt]=result;
	}
	if(an_instruction->opcode==6){
		result=OR(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==7){
		result=ORI(gpReg[an_instruction->rs], an_instruction->imm);
		gpReg[an_instruction->rt]=result;
	}
	if(an_instruction->opcode==8){
		result=AND(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==9){
		result=ANDI(gpReg[an_instruction->rs], an_instruction->imm);
		gpReg[an_instruction->rt]=result;
	}
	if(an_instruction->opcode==10){
		result=XOR(gpReg[an_instruction->rs], gpReg[an_instruction->rt]);
		gpReg[an_instruction->rd]=result;
	}
	if(an_instruction->opcode==11){
		result=XORI(gpReg[an_instruction->rs], an_instruction->imm);
		gpReg[an_instruction->rt]=result;
	}
/*	
*/


	//gpReg[an_instruction->rd]=result;
	return 0;
}

int ADD(int rs, int rt){
	return rs+rt;
}	

int ADDI(int rs, int imm){
	int rt=rs+imm;
	//printf("num is %d\n", num);
	return rt;
}

int SUB(int rs, int rt){
	return rs-rt;
}

int SUBI(int rs, int imm){
	int rt=rs+imm;
	return rt;
}

int MUL(int rs, int rt){
	return rs*rt;
}

int MULI(int rs, int imm){
	int rt=rs*imm;
	return rt;
}

int OR(int rs, int rt){
	return rs|rt;
}

int ORI(int rs, int imm){
	int rt=rs|imm;
	return rt;
}

int AND(int rs, int rt){
	return rs&rt;
}

int ANDI(int rs, int imm){
	int rt=rs&imm;
	return rt;
}

int XOR(int rs, int rt){
	return rs^rt;
}

int XORI(int rs, int imm){
	int rt=rs^imm;
	return rt;
}

void initialize_reg(int gpReg[32]){

	for(int i=0;i<SIZ;i++){
		gpReg[i]=0;
	}
}


void print_reg(int gpReg[]){
	for(int i=0;i<SIZ;i++){
		printf("register[%d] is: %d\n", i, gpReg[i]);
	}
}

//function that assigns value of one struct to another.
////TODO rename this function to have a better name.
void copy_to_pipe(struct instruction *an_instruction, struct instruction *other_instruction){
	
	other_instruction->opcode=an_instruction->opcode;
	for(int i=0;i<5;i++){
		other_instruction->opcode_name[i]=an_instruction->opcode_name[i];
	}
	/*
	for(int i=0;i<5;i++){
		printf("%c",other_instruction->opcode_name[i]);
	}
	printf("\n");
	*/
	
	other_instruction->rs=an_instruction->rs;
	other_instruction->rt=an_instruction->rt;
	other_instruction->rd=an_instruction->rd;
	other_instruction->imm=an_instruction->imm;

	other_instruction->TYPE=an_instruction->TYPE;
	other_instruction->ready=an_instruction->ready;
	
	other_instruction->pipe_stage=an_instruction->pipe_stage;
	
	other_instruction->serial=an_instruction->serial;
	//when doing this function, need to maintain the number of items in the array.  
}

void display_struct(struct instruction *an_instruction){
	if(an_instruction->TYPE==0){
	printf("opcode is: %d     ", an_instruction->opcode);
	printf("opcode name is:");
	//for(int i=0;i<5;i++){
		//printf("%c",an_instruction->opcode_name[i]);

		printf("%s",an_instruction->opcode_name);
	//}
	printf("     ");
	printf("rd is: %d     ", an_instruction->rd);
	printf("rs is: %d     ", an_instruction->rs);
	printf("rt is: %d     ", an_instruction->rt);
	printf("type is %d     ", an_instruction->TYPE);
	//printf("ready is: %d   ", an_instruction->ready);

	printf("pipestage is: %d    ", an_instruction->pipe_stage);
	printf("serial is: %d\n", an_instruction->serial);
	}

	if(an_instruction->TYPE==1){
	printf("opcode is: %d     ", an_instruction->opcode);
	printf("opcode name is:");
	//for(int i=0;i<5;i++){
		printf("%s",an_instruction->opcode_name);
	//}
	printf("     ");
	printf("rt is: %d     ", an_instruction->rt);
	printf("rs is: %d     ", an_instruction->rs);
	printf("imm is: %d     ", an_instruction->imm);
	printf("type is %d     ", an_instruction->TYPE);
	//printf("ready is: %d   ", an_instruction->ready);

	printf("pipestage is: %d   ", an_instruction->pipe_stage);
	printf("serial is: %d\n", an_instruction->serial);

	}
	
	}//close display function.

