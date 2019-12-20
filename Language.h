#include "includes/test-log.define"

#undef  TEST_LEVEL
#define TEST_LEVEL 0
#undef   LOG_LEVEL
#define  LOG_LEVEL 2

#include "includes/node.h"
//#define USING_WCHAR_T
#include "includes/ionegin"


const char TEX_FILE[] 		= "tex_file.tex";
const char TEX_FILE_SUB[] 	= "tex_file";
const int  MAX_FUNC_NAME 	= 100;
const int ACCURACY		= 1000;
const int MAX_VARIABLE_NUM	= 100;

const int __MODE_ZERO 		= __LINE__;
const int MODE_GLOBAL_INIT	= __LINE__ - __MODE_ZERO;
const int MODE_FUNC_DEF		= __LINE__ - __MODE_ZERO;


struct Var_elem_t{

	Node_elem_t data;
	long long int pos;
	int is_local;
};


typedef struct Multi_symbol_token_t{
	
	int mode;
	Buff_elem_t *token;
	int size;

} List_elem_t;

struct Token_elem_t{
	
	int line;
	int column;
	int mode;
	Node_elem_t token_id;

};

#include "includes/List.h"

List_t * mstokens = nullptr;

#include "includes/RDSft.h"

#define DATA   node->data


void compile_tree_to_asm_file(Node_t * node, FILE * file, Var_elem_t *variables, int __mode =-1);


Var_elem_t *variables_copy(Var_elem_t *variables);


long long int variables_find(Var_elem_t *variables, Node_elem_t pos);


long long int variables_find_free(Var_elem_t *variables);


long long int variables_count_global(Var_elem_t *variables);


#include "includes/Differentiator.h"


void translate_tree_to_file(Node_t * node, FILE * file, int tabulation);


void log_tree_to_file(Node_t * node, FILE * file);


void color_node(Node_t * node, FILE * file);


Node_t * node_make_from_tree_file(FILE * file);


Node_t * node_make_from_tree_buff(Node_t * node, Buff_elem_t * buff, const int buff_size, int *pc);


Node_t * node_make_from_file_RDS_with_tokenization(FILE * file);


Token_elem_t * language_file_tokenize_to_new_buff(Buff_elem_t * buff, size_t buff_size);


Node_elem_t Token_get_N(Buff_elem_t * str, int * pc);


Node_elem_t Token_get_ID(Buff_elem_t * str, int * pc, int * column, bool is_reading_standart = false);


Node_elem_t Token_get_OP(Buff_elem_t * str, int * pc);


void print_token_buff(Token_elem_t * buff);


#define DATA node->data
#define THIS node


