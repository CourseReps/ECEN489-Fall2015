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

This object will be sent (as bytes), on request, from the Teensy to the NUC, and then sent out by the NUC to a central server.
