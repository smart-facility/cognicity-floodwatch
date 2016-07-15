#include <pebble.h>
#include <string.h>
#include "libs/strdup.h"
#include "modules/data.h"

// Process incoming data
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
