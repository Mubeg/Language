#pragma once
//#define USING_WCHAR_T
#include "test-log.define"
#include "ionegin"

#define IS_ANS(arg) strcasecmp(ans, Long_string((arg))) == 0

#define asserted || (fprintf(stderr, "Asserted from %s on %d in %s\n", __LOCATION__), abort(), false);
//#define node_assert(condition) if(!(condition)){fprintf(stderr, "File %s line %d function %s %s\n", __LOCATION__, #condition); abort();}
#define node_assert(condition)

#define check_parent(arg) arg->parent == nullptr ? false : arg->parent->right == arg 

#define NODE_INIT(name, ...) node_init((name), #name, ##__VA_ARGS__)

#define NODE_ADD(DIR, elem) 	if(DIR != nullptr && DIR != THIS){return nullptr;}\
				DIR = node_create_new(-1, elem, nullptr, nullptr, (DIR != THIS) ? THIS : nullptr);\
				THIS->hash = find_sum_hash(THIS, sizeof(Node_t));


#define PC_N (*pc)
#define PC_INCR (*pc)+=1

#define THIS   node
#define DATA   node->data
#define LEFT   node->left
#define RIGHT  node->right
#define PARENT node->parent

//#define TESTING

#ifdef TESTING
#define NDEBUG
#endif
#include <assert.h>

typedef long long int Node_elem_t;
#define NODE_ELEM_PRINT_VALUE "%lli"
#define NODE_ELEM_PRINT_FUNCTION "%s" 
#define NODE_ELEM_PRINT "%c" 

typedef unsigned long long int Canary_t;
#define CANARY_PRINT "%LX"

typedef unsigned long long int hash_t;
#define HASH_PRINT "%Lu"

struct Node_t {

	hash_t hash; // always comes first
	const char *name;

	Canary_t canary1;

	Node_elem_t data;
	Node_t * left;
	Node_t * right;
	Node_t * parent;
	int level;
	int mode;

	int errnum;

	Canary_t canary2;

};


const int CHECK_ZERO_ = __LINE__;
const int CHECK_CANARY             = 1 <<  (__LINE__ - CHECK_ZERO_ - 1); //= 1;
const int CHECK_HASH               = 1 <<  1; //= 2;
const int CHECK_SIZE               = 1 <<  2; //= 4;
const int BUFF_NULLPTR             = 1 <<  3; //= 8;
const int NAME_NULLPTR             = 1 <<  4; //= 16;
const int CANARY_DYNAMIC1_NULLPTR  = 1 <<  5; //= 32;
const int CANARY_DYNAMIC2_NULLPTR  = 1 <<  6; //= 64;
const int DATA_NULLPTR             = 1 <<  7; //= 128;
const int NEG_SIZE_MAXSIZE         = 1 <<  8; //= 256;
const int CHECK_DYN_CAN_AND_DATA   = 1 <<  9; //= 512;
const int NEG_SIZE_FALSE_POISON    = 1 << 10; //= 1024;
const int CHECK_NUM_POISON         = 1 << 11; //= 2048;
const int MAX_ERROR_NO 		   = (__LINE__ - CHECK_ZERO_ - 1);


const bool DEBUG 			= 0;
const Canary_t CANARY_VALUE 		= 0xBEDAAAAAAAAAAAAA;
const long long int POISON 		= 'IKIN';
const long long int POISON_POISON 	= '3DED';
const char LOG_FILE[]   		= "node_log.txt";
const char IMAGE_OUT[]  		= "Graph.pdf";	
const char IMAGE_OUT_2[]  		= "Graph_1.pdf";	
const char TEMPFILE[] 			= "tempfile.txt";
const char TREE_FILE[]			= "tree_file.tree";
const int MAX_COMMAND_LEN 		= 100;
const int END_FUNCTION 			= 0;
const char SEPARATOR_START		= '{';
const char SEPARATOR_END		= '}';


const int _MODE_ZERO 			= __LINE__;
const int MODE_VARL			= __LINE__ - _MODE_ZERO;
const int MODE_CNST			= __LINE__ - _MODE_ZERO;
const int MODE_OPER			= __LINE__ - _MODE_ZERO;
const int MODE_OP			= __LINE__ - _MODE_ZERO;
const int MODE_FUNC			= __LINE__ - _MODE_ZERO;
const int MODE_ID			= __LINE__ - _MODE_ZERO;

