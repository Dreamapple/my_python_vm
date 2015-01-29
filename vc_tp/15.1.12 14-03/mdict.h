#include <stdlib.h>
#include <stdio.h>
#include "m.h"
#ifndef __MDICT__
#define __MDICT__

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

void MDict_SetItem(MDict *self, MObj *k, MObj *v);
void MDict_SetItem_string(MDict *self, const char *k, MObj *v);
MObj *MDict_GetItem(MDict *self, MObj *k);

void *MDict_GetItem(MDict *self, int k);

void MDict_Print(MDict *self);


#endif // __MDICT__