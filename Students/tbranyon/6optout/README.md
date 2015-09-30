6OptOut

This CMake project contains a UDP client and server for Linux

The client will read ADC values from the Teensy board over USB and send them with other data in a JSON object over UDP. Broadcast is currently not working, so the IP of the server must be known. Trying to send to the broadcast IP of an internet interface yields a "Permission denied" error.

The server listens for UDP packets and wraps them into an sqlite database.

Usage: `server <database name> --
       client <IP address of server>`
       
The client may also be called continously at 2Hz from the script "runclient"

Interoperability issues: This program deviates from the spec in that it uses port 2015 instead of port 1111. Ports below 2000 on linux are privileged ports. Trying to run the server on port 1111 on Texas A&M WiFi fails.
