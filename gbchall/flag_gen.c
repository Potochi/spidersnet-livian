#include <stdio.h>

void xor_decrypt(char *data, char *key, int data_len, int key_len) {
  for (int i = 0; i < data_len; i++) {
    data[i] = data[i] ^ key[i % key_len];
  }
}

char password[] = "Cr0wD57rIk3";
char flag[] = "You got it! CRWD{WH0_KN3W_hax0r2_kAN_83_hAX0r3d}";
char sudo[] = "Not even vulnerable to CVE-2021-3156";
char bash[] = "Everyone knows hackers use zsh...";

#define ARRAY_SIZE(x) (sizeof(x) / (sizeof(x[0])))

void print_arr(char *arr, int size) {
  printf("{");

  for (int i = 0; i < size; i++) {
    printf("0x%02hhx", arr[i]);
    if (i != size - 1) {
      printf(", ");
    }
  }
  printf("}\n");
}

int main() {
  xor_decrypt(flag, password, ARRAY_SIZE(flag), ARRAY_SIZE(password)- 1);
  xor_decrypt(sudo, password, ARRAY_SIZE(sudo), ARRAY_SIZE(password)- 1);
  xor_decrypt(bash, password, ARRAY_SIZE(bash), ARRAY_SIZE(password)- 1);

  print_arr(flag, ARRAY_SIZE(flag));
  print_arr(sudo, ARRAY_SIZE(sudo));
  print_arr(bash, ARRAY_SIZE(bash));

  xor_decrypt(flag, password, ARRAY_SIZE(flag), ARRAY_SIZE(password) - 1);
  xor_decrypt(sudo, password, ARRAY_SIZE(sudo), ARRAY_SIZE(password) - 1);
  xor_decrypt(bash, password, ARRAY_SIZE(bash), ARRAY_SIZE(password) - 1);

  puts(flag);
  puts(sudo);
  puts(bash);
}
