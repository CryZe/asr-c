#include "asr/asr_helpers.h"
#include "asr/mini_libc.h"

ProcessId process = 0;
Address unity = 0;

uint8_t old_run_active = 0;
uint8_t old_timer_paused = 0;

typedef struct {
    float minutes;
    float seconds;
    float fraction;
} Digits;

typedef struct {
    float level_timer;
    float run_timer;
    Digits level_timer_digits;
    Digits run_timer_digits;
    uint8_t timer_stopped;
    uint8_t timer_paused;
    uint8_t run_active;
    uint8_t speedrun;
    uint32_t character;
} TimeInfo;

__attribute__((export_name("update"))) void update(void)
{
    if (!process) {
        process = process_attach_cstr("Eversleep Lunistice Gaiden.exe");
        if (!process) {
            return;
        }
    }
    if (!process_is_open(process)) {
        process_detach(process);
        process = 0;
        unity = 0;
        return;
    }
    if (!unity) {
        unity = process_get_module_address_cstr(process, "UnityPlayer.dll");
        if (!unity) {
            return;
        }
    }

    TimeInfo time_info;
    if (!process_read_pointer_path64(
        process,
        unity,
        (uint64_t[]) {0x01A26D60, 0x128, 0x8, 0x80, 0x38}, 5,
        &time_info, sizeof(time_info)
    )) {
        return;
    }

    char level_time[33];
    char* p = &level_time[0];
    char* p_end = &level_time[30];
    IntBuf int_buf = IntBuf_from_u32(time_info.level_timer_digits.minutes);
    p = IntBuf_copy_into(&int_buf, p, p_end);
    *p++ = ':';
    int_buf = IntBuf_from_u32(time_info.level_timer_digits.seconds);
    IntBuf_pad(&int_buf, '0', 2);
    p = IntBuf_copy_into(&int_buf, p, p_end);
    *p++ = '.';
    int_buf = IntBuf_from_u32(time_info.level_timer_digits.fraction);
    IntBuf_pad(&int_buf, '0', 2);
    p = IntBuf_copy_into(&int_buf, p, p_end);
    *p++ = '\0';

    timer_set_variable_cstr("Level Time", level_time);

    timer_set_variable_cstr(
        "Character",
        time_info.character == 0
            ? "Hana"
            : time_info.character == 1
                ? "Toree"
                : "Unknown"
    );

    switch (timer_get_state()) {
        case NOT_RUNNING: {
            if (old_run_active == 0 && time_info.run_active != 0) {
                timer_start();
                timer_pause_game_time();
            }
            break;
        }
        case PAUSED:
        case RUNNING: {
            if (old_run_active != 0 && time_info.run_active == 0) {
                timer_reset();
            } else if (old_timer_paused == 0 && time_info.timer_paused != 0) {
                timer_split();
            }
            timer_set_game_time_seconds_f32(time_info.run_timer);
            break;
        }
        default: {}
    }

    old_run_active = time_info.run_active;
    old_timer_paused = time_info.timer_paused;
}
