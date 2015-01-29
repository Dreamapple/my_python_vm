#include "m.h"

MFunc_builtin::MFunc_builtin(pf f)
	:type(8), val(f)
{

}

MObj *MFunc_builtin::MFunc_builtin_Run(MTuple *t, mdict *k){
	return val(t, k);
}

MObj *m_print(MTuple *t, mdict *k){
	if (t){
		int i = 0;
		while (i < t->len){
			printf("%s",MPrint_MObj(t->item[i]));
			i++;
		}
	}
	putchar('\n');
	return nullptr;
}