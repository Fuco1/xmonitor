#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xinerama.h>
#include <X11/Xlib.h>

#include <stdio.h>
#include <stdlib.h>

void execute_and_wait(char* program) {
  FILE* output = popen(program, "r");
  pclose(output);
}

int connected_screens(Display* d) {
  XRROutputInfo* info;
  XRRScreenResources* screen;
  screen = XRRGetScreenResources(d, DefaultRootWindow(d));

  int iscres;
  int connected = 0;
  for (iscres = screen->noutput; iscres > 0;) {
    --iscres;

    info = XRRGetOutputInfo(d, screen, screen->outputs[iscres]);
    if (info->connection == RR_Connected) {
      connected++;
    }
    XRRFreeOutputInfo(info);
  }
  return connected;
}

int main() {
  Display* d;
  XEvent e;

  d = XOpenDisplay(NULL);
  if (!d) {
    fprintf(stderr, "Cannot open display\n");
    exit(1);
  }

  XRRSelectInput(d, DefaultRootWindow(d), RRScreenChangeNotifyMask);

  /* Buffer for command */
  char cmd[1024];
  char* home = getenv("HOME");
  /* Time of last update */
  int screens = connected_screens(d);
  while (1) {
    XNextEvent(d, &e);
    /* Safety first kids! */
    XRRScreenChangeNotifyEvent* event = (XRRScreenChangeNotifyEvent*)&e;
    XRRUpdateConfiguration(&e);

    int current_screens = connected_screens(d);
    fprintf(stderr, "Number of connected monitors: %d [%dx%d]\n", current_screens, event->width, event->height);

    if (current_screens != screens) {
      screens = current_screens;
      char* operation;
      if (current_screens == 1) {
        operation = "detach";
      } else {
        operation = "attach";
      }
      sprintf(cmd, "run-parts %s/.screenlayout/%s.d", home, operation);
      execute_and_wait(cmd);
      while (XPending(d)) {
        XNextEvent(d, &e);
      }
    }
    fflush(stderr);
  }

  XCloseDisplay(d);

  return 0;
}
