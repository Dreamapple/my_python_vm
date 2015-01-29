
#include "unpyc.h"
//todo: I'd like to write a python vm(virtue m...)
//Firstly,It can decode bytecodes of "print('hello')" for ver 3.4 and run it.

//Now,can I open a file?
//this file,I imply :parse pyc file'header and code object

#ifdef DEBUG

marshal::marshal(RFILE *f)
	:code(f->fp->_base), pos(f->fp->_cnt), size(f->fp->_bufsiz), list(f->list)
{

}

unsigned char marshal::get(){
	unsigned char ch = code[pos++];
	printf("[*] We get a char %u\n", ch);
	return (unsigned char)ch;
}
#else
#define get(f) (unsigned char)getc(f->fp)
#endif
unsigned char marshal::r_byte(){
	return get();
}
unsigned short marshal::r_short(){
	unsigned short r = 0;
	r = get();
	r = r + (get() << 8);
	return r;
};
unsigned long marshal::r_long(){
	unsigned long r = 0;
	r = get();
	r = r + (get() << 8);
	r = r + (get() << 16);
	r = r + (get() << 24);
	return r;
};
MString *marshal::r_string(int count){
	char* p = (char *)malloc(count + 1);
	MString *m = (MString*)malloc(sizeof(MString));
	m->type = 2;
	m->len = count;
	int i = 0;
	while (i < count){
		p[i] = get();
		i++;
	}
	p[i] = '\0';
	m->val = p;
	return m;
}


MTuple *marshal::r_tuple(int n){
	MTuple *v = new MTuple(n);
	MObj *v2;
	int i;
	for (i = 0; i<n; i++){
		v2 = r_object();
		v->MTuple_SetItem(i, v2);
	}
	return v;
}

MNone None;

MObj *marshal::r_object(){
	byte code = r_byte();
	byte type = code & (~0x80);
	MObj *p = nullptr;

	int count;
	switch (type){

	case TYPE_STRING: //115
		count = r_long();
		p = (MObj *)r_string(count);
		break;
	case TYPE_SMALL_TUPLE: //41
		count = r_byte();
		return (MObj *)r_tuple(count);
	case TYPE_SHORT_ASCII_INTERNED://90 'Z'
	case TYPE_SHORT_ASCII:
		count = r_byte();
		p = (MObj *)r_string(count);
		break;
	case TYPE_NONE: //78 'N'
		break;
	case TYPE_CODE:
		p = (MObj *)load_pyc();
		return p;
	case TYPE_REF://114
		count = r_long();
		p = list->MList_GetItem(count);
		return p;
	default:
		printf("%s type not found", type);

		//case TYPE_INT:
		//	p = r_long(f);
		//	break;
	}
	list->MList_Append(p);
	return p;
}

//void dump_head(Header *h){
//	printf("Header size is %u\n", sizeof(h));
//	printf("magic is %u.\n", h->magic);
//	time_t ltime = (time_t)h->mtime;
//	wchar_t buf[26];
//	_wctime_s(buf, 26, &ltime);
//	wprintf_s(L"The time is %s\n", buf);
//	printf("codesize is %u.\n", h->code_size);
//	return;
//}

Code *marshal::load_pyc(){
	Code *code = new Code;
	code->type = 10;
	code->co_argcount = r_long();

	code->co_kwonlyargcount = r_long();

	code->co_nlocals = r_long();

	code->co_stacksize = r_long();

	code->co_flags = r_long();

	code->co_code = (MString*)r_object();

	code->co_consts = (MTuple*)r_object();

	code->co_names = (MTuple*)r_object();

	code->co_varnames = (MTuple*)r_object();

	code->co_freevars = (MTuple*)r_object();

	code->co_cellvars = (MTuple*)r_object();

	code->co_filename = (MString*)r_object();

	code->co_name = (MString *)r_object();

	code->co_firstlineno = r_long();

	code->co_lnotab = (MString *)r_object();

	return code;
}
void dump_codeobj(Code *code){
	printf("Argument count:    %i\n", code->co_argcount);
	
	printf("Kw-only arguments: %i\n", code->co_kwonlyargcount);
	printf("Number of locals:  %i\n", code->co_nlocals);
	printf("Stack size:        %i\n", code->co_stacksize);
	printf("Flags:             %i\n", code->co_flags);
	printf("Code:             %i\n", code->co_code);
	printf("constant leagth is %i\n", code->co_consts->len);
	code->co_consts->MPrint_Tuple();
	if (code->co_freevars)
		printf("co_freevars leagth is %i\n", code->co_freevars->len);
	else
		printf("co_freevars leagth is 0");
	code->co_freevars->MPrint_Tuple();
	printf("const names leagth is %i\n", code->co_names->len);
	code->co_names->MPrint_Tuple();
	printf("co_varnames leagth is %i\n", code->co_varnames->len);
	code->co_varnames->MPrint_Tuple();
	printf("co_cellvars is ");
	code->co_cellvars->MPrint_Tuple();
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
	
	f->list = new MList;
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


int RFILE::NEW(TCHAR *path){
	fp = (FILE*)malloc(sizeof(FILE));

	std::ifstream in;
	in.open(path, std::ios_base::binary);
	in.seekg(0, std::ios::end);
	int size = (int)in.tellg();
	size -= sizeof(head);
	char *s = new char[size];
	in.seekg(0, std::ios::beg);
	in.read((char*)&head, sizeof(head));
	in.read(s, size);

	
	fp->_bufsiz = size;
	fp->_base = s;
	fp->_cnt = 0;

	//if (_wfopen_s(&fp, path, L"rb") != 0){
	//	return 0;
	//}
	this->list = new MList;
	return 0;
}