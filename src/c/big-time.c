#include <pebble.h>

static Window *s_main_window;
// static TextLayer *s_text_layer;

static BitmapLayer *s_bitmap_layer_background;
static GBitmap *s_bitmap_background;

static BitmapLayer *s_bitmap_layer_time_h1;
static GBitmap *s_bitmap_time_h1;

static BitmapLayer *s_bitmap_layer_time_h1_offset;
static GBitmap *s_bitmap_time_h1_offset;

static BitmapLayer *s_bitmap_layer_time_h2;
static GBitmap *s_bitmap_time_h2;

static BitmapLayer *s_bitmap_layer_time_m1;
static GBitmap *s_bitmap_time_m1;

static BitmapLayer *s_bitmap_layer_time_m1_offset;
static GBitmap *s_bitmap_time_m1_offset;

static BitmapLayer *s_bitmap_layer_time_m2;
static GBitmap *s_bitmap_time_m2;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ? "%H:%M" : "%I:%M", tick_time);

  // text_layer_set_text(s_text_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  // Bitmap Layers
  s_bitmap_layer_background = bitmap_layer_create(bounds);
  s_bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_STATIC);
  bitmap_layer_set_bitmap(s_bitmap_layer_background, s_bitmap_background);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_background));

  s_bitmap_layer_time_h1 = bitmap_layer_create(GRect(44, 4, 46, 71));
  s_bitmap_time_h1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_2);
  bitmap_layer_set_bitmap(s_bitmap_layer_time_h1, s_bitmap_time_h1);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_h1));

  s_bitmap_layer_time_h1_offset = bitmap_layer_create(GRect(58, 4, 46, 71)); // offset by 14px
  s_bitmap_time_h1_offset = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_2);

  s_bitmap_layer_time_h2 = bitmap_layer_create(GRect(94, 4, 46, 71));
  s_bitmap_time_h2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_4);
  bitmap_layer_set_bitmap(s_bitmap_layer_time_h2, s_bitmap_time_h2);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_h2));

  s_bitmap_layer_time_m1 = bitmap_layer_create(GRect(44, 79, 46, 71));
  s_bitmap_time_m1 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_6);
  bitmap_layer_set_bitmap(s_bitmap_layer_time_m1, s_bitmap_time_m1);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_m1));

  s_bitmap_layer_time_m1_offset = bitmap_layer_create(GRect(58, 79, 46, 71)); // offset by 14px
  s_bitmap_time_m1_offset = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_6);

  s_bitmap_layer_time_m2 = bitmap_layer_create(GRect(94, 79, 46, 71));
  s_bitmap_time_m2 = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_8);
  bitmap_layer_set_bitmap(s_bitmap_layer_time_m2, s_bitmap_time_m2);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_m2));

  // Text Layer
  // s_text_layer = text_layer_create(
  //   GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // text_layer_set_background_color(s_text_layer, GColorClear);
  // text_layer_set_text_color(s_text_layer, GColorBlack);
  // // text_layer_set_text(s_text_layer, "00:00");
  // text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  // text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

  // layer_add_child(root_layer, text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
  // text_layer_destroy(s_text_layer);

  gbitmap_destroy(s_bitmap_background);
  bitmap_layer_destroy(s_bitmap_layer_background);

  gbitmap_destroy(s_bitmap_time_h1);
  bitmap_layer_destroy(s_bitmap_layer_time_h1);

  gbitmap_destroy(s_bitmap_time_h1_offset);
  bitmap_layer_destroy(s_bitmap_layer_time_h1_offset);

  gbitmap_destroy(s_bitmap_time_h2);
  bitmap_layer_destroy(s_bitmap_layer_time_h2);

  gbitmap_destroy(s_bitmap_time_m1);
  bitmap_layer_destroy(s_bitmap_layer_time_m1);

  gbitmap_destroy(s_bitmap_time_m1_offset);
  bitmap_layer_destroy(s_bitmap_layer_time_m1_offset);

  gbitmap_destroy(s_bitmap_time_m2);
  bitmap_layer_destroy(s_bitmap_layer_time_m2);
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_background_color(s_main_window, GColorBlack);

  window_stack_push(s_main_window, true);

  update_time();

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}