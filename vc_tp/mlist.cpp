//mlist
#include "mlist.h"

MList *MList_New(){
	MList *m = (MList *)malloc(sizeof(MList));
	m->len = 0;
	m->type = 4;
	return m;
}


void MList_Append(MList *self, MObj* e){
	MListItem* n = (MListItem *)malloc(sizeof(MListItem));
	n->val = e;
	n->next = 0;
	if (self->len == 0){
		self->val = n;
		self->len = 1;
	}
	else if (self->len > 0){
		MListItem *l = self->val;
		int i = 1;
		while (i<self->len){
			l = l->next;
			i++;
		}
		l->next=n;
		self->len++;
	}
	return;
}

MObj *MList_GetItem(MList *m, int n){
	int i = 0;
	MListItem *l = m->val;
	while (i < n){
		l = l->next;
		i++;
	}
	return l->val;
}


void MPrint_List(MList *self){
	if (self->len > 0){
		printf("[");
		MListItem *l = self->val;
		while (l->next!=0){
			printf("%s,\n", ((MString *)(l->val))->val);
			l = l->next;
		}
		printf(" %s]\n", ((MString *)(l->val))->val);
	}
}
