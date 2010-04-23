package nMongo;

class DBConnection {

	var connection:Void;

	static var _connect = neko.Lib.load("nmongo","n_dbconnect",1);
	static var _getserveraddress = neko.Lib.load("nmongo","n_getserveraddress",1);
	static var _test = neko.Lib.load("nmongo","n_test",0);
		
	public static function connect(host:String='localhost') {
		var ret = Type.createEmptyInstance(DBConnection);
		untyped ret.connection = _connect(neko.Lib.haxeToNeko(host));
		return ret;
	}
	
	public static function test():Void
		return DBConnection._test()
	
	public function getServerAddress():String
		return DBConnection._getserveraddress(this.connection)

}
