### intro
The goal of [liblg](https://github.com/codr7/liblg) is to create a reasonably fast, minimal stack-based VM in C. It implements just enough functionality to do recursive Fibonacci (with tail call optimization) and comes equipped with a custom assembler. The general idea is that it's very useful to have access to extensible core stack vm functionality in library form, for implementing custom DSLs on top etc.

### setup
[liblg](https://github.com/codr7/liblg) requires a C compiler and CMake to build.

```
$ git clone https://github.com/codr7/liblg.git
$ cd liblg
$ mkdir build
$ cd build
$ cmake ..
$ make fibrec
$ ./fibrec
193128us
```

### design
The core loop uses computed goto, which means that new instructions must be added in identical order [here](https://github.com/codr7/liblg/blob/master/src/lg/op.h) and [here](https://github.com/codr7/liblg/blob/master/src/lg/vm.c).

Fundamental types are global (as in not tied to a specific VM instance), new types may be added [here](https://github.com/codr7/liblg/tree/master/src/lg/types) and initialized [here](https://github.com/codr7/liblg/blob/master/src/lg/init.c).

[Values](https://github.com/codr7/liblg/blob/master/src/lg/val.h) are represented as tagged unions.

### assembler
[liblg](https://github.com/codr7/liblg) comes equipped with a custom [assembler](https://github.com/codr7/liblg/tree/master/bench/fibtail.lga) syntax that provides a transparent interface to it's functionality.

### benchmarks
The VM is currently around 40% faster than Python3 and 14 times as slow as Go, ideas on how to make it run significantly faster without making a mess are most welcome.

```
$ cd bench
$ python3 fib.py
353813us
$ go run fib.go
14170us
```

### license
[MIT](https://github.com/codr7/liblg/blob/master/LICENSE.txt)