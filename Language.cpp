#include "Language.h"

#define ANOTHER_TREE

int print_tab_to_file(int tab_i, FILE * file){
	for(int i = 0; i < tab_i; i++) fprintf(file, "\t");
	return tab_i;
}

int main(){

	std::setlocale(LC_ALL, "ru_RU.UTF-8");

	List_t mstoken_list = {};
	mstokens = &mstoken_list;

	LIST_INIT(mstokens); // Multi_symbol_token_t temp = ...;

	temp = {};

	#define DEF_CPL(name, mode, equiv, code, do) \
	if(mode == MODE_FUNC){\
		temp.token = name;\
		temp.size = strlen(temp.token);\
		list_add_after(mstokens, temp, mstokens->head);\
		temp = {};\
	}

	#include"commands.h"
	#undef DEF_CPL

	#ifndef ANOTHER_TREE
	FILE * tree_file = fopen(TREE_FILE, "r");
	if(tree_file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", TREE_FILE);
		return 1;
	}

	Node_t *node = node_make_from_file_RDS_with_tokenization(tree_file);
	if(node == nullptr){
		fprintf(stderr, "Unable to make node from %s\n", TREE_FILE);
		return 1;
	}

	fclose(tree_file);

	FILE * new_tree_file = fopen("main.tree", "w");
	if(new_tree_file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", "main.tree");
		return 1;
	}

	log_tree_to_file(node, new_tree_file);

	fclose(new_tree_file);

	#endif

	FILE * old_tree_file = fopen("main.tree", "r");
	if(old_tree_file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", "main.tree");
		return 1;
	}

	#ifndef ANOTHER_TREE

	node = node_make_from_tree_file(old_tree_file);

	#else
	Node_t * node = node_make_from_tree_file(old_tree_file);
	#endif
	if(node == nullptr){
		fprintf(stderr, "Unable to make node from %s\n", "main.tree");
		return 1;
	}

	fclose(old_tree_file);

	FILE * new_new_tree_file = fopen("new_main.tree", "w");
	if(new_new_tree_file == nullptr){
		fprintf(stderr, "Unable to open file %s\n", "new_main.tree");
		return 1;
	}

	log_tree_to_file(node, new_new_tree_file);

	fclose(new_new_tree_file);

	FILE * new_tree_file_origin = fopen("new_tree_file.tree", "w");
	if(new_tree_file_origin == nullptr){
		fprintf(stderr, "Unable to open file %s\n", "new_tree_file.tree");
		return 1;
	}

	translate_tree_to_file(node, new_tree_file_origin, 0);

	fclose(new_tree_file_origin);

	FILE * asm_file = fopen("out.asm", "w");
	if(new_tree_file_origin == nullptr){
		fprintf(stderr, "Unable to open file %s\n", "out.asm");
		return 1;
	}

	compile_tree_to_asm_file(node, asm_file, nullptr);

	dot_node(node, IMAGE_OUT_2);

	fclose(asm_file);
	
	node_deinit(node);

	return 0;
}

void compile_tree_to_asm_file(Node_t * node, FILE * file, Var_elem_t *variables, int __mode/*=-1*/){


	if(node == nullptr){
		return;
	}

	bool did_alloc = false;

	if(variables == nullptr){
		variables = variables_copy(variables);
		did_alloc = true;
	}

	#define DEF_CPL(name, _mode, equiv, from_tree, to_asm) \
		else if((node->mode == MODE_FUNC && (strcmp(name, "@function@") == 0 || strcmp(name, list_get(mstokens, DATA).token) == 0)) || ((node->mode == MODE_OPER || node->mode == MODE_OP) && node->data == equiv) || (node->mode == _mode && (node->mode != MODE_OPER && node->mode != MODE_OP && node->mode != MODE_FUNC))){\
			to_asm;\
		}

	if(true == false){
		abort();
	}
	#include"commands.h"
	else{
		return;
	}
	#undef DEF_CPL

	if(did_alloc){
		free(variables);
	}

}

Var_elem_t *variables_copy(Var_elem_t *variables){
	

	Var_elem_t *temp = (Var_elem_t *) calloc(sizeof(variables[0]), MAX_VARIABLE_NUM);

	if(variables == nullptr){
		return temp;
	}

	for(int i = 0; i < MAX_VARIABLE_NUM; i++){
		temp[i] = variables[i];
	}

	return temp;

}

long long int variables_find(Var_elem_t *variables, Node_elem_t pos){

	if(variables == nullptr){
		return 0;
	}

	for(long long int i = 1; i < MAX_VARIABLE_NUM; i++){
		if(variables[i].data == pos){
			return i;
		}
	}
	
	return -1;

}

