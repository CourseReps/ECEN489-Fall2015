# Teensy/Serial/UDP broadcast/SQLite

This project uses the Boost library, if it is not installed then the project will not compile.

Making this project will yield two executables

### Server

This is run with the port number to listen on as the one arguments. You may need to enable root access with sudo or run as an Administrator for proper functionality.

The server will wait for broadcasts, and once it receives one, it will wait for a JSON message to parse and push to the database, called photodata.db.


### Client

This is run with the port number to broadcast on as an argument. After launching the executable, it will read from the Teensy over serial, wrap this information into a JSON along with a timestamp, IP, etc., and broadcast this message.
