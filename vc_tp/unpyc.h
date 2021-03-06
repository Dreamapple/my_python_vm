//可以打开一个pyc文件，目前只测试了把“print("qwe")”编译后的解压。
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>

#include "obj.h"
#ifndef __UNPYC__
#define __UNPYC__

#define TYPE_NULL               '0'
#define TYPE_NONE               'N'
#define TYPE_FALSE              'F'
#define TYPE_TRUE               'T'
#define TYPE_STOPITER           'S'
#define TYPE_ELLIPSIS           '.'
#define TYPE_INT                'i'
#define TYPE_FLOAT              'f'
#define TYPE_BINARY_FLOAT       'g'
#define TYPE_COMPLEX            'x'
#define TYPE_BINARY_COMPLEX     'y'
#define TYPE_LONG               'l'
#define TYPE_STRING             's'   /* 115 */
#define TYPE_INTERNED           't'
#define TYPE_REF                'r'
#define TYPE_TUPLE              '('
#define TYPE_LIST               '['
#define TYPE_DICT               '{'
#define TYPE_CODE               'c'
#define TYPE_UNICODE            'u'
#define TYPE_UNKNOWN            '?'
#define TYPE_SET                '<'
#define TYPE_FROZENSET          '>'
#define FLAG_REF                '\x80' /* with a type, add obj to index */

#define TYPE_ASCII              'a'
#define TYPE_ASCII_INTERNED     'A'
#define TYPE_SMALL_TUPLE        ')'
#define TYPE_SHORT_ASCII        'z'
#define TYPE_SHORT_ASCII_INTERNED 'Z'

typedef unsigned char byte;

class RFILE{
public:
	RFILE(){};
	RFILE(TCHAR *path){
		fp = (FILE*)malloc(sizeof(FILE));
		_wfopen_s(&fp, path, L"rb");
		this->list = new MList;
	}
	FILE *fp;
	MList *list;
	struct{
		short magic;
		short blank;
		int mtime;
		int code_size;
	} head;

	int NEW(TCHAR *path);
};

/* Bytecode object */
class Code{
public:
	int type;        //10
	int co_argcount;		/* #arguments, except *args */
	int co_kwonlyargcount;	/* #keyword only arguments */
	int co_nlocals;		/* #local variables */
	int co_stacksize;		/* #entries needed for evaluation stack */
	int co_flags;		/* CO_..., see below */
	MString *co_code;		/* instruction opcodes */
	MTuple *co_consts;	/* list (constants used) */
	MTuple *co_names;		/* list of strings (names used) */
	MTuple *co_varnames;	/* tuple of strings (local variable names) */
	MTuple *co_freevars;	/* tuple of strings (free variable names) */
	MTuple *co_cellvars;      /* tuple of strings (cell variable names) */
	/* The rest doesn't count for hash or comparisons */
	unsigned char *co_cell2arg; /* Maps cell vars which are argumentsn */
	MString *co_filename;	/* unicode (where it was r_objected from) */
	MString *co_name;		/* unicode (name, for reference) */
	int co_firstlineno;		/* first source line number */
	MString *co_lnotab;	/* string (encoding addr<->lineno mapping) See
							Objects/lnotab_notesntxt for details. */
	void *co_zombieframe;     /* for optimization only (see frameobject.c) */
	void *co_weakreflist;   /* to support weakrefs to code objects */
};




class marshal
{
public:
	char *code;
	int pos;
	int size;
	MList *list;

	marshal(RFILE *f);

	MObj *r_object();
	MTuple *r_tuple(int n);
	MString *r_string(int count);
	unsigned long r_long();
	unsigned short r_short();
	unsigned char r_byte();
	unsigned char get();
	Code *load_pyc();
};
#endif

