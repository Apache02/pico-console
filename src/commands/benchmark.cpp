#include "commands.h"

#include <FreeRTOS.h>
#include <task.h>

#include <stdio.h>
#include "pico/time.h"


template<typename T>
static void bench_mul(size_t count) {
    volatile T a = 12345679.01,
            b = 8.1;

    for (size_t i = 0; i < count; i++) {
        volatile T c = a * b;
    }
}

template<typename T>
static void bench_div(size_t count) {
    volatile T a = 12345679.01,
            b = 8.1;

    for (size_t i = 0; i < count; i++) {
        volatile T c = a / b;
    }
}

static const char tbl_header[] = "%12s | %12s | %14s | %10s \n";
static const char tbl_divider[] = "----------------------------------------------------------\n";

static void print_bench_tbl_header() {
    printf("\n");
    printf(tbl_header, "Description", "Count", "Duration (us)", "FLOPS");
    printf(tbl_divider);
}

static void print_bench_tbl_row(const char *type, size_t count, uint64_t duration) {
    char s_count[32];
    char s_duration[32];
    char s_flops[32];

    sprintf(s_count, "%d", count);
    sprintf(s_duration, "%lld", duration);
    sprintf(s_flops, "%.2f", float(count) * 1000000.0 / float(duration));

    printf(tbl_header, type, s_count, s_duration, s_flops);
}

void command_benchmark(Console &c) {
    print_bench_tbl_header();

    static struct {
        void (*func)(size_t);

        size_t count;
        const char *description;
    } benchmark_configs[] = {
            {&(bench_mul<float>),  100000,  "mul float"},
            {&(bench_mul<float>),  500000,  "mul float"},
            {&(bench_mul<float>),  1000000, "mul float"},
            {&(bench_mul<float>),  2000000, "mul float"},
            {&(bench_mul<float>),  4000000, "mul float"},
            // ---
            {&(bench_mul<double>), 100000,  "mul double"},
            {&(bench_mul<double>), 500000,  "mul double"},
            {&(bench_mul<double>), 1000000, "mul double"},
            {&(bench_mul<double>), 2000000, "mul double"},
            {&(bench_mul<double>), 4000000, "mul double"},
            // ---
            {&(bench_div<float>),  100000,  "div float"},
            {&(bench_div<float>),  500000,  "div float"},
            {&(bench_div<float>),  1000000, "div float"},
            {&(bench_div<float>),  2000000, "div float"},
            {&(bench_div<float>),  4000000, "div float"},
    };

    for (auto[func, count, description]: benchmark_configs) {
        taskENTER_CRITICAL();
        absolute_time_t time_before = get_absolute_time();
        func(count);
        absolute_time_t time_after = get_absolute_time();
        taskEXIT_CRITICAL();

        print_bench_tbl_row(description, count, absolute_time_diff_us(time_before, time_after));

        taskYIELD();
    }

    printf(tbl_divider);
    printf("\n");
}
