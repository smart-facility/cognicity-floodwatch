#include <pebble.h>
#include <string.h>
#include "libs/strdup.h"
#include "libs/strtok.h"
#include "modules/floodwatch.h"

// Initialize a structure to store the flood data values
typedef struct {
  char *distance;
  char *time;
  char *description;
} Floodinfo;

static Floodinfo flood_info[256] = {};

// UI Elements
static Window *listing_window, *report_window;
static MenuLayer *menu_layer;
static TextLayer *report_text_layer;

// Create variable for weather layer
static char weather_layer_buffer[700];

// Integer variable to store the number of reports
static int floodcount = 0;

// Integer variables containing the maximum and minimum number of flood reports
static int min_token_count = 0;
static int max_token_count = 0;

// Define Key Values for dictionary to obtain variables from the javascript code
#define KEY_PKEY 1
#define KEY_DISTANCE 2
#define KEY_TIME 3
#define KEY_DESCRIPTION 4

// Create variable to store the index of the currenly selected menu item
static int current_report = 0;

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
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
  //text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

// Down click handler
static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  floodcount++;
  if (floodcount > max_token_count) floodcount = max_token_count;
  //snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
  //text_layer_set_text(s_weather_layer, weather_layer_buffer);
}

// Click configuration
static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Callback for number of menu sections
extern uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  //return Num_menu_sections;
  const uint16_t num_sections = 1;
  return num_sections;
}

// Callback for number of menu rows
extern uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  const uint16_t num_rows = 4;
  return num_rows;
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

  static char dist_buffer[15];
  static char desc_buffer[100];
  snprintf(dist_buffer, sizeof(dist_buffer), "%s %s km", flood_info[cell_index[0].row].time, flood_info[cell_index[0].row].distance);
  snprintf(desc_buffer, sizeof(desc_buffer), "%s", flood_info[cell_index[0].row].description);

  menu_cell_basic_draw(ctx, cell_layer, dist_buffer, desc_buffer, NULL);
  //}
};

// Load report window
static void report_window_load(Window *window) {
  window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);

  static char dist_buffer[10];
  static char time_buffer[10];
  static char desc_buffer[100];
  static char buffer[110];
  snprintf(dist_buffer, sizeof(dist_buffer), "%s", flood_info[current_report].distance);
  snprintf(time_buffer, sizeof(time_buffer), "%s", flood_info[current_report].time);
  snprintf(desc_buffer, sizeof(desc_buffer), "%s", flood_info[current_report].description);
  snprintf(buffer, sizeof(buffer), "%s km\n%s\n%s", dist_buffer, time_buffer, desc_buffer);

  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  report_text_layer = text_layer_create(
    GRect(0, 0,window_bounds.size.w, window_bounds.size.h));

  text_layer_set_text_alignment(report_text_layer, GTextAlignmentCenter);
  text_layer_set_text_color(report_text_layer, GColorBlack);
  text_layer_set_background_color(report_text_layer, GColorClear);
  text_layer_set_font(report_text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text_alignment(report_text_layer, GTextAlignmentLeft);
  text_layer_set_text(report_text_layer, buffer);

  layer_add_child(window_layer, text_layer_get_layer(report_text_layer));
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
    .select_click = select_click,
    .get_cell_height = PBL_IF_ROUND_ELSE(get_cell_height_callback, NULL),
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

  // Store incoming information
  static char time_buffer[1024];
  static char distance_buffer[1024];
  static char description_buffer[1024];

  char *time_temp;
  char *distance_temp;
  char *description_temp;
  const char delim[] = ","; /*Delimiter to seperate area and source string into seperate values*/
  const char desc_delim[] = "|"; /*Delimiter to seperate description string into seperate values*/
  int token_count = 0;

  // Read tuples for data
  Tuple *time_tuple = dict_find(iterator, KEY_TIME);
  Tuple *distance_tuple = dict_find(iterator, KEY_DISTANCE);
  Tuple *description_tuple = dict_find(iterator, KEY_DESCRIPTION);

  // If all data is available, use it
  if(description_tuple == NULL) {
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "No Floods at the moment");
  }
  else{
    snprintf(time_buffer, sizeof(time_buffer), "%s", time_tuple->value->cstring);
    snprintf(distance_buffer, sizeof(distance_buffer), "%s", distance_tuple->value->cstring);
    snprintf(description_buffer, sizeof(description_buffer), "%s", description_tuple->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:%s\nSource:%s\n%s\n",time_buffer,distance_buffer,description_buffer);
    APP_LOG(APP_LOG_LEVEL_INFO, time_buffer);

    /* get the first token */
    time_temp = strtok(time_buffer, delim);

    /* walk through other tokens */
    while( time_temp != NULL )
    {
      flood_info[token_count].time = strdup(time_temp);
      token_count++;
      time_temp = strtok(NULL, delim);
    }

    //reset token count back to 0
    token_count = 0;

    distance_temp = strtok(distance_buffer, delim);

    /* walk through other tokens */
    while( distance_temp != NULL )
    {
      flood_info[token_count].distance = strdup(distance_temp);
      token_count++;
      distance_temp = strtok(NULL, delim);
    }

    token_count = 0;
    description_temp = strtok(description_buffer, desc_delim);

    /* walk through other tokens */
    while( description_temp != NULL )
    {
      flood_info[token_count].description = strdup(description_temp);
      token_count++;
      description_temp = strtok(NULL, desc_delim);
    }

    listing_window = window_create();
    window_set_window_handlers(listing_window, (WindowHandlers){
        .load = listing_window_load,
        .unload = listing_window_unload
    });
    window_stack_remove(loading_window, false);
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
