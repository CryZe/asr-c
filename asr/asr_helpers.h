#pragma once

#include "asr.h"

/// Sets a custom key value pair. This may be arbitrary information that
/// the auto splitter wants to provide for visualization.
void timer_set_variable_cstr(char const *key, char const *value);

/// Sets the game time.
void timer_set_game_time_seconds_f32(float seconds);

/// Attaches to a process based on its name.
ProcessId process_attach_cstr(char const *name);
/// Gets the address of a module in a process.
Address process_get_module_address_cstr(ProcessId process, char const *name);
bool process_read_pointer_path64(
    ProcessId process,
    Address address,
    uint64_t *offsets_arr, size_t offsets_len,
    void *dst_ptr, size_t dst_len);

/// Prints a log message for debugging purposes.
void runtime_print_message_cstr(char const *text);

typedef struct {
    char buf[15];
    uint8_t start;
} IntBuf;

IntBuf IntBuf_from_u32(uint32_t x);
static inline size_t IntBuf_len(IntBuf* buf) {
    return 15 - buf->start;
}
void IntBuf_pad(IntBuf* buf, char c, size_t len);
char* IntBuf_copy_into(IntBuf* buf, char* dst_start, char* dst_end);
