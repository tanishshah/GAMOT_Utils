using UnityEngine;
using MongoDB.Driver;
using System;

public class tester : MonoBehaviour
{
    private User _user;

    public void TestFeature()
    {
        Debug.Log("Testing this feature");

        var connectionUri = Environment.GetEnvironmentVariable("MONGO_URI");
        if (connectionUri == null)
        {
            Debug.Log("MONGO_URI env var could not be loaded");
            Environment.Exit(0);
        }

        var client = new MongoClient(connectionUri);
        var collection = client.GetDatabase("GAMOT").GetCollection<User>("users");

        _user = new User();
        _user.UserName = "unity";
        _user.Password = "password";

        try
        {
            collection.InsertOne(_user);
            Debug.Log("Inserting new user: " + _user.UserName);
        } catch (Exception ex)
        {
            Debug.Log(ex.ToString());
        }      
    }
}
