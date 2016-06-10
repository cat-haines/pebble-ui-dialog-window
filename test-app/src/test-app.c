#include <pebble.h>
#include "pebble-ui-dialog-window/pebble-ui-dialog-window.h"

static UIDialogWindow *s_window;
static GBitmap* s_icon;
static char *s_message = "Hello World";

static void prv_init(void) {
  s_icon = gbitmap_create_with_resource(RESOURCE_ID_FROG);
  s_window = ui_dialog_window_create(s_message, s_icon);

  ui_dialog_window_set_icon_alignment(s_window, GAlignCenter);
  ui_dialog_window_set_label_alignment(s_window, GTextAlignmentCenter);

  bool animated = true;
  window_stack_push(ui_dialog_window_get_window(s_window), animated);
}

static void prv_deinit(void) {
  gbitmap_destroy(s_icon);
  ui_dialog_window_destroy(s_window);
}

int main(void) {
  prv_init();
  app_event_loop();
  prv_deinit();
}
