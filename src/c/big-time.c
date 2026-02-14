#include <pebble.h>
#include "bitmap_info.h"

const uint16_t L_TENS_X = 44;
const uint16_t L_TENS_X_OFFSET = 17;
const uint16_t L_ONES_X = 94;
const uint16_t L_HOURS_Y = 4;
const uint16_t L_MINUTES_Y = 79;
const uint16_t L_WIDTH = 46; 
const uint16_t L_HEIGHT = 71;

const uint16_t S_WIDTH = 8;
const uint16_t S_WIDTH_1 = 5;
const uint16_t S_WIDTH_DASH = 4;
const uint16_t S_WIDTH_DEGREE = 4;
const uint16_t S_WIDTH_LOADING = 4;
const uint16_t S_HEIGHT = 13;

const uint16_t XS_WIDTH = 6;
const uint16_t XS_WIDTH_1 = 4;
const uint16_t XS_WIDTH_DASH = 4;
const uint16_t XS_HEIGHT = 10;

// Special character indexes
const uint16_t INDEX_DASH = 10; // '-'
const uint16_t INDEX_DEGREE = 11; // '°'
const uint16_t INDEX_LOADING = 12; // '...'

// UI element dimensions

// Date
const uint16_t UI_DATE_X = 4;
const uint16_t UI_DATE_Y = 4;
const uint16_t UI_DATE_W = 36;
const uint16_t UI_DATE_H = 21;
const uint16_t UI_DATE_SPACING = 1;
const uint16_t UI_DATE_CONTENT_Y = 6;

// Steps
const uint16_t UI_STEPS_X = 4;
const uint16_t UI_STEPS_Y = 29;
const uint16_t UI_STEPS_W = 36;
const uint16_t UI_STEPS_H = 21;
const uint16_t UI_STEPS_SPACING = 1;
const uint16_t UI_STEPS_CONTENT_Y = 6;

// Temp (All)
const uint16_t UI_TEMP_SPACING = 1;
const uint16_t UI_TEMP_CONTENT_Y = 4;
const uint16_t UI_TEMP_OFFSET = 1;

// Temp High
const uint16_t UI_TEMP_HI_X = 4;
const uint16_t UI_TEMP_HI_Y = 54;
const uint16_t UI_TEMP_HI_W = 36;
const uint16_t UI_TEMP_HI_H = 21;

// Temp Current
const uint16_t UI_TEMP_CUR_X = 4;
const uint16_t UI_TEMP_CUR_Y = 79;
const uint16_t UI_TEMP_CUR_W = 36;
const uint16_t UI_TEMP_CUR_H = 21;

// Temp Low
const uint16_t UI_TEMP_LO_X = 4;
const uint16_t UI_TEMP_LO_Y = 104;
const uint16_t UI_TEMP_LO_W = 36;
const uint16_t UI_TEMP_LO_H = 21;

// Conditions
const uint16_t UI_CONDITIONS_X = 4;
const uint16_t UI_CONDITIONS_Y = 128;
const uint16_t UI_CONDITIONS_W = 36;
const uint16_t UI_CONDITIONS_H = 36;

static struct tm s_current_time;
static int s_current_steps = 0;
static bool s_temp_high_loading = true;
static int s_temp_high = 0;
static bool s_temp_current_loading = true;
static int s_temp_current = 0;
static bool s_temp_low_loading = true;
static int s_temp_low = 0;
static long s_sunrise_seconds = 0;
static long s_sunset_seconds = 0;
static char s_condition[50];

static Window *s_main_window;
// static TextLayer *s_text_layer;
static GBitmap *s_bitmap_background;

static int s_sunrise_sunset_ui_start_x = 45;
static int s_sun_index_y = 165;
static int s_sun_index_bitmap_left_offset = 3;
static int s_sun_index_bitmap_width = 4;
static int s_sun_index_bitmap_height = 3;
static GBitmap *s_bitmap_sun_index;

static GBitmap *s_bitmap_numbers_lg[10];
static BitmapInfo s_bitmap_numbers_s_light[13];
static BitmapInfo s_bitmap_numbers_s_dark[13];
static BitmapInfo s_bitmap_numbers_xs_light[11];
static BitmapInfo s_bitmap_numbers_xs_dark[11];

