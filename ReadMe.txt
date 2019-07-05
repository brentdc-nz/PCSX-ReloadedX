//
// PCSX-ReloadedX - Pre-Beta v0.7
//

This is a port of PCSX-Reloaded to Xbox using hardware rendered graphics.

Note: This is still a work in progress and in pre-beta/alpha stages.

Knowen bugs:
- Scissor screwed after re-entering game from GUI until the game sets it again (usually makes menus look broken).
- Analog sticks stop working after re-entering from GUI (I think I know the issue, should be an easy fix).
- Fix rumble, it currently works but has a bug that causes it to say on sometimes.

Main remaining tasks:
- Explore speed optimizations further. 
- Get built in cheats working.
- Get save states working.