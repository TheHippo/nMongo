
class Test {

	public static function main() {
		trace("testing");
		nMongo.DBConnection.test();
		trace("test done");
		var c = nMongo.DBConnection.connect('localhost');
		trace(c.getServerAddress());
	}
}
