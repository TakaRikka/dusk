const char* __asan_default_options(void) {
    return "abort_on_error=1:symbolize=1:intercept_memcmp=0:detect_leaks=0";
}
