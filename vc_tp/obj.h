//basic object
#include "m.h"
#include <vector>
#include <string>

#ifndef __M_OBJ__
#define __M_OBJ__

class MObj
{
public:
	int type;
};

class MInt{
	int type; //1
public:
	int val;
};

class MString{
public:
	int type; //2
	int len;
	int hash; //
	MString(const char *s);
	const char *val;
} ;

/*MTuple end */

class MTuple
{
	int type; //3
public:
	std::vector<MObj *> item;
	int len;
	MTuple(int len);
	void MTuple_SetItem(int pos, MObj *v);
	MObj *MTuple_GetItem(int pos);
	void MPrint_Tuple();
};

/*MTuple end */

class MList{
public:
	int type; //4
	std::vector<MObj *> item;
	int len;
	MList();
	void MList_Append(MObj *v);
	MObj *MList_GetItem(int k);
	void MPrint_List();
};


///* my dict struct
typedef struct tagMDictItem{
	int key;
	void *v;
} MDictItem;

class mdict
{
	void *MDict_GetItem_int(int k);
	int hash_obj(MObj *k);
	int hash(int key);
	int hash_string(const char *key);
	int hash_mstring(MString *k);
	int hash_tuple(MTuple *m);

	void MDict_SetItem_int(int key, MObj *v);

public:
	int type; //5
	std::vector<MDictItem> item;
	int len_used;
	int len_all;

	mdict();
	void MDict_SetItem(MObj *k, MObj *v);
	void MDict_SetItem_string(const char *k, MObj *v);
	MObj *MDict_GetItem(MObj *k);
	void *MDict_GetItem(int k);
	void MDict_Print();

};

//my dict struct end */

typedef struct tagMNone{
	int type; //6
}MNone;

typedef MObj *(*pf)(MTuple *t, mdict *k);

class MFunc{
	int type; //7 my function
	MObj *val;
};

class MFunc_builtin
{
public:
	int type; //8 my builtin function
	pf val;
	MFunc_builtin(pf f);
	MObj *MFunc_builtin_Run(MTuple *t, mdict *k);
};

MObj *m_print(MTuple *t, mdict *k);

const char* MPrint_MObj(MObj *self);
void MPrint_MObj_out(MObj *self);

class Code;
class MFrame
{

	Code *code;
	std::vector<MObj *> stack;
	int pos;
	//MObj **stack;

	mdict *local;
	mdict *global;
public:
	static mdict *builtin;

	void static init_builtin();
	MFrame(Code *code, mdict *local, mdict *global);
	void call_function(int oparg);
	void *evalFrame();
};

#endif /* __M_OBJ__ */
