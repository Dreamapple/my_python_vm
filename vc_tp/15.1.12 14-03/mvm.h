#include "mobj.h"
#include "unpyc.h"

#ifndef __MVM__
#define __MVM__

typedef struct tagMFrame{
	PyCodeObject *code;
	MObj **stack;
	
	MDict *local;
	MDict *global;
	MDict *builtin;
	
} MFrame;

//int M_print()
void init_builtin();
MFrame *MFrame_New(MCode *code, MDict *local, MDict *global);
MObj *call_function(MObj ***pp_stack, int oparg);
MObj *evalFrame(MFrame *f);

#endif