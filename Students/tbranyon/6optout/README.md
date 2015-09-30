6OptOut

This CMake project contains a UDP client and server for Linux

The client will read ADC values from the Teensy board over USB and send them with other data in a JSON object over UDP. Broadcast is currently not working, so the IP of the server must be known.

The server listens on its own IP for UDP packets and wraps them into an sqlite database.

Usage: server <database name>
       client <IP address of server>
       
The client may also be called continously at 2Hz from the script "runclient"
