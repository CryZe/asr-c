#include "asr_helpers.h"
#include "mini_libc.h"

void timer_set_variable_cstr(char const *key, char const *value) {
    timer_set_variable(key, strlen(key), value, strlen(value));
}

void timer_set_game_time_seconds_f32(float seconds) {
    int64_t truncated = seconds;
    timer_set_game_time(truncated, (seconds - truncated) * 1000000000.0f);
}

ProcessId process_attach_cstr(char const *name) {
    return process_attach(name, strlen(name));
}

Address process_get_module_address_cstr(ProcessId process, char const *name) {
    return process_get_module_address(process, name, strlen(name));
}

bool process_read_pointer_path64(
    ProcessId process,
    Address address,
    uint64_t* offsets_arr, size_t offsets_len,
    void* dst_ptr, size_t dst_len
) {
    if (!offsets_len) {
        return false;
    }
    uint64_t last = offsets_arr[offsets_len - 1];
    for (size_t i = 0; i < offsets_len - 1; i++) {
        bool worked = process_read(process, address + offsets_arr[i], &address, sizeof(address));
        if (!worked) {
            return false;
        }
    }
    return process_read(process, address + last, dst_ptr, dst_len);
}

void runtime_print_message_cstr(char const *text) {
    runtime_print_message(text, strlen(text));
}

IntBuf IntBuf_from_u32(uint32_t x) {
    IntBuf int_buf;
    char *p = &int_buf.buf[15];
    do {
        *--p = '0' + x % 10;
		x /= 10;
    } while (x);
    int_buf.start = p - (&int_buf.buf[0]);
    return int_buf;
}

void IntBuf_pad(IntBuf* buf, char c, size_t len) {
    if (len > 15) {
        len = 15;
    }
    char* p = (&buf->buf[15]) - len;
    char* end = &buf->buf[buf->start];
    memset(p, c, end - p);
    buf->start = 15 - len;
}

char* IntBuf_copy_into(IntBuf* buf, char* dst_start, char* dst_end) {
    size_t src_len = IntBuf_len(buf);
    size_t dst_len = dst_end - dst_start;
    size_t len = src_len < dst_len ? src_len : dst_len;
    return memcpy(dst_start, &buf->buf[buf->start], len);
}
