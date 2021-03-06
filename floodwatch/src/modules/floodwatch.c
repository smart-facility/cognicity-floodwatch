#include <pebble.h>
#include <string.h>
#include "libs/strdup.h"
#include "libs/strtok.h"
#include "modules/floodwatch.h"
#include "windows/message.h"
#include "windows/splash.h"

// Initialize a structure to store the flood data values
typedef struct {
  char *pkey;
  char *distance;
  char *time;
  char *description;
} Floodinfo;

static Floodinfo flood_info[10] = {};

static long data_length;
static char description_buffer[1609];

// UI Elements
static Window *listing_window, *report_window;
static MenuLayer *menu_layer;
static TextLayer *report_text_layer, *footer_text_layer, *time_text_layer, *distance_text_layer;

// Define Key Values for dictionary to obtain variables from the javascript code
#define KEY_PKEY 1
#define KEY_DISTANCE 2
#define KEY_TIME 3
#define KEY_DESCRIPTION 4
#define KEY_DATA_LENGTH 5

// Create variable to store the index of the currenly selected menu item
static int current_report = 0;

// Up click handler
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_report > 0){
    current_report -= 1;
    window_stack_remove(report_window, true);
    window_destroy(report_window);
    report_window = window_create();
    window_set_window_handlers(report_window,(WindowHandlers){
      .load = report_window_load,
      .unload = report_window_unload
    });
    window_stack_push(report_window, true);
  }
}

// Down click handler
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (current_report < data_length-1){
    current_report += 1;
    window_stack_remove(report_window, true);
    window_destroy(report_window);
    report_window = window_create();
    window_set_window_handlers(report_window,(WindowHandlers){
      .load = report_window_load,
      .unload = report_window_unload
    });
    window_stack_push(report_window, true);
  }
}

// Down click handler
static void back_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Set selected item
  MenuIndex menu_index = {.section = 1, .row = current_report};
  menu_layer_set_selected_index(menu_layer, menu_index, MenuRowAlignCenter, false);
  window_stack_remove(report_window, true);
  window_destroy(report_window);
  window_stack_push(listing_window, true);
}


// Click configuration (currently disabled)
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_BACK, back_click_handler);
}

// Callback for number of menu sections
extern uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  //return Num_menu_sections;
  const uint16_t num_sections = 1;
  return num_sections;
}

// Callback for number of menu rows
extern uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){

  return data_length;
}

// Callback for menu header height
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return MENU_CELL_BASIC_HEADER_HEIGHT;
}

// Draw header
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data){
      menu_cell_basic_header_draw(ctx, cell_layer, "Nearby Flood Reports");
}

// Draw row
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {

  static char dist_buffer[16];
  static char desc_buffer[25];
  snprintf(dist_buffer, sizeof(dist_buffer), "%s · %s km", flood_info[cell_index[0].row].time, flood_info[cell_index[0].row].distance);
  snprintf(desc_buffer, sizeof(desc_buffer), "%s", flood_info[cell_index[0].row].description);

  menu_cell_basic_draw(ctx, cell_layer, dist_buffer, desc_buffer, NULL);
  //}
};