static BitmapLayer *s_bitmap_layer_background;
static BitmapLayer *s_bitmap_layer_sun_index;
static BitmapLayer *s_bitmap_layer_time_h1;
static BitmapLayer *s_bitmap_layer_time_h1_offset;
static BitmapLayer *s_bitmap_layer_time_h2;
static BitmapLayer *s_bitmap_layer_time_m1;
static BitmapLayer *s_bitmap_layer_time_m1_offset;
static BitmapLayer *s_bitmap_layer_time_m2;

static Layer *s_layer_date;
static Layer *s_layer_steps;
static Layer *s_layer_temp_high;
static Layer *s_layer_temp_current;
static Layer *s_layer_temp_low;

static GBitmap *s_bitmap_conditions[10];
static BitmapLayer *s_bitmap_layer_conditions;

int calculate_string_width_px(char *str, BitmapInfo *bitmapInfoArray, uint16_t spacing_px) {
  int total_width = 0;

  for (int i = 0; str[i] != '\0'; i++) {
    char current_char = str[i];

    switch (current_char)
    {
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      {
        int digit_int = str[i] - '0';
        total_width += bitmapInfoArray[digit_int].width;
        break;
      }
      case '-':
        total_width += bitmapInfoArray[INDEX_DASH].width;
        break;
      case '*': // '°'
        total_width += bitmapInfoArray[INDEX_DEGREE].width;
        break;
      case '~':
        total_width += bitmapInfoArray[INDEX_LOADING].width;
        break;
      default:
        break;
    }

    if (str[i+1] != '\0') {
      total_width += spacing_px;
    }
  }

  return total_width;
}

void draw_string(GContext *ctx, char *str, int x_start, int y, BitmapInfo *bitmapInfoArray, uint16_t spacing_px) {
  int x_current = x_start;
  
  for (int i = 0; str[i] != '\0'; i++) {
    char current_char = str[i];
    int current_width = 0;

    // APP_LOG(APP_LOG_LEVEL_INFO, "About to draw character: %c", current_char);

    switch (current_char)
    {
      case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
      {
        int digit_int = current_char - '0';
        current_width = bitmapInfoArray[digit_int].width;
        graphics_draw_bitmap_in_rect(ctx, bitmapInfoArray[digit_int].gbitmap, GRect(x_current, y, bitmapInfoArray[digit_int].width, bitmapInfoArray[digit_int].height));
        break;
      }
      case '-':
        current_width = bitmapInfoArray[INDEX_DASH].width;
        graphics_draw_bitmap_in_rect(ctx, bitmapInfoArray[INDEX_DASH].gbitmap, GRect(x_current, y, bitmapInfoArray[INDEX_DASH].width, bitmapInfoArray[INDEX_DASH].height));
        break;
      case '*':
        current_width = bitmapInfoArray[INDEX_DEGREE].width;
        graphics_draw_bitmap_in_rect(ctx, bitmapInfoArray[INDEX_DEGREE].gbitmap, GRect(x_current, y, bitmapInfoArray[INDEX_DEGREE].width, bitmapInfoArray[INDEX_DEGREE].height));
        break;
      case '~':
        current_width = bitmapInfoArray[INDEX_LOADING].width;
        graphics_draw_bitmap_in_rect(ctx, bitmapInfoArray[INDEX_LOADING].gbitmap, GRect(x_current, y, bitmapInfoArray[INDEX_LOADING].width, bitmapInfoArray[INDEX_LOADING].height));
        break;
      default:
        break;
    }

    // increase x (include spacing)
    x_current += current_width + spacing_px;
  }
}

static void layer_date_update_proc(Layer *layer, GContext *ctx) {
  
  static char date_str[] = "MM-DD";
  strftime(date_str, sizeof(date_str), "%m-%d", &s_current_time);
  
  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bounds = layer_get_bounds(layer);
  int layer_width = bounds.size.w;

  int date_width = calculate_string_width_px(date_str, s_bitmap_numbers_xs_light, UI_DATE_SPACING);

  int starting_x = (layer_width - date_width) / 2;

  draw_string(ctx, date_str, starting_x, UI_DATE_CONTENT_Y, s_bitmap_numbers_xs_light, UI_DATE_SPACING);
}

