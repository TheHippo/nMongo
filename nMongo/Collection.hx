package nMongo;

class Collection {

	static var _getcollectionnames = neko.Lib.load("nmongo","n_countcollectionitems",2);
	static var _insert = neko.Lib.load("nmongo","n_insert",3);
	static var _query = neko.Lib.load("nmongo","n_query",5);

	public var name(default,null):String;
	private var ns:Void;
	private var db:String;
	private var connection:Void;
	
	private function new(name:String, db:String, connection:Void) {
		this.name = name;
		this.db = db;
		this.connection = connection;
		this.ns = untyped neko.Lib.haxeToNeko(this.db+"."+this.name);
	}
	
	public inline function count()
		return Collection._getcollectionnames(this.connection,this.ns)
		
	public inline function insert(obj:BSON)
		return Collection._insert(this.connection,this.ns,untyped obj.data)
	
	public inline function query(query:Query):Cursor
		return Type.createInstance(Cursor,
			[Collection._query(this.connection,this.ns,neko.Lib.haxeToNeko(untyped query._query),untyped query._limit,untyped query._skip),
			this]
		)
	
		


}
