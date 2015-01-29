//MList
#include "m.h"

MList::MList():type(4){}


void MList::MList_Append( MObj* e){
	item.push_back(e);
	return;
}

MObj *MList::MList_GetItem(int n){
	return item[n];
}


void MList::MPrint_List(){
	printf("[");
	for (int i = 0; i < item.size(); i++)
		printf("%s,\n", ((MString *)(item[i]))->val);
	printf(" ]\n");
}
