#include "m.h"

int main(int argc, char *argv[])
{
	RFILE *f;
	if (argc == 1){
		f = MRFILE_New("helloworld.pyc");
	}
	else{
		f = MRFILE_New(argv[1]);
	}

	Header *h = (Header *)load_head(f);
	MCode *code = (MCode *)r_object(f);

	init_builtin();
	MDict *local = MDict_New();
	MDict *global = MDict_New();
	MFrame *frame=MFrame_New(code, local, global);
	evalFrame(frame);
	getchar();
	return 0;
}
