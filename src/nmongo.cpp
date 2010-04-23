#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>
#include "bson.h"

using namespace mongo;

DEFINE_KIND(k_DBClientConnection);

value n_dbconnect(value host) {
	
	val_check(host,string);
	
	DBClientConnection c;
	
	try {
		c.connect(val_string(host));
		cout << c.toString() << endl;
		return alloc_abstract(k_DBClientConnection,&c);
	}
	catch (DBException &e) {
		failure(e.what());
	}
	return val_null;
}

value n_getserveraddress(value con) {
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);
	
	//DBClientConnection dbc(c); 
	//pure virtual method called
	//terminate called without an active exception
	
	//cout << c->toString() << endl;
	//Uncaught exception - Segmentation fault
	
	return alloc_string("foo");
}

value n_test() {
	value con = n_dbconnect(alloc_string("localhost"));
	cout << val_string(n_getserveraddress(con)) << endl;
	return val_null;
}

//connection
DEFINE_PRIM(n_dbconnect,1);
DEFINE_PRIM(n_getserveraddress,1);
DEFINE_PRIM(n_test,0);

//bson
DEFINE_PRIM(n_bson_encode,1);
DEFINE_PRIM(n_bson_decode,1);

