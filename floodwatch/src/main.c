#include <pebble.h>
#include "modules/floodwatch.h"
#include "windows/message.h"

// CogniCity Floodwatch - Flood Alerts to Pebble Smart Watch from PetaJakarta.org
// (c) Hasitha Jayanandana, Tomas Holderness & Matthew Berryman 2016
// Released under GNU GPLv3 (see LICENSE.txt)

// Define the App elemments for the Main Window and Report window
Window *loading_window, *message_window;
static TextLayer *title_layer, *subtitle_layer;

// Constants for data transfer
static int INBOX_SIZE = 2220; //originally defined as const, needs fixing.
static int OUTBOX_SIZE = 1; //originally defined as const
static char message_buffer[50];

// loading_window_load
static void loading_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  title_layer = text_layer_create(
    GRect(0, (window_bounds.size.h/2)-28, window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
    text_layer_set_text(title_layer, "FloodWatch");
    text_layer_set_text_color(title_layer, GColorBlack);
    text_layer_set_background_color(title_layer, GColorClear);
    text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    layer_add_child(window_layer, text_layer_get_layer(title_layer));

  subtitle_layer = text_layer_create(
    GRect(0, (window_bounds.size.h)-35,window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(subtitle_layer, GTextAlignmentCenter);
    text_layer_set_text(subtitle_layer, "loading...");
    text_layer_set_text_color(subtitle_layer, GColorBlack);
    text_layer_set_background_color(subtitle_layer, GColorClear);
    text_layer_set_font(subtitle_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
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

  bool app_connection = connection_service_peek_pebble_app_connection();
  if (app_connection == 1){
     app_message_open(INBOX_SIZE, OUTBOX_SIZE);
  }
  else {
    snprintf(message_buffer, sizeof(message_buffer), "%s", "[Error] No bluetooth connection available");
    message_window = window_create();
    window_set_user_data(message_window, message_buffer);
    window_set_window_handlers(message_window, (WindowHandlers){
      .load = message_window_load,
      .unload = message_window_unload
    });
    window_stack_remove(loading_window, false);
    window_stack_push(message_window, true);
  }
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
