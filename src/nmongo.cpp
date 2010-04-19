#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>

using namespace mongo;

value test() {
	return alloc_string("foo");
}


DEFINE_PRIM(test,0);
