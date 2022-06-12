CFLAGS = -target wasm32 -O3

script.wasm: script.o
	wasm-ld-10 --no-entry --allow-undefined-file=asr/asr.syms script.o malloc.o mini_libc.o asr_helpers.o -o script.wasm

asr_helpers.o: asr/asr_helpers.h asr/asr_helpers.c asr/asr.h mini_libc.o
	clang $(CFLAGS) -c asr/asr_helpers.c

malloc.o: asr/malloc.h asr/malloc.c
	clang $(CFLAGS) -c asr/malloc.c

mini_libc.o: asr/mini_libc.h asr/mini_libc.c
	clang $(CFLAGS) -c asr/mini_libc.c

script.o: script.c malloc.o asr_helpers.o
	clang $(CFLAGS) -c script.c
