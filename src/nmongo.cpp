#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>
#include "bson.h"

using namespace mongo;

DEFINE_KIND(k_DBClientConnection);

#define GET_CONNECTION(c) ((DBClientConnection*)val_data(c))

//called by garbage collection, after cleaning up a mongo.DBConnection object
void kill_connection(value c) {
	delete GET_CONNECTION(c);
}

//connect to a simple database, format: host:port
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
	return val_null; //should never happen
}

//return the adress of the current connection
value n_getserveraddress(value con) {
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = GET_CONNECTION(con);
	return alloc_string(c->toString().c_str());
}

//connection
DEFINE_PRIM(n_dbconnect,1);
DEFINE_PRIM(n_getserveraddress,1);

//bson
DEFINE_PRIM(n_bson_encode,1);
DEFINE_PRIM(n_bson_decode,1);

