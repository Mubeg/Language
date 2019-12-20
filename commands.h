//DEF_CPL(name, mode, wchar_t equivalent, from_tree, to_asm)

#define PRINT(arg) fprintf(file, arg)
#define TRANSLATE(arg, tab) translate_tree_to_file(arg, file, tabulation + tab) 
#define TAB(delta) print_tab_to_file(tabulation - delta, file)
#define COMPILE(arg) compile_tree_to_asm_file(arg, file, variables)
#define VAR(arg) long long int pos = variables_find(variables, arg->data); if(pos <= 0){pos = variables_find_free(variables); variables[pos].data = arg->data; variables[pos].pos = pos - ((__mode != MODE_GLOBAL_INIT) ? 0 : variables_count_global(variables)); variables[pos].is_local = (__mode == MODE_GLOBAL_INIT) ? 0 : 1;} fprintf(file, "push %d\n", variables[pos].pos); if(variables[pos].is_local) fprintf(file, "push bp\nadd\n");
#define COPY_VAR did_alloc = true; variables = variables_copy(variables);

DEF_CPL("@const@", MODE_CNST, '\0', 
	fprintf(file, "%d", DATA/ACCURACY);
	if(DATA % ACCURACY != 0){fprintf(file, ".%d", DATA % ACCURACY);}, 

	fprintf(file, "push %d\n", DATA);)

DEF_CPL("@variable@", MODE_VARL, '\0', 
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token);, 
	
	long long int pos = variables_find(variables, DATA); 
	(variables[pos].is_local) ? fprintf(file, "push %d\npush bp\nadd\npop ax\npop [ax]\n", variables[pos].pos) : 
				    fprintf(file, "pop [%d]\n", variables[pos].pos))


DEF_CPL("sin", MODE_FUNC, '\0', 
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token);
	PRINT("("); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	PRINT("sin\n");)

DEF_CPL("sqrt", MODE_FUNC, '\0', 
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token); 
	PRINT("("); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	PRINT("sqrt\n");)

DEF_CPL("cos", MODE_FUNC, '\0', 
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token); 
	PRINT("("); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	PRINT("cos\n");)

DEF_CPL("ln", MODE_FUNC, '\0', 
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token); 
	PRINT("("); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	PRINT("ln\n");)

DEF_CPL("@function@", MODE_FUNC, '\0',
	fprintf(file, NODE_ELEM_PRINT_FUNCTION, list_get(mstokens, DATA).token); 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(")"); 
	RIGHT == nullptr ? 0 : (PRINT("{\n\n"), TRANSLATE(RIGHT, 1), PRINT("\n}\n")),

	if(__mode == MODE_FUNC_DEF){
		fprintf(file, "jmp %s_end\n\n%s:\npop dx\n", list_get(mstokens, DATA).token, list_get(mstokens, DATA).token);
		compile_tree_to_asm_file(LEFT, file, variables, MODE_FUNC_DEF);
		COMPILE(RIGHT); 
		fprintf(file, "push dx\nreturn\nend\n%s_end:\n", list_get(mstokens, DATA).token);
	}
	else{
		PRINT("push dx\n"); 
		COMPILE(LEFT);
		fprintf(file, "push 10\npush bp\nadd\npop bp\ncall %s\n", list_get(mstokens, DATA).token); 
		PRINT("pop dx\npush ax\n");
	})

DEF_CPL("(", MODE_OPER, '(', 0, 0)

DEF_CPL(")", MODE_OPER, ')', 0, 0)

DEF_CPL(",", MODE_OPER, ',', 
	TRANSLATE(LEFT, 0); 
	(LEFT == nullptr) ? PRINT("") : PRINT(", "); 
	TRANSLATE(RIGHT, 0);, 

	__mode == MODE_FUNC_DEF ? 0 : (COMPILE(LEFT), 0);
	if(__mode == MODE_FUNC_DEF){
		VAR(RIGHT); 
		PRINT("pop ax\npush [ax]\n"); 
		compile_tree_to_asm_file(LEFT, file, variables, MODE_FUNC_DEF);
	} 
	else{
		COMPILE(RIGHT);
	})

