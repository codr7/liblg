package main

import (
	"fmt"
	"time"
)

func fib(n int) int {
	if n < 2 {
		return n
	}

	return fib(n-1) + fib(n-2)
}

func main() {
	t := time.Now()
	
	for i := 0; i < 100; i++ {
		if (fib(20) != 6765) {
			panic("Wrong result")
		}
	}

	fmt.Printf("%vus\n", time.Since(t).Microseconds())
}
