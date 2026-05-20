package bug1

import "sync"

// Counter stores a count.
type Counter struct {
	n  int64
	mu sync.Mutex
}

func (c *Counter) Inc() {
	c.mu.Lock()
	c.n++
	c.mu.Unlock()
}