long long int variables_find_free(Var_elem_t *variables){
	
	if(variables == nullptr){
		return 0;
	}

	for(int i = 1; i < MAX_VARIABLE_NUM; i++){
		if(variables[i].data == 0){
			return i;
		}
	}

	return -1;

}

long long int variables_count_global(Var_elem_t *variables){

	if(variables == nullptr){
		return 0;	
	}

	long long int i = 0;
	for(; i < MAX_VARIABLE_NUM && variables[i].pos != 0; i++){
		if(variables[i].is_local == 1){
			break;
		}
	}
	
	return i;

}

void translate_tree_to_file(Node_t * node, FILE * file, int tabulation){

	if(node == nullptr){
		return;
	}

	#define DEF_CPL(name, _mode, equiv, from_tree, to_asm) \
		else if((node->mode == MODE_FUNC && (strcmp(name, "@function@") == 0 || strcmp(name, list_get(mstokens, DATA).token) == 0)) || ((node->mode == MODE_OPER || node->mode == MODE_OP) && node->data == equiv) || (node->mode == _mode && (node->mode != MODE_OPER && node->mode != MODE_OP && node->mode != MODE_FUNC))){\
			from_tree;\
		}

	if(true == false){
		abort();
	}
	#include"commands.h"
	else{
		return;
	}
	#undef DEF_CPL
	
}

void log_tree_to_file(Node_t * node, FILE * file){

	if(node == nullptr){
		fprintf(file, "@");
		return;
	}

	fprintf(file, "{");

	if(THIS->mode == MODE_CNST){
		fprintf(file, "%d", DATA/ACCURACY); if(DATA % ACCURACY != 0){fprintf(file, ".%d", DATA % ACCURACY);}
	}
	else if(THIS->mode == MODE_FUNC || THIS->mode == MODE_VARL){
		char * temp = list_get(mstokens, DATA).token;


	#define DEF_CPL(name, _mode, equiv, from_tree, to_asm) \
		else if(_mode == MODE_FUNC && strcmp(temp, name) == 0){\
				fprintf(file, "%s", name);\
			}
		if(true == false){
			abort();
		}
		#include"commands.h"
		else
			fprintf(file, "%s" NODE_ELEM_PRINT_FUNCTION , (THIS->mode == MODE_FUNC) ? "$" : "", (strcmp("главная", temp) == 0 && THIS->mode == MODE_FUNC)? "main" : temp);
		#undef DEF_CPL

	}
	else{
		unsigned char temp[MAX_LINE_LEN] = {};
		for(Node_elem_t i = 1, j = 0; i < DATA && i != 0; i <<= 8, j++){
			temp[j] = (DATA >> 8*j) % 256;
		}

		fprintf(file, "%s", temp);
	}
	
	if(LEFT == nullptr && RIGHT == nullptr){
		fprintf(file, "}");
		return;
	}
	log_tree_to_file(LEFT , file);
	log_tree_to_file(RIGHT, file);

	fprintf(file, "}");

}

void color_node(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	char color[MAX_COMMAND_LEN] = {};
	sprintf(color, "color = \"%s\"", THIS->mode == MODE_OPER ? "yellow"  :
					(THIS->mode == MODE_VARL ? "red"     :
					(THIS->mode == MODE_CNST ? "green"   :
					(THIS->mode == MODE_FUNC ? "blue"    : "black"))));

	if(THIS->mode == MODE_CNST){
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT_VALUE    "\"]\n", THIS, color, DATA);
	}
	else if(THIS->mode == MODE_FUNC || THIS->mode == MODE_VARL){
		fprintf(file, "%d [%s; shape = record; label =\"%s" NODE_ELEM_PRINT_FUNCTION "\"]\n", THIS, color, (THIS->mode == MODE_FUNC) ? "$" : "", list_get(mstokens, DATA).token);
	}
	else{
		unsigned char temp[MAX_LINE_LEN] = {};
		for(Node_elem_t i = 1, j = 0; i < DATA && i != 0; i <<= 8, j++){
			temp[j] = (DATA >> 8*j) % 256;
		}

		fprintf(file, "%d [%s; shape = record; label =\"%s\"]\n", THIS, color, temp);
	}
	color_node(LEFT, file);
	color_node(RIGHT, file);

}

Node_t * node_make_from_tree_file(FILE * file){

	assert(file != nullptr);

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff_opened_file(file, &buff_size);

	int pc = 0;
	Node_t * root = node_make_from_tree_buff(nullptr, buff, buff_size, &pc);
	free(buff);

	return root;
}

