#include <pebble.h>
#include "modules/data.h"
#include "modules/windows.h"


int main(void) {
  init_windows();
  app_event_loop();
  deinit_windows();
}
