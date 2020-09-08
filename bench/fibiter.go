package main

import (
	"fmt"
	"time"
)

func fib(n int) int {
	a, b := 0, 1
	
	for i := 0; i < n; i++ {
		a, b = b, a+b
	}
	
	return a
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
