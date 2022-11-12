build:
	gcc \
    -Wall \
    -Wextra \
    -Wl,-z,relro,-z,now \
    -Wconversion \
    -fstack-protector-all \
    -Werror \
    -no-pie \
    -fno-pic \
    -D_FORTIFY_SOURCE=2 \
    -s \
    -O2 \
    cold_storage.c -o cold_storage
