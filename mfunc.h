#include "m.h"

#ifndef __MFUNC__
#define __MFUNC__
typedef MObj *(*pf)(MTuple *t,MDict *k);

typedef struct tagMFunc{
	int type; //7 my function
	MObj *val;
} MFunc;

typedef struct tagMFunc_builtin{
	int type; //8 my builtin function
	pf val;
} MFunc_builtin;

MFunc_builtin *MFunc_builtin_New(pf f);
MObj *MFunc_builtin_Run(MFunc_builtin *self, MTuple *t, MDict *k);
MObj *m_print(MTuple *t, MDict *k);

#endif 