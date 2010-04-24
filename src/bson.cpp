#include <iostream>
#include <neko.h>
#include <mongo/client/dbclient.h>

using namespace mongo;

DEFINE_KIND(k_BSONObject);

value print( value v ) {
	switch( val_type(v) ) {
	case VAL_NULL:
		printf("null");
		break;
	case VAL_INT:
		printf("int : %d",val_int(v));
		break;
	case VAL_FLOAT:
		printf("float : %f",val_float(v));
		break;
	case VAL_BOOL:
		printf("bool : %s",val_bool(v)?"true":"false");
		break;
	case VAL_ARRAY:
		printf("array : size %d",val_array_size(v));
		break;
	case VAL_FUNCTION:
		printf("function : %d args",val_fun_nargs(v));
		break;
	case VAL_STRING:
		printf("string : %s (%d bytes)",val_string(v),val_strlen(v));
		break;
	case VAL_OBJECT:
		printf("object");
		break;
	case VAL_ABSTRACT:
		printf("abstract of kind %X",val_kind(v));
		break;
	default:
		printf("?????");
		break;
	}
	printf("\n");
	return val_null;
}


void iter_object(value v, field f, void* data) {
		printf("%s: ",val_string(val_field_name(f)));
		print(v);		
}

//decoding a neko object into bson data
value n_bson_encode(value obj) {
	if (val_type(obj) != VAL_OBJECT)
		failure("Given value is no object!");
	
	BSONObjBuilder *build;
	val_iter_fields(obj,iter_object,build);
	
	value ret = alloc_abstract(k_BSONObject,build);
	
	return ret;
}
//decode bson data into a neko object
value n_bson_decode(value str) {
	return val_null;
}
