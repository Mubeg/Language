#pragma once
#include"node.h"
#undef USING_WCHAR_T
#include"ionegin"


#define syntax_assert(cond) return nullptr; 


Node_t * Get_G(Token_elem_t * str, int * pc);


Node_t * Get_FUNC(Token_elem_t * str, int * pc);


Node_t * Get_OP(Token_elem_t * str, int * pc);


Node_t * Get_FUNC_DEF(Token_elem_t * str, int * pc);


Node_t * Get_GET(Token_elem_t * str, int * pc);


Node_t * Get_PUT(Token_elem_t * str, int * pc);


Node_t * Get_E(Token_elem_t * str, int * pc);


Node_t * Get_INIT(Token_elem_t * str, int * pc);


Node_t * Get_DERIV(Token_elem_t * str, int * pc);


Node_t * Get_IF(Token_elem_t * str, int * pc);


Node_t * Get_ELSE(Token_elem_t * str, int * pc);


Node_t * Get_WHILE(Token_elem_t * str, int * pc);


Node_t * Get_BREAK(Token_elem_t * str, int * pc);


Node_t * Get_RETURN(Token_elem_t * str, int * pc);


Node_t * Get_ASSIGN(Token_elem_t * str, int * pc);


Node_t * Get_CMP(Token_elem_t * str, int * pc);


Node_t * Get_T(Token_elem_t * str, int * pc);


Node_t * Get_POW(Token_elem_t * str, int * pc);


Node_t * Get_UNAR(Token_elem_t * str, int * pc);


Node_t * Get_P(Token_elem_t * str, int * pc);


Node_t * Get_FUNC_CALL(Token_elem_t * str, int * pc);


Node_t * Get_N(Token_elem_t * str, int * pc);


Node_elem_t Get_ID(Token_elem_t * str, int * pc);




Node_t * Get_G(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;	
	Node_t * root = nullptr;

	bool done = false;
	bool has_error = false;
	while(true){

		Node_t * val1 = Get_FUNC(str, pc);

		if(val1 == nullptr){
			break;
		}

		if(val  == nullptr){
			val = val1;
		}
		else{
			val->right = val1;
			val = val1;

		}

		if(root == nullptr){
			root = val;
		}

		done = true;
	}
	if(has_error){
		syntax_assert("Ошибка компиляции\n");
	}
	if(done == false && val == nullptr){
		syntax_assert("Программа не найдена\n");
	}
	else{
		if(str[*pc].mode == '\0'){
			return root;
		}
		else{
			syntax_assert("Конец разбора не на '\\0'\n");
		}
	}
}


Node_t * Get_FUNC(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;

	val = Get_OP(str, pc);
	if(val == nullptr){
		val = Get_FUNC_DEF(str, pc);
		if(val == nullptr){
			syntax_assert("Функция не найдена\n");
		}
	}
	return node_create_new(MODE_OPER, ';', val, nullptr);

}


Node_t * Get_OP(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	val = Get_PUT(str, pc);
	if(val == nullptr){//7
	val = Get_GET(str, pc);
	if(val == nullptr){//6
	val = Get_ASSIGN(str, pc);
	if(val == nullptr){//5
	val = Get_INIT(str, pc);
	if(val == nullptr){//4
	val = Get_IF(str, pc);
	if(val == nullptr){//3
	val = Get_WHILE(str, pc);
	if(val == nullptr){//2
	val = Get_RETURN(str, pc);
	if(val == nullptr){//1
		val = Get_E(str, pc);
		if(val != nullptr && str[*pc].mode == MODE_OPER && str[*pc].token_id == ';'){
			(*pc)++;
		}
		else{
			*pc = pc_save;
			if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '{'){
				
				(*pc)++;

				Node_t * root = nullptr;
				while(true){
					Node_t * val1 = Get_OP(str, pc);
					if(val1 == nullptr){
						break;
					}

					if(val == nullptr){
						val = val1;				
					}
					else{
						val->right = val1;
						val = val1;
					}
					if(root == nullptr){
						root = val;
					}
				}
				if(root != nullptr && str[*pc].mode == MODE_OPER && str[*pc].token_id == '}'){
					(*pc)++;
					return root;
				}
				else{
					*pc = pc_save;
					syntax_assert("Операция не найдена\n");		
				}
			}
			else{
				syntax_assert("Операция не найдена\n");
			}
		}
	}//1
	}//2
	}//3
	}//4
	}//5
	}//6
	}//7

	return node_create_new(MODE_OP, 'po', val, nullptr);

}


