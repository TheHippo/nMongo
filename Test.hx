
class Test {

	public static function main() {
		var c = nMongo.DBConnection.connect('localhost');
		trace(c.getServerAddress());
		
		var data = nMongo.BSON.encode({
			foo:"bar",
			bar:[1,2,3],
			date:Date.now().getTime(),
			obj: {
				bar:"foo",
				foo:["a","b","c"]
			}
		});
		trace(data.json());
		trace(data.decode());
	}
}
