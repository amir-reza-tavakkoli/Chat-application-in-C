# Project description

Multi-user chat application using C and socket programming.

Using threading and TCP sockets enable us to communicate Data between multiple clients from multiple places, for simplicity I just used localhost as the server, and I used meta-command-line-argument to facilitate communication between clients and server, additionally, we have a backup server that can be run (**optionally**) on any of clients, it's job is preventing from disconnection, the backup server uses the same structure as the main server, the only difference is that it runs on port 8081 while the main server runs on 8080, and it has no other capabilities.

## Author Information

Developed by _AMIR REZA TAVAKKOLI.
Mainly developed for principles of programming course.

## REQUIREMENTS AND RUNNING

Any distribution of Linux mainly UBUNTU, GCC compiler.
For compiling, run this in the project directory:

```shell
 gcc
```

After that, we can run them:

```shell
./server
./client
./backup-server
```

## HOW IT WORKS

On the client side, we use meta-commands to communicate between clients, here is how it works :

_expressions between <> are parameters_

    pressing H ~ for quick help on commands

    pressing ME ~ tells you your client id on the server

    pressing L ~ prints out all active (online) clients and their names (which by default is "default name")

    pressing NAME 0 <name> ~ you can name yourself, so others can know who you are using L

    pressing HI ~ quickly prints out 'hi' to every client

    pressing Q ~ lets you quit your client

    pressing S <id> <string> ~ let you send a message (string) to clientid id

    pressing M 0 <string> ~ let you send a message to all clients (multiple clients)

    pressing F <id> <filepath> ~ you can send a file with filepath to client with clientid id

## THINGS TO KNOW

First off, it's obvious that sending to multiple clients on localhost (same computer) is not really useful and practical, nevertheless, it only needs a loop to create a command for that but we all know coping a file multiples time on the same computer is a waste of memory.

The next thing is the DESKTOP path, in the project description, it is specified that the project only needs to run on your computer, so for creating files I used my desktop path on my Linux computer which is :
`/home/Foli/desktop `

So if you need to run this on your computer you might wanna change my PC name to yours.

And about the backup server, it runs on one of the client's computers, and when the server goes offline, it takes control.

### Resources

Things that I used:

-   YouTube
-   Google
-   Geegsforgeeks
-   Hands-on network programming with c by LEWIS VAN WINKLE
-   ...
