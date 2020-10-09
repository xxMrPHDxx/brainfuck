#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

#define MAX_CELL 30000
#define MAX_LOOP 100

typedef struct {
	int start, ptr, end;
} LoopStack;

LoopStack LOOPS[MAX_LOOP];
int stack_ptr = 0;

char* CELLS;

void end(int exit_code){
	free(CELLS);
	exit(exit_code);
}

void push_stack(int start, int ptr){
	if(stack_ptr >= MAX_LOOP){
		printf("Error: Stack Overflowed! Current: %i, Max: 100\n", stack_ptr);
		end(-1);
	}
	LoopStack* loop = &LOOPS[stack_ptr++];
	loop->start = loop->end = start;
	loop->ptr = ptr;
}

uint8_t stack_empty(){
	return stack_ptr <= 0;
}

LoopStack* pop_stack(){
	if(stack_empty()){
		printf("Error: Stack Underflowed! Current: %i\n", stack_ptr);
		end(-1);
	}
	return &LOOPS[--stack_ptr];
}

LoopStack* peek(){
	return &LOOPS[stack_ptr-1];
}

void interpret(char* CODE, size_t len, LoopStack* main){
		size_t i = 0;
		while(i < len){
			LoopStack* loop = peek();

			char* c = &CODE[i];
			switch(*c){
				case '+': CELLS[loop->ptr]++; break;
				case '-': CELLS[loop->ptr]--; break;
				case '<': loop->ptr--; break;
				case '>': loop->ptr++; break;
				case '[': {
					push_stack(i++, loop->ptr);
					continue;
				} break;
				case ']': {
					loop->end = ++i;
					if(!CELLS[loop->ptr]){
						if(loop == main){
							printf("Missing ] (Loop wasn't closed somewhere...)\n");
							end(-1);
						}
						pop_stack();
						peek()->ptr = loop->ptr;
						i = loop->end;
					}else{
						i = loop->start + 1;
					}
				} continue;
				case ',': scanf("%c", &CELLS[loop->ptr]); break;
				case '.': printf("%c", CELLS[loop->ptr]); break;
				default: ;
			}
			i++;
		}
}

int main(int argc, char** argv){
	CELLS = (char*) calloc(30000, sizeof(char));
	if(argc == 2){
		FILE* f = fopen(argv[1], "r");
		fseek(f, 0, SEEK_END);
		int len = ftell(f);
		fseek(f, 0, SEEK_SET);

		push_stack(0, 0);
		peek()->end = len;
		LoopStack* main = peek();
		
		char CODE[len];
		int i=0, flag;
		while((flag = fgetc(f)) != EOF) CODE[i++] = flag;

		interpret(CODE, len, main);
		printf("\n");
		exit(0);
	}

	size_t MAX_CODE_LEN = 1000;
	char CODE[MAX_CODE_LEN];
	char* ptr = CODE;
	size_t char_len;

	printf("Welcome to BrainF!!!\n");
	printf("Type \"exit\" to quit...\n");
	while(1){
		printf("> ");
		char_len = getline(&ptr, &MAX_CODE_LEN, stdin);
		CODE[--char_len] = 0;
		if(!strcmp(CODE, "exit"))
			break;
		push_stack(0, 0);
		LoopStack* main = peek();
		main->end = char_len;
		interpret(CODE, char_len, main);
		pop_stack();
		printf("\n");
	}

	end(0);
}
