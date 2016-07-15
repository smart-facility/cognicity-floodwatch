#pragma once

// UI constants
#define Num_menu_sections 2
#define Num_menu_icons 3
#define Num_first_menu_items 3
#define Num_second_menu_items 1

// Define the App elemments for the Main Window and Report window
static Window *main_window, *report_window, *contact_window;
static TextLayer *title_layer, *region_layer;
static MenuLayer *menu_layer;
static TextLayer *s_weather_layer;
static TextLayer *contact_layer;

// Create variable to store the index of the currenly selected menu item
static int current_icon = 0;

// Constants for data transfer
static int INBOX_SIZE = 256; //originally defined as const
static int OUTBOX_SIZE = 256; //originally defined as const

// Callback for number of menu sections
static uint16_t menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);

// Callback for number of menu rows
static uint16_t menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

// Callback for menu header height
static int16_t menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

// Draw header
static void menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);

// Draw row
static void menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);

// Select click
static void select_click(struct MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);

// Get cell height
static int16_t get_cell_height_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context);

// Select click handler
static void select_click_handler(ClickRecognizerRef recognizer, void *context);

// Up click handler
static void up_click_handler(ClickRecognizerRef recognizer, void *context);

// Down click handler
static void down_click_handler(ClickRecognizerRef recognizer, void *context);

// Click configuration
static void click_config_provider(void *context);

// Report window
static void report_window_load(Window *window);

// Report window unload
static void report_window_unload(Window *window);

// Contact window load
static void contact_window_load(Window *window);

// Contact window unload
static void contact_window_unload(Window *window);

// Main window unload
static void main_window_unload(Window *window);

// Main window
static void main_window_load(Window *window);

// Initialise windows
extern void init_windows(void);

// Deinitialise windows
extern void deinit_windows(void);
