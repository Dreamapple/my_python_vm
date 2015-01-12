#include "m.h"

#ifndef __MLIST__
#define __MLIST__
/* my list struct */
typedef struct tagMListItem{
	MObj *val;
	struct tagMListItem *next;
} MListItem;

typedef struct tagMLIST{
	int type; //4
	MListItem *val;
	int len;
} MList;

MList *MList_New();

void MList_Append(MList *self, MObj *v);

MObj *MList_GetItem(MList *self, int k);

void MPrint_List(MList *self);
/* my list struct end */

#endif