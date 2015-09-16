# More Sockets

Opt-Out Challenge 4 continues our exploration of [Internet protocols](https://en.wikipedia.org/wiki/Internet_Protocol).
It also builds on past work.
In this challenge, you must program several components.

## JSON Object

The first part of the challenge is to query your own computer for its current IP address.
Then, you must create a simple [JSON](https://en.wikipedia.org/wiki/JSON) object that contains the following information.

```
{
  "firstName": "name",
  "lastName": "Doe",
  "gitUserName": "JaneDoe",
  "currentIP": "127.0.0.1"
}
```


## Broadcast

You have to program an application that uses [UDP](https://en.wikipedia.org/wiki/User_Datagram_Protocol) [broadcast](https://en.wikipedia.org/wiki/Broadcast_address) to share you JSON file with your peers.


# Server

Your server should be listening for a UDP broadcast message.
It should then extract the information contained into JSON file and place it into a local [SQLite](https://www.sqlite.org/) database/file.


## Interoperability

You should establish a standards for interoperability (required):

1. Port number.
2. Message format.
