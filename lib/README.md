# Tedit Libraries
Tedit is aiming to not use libc where possible, and simply creating a thin
wrappers around zig APIs. This content (called `libzig`) should live under `libzig`.

Since I will also not have access to the C++ stdlib either, I will also have to
roll my own (not the whole thing and not 100% standard compliant either). Much
how like libc++ will wrap C/OS APIs, my C++ library will have to wrap the APIs
I expose in `libzig`. This c++ library will live under `libcpp`
