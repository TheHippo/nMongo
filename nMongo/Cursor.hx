package nMongo;

class Cursor {

	static var _has = neko.Lib.load("nmongo","n_cursor_has",1);
	static var _value = neko.Lib.load("nmongo","n_cursor_value",1);

	var _cursor:Void;
	var _collecion:Collection;

	private function new(cursor:Void,collection:Collection) {
		this._collecion = collection;
		this._cursor = cursor;
	}
	
	public function iterator():Iterator<BSON> {
		var c = this;
		return {
			hasNext:function() return Cursor._has(c._cursor),
			next:function() {
				var b = Type.createEmptyInstance(BSON);
				untyped b.data = Cursor._value(c._cursor);
				return b;
			}
		};	
	}
	
}
