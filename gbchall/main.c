#include <gb/drawing.h>
#include <gb/gb.h>
#include <gbdk/console.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gb/hardware.h"
#include "gbdk/font.h"
#include "tilemap.h"
#include "tileset.h"

#include "Export.h"

/*
 * Real hacker's device
 * "Kali lincox" meme
 * Shell with a few programs
 * Crack sudo password to get the flag
 */

typedef struct lcg {
  uint8_t a;
  uint8_t c;
  uint8_t seed;
} lcg_t;

void lcg_init(lcg_t *lcg, int a, int c, int seed) {
  lcg->a = a;
  lcg->c = c;
  lcg->seed = seed;
}

uint8_t lcg_next(lcg_t *lcg) {
  uint8_t rnd = lcg->a * lcg->seed + lcg->c;
  lcg->seed = rnd;

  return rnd;
}

static lcg_t lcg;

void slow_print_text(const char *const text, uint16_t delay_amount) {
  for (uint8_t i = 0; i < strlen(text); i++) {
    putchar(text[i]);
    delay(delay_amount);
  }
}

typedef struct message {
  const char *message;
  uint16_t wait;
} message_t;

static const message_t message_list[] = {{.message = "", .wait = 1000}};

void halt() {
  cls();
  slow_print_text("Frobnication error!", 30);
  puts("\n\n");
  slow_print_text("Please call a\nlevel II certified\nLinux H@Xx0R "
                  "for\nfurther assistance.",
                  30);
  while (1) {
    __asm__("halt;");
  }
}

uint8_t read_binary_uchar(uint8_t silent) {
  uint8_t chr = 0;

  for (uint8_t i = 0; i < 8; i++) {
    uint8_t mask = waitpad(J_A | J_B);

    if (mask & J_A) {
      chr = (chr << 1) | 1;
      if (!silent) {
        putchar('-');
      }
    } else if (mask & J_B) {
      chr = (chr << 1) | 0;
      if (!silent) {
        putchar('.');
      }
    }

    waitpadup();
    wait_vbl_done();
  }

  return chr;
}

void setup_entropy() {
  unsigned char expected[32] = {0x1c, 0x57, 0x08, 0x1b, 0x54, 0xff, 0x00, 0x03,
                                0x0c, 0x27, 0x78, 0x6b, 0x44, 0xcf, 0x70, 0x53,
                                0xfc, 0xf7, 0xe8, 0xbb, 0x34, 0x9f, 0xe0, 0xa3,
                                0xec, 0xc7, 0x58, 0x0b, 0x24, 0x6f, 0x50, 0xf3};
  unsigned char got[32];

  uint8_t seed = read_binary_uchar(0);

  /* Seed should be 0b10110011 */
  lcg_init(&lcg, seed & 0xF, seed & 0x0F, seed);

  for (uint8_t i = 0; i < 32; i++) {
    got[i] = lcg_next(&lcg);
  }

  if (memcmp(expected, got, 32)) {
    halt();
  }
}

#define ARRAY_SIZE(x) (sizeof(x) / (sizeof(x[0])))

typedef struct file {
  uint8_t idx;
  char *name;
  char *data;
  uint8_t len;
} file_t;

static uint8_t authed = 0;

static char flag_data[] = {0x1a, 0x1d, 0x45, 0x57, 0x23, 0x5a, 0x43, 0x52, 0x20,
                           0x1f, 0x12, 0x63, 0x31, 0x62, 0x20, 0x00, 0x4e, 0x60,
                           0x3a, 0x79, 0x34, 0x78, 0x0d, 0x41, 0x67, 0x28, 0x2c,
                           0x54, 0x4f, 0x42, 0x3b, 0x59, 0x6c, 0x28, 0x33, 0x7e,
                           0x28, 0x7c, 0x06, 0x68, 0x1a, 0x08, 0x33, 0x03, 0x31,
                           0x41, 0x54, 0x0a, 0x44};
static char sudo_data[] = {0x0d, 0x1d, 0x44, 0x57, 0x21, 0x43, 0x52, 0x1c,
                           0x69, 0x1d, 0x46, 0x2f, 0x1c, 0x55, 0x05, 0x25,
                           0x57, 0x5b, 0x17, 0x69, 0x1f, 0x5c, 0x63, 0x31,
                           0x66, 0x32, 0x69, 0x07, 0x07, 0x40, 0x78, 0x46,
                           0x00, 0x72, 0x47, 0x06, 0x77};
