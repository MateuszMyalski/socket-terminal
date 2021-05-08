# socket-terminal
Simple telnet server for connecting IOT devices in eg. star topology. Implemented few basic commands. Build for debian tested on WSL. Implemented no message timeout. Peer timeout set to 120s, 30s before disconection warning message will be send. To eddit this param check `./inc/client_session.hpp` file. Each connection works in seperate thread. To construct own commands make use of virtual class in file `./inc/command.hpp` and add it to the main server command dispacher in `main.cpp`.
```bash
Usage /server.o [-i IPv4] [-p port] [-c] colors
```
# How to compile
1. Open WSL in server catalog
2. Run: ``make``
3. Run: ``./server.o -c``
4. Default server runs on ``127.0.0.1:8888``
5. To test it connect with standard linux telnet: `telnet 127.0.0.1 8888`


# Commands
1. __ping__ - response `pong`.
2. __echo__ [args] - echo args.
3. __peers__ - display connected peers.
4. __msg__ [IPv4] [message] - send message to the provided ip.