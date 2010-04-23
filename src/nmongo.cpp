#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>
#include "bson.h"

using namespace mongo;

DEFINE_KIND(k_DBClientConnection);

void kill_connection(value c) {
	delete (DBClientConnection*) val_data(c);
}

value n_dbconnect(value host) {
	
	val_check(host,string);
	DBClientConnection *c = new DBClientConnection();
	try {
		c->connect(val_string(host));
		value ret = alloc_abstract(k_DBClientConnection,c);
		val_gc(ret,kill_connection);
		return ret;
	}
	catch (DBException &e) {
		failure(e.what());
	}
	return val_null;
}

value n_getserveraddress(value con) {
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);	
	return alloc_string(c->toString().c_str());
}

//connection
DEFINE_PRIM(n_dbconnect,1);
DEFINE_PRIM(n_getserveraddress,1);

//bson
DEFINE_PRIM(n_bson_encode,1);
DEFINE_PRIM(n_bson_decode,1);

