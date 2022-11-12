typedef unsigned long long size_t;

int __attribute__((naked)) read(unsigned int fd, char *buf, size_t count) {
  __asm("mov rax, 0x0;"
        "syscall;"
        "ret;");
}

int __attribute__((naked))
write(unsigned int fd, const char *buf, size_t count) {
  __asm("mov rax, 0x1;"
        "syscall;"
        "ret;");
}

int __attribute__((naked)) exit(unsigned int error_code) {
  __asm("mov rax, 0x3c;"
        "syscall;"
        "ret;");
}

static char welcome[] = "What sirop do you desire?\n";
static char give[] = "Here you go:\n";
static char question[] = "\nIs this all?\n";

void main() {
  char sirop[0x100];
  int r = read(0, sirop, sizeof(sirop) * 4);

  write(1, give, sizeof(give) - 1);
  write(1, sirop, r);

  write(1, question, sizeof(question));
  read(0, sirop, sizeof(sirop));

  return;
}

void _start() {
  write(1, welcome, sizeof(welcome));
  main();
  exit(0);
}
