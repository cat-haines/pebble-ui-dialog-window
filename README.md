# Dialog Window

This package enables [Pebble developers](https://developer.pebble.com) to create
and customize simple dialog windows in their Pebble watchapps. Dialog windows
consist of an icon, and a message.

![Aplite](/images/aplite.png) ![Basalt](/images/basalt.png) ![Chalk](/images/chalk.png)

## Documentation

### UIDialogWindow* ui_dialog_window_create(char* message, GBitmap* icon);

### void ui_dialog_window_destroy(UIDialogWindow* this);
### Window* ui_dialog_window_get_window(UIDialogWindow* this);

### void ui_dialog_window_set_background_color(UIDialogWindow* this, const GColor color);

### void ui_dialog_window_set_label_color(UIDialogWindow* this, const GColor color);

### void ui_dialog_window_set_label_font(UIDialogWindow* this, const GFont font);

### void ui_dialog_window_set_label_text(UIDialogWindow* this, char* message);

### void ui_dialog_window_set_icon(UIDialogWindow* this, GBitmap* icon);

## License

This library is licensed under the [MIT license](/LICENSE).