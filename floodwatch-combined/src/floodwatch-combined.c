#include <pebble.h>
#include <string.h>
#include "libs/strtok.h"
#include "libs/strdup.h"

#define Num_menu_sections 2
#define Num_menu_icons 3
#define Num_first_menu_items 3
#define Num_second_menu_items 1

/*Define Key Values for dictionary to obtain variables from the javascript code*/

#define KEY_AREA 1
#define KEY_SOURCE 2
#define KEY_DESCRIPTION 3

// Define the App elemments for the Main Window and Report window
static Window *main_window, *report_window, *contact_window;
static TextLayer *title_layer, *region_layer;
static MenuLayer *menu_layer;
static TextLayer *s_weather_layer;
static TextLayer *contact_layer;

//Create variable to store the index of the currenly selected menu item
static int current_icon = 0;

//static void inbox_dropped_callback(AppMessageResult reason, void *context) {
//  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
//}

/*Initialize a structure to store the flood data values*/
typedef struct {
  char *area;
  char *source;
  char *description;
} Floodinfo;

static Floodinfo flood_info[256] = {};

// Create variable for weather layer
static char weather_layer_buffer[700];

// Integer variable to store the number of reports
static int floodcount = 0;

// Integer variables containing the maximum and minimum number of flood reports
static int min_token_count = 0;
static int max_token_count = 0;

extern void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
  static char area_buffer[1024];
  static char source_buffer[1024];
  static char description_buffer[1024];

  char *area_temp;
  char *source_temp;
  char *description_temp;
  const char delim[] = ","; /*Delimiter to seperate area and source string into seperate values*/
  const char desc_delim[] = "|"; /*Delimiter to seperate description string into seperate values*/
  int token_count = 0;


  // Read tuples for data
  Tuple *area_tuple = dict_find(iterator, KEY_AREA);
  Tuple *source_tuple = dict_find(iterator, KEY_SOURCE);
  Tuple *description_tuple = dict_find(iterator, KEY_DESCRIPTION);

  // If all data is available, use it
  if(area_tuple == NULL) {

    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "No Floods at the moment");
  }
  else{
    snprintf(area_buffer, sizeof(area_buffer), "%s", area_tuple->value->cstring);
    snprintf(source_buffer, sizeof(source_buffer), "%s", source_tuple->value->cstring);
    snprintf(description_buffer, sizeof(description_buffer), "%s", description_tuple->value->cstring);

    // Assemble full string and display
    snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:%s\nSource:%s\n%s\n",area_buffer,source_buffer,description_buffer);

    /* get the first token */
    area_temp = strtok(area_buffer, delim);

    /* walk through other tokens */
    while( area_temp != NULL )
    {
      flood_info[token_count].area= strdup(area_temp);
      token_count++;
      area_temp = strtok(NULL, delim);
    }
//reset token count back to 0
    token_count = 0;

    source_temp = strtok(source_buffer, delim);

    /* walk through other tokens */
    while( source_temp != NULL )
    {
      flood_info[token_count].source= strdup(source_temp);
      token_count++;
      source_temp = strtok(NULL, delim);
    }

    token_count = 0;
    description_temp = strtok(description_buffer, desc_delim);

    /* walk through other tokens */
    while( description_temp != NULL )
    {
      flood_info[token_count].description= strdup(description_temp);
      token_count++;
      description_temp = strtok(NULL, desc_delim);
    }

    max_token_count = token_count-1;
  }
}

extern void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
  return Num_menu_sections;
}

static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  switch (section_index){
    case 0:
    return Num_first_menu_items;
    case 1:
    return Num_second_menu_items;
    default:
    return 0;
  }
}

static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
  return MENU_CELL_BASIC_HEADER_HEIGHT;

}

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


    static void main_window_unload(Window *window) {
      text_layer_destroy(title_layer);
      text_layer_destroy(region_layer);
      menu_layer_destroy(menu_layer);
    }

    static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
      window_set_background_color(report_window, GColorRajah);

    }

    static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
      floodcount--;
      if (floodcount < min_token_count) floodcount = 0;
      snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
      text_layer_set_text(s_weather_layer, weather_layer_buffer);
    }

    static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
      floodcount++;
      if (floodcount > max_token_count) floodcount = max_token_count;
      snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), "Where?:\n%s\nSource:%s\n%s\n",flood_info[floodcount].area,flood_info[floodcount].source,flood_info[floodcount].description);;
      text_layer_set_text(s_weather_layer, weather_layer_buffer);
    }

    static void click_config_provider(void *context) {
      window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
      window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
      window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
    }

    static void report_window_load(Window *window) {

      window_set_click_config_provider(window, (ClickConfigProvider) click_config_provider);
      // Get information about the Window
      Layer *window_layer = window_get_root_layer(window);
      GRect bounds = layer_get_bounds(window_layer);


      s_weather_layer = text_layer_create(
        GRect(0, 0, bounds.size.w, 600));

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

      static void report_window_unload(Window *window) {
        layer_remove_child_layers(window_get_root_layer(window));
        text_layer_destroy(s_weather_layer);
      }

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



        static void init(void) {

          main_window = window_create();
          window_set_background_color(main_window, GColorGreen);

          window_set_window_handlers(main_window,(WindowHandlers){
            .load = main_window_load,
            .unload = main_window_unload});


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

            const int inbox_size = 256;
            const int outbox_size = 256;
            app_message_open(inbox_size, outbox_size);


          }

          static void deinit(void) {
            window_destroy(contact_window);
            window_destroy(report_window);
            window_destroy(main_window);

          }

          int main(void) {
            init();
            app_event_loop();
            deinit();
          }
