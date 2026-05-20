package smash

import (
	"bufio"
	"io"
	"sync"
)

type word string

func Smash(r io.Reader, smasher func(word) uint32) map[uint32]uint {
	var wg sync.WaitGroup
	m := make(map[uint32]uint)
	mu := sync.Mutex{}
	scanner := bufio.NewScanner(r)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		wg.Add(1)
		go func(w word) {
			defer wg.Done()
			hashed := smasher(w)
			mu.Lock()
			m[hashed]++
			mu.Unlock()
		}(word(scanner.Text()))
	}

	wg.Wait()
	if err := scanner.Err(); err != nil {
		panic("reading input failed")
	}

	return m
}
