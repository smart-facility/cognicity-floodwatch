#include <pebble.h>
#include "modules/floodwatch.h"

// CogniCity Floodwatch - Flood Alerts to Pebble Smart Watch from PetaJakarta.org
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

// Define the App elemments for the Main Window and Report window
Window *loading_window;
static TextLayer *title_layer, *subtitle_layer;

// Constants for data transfer
static int INBOX_SIZE = 2220; //originally defined as const, needs fixing.
static int OUTBOX_SIZE = 1; //originally defined as const

// loading_window_load
static void loading_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  title_layer = text_layer_create(
    GRect(0, 0,window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
    text_layer_set_text(title_layer, "FloodWatch");
    text_layer_set_text_color(title_layer, GColorBlack);
    text_layer_set_background_color(title_layer, GColorClear);
    text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(title_layer));

  subtitle_layer = text_layer_create(
    GRect(0, (window_bounds.size.h/2)-15,window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(subtitle_layer, GTextAlignmentCenter);
    text_layer_set_text(subtitle_layer, "loading...");
    text_layer_set_text_color(subtitle_layer, GColorBlack);
    text_layer_set_background_color(subtitle_layer, GColorClear);
    text_layer_set_font(subtitle_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
    layer_add_child(window_layer, text_layer_get_layer(subtitle_layer));
}

// loading_window_unload
static void loading_window_unload(Window *window) {
  text_layer_destroy(title_layer);
  text_layer_destroy(subtitle_layer);
}

// Initialise windows
extern void init_windows(void) {

  loading_window = window_create();

  window_set_window_handlers(loading_window,(WindowHandlers){
    .load = loading_window_load,
    .unload = loading_window_unload
  });

  window_stack_push(loading_window, true);

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);

  app_message_open(INBOX_SIZE, OUTBOX_SIZE);
}

// Deinitialise windows
extern void deinit_windows(void) {
  window_destroy(loading_window);
}

// Main
int main(void) {
  init_windows();   // Start
  app_event_loop(); // App
  deinit_windows(); // Close
}