static void layer_steps_update_proc(Layer *layer, GContext *ctx) {
  static char steps_str[] = "00000";
  snprintf(steps_str, sizeof(steps_str), "%03d", s_current_steps);

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bounds = layer_get_bounds(layer);
  int layer_width = bounds.size.w;

  int steps_width = calculate_string_width_px(steps_str, s_bitmap_numbers_xs_dark, UI_STEPS_SPACING);
  
  int starting_x = (layer_width - steps_width) / 2;

  draw_string(ctx, steps_str, starting_x, UI_STEPS_CONTENT_Y, s_bitmap_numbers_xs_dark, UI_STEPS_SPACING);
}

static void layer_temp_high_update_proc(Layer *layer, GContext *ctx) {
  static char temp_high_str[] = "-2000°";
  if (s_temp_high_loading) {
    strcpy(temp_high_str, "~");
  }
  else {
    snprintf(temp_high_str, sizeof(temp_high_str), "%d*", s_temp_high);
  }

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bounds = layer_get_bounds(layer);
  int layer_width = bounds.size.w;

  int temp_high_width = calculate_string_width_px(temp_high_str, s_bitmap_numbers_s_light, UI_TEMP_SPACING);

  int starting_x = (layer_width - temp_high_width) / 2;

  // Ignore width of degree symbol
  starting_x += UI_TEMP_OFFSET;

  draw_string(ctx, temp_high_str, starting_x, UI_TEMP_CONTENT_Y, s_bitmap_numbers_s_light, UI_TEMP_SPACING);
}

static void layer_temp_current_update_proc(Layer *layer, GContext *ctx) {
  static char temp_current_str[] = "-2000°";
  if (s_temp_current_loading) {
    strcpy(temp_current_str, "~");
  }
  else {
    snprintf(temp_current_str, sizeof(temp_current_str), "%d*", s_temp_current);
  }

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bounds = layer_get_bounds(layer);
  int layer_width = bounds.size.w;

  int temp_current_width = calculate_string_width_px(temp_current_str, s_bitmap_numbers_s_dark, UI_TEMP_SPACING);

  int starting_x = (layer_width - temp_current_width) / 2;

  // Ignore width of degree symbol
  starting_x += UI_TEMP_OFFSET;

  draw_string(ctx, temp_current_str, starting_x, UI_TEMP_CONTENT_Y, s_bitmap_numbers_s_dark, UI_TEMP_SPACING);
}

static void layer_temp_low_update_proc(Layer *layer, GContext *ctx) {
  static char temp_low_str[] = "-2000°";
  if (s_temp_low_loading) {
    strcpy(temp_low_str, "~");
  }
  else {
    snprintf(temp_low_str, sizeof(temp_low_str), "%d*", s_temp_low);
  }

  graphics_context_set_compositing_mode(ctx, GCompOpSet);
  GRect bounds = layer_get_bounds(layer);
  int layer_width = bounds.size.w;

  int temp_low_width = calculate_string_width_px(temp_low_str, s_bitmap_numbers_s_light, UI_TEMP_SPACING);

  int starting_x = (layer_width - temp_low_width) / 2;

  // Ignore width of degree symbol
  starting_x += UI_TEMP_OFFSET;

  draw_string(ctx, temp_low_str, starting_x, UI_TEMP_CONTENT_Y, s_bitmap_numbers_s_light, UI_TEMP_SPACING);
}

static void update_time(struct tm *tick_time) {

  int display_hour = tick_time->tm_hour;

  if (clock_is_24h_style() == false) {
    if (display_hour > 12) {
      display_hour -= 12;
    }
    else if (display_hour == 0) {
      display_hour = 12;
    }
  }

  int h_tens = display_hour / 10;
  int h_ones = display_hour % 10;

  bitmap_layer_set_bitmap(s_bitmap_layer_time_h2, s_bitmap_numbers_lg[h_ones]);
  if (!clock_is_24h_style() && h_tens == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1, NULL);
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1_offset, NULL);
  }
  else if (h_ones == 1) {
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1, NULL);
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1_offset, s_bitmap_numbers_lg[h_tens]);
  }
  else {
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1, s_bitmap_numbers_lg[h_tens]);
    bitmap_layer_set_bitmap(s_bitmap_layer_time_h1_offset, NULL);
  }

  int display_minutes = tick_time->tm_min;
  
  int m_tens = display_minutes / 10;
  int m_ones = display_minutes % 10;

  bitmap_layer_set_bitmap(s_bitmap_layer_time_m2, s_bitmap_numbers_lg[m_ones]);
  if (m_ones == 1) {
    bitmap_layer_set_bitmap(s_bitmap_layer_time_m1, NULL);
    bitmap_layer_set_bitmap(s_bitmap_layer_time_m1_offset, s_bitmap_numbers_lg[m_tens]);
  }
  else {
    bitmap_layer_set_bitmap(s_bitmap_layer_time_m1, s_bitmap_numbers_lg[m_tens]);
    bitmap_layer_set_bitmap(s_bitmap_layer_time_m1_offset, NULL);
  }
}

