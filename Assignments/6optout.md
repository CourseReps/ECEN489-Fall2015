# Opt-out 6

For this opt-out challenge you will be putting together several of the pieces from past opt-outs and quick-fire challenges to collectively create a network of sensors with remote databases. 


##Client side

### Teensy sensing / serial reading
The first part of this challenge is to read photosensor data using a Teensy, and read this information via serial connection to your laptop.

### JSON wrapping
Once the client program obtains the photosensor data, you will wrap this into a JSON object along with the client's IP, according to the format laid out in the wiki for 5optout.
For this challenge, we can ignore the authentication/security measures, and just transmit relevant data.

Example:
```
{
  "DeviceID": "xxx",
  "DeviceType": "Photosensor",
  "Data": "xxx",
  "Timestamp":"Unix time stamp (Epoch)",
  "currentIP": "127.0.0.1"
}
```


### Broadcast
As in opt-out 4, you will broadcast this message over UDP with port number 1111.

## Server with database
Your server should be listening for broadcast messages. Once it receives one, it should unwrap the JSON messsage and push the information into an SQLite database.
