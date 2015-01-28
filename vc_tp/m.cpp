
#include "m.h"
#include <iostream>
using namespace std;

#define out(s) cout<< #s <<":" << f.fp->s<<endl;
int _tmain(int argc, _TCHAR* argv[])
{
	RFILE f;
	if (argc == 1){
		f.NEW(L"D:/tinypy/my_py/bin/Debug/t.pyc");
	}
	else{
		f.NEW(argv[1]);
	}

	MCode *code = (MCode *)r_object(&f);

	init_builtin();
	MDict *local = MDict_New();
	MDict *global = MDict_New();
	MFrame *frame=MFrame_New(code, local, global);
	evalFrame(frame);
	getchar();
	return 0;
}