static void update_date(struct tm *tick_time) {
  // TODO add logic so you only update the UI when the day changes for efficiency
  layer_mark_dirty(s_layer_date);
}

static void update_sun_index(struct tm *tick_time) {
  int hr = tick_time->tm_hour;
  int min = tick_time->tm_min;
  
  int minute_of_day = hr * 60;
  minute_of_day += min;

  int sun_index_x = s_sunrise_sunset_ui_start_x - s_sun_index_bitmap_left_offset + (minute_of_day / 15);

  layer_set_frame(bitmap_layer_get_layer(s_bitmap_layer_sun_index), GRect(sun_index_x, s_sun_index_y, s_sun_index_bitmap_width, s_sun_index_bitmap_height));
}

static void update_steps() {
  #if defined(PBL_HEALTH)

  HealthMetric metric = HealthMetricStepCount;

  time_t start = time_start_of_today();
  time_t end = time(NULL);

  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, start, end);

  bool data_available = mask & HealthServiceAccessibilityMaskAvailable;
  if (data_available) {
    s_current_steps = (int)health_service_sum_today(metric);
    layer_mark_dirty(s_layer_steps);
  }

  #else
  // Health data not available
  #endif
}

static time_t get_midnight_today() {
  // 1. Get the current time
  time_t now = time(NULL);
  
  // 2. Convert to struct tm (Local Time)
  struct tm *t = localtime(&now);
  
  // 3. Set time to 00:00:00
  t->tm_sec = 0;
  t->tm_min = 0;
  t->tm_hour = 0;
  
  // 4. Optional: Ensure daylight savings flag is handled automatically
  t->tm_isdst = -1; 
  
  // 5. Convert back to seconds since epoch
  return mktime(t);
}

static bool is_night() {
  time_t midnight_today = get_midnight_today();
  if (s_sunrise_seconds > midnight_today && s_sunset_seconds > midnight_today) {
    time_t now = time(NULL);
    return (now < s_sunrise_seconds || now > s_sunset_seconds);
  }
  else {
    // old sunrise/sunset values - return true
    return false;
  }
}

static void update_conditions() {

  if (strcmp(s_condition, "CLEAR") == 0) {
    if (is_night()) {
      bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[1]); // moon
    }
    else {
      bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[0]); // sun
    }
  }
  else if (strcmp(s_condition, "PARTLY_CLOUDY") == 0) {
    if (is_night()) {
      bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[3]); // moon-clouds
    }
    else {
      bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[2]); // sun-clouds
    }
  }
  else if (strcmp(s_condition, "CLOUDS") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[4]);
  }
  
  else if (strcmp(s_condition, "DRIZZLE") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[5]);
  }
  
  else if (strcmp(s_condition, "RAIN") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[6]);
  }
  
  else if (strcmp(s_condition, "SNOW") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[7]);
  }
  
  else if (strcmp(s_condition, "THUNDERSTORM") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[8]);
  }
  
  else if (strcmp(s_condition, "ATMOSPHERE") == 0) {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, s_bitmap_conditions[9]);
  }
  else {
    bitmap_layer_set_bitmap(s_bitmap_layer_conditions, NULL);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  s_current_time = *tick_time;

  update_time(tick_time);
  update_date(tick_time);
  update_sun_index(tick_time);
  update_steps();

  // Request weather info
  if (tick_time->tm_min %30 == 0) {
  // if (tick_time->tm_min %1 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);

    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);

    // Send the message!
    app_message_outbox_send(); // This requests the latest weather and daylight
  }
}

