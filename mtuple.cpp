//MTuple implyment
#include "m.h"
/* typedef struct tagMTUPLE{
	int type; //3
	MOBJ **val;
	int len;
} MTuple;
 */
 
 MTuple *MTuple_New(int length){
	MObj **p=(MObj **)malloc(length);
	MTuple *m= (MTuple *)malloc(sizeof(MTuple));
	int i;
	for (i=0;i<length;i++){
		p[i]=nullptr;
	}
	m->type=3;
	m->len=length;
	m->val=p;
	return m;
 }
 
 void MTuple_SetItem(MTuple *self,int pos,MObj *v){
	if (self->len > pos){
		self->val[pos]=v;
	}else{
		printf("Tuple set item out of length");
	}
 }
 
 MObj *MTuple_GetItem(MTuple *self,int pos){
	if(pos<0){
		pos=self->len+pos;
	}
	if(pos > self->len){
		printf("Tuple set item out of length");
		return nullptr;
	}
	return self->val[pos];
 }
 
 void MPrint_Tuple(MTuple *self){
	 if (self){
		 if (self->len == 0){
			 printf("()");
		 }
		 else if (self->len == 1){
			 printf("(%s,)", MPrint_MObj(self->val[0]));
		 }

		 else{
			 int i = 0;
			 putchar('(');
			 while (i < self->len){
				 MPrint_MObj_out(self->val[i]);
				 putchar(',');
				 i++;
			 }
			 putchar(')');
		 }
	 }
 }