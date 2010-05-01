#include <iostream>
#include <sstream>
#include <neko.h>
#include <mongo/client/dbclient.h>
#include "bson.h"

using namespace mongo;

DEFINE_KIND(k_BSONObject);

//free data, called by GC
void kill_object(value obj) {
	BSONData *data = (BSONData*)val_data(obj);
	free(data->data);
	delete data;
}

const char * intToString(int i) {
    ostringstream temp;
    temp << i;
    return temp.str().c_str();
}

void iter_object(value v, field f, void* data);


void appendToBuilder(value v, const char *name, BSONObjBuilder *b) {
	switch (val_type(v)) {
		case VAL_NULL:
			b->appendNull(name);
			break;
		case VAL_INT:
			b->append(name,val_int(v));
			break;
		case VAL_FLOAT:
			b->append(name,val_float(v));
			break;
		case VAL_BOOL:
			{
				if (v==val_true)
					b->appendBool(name,1);
				else
					b->appendBool(name,0);
			}
			break;
		case VAL_ARRAY:
			{
				//build subobject, object indexes are 0, 1, 2, usw.
				BSONObjBuilder *build = new BSONObjBuilder();
				value *ar = val_array_ptr(v);
				int n = val_array_size(v);
				for (int i=0; i<n; i++) {			
					appendToBuilder(ar[i],intToString(i),build);
				}
				b->appendArray(name,build->obj());
				delete build; 
			}
			break; 
		case VAL_STRING:
			b->append(name,val_string(v));
			break;
		case VAL_OBJECT:
			{ //c++ ... feels strange
				BSONObjBuilder *build = new BSONObjBuilder();
				//recrusion
				val_iter_fields(v,iter_object,build);
				b->append(name,build->obj());
				delete build; 
			}
			break;		
		default: // function undefined
			failure("Strange value!");
			break;
	}
}

//encode loop function
void iter_object(value v, field f, void* data) {	
	char *name = val_string(val_field_name(f));
	BSONObjBuilder *b = (BSONObjBuilder*)data;
	appendToBuilder(v,name,b);	
}

BSONData* buildData(BSONObjBuilder *build) {
	BSONObj bo = build->obj();	
	BSONData *data = new BSONData;
	data->length = bo.objsize();
	data->data = (char*)malloc(data->length*sizeof(char));
	memcpy(data->data,bo.objdata(),data->length);
	return data;
}

//decoding a neko object into bson data
value n_bson_encode(value obj) {
	if (val_type(obj) != VAL_OBJECT)
		failure("Given value is no object!");
	
	BSONObjBuilder *build = new BSONObjBuilder();
	val_iter_fields(obj,iter_object,build);
	BSONData *data = buildData(build);
	delete build;
	value ret = alloc_abstract(k_BSONObject,data);
	val_gc(ret,kill_object);
	return ret;
}

//called by the decode loop
value getObject(BSONElement *el, BSONObj *obj) {
	value ret;
	switch (el->type()) {
		case String:
			ret = alloc_string(obj->getStringField(el->fieldName()));
			break;
		case NumberInt:
			ret = alloc_int(obj->getIntField(el->fieldName()));
			break;
		case NumberDouble:
			ret = alloc_float(el->number());
			break;
		case Array:
			{
				BSONObj x = el->embeddedObject();
				ret = alloc_array(x.nFields());
				value *ptr = val_array_ptr(ret);
				BSONObjIterator it(x);
				BSONElement el = it.next();
				int i = 0;
				while (el.eoo() != true) {
					ptr[i++] = getObject(&el,&x);
					el = it.next();
				}
			}
			break;
		case Object:
			{
				BSONObj x = el->embeddedObject();
				ret = alloc_object(NULL);
				BSONObjIterator it(x);
				BSONElement el = it.next();
				while (el.eoo() != true) {
					alloc_field(ret,val_id(el.fieldName()),getObject(&el,&x));
					el = it.next();
				}
			}
			break;
		case Bool:
			ret = alloc_bool(el->boolean());
			break;
		case jstNULL:
			ret = val_null;
			break;
		default:
			failure("Could not decode element");
			//maybe a little bit hard, needs to be tested
			break;
	}
	return ret;
}

//decode bson data into a neko object
value n_bson_decode(value o) {
	val_check_kind(o,k_BSONObject);
	BSONData *data = (BSONData*)val_data(o);
	string test (data->data,data->length);
	BSONObj obj (test.c_str(),false);
	
	BSONObjIterator it(obj);
	BSONElement el = it.next();
	value ret = alloc_object(NULL);
	value insert = val_null;
	while(el.eoo()!=true) {
		insert = getObject(&el, &obj);
		alloc_field(ret,val_id(el.fieldName()),insert);
		el = it.next();
	}	
	return ret;
}

int checkBSONValue(value o) {
	val_check_kind(o,k_BSONObject);
	return 1;
}

//return the json string representation of bson object
value n_get_json(value o) {
	val_check_kind(o,k_BSONObject);
	BSONData *data = (BSONData*)val_data(o);
	string test (data->data,data->length);
	BSONObj obj (test.c_str(),false);
	return alloc_string(obj.jsonString().c_str());
}