Node_t * Get_FUNC_DEF(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	Node_elem_t token_id = Get_ID(str, pc);
	if(token_id <= 0){
		syntax_assert("Неверное имя функции\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '('){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Не хватает (\n");
	}

	Node_elem_t temp_id = Get_ID(str, pc);
	if(temp_id > 0){

		val = node_create_new(MODE_OPER, ',', val, node_create_new(MODE_VARL, temp_id, nullptr, nullptr));
		while(true){

			if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ','){
				(*pc)++;
			}
			else{
				break;	
			}

			temp_id = -1;
			temp_id = Get_ID(str, pc);
			if(temp_id <= 0){
				*pc = pc_save;
				syntax_assert("Нет второго выражения\n");
			}
			val = node_create_new(MODE_OPER, ',', val, node_create_new(MODE_VARL, temp_id, nullptr, nullptr));
		}
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ')'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Не хватает )\n");
	}

	Node_t * val1 = Get_OP(str, pc);
	if(val1 == nullptr){
		syntax_assert("Нет тела функции\n");
	}

	return node_create_new(MODE_FUNC, token_id, val, val1);
}



Node_t * Get_GET(Token_elem_t * str, int * pc){


	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "get") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный верни\n");
	}

	Node_elem_t token_id = Get_ID(str, pc);
	if(token_id <= 0){
		syntax_assert("Неверная переменная\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ';'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет ';'\n");
	}

	return node_create_new(MODE_OP, 'teg', node_create_new(MODE_VARL, token_id, nullptr, nullptr), nullptr);

}


Node_t * Get_PUT(Token_elem_t * str, int * pc){


	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "put") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный верни\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ';'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет ';'\n");
	}

	return node_create_new(MODE_OP, 'tup', val, nullptr);

}


Node_t * Get_E(Token_elem_t * str, int * pc){


	Node_t * val = nullptr;
	
	int pc_save = *pc;

	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "deriv1") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){

		val = Get_DERIV(str, pc);
	}

	if(val == nullptr){
		val = Get_CMP(str, pc);
		int mode = -1;
		while(str[*pc].mode == MODE_OPER && (str[*pc].token_id == '+' || str[*pc].token_id == '-')){

			mode = str[*pc].token_id;
			(*pc)++;

			Node_t * val2 = Get_CMP(str, pc);
			if(val2 == nullptr){
				*pc = pc_save;
				syntax_assert("Нет второго выражения\n");
			}

			val = node_create_new(MODE_OPER, mode, val, val2);

		}
	}

	if(val == nullptr){
		*pc = pc_save;
		syntax_assert("Не найдено выражение\n");
	}

	return val;

}


Node_t * Get_INIT(Token_elem_t * str, int * pc){
	return nullptr; // Из-за стандарта нет инициализации
}


Node_t * Get_DERIV(Token_elem_t * str, int * pc){


	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "deriv1") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный если\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}

	temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "deriv2") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный если\n");
	}

	Node_elem_t token_id = Get_ID(str, pc);

	if(token_id <= 0){
		*pc = pc_save;
		syntax_assert("Нет переменой производной\n");
	}

	Node_elem_t name = 'v';
	name = (name << 8*4) + 'ired';
	if(LOG_LEVEL >= 3){printf("deriv = %lld\n", name);}
	return node_create_new(MODE_OP, name, node_create_new(MODE_VARL, token_id, nullptr, nullptr), val);
	
	return nullptr;


}


