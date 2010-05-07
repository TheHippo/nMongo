package nMongo;


class BSON {

	var data:Void;

	static var _decode = neko.Lib.load("nmongo","n_bson_decode",1);
	static var _encode = neko.Lib.load("nmongo","n_bson_encode",1);
	static var _json = neko.Lib.load("nmongo","n_get_json",1);
	
	public inline static var empty:BSON = new BSON({});
	
	public inline function decode():Dynamic
		return neko.Lib.nekoToHaxe(BSON._decode(this.data))
	
	public function new(obj:Dynamic)
		this.data = BSON._encode(neko.Lib.haxeToNeko(obj))
		
	public inline function json():String
		return BSON._json(this.data)

}
