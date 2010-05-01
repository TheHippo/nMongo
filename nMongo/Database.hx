package nMongo;

class Database implements Dynamic<Collection> {
	
	static var _getcollectionnames = neko.Lib.load("nmongo","n_getcollectionnames",2);
	
	public var name(default,null):String;
	private var connection:Void;
	
	private function new (connection:Void, name:String) {
		this.connection = connection;
		this.name = name;
	}
	
	public function getCollectionNames():List<String>
		return Lambda.list(neko.Lib.nekoToHaxe(Database._getcollectionnames(this.connection, neko.Lib.haxeToNeko(this.name))))
		
	public inline function openCollection(name:String):Collection
		return Type.createInstance(Collection,[name,this.name,this.connection])
		
	public inline function resolve(name:String):Collection
		return this.openCollection(name)

}