static void main_window_load(Window *window) {
  Layer *root_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(root_layer);

  // Load Bitmaps

  // LG
  s_bitmap_numbers_lg[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_0);
  s_bitmap_numbers_lg[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_1_V3);
  s_bitmap_numbers_lg[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_2);
  s_bitmap_numbers_lg[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_3);
  s_bitmap_numbers_lg[4] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_4_V2);
  s_bitmap_numbers_lg[5] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_5_V2);
  s_bitmap_numbers_lg[6] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_6_V5);
  s_bitmap_numbers_lg[7] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_7);
  s_bitmap_numbers_lg[8] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_8);
  s_bitmap_numbers_lg[9] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_LG_9_V4);

  // S Light
  s_bitmap_numbers_s_light[0] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_0_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[1] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_1_LIGHT), .width = S_WIDTH_1, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[2] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_2_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[3] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_3_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[4] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_4_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[5] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_5_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[6] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_6_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[7] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_7_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[8] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_8_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[9] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_9_LIGHT), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[INDEX_DASH] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_DASH_LIGHT), .width = S_WIDTH_DASH, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[INDEX_DEGREE] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_DEGREE_LIGHT), .width = S_WIDTH_DEGREE, .height = S_HEIGHT};
  s_bitmap_numbers_s_light[INDEX_LOADING] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_LOADING_LIGHT), .width = S_WIDTH_LOADING, .height = S_HEIGHT};

  // S Dark
  s_bitmap_numbers_s_dark[0] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_0_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[1] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_1_DARK), .width = S_WIDTH_1, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[2] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_2_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[3] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_3_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[4] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_4_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[5] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_5_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[6] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_6_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[7] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_7_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[8] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_8_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[9] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_9_DARK), .width = S_WIDTH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[INDEX_DASH] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_DASH_DARK), .width = S_WIDTH_DASH, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[INDEX_DEGREE] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_DEGREE_DARK), .width = S_WIDTH_DEGREE, .height = S_HEIGHT};
  s_bitmap_numbers_s_dark[INDEX_LOADING] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_S_LOADING_DARK), .width = S_WIDTH_LOADING, .height = S_HEIGHT};

  // XS Light
  s_bitmap_numbers_xs_light[0] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_0_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[1] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_1_LIGHT), .width = XS_WIDTH_1, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[2] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_2_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[3] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_3_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[4] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_4_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[5] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_5_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[6] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_6_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[7] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_7_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[8] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_8_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[9] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_9_LIGHT), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_light[INDEX_DASH] =(BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_DASH_LIGHT), .width = XS_WIDTH_DASH, .height = XS_HEIGHT};

  // XS Dark
  s_bitmap_numbers_xs_dark[0] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_0_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[1] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_1_DARK), .width = XS_WIDTH_1, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[2] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_2_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[3] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_3_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[4] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_4_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[5] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_5_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[6] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_6_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[7] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_7_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[8] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_8_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[9] = (BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_9_DARK), .width = XS_WIDTH, .height = XS_HEIGHT};
  s_bitmap_numbers_xs_dark[INDEX_DASH] =(BitmapInfo) {.gbitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NUMBER_XS_DASH_DARK), .width = XS_WIDTH_DASH, .height = XS_HEIGHT};

  s_bitmap_conditions[0] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_CLEAR_SUN_LIGHT);
  s_bitmap_conditions[1] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_CLEAR_MOON_LIGHT);
  s_bitmap_conditions[2] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_PARTLY_CLOUDY_SUN_LIGHT);
  s_bitmap_conditions[3] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_PARTLY_CLOUDY_MOON_LIGHT);
  s_bitmap_conditions[4] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_CLOUDS_LIGHT);
  s_bitmap_conditions[5] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_DRIZZLE_LIGHT);
  s_bitmap_conditions[6] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_RAIN_LIGHT);
  s_bitmap_conditions[7] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_SNOW_LIGHT);
  s_bitmap_conditions[8] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_THUNDERSTORM_LIGHT);
  s_bitmap_conditions[9] = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_CONDITION_ATMOSPHERE_LIGHT);

  // Bitmap Layers
  s_bitmap_layer_background = bitmap_layer_create(bounds);
  s_bitmap_background = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND_STATIC_V8_CONDITIONS);
  bitmap_layer_set_bitmap(s_bitmap_layer_background, s_bitmap_background);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_background));

  s_bitmap_layer_sun_index = bitmap_layer_create(GRect(s_sunrise_sunset_ui_start_x, s_sun_index_y, s_sun_index_bitmap_width, s_sun_index_bitmap_height)); // initial sun_index at 12:00am.
  s_bitmap_sun_index = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SUN_INDEX);
  bitmap_layer_set_bitmap(s_bitmap_layer_sun_index, s_bitmap_sun_index);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_sun_index));

  s_bitmap_layer_time_h1 = bitmap_layer_create(GRect(L_TENS_X, L_HOURS_Y, L_WIDTH, L_HEIGHT));
  bitmap_layer_set_alignment(s_bitmap_layer_time_h1, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_h1));
  
  s_bitmap_layer_time_h1_offset = bitmap_layer_create(GRect((L_TENS_X + L_TENS_X_OFFSET), L_HOURS_Y, L_WIDTH, L_HEIGHT)); // offset when ones place is a '1' since it's not as wide.
  bitmap_layer_set_alignment(s_bitmap_layer_time_h1_offset, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_h1_offset));
  
  s_bitmap_layer_time_h2 = bitmap_layer_create(GRect(L_ONES_X, L_HOURS_Y, L_WIDTH, L_HEIGHT));
  bitmap_layer_set_alignment(s_bitmap_layer_time_h2, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_h2));
  
  s_bitmap_layer_time_m1 = bitmap_layer_create(GRect(L_TENS_X, L_MINUTES_Y, L_WIDTH, L_HEIGHT));
  bitmap_layer_set_alignment(s_bitmap_layer_time_m1, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_m1));
  
  s_bitmap_layer_time_m1_offset = bitmap_layer_create(GRect((L_TENS_X + L_TENS_X_OFFSET), L_MINUTES_Y, L_WIDTH, L_HEIGHT)); // offset when ones place is a '1' since it's not as wide.
  bitmap_layer_set_alignment(s_bitmap_layer_time_m1_offset, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_m1_offset));
  
  s_bitmap_layer_time_m2 = bitmap_layer_create(GRect(L_ONES_X, L_MINUTES_Y, L_WIDTH, L_HEIGHT));
  bitmap_layer_set_alignment(s_bitmap_layer_time_m2, GAlignRight);
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_time_m2));

  s_layer_date = layer_create(GRect(UI_DATE_X, UI_DATE_Y, UI_DATE_W, UI_DATE_H));
  layer_set_update_proc(s_layer_date, layer_date_update_proc);
  layer_add_child(root_layer, s_layer_date);

  s_layer_steps = layer_create(GRect(UI_STEPS_X, UI_STEPS_Y, UI_STEPS_W, UI_STEPS_H));
  layer_set_update_proc(s_layer_steps, layer_steps_update_proc);
  layer_add_child(root_layer, s_layer_steps);

  s_layer_temp_high = layer_create(GRect(UI_TEMP_HI_X, UI_TEMP_HI_Y, UI_TEMP_HI_W, UI_TEMP_HI_H));
  layer_set_update_proc(s_layer_temp_high, layer_temp_high_update_proc);
  layer_add_child(root_layer, s_layer_temp_high);

  s_layer_temp_current = layer_create(GRect(UI_TEMP_CUR_X, UI_TEMP_CUR_Y, UI_TEMP_CUR_W, UI_TEMP_HI_H));
  layer_set_update_proc(s_layer_temp_current, layer_temp_current_update_proc);
  layer_add_child(root_layer, s_layer_temp_current);

  s_layer_temp_low = layer_create(GRect(UI_TEMP_LO_X, UI_TEMP_LO_Y, UI_TEMP_LO_W, UI_TEMP_LO_H));
  layer_set_update_proc(s_layer_temp_low, layer_temp_low_update_proc);
  layer_add_child(root_layer, s_layer_temp_low);

  s_bitmap_layer_conditions = bitmap_layer_create(GRect(UI_CONDITIONS_X, UI_CONDITIONS_Y, UI_CONDITIONS_W, UI_CONDITIONS_H));
  layer_add_child(root_layer, bitmap_layer_get_layer(s_bitmap_layer_conditions));
}

static void main_window_unload(Window *window) {
  
  bitmap_layer_destroy(s_bitmap_layer_time_h1);
  bitmap_layer_destroy(s_bitmap_layer_time_h1_offset);
  bitmap_layer_destroy(s_bitmap_layer_time_h2);
  bitmap_layer_destroy(s_bitmap_layer_time_m1);
  bitmap_layer_destroy(s_bitmap_layer_time_m1_offset);
  bitmap_layer_destroy(s_bitmap_layer_time_m2);
  
  size_t array_length = sizeof(s_bitmap_numbers_lg) / sizeof(*s_bitmap_numbers_lg);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_numbers_lg[i]);
  }

  layer_destroy(s_layer_date);
  layer_destroy(s_layer_steps);
  layer_destroy(s_layer_temp_high);
  layer_destroy(s_layer_temp_current);
  layer_destroy(s_layer_temp_low);

  array_length = sizeof(s_bitmap_numbers_s_light) / sizeof(*s_bitmap_numbers_s_light);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_numbers_s_light[i].gbitmap);
  }

  array_length = sizeof(s_bitmap_numbers_s_dark) / sizeof(*s_bitmap_numbers_s_dark);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_numbers_s_dark[i].gbitmap);
  }

  array_length = sizeof(s_bitmap_numbers_xs_light) / sizeof(*s_bitmap_numbers_xs_light);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_numbers_xs_light[i].gbitmap);
  }

  array_length = sizeof(s_bitmap_numbers_xs_dark) / sizeof(*s_bitmap_numbers_xs_dark);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_numbers_xs_dark[i].gbitmap);
  }

  array_length = sizeof(s_bitmap_conditions) / sizeof(*s_bitmap_conditions);
  for (size_t i = 0; i < array_length; i++) {
    gbitmap_destroy(s_bitmap_conditions[i]);
  }
  bitmap_layer_destroy(s_bitmap_layer_conditions);

  gbitmap_destroy(s_bitmap_sun_index);
  bitmap_layer_destroy(s_bitmap_layer_sun_index);

  gbitmap_destroy(s_bitmap_background);
  bitmap_layer_destroy(s_bitmap_layer_background);
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Inbox received message!");

  Tuple *temp_hi_tuple = dict_find(iterator, MESSAGE_KEY_TEMP_HI);
  Tuple *temp_cur_tuple = dict_find(iterator, MESSAGE_KEY_TEMP_CUR);
  Tuple *temp_lo_tuple = dict_find(iterator, MESSAGE_KEY_TEMP_LO);
  Tuple *conditions_tuple = dict_find(iterator, MESSAGE_KEY_CONDITIONS);
  Tuple *sunrise_tuple = dict_find(iterator, MESSAGE_KEY_SUNRISE);
  Tuple *sunset_tuple = dict_find(iterator, MESSAGE_KEY_SUNSET);

  if (temp_hi_tuple) {
    s_temp_high = (int)temp_hi_tuple->value->int32;
    s_temp_high_loading = false;
    layer_mark_dirty(s_layer_temp_high);
  }
  if (temp_cur_tuple) {
    s_temp_current = (int)temp_cur_tuple->value->int32;
    s_temp_current_loading = false;
    layer_mark_dirty(s_layer_temp_current);
  }
  if (temp_lo_tuple) {
    s_temp_low = (int)temp_lo_tuple->value->int32;
    s_temp_low_loading = false;
    layer_mark_dirty(s_layer_temp_low);
  }
  if (sunrise_tuple) {
    s_sunrise_seconds = (int)sunrise_tuple->value->int32;
  }
  if (sunset_tuple) {
    s_sunset_seconds = (int)sunset_tuple->value->int32;
  }
  if (conditions_tuple) {
    snprintf(s_condition, sizeof(s_condition), "%s", conditions_tuple->value->cstring);
    APP_LOG(APP_LOG_LEVEL_INFO, "RECEIVED CONDITION: %s", s_condition);
    update_conditions();
  }
}

static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void init() {
  s_main_window = window_create();

  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  window_set_background_color(s_main_window, GColorBlack);

  window_stack_push(s_main_window, true);
  
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  update_time(tick_time);
  update_date(tick_time);
  update_sun_index(tick_time);
  update_steps();

  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  // Open AppMessage
  const int inbox_size = 128;
  const int outbox_size = 128;
  app_message_open(inbox_size, outbox_size);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_handler(tick_time, MINUTE_UNIT);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}