#pragma once

#define DIALOG_MESSAGE_WINDOW_MARGIN   10

typedef struct UIDialogWindow {
  // "View"
  Window* window;
  Layer* background_layer;
  Layer* icon_layer;
  TextLayer* message_layer;

  // "Model"
  bool loaded;

  GColor background_color;
  GBitmap* icon_bitmap;
  char* message_text;
  GColor message_color;
  GFont message_font;
} UIDialogWindow;

UIDialogWindow* ui_dialog_window_create(char* message, GBitmap* icon);
void ui_dialog_window_destroy(UIDialogWindow* this);
Window* ui_dialog_window_get_window(UIDialogWindow* this);

void ui_dialog_window_set_background_color(UIDialogWindow* this, const GColor color);
void ui_dialog_window_set_label_color(UIDialogWindow* this, const GColor color);
void ui_dialog_window_set_label_font(UIDialogWindow* this, const GFont font);
void ui_dialog_window_set_label_text(UIDialogWindow* this, char* message);
void ui_dialog_window_set_icon(UIDialogWindow* this, GBitmap* icon);
