//DEF_OPTIM(condition, do)

#define L  LEFT->data
#define mL LEFT->mode
#define M  THIS->mode
#define D  THIS->data
#define R  RIGHT->data
#define mR RIGHT->mode
#define AFTER_CONST 	node_deinit(LEFT);\
			node_deinit(RIGHT);\
			LEFT = nullptr;\
			RIGHT = nullptr;


DEF_OPTIM(((mL == MODE_CNST && L == 0) || (mR == MODE_CNST && R == 0)) && M == MODE_OPER && D == '*', 
	  D = 0; M = MODE_CNST; AFTER_CONST;)

DEF_OPTIM((mL == MODE_CNST && L == 0) && M == MODE_OPER && D == '/',
	  D = 0; M = MODE_CNST; AFTER_CONST;)

DEF_OPTIM((mR == MODE_OPER && (R == '-' || R == '+') && RIGHT->left == nullptr) && M == MODE_OPER && (D == '+' || D == '-'), 
	  D = (R == '-') ? ((D == '+') ? '-' : '+'): D; M = MODE_OPER; Node_t * temp = RIGHT->right; RIGHT->right = nullptr; node_deinit(RIGHT); RIGHT = temp;)

DEF_OPTIM(((mL == MODE_CNST && L == 0)) && M == MODE_OPER && (D == '+'),
	  D = R; M = mR; node_deinit(LEFT); Node_t * temp1 = RIGHT->right; Node_t * temp2 = RIGHT->left; RIGHT->right = nullptr; RIGHT->left = nullptr; node_deinit(RIGHT); RIGHT = temp1; LEFT = temp2;)

DEF_OPTIM(((mR == MODE_CNST && R == 0)) && M == MODE_OPER && (D == '+' || D == '-'),
	  D = L; M = mL; node_deinit(RIGHT); Node_t * temp1 = LEFT->right; Node_t * temp2 = LEFT->left; LEFT->right = nullptr; LEFT->left = nullptr; node_deinit(LEFT); RIGHT = temp1; LEFT = temp2;)

DEF_OPTIM(((mR == MODE_CNST && R == 1*ACCURACY)) && M == MODE_OPER && (D == '*' || D == '/' || D == '^'),
	  D = L; M = mL; node_deinit(RIGHT); Node_t * temp1 = LEFT->right; Node_t * temp2 = LEFT->left; LEFT->right = nullptr; LEFT->left = nullptr; node_deinit(LEFT); RIGHT = temp1; LEFT = temp2;)

DEF_OPTIM(((mL == MODE_CNST && L == 1*ACCURACY)) && M == MODE_OPER && (D == '*'),
	  D = R; M = mR; node_deinit(LEFT); Node_t * temp1 = RIGHT->right; Node_t * temp2 = RIGHT->left; RIGHT->right = nullptr; RIGHT->left = nullptr; node_deinit(RIGHT); RIGHT = temp1; LEFT = temp2;)

DEF_OPTIM((mL == MODE_CNST && L == 0) && M == MODE_OPER && (D == '*'),
	  D = 0; M = MODE_CNST; AFTER_CONST;)

DEF_OPTIM(((mR == MODE_CNST && R == 0)) && M == MODE_OPER && (D == '^'),
	  D = 1*ACCURACY; M = MODE_CNST; AFTER_CONST;)

DEF_OPTIM(((mL == MODE_CNST && L == 0)) && M == MODE_OPER && (D == '^'),
	  D = 0; M = MODE_CNST; AFTER_CONST;)

DEF_OPTIM(((mL == MODE_CNST && L == 1*ACCURACY)) && M == MODE_OPER && (D == '^'),
	  D = 1*ACCURACY; M = MODE_CNST; AFTER_CONST;)

#undef L
#undef R
#undef mR
#undef mL
#undef M
#undef D
