package nMongo;

class NMongo {

	static var _test:Void->String = neko.Lib.load("nmongo","test",0);
	
	public static function test()
		return NMongo._test()

}
