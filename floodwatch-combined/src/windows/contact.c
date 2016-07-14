#include <pebble.h>
#include "windows/contact.h"
/*
static void contact_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  contact_layer = text_layer_create(
    GRect(0, 20, bounds.size.w, bounds.size.h));

  // Style the text
  text_layer_set_background_color(contact_layer, GColorClear);
  text_layer_set_text_color(contact_layer, GColorWhite);
  text_layer_set_text_alignment(contact_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(contact_layer, GTextOverflowModeWordWrap);
  text_layer_set_text(contact_layer, "Emergency Call\n000");
  text_layer_set_font(contact_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));

  // Create flood Layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(contact_layer));
  window_set_background_color(contact_window, GColorBlack);
}

static void contact_window_unload(Window *window) {
  layer_remove_child_layers(window_get_root_layer(window));

  // Destroy weather elements
  text_layer_destroy(contact_layer);
}*/