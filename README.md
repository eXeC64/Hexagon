Hexagon
=======
Simple immitation of Super Hexagon

About
-----
This is a quick and simple game I threw together based off of Super Hexagon.
It was done as an experiment in games programming using the MVC architecture.
I also tried to apply some functional style to the code, inspired by Carmack's
blog on the subject. It was fun to write, and it's fun to poke at.

Known Issues
------------
 * Player collision is visually inaccurate - The player is positioned on the
edge of a circle instead of a hexagon so they appear to be father out than
they really are. This shouldn't be very difficult to correct, just take the
corners of the hexagon and do linear interpolation along a side to find
where the tip of player's arrow should be, then you can position the base
towards the world's origin.
 * Obstacles are not spawned in fun patterns - The game is boring, and unless
meaningful and interesting patterns of obstacles are created it will remain
boring, no matter how much you 'juice' it.
 * No music - Funnily enough you can generate the start of one of SuperHexagon's
songs with a few lines of C. If I could remember the snippet I'd just use that
and loop it, until then the game is silent.
 * No sound effects - Some nice sound effects would really lend themselves
towards the experience
 * No high scores - Obviously this should be subject to always-online DRM with
lots of safeguards that hassle the player(!)
 * No menu or settings - A way to choose various difficulties and change
settings would really bring this up to the level of Super Hexagon.

License
-------
This README and all the included source code is licensed under the GNU General
Public License Version 3.
