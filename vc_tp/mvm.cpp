#include "m.h"

mdict *MFrame::builtin = new mdict;

void MFrame::init_builtin()
{
	MFunc_builtin *fun = new MFunc_builtin(m_print);
	builtin->MDict_SetItem_string("print", (MObj *)fun);
}

MFrame::MFrame(Code *code, mdict *local, mdict *global)
	:code(code), local(local), global(global), pos(0)
{
	stack.assign(code->co_stacksize, 0);
}

void MFrame::call_function(int oparg){
	int na = oparg & 0xff;
	int nk = (oparg >> 8) & 0xff;
	int n = na + 2 * nk;
	int p = pos - n - 1;
	MObj *func = stack[p++];
	MTuple *arg_t = new MTuple(na);
	
	for (int i = 0; i < na; i++){
		arg_t->MTuple_SetItem(i, stack[p++]);
	}
	mdict *arg_k = new mdict;
	for (int i = 0; i < nk; i = i + 2){
		arg_k->MDict_SetItem(stack[p++], (MObj *)stack[p++]);
	}

	//	MObj *retval, *temp;
	if (((MObj *)func)->type == 8){ //builtin
		pos = pos - n - 1;
		stack[pos++] = ((MFunc_builtin *)func)->MFunc_builtin_Run(arg_t, arg_k);
	}
	else{
		return;
	}
}

void *MFrame::evalFrame(){
	Code *c = code;
	const char *code = c->co_code->val;
	int i = 0;
	unsigned char op = 0;
	int arg = 0;
	int t_arg = 0;
	MObj *name;//in load_name
	while (i < c->co_code->len){
		op = code[i++];
		if (HAS_ARG(op)){
			arg = code[i++];
			t_arg = code[i++];
			arg = arg + (t_arg << 8);
		}
		switch (op){
		case POP_TOP:
			pos--;
			break;
		case LOAD_NAME:
			name = c->co_names->MTuple_GetItem(arg);
			if (local->MDict_GetItem(name) != 0){
				stack[pos++]=local->MDict_GetItem(name);
			}
			else if (global->MDict_GetItem(name) != 0){
				stack[pos++] = global->MDict_GetItem(name);
			}
			else if (builtin->MDict_GetItem(name) != 0){
				stack[pos++] = builtin->MDict_GetItem(name);
			}
			else{
				printf("can not found name %s in anywhere", ((MString *)name)->val);///???
			}
			break;
		case LOAD_CONST:
			stack[pos++] = c->co_consts->MTuple_GetItem(arg);
			break;
		case CALL_FUNCTION:
			call_function(arg);
			break;
		case RETURN_VALUE:
			return (void *)stack[pos];
		default:
			printf("not found opcode %i", op);
		}
	}
	return nullptr;
}
