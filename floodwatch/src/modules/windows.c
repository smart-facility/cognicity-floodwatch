#include <pebble.h>
#include "modules/data.h"
#include "modules/windows.h"


// Callback for number of menu sections
extern uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  return Num_menu_sections;
}

// Callback for number of menu rows
extern uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  switch (section_index){
    case 0:
    return Num_first_menu_items;
    case 1:
    return Num_second_menu_items;
    default:
    return 0;
  }
}

// Callback for menu header height
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Draw header
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data){
  switch (section_index){
    case 0:
      menu_cell_basic_header_draw(ctx, cell_layer, "#FloodWatch");
      break;
    case 1:
      menu_cell_basic_header_draw(ctx, cell_layer, "Configure Area on Companion App");
      break;
  }
}

// Draw row
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
  switch (cell_index ->section){
    case 0 :
    // use row to specify hich item to draw
    switch (cell_index->row){
      case 0:
        menu_cell_basic_draw(ctx, cell_layer, "Floods In Area","",NULL);
        break;
      case 1:
        menu_cell_basic_draw(ctx, cell_layer, "Emergency Contacts", "SOS", NULL);
        break;
      case 2:
        menu_cell_basic_draw(ctx, cell_layer, "Configure App", "Select", NULL);
        break;
    }
  }
}

// Select click
static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  current_icon = cell_index->row;
  switch (cell_index->row){
    case 0:
      vibes_short_pulse();
      window_stack_push(report_window, true);
      break;
    case 1:
      vibes_short_pulse();
      window_stack_push(contact_window, true);
      break;
    case 2:
      vibes_short_pulse();
      break;
  }
}

// Pebble round support
#ifdef PBL_ROUND
static int16_t get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  if (menu_layer_is_index_selected(menu_layer, cell_index)) {
    switch (cell_index->row) {
      case 0:
      return MENU_CELL_ROUND_FOCUSED_SHORT_CELL_HEIGHT;
      break;
      default:
      return MENU_CELL_ROUND_FOCUSED_TALL_CELL_HEIGHT;
    }
  } else {
    return MENU_CELL_ROUND_UNFOCUSED_SHORT_CELL_HEIGHT;
  }
}
#endif

// Select click handler
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  window_set_background_color(report_window, GColorRajah);
}

// Up click handler
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  floodcount--;
  if (floodcount < min_token_count) floodcount = 0;
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

// Down click handler
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  floodcount++;
  if (floodcount > max_token_count) floodcount = max_token_count;
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

// Click configuration
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Load report window
static void report_window_load(Window *window) {
  window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  s_weather_layer = text_layer_create(
    GRect(0, 0, bounds.size.w, 600)
  );

  // Style the text
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorBlack);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentCenter);
  text_layer_set_overflow_mode(s_weather_layer, GTextOverflowModeWordWrap);
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
  //text_layer_set_text(s_weather_layer, "Loading...");
  text_layer_set_font(s_weather_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));

  // Create flood Layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
}

// Unload report window
static void report_window_unload(Window *window) {
  layer_remove_child_layers(window_get_root_layer(window));
  text_layer_destroy(s_weather_layer);
}

// Load contact window
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
}

// Unload main window
static void main_window_unload(Window *window) {
  text_layer_destroy(title_layer);
  text_layer_destroy(region_layer);
  menu_layer_destroy(menu_layer);
}

// Load main window
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
}

// Initialise windows
extern void init_windows(void) {

  main_window = window_create();
  window_set_background_color(main_window, GColorGreen);

  window_set_window_handlers(main_window,(WindowHandlers){
    .load = main_window_load,
    .unload = main_window_unload
  });

  report_window = window_create();
  window_set_window_handlers(report_window, (WindowHandlers) {
    .load = report_window_load,
    .unload = report_window_unload
    });

  contact_window = window_create();
  window_set_window_handlers(contact_window, (WindowHandlers) {
    .load = contact_window_load,
    .unload = contact_window_unload
  });

  window_stack_push(main_window, true);

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  app_message_open(INBOX_SIZE, OUTBOX_SIZE);
  }

// Deinitialise windows
extern void deinit_windows(void) {
  window_destroy(contact_window);
  window_destroy(report_window);
  window_destroy(main_window);
}
