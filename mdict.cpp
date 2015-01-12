#include "m.h"


int hash_obj(MObj *k);

void MDict_Resize(MDict *m){
	if (m->len_all == 0){
		MDictItem **p = (MDictItem **)malloc(10 * sizeof(MDictItem *));
		int i = 0;
		for (i = 0; i < 10; i++){
			*(p+i) = 0;
		}
		m->item = p;
		m->len_all = 10;
	}else if(m->len_used / m->len_all > 0.5){
		MDictItem **p = (MDictItem **)realloc(m->item, m->len_all * 2);
		int i;
		for (i = m->len_all; i < m->len_all*2; i++){
			p[i] = nullptr;
		}
		m->item = p;
		m->len_all *= 2;
	}
}

MDict *MDict_New(){
	MDict *m = (MDict *)malloc(sizeof(MDict));
	m->type = 5;
	m->len_all = 0;
	m->len_used = 0;
	MDict_Resize(m);
	return m;
}

MDictItem *MDictItem_New(int k,void *v){
	MDictItem *m = (MDictItem *)malloc(sizeof(MDictItem));
	m->key = k;
	m->v = v;
	return m;
}

int hash(int key){
	return key;
}
int hash_string(const char *key){
	int p = 0;
	int r = 0;
	while (key[p] != '\0'){
		r += key[p];
		p++;
	}
	return r;
}

int hash_mstring(MString *k){
	const char *key = k->val;
	return hash_string(key);
}

int hash_tuple(MTuple *m){
	int i;
	int p = 0;
	int r = 0;
	for(i=0;i < m->len;i++){
		r += hash_obj(m->val[i]);
	}
	return r;
}
int hash_obj(MObj *k){
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

void MDict_SetItem_int(MDict *self,int key,MObj *v,int loop=5){
	int hash_key = key % 13;
	hash_key = hash_key%self->len_all;
	MDictItem *item = MDictItem_New(key, v);
	int c = 0;
	while (self->item[hash_key] != nullptr && c<loop){
		hash_key = hash_key * 2 % self->len_all;
		c++;
	}
	if (c >= loop){
		MDict_Resize(self);
		MDict_SetItem_int(self, key, v,loop*2);
	}
	else{
		self->item[hash_key] = item;
	}
}

void MDict_SetItem_string(MDict *self, const char *k, MObj *v){
	MDict_SetItem_int(self, hash_string(k), v, 5);
}

void MDict_SetItem(MDict *self,MObj *k,MObj *v){
	MDict_SetItem_int(self, hash_obj(k), v, 5);
}
void *MDict_GetItem_int(MDict *self, int k){
	int hash_key = k % 13;
	hash_key = hash_key%self->len_all;
	int i = 0;
	while (self->item[hash_key]!=0 && self->item[hash_key]->key != k && i<100){
		hash_key = hash_key * 2 % self->len_all;
		i++;
	}
	if (i >= 100){
		printf("Can not find the Item in Dict.");
	}
	else if (self->item[hash_key])
	{
		return self->item[hash_key]->v;
	}
	else{
		return 0;
	}
}

MObj *MDict_GetItem(MDict *self, MObj *k){
	return (MObj *)MDict_GetItem_int(self,hash_obj(k));
}

void MDict_Print(MDict *self){
	printf("{");
	int t = 0;
	for (t = 0; t < self->len_all; t++){
		if (self->item[t] != nullptr){
			printf(" %u : %s,", self->item[t]->key, self->item[t]->v); //////////
		}
	}
	printf("}");
}