Node_t * node_make_from_tree_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc){

	assert(buff != nullptr);
	assert(pc   != nullptr);

	if(*pc >= buff_size){
		return nullptr;
	}

	if(buff[*pc] == SEPARATOR_START){
		(*pc)++;
	}
	else{
		return nullptr;
	}

	bool is_function = false;
	if(buff[*pc] == '$'){
		(*pc)++;
		is_function = true;
	}

	Node_elem_t val = Token_get_N(buff, pc);
	if(val < 0){
		val = Token_get_OP(buff, pc);
		if(val < 0){

			int column_delta = 0;
			val = Token_get_ID(buff, pc, &column_delta, true);

			if(val < 0){
				return nullptr;
			}
			else{

				char * temp_name = list_get(mstokens, val).token;
				int temp_size = strlen(temp_name);

				#define DEF_CPL(name, _mode, equiv, do1, do2) \
				 || (_mode == MODE_OP && strcmp(temp_name, name) == 0)

				if(false 
				#include"commands.h"
				){

					Node_elem_t name = 0;
					for(int i = 0; i < temp_size; i++){
						name = (name << 8) + temp_name[temp_size - i - 1];
					}

					node = node_create_new(MODE_OP, name, nullptr, nullptr);

				}
				else{

				#define DEF_CPL(name, _mode, equiv, from_tree, to_asm) \
					else if(_mode == MODE_FUNC && strcmp(temp_name, name) == 0){\
							node = node_create_new(MODE_FUNC, val, nullptr, nullptr);\
						}
					if(true == false)
						abort();
					#include"commands.h"
					else
						node = node_create_new((is_function) ? MODE_FUNC : MODE_VARL, val, nullptr, nullptr);
					#undef DEF_CPL
				}
			}
		}
		else{
			node = node_create_new(MODE_OPER, val, nullptr, nullptr);
		}
	}
	else{
		node = node_create_new(MODE_CNST, val, nullptr, nullptr);
	}
	
	if(buff[*pc] == '@'){
		LEFT  = nullptr;
		(*pc)++;
	}
	else{
		LEFT  = node_make_from_tree_buff(THIS, buff, buff_size, pc);
	}
	if(buff[*pc] == '@'){
		RIGHT = nullptr;
		(*pc)++;
	}
	else{
		RIGHT  = node_make_from_tree_buff(THIS, buff, buff_size, pc);
	}

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	if(buff[*pc] == SEPARATOR_END){
		(*pc)++;
		return node;
	}

	return nullptr;
}


Node_t * node_make_from_file_RDS_with_tokenization(FILE * file){

	assert(file != nullptr);

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff_opened_file(file, &buff_size);

	buff[buff_size - 2] = '\0';

	Token_elem_t * buff_tokenized = nullptr;

	buff_tokenized = language_file_tokenize_to_new_buff(buff, buff_size);

	if(buff_tokenized == nullptr){
		fprintf(stderr, "No luck\n");
		return nullptr;
	}

	if(LOG_LEVEL > 2){
		for(int i = 2; (mstokens->data)[i].prev >= 0 && i < mstokens->maxsize; i++){
				printf("[%4d] Data: %s\n", i, (mstokens->data)[i].data.token);
		}
		printf("--------\n");
		print_token_buff(buff_tokenized);
	}

	int pc = 0;
	Node_t * root = Get_G(buff_tokenized, &pc);
	free(buff);
	free(buff_tokenized);

	return root;
}

Token_elem_t * language_file_tokenize_to_new_buff(Buff_elem_t * buff, size_t buff_size){

	assert(buff != nullptr);

	Token_elem_t * new_buff = (Token_elem_t *) calloc(buff_size, sizeof(Token_elem_t));

	int j = 0;

	for(int i = 0, column = 0, line = 0; i < buff_size;){

		if(isspace(buff[i])){
			if(LOG_LEVEL > 6)
				printf("Space encountered: %d\n", buff[i]);
			if(buff[i] == '\n'){
				line++;
				column = 0;		
			}
			else{
				column += (buff[i] == ' ') ? 1 : (buff[i] == '\t') ? 8 : 0;		
			}
			i++;
			continue;
		}
		if(buff[i] == '\0'){
			break;
		}
		int save_i = i;
		Node_elem_t val = Token_get_N(buff, &i);
		if(val < 0){
			val = Token_get_OP(buff, &i);
			if(val < 0){
				int column_delta = 0;
				val = Token_get_ID(buff, &i, &column_delta);
				if(val < 0){
					//print_token_buff(new_buff);
					free(new_buff);
					return nullptr;
				}
				else{
					new_buff[j].mode 	= MODE_ID;
					new_buff[j].token_id 	= val;
					new_buff[j].line 	= line;
					new_buff[j].column 	= column;
					j++;
					column += column_delta;
				}		
			}
			else{
				new_buff[j].mode 	= MODE_OPER;
				new_buff[j].token_id 	= val;
				new_buff[j].line 	= line;
				new_buff[j].column 	= column;
				j++;
				column += i - save_i;
			}
		}
		else{
			new_buff[j].mode 	= MODE_CNST;
			new_buff[j].token_id 	= val;
			new_buff[j].line 	= line;
			new_buff[j].column 	= column;
			j++;
			column += i - save_i;
		}
		
	}
	new_buff[j].mode = 0;

	return new_buff;
}

