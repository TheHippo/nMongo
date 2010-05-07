#include <iostream>
#include <mongo/client/dbclient.h>

extern vkind k_BSONObject;

//data wrapper
struct BSONData{
	int length;
	char *data;  //raw data
};

value n_bson_encode(value obj);
value n_bson_decode(value str);
value n_get_json(value o);

BSONData* intern_encode(value obj);

void kill_object(value obj);
