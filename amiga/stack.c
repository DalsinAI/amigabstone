/* A Shell gives 4 KB of stack; BStone and SDL 2 want much more. libnix's
 * startup sets this much up.
 * Copyright (c) 2026 AmiCradle Team. GPL 2 or later, like BStone. */
unsigned long __stack = 1024 * 1024;
static const char version[] __attribute__((used)) = "$VER: BStone 1.3.4 Amiga (05.11.2026)";
