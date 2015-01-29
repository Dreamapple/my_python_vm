#include "m.h"

int main(int argc, char *argv[])
{
	RFILE *f = MRFILE_New("D:/tinypy/my_py/bin/Debug/t.pyc");
	Header *h = (Header *)load_head(f);
	MCode *code = (MCode *)r_object(f);

	init_builtin();
	MDict *local = MDict_New();
	MDict *global = MDict_New();
	MFrame *frame=MFrame_New(code, local, global);
	evalFrame(frame);

	system("pause");
	return 0;
}
