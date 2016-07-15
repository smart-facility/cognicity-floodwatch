#pragma once
/*
extern void inbox_dropped_callback(AppMessageResult reason, void *context);
*/

//Initialize a structure to store the flood data values
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

// Define Key Values for dictionary to obtain variables from the javascript code
#define KEY_AREA 1
#define KEY_SOURCE 2
#define KEY_DESCRIPTION 3

// Process incoming data
extern void inbox_received_callback(DictionaryIterator *iterator, void *context);
// Logging for incoming data errors
extern void inbox_dropped_callback(AppMessageResult reason, void *context);
// Logging for outgoing data handling
extern void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
// Logging for outgoing data handling
extern void outbox_sent_callback(DictionaryIterator *iterator, void *context);
