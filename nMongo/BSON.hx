package nMongo;


class BSON {

	var data:Void;

	static var _decode = neko.Lib.load("nmongo","n_bson_decode",1);
	static var _encode = neko.Lib.load("nmongo","n_bson_encode",1);
	static var _json = neko.Lib.load("nmongo","n_get_json",1);
	
	public inline function decode<T>():T
		return neko.Lib.nekoToHaxe(BSON._decode(this.data))
	
	public static inline function encode<T>(object:T):BSON
		return new BSON(BSON._encode(neko.Lib.haxeToNeko(object)))
	
	private function new(data:Void)
		this.data = data
		
	public function json():String
		return BSON._json(this.data)


}