const int MAX_PRIORITY 			= 10;
const int MIN_PRIORITY 			= -1;


//Node_tex(node_done);

void node_set_parents(Node_t * node);


Node_t * node_copy(Node_t * node);


Node_t * node_create_new(const int mode, const Node_elem_t elem, Node_t * left, Node_t * right, Node_t * parent = nullptr);


void node_write_to_file(Node_t * node, FILE * file);


void node_write_to_file_less_brackets(Node_t * node, FILE * file);


Node_t * node_make_from_file(const char file_name[]);


//Node_t * node_make_from_buff_with_RDS(Buff_elem_t * buff, const int buff_size, int *pc);


Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc);


Node_elem_t get_num(Buff_elem_t * str, int * pc);


int get_mode(str_ptr str);


bool get_data_to_new(Buff_elem_t * buff, const int buff_size, int *pc, str_ptr str);


bool node_init(Node_t *node, const char name[], Node_t *parent);


Node_t *node_add_right(Node_t *node, Node_elem_t elem);


Node_t *node_add_left(Node_t *node, Node_elem_t elem);


Node_t * node_find(Node_t *node, str_ptr cmp_elem);


bool node_deinit(Node_t *node);


Node_elem_t node_get(Node_t *node);


bool node_set(Node_t *node, Node_elem_t elem, const int mode);


static bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size);


static void zero_data(void * const data, const int start, const int end);


bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all = true);


void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime = -1);


void dot_node(Node_t * root, const char ofile[]);


void color_node(Node_t * node, FILE * file);


void node_dot_dependences(Node_t * node, FILE * file);


static bool check_nullptr(const void * const node);


static bool check_canary(Node_t * const node);


static bool check_hash(Node_t * const node);


static bool check_hashable(Node_t * const node);


static bool is_equal_hash_t(const hash_t left, const hash_t right);


static hash_t find_sum_hash(Node_t * const node, size_t size);


static hash_t hash(const void * const node, size_t size);


static bool check_size(Node_t * const node);


static int clear_stdin(void);

//#include "RDSft.h"





void node_set_parents(Node_t * node){
	
	if(THIS == nullptr){
		return;
	}

	if(LEFT  != nullptr) {LEFT->parent  = THIS;}
	if(RIGHT != nullptr) {RIGHT->parent = THIS;}

	node_set_parents(LEFT );
	node_set_parents(RIGHT);
}

Node_t * node_copy(Node_t * node){

	if(node == nullptr){
		return nullptr;
	}

	return node_create_new(node->mode, node->data, node_copy(node->left), node_copy(node->right), node->parent);
	
}

Node_t * node_create_new(const int mode, const Node_elem_t elem, Node_t * left, Node_t * right, Node_t * parent /* = nullptr */){

	
	Node_t * new_node = nullptr;

	new_node = (Node_t *) calloc(1, sizeof(Node_t));
	NODE_INIT(new_node, nullptr);
	node_set(new_node, elem, mode);

	new_node->parent = parent;
	new_node->left  = left;
	new_node->right = right;

	return new_node;

}

void node_write_to_file(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;
	}

	node_assert(check_nullptr(THIS));

	node_write_to_file(LEFT, file);

	fprintf(file, NODE_ELEM_PRINT, SEPARATOR_START);
	fprintf(file, "\"" NODE_ELEM_PRINT "\"", DATA);

	node_write_to_file(RIGHT, file);

	fprintf(file, NODE_ELEM_PRINT, SEPARATOR_END);

}

void node_write_to_file_less_brackets(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;
	}

	node_assert(check_nullptr(THIS));

	int first_priority = MIN_PRIORITY;
	int second_priority = MAX_PRIORITY;

	#define DEF_OPER(name, equal, prior)	else if(PARENT->data == equal){\
							first_priority = prior;\
						}
	if(PARENT == nullptr || PARENT->mode != MODE_OPER){
		0;
	}
	#include"opers.h"
	else{
		assert(("unexpected situation\n", false) == true);
	}
	#undef DEF_OPER

	#define DEF_OPER(name, equal, prior)	else if(DATA == equal){\
							second_priority = prior;\
						}
	if(THIS->mode != MODE_OPER){
		0;
	}
	#include"opers.h"
	else{
		assert(("unexpected situation\n", false) == true);
	}
	#undef DEF_OPER

	if(first_priority - second_priority > 0){
		fprintf(file, NODE_ELEM_PRINT, SEPARATOR_START);
	}

	node_write_to_file_less_brackets(LEFT, file);

	if(THIS->mode == MODE_CNST){
		fprintf(file, NODE_ELEM_PRINT_VALUE, DATA);
	}
	else{
		fprintf(file, NODE_ELEM_PRINT, DATA);
	}

	node_write_to_file_less_brackets(RIGHT, file);

	if(first_priority - second_priority > 0){
		fprintf(file, NODE_ELEM_PRINT, SEPARATOR_END);
	}

}

