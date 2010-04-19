package nMongo;

class DBConnection {

	var connection:Void;

	static var _connect = neko.Lib.load("nmongo","n_dbconnect",1);
		
	public static function connect(host:String='localhost') {
		var ret = Type.createEmptyInstance(DBConnection);
		untyped ret.connection = _connect(neko.Lib.haxeToNeko(host));
		return ret;
	}

}
