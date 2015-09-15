# C++ TCP Sockets

Making this project will yield two executables

### Server

This is run with no arguments. You may need to enable root access with sudo or run as an Administrator for proper functionality.

The server will wait for connections, and once it receives one, will wait for the command "get_photo", upon which it will take a screenshot and send the image over.


### Client

This is run with the hostname or IP address of the server as an argument. After launching the executable, you can type a command to send to the server. As mentioned above, sending "get_photo" will tell the server to send back a current screenshot.
