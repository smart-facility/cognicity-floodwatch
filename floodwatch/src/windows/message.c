#include <pebble.h>
#include "./windows/message.h"

static TextLayer *message_text_layer;

// Load report window, expects a message (string buffer) to be attached via window_set_user_data()
void message_window_load(Window *window) {

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  message_text_layer = text_layer_create(
    GRect(0, (window_bounds.size.h/2)-20, window_bounds.size.w, window_bounds.size.h));

  text_layer_set_text_alignment(message_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(message_text_layer, GColorBlack);
  text_layer_set_background_color(message_text_layer, GColorWhite);
  text_layer_set_font(message_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(message_text_layer, GTextAlignmentCenter);
  text_layer_set_text(message_text_layer, window_get_user_data(window));

  layer_add_child(window_layer, text_layer_get_layer(message_text_layer));
}

// Unload report window
void message_window_unload(Window *window) {
  layer_remove_child_layers(window_get_root_layer(window));
  text_layer_destroy(message_text_layer);
}