Node_t * Get_IF(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "if1") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный если\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}

	temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "if2") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный если\n");
	}

	Node_t * val1 = Get_OP(str, pc);

	if(val1 == nullptr){
		*pc = pc_save;
		syntax_assert("Нет тела если\n");
	}

	Node_t * val2 = Get_ELSE(str, pc);

	if(val2 == nullptr){
		return node_create_new(MODE_OP, 'fi', val, val1);
	}
	else{
		Node_elem_t name = 'esl';
		name = (name << 8*4) + 'e-fi';
		if(LOG_LEVEL >= 3){printf("if-else = %lld\n", name);}
		return node_create_new(MODE_OP, 'fi', val, node_create_new(MODE_OP, name, val1, val2));
	}
	
	return nullptr;

}


Node_t * Get_ELSE(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;

	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "if-else") == 0){\
			temp_name = translation;	\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный 'a'\n");
	}

	val = Get_OP(str, pc);

	if(val == nullptr){
		*pc = pc_save;
		syntax_assert("Нет тела 'a'\n");
	}
	else{
		return val;
	}
	
	return nullptr;

}


Node_t * Get_WHILE(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "while") == 0){\
			temp_name = translation;	\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный пока\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '('){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет '('\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ')'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет ')'\n");
	}

	Node_t * val1 = Get_OP(str, pc);

	if(val1 == nullptr){
		*pc = pc_save;
		syntax_assert("Нет тела если\n");
	}

	Node_elem_t name = 'e';
	name = (name << 8*4) + 'lihw';
	if(LOG_LEVEL >= 3){printf("while = %lld\n", name);}
	return node_create_new(MODE_OP, name, val, val1);
}


Node_t * Get_BREAK(Token_elem_t * str, int * pc){
	return nullptr;
}


Node_t * Get_RETURN(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	
	char * temp_name = nullptr;
	#define DEF_TRSL(name, translation) \
		if(strcmp(#name, "ret") == 0){\
			temp_name = translation;\
		}
	#include"translation.h"
	#undef DEF_TRSL

	if(str[*pc].mode == MODE_ID && strcmp(list_get(mstokens, str[*pc].token_id).token, temp_name) == 0){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный верни\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ';'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет ';'\n");
	}

	return node_create_new(MODE_OP, 'ter', val, nullptr);
}


Node_t * Get_ASSIGN(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;

	Node_elem_t token_id = Get_ID(str, pc);
	if(token_id <= 0){
		syntax_assert("Неверное имя переменной\n")
	}
	
	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '='){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Неправильный знак присваивания\n");
	}

	val = Get_E(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n");
	}
	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ';'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Нет ;\n");
	}

	return node_create_new(MODE_OPER, '=', node_create_new(MODE_VARL, token_id, nullptr, nullptr), val);
}


Node_t * Get_CMP(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	val = Get_T(str, pc);
	if(val == nullptr){
		syntax_assert("Неверное выражение\n")
	}
	while(true){
		if(str[*pc].mode == MODE_OPER && (str[*pc].token_id == '==' || str[*pc].token_id == '<' || str[*pc].token_id == '>' || str[*pc].token_id == '=!' || str[*pc].token_id == '=>' || str[*pc].token_id == '=<')){
			(*pc)++;
		}
		else{
			break;
		}
		Node_elem_t mode = str[*pc - 1].token_id;

		Node_t * val1 = Get_T(str, pc);
		if(val1 == nullptr){
			syntax_assert("Неверное выражение\n");
		}

		val = node_create_new(MODE_OPER, mode, val, val1);
	}

	return val;

}


Node_t * Get_T(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;

	int pc_save = *pc;

	val = Get_POW(str, pc);
	if(val == nullptr){
		syntax_assert("Не нашлось поддерево POW\n");
	}

	int mode = -1;
	while(str[*pc].mode == MODE_OPER && (str[*pc].token_id == '*' || str[*pc].token_id == '/')){

		mode = str[*pc].token_id;
		(*pc)++;
		Node_t * val1 = Get_POW(str, pc);
		if(val1 == nullptr){
			*pc = pc_save;
			syntax_assert("Нет второго выражения\n");
		}

		val = node_create_new(MODE_OPER, mode, val, val1);
	}

	return val;
}


