#include "m.h"
MDict *builtin;

void init_builtin(){
	builtin = MDict_New();
	MFunc_builtin *fun = MFunc_builtin_New(m_print);
	//MString *s = MString_New("print");
	MDict_SetItem_string(builtin, "print", (MObj *)fun);
}
MFrame *MFrame_New(MCode *code,MDict *local,MDict *global){
	MFrame *f = (MFrame *)malloc(sizeof(MFrame));
	f->code = code;
	f->local = local;
	f->global = global;
	f->builtin = builtin;
	f->stack = (MObj **)malloc(code->co_stacksize);
	return f;
}

void call_function(MObj ***pp_stack, int oparg){
	MObj **stack = *pp_stack;
	int na = oparg & 0xff;
	int nk = (oparg >> 8) & 0xff;
	int n = na + 2 * nk;
	MObj **pfunc = stack - n - 1;
	MObj *func = *pfunc++;
	MTuple *arg_t = MTuple_New(na);
	int i = 0;
	for (; i < na; i++){
		MTuple_SetItem(arg_t, i, *pfunc++);
	}
	MDict *arg_k = MDict_New();
	for (; i < nk; i = i + 2){
		MDict_SetItem(arg_k, *pfunc++, *pfunc++);
	}

	//	MObj *retval, *temp;
	if (func->type == 8){ //builtin
		stack = stack - n - 1;
		*stack++ = MFunc_builtin_Run((MFunc_builtin *)func, arg_t, arg_k);
	}
	else{
		return;
	}
}

MObj *evalFrame(MFrame *f){
	PyCodeObject *c = f->code;
	const char *code = c->co_code->val;
	MObj **stack = f->stack;
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
			stack--;
			break;
		case LOAD_NAME:
			name = MTuple_GetItem(c->co_names, arg);
			if (MDict_GetItem(f->local, name) != nullptr){
				*stack++ = MDict_GetItem(f->local, name);
			}
			else if (MDict_GetItem(f->global, name) != nullptr){
				*stack++ = MDict_GetItem(f->global, name);
			}
			else if (MDict_GetItem(f->builtin, name) != nullptr){
				*stack++ = MDict_GetItem(f->builtin, name);
			}
			else{
				printf("can not found name %s in anywhere", ((MString *)name)->val);///???
			}
			break;
		case LOAD_CONST:
			*stack++ = MTuple_GetItem(c->co_consts, arg);
			break;
		case CALL_FUNCTION:
			call_function(&stack, arg);
			break;
		case RETURN_VALUE:
			return *stack;
		default:
			printf("not found opcode %i", op);
		}


	}
	return nullptr;
}
