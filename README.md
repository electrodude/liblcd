# liblcd

Serial character LCD abstraction library in C.  Works on Unix, should work on Windows under mingw.

Modular design makes it easy to add support for other displays, operating systems, transport protocols (tty, tcp), etc..

## (Planned) Features

* Minimal display updates
* Windows
* Screen space allocator/scheduler
* Virtual custom characters - define more custom characters than available slots but don't display them all at once, and it will deal with it for you.
* Progress bars
* Input

## Supported LCDs
* Works
	* none yet
* Planned
	* Parallax
	* Scott Edwards Electronics
	* Crystalfontz
	* VT-100
