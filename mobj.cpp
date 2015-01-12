#include "m.h"

MString *MString_New(const char *s){
	MString *r = (MString *)malloc(4);
	r->type = 2;
	r->val = s;
	return r;
}
const char* MPrint_MObj(MObj *self){
	if (self->type == 0){
		char *s = "<MObject>";
		char *retval = (char*) malloc(sizeof(s));
		sprintf_s(retval, sizeof(s), "%s", s);
		return retval;
	}
	else if (self->type == 1){
		char *retval = (char*)malloc(10);
		sprintf_s(retval, 10, "%i", ((MInt *)self)->val);
		return retval;
	}
	else if (self->type == 2){
		return ((MString *)self)->val;
	}
	else if (self->type == 3){
		
		//return retval;
	}
	return "not implyment";
}

void MPrint_MObj_out(MObj *self){
	if (self){
		if (self->type == 0){
			char *s = "<MObject>";
			printf("%s", s);
		}
		else if (self->type == 1){
			printf("%i", ((MInt *)self)->val);
		}
		else if (self->type == 2){
			printf("%s", ((MString *)self)->val);
		}
		else if (self->type == 3){

			//return retval;
		}
		else{
			printf("%s", "not implyment");
		}
	}
	else{
		printf("None");
	}
}