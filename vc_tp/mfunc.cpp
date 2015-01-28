#include "m.h"

MFunc_builtin *MFunc_builtin_New(pf f){
	MFunc_builtin *m = (MFunc_builtin *)malloc(sizeof(MFunc_builtin));
	m->type = 8;
	m->val = f;
	return m;
}

MObj *MFunc_builtin_Run(MFunc_builtin *self, MTuple *t, MDict *k){
	//pf f=self->val;
	return (self->val)(t, k);
}

MObj *m_print(MTuple *t, MDict *k){
	if (t){
		int i = 0;
		while (i < t->len){
			printf("%s",MPrint_MObj(t->val[i]));
			i++;
		}
	}
	putchar('\n');
	return nullptr;
}