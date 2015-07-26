DISCLAIMER

Savage 2: A Tortured Soul (http://www.savage2.com) is a game developed by S2 Games (http://www.s2games.com) and has become abandonware. All game servers hosted by S2 Games have been shut down [http://masterserver.savage2.s2games.com/, last accessed 26/7/2015]. Since the game's release, a community of gamers have come together and spent countless hours of fun playing this game and creating custom content, very little of which has become official. Towards the end of 2014, the lead volunteer maintainer, known by the handle S2Stony, has announced the end of his support, citing lack of cooperation from S2 Games [https://www.facebook.com/pages/Savage-2-A-Tortured-Soul/105471299303?fref=nf, last accessed 26/7/2015]. In a bid to keep the game alive for the community, volunteers have developed and maintained an unofficial master server, chat server, as well as server-side plugins that facilitate administrative tasks such as auto-kicking spammers, and disabling certain in-game functionality for abusers. Due to lack of volunteers, this resource has been made accessible to the public.

This repository contains work done for the unofficial Savage 2 maintenance project. If an individual or company owns the rights to Savage 2: A Tortured Soul and has an issue with this resource being made publicly accessible, they are welcome to message me and request that the resource be removed.

# s2n - cloned from https://github.com/SammyH/s2n

assumes /tmp/s2hooker contains the following files:
buildbanlist.txt
adminlist.txt
both have the format:
id nick

call shuffle vote
S2Daemon 0: 0E 00 00 00 03 E3 A2 C8 5E 02 00 00 00   ........^....
call next map vote
S2Daemon 0: 0F 00 00 00 03 E3 A2 C8 5E 03 00 00 00   ........^....

/*
call next map vote command...
10 00 00 00
03
1e 5f
c8 5e
03 00 00 00

F3 on a next map vote..
09 00 00 00
03
1e 5f
c8 5f
*/

select build ability (human builder)...
0b 00 00 00 03 80 16 c8 4b 75 00

build stronghold main mine (human builder) on ashrock..
0c 00 00 00 03 80 16 c8 08 75 00 22 73 7f 46 96 39 0a 46 17 b7 d1 38 00 00 00 00 00 00 00 00 67 99 8e 43 50 01 00 00
