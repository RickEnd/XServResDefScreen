#ifndef XSERVRESDEFSCREEEN_H
#define XSERVRESDEFSCREEEN_H

#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>

// #ifndef DEBUG
// #define DEBUG 0
// #endif

void DisplayChangeRes(Display *dpy, int screen_num, Window root_window);
void EchoScreenInfo(Display *dpy, int num_sizes, short **pos_freq, XRRScreenSize *xrrs);
int ChoiceResAndRate(short **pos_freq, int *choice, int num_sizes);

#endif // XSERVRESDEFSCREEEN_H