// Load report window
static void report_window_load(Window *window) {

  static char page_buffer[6]; //10/10
  static char dist_buffer[8]; //5.0 km
  static char time_buffer[6]; // hh:mm
  static char desc_buffer[160];
  snprintf(page_buffer, sizeof(page_buffer), "%u/%lu", current_report+1, data_length);
  snprintf(dist_buffer, sizeof(dist_buffer), "%s km", flood_info[current_report].distance);
  snprintf(time_buffer, sizeof(time_buffer), "%s", flood_info[current_report].time);
  snprintf(desc_buffer, sizeof(desc_buffer), "%s", flood_info[current_report].description);

  // Click functionality to scroll through reports
  window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  distance_text_layer = text_layer_create(
    GRect(0, 0, window_bounds.size.w, 22));

  text_layer_set_text_alignment(distance_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(distance_text_layer, GColorWhite);
  text_layer_set_background_color(distance_text_layer, GColorBlack);
  text_layer_set_font(distance_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(distance_text_layer, GTextAlignmentRight);
  text_layer_set_text(distance_text_layer, dist_buffer);

  layer_add_child(window_layer, text_layer_get_layer(distance_text_layer));

  time_text_layer = text_layer_create(
    GRect(0, 0, window_bounds.size.w-45, 22));

  text_layer_set_text_alignment(time_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(time_text_layer, GColorWhite);
  text_layer_set_background_color(time_text_layer, GColorBlack);
  text_layer_set_font(time_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(time_text_layer, GTextAlignmentLeft);
  text_layer_set_text(time_text_layer, time_buffer);

  layer_add_child(window_layer, text_layer_get_layer(time_text_layer));

  report_text_layer = text_layer_create(
    GRect(0, 22, window_bounds.size.w, window_bounds.size.h-20));

  text_layer_set_text_alignment(report_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(report_text_layer, GColorWhite);
  text_layer_set_background_color(report_text_layer, GColorBlack);
  text_layer_set_font(report_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(report_text_layer, GTextAlignmentLeft);
  text_layer_set_text(report_text_layer, desc_buffer);

  layer_add_child(window_layer, text_layer_get_layer(report_text_layer));

  footer_text_layer = text_layer_create(
    GRect(0,window_bounds.size.h-20,window_bounds.size.w, 20));

    text_layer_set_text_alignment(footer_text_layer, GTextAlignmentCenter);
    text_layer_set_text_color(footer_text_layer, GColorWhite);
    text_layer_set_background_color(footer_text_layer, GColorBlack);
    text_layer_set_font(footer_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(footer_text_layer, GTextAlignmentRight);
    text_layer_set_text(footer_text_layer, page_buffer);

    layer_add_child(window_layer, text_layer_get_layer(footer_text_layer));
}

// Unload report window
static void report_window_unload(Window *window) {
  layer_remove_child_layers(window_get_root_layer(window));
  text_layer_destroy(report_text_layer);
}

// Select click
static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context) {
  current_report = cell_index[0].row;
  report_window = window_create();
  window_set_window_handlers(report_window,(WindowHandlers){
    .load = report_window_load,
    .unload = report_window_unload
  });
  window_stack_push(report_window, true);
}

// Window listing flood reports
static void listing_window_load (Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  menu_layer = menu_layer_create(GRect(0, 0,window_bounds.size.w, window_bounds.size.h));
  //menu_layer_set_normal_colors(MenuLayer * menu_layer, GColorPictonBlue, GColorBlack);
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks){
    .get_num_sections = menu_get_num_sections_callback,
    .get_num_rows = menu_get_num_rows_callback,
    .get_header_height = PBL_IF_RECT_ELSE(menu_get_header_height_callback, NULL),
    .draw_header = PBL_IF_RECT_ELSE(menu_draw_header_callback, NULL),
    .draw_row = menu_draw_row_callback,
    .select_click = select_click
  });

  // Bind the menu layer's click config provider to the window for interactivity
  menu_layer_set_click_config_onto_window(menu_layer, window);

  layer_add_child(window_layer, menu_layer_get_layer(menu_layer));
}

// Unload report window
static void listing_window_unload(Window *window) {
  menu_layer_destroy(menu_layer);
}

// Process incoming data
extern void inbox_received_callback(DictionaryIterator *iterator, void *context) {

  static char data_length_buffer[3];
  Tuple *data_length_tuple = dict_find(iterator, KEY_DATA_LENGTH);
  snprintf(data_length_buffer, sizeof(data_length_buffer), "%s", data_length_tuple->value->cstring);

  data_length = atoi(data_length_buffer);

  Tuple *description_tuple = dict_find(iterator, KEY_DESCRIPTION);
  snprintf(description_buffer, sizeof(description_buffer), "%s", description_tuple->value->cstring);

  if (data_length == 0) {
    splash_window_pull();
    message_window_push(description_buffer);
  }

  else if (data_length > 0) {
    // Store incoming information
    static char pkey_buffer[209]; // *TODO* Needs adjusting
    static char time_buffer[69];
    static char distance_buffer[40];

    char *pkey_vals;
    char *time_vals;
    char *distance_vals;
    char *description_vals;

    const char delim[] = ","; /*Delimiter to seperate area and source string into seperate values*/
    const char desc_delim[] = "|"; /*Delimiter to seperate description string into seperate values*/

    int token_count = 0; // counter for array variables

    // Read tuples for data
    Tuple *pkey_tuple = dict_find(iterator, KEY_PKEY);
    Tuple *time_tuple = dict_find(iterator, KEY_TIME);
    Tuple *distance_tuple = dict_find(iterator, KEY_DISTANCE);

    snprintf(pkey_buffer, sizeof(pkey_buffer), "%s", pkey_tuple->value->cstring);
    snprintf(time_buffer, sizeof(time_buffer), "%s", time_tuple->value->cstring);
    snprintf(distance_buffer, sizeof(distance_buffer), "%s", distance_tuple->value->cstring);

    /* get the first token */
    time_vals = strtok(time_buffer, delim);

    /* walk through other tokens */
    while( time_vals != NULL )
    {
      flood_info[token_count].time = strdup(time_vals);
      token_count++;
      time_vals = strtok(NULL, delim);
    }

    //reset token count back to 0
    token_count = 0;

    /* get the first token */
    pkey_vals = strtok(pkey_buffer, delim);

    /* walk through other tokens */
    while( pkey_vals != NULL )
    {
      flood_info[token_count].pkey = strdup(pkey_vals);
      token_count++;
      pkey_vals = strtok(NULL, delim);
    }

    //reset token count back to 0
    token_count = 0;

    distance_vals = strtok(distance_buffer, delim);

    /* walk through other tokens */
    while( distance_vals != NULL )
    {
      flood_info[token_count].distance = strdup(distance_vals);
      token_count++;
      distance_vals = strtok(NULL, delim);
    }

    token_count = 0;
    description_vals = strtok(description_buffer, desc_delim);

    /* walk through other tokens */
    while( description_vals != NULL )
    {
      flood_info[token_count].description = strdup(description_vals);
      token_count++;
      description_vals = strtok(NULL, desc_delim);
    }

    listing_window = window_create();
    window_set_window_handlers(listing_window, (WindowHandlers){
        .load = listing_window_load,
        .unload = listing_window_unload
    });

    splash_window_pull();
    window_stack_push(listing_window, true);
  }
}

// Logging for incoming data handling
extern void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

// Logging for outgoing data handling
extern void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

// Logging for outgoing data handling
extern void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}
