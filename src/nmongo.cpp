#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>

using namespace mongo;

DEFINE_KIND(k_DBClientConnection);

value n_dbconnect(value host) {
	
	val_check(host,string);
	
	DBClientConnection c;
	
	try {
		c.connect(val_string(host));
		void *ptr = &c;
		return alloc_abstract(k_DBClientConnection,ptr);
	}
	catch (DBException &e) {
		failure(e.what());
	}
	return val_null;
}

/* value n_getserver(value con) {
		val_check_kind(con,k_DBClientConnection);
		void *ptr = val_data(con);
		DBClientConnection c = (DBClientConnection)ptr;
		
		return val_null;
} */

DEFINE_PRIM(n_dbconnect,1);

