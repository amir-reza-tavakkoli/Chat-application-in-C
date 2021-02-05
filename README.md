
# GENERAL INFORMATION
Developed by *AMIR REZA TAVAKKOLI*  
Student id 9732593   
Mainly developed for P.O.P course   
Fall 99 shiraz university   
DR.ALI HAMZEH

# PROJECT STRUCTURE
Project name : multi-user chat application using c socket programming  

Brief description : using threading and TCP sockets enables us to communicate Data between multiple clients from multiple places, for simplicity I just used localhost as server, and I used meta-command-line-argument to facilitate communication between clients and server, additionally we have a backup server which can be run (***optionally**)  on any of clients computer, it's job is preventing from disconnection, backup server uses same structure as main server, the only difference is that it runs on port 8081 while main server runs on 8080 ,and it has no other capabilities. 

## REQUIREMENTS AND RUNNING
Any distribution of Linux mainly UBUNTU , GCC compiler  
For compiling: gcc  
For running : ./server ./client ./bserver

## HOW IT WORKS

On client side we use meta-commands to communicate between clients, here how it works :     

**expressions between <> are parameters*

    pressing H ~ for quick help on commands  

    pressing ME ~ tells you your client id on server  

    pressing L ~ prints out all active (online) clients and their names (which by default is "default name")  

    pressing NAME 0 <name> ~ you can name yourself, so others can know who you are using L  

    pressing HI ~ quickly prints out 'hi' to every client  

    pressing Q ~ let you quit your client   

    pressing S <id> <string> ~ let you send a message (string) to clientid id  

    pressing M 0 <string> ~ let you send a message to all clients (multiple clients)  

    pressing F <id> <filepath> ~ you can send a file with filepath to client with clientid id  

## THINGS TO KNOW

First off, it's obvious that sending to multiple clients on localhost (same computer) is not really useful and practical, nevertheless, it only needs a loop to create a command for that but we all know coping a file multiples time on same computer is a waste of memory.  

Next thing is DESKTOP path, in project description specified that the project only needs to run on your computer, so for creating files I used my desktop path on my linux computer which is : ```/home/Foli/desktop  ```.  

So if you need to run this on your computer you might wanna change my PC name to yours.  

And about backup server : it runs on one of clients computers, and when server goes offline, it takes control.  

### RESOURCES
Thing that i used:
- YouTube  
- Google  
- Geegsforgeeks  
- Hands on network programming with c by LEWIS VAN WINKLE  
- ...  