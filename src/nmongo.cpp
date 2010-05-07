#include <sstream>
#include <iostream>
#include <mongo/client/dbclient.h>
#include <neko.h>
#include "bson.h"

using namespace mongo;

DEFINE_KIND(k_DBClientConnection);
DEFINE_KIND(k_DBCursor);

//called by garbage collection, after cleaning up a mongo.DBConnection object
void kill_connection(value c) {
	delete (DBClientConnection*)val_data(c);
}

void kill_cursor(value c) {
	//seems to not work with gc
//	auto_ptr<DBClientCursor> *cursor = (auto_ptr<DBClientCursor>*) val_data(c);
//	delete cursor;
}

//connect to a simple database, format: host:port
value n_dbconnect(value host) {	
	val_check(host,string);
	DBClientConnection *c = new DBClientConnection();
	try {
		//connect and wrap into neko abstract
		c->connect(val_string(host));
		value ret = alloc_abstract(k_DBClientConnection,c);
		val_gc(ret,kill_connection);
		return ret;
	}
	catch (DBException &e) {
		// throw a neko error
		failure(e.what());
	}
	return val_null; //should never happen
}

//return the adress of the current connection
value n_getserveraddress(value con) {
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = ((DBClientConnection*)val_data(con));
	return alloc_string(c->toString().c_str());
}

//return a list of all databases
value n_getdatabasenames(value con) {
	//check
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);
	
	//fetch result
	list<string> db_result = c->getDatabaseNames();
	
	//neko types
	value ret = alloc_array((int)db_result.size());
	value *data = val_array_ptr(ret);
	
	//iterate
	list<string>::iterator i;
	int n = 0;
	for (i=db_result.begin(); i!=db_result.end(); i++,n++ ) {
		data[n]=alloc_string((*i).c_str());
	}
	
	return ret;
}

//returns a list of all collections within a db, without the name of the db
value n_getcollectionnames(value con, value db) {
	//check
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);
	val_check(db,string);
	
	//fetch result
	list<string> db_result = c->getCollectionNames(val_string(db));

	// init neko types
	value ret = alloc_array((int)db_result.size());
	value *data = val_array_ptr(ret);
	
	//iterate
	list<string>::iterator i;
	int n = 0;
	for (i=db_result.begin(); i!=db_result.end(); i++,n++ ) {
		data[n]=alloc_string(c->nsGetCollection((*i)).c_str());
	}
	
	return ret;
}

//return the count of all items in a collection
value n_countcollectionitems(value con, value ns) {
	//check
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);
	val_check(ns,string);
	//get & return result
	return alloc_int(c->count(val_string(ns)));	
}

value n_insert(value con,value ns, value obj) {
	//check
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);	
	val_check(ns,string);	
	val_check_kind(obj,k_BSONObject);
	
	BSONData *data = (BSONData*)val_data(obj);
	string test (data->data,data->length);
	BSONObj b (test.c_str(),false);
	
	c->insert(val_string(ns),b);
	return val_null;
}

value n_query(value con, value ns, value query, value limit, value skip) {
	//check
	val_check_kind(con,k_DBClientConnection);
	DBClientConnection *c = (DBClientConnection*)val_data(con);	
	val_check(ns,string);	
	val_check(limit,int);	
	val_check(skip,int);
	
	if (val_type(query) != VAL_OBJECT)
		failure("Invalid Query");
		
	BSONData* data = intern_encode(query);
	string test (data->data,data->length);
	BSONObj b (test.c_str(),false);
	Query queryData (b);
	
	auto_ptr<DBClientCursor> cursor = c->query(val_string(ns),queryData,val_int(limit), val_int(skip));
	auto_ptr<DBClientCursor> *ptr = new auto_ptr<DBClientCursor>();
	*ptr = cursor;
	value ret = alloc_abstract(k_DBCursor,ptr);
	val_gc(ret,kill_cursor);
	return ret;
}

value n_cursor_has(value cur) {
	cout << "has" << endl;
	val_check_kind(cur,k_DBCursor);
	auto_ptr<DBClientCursor> *cursor = (auto_ptr<DBClientCursor>*) val_data(cur);
	if ((*cursor)->more())
		return val_true;
	else
		return val_false;
}

value n_cursor_value(value cur) {
	val_check_kind(cur,k_DBCursor);
	auto_ptr<DBClientCursor> *cursor = (auto_ptr<DBClientCursor>*) val_data(cur);
	BSONObj obj = (*cursor)->next();
	BSONData *data = new BSONData;
	//grab size
	data->length = obj.objsize();
	//alloc space
	data->data = (char*)malloc(data->length*sizeof(char));
	//copy raw data
	memcpy(data->data,obj.objdata(),data->length);
	value ret = alloc_abstract(k_BSONObject,data);
	val_gc(ret,kill_object);
	return ret;
}

//connection
DEFINE_PRIM(n_dbconnect,1);
DEFINE_PRIM(n_getserveraddress,1);

//information
DEFINE_PRIM(n_getdatabasenames,1);
DEFINE_PRIM(n_getcollectionnames,2);

//querying
DEFINE_PRIM(n_countcollectionitems,2);
DEFINE_PRIM(n_insert,3)
DEFINE_PRIM(n_query,5);

//cursor
DEFINE_PRIM(n_cursor_has,1);
DEFINE_PRIM(n_cursor_value,1);

//bson
DEFINE_PRIM(n_bson_encode,1);
DEFINE_PRIM(n_bson_decode,1);
DEFINE_PRIM(n_get_json,1);

