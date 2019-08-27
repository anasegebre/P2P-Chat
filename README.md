# P2P-Chat

Peer to Peer Chat is a program that allows messaging between different machines without a central server. Users act as both clients and servers in this network and connecting to one peer, connects you to all of them. This project posed several problems in distributed systems, like how it tolerates disconnection from a user and reconnection back into the network from those user's clients.


First, make sure to compile the project and run the command:

$make


To start a new chat:

$./p2pchat `<username>`


To join an existing chat (the port number is displayed at top of chatbox):

$./p2pchat `<username>` `<other user's machine name>` `<port number>`


You can type a message and send it to the chat by pressing **Enter**. You can quit the chat by entering **:quit** or **:q** 
