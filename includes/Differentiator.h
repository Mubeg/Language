#define DATA node->data

Node_t * differentiate_to_new(Node_t * node, Node_elem_t cur_diff = -1);


Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff = -1);


bool node_tree_optimize(Node_t * node);


Node_t * differentiate_to_new(Node_t * node, Node_elem_t cur_diff/* = -1*/){

	Node_t * root = do_diff_recursive(node, cur_diff);
	return root;
}

Node_t * do_diff_recursive(Node_t * node, Node_elem_t cur_diff /* = -1*/){

	if(node == nullptr){
		return nullptr;
	}

	#define DEF_DIF(name, _mode, equal, command, do) \
							else if((node->mode == _mode) && (node->mode != MODE_OPER || equal == DATA)\
								&& (node->mode != MODE_FUNC || list_find_by_elem(mstokens, #name) == DATA)){\
								command;\
					      		}

	if(true == false){assert(true == false);}
	#include"diff_commands.h"
	else{

		fprintf(stderr, "No such command found " NODE_ELEM_PRINT "\n", DATA);
		return nullptr;
	}

	#undef DEF_DIF
}

bool node_tree_optimize(Node_t * node){


	if(node == nullptr){
		return false;
	}

	bool optimized = false;

	#define DEF_OPTIM(condition, do)	if(THIS->mode == MODE_OPER && LEFT != nullptr && condition){\
							optimized = true;\
							do;\
						}
	#include"diff_optims.h"
	#undef DEF_OPTIM

	#define DEF_DIF(name, _mode, equal, command, do)	else if(equal == DATA || list_find_by_elem(mstokens, #name) == DATA){\
									optimized = true;\
									\
									THIS->mode = MODE_CNST;\
									do;\
									\
									node_deinit(LEFT);\
									node_deinit(RIGHT);\
									LEFT = nullptr;\
									RIGHT = nullptr;\
			      					}

	if(optimized || (LEFT != nullptr && LEFT->mode != MODE_CNST) || RIGHT == nullptr || RIGHT->mode != MODE_CNST ||
	   (THIS->mode != MODE_OPER && THIS->mode != MODE_FUNC)){
		0;
	}
	#include"diff_commands.h"
	else{
		assert(("unexpected situation\n", false) == true);
	}
	#undef DEF_DIF


	bool temp1 = node_tree_optimize(LEFT );
	bool temp2 = node_tree_optimize(RIGHT);

	optimized = optimized || temp1 || temp2;

	return optimized;
}

#undef DATA





