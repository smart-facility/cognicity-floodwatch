#pragma once

// Global reference for loading_window
extern Window *loading_window;

// Process incoming data
extern void inbox_received_callback(DictionaryIterator *iterator, void *context);
// Logging for incoming data errors
extern void inbox_dropped_callback(AppMessageResult reason, void *context);
// Logging for outgoing data handling
extern void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
// Logging for outgoing data handling
extern void outbox_sent_callback(DictionaryIterator *iterator, void *context);
// Load report window
static void report_window_load(Window *window);
// Unload report window
static void report_window_unload(Window *window);
// Load report window
void message_window_load(Window *window);
// Unload report window
void message_window_unload(Window *window);