DEF_CPL("{", MODE_OPER, '{', 0, 0)

DEF_CPL("}", MODE_OPER, '}', 0, 0)

DEF_CPL("@", MODE_OPER, '@', 0, 0)


DEF_CPL("==", MODE_OPER, '==', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" == "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "je metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)


DEF_CPL("!=", MODE_OPER, '=!', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" != "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "jne metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)


DEF_CPL("<=", MODE_OPER, '=<', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" <= "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "jbe metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)


DEF_CPL(">=", MODE_OPER, '=>', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" >= "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "jae metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)
DEF_CPL(">", MODE_OPER, '>', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" > "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "ja metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)


DEF_CPL("<", MODE_OPER, '<', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" < "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 

	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	fprintf(file, "jb metka_%u_1\njmp metka_%u_2\nmetka_%u_1:\npush 1\njmp metka_%u_3\nmetka_%u_2:\npush 0\nmetka_%u_3:\npush 0\npop\n", node, node, node, node, node, node);)


DEF_CPL("if", MODE_OP, 'fi', 
	PRINT("если "); 
	TRANSLATE(LEFT, 0); 
	PRINT(" то{\n"); 
	RIGHT->data == 28555882038912617 ? (TRANSLATE(RIGHT, 0), 0) : 
					   (TRANSLATE(RIGHT, 1), TAB(0), PRINT("}\n"));, 
	
	COMPILE(LEFT); 
	fprintf(file, "push 0\nje metka_%u_1\n", RIGHT->data == 28555882038912617 ? RIGHT : THIS);
	RIGHT->data == 28555882038912617 ? (COMPILE(RIGHT->left), fprintf(file, "jmp metka_%u_2\n", RIGHT), COMPILE(RIGHT), 0) :
					   (COMPILE(RIGHT), fprintf(file, "metka_%u_1:\npush 0\npop\n", THIS));) 


DEF_CPL("if-else", MODE_OP, 28555882038912617, 

	TRANSLATE(LEFT, 1); 
	TAB(0); 
	PRINT("}\n"); 
	TAB(0); 
	PRINT("а ");  
	RIGHT != nullptr ? 
	RIGHT->left != nullptr ?
	(RIGHT->left->data == 'fi') ?
	(TRANSLATE(RIGHT->left, 0), PRINT("")) :
	(PRINT("{\n"), TRANSLATE(RIGHT, 1), TAB(0), PRINT("}\n")) :
	(PRINT("{\n"), TRANSLATE(RIGHT, 1), TAB(0), PRINT("}\n")) : 
	(PRINT("{\n"), TRANSLATE(RIGHT, 1), TAB(0), PRINT("}\n"));

, 
	
	fprintf(file, "metka_%u_1:\npush 0\npop\n", THIS); 
	COMPILE(RIGHT); 
	fprintf(file, "metka_%u_2:\npush 0\npop\n", THIS);) //'esle-fi' = 28555882038912617


DEF_CPL("ret", MODE_OP, 'ter', 
	PRINT("верни "); 
	TRANSLATE(LEFT, 0); 
	PRINT(";\n");, 
	
	COMPILE(LEFT); 
	PRINT("push 10\npush bp\nsub\npop bp\npop ax\npush dx\nreturn\nend\n");)


DEF_CPL("op", MODE_OP, 'po', 
	TAB(0); 
	TRANSLATE(LEFT, 0); 
	LEFT->mode == MODE_OP ? PRINT("") : PRINT(";\n"); 
	TRANSLATE(RIGHT, 0); 
	LEFT->data == 508575245668 ? PRINT(";\n") : 0, 

	COPY_VAR; 
	COMPILE(LEFT); 
	(LEFT->mode == MODE_FUNC || (LEFT->mode == MODE_OP && LEFT->data != 508575245668) || (LEFT->mode == MODE_OPER && LEFT->data == '=')) ?
		0 : PRINT("pop\n"); COMPILE(RIGHT);)


DEF_CPL("while", MODE_OP, 435610544247, // 'elihw' = 435610544247
	PRINT("пока("); 
	TRANSLATE(LEFT, 0); 
	PRINT("){\n"); 
	TRANSLATE(RIGHT, 1); 
	TAB(0); 
	PRINT("}\n");, 

	fprintf(file, "metka_%u_1:\n", THIS); 
	COMPILE(LEFT); 
	fprintf(file, "push 0\nje metka_%u_2\n", THIS); 
	COMPILE(RIGHT); 
	fprintf(file, "jmp metka_%u_1\nmetka_%u_2:\n", THIS, THIS);)  

DEF_CPL(";", MODE_OPER, ';', 
	TRANSLATE(LEFT, 0); 
	PRINT("\n"); 
	TRANSLATE(RIGHT, 0);, 

	(LEFT->mode == MODE_FUNC) ? compile_tree_to_asm_file(LEFT, file, variables, MODE_FUNC_DEF) :
	(LEFT->left->mode == MODE_OPER && LEFT->left->data == '=') ? compile_tree_to_asm_file(LEFT->left, file, variables, MODE_GLOBAL_INIT) :
		COMPILE(LEFT); 
		COMPILE(RIGHT); 
		PRINT("call main\nend\n");)


DEF_CPL("=", MODE_OPER, '=', 
	TRANSLATE(LEFT, 0); 
	PRINT(" = "); 
	TRANSLATE(RIGHT, 0);, 

	COMPILE(RIGHT); 
	VAR(LEFT); 
	PRINT("pop ax\npush [ax]\n");)


DEF_CPL("+", MODE_OPER, '+', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" + "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	PRINT("add\n");)


DEF_CPL("-", MODE_OPER, '-', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT(" - "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	PRINT("sub\n");)


DEF_CPL("*", MODE_OPER, '*', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT("*"); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	PRINT("fmul\n");)


DEF_CPL("/", MODE_OPER, '/', 
	PRINT("("); 
	TRANSLATE(LEFT, 0); 
	PRINT("/"); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	PRINT("fdiv\n");)


DEF_CPL("^", MODE_OPER, '^', 
	PRINT("("); 
	TRANSLATE(LEFT, 0);
	PRINT("^"); 
	TRANSLATE(RIGHT, 0); 
	PRINT(")");, 
	
	COMPILE(RIGHT); 
	COMPILE(LEFT); 
	PRINT("pow\n");)


DEF_CPL("get", MODE_OP, 'teg', 
	PRINT("введи "); 
	TRANSLATE(LEFT, 0); 
	PRINT(";\n"), 
	
	PRINT("fin\n"); 
	VAR(LEFT); 
	PRINT("pop ax\npush [ax]\n");)


DEF_CPL("put", MODE_OP, 'tup', 
	PRINT("напечатай ");
	TRANSLATE(LEFT, 0); 
	PRINT(";\n"), 
	
	COMPILE(LEFT); 
	PRINT("fout\npush 10\ncout\n");)


DEF_CPL("deriv", MODE_OP, 508575245668, 
	PRINT("производная "); 
	TRANSLATE(RIGHT, 0); 
	PRINT(" по "); 
	TRANSLATE(LEFT, 0);, 
	
	Node_t * diff_node = differentiate_to_new(RIGHT, list_get(mstokens, LEFT->data).token[0]); 
	while(node_tree_optimize(diff_node)){0;} 
	node_deinit(RIGHT); 
	RIGHT = diff_node; 
	COMPILE(RIGHT);)//'deriv' = 508575245668

#undef COPY_VAR
#undef VAR
#undef COMPILE
#undef TAB
#undef PRINT
#undef TRANSLATE

