
struct instruction {
	int opcode;
	char opcode_name[5];
	
	int rs;
	int rt;
	int rd;
	int imm;

	int TYPE;// 0 is R-TYPE; 1 is I-TYPE
	int ready;
	
	int pipe_stage; //if this is set to -1 it means it is 'empty' 
				//this can have 5 values according to every time I put an instruction in a stage, 
				//I need to update this value to reflect what part of the pipeline this instruction is in.  
	int serial;      //
};


