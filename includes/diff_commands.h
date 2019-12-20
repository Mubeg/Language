//DEF_DIF(name, mode, wchar_t equivalent, code, do)

#define d(arg) do_diff_recursive(arg, cur_diff)
#define dL do_diff_recursive(node->left, cur_diff)
#define cL node_copy(node->left)
#define dR do_diff_recursive(node->right, cur_diff)
#define cR node_copy(node->right)
#define NEW(mode, elem, left, right) node_create_new(mode, elem, left, right)
#define L LEFT->data
#define R RIGHT->data
#define FUNC(arg) list_find_by_elem(mstokens, #arg)

DEF_DIF(cst, MODE_CNST, '\0' , return NEW(MODE_CNST, 0, nullptr, nullptr);, 0;)

DEF_DIF(var, MODE_VARL, '\0' , return (list_get(mstokens, DATA).token[0] == cur_diff || cur_diff == -1) ? NEW(MODE_CNST, 1*ACCURACY, nullptr, nullptr) : NEW(MODE_CNST, 0, nullptr, nullptr);, 0;)

DEF_DIF(add, MODE_OPER, '+', return NEW(MODE_OPER, '+', dL, dR);, DATA = (LEFT == nullptr) ? +R : L + R)

DEF_DIF(sub, MODE_OPER, '-', return NEW(MODE_OPER, '-', dL, dR);, DATA = (LEFT == nullptr) ? -R : L - R)

DEF_DIF(mul, MODE_OPER, '*', return NEW(MODE_OPER, '+', NEW(MODE_OPER, '*', dL, cR), NEW(MODE_OPER, '*', cL, dR));, DATA = L*R/ACCURACY)

DEF_DIF(div, MODE_OPER, '/', return NEW(MODE_OPER, '/', NEW(MODE_OPER, '-', NEW(MODE_OPER, '*', dL, cR), NEW(MODE_OPER, '*', cL, dR)), NEW(MODE_OPER, '*', cR, cR));, DATA = (L*ACCURACY)/R)

DEF_DIF(pow, MODE_OPER, '^', return NEW(MODE_OPER, '*', NEW(MODE_OPER, '^', cL, cR), d(NEW(MODE_OPER, '*', cR, NEW(MODE_FUNC, FUNC(ln), nullptr, cL))));, DATA = pow(((float) L)/ACCURACY, ((float) R)/ACCURACY)*ACCURACY)

DEF_DIF(sin, MODE_FUNC, '\0', return NEW(MODE_OPER, '*', dR, NEW(MODE_FUNC, FUNC(cos), nullptr, cR));, DATA = sin(((float) R)/ACCURACY)*ACCURACY;)

DEF_DIF(cos, MODE_FUNC, '\0', return NEW(MODE_OPER, '-', nullptr, NEW(MODE_OPER, '*', dR, NEW(MODE_FUNC, FUNC(sin), nullptr, cR)));, DATA = cos(((float) R)/ACCURACY)*ACCURACY;)

DEF_DIF(ln, MODE_FUNC, '\0', return NEW(MODE_OPER, '/', dR, cR);, DATA = log(((float) R)/ACCURACY)/log(2.718)*ACCURACY;)

#undef d
#undef dL
#undef cL
#undef dR
#undef cR
#undef NEW
#undef L
#undef R
#undef FUNC

