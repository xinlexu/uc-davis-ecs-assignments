package bug2

import (
	"sync"
)

func bug2(n int, foo func(int) int, out chan int) {
	var wg sync.WaitGroup
	for i := 0; i < n; i++ {
		wg.Add(1)
		go func(x int) {
			out <- foo(x)
			wg.Done()
		}(i)
	}
	wg.Wait()
	close(out)
}