static char bash_data[] = {0x06, 0x04, 0x55, 0x05, 0x3d, 0x5a, 0x59, 0x17, 0x69,
                           0x00, 0x5d, 0x2c, 0x05, 0x43, 0x57, 0x2c, 0x54, 0x54,
                           0x19, 0x2c, 0x19, 0x40, 0x63, 0x07, 0x43, 0x12, 0x64,
                           0x4f, 0x44, 0x1a, 0x67, 0x45, 0x1d, 0x43};

static const file_t files[] = {
    {.idx = 0, .name = "bash", .data = bash_data, .len = ARRAY_SIZE(bash_data)},
    {.idx = 1,
     .name = "flag.pdf",
     .data = flag_data,
     .len = ARRAY_SIZE(flag_data)},
    {.idx = 2,
     .name = "sudo.exe",
     .data = sudo_data,
     .len = ARRAY_SIZE(sudo_data)},
};

void read_string(char *str, uint8_t size) {
  int i;

  for (i = 0; i < size - 1; i++) {
    str[i] = read_binary_uchar(0);

    if (str[i] == 0) {
      break;
    }
  }

  str[i + 1] = 0;
}

char *m_strchr(char *string, char chr) {
  while (*string != chr && *string != 0) {
    string += 1;
  };
  return *string == 0 ? NULL : string;
}

void xor_decrypt(char *data, char *key, int data_len, int key_len) {
  for (int i = 0; i < data_len; i++) {
    data[i] = data[i] ^ key[i % key_len];
  }
}

void shell() {
  char cmd[0x32];

  while (1) {
    putchar('\n');
    putchar(authed ? '#' : '$');
    putchar(' ');

    read_string(cmd, ARRAY_SIZE(cmd));

    char *space = m_strchr(cmd, ' ');

    /* Commands with no parameters */
    if (space == NULL) {
      if (!strcmp(cmd, "help")) {
        slow_print_text("\nsu, exit, ls, cat\n", 30);
      } else if (!strcmp(cmd, "exit")) {
        slow_print_text("\nlogout?\n", 30);
        slow_print_text("Press the 'Any' key to confirm\n", 30);
        slow_print_text("...\n", 1000);
        slow_print_text("Confirmation failure", 30);
        slow_print_text("Continuing...\n", 30);
      } else if (!strcmp(cmd, "ls")) {
        for (uint8_t i = 0; i < ARRAY_SIZE(files); i++) {
          putchar('\n');
          printf("[%hu] - ", files[i].idx);
          slow_print_text(files[i].name, 200);
          putchar('\n');
        }
      } else if (!strcmp(cmd, "su")) {
        slow_print_text("\nPassword: ", 200);
        /* Cool password Cr0wD57rIk3 */
        /*               01234567890 */
        /*               XXXXXXXXXXX */
        char password[32];
        read_string(password, ARRAY_SIZE(password));

        if (password[2] == '0') {
          if (password[6] == '7') {
            if (password[0] == 'C') {
              if (password[8] == 'I') {
                if (password[4] == 'D') {
                  if (password[1] == 'r') {
                    if (password[5] == '5') {
                      if (password[7] == 'r') {
                        if (password[3] == 'w') {
                          if (password[10] == '3') {
                            if (password[9] == 'k') {
                              authed = 1;
                              for (int i = 0; i < ARRAY_SIZE(files); i++) {
                                xor_decrypt(files[i].data, password,
                                            files[i].len, strlen(password));
                              }
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    } else {
      if (!strncmp(cmd, "cat", 3)) {
        *space = 0;
        space += 1;

        uint8_t printed = 0;

        for (int x = 0; x < ARRAY_SIZE(files); x++) {
          putchar('\n');
          if (!strcmp(space, files[x].name)) {
            printed = 1;

            for (int j = 0; j < files[x].len; j++) {
              putchar(files[x].data[j]);
            }
            putchar('\n');
          }
        }

        if (!printed) {
          puts("No such file");
        }
      }
    }

    wait_vbl_done();
  }
}

int main() {
  set_bkg_data(0, TILESET_TILE_COUNT, TILESET);
  set_bkg_tiles(0, 0, TILEMAP_WIDTH, TILEMAP_HEIGHT, TILEMAP);

  SHOW_BKG;
  DISPLAY_ON;
  SHOW_SPRITES;

  set_sprite_data(0, 4, TileLabel);
  set_sprite_tile(0, 0);
  move_sprite(0, 80, 120);

  for (int i = 0 ; i < 30 ; i ++) {
    set_sprite_tile(0, i % 4);
    delay(100);
    wait_vbl_done();
  }
  hide_sprite(0);

  delay(1000);
  DISPLAY_OFF;

  font_init();
  cls();
  DISPLAY_ON;
  
  slow_print_text("login: ", 30);
  setup_entropy();
  shell();
  while (1) {
    wait_vbl_done();
  }
}
