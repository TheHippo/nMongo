#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>

value n_bson_encode(value obj);
value n_bson_decode(value str);
value n_get_json(value o);
