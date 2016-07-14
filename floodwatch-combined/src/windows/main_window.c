#include <pebble.h>
#include "windows/main_window.h"
/*
static Window *main_window;

static void main_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  title_layer = text_layer_create(
    GRect(0, 0,window_bounds.size.w, window_bounds.size.h));
    text_layer_set_text_alignment(title_layer, GTextAlignmentCenter);
    text_layer_set_text(title_layer, "FloodWatch");
    text_layer_set_text_color(title_layer, GColorBlack);
    text_layer_set_background_color(title_layer, GColorClear);
    text_layer_set_font(title_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    layer_add_child(window_layer, text_layer_get_layer(title_layer));

    region_layer = text_layer_create(
      GRect(0, 20,window_bounds.size.w, window_bounds.size.h));
      text_layer_set_text_alignment(region_layer, GTextAlignmentCenter);
      text_layer_set_text(region_layer, "PetaJakarta.org");
      text_layer_set_text_color(region_layer, GColorWhite);
      text_layer_set_background_color(region_layer, GColorClear);
      text_layer_set_font(region_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
      layer_add_child(window_layer, text_layer_get_layer(region_layer));


      menu_layer = menu_layer_create(GRect(0, 50,window_bounds.size.w, window_bounds.size.h));
      //menu_layer_set_normal_colors(MenuLayer * menu_layer, GColorPictonBlue, GColorBlack);
      menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
        .get_num_sections = menu_get_num_sections_callback,
        .get_num_rows = menu_get_num_rows_callback,
        .get_header_height = PBL_IF_RECT_ELSE(menu_get_header_height_callback, NULL),
        .draw_header = PBL_IF_RECT_ELSE(menu_draw_header_callback, NULL),
        .draw_row = menu_draw_row_callback,
        .select_click = select_click,
        .get_cell_height = PBL_IF_ROUND_ELSE(get_cell_height_callback, NULL),
      });

      // Bind the menu layer's click config provider to the window for interactivity
      menu_layer_set_click_config_onto_window(menu_layer, window);

      layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
    }*/