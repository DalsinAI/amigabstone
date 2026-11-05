Blake Stone (BStone) for AmigaOS 3.x
====================================

A port of BStone (https://github.com/bibendovsky/bstone, at commit
e65a6b40b960a409d7ea840580c2431f56fb6fd4, version 1.3.4), the source port
of JAM Productions' Blake Stone: Aliens of Gold and Planet Strike, for
AmigaOS 3.2 on a 68040 or 68060 with an FPU and an RTG screen, on
GFOpenGPU's SDL 2.

Status: runs in the AmiCradle lab with the Aliens of Gold shareware data
(v3.0): title, high scores, menus, mission and difficulty choice, the
mission briefing and play on floor 1 (walking, turning, the HUD, the
right colours); the menu's Logoff (with the shareware's closing screen)
and Right Amiga+Q quit cleanly to the Workbench. The lab is muted, so the
sound was not heard.


What you need
-------------

- A 68040 or 68060 with an FPU (not an LC040 or EC040), AmigaOS 3.2, an RTG
  card (or the AmiCradle lab), and about 32 MB of free memory.
- GFOpenGPU 0.8 or later: LIBS:GFOpenGPU/SDL2.module (installed by OpenUp).
- The game's data, which is not included (see Licences.txt):
  - Aliens of Gold shareware (1bs30.zip, free to download): AUDIOHED.BS1,
    AUDIOT.BS1, IANIM.BS1, MAPHEAD.BS1, MAPTEMP.BS1, SANIM.BS1,
    VGADICT.BS1, VGAGRAPH.BS1, VGAHEAD.BS1 and VSWAP.BS1;
  - or the full Aliens of Gold (the same names ending .BS6) or Planet
    Strike (.VSI), sold by Apogee (3D Realms), GOG and Steam.
  Copy them into the BStone drawer, beside the program. BStone finds what
  is there; with more than one game, --aog, --aog_sw or --ps picks one.


How to play
-----------

Run Install-BStone (or copy the BStone drawer where you want it), add the
data, then double-click BStone, or from a Shell:

    cd BStone
    BStone [--aog | --aog_sw | --ps]

- Keys, as on the PC: cursor keys walk and turn, Ctrl fires, Alt strafes,
  Shift runs, Space opens doors and uses things, Enter selects in the
  menus, 1-7 pick the weapons. The game's Options change them; the mouse
  works in the window.
- Esc opens the game's menu; Logoff at the bottom quits (Y answers its
  question; the shareware then shows its closing screen until a key).
  Right Amiga+Q, or the window's close gadget, quits at once.
- The settings (bstone_config.txt), saved games, high scores and the log
  are in the drawer's User drawer; an update by Install-BStone keeps it.
  The game opens a 640x480 window; vid_window_mode in
  User/bstone_config.txt chooses another mode (not tried here).


Building
--------

GFOpenAmigaGCC 16.2.0-open6 (the os32-gcc16-open6 stove, with the GFOpenGPU
SDK 0.8 in it):

    make -f amiga/Makefile.amigaos -j4 dist STOVE=/path/to/os32-gcc16-open6

STOVE defaults to ~/AmiCradle/stoves/os32-gcc16-open6. Output:
build/amigaos/BStone; 'dist' makes dist/BStone/ (the program, ReadMe,
ReadMe-bstone, COPYING.txt, Licences.txt, an empty User drawer, with
GlowIcons), dist/BStone.info and dist/Install-BStone with its icon. The
icons are made from amiga/icons/icons.json and Install-BStone from
amiga/installer.json with amicradle's scripts/port_icons.py and
port_installer.py (see "about" in icons.json).


What was changed
----------------

- amiga/Makefile.amigaos: upstream's src/bstone/CMakeLists.txt list with
  Nuked OPL3, xBRZ and the Vulkan loader, linked with the stove's
  libSDL2.a, libpthread and libatomic. libSDL2.a's GL half needs libGL.a,
  whose gl* names meet BStone's own GL function pointers, so a copy of it
  with every symbol weak is linked. bstone_version_.h is made from its .in.
- amiga/amiga_posix.cpp: dlopen() that finds nothing (no OpenAL, OpenGL or
  Vulkan library: BStone falls back to its software renderer and its own
  SDL audio mixer), flock(), execle() and clock_gettime(), which libnix
  lacks. amiga/stack.c: a 1 MB stack and the $VER string.
- id_vl.cpp: always the software renderer (GFOpenGPU's GL is not the GL 2.0
  / 3.2 or Vulkan BStone's hardware renderers want).
- 3d_main.cpp, bstone_fs_utils.cpp: the data is read from PROGDIR:; the
  settings, saves, high scores and log go to PROGDIR:User.
- Big-endian (68k) fixes: the graphics dictionary (VGADICT), the 3-byte
  chunk offsets (VGAHEAD), chunk lengths, the picture table, the font
  headers, MAPHEAD and the map headers are read as little-endian words
  (id_ca.cpp, jm_free.cpp); the text presenter's two-letter codes
  (jm_tp.h); the software renderer's 0xAARRGGBB pixels are
  SDL_PIXELFORMAT_ARGB8888 (bstone_sys_texture_sdl2.cpp); movie.cpp's
  AnimFrame is read as 12 bytes.
- bstone_cvar.cpp: a string setting that is not a number takes its default
  without a C++ throw. GCC 16 on libnix cannot unwind a throw made while
  the static objects are built (before main()); the program aborted there.
- bstone_mt_task_mgr.cpp: no worker thread (it polled its queue every
  millisecond); its tasks (screenshots) run in the thread that adds them.
- bstone_sys_event_mgr_sdl2.cpp: SDL_QUIT (the close gadget) and Right
  Amiga+Q end the game through its own Quit(), so the settings and high
  scores are written.


Known limits
------------

- Speed: the software renderer draws a 640x480 window at 320x200 game
  pixels with widescreen on; in the lab (on a heavily loaded host) the
  menus' fades took a few seconds and play was slow. vid_is_widescreen "0"
  in User/bstone_config.txt gives the 68040 less to draw. The frame rate on
  a real 68040 was not measured.
- No OpenAL: the sound is BStone's own mixer through SDL 2 (AdLib music and
  effects emulated by DBOPL, which costs the CPU; snd_is_music_enabled "0"
  in User/bstone_config.txt turns the music off). Not heard in the muted
  lab.
- Only the Aliens of Gold shareware was tried; the full games and Planet
  Strike were not.
- Full screen (vid_window_mode) was not tried.


Licence
-------

The program is under the GNU GPL, version 2 or later, with BStone's new
code under MIT (COPYING.txt); xBRZ (GPL 3) is linked in. The game data is
Apogee's and is not included; see Licences.txt. The port's own files are
GPL 2 or later, Copyright (c) 2026 AmiCradle Team.
