# L2CAP COC FLood Test

Application for opening a L2CAP COC between two nodes and allowing to flood this
connection with dummy data.

The nodes `source` and `sink` are configured in a way, that they connect
automatically based on the value of the name field in their advertising data.

Use the 'flood' shell command on the `source` node to start flooding, e.g.
```
flood 100 10000
```
to send 10k chunks of 100 byte each over that channel.
