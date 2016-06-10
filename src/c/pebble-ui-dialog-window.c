#include <pebble.h>
#include "pebble-ui-dialog-window.h"

struct UIDialogWindow {
  // "View"
  Window* window;
  Layer* background_layer;
  BitmapLayer* icon_layer;
  TextLayer* message_layer;

  // "Model"
  bool loaded;

  GColor background_color;

  GBitmap* icon_bitmap;
  GAlign icon_alignment;

  char* message_text;
  GColor message_color;
  GFont message_font;
  GTextAlignment message_alignment;
};


// Private Methods
static void prv_window_update_ui(UIDialogWindow* this) {
  if (!this->loaded) return;

  Layer* window_layer = window_get_root_layer(this->window);
  const GRect window_bounds = layer_get_bounds(window_layer);

  bitmap_layer_set_bitmap(this->icon_layer, this->icon_bitmap);
  bitmap_layer_set_compositing_mode(this->icon_layer, GCompOpSet);
  GRect icon_bounds = gbitmap_get_bounds(this->icon_bitmap);

  int16_t icon_x_orig = DIALOG_MESSAGE_WINDOW_MARGIN;

  if (this->icon_alignment == GAlignCenter) {
    icon_x_orig = (window_bounds.size.w - icon_bounds.size.w) / 2;
  } else if (this->icon_alignment == GAlignRight) {
    icon_x_orig = window_bounds.size.w - DIALOG_MESSAGE_WINDOW_MARGIN;
  }

  layer_set_frame(bitmap_layer_get_layer(this->icon_layer), GRect(
    icon_x_orig,
    DIALOG_MESSAGE_WINDOW_MARGIN,
    icon_bounds.size.w,
    icon_bounds.size.h
  ));

  text_layer_set_text_alignment(this->message_layer, this->message_alignment);
  text_layer_set_text_color(this->message_layer, this->message_color);
  text_layer_set_font(this->message_layer, this->message_font);
  text_layer_set_text(this->message_layer, this->message_text);

  layer_set_frame(text_layer_get_layer(this->message_layer), GRect(
    DIALOG_MESSAGE_WINDOW_MARGIN,
    DIALOG_MESSAGE_WINDOW_MARGIN + icon_bounds.size.h,
    window_bounds.size.w - (2 * DIALOG_MESSAGE_WINDOW_MARGIN),
    window_bounds.size.h - (2 * DIALOG_MESSAGE_WINDOW_MARGIN) - icon_bounds.size.h
  ));
}

// Event Handlers
static void prv_on_background_layer_update(Layer* layer, GContext* ctx) {
  UIDialogWindow* this = window_get_user_data(layer_get_window(layer));

  graphics_context_set_fill_color(ctx, this->background_color);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, 0);
}

// Window Handlers
static void prv_window_load(Window* window) {
  UIDialogWindow* this = window_get_user_data(window);

  Layer* window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create and add all the layers
  this->background_layer = layer_create(window_bounds);
  layer_set_update_proc(this->background_layer, prv_on_background_layer_update);

  this->icon_layer = bitmap_layer_create(window_bounds);

  this->message_layer = text_layer_create(window_bounds);
  text_layer_set_background_color(this->message_layer, GColorClear);

  layer_add_child(window_layer, this->background_layer);
  layer_add_child(this->background_layer, bitmap_layer_get_layer(this->icon_layer));
  layer_add_child(this->background_layer, text_layer_get_layer(this->message_layer));

  // Set the loaded flag in the model
  this->loaded = true;

  // Update the UI
  prv_window_update_ui(this);
}

static void prv_window_unload(Window* window) {
  UIDialogWindow* this = window_get_user_data(window);

  // Destroy Layers
  layer_destroy(this->background_layer); this->background_layer = NULL;
  bitmap_layer_destroy(this->icon_layer); this->icon_layer = NULL;
  text_layer_destroy(this->message_layer); this->message_layer =  NULL;

  // Update model
  this->loaded = false;
}

// Public Methods
UIDialogWindow* ui_dialog_window_create(char* message, GBitmap* icon) {
  UIDialogWindow* this = malloc(sizeof(UIDialogWindow));

  // Create the window
  Window* window = window_create();
  window_set_user_data(window, this);

  // Set window handlers
  window_set_window_handlers(window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload
  });

  this->loaded = false;
  this->background_color = GColorClear;
  this->message_color = GColorBlack;
  this->message_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  this->message_alignment = PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft);
  this->icon_alignment = PBL_IF_ROUND_ELSE(GAlignCenter, GAlignLeft);

  this->window = window;
  this->message_text = message;
  this->icon_bitmap = icon;

  return this;
}

void ui_dialog_window_destroy(UIDialogWindow* this) {
  // Destroy Layers
  if (this->background_layer) layer_destroy(this->background_layer);
  if (this->icon_layer) bitmap_layer_destroy(this->icon_layer);
  if (this->message_layer) text_layer_destroy(this->message_layer);

  // Destroy Window
  if (this->window) window_destroy(this->window);

  // Free memory
  if (this) free(this);
  this = NULL;
}

Window* ui_dialog_window_get_window(UIDialogWindow* this) {
  return this->window;
}

void ui_dialog_window_set_background_color(UIDialogWindow* this, const GColor color) {
  this->background_color = color;
  if (this->loaded) prv_window_update_ui(this);
}

void ui_dialog_window_set_label_color(UIDialogWindow* this, const GColor color) {
  this->message_color = color;
  prv_window_update_ui(this);
}

void ui_dialog_window_set_label_font(UIDialogWindow* this, const GFont font) {
  this->message_font = font;
  prv_window_update_ui(this);
}

void ui_dialog_window_set_label_text(UIDialogWindow* this, char* message) {
  this->message_text = message;
  prv_window_update_ui(this);
}

void ui_dialog_window_set_label_alignment(UIDialogWindow* this, GTextAlignment align) {
  this->message_alignment = align;
  prv_window_update_ui(this);
}

void ui_dialog_window_set_icon(UIDialogWindow* this, GBitmap* icon) {
  this->icon_bitmap = icon;
  prv_window_update_ui(this);
}

GAlign ui_dialog_window_set_icon_alignment(UIDialogWindow* this, GAlign align) {
  if (align == GAlignTopLeft || align == GAlignLeft || align == GAlignBottomLeft) {
    this->icon_alignment = GAlignLeft;
    return GAlignLeft;
  }
  if (align == GAlignTopRight || align == GAlignRight || align == GAlignBottomRight) {
    this->icon_alignment = GAlignRight;
    return GAlignRight;
  }

  this->icon_alignment = GAlignCenter;
  return GAlignCenter;
}