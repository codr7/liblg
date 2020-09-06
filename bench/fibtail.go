package main

import (
	"fmt"
	"time"
)

func fib(n, a, b int) int {
	if n == 0 {
		return a;
	}

	if n == 1 {
		return b;
	}
	
	return fib(n-1, b, a+b)
}

func main() {
	t := time.Now()
	
	for i := 0; i < 100; i++ {
		if (fib(20, 0, 1) != 6765) {
			panic("Wrong result")
		}
	}

	fmt.Printf("%vus\n", time.Since(t).Microseconds())
}
