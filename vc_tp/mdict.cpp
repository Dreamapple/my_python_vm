#include "m.h"



mdict::mdict()
	:type(5), len_all(0), len_used(0)
{

}

MDictItem *MDictItem_New(int k, void *v){
	MDictItem *m = new MDictItem;
	m->key = k;
	m->v = v;
	return m;
}

int mdict::hash(int key){
	return key;
}
int mdict::hash_string(const char *key){
	int p = 0;
	int r = 0;
	while (key[p] != '\0'){
		r += key[p];
		p++;
	}
	return r;
}

int mdict::hash_mstring(MString *k){
	const char *key = k->val;
	return hash_string(key);
}

int mdict::hash_tuple(MTuple *m){
	int i;
	int p = 0;
	int r = 0;
	for(i=0;i < m->len;i++){
		r += hash_obj(m->item[i]);
	}
	return r;
}
int mdict::hash_obj(MObj *k){
	switch(k->type){
	case 1: //int
		k;
		return ((MInt *)k)->val;
	case 2: //string
		return hash_mstring((MString *)k);
	case 3: //tuple
		return hash_tuple((MTuple *)k);
	case 4: //list
	case 5: //dict
		break;
	case 6:// None
		return 0;
	}
	printf("Unhashable type %i",k->type);
	return -1;
}

void mdict::MDict_SetItem_int(int key, MObj *v){
	
	unsigned int c;
	for (c = 0; c < item.size(); c++)
	{
		if (item[c].key == key){
			item[c].v = v;
			return;
		}
	}
	MDictItem d = { key, v };
	item.push_back(d);
}

void mdict::MDict_SetItem_string(const char *k, MObj *v){
	MDict_SetItem_int(hash_string(k), v);
}

void mdict::MDict_SetItem( MObj *k, MObj *v){
	MDict_SetItem_int(hash_obj(k), v);
}
void *mdict::MDict_GetItem_int( int k){
	for (unsigned int c = 0; c < item.size(); c++)
	{
		if (item[c].key == k){
			return item[c].v;
		}
	}
	printf("Can not find the Item in Dict.\n");
	return 0;
}

MObj *mdict::MDict_GetItem( MObj *k){
	return (MObj *)MDict_GetItem_int(hash_obj(k));
}

void mdict::MDict_Print(){
	printf("{");
	int t = 0;
	for (t = 0; t < len_all; t++){
		if (item[t].v != nullptr){
			printf(" %u : %s,", item[t].key, item[t].v); //////////
		}
	}
	printf("}");
}

