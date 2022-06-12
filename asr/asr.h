#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum TimerState
{
    /// The timer is not running.
    NOT_RUNNING = 0,
    /// The timer is running.
    RUNNING = 1,
    /// The timer started but got paused. This is separate from the game
    /// time being paused. Game time may even always be paused.
    PAUSED = 2,
    /// The timer has ended, but didn't get reset yet.
    ENDED = 3,
} TimerState;

typedef uint64_t ProcessId;
typedef uint64_t Address;

/// Gets the state that the timer currently is in.
TimerState timer_get_state(void);

/// Starts the timer.
void timer_start(void);
/// Splits the current segment.
void timer_split(void);
/// Resets the timer.
void timer_reset(void);
/// Sets a custom key value pair. This may be arbitrary information that
/// the auto splitter wants to provide for visualization.
void timer_set_variable(char const *key_ptr, size_t key_len, char const *value_ptr, size_t value_len);

/// Sets the game time.
void timer_set_game_time(int64_t secs, int32_t nanos);
/// Pauses the game time. This does not pause the timer, only the
/// automatic flow of time for the game time.
void timer_pause_game_time(void);
/// Resumes the game time. This does not resume the timer, only the
/// automatic flow of time for the game time.
void timer_resume_game_time(void);

/// Attaches to a process based on its name. Returns 0 if the process can't be
/// found.
ProcessId process_attach(char const *name_ptr, size_t name_len);
/// Detaches from a process.
void process_detach(ProcessId process);
/// Checks whether is a process is still open. You should detach from a
/// process and stop using it if this returns `false`.
bool process_is_open(ProcessId process);
/// Reads memory from a process at the address given. This will write
/// the memory to the buffer given. Returns `false` if this fails.
bool process_read(ProcessId process, Address address, void *buf_ptr, size_t buf_len);
/// Gets the address of a module in a process.
Address process_get_module_address(ProcessId process, char const *name_ptr, size_t name_len);

/// Sets the tick rate of the runtime. This influences the amount of
/// times the `update` function is called per second.
void runtime_set_tick_rate(double ticks_per_second);
/// Prints a log message for debugging purposes.
void runtime_print_message(char const *text_ptr, size_t text_len);
