# Stream Socket

Opt-Out Challenge 3 centers around communicating over the Internet. 
In this challenge, you will program a `CppServer` and a `CppClient`.

The `CppServer` must listen for a connection.
After receiving a connection request and performing handshaking, the server must transfer either a screenshot or a snapshot from a webcame to the client.
The `CppClient` must display the image on the client computer.
Upon completing this task, the client closes the connection.
Commit your server code under `3optout/CppServer`, and your client code as a CMake project under `3optout/CppClient`.
Before performing this challenge, we must agree as a group on the interface between the client and the server for interoperability among implementations.


## Action Items

1. Write a program `CppServer` that listen for a connection and provides an image to the client.
2. Write a program `CppCLient` that connects to the server, receives an image, and subsequently displays the image on the local computer.

You should leverage available libraries for that project.
Your solution does not need to work on different OS.
Using Git and GitHub, commit your code as a CMake project in a directory labeled `3optout`.

### Interoperability

We have established the following standards for interoperability (not required):

1. The following command should be sent from the client to the server to retrieve a photo: "get_photo"
2. The server should send the photo (and only the photo, no other data) to the client in a byte array.
3. The photo should be in jpeg format.
