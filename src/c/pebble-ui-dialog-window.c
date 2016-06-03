#include <pebble.h>
#include "./ui_dialog_window.h"

// Private Methods
static void prv_window_update_ui(UIDialogWindow* this) {
  if (!this->loaded) return;

  Layer* window_layer = window_get_root_layer(this->window);
  const GRect window_bounds = layer_get_bounds(window_layer);
  GRect icon_bounds = gbitmap_get_bounds(this->icon_bitmap);

  layer_set_frame(this->icon_layer, GRect(
    PBL_IF_ROUND_ELSE((window_bounds.size.w - icon_bounds.size.w) / 2, DIALOG_MESSAGE_WINDOW_MARGIN),
    DIALOG_MESSAGE_WINDOW_MARGIN,
    icon_bounds.size.w,
    icon_bounds.size.h
  ));

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

static void prv_on_icon_layer_update(Layer* layer, GContext* ctx) {
  UIDialogWindow* this = window_get_user_data(layer_get_window(layer));

  GRect bounds = layer_get_bounds(layer);
  GRect bitmap_bounds = gbitmap_get_bounds(this->icon_bitmap);
  graphics_context_set_compositing_mode(ctx, GCompOpSet);

  graphics_draw_bitmap_in_rect(ctx, this->icon_bitmap, (GRect){
    .origin = GPoint(0,0),
    .size = bitmap_bounds.size
  });
}

// Window Handlers
static void prv_window_load(Window* window) {
  UIDialogWindow* this = window_get_user_data(window);

  Layer* window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  // Create and add all the layers
  this->background_layer = layer_create(window_bounds);
  layer_set_update_proc(this->background_layer, prv_on_background_layer_update);

  this->icon_layer = layer_create(window_bounds);
  layer_set_update_proc(this->icon_layer, prv_on_icon_layer_update);

  this->message_layer = text_layer_create(window_bounds);
  text_layer_set_text_alignment(this->message_layer, PBL_IF_ROUND_ELSE(GTextAlignmentCenter, GTextAlignmentLeft));
  text_layer_set_background_color(this->message_layer, GColorClear);

  layer_add_child(window_layer, this->background_layer);
  layer_add_child(this->background_layer, this->icon_layer);
  layer_add_child(this->background_layer, text_layer_get_layer(this->message_layer));

  // Set the loaded flag in the model
  this->loaded = true;

  // Update the UI
  prv_window_update_ui(this);
}

static void prv_window_unload(Window* window) {
  UIDialogWindow* this = window_get_user_data(window);

  // Destroy Layers
  layer_destroy(this->background_layer);
  layer_destroy(this->icon_layer);
  text_layer_destroy(this->message_layer);

  // Clean-up pointers
  this->background_layer = NULL;
  this->icon_layer = NULL;
  this->message_layer = NULL;

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

  this->window = window;
  this->message_text = message;
  this->icon_bitmap = icon;

  return this;
}

void ui_dialog_window_destroy(UIDialogWindow* this) {
  // Destroy Layers
  if (this->background_layer) layer_destroy(this->background_layer);
  if (this->icon_layer) layer_destroy(this->icon_layer);
  if (this->message_layer) text_layer_destroy(this->message_layer);

  // Destroy Window
  if (this->window) window_destroy(this->window);

  // Free memory
  if (this) free(this);

  // Cleanup stray pointers
  this->window = NULL;
  this->background_layer = NULL;
  this->icon_layer = NULL;
  this->message_layer = NULL;

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

void ui_dialog_window_set_icon(UIDialogWindow* this, GBitmap* icon) {
  this->icon_bitmap = icon;
  prv_window_update_ui(this);
}