Node_t * Get_POW(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;

	int pc_save = *pc;

	val = Get_UNAR(str, pc);
	if(val == nullptr){
		syntax_assert("Не нашлось поддерево UNAR\n");
	}

	int mode = -1;
	while(str[*pc].mode == MODE_OPER && str[*pc].token_id == '^'){

		mode = str[*pc].token_id;
		(*pc)++;
		Node_t * val1 = Get_UNAR(str, pc);
		if(val1 == nullptr){
			*pc = pc_save;
			syntax_assert("Нет второго выражения\n");
		}

		val = node_create_new(MODE_OPER, mode, val, val1);
	}

	return val;
}


Node_t * Get_UNAR(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;

	int pc_save = *pc;

	if(str[*pc].mode == MODE_OPER && (str[*pc].token_id == '+' || str[*pc].token_id == '-')){

		Node_elem_t mode = str[*pc].token_id;

		(*pc)++;

		Node_t * val1 = Get_P(str, pc);
		if(val1 == nullptr){
			*pc = pc_save;
			syntax_assert("Нет выражения\n");
		}

		val = node_create_new(MODE_OPER, mode, node_create_new(MODE_CNST, 0, nullptr, nullptr), val1); //LEFT != nullptr, т.к. так в стандарте
	}
	else{
		val = Get_P(str, pc);

	}

	return val;
}


Node_t * Get_P(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	Node_elem_t token_id = -1;

	int pc_save = *pc;
	val = Get_N(str, pc);
	if(val == nullptr){//1
	val = Get_FUNC_CALL(str, pc);
	if(val == nullptr){//2
	token_id = Get_ID(str, pc);
	if(token_id <= 0){//3
		if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '('){
			(*pc)++;
			
			val = Get_E(str, pc);
			if(val != nullptr && str[*pc].mode == MODE_OPER && str[*pc].token_id == ')'){
				(*pc)++;
			}
			else{
				*pc = pc_save;
				syntax_assert("Не найдено P\n");		
			}
		}
	}//3
	else{
		return node_create_new(MODE_VARL, token_id, nullptr, nullptr);
	}
	}//2
	}//1

	return val;

}


Node_t * Get_FUNC_CALL(Token_elem_t * str, int * pc){

	Node_t * val = nullptr;
	
	int pc_save = *pc;
	Node_elem_t token_id = Get_ID(str, pc);
	if(token_id <= 0){
		syntax_assert("Неверное имя функции\n");
	}

	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == '('){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Не хватает (\n");
	}

	val = Get_E(str, pc);
	if(val != nullptr){
		val = node_create_new(MODE_OPER, ',', nullptr, val);
		while(true){
			if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ','){
				(*pc)++;
			}
			else{
				break;
			}
			Node_t * val1 = Get_E(str, pc);
			if(val1 == nullptr){
				break;
			}
			val = node_create_new(MODE_OPER, ',', val, val1);
		}
	}
	if(str[*pc].mode == MODE_OPER && str[*pc].token_id == ')'){
		(*pc)++;
	}
	else{
		*pc = pc_save;
		syntax_assert("Не хватает )\n");
	}

	#define DEF_CPL(name, _mode, equiv, from_tree, to_asm) \
		if(_mode == MODE_FUNC && strcmp(name, list_get(mstokens, token_id).token) == 0){\
			return node_create_new(MODE_FUNC, token_id, nullptr, val);\
		}

	#include "../commands.h"
	#undef DEF_CPL

	return node_create_new(MODE_FUNC, token_id, val, nullptr);

}


Node_t * Get_N(Token_elem_t * str, int * pc){

	if(str[*pc].mode == MODE_CNST){
		(*pc)++;
		return node_create_new(MODE_CNST, str[*pc - 1].token_id, nullptr, nullptr);
	}

	return nullptr;

}


Node_elem_t Get_ID(Token_elem_t * str, int * pc){
	
	if(str[*pc].mode == MODE_ID){
		(*pc)++;
		return str[*pc - 1].token_id;
	}
	return -1;

}

#undef syntax_assert

