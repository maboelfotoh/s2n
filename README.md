# s2n

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