Node_elem_t Token_get_N(Buff_elem_t * str, int * pc){
	
	Node_elem_t val = 0;
	bool done = false;
	while('0' <= str[*pc] && str[*pc] <= '9'){
		
		done = true;
		val  = val*10 + str[*pc] - '0';
		(*pc)++;
	}
	int i = ACCURACY;
	if(str[*pc] == '.'){

		(*pc)++;
		for(; i > 1 && '0' <= str[*pc] && str[*pc] <= '9'; i/=10){
			done = true;
			val = val*10 + str[*pc] - '0';
			(*pc)++;
		}
	
	}
	val *= i;

	if(!done){
		val = -1;
	}

	return val;
}

Node_elem_t Token_get_ID(Buff_elem_t * str, int * pc, int * column, bool is_reading_standart /*=false*/){

	Buff_elem_t * val = (Buff_elem_t *) calloc(MAX_FUNC_NAME, sizeof(Buff_elem_t));
	bool done = false;
	int pc_save  = *pc;
	int col_save = *column;

	wchar_t temp_wchar = 0;
	for(int i = 0;; i++){

		if(isspace(str[*pc]) || str[*pc] == '\0'){
			break;	
		}

		if(i >= MAX_FUNC_NAME){
			done = false;
			break;
		}

		temp_wchar =  1024 + ((unsigned char *)str)[*pc+1] - (210 - ((unsigned char *)str)[*pc])*64;

		Node_elem_t try_oper = -1;
		if(!is_reading_standart){

			try_oper = Token_get_OP(str, pc);
			if(try_oper >= 256){
				*pc-=2; // Token_get_OP does *pc++ if val >= 256
			}
			else if(try_oper >= 0){
				*pc-=1;
			}
		}
		else{
			try_oper = (str[*pc] == SEPARATOR_START || str[*pc] == '@' || str[*pc] == SEPARATOR_END) ? str[*pc] : -1;	
		}
		#undef iswalpha
		if(try_oper == -1 && iswalpha(temp_wchar)){
			if(i >= MAX_FUNC_NAME - 1){
				done = false;
				break;
			}
			val[i] = str[*pc];
			(*pc)++;
			val[++i] = str[*pc];
			(*pc)++;
			(*column)++;
			done = true;
		}
		else if(!isspace(str[*pc]) && try_oper == -1){

			val[i] = str[*pc];
			(*pc)++;
			(*column)++;
			done = true;
			
		}
		else{
			break;		
		}

	}

	if(!done){
		*pc = pc_save;
		*column = col_save;
		return -1;
	}

	int pos = list_find_by_elem(mstokens, val);
	if(pos <= 0){

		List_elem_t temp = {};
		temp.mode = MODE_ID;
		temp.token = val;
		temp.size = strlen(val);
		pos = list_add_after(mstokens, temp, mstokens->head);
	}

	return pos;
}

Node_elem_t Token_get_OP(Buff_elem_t * str, int * pc){

	int pc_save = *pc;
	Node_elem_t val = -1;
#define DEF_CPL(name, mode, equivalent, code, do) \
	else if(mode == MODE_OPER && ((equivalent < 256) ? str[*pc] == equivalent : str[*pc] == equivalent % 256 && str[*pc + 1] == (equivalent >> 8) % 256)){\
		val = ((equivalent >= 256) ? *pc = *pc + 2 : *pc = *pc + 1, equivalent);\
	}\

	if(true == false){
		abort();
	}
	#include "commands.h"
	#undef DEF_CPL
	else{
		*pc = pc_save;
		val = -1;
	}

	return val;

}

void print_token_buff(Token_elem_t * buff){
	
	assert(buff != nullptr);
	
	for(int i = 0; buff[i].mode > 0; i++){
		printf("[%4d] Line: %4d; Column: %4d; Mode: %2d; Token: %d = %c\n", i, buff[i].line, buff[i].column, buff[i].mode, buff[i].token_id, buff[i].token_id);
	}
	printf("----END-TOKEN-BUFF-----\n");
}


#undef THIS
#undef DATA

