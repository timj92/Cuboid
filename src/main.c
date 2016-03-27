#include <pebble.h>
#include <stdbool.h>

static GBitmap *num0;
static GBitmap *num1left;
static GBitmap *num1right;
static GBitmap *num2;
static GBitmap *num3;
static GBitmap *num4;
static GBitmap *num5;
static GBitmap *num6;
static GBitmap *num7;
static GBitmap *num8;
static GBitmap *num9;

static BitmapLayer *topleft_image;
static BitmapLayer *topright_image;
static BitmapLayer *bottomleft_image;
static BitmapLayer *bottomright_image;

static Window *s_main_window;

static void center() {
  if(bitmap_layer_get_bitmap(topleft_image) == num1right && bitmap_layer_get_bitmap(topright_image) != num1left) {
    // top row left alignment
    layer_set_frame(bitmap_layer_get_layer(topleft_image), GRect(0, 5, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(topright_image), GRect(64, 5, 64, 76));
  }
  else if(bitmap_layer_get_bitmap(topleft_image) != num1right && bitmap_layer_get_bitmap(topright_image) == num1left) {
    // top row right alignment
    layer_set_frame(bitmap_layer_get_layer(topleft_image), GRect(14, 5, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(topright_image), GRect(78, 5, 64, 76));
  } else {
    // top row default alignment
    layer_set_frame(bitmap_layer_get_layer(topleft_image), GRect(7, 5, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(topright_image), GRect(71, 5, 64, 76));
  }
  
  if(bitmap_layer_get_bitmap(bottomleft_image) == num1right && bitmap_layer_get_bitmap(bottomright_image) != num1left) {
    // bottom row left alignment
    layer_set_frame(bitmap_layer_get_layer(bottomleft_image), GRect(0, 86, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(bottomright_image), GRect(64, 86, 64, 76));
  }
  else if(bitmap_layer_get_bitmap(bottomleft_image) != num1right && bitmap_layer_get_bitmap(bottomright_image) == num1left) {
    // bottom row right alignment
    layer_set_frame(bitmap_layer_get_layer(bottomleft_image), GRect(14, 86, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(bottomright_image), GRect(78, 86, 64, 76));
  } else {
    // bottom row default alignment
    layer_set_frame(bitmap_layer_get_layer(bottomleft_image), GRect(7, 86, 64, 76));
    layer_set_frame(bitmap_layer_get_layer(bottomright_image), GRect(71, 86, 64, 76));
  }
}

static void set_digit(BitmapLayer *bitmap_layer, char digit, bool isLeftColumn) {
  switch(digit) {
    case '1':
      if(isLeftColumn) {
        bitmap_layer_set_bitmap(bitmap_layer, num1right);
      } else {
        bitmap_layer_set_bitmap(bitmap_layer, num1left);
      }
      break;
    case '2':
      bitmap_layer_set_bitmap(bitmap_layer, num2);
      break;
    case '3':
      bitmap_layer_set_bitmap(bitmap_layer, num3);
      break;
    case '4':
      bitmap_layer_set_bitmap(bitmap_layer, num4);
      break;
    case '5':
      bitmap_layer_set_bitmap(bitmap_layer, num5);
      break;
    case '6':
      bitmap_layer_set_bitmap(bitmap_layer, num6);
      break;
    case '7':
      bitmap_layer_set_bitmap(bitmap_layer, num7);
      break;
    case '8':
      bitmap_layer_set_bitmap(bitmap_layer, num8);
      break;
    case '9':
      bitmap_layer_set_bitmap(bitmap_layer, num9);
      break;
    default:
      bitmap_layer_set_bitmap(bitmap_layer, num0);
      break;
  }
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H%M" : "%I%M", tick_time);
  
  // Display this time on screen
  set_digit(topleft_image, s_buffer[0], true);
  set_digit(topright_image, s_buffer[1], false);
  set_digit(bottomleft_image, s_buffer[2], true);
  set_digit(bottomright_image, s_buffer[3], false);
  center();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  // Get information about the Window
  Layer *window_layer = window_get_root_layer(window);
  
  // Create GBitmaps
  num0 = gbitmap_create_with_resource(RESOURCE_ID_NUM_0);
  num1left = gbitmap_create_with_resource(RESOURCE_ID_NUM_1_LEFT);
  num1right = gbitmap_create_with_resource(RESOURCE_ID_NUM_1_RIGHT);
  num2 = gbitmap_create_with_resource(RESOURCE_ID_NUM_2);
  num3 = gbitmap_create_with_resource(RESOURCE_ID_NUM_3);
  num4 = gbitmap_create_with_resource(RESOURCE_ID_NUM_4);
  num5 = gbitmap_create_with_resource(RESOURCE_ID_NUM_5);
  num6 = gbitmap_create_with_resource(RESOURCE_ID_NUM_6);
  num7 = gbitmap_create_with_resource(RESOURCE_ID_NUM_7);
  num8 = gbitmap_create_with_resource(RESOURCE_ID_NUM_8);
  num9 = gbitmap_create_with_resource(RESOURCE_ID_NUM_9);
  
  // Create BitmapLayers
  topleft_image = bitmap_layer_create(GRect(7, 5, 64, 76));
  topright_image = bitmap_layer_create(GRect(71, 5, 64, 76));
  bottomleft_image = bitmap_layer_create(GRect(7, 86, 64, 76));
  bottomright_image = bitmap_layer_create(GRect(71, 86, 64, 76));

  // Set transparency
  bitmap_layer_set_background_color(topleft_image, GColorClear);
  bitmap_layer_set_background_color(topright_image, GColorClear);
  bitmap_layer_set_background_color(bottomleft_image, GColorClear);
  bitmap_layer_set_background_color(bottomright_image, GColorClear);
  
  bitmap_layer_set_compositing_mode(topleft_image, GCompOpSet);
  bitmap_layer_set_compositing_mode(topright_image, GCompOpSet);
  bitmap_layer_set_compositing_mode(bottomleft_image, GCompOpSet);
  bitmap_layer_set_compositing_mode(bottomright_image, GCompOpSet);

  // Add BitmapLayers as child layers to the Window's root layer
  layer_add_child(window_layer, bitmap_layer_get_layer(topleft_image));
  layer_add_child(window_layer, bitmap_layer_get_layer(topright_image));
  layer_add_child(window_layer, bitmap_layer_get_layer(bottomleft_image));
  layer_add_child(window_layer, bitmap_layer_get_layer(bottomright_image));
}

static void main_window_unload(Window *window) {
  // Destroy BitmapLayers
  bitmap_layer_destroy(topleft_image);
  bitmap_layer_destroy(topright_image);
  bitmap_layer_destroy(bottomleft_image);
  bitmap_layer_destroy(bottomright_image);
  
  // Destroy Gbitmaps
  gbitmap_destroy(num0);
  gbitmap_destroy(num1left);
  gbitmap_destroy(num1right);
  gbitmap_destroy(num2);
  gbitmap_destroy(num3);
  gbitmap_destroy(num4);
  gbitmap_destroy(num5);
  gbitmap_destroy(num6);
  gbitmap_destroy(num7);
  gbitmap_destroy(num8);
  gbitmap_destroy(num9);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);

  // Make sure the time is displayed from the start
  update_time();

  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}