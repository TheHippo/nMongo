package nMongo;

class DBConnection {

	var connection:Void;

	static var _connect = neko.Lib.load("nmongo","n_dbconnect",1);
	static var _getserveraddress = neko.Lib.load("nmongo","n_getserveraddress",1);
	static var _getdatabasenames = neko.Lib.load("nmongo","n_getdatabasenames",1);
		
	public static function connect(host:String='localhost') {
		var ret = Type.createEmptyInstance(DBConnection);
		untyped ret.connection = DBConnection._connect(neko.Lib.haxeToNeko(host));
		return ret;
	}
	
	public function getServerAddress():String
		return DBConnection._getserveraddress(this.connection)
		
	public function getDatabaseNames():List<String>
		return Lambda.list(neko.Lib.nekoToHaxe(DBConnection._getdatabasenames(this.connection)))

}
