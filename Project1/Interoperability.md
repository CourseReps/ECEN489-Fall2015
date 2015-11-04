#Interoperability

###Distribution
Ubuntu, lates LTS (long-term stable) release

###Database
SQLite

###Datastructure
JSON

```
{
  "ID":"TeamName",
  "IRRange":"xxx",
  "PumpRate":"xxx",
  "FlowRate":"xxx",
  "SolenoidState":"ON/OFF",
  "Timestamp":"Unix epoch",
  "CurrentIP":"xx.xxx.xxx.xx"
}
```

### Sending data
This object will be sent (as bytes), on request, from the Teensy to the NUC, and then sent out by the NUC to a central client.

Each NUC will be running a TCP server, which will listen for connections from the central client, upon which the NUC's server will send out it's most recent dataset to the central client for plotting/visualization. 