/*Node_t * node_make_from_file(FILE * file){

	assert(file != nullptr);

	int buff_size = 0;
	Buff_elem_t * buff = read_file_to_created_buff_opened_file(file, &buff_size);

	int pc = 0;
	Node_t * root = node_make_from_buff(nullptr, buff, buff_size, &pc);
	free(buff);

	return root;
}*/

/*Node_t * node_make_from_buff_with_RDS(Buff_elem_t * buff, const int buff_size, int *pc){

	buff[buff_size - 2] = '\0';

	return Get_G(buff, pc);
}*/

/*Node_t * node_make_from_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc){

	assert(buff != nullptr);
	assert(pc   != nullptr);

	if(*pc >= buff_size){
		return nullptr;
	}

	if(buff[PC_N] == SEPARATOR_START){

		
		PC_INCR;

	}
	else{
		return nullptr;
	}



	LEFT  = node_make_from_buff(THIS, buff, buff_size, pc);


	str string = {};
	if(!get_data_to_new(buff, buff_size, pc, &string)){
		return nullptr;
	}	
	int mode = get_mode(&string);

	assert(string.str != nullptr);

	Node_elem_t elem = string.str[0];
	int pc_temp = 0;

	if(mode == MODE_CNST){
		elem = get_num(string.str, &pc_temp);
	}

	node_set(THIS, elem, mode);


	RIGHT = node_make_from_buff(THIS, buff, buff_size, pc);


	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	if(buff[PC_N] == SEPARATOR_END){
		PC_INCR;
		return node;
	}

	free(string.str);

	return nullptr;
}*/

Node_elem_t get_num(Buff_elem_t * str, int * pc){

	assert(str != nullptr);
	
	Node_elem_t val = 0;
	bool done = false;
	while('0' <= str[*pc] && str[*pc] <= '9'){
		
		done = true;
		val  = val*10 + str[*pc] - '0';
		(*pc)++;
	}
	if(!done){
		assert(("Get_N", false) == true);
	}

	return val;
}

int get_mode(str_ptr string){
	
	if(string == nullptr || string->size <= 0){
		return -1;
	}

#define DEF_OPER(name, equal, prior) 	else if(equal == string->str[0]){\
								return MODE_OPER;\
							}
						 	
	if(is_digits(string)){
		return MODE_CNST;
	}
	#include"opers.h"
	else{
		return MODE_VARL;
	}

#undef DEF_DIF
	
	return -1;
	
}

bool get_data_to_new(Buff_elem_t * buff, const int buff_size, int *pc, str_ptr string){

	assert(string != nullptr);

	Buff_elem_t *temp_buff = (Buff_elem_t *) calloc(sizeof(Buff_elem_t), MAX_LINE_LEN);
	int temp = 0;
	int ans = sscanf(buff + *pc, Long_string("%[^()]%n"), temp_buff, &temp);

	if(ans == 0){
		free(temp_buff);
		return false;
	}
	*pc += temp;

	string->str = temp_buff;
	string->size = strlen(temp_buff);

	return true;

}


bool node_init(Node_t *node, const char name[], Node_t *parent){

	node_assert(check_nullptr(THIS));

	THIS->canary1 = CANARY_VALUE;

	LEFT   = nullptr;
	RIGHT  = nullptr;
	PARENT = parent;
	
	THIS->level = parent == nullptr ? 0 : parent->level + 1;

	THIS->errnum = 0;

	THIS->mode = -1;

	THIS->name = name[0] == '&' ? name + 1 : name;

	THIS->canary2 = CANARY_VALUE;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));
 
	return true;
}

