// Modified script provided by MONGO DB to test connection
using MongoDB.Driver;
using MongoDB.Bson;

var connectionUri = Environment.GetEnvironmentVariable("MONGO_URI");
if (connectionUri == null)
{
    Console.WriteLine("MONGO_URI env var could not be loaded");
    Environment.Exit(0);
}

var settings = MongoClientSettings.FromConnectionString(connectionUri);

// Set the ServerApi field of the settings object to Stable API version 1
settings.ServerApi = new ServerApi(ServerApiVersion.V1);

// Create a new client and connect to the server
var client = new MongoClient(settings);

// Send a ping to confirm a successful connection
try {
  var result = client.GetDatabase("admin").RunCommand<BsonDocument>(new BsonDocument("ping", 1));
  Console.WriteLine("Pinged your deployment. You successfully connected to MongoDB!");
} catch (Exception ex) {
  Console.WriteLine(ex);
}