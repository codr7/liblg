### intro
The goal of [liblg](https://github.com/codr7/liblg) is to create a reasonably fast, minimal stack-based VM in C. It currently implements just enough functionality to do recursive Fibonacci. The general idea is that it's potentially useful to have access to stack vm functionality in library form, for implementing custom DSLs on top etc.

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
148810us
```

### benchmarks
The VM is currently significantly faster than Python3 and around 10 times as slow as Go, ideas on how to improve its performance further without making a mess are most welcome.


```
$ cd bench
$ python3 fibrec.py
353813us
$ go run fibrec.go
14170us
```

```
$ cd build
$ make lgasm
$ cd ..
$ build/lgasm --bench 100 bench/fibtail.lga
147us
$ cd ../bench
$ python3 fibtail.py
543us
$ go run fibtail.go
9us
```

### design
The core loop uses computed goto, which means that new instructions must be added in identical order [here](https://github.com/codr7/liblg/blob/master/src/lg/op.h) and [here](https://github.com/codr7/liblg/blob/master/src/lg/vm.c).

Fundamental types are global (as in not tied to a specific VM instance), new types may be added [here](https://github.com/codr7/liblg/tree/master/src/lg/types) and initialized [here](https://github.com/codr7/liblg/blob/master/src/lg/init.c).

[Values](https://github.com/codr7/liblg/blob/master/src/lg/val.h) are represented as tagged unions.

### assembler
[liblg](https://github.com/codr7/liblg) comes equipped with a simple custom [assembler](https://github.com/codr7/liblg/tree/master/bench/fibtail.lga) that provides a transparent interface to it's functionality.

#### add
Pop and add the top of the stack to the previous item.

#### biq $offset $condition $label
Branch to $label if the stack value att $offset is equal to $condition.

#### call $label
Push the program counter on the call stack and jump to $label.

#### cp $offset
Push a copy of the value at stack $offset.

#### dec $offset
Decrease the value att stack $offset by one.

#### drop $offset $count
Drop $count items from stack $offset.

#### jmp $label
Jump to $label.

#### push $value
Push $value on the stack.

#### ret
Return from the current call.

#### swap
Swap the top two items on the stack.

### license
[MIT](https://github.com/codr7/liblg/blob/master/LICENSE.txt)