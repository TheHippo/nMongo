package nMongo;

class Query {

	var _query:Dynamic;
	var _limit:Int;
	var _skip:Int;
	
	public function new() {
		this._query = {}
		this._limit = 0;
		this._skip = 0;
	}
		
	public function where(name:String,value:Dynamic):Query {
		Reflect.setField(this._query,name,value);
		return this;
	}
	
	public function limit(limit:Int):Query {
		this._limit = limit;
		return this;
	}
	
	public function skip(skip:Int):Query {
		this._skip = skip;
		return this;	
	}
		
}
