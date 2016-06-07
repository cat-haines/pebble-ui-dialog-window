# UIDialogWindow

A library to create and customize simple dialog windows for Pebble watchapps.

![Aplite](/images/aplite.png) ![Basalt](/images/basalt.png) ![Chalk](/images/chalk.png)

## Behavior

`UIDialogWindow`s are pre-configured, customizable dialog windows that
developers can create and use in their Pebble watchapps.

By default, the dialog window has the following properties and behavior:
- Default Background Color: `GColorClear`
- Default Font Color: `GColorBlack`
- Default Font: `FONT_KEY_GOTHIC_24_BOLD`
- `back` button will close the window

## Installation

```
pebble package install pebble-ui-dialog-window
```

## Sample Usage

```c
#include <pebble.h>
#include "pebble-ui-dialog-window/pebble-ui-dialog-window.h"

UIDialogWindow* dialog;
char* dialog_message = "Battery is low! Connect Pebble to charging cable.";
GBitmap* dialog_icon = NULL;

static void init() {
  dialog_icon = gbitmap_create_with_resource(RESOURCE_ID_WARNING);
  dialog = ui_dialog_window_create(dialog_message, dialog_icon);
  ui_dialog_window_set_background_color(dialog, GColorYellow);

  Window* window = ui_dialog_window_get_window(dialog);
  window_stack_push(window, true);
}

static void deinit() {
  gbitmap_destroy(dialog_icon);
  if (dialog) ui_dialog_window_destroy(dialog);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
```

### Documentation

#### `UIDialogWindow* ui_dialog_window_create(char* message, GBitmap* icon);`

Creates a new `UIDialogWindow` object that can be used to display and customize
a dialog window.

**NOTE**: The `UIDialogWindow` does not make copies of `message` and `icon`
parameters, so the developer must ensure the objects persist for the lifetime of
the dialog window.

#### `void ui_dialog_window_destroy(UIDialogWindow* this);`

Frees all memory associated with the `UIDialogWindow` object.

**NOTE**: `ui_dialog_window_destroy` will not free the memory associated with
the `message` or `icon` parameters passed into `ui_dialog_window_create`.

#### `Window* ui_dialog_window_get_window(UIDialogWindow* this);`

Returns a [Window](https://developer.pebble.com/docs/c/User_Interface/Window)
object that can be pushed onto the
[WindowStack](https://developer.pebble.com/docs/c/User_Interface/Window_Stack/)
or manipulated in other ways (such as assigning ClickHandlers with
[window_set_click_config_provider](https://developer.pebble.com/docs/c/User_Interface/Window/#window_set_click_config_provider_with_context).

#### `void ui_dialog_window_set_background_color(UIDialogWindow* this, const GColor color);`

Sets the `UIDialogWindow`'s background color.

#### `void ui_dialog_window_set_label_color(UIDialogWindow* this, const GColor color);`

Set's the font color of the `UIDialogWindow`'s label.

#### `void ui_dialog_window_set_label_font(UIDialogWindow* this, const GFont font);`

Set's the font of the `UIDialogWindow`'s label.

#### `void ui_dialog_window_set_label_text(UIDialogWindow* this, char* message);`

Set's the text of the `UIDialogWindow`'s label.

#### `void ui_dialog_window_set_icon(UIDialogWindow* this, GBitmap* icon);`

Set's the image of the `UIDialogWindow`'s icon.

## License

This library is licensed under the [MIT license](/LICENSE).
