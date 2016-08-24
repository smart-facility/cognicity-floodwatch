#include <pebble.h>
#include "windows/splash.h"

static Window *splash_window;
static TextLayer *title_layer, *subtitle_layer;

// splash_window_load
static void splash_window_load(Window *window) {

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

// splash_window_unload
static void splash_window_unload(Window *window) {
  text_layer_destroy(title_layer);
  text_layer_destroy(subtitle_layer);
}

void splash_window_push(){
  splash_window = window_create();

  window_set_window_handlers(splash_window,(WindowHandlers){
    .load = splash_window_load,
    .unload = splash_window_unload
  });

  window_stack_push(splash_window, true);
}

void splash_window_pull(){
  window_stack_remove(splash_window, false);
  window_destroy(splash_window);
}
