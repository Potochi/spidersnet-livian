build:
	gcc \
		-s \
		-no-pie \
		-fno-pic \
		-Wno-unused-parameter \
		-Wno-builtin-declaration-mismatch \
		-Wno-main \
		-fno-stack-protector \
		-masm=intel \
		-fomit-frame-pointer \
		-fno-exceptions \
		-fno-asynchronous-unwind-tables \
		-fno-unwind-tables \
		-Wall \
		-Wextra \
		-nostartfiles \
		sirop.c -o sirop