Node_t *node_add_right(Node_t *node, Node_elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(RIGHT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t *node_add_left(Node_t *node, Node_elem_t elem){

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * new_node = nullptr;
	NODE_ADD(LEFT, elem);
	
	node_assert(check_node(THIS, __LOCATION__));

	return new_node;
}

Node_t * node_find(Node_t *node, str_ptr cmp_elem){

	if(cmp_elem == nullptr || cmp_elem->size < 0){
		return nullptr;
	}
	
	if(THIS == nullptr){
		return nullptr;	
	}

	if(cmp_elem->size > MAX_LINE_LEN){	
		cmp_elem->str[MAX_LINE_LEN] = '\0';
		cmp_elem->size = MAX_LINE_LEN;
	}

	if(DATA, cmp_elem->str[0] == 0){
		return THIS;
	}

	node_assert(check_node(THIS, __LOCATION__));

	Node_t * ans = node_find(LEFT, cmp_elem);
	if(ans != nullptr){
		return ans;	
	}

	ans = node_find(RIGHT, cmp_elem);
	
	return ans;
}

bool node_deinit(Node_t *node){

	if(THIS == nullptr){
		return true;
	}

	node_assert(check_node(THIS, __LOCATION__));

	node_deinit(LEFT);
	node_deinit(RIGHT);
	
	zero_data(THIS, 0, sizeof(Node_t));
	free(THIS);

	return true;
}


Node_elem_t node_get(Node_t *node){

	node_assert(check_node(THIS, __LOCATION__));
	
	return DATA;
}


bool node_set(Node_t *node, Node_elem_t elem, const int mode){

	node_assert(check_node(THIS, __LOCATION__));

	node->mode = mode;

	DATA = elem;

	THIS->hash = find_sum_hash(THIS, sizeof(Node_t));

	node_assert(check_node(THIS, __LOCATION__));

	return true;
}

bool fill_data_with_poison(void * data, const size_t size, long long int poison, const size_t Elem_size){
	
	node_assert(data != nullptr);

	for(unsigned int i = 0; i < size * Elem_size; i++){
		((char *)data)[i] = ((char *) &poison)[i % Elem_size];
	}

	return true;

}

void zero_data(void * const data, const int start, const int end){

	node_assert(data != nullptr);
	node_assert(start < end);	

	for(int i = start; i < end; i++){
		((char *) data)[i] = '\0';
	}

}

bool check_node(Node_t *node, const char called_from_file[], int line, const char func[], bool print_all/* = true*/){


	bool passed = true;
	bool hashable = true;

	if(!check_nullptr(THIS)){
		return false;
	}

	if(THIS->errnum != 0){
		return false;	
	}

// A Lot of checks below
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_canary(THIS)                     ? passed : (THIS->errnum += CHECK_CANARY, false);
	if(DEBUG) printf("Passed: %d\n", passed);

	passed = check_hash(THIS)		        ? passed : (THIS->errnum += CHECK_HASH, false);
	if(DEBUG) printf("Passed: %d\n", passed);
 
	passed = check_nullptr(THIS->name)              ? passed : (THIS->errnum += NAME_NULLPTR, false);
	if(DEBUG) printf("Passed: %d\n", passed);

/*	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);
	passed = check_canary(THIS)? passed : (THIS->errnum = 1, false);*/

	if(DEBUG) printf("\n");

// End of a Lot of checks

	if(print_all || !passed){

		node_dump(THIS, called_from_file, line, func, passed);
	}

	return passed;
}


#undef fprintf

void node_dump(Node_t * const node, const char called_from_file[], int line, const char func[], bool ok, time_t curtime /* = -1*/){

	if(THIS == nullptr) return;

	bool cur_time_setted = true;

	if(curtime == -1){
		time(&curtime);	
		cur_time_setted = false;
	}

	FILE *log_file = fopen(LOG_FILE, "a");
	if(log_file == nullptr){
		fprintf(stderr, "Not able to open %s in %s line %d in %s\n", LOG_FILE, __LOCATION__);
	}


	fprintf(log_file, (cur_time_setted) ? "%s" : "\nTime and date: %s\n"
			  "Dumping node from file %s, line %d, function %s [%s]\n"
			  "node [%p] %s [%p] {\n"
			  "\n\t hash    = " HASH_PRINT "\n"
			  "\n\t canary1 = " CANARY_PRINT "\n"
			  "\t data    = [%p] {" NODE_ELEM_PRINT "}\n"
			  "\t right   = [%p]\n"
			  "\t left    = [%p]\n"
			  "\t parent  = [%p]\n",
		(cur_time_setted) ? "" : ctime(&curtime),
		called_from_file, line, func, ok ? "ok" : "ERROR!!!",
		THIS, THIS->name, THIS->name,
		THIS->hash,
		THIS->canary1,
		DATA, DATA,
		RIGHT,
		LEFT,
		PARENT);
	fflush(log_file);
	
	fprintf(log_file, "\n\t errnum   = ");
	for(int i = 0; i < MAX_ERROR_NO; i++){
		fprintf(log_file, "%d", (THIS->errnum >> i) % 2);
	}
	fprintf(log_file, "\n");
	fflush(log_file); 

	fprintf(log_file, "\n\t canary2 = " CANARY_PRINT "\n\n}\n\n",
		THIS->canary2);

	fflush(log_file);

	fclose(log_file);

}

void dot_node(Node_t * root, const char ofile[]){

	node_assert(check_node(root, __LOCATION__));
	assert(root != nullptr);

	FILE *file = fopen(TEMPFILE, "wb");
	if(file == nullptr){

		fprintf(stderr, "Unable to open file %s\n", ofile);	
		return;
	}

	fprintf(file, "digraph %s{\n", root->name);

	color_node(root, file);
	
	node_dot_dependences(root, file);	

	fprintf(file, "}\n");

	fclose(file);
	
	char command[MAX_COMMAND_LEN];

	sprintf(command, "dot -Tpdf %s -o %s\n", TEMPFILE, ofile);
	std::system(command);

	sprintf(command, "rm %s\n", TEMPFILE);
	//std::system(command);
	
}

/*void color_node(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	char color[MAX_COMMAND_LEN] = {};
	sprintf(color, "color = \"%s\"", THIS->mode == MODE_OPER ? "yellow"  :
					(THIS->mode == MODE_VARL ? "red"     :
					(THIS->mode == MODE_CNST ? "green"   :
					(THIS->mode == MODE_FUNC ? "blue"    : "black"))));

	if(THIS->mode == MODE_CNST)
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT_VALUE    "\"]\n", THIS, color, DATA);

	else if(THIS->mode == MODE_FUNC)
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT_FUNCTION "\"]\n", THIS, color, list_get(mstokens, DATA).token);
	else
		fprintf(file, "%d [%s; shape = record; label =\"" NODE_ELEM_PRINT          "\"]\n", THIS, color, DATA);

	color_node(LEFT, file);
	color_node(RIGHT, file);

}*/

void node_dot_dependences(Node_t * node, FILE * file){

	if(THIS == nullptr){
		return;	
	}

	if(PARENT != nullptr){
		fprintf(file, "%d -> %d\n", THIS, PARENT);
	}

	if(LEFT   != nullptr){
		fprintf(file, "%d -> %d\n", THIS, LEFT);
	}
	if(RIGHT  != nullptr){
		fprintf(file, "%d -> %d\n", THIS, RIGHT);
	}
	
	node_dot_dependences(LEFT, file);
	node_dot_dependences(RIGHT, file);
}

static bool check_nullptr(const void * const data){
	
	return data != nullptr;
}

bool check_canary(Node_t * const node){

	if(THIS == nullptr) return false;
	return THIS->canary1 == THIS->canary2 && THIS->canary2 == CANARY_VALUE;
}

bool check_hash(Node_t * const node){

	if(!check_hashable(THIS)) return false;
	return is_equal_hash_t(find_sum_hash(THIS, sizeof(Node_t)), THIS->hash);
}

bool check_hashable(Node_t * const node){
	
	return true;
}

bool is_equal_hash_t(const hash_t left, const hash_t right){
	
	return left == right;
}

hash_t find_sum_hash(Node_t * const node, size_t size){

	if(THIS == nullptr) return false;

	hash_t hash_sum = 0;
	
	hash_sum += hash(((char *)THIS) + sizeof(hash_t), size - sizeof(hash_t));

	return hash_sum;

}

hash_t hash(const void * const data, size_t size){

	if(data == nullptr) return false;

	hash_t hash_sum = 0;

	for(size_t counter = 0; counter < size; counter++){
		
		hash_sum = (hash_sum << 2) + (counter+1)*((*((char *) data + counter))^0xfc);
	}
	
	return hash_sum;

}


int clear_stdin(void){

	int did = 0;
	Node_elem_t c = fgetc(stdin);

	while(c != Long_string('\n') && c != EOF){
		did = 1;
		c = fgetc(stdin);
	}

	return did;	
}



#undef DATA




