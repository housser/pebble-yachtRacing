#include "window_main.h"
#include <pebble.h>

// BEGIN AUTO-GENERATED UI CODE; DO NOT MODIFY
static Window *s_window;
static GFont s_res_font_avenir_50;
static GFont s_res_font_avenir_30;
static GFont s_res_bitham_34_medium_numbers;
static TextLayer *timer_layer;
static TextLayer *hour_layer;
static TextLayer *time_layer;

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorBlack);
  window_set_fullscreen(s_window, false);
  
  s_res_font_avenir_50 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVENIR_50));
  s_res_font_avenir_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVENIR_30));
  s_res_bitham_34_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  // timer_layer
  timer_layer = text_layer_create(GRect(0, 45, 144, 50));
  text_layer_set_background_color(timer_layer, GColorBlack);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_text(timer_layer, "-5:00");
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_font(timer_layer, s_res_font_avenir_50);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);
  
  // hour_layer
  hour_layer = text_layer_create(GRect(0, 10, 144, 30));
  text_layer_set_background_color(hour_layer, GColorBlack);
  text_layer_set_text_color(hour_layer, GColorWhite);
  text_layer_set_text(hour_layer, "1");
  text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
  text_layer_set_font(hour_layer, s_res_font_avenir_30);
  layer_add_child(window_get_root_layer(s_window), (Layer *)hour_layer);
  
  // time_layer
  time_layer = text_layer_create(GRect(0, 124, 144, 50));
  text_layer_set_text(time_layer, "15:34:56");
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_font(time_layer, s_res_bitham_34_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)time_layer);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(hour_layer);
  text_layer_destroy(time_layer);
  fonts_unload_custom_font(s_res_font_avenir_50);
  fonts_unload_custom_font(s_res_font_avenir_30);
}
// END AUTO-GENERATED UI CODE

static void handle_window_unload(Window* window) {
  destroy_ui();
}

void show_window_main(void) {
  initialise_ui();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
  });
  window_stack_push(s_window, true);
}

void hide_window_main(void) {
  window_stack_remove(s_window, true);
}
