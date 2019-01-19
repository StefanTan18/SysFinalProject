# SysFinalProject: Rise of the Mafia

Team Members:
Stefan Tan, Reed Scheinberg, Dahong Jiang

Class Period:
4

## Description:
In a peaceful town, a mysterious mafia found a way to blend in among the happy citizens.
No one knew of this arrival until the clock struck midnight. A scream raced through the streets,
followed by utter silence. The next morning, a citizen lay dead. The mafia has struck. It is up
to the people of the town to eliminate the mafia, through the crude system of votes and
execution. Whether or not the citizens have made the right choice will be signaled by what they
see in the morning; another body or clear tranquility. You will play your part in this story
and your decisions will mark your fate.

## Libraries:
-<stdio.h>
-<stdlib.h>
-<unistd.h>
-<sys/types.h>
-<sys/socket.h>
-<netinet/in.h>
-<arpa/inet.h>
-<netdb.h>
-<string.h>
-<errno.h>
-<sys/shm.h>
-<sys/ipc.h>
-<ctype.h>
-<time.h>

## Instructions:
- Setup:
  1. Clone the repository onto your machine
  2. Navigate to the repo in the terminal, and type "make forking"
  3. The host user needs to run ./server in that terminal
  4. Open a new terminal process, on the same machine, navigate to the repo, and run ./client
  5. As prompted, enter the number of players that are going to be participating

Now that the game is setup, lets play!

- Game Assumptions:
  1. All users who are playing are in the same room, and can talk amongst each other
  2. The host is using either a Windows, Linux, or Mac operating system

- Game Overview:
  - Each player will be randomly assigned a role that they should not share with other users.  
  - The two roles are civilian and mafia.
  - Civilian:
    - Objective: Kill the mafia members by voting their death through day town votes
    - Civilians should be questioning other town members during the day to try and figure out who the mafia member is
      - Be careful, you might accidentally vote to hand another civilian!
      - Majority vote is required to kill another player
    - Can be killed at night
  - Mafia:
    - Objective: Kill all of the town members
    - Can use their kill ability only at night against any other player
    - Mafia members should be careful to not take action that would possibly hint at their role during the day
  - The game design is essentially pass 'n play (turn based), meaning users will have to hand the computer to each other to take action such as voting
  - Users should close their eyes at night to prevent them from seeing who the mafia member is
  - A common practice is to have one non-player person act as the "moderator", who makes sure people aren't peeking at night and in general, makes the game runs smoothly

- Victory Conditions:
  - Mafia Victory: All other town members have been killed
  - Civilian Victory: All mafia members have been killed

- Similar Games: (for the mafia diehards who want something else to do)
  - "Town of Salem"
