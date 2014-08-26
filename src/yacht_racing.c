#include <pebble.h>
#include <time.h>

Window *my_window;

TextLayer *timer_layer;
TextLayer *hour_layer;
bool started = false;

#define TIMER_STR_BUFFER_BYTES 32
char timer_str_buffer[TIMER_STR_BUFFER_BYTES];
char timer_hour_str_buffer[TIMER_STR_BUFFER_BYTES];

int timer_counter = -5 * 60;
int hours, minutes, seconds;


TextLayer *time_layer;
#define TIME_STR_BUFFER_BYTES 32
char time_str_buffer[TIME_STR_BUFFER_BYTES];


int abs(int v) 
{
	return v * ((v>0) - (v<0));
}

void parse_time() {
	hours = (timer_counter / 60 / 60) % 24;
	minutes = (timer_counter / 60) % 60;
	seconds = timer_counter % 60;
}

void update_countdown_display() {
	snprintf(
			 timer_str_buffer, 
			 TIMER_STR_BUFFER_BYTES, 
			 "-%d:%02d",
			 abs(minutes), 
			 abs(seconds)
			 );
	text_layer_set_text(timer_layer, timer_str_buffer);
}


static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
	if(started) {
		timer_counter++;
		
		parse_time();
		
		if(timer_counter == 0) {
			vibes_long_pulse();
			update_countdown_display();
		}
		
		if(timer_counter < 0) { // in start sequence
			if (timer_counter >= -10 || seconds == -30) {  // last 10 seconds
				vibes_short_pulse();
			} else if(
			   seconds == 0 // every minutes
			   || (minutes == 0 && seconds % 10 == 0) // every 10 seconds in the last minute
			   ) {
				vibes_double_pulse();
			}
			
			update_countdown_display();
		} else {
			
			snprintf(
					 timer_str_buffer, 
					 TIMER_STR_BUFFER_BYTES, 
					 "%02d:%02d",
					 abs(minutes), 
					 abs(seconds)
					 );	
			
			text_layer_set_text(timer_layer, timer_str_buffer);
			
			
			snprintf(
					 timer_hour_str_buffer, 
					 TIMER_STR_BUFFER_BYTES, 
					 "%02d",
					 abs(hours)
					 );	
			
			text_layer_set_text(hour_layer, timer_hour_str_buffer);
		}
	}
	
  strftime(time_str_buffer, TIME_STR_BUFFER_BYTES, "%H:%M:%S", tick_time);
	text_layer_set_text(time_layer, time_str_buffer);
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	started = ! started;
	vibes_long_pulse();
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	if (timer_counter < 0) {
		parse_time();

		if(started) {
			//minutes--;
			vibes_long_pulse();
		}
		
		seconds = 0;
		minutes--;
		timer_counter = minutes * 60;
		update_countdown_display();
	}
}

void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {	
	if (timer_counter < -60 && minutes < 0) {
		parse_time();
		seconds = 0;
		if ( !started && seconds == 0) {
			minutes++;
		}
		timer_counter = minutes * 60;
		update_countdown_display();
		
		if ( started ) {
			vibes_long_pulse();
		}
	}
}

void click_config_provider(Window *my_window) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
}

void handle_init(void) {
	my_window = window_create();
  
	window_set_fullscreen(my_window, true);
	window_set_background_color(my_window, GColorBlack);
	window_stack_push(my_window, true /* Animated */);
  
  Layer *my_window_layer = window_get_root_layer(my_window);
  /* GRect bounds = layer_get_frame(my_window_layer); */
	
	parse_time();
	
	GFont font_avenir_30 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVENIR_30));
	GFont font_avenir_50 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVENIR_50));
	
	hour_layer = text_layer_create(GRect(0, 10, 144, 30));
	text_layer_set_background_color(hour_layer, GColorBlack);
	text_layer_set_text_color(hour_layer, GColorWhite);
	text_layer_set_font(hour_layer, font_avenir_30);
	text_layer_set_text_alignment(hour_layer, GTextAlignmentCenter);
	text_layer_set_text(hour_layer, "");
	layer_add_child(my_window_layer, text_layer_get_layer(hour_layer));
	
	
	timer_layer = text_layer_create(GRect(0, 45, 144, 50));
	text_layer_set_background_color(timer_layer, GColorBlack);
	text_layer_set_text_color(timer_layer, GColorWhite);
	text_layer_set_font(timer_layer, font_avenir_50);
	text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
	text_layer_set_text(timer_layer, "-5:00");
	layer_add_child(my_window_layer, text_layer_get_layer(timer_layer));
	
	
	// 168 h
	time_layer = text_layer_create(GRect(0, 124, 144, 50));
	text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

	text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	strcpy(time_str_buffer, "");
	text_layer_set_text(time_layer, time_str_buffer);
	layer_add_child(my_window_layer, text_layer_get_layer(time_layer));
	
	// Attach our desired button functionality
	window_set_click_config_provider(my_window, (ClickConfigProvider) click_config_provider);
  
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
}

void handle_deinit(void) {
  text_layer_destroy(timer_layer);
  text_layer_destroy(hour_layer);
  text_layer_destroy(time_layer);
  window_destroy(my_window);
  tick_timer_service_unsubscribe();
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}