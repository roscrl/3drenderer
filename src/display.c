#include "display.h"

#include "triangle.h"

static int window_width;
static int window_height;
static SDL_Window *window = NULL;

static SDL_Renderer *renderer = NULL;

static u32 *color_buffer = NULL;
static f32 *z_buffer = NULL;
static SDL_Texture *color_buffer_texture = NULL;

static int render_method = 0;
static int cull_method = 0;

int get_window_width() { return window_width; }
int get_window_height() { return window_height; }

bool initalize_window() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    SDL_Log("initalising SDL: %s", SDL_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "initalising SDL", SDL_GetError(), 0);

    return false;
  }

  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);
  int fullscreen_width = display_mode.w;
  int fullscreen_height = display_mode.h;

  window_width = fullscreen_width / 1.5;
  window_height = fullscreen_height / 1.5;

  window = SDL_CreateWindow(
    NULL, 
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED, 
    fullscreen_width, 
    fullscreen_height, 
    SDL_WINDOW_BORDERLESS
  );
  if (!window) {
    SDL_Log("creating SDL window: %s", SDL_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "creating SDL window", SDL_GetError(), 0);

    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);
  if (!renderer) {
    SDL_Log("creating SDL renderer: %s", SDL_GetError());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "creating SDL renderer", SDL_GetError(), 0);

    return false;
  }

  color_buffer = (u32*) malloc(sizeof(u32) * window_width * window_height);
  z_buffer     = (f32*) malloc(sizeof(f32) * window_width * window_height);

  color_buffer_texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_RGBA32,
    SDL_TEXTUREACCESS_STREAMING, 
    window_width, 
    window_height
  );

  return true;
}

void render_color_buffer() {
  SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(u32)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}

void draw_pixel(int x, int y, u32 color) {
  if (!((x >= 0 && x < window_width) && (y >= 0 && y < window_height))) return;

  color_buffer[(window_width * y) + x] = color;
}

void draw_line(int x0, int y0, int x1, int y1, u32 color) {
  int delta_x = x1 - x0;
  int delta_y = y1 - y0;

  int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

  // (x_inc || y_inc) will be 1
  f32 x_inc = delta_x / (f32)longest_side_length;
  f32 y_inc = delta_y / (f32)longest_side_length;

  f32 current_x = x0;
  f32 current_y = y0;

  for (int i = 0; i <= longest_side_length; i++) {
    draw_pixel(round(current_x), round(current_y), color);
    current_x += x_inc;
    current_y += y_inc;
  }
}

void draw_triangle(triangle triangle, u32 color) {
  draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
  draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[2].x, triangle.points[2].y, color);
  draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
}

void draw_rect(int x, int y, int width, int height, u32 color) {
  for (int inY = y; inY < y + height; inY++) {
    for (int inX = x; inX < x + width; inX++) {
      draw_pixel(inX, inY, color);
    }
  }
}

void set_render_method(int method) {
  render_method = method;
}

void set_cull_method(int method) {
  cull_method = method;
}

bool is_cull_backface() {
  return cull_method == CULL_BACKSPACE;
}

bool should_render_filled_triangles() {
  return render_method == RENDER_FILL_TRIANGLE_WIRE || render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_TEXTURED_WIRE;
}

bool should_render_textured_triangles() {
  return render_method == RENDER_TEXTURED_WIRE || render_method == RENDER_TEXTURED;
}

bool should_render_wireframe() {
  return render_method == RENDER_WIRE || render_method == RENDER_TEXTURED_WIRE || render_method == RENDER_FILL_TRIANGLE_WIRE;
}

bool should_render_wire_vertex() {
  return render_method == RENDER_WIRE_VERTEX;
}

void draw_grid() {
  for (int y = 0; y < window_height; y += 10) {
    for (int x = 0; x < window_width; x += 10) {
      color_buffer[(window_width * y) + x] = 0xFF808080;
    }
  }
}

void clear_color_buffer(u32 color) {
  for (int i = 0; i < window_width * window_height; i++) {
    color_buffer[i] = color;
  }
}

void clear_z_buffer() {
  for (int i = 0; i < window_width * window_height; i++) {
      z_buffer[i] = 1.0;
  }
}

f32 get_z_buffer_at(int x, int y) {
  if (!((x >= 0 && x < window_width) && (y >= 0 && y < window_height))) return 1.0;

  return z_buffer[(window_width * y) + x];
}

void update_z_buffer_at(int x, int y, f32 z) {
  if (!((x >= 0 && x < window_width) && (y >= 0 && y < window_height))) return;

  z_buffer[(window_width * y) + x] = z;
}


void destroy_window() {
  free(color_buffer);
  free(z_buffer);
  SDL_DestroyTexture(color_buffer_texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
