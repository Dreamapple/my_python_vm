//MTuple impliment
#include "m.h"

MTuple::MTuple(int length)
	:type(3), len(length)
{
	item.assign(length, 0);
 }
 
void MTuple::MTuple_SetItem(int pos, MObj *v){
	if (len > pos){
		item[pos]=v;
	}else{
		printf("Tuple set item out of length");
	}
 }
 
MObj *MTuple::MTuple_GetItem(int pos){
	if(pos<0){
		pos=len+pos;
	}
	if(pos > len){
		printf("Tuple set item out of length");
		return nullptr;
	}
	return item[pos];
 }
 
void MTuple::MPrint_Tuple(){
	if (len == 0){
		printf("()");
	}
	else if (len == 1){
		printf("(%s,)", MPrint_MObj(item[0]));
	}

	else{
		int i = 0;
		putchar('(');
		while (i < len){
			MPrint_MObj_out(item[i]);
			putchar(',');
			i++;
		}
		putchar(')');
	}
}