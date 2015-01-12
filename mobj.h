//basic object
#include "m.h"
#ifndef __M_OBJ__
#define __M_OBJ__

typedef struct tagMOBJ{
	int type; //0
	int hash; //0:hasn't hash  -1:not hashable
} MObj;

typedef struct tagMINT{
	int type; //1
	int val;
} MInt;

typedef struct tagMSTRING{
	int type; //2
	const char *val;
	int len;
	int hash; //
} MString;

MString *MString_New(const char *s);

/*MTuple end */
typedef struct tagMTUPLE{
	int type; //3
	MObj **val;
	int len;
} MTuple;

MTuple *MTuple_New(int length);

void MTuple_SetItem(MTuple *self,int pos,MObj *v);

MObj *MTuple_GetItem(MTuple *self,int pos);

void MPrint_Tuple(MTuple *self);
/*MTuple end */

/* my dict struct
typedef struct tagMDictItem{
	int key;
	void *v;
} MDictItem;

typedef struct tagMDict{
	int type; //5
	MDictItem **item;
	int len_used;
	int len_all;
}MDict;

MDict *MDict_New();

void MDict_SetItem(MDict *self,MObj *k,MObj *v);

MObj *MDict_GetItem(MDict *self, MObj *k);

void *MDict_GetItem(MDict *self, int k);

void MDict_Print(MDict *self);

my dict struct end */

typedef struct tagMNone{
	int type; //6
}MNone;


const char* MPrint_MObj(MObj *self);
void MPrint_MObj_out(MObj *self);
#endif /* __M_OBJ__ */
