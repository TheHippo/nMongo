
class Test {

	public static function main() {
		var c = nMongo.DBConnection.connect('localhost');
		trace(c.getServerAddress());
		trace(c.getDatabaseNames());
		var data = new nMongo.BSON({
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
		trace(c.test.test.count());
		c.foo.foo.insert(data);
		var cur = c.foo.foo.query(new nMongo.Query().limit(5).where("foo","bar"));
		for (i in cur)
			trace(i.json());
		
		//trace(nMongo.DBConnection.connect("localhost").foo.foo.query(new nMongo.Query().limit(1).where("foo","bar")));
	}
}
