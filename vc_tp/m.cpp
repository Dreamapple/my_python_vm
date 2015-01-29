
#include "m.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	RFILE f;
	if (argc == 1){
		f.NEW(L"D:/tinypy/my_py/bin/Debug/t.pyc");
	}
	else{
		f.NEW(argv[1]);
	}

	marshal ms(&f);

	Code *code = (Code *)ms.r_object();

	MFrame::init_builtin();

	mdict local;
	mdict global;
	MFrame frame(code, &local, &global);
	frame.evalFrame();
	getchar();
	return 0;
}
