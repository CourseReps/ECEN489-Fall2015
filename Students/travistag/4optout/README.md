# UDP Broadcast of JSON

Making this project with CMake will yield two executables

###Server
Will most likely need to be run with root/adminitstrator access. Currently listens on port 2015 for UDP broadcasts

###Client
Extracts current IP, wraps into a JSON object, and sends out as a UDP broadcast.
