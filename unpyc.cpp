#include <stdio.h>
#include <stdlib.h>
#include "m.h"
//todo: I'd like to write a python vm(virtue m...)
//Firstly,It can decode bytecodes of "print('hello')" for ver 3.4 and run it.

//Now,can I open a file?

#ifdef DEBUG
unsigned char get(RFILE *f){
	unsigned char ch = getc(f->fp);
	printf("We get a char %u\n", ch);
	return (unsigned char)ch;
}
#else
#define get(f) (unsigned char)getc(f->fp)
#endif
unsigned char r_byte(RFILE *f){
	return get(f);
}
unsigned short r_short(RFILE *f){
	unsigned short r = 0;
	r = get(f);
	r = r + (get(f) << 8);
	return r;
};
unsigned long r_long(RFILE *f){
	unsigned long r = 0;
	r = get(f);
	r = r + (get(f) << 8);
	r = r + (get(f) << 16);
	r = r + (get(f) << 24);
	return r;
};
MString *r_string(int count, RFILE *f){
	char* p = (char *)malloc(count + 1);
	MString *m = (MString*)malloc(sizeof(MString));
	m->type = 2;
	m->len = count;
	int i = 0;
	while (i < count){
		p[i] = get(f);
		i++;
	}
	p[i] = '\0';
	m->val = p;
	return m;
}


MTuple *r_tuple(int n, RFILE *file){
	MTuple *v = MTuple_New(n);
	MObj *v2;
	int i;
	for (i = 0; i<n; i++){
		v2 = r_object(file);
		MTuple_SetItem(v, i, v2);
	}
	return v;
}

MNone None;

MObj *r_object(RFILE *f){
	byte code = r_byte(f);
	byte type = code & (~0x80);
	MObj *p = nullptr;

	int count;
	switch (type){

	case TYPE_STRING: //115
		count = r_long(f);
		p = (MObj *)r_string(count, f);
		break;
	case TYPE_SMALL_TUPLE: //41
		count = r_byte(f);
		return (MObj *)r_tuple(count, f);
	case TYPE_SHORT_ASCII_INTERNED://90 'Z'
	case TYPE_SHORT_ASCII:
		count = r_byte(f);
		p = (MObj *)r_string(count, f);
		break;
	case TYPE_NONE: //78 'N'
		break;
	case TYPE_CODE:
		p = (MObj *)load_pyc(f);
		return p;
	case TYPE_REF://114
		count = r_long(f);
		p = MList_GetItem(f->list, count);
		return p;
	default:
		printf("%s type not found", type);

		//case TYPE_INT:
		//	p = r_long(f);
		//	break;
	}
	MList_Append(f->list, p);
	return p;
}

Header *load_head(RFILE *f){
	Header *h = (Header *)malloc(12);
	h->magic = r_short(f);
	h->blank = r_short(f);
	h->mtime = (time_t)r_long(f);
	h->code_size = r_long(f);
#ifdef DEBUG
	printf("Header size is %u\n", sizeof(Header));
	printf("magic is %u.\n", h->magic);
	char t[30];
	ctime_s(t, 30, &(h->mtime));
	printf("mtime is %u,ctime is %s.\n", h->mtime, t);
	printf("codesize is %u.\n", h->code_size);
	//printf("Now the cur is %s\n",ftell(f));
#endif
	return h;
}

MCode *load_pyc(RFILE *f){
	MCode *code = (MCode *)malloc(sizeof(MCode));
	code->type = 10;
	code->co_argcount = r_long(f);

	code->co_kwonlyargcount = r_long(f);

	code->co_nlocals = r_long(f);

	code->co_stacksize = r_long(f);

	code->co_flags = r_long(f);

	code->co_code = (MString*)r_object(f);

	code->co_consts = (MTuple*)r_object(f);

	code->co_names = (MTuple*)r_object(f);

	code->co_varnames = (MTuple*)r_object(f);

	code->co_freevars = (MTuple*)r_object(f);

	code->co_cellvars = (MTuple*)r_object(f);

	code->co_filename = (MString*)r_object(f);

	code->co_name = (MString *)r_object(f);

	code->co_firstlineno = r_long(f);

	code->co_lnotab = (MString *)r_object(f);

	return code;
}
void dump_codeobj(MCode *code){
	printf("Argument count:    %i\n", code->co_argcount);
	
	printf("Kw-only arguments: %i\n", code->co_kwonlyargcount);
	printf("Number of locals:  %i\n", code->co_nlocals);
	printf("Stack size:        %i\n", code->co_stacksize);
	printf("Flags:             %i\n", code->co_flags);
	printf("Code:             %i\n", code->co_code);
	printf("constant leagth is %i\n", code->co_consts->len);
	MPrint_Tuple(code->co_consts);
	if (code->co_freevars)
		printf("co_freevars leagth is %i\n", code->co_freevars->len);
	else
		printf("co_freevars leagth is 0");
	MPrint_Tuple(code->co_freevars);
	printf("const names leagth is %i\n", code->co_names->len);
	MPrint_Tuple(code->co_names);
	printf("co_varnames leagth is %i\n", code->co_varnames->len);
	MPrint_Tuple(code->co_varnames);
	printf("co_cellvars is ");
	MPrint_Tuple(code->co_cellvars);
	printf("Filename:          %s\n", code->co_filename->val);
	printf("Name:              %s\n", code->co_name->val);
	printf("First line number:  %i\n", code->co_firstlineno);
	printf("co_lnotab:          %s\n", code->co_lnotab->val);
}
RFILE *MRFILE_New(char * path){
	RFILE *f = (RFILE*)malloc(sizeof(RFILE));
	FILE *fp=(FILE*)malloc(sizeof(FILE));
	if (fopen_s(&fp, path, "rb") != 0){
		return 0;
	}
	f->list = MList_New();
	f->fp = fp;
	return f;
}

void walkfile(char *path){
	FILE *fp;
	fopen_s(&fp, path, "rb");
	unsigned char ch;
	int t = fread(&ch, 1, 1, fp);
	while (t == 1){
		printf("%i ", ch);
		t = fread(&ch, 1, 1, fp);
	}
}

void print_fp_cur(FILE *fp){
	long pos = ftell(fp);
	printf("Now the cur is %ld\n", pos);
}

void simple_test(){
	RFILE *f = MRFILE_New("D:/tinypy/my_py/bin/Debug/t.pyc");
	Header *h = (Header *)load_head(f);
	PyCodeObject *code = (PyCodeObject *)r_object(f);
}