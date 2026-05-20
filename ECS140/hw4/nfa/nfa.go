package nfa

import (
	"sync"
)

// A nondeterministic Finite Automaton (NFA) consists of states,
// symbols in an alphabet, and a transition function.

// A state in the NFA is represented as an unsigned integer.
type state uint

// Given the current state and a symbol, the transition function
// of an NFA returns the set of next states the NFA can transition to
// on reading the given symbol.
// This set of next states could be empty.
type TransitionFunction func(st state, sym rune) []state

// Reachable returns true if there exists a sequence of transitions
// from `transitions` such that if the NFA starts at the start state
// `start` it would reach the final state `final` after reading the
// entire sequence of symbols `input`; Reachable returns false otherwise.
func Reachable(
	// `transitions` tells us what our NFA looks like
	transitions TransitionFunction,
	// `start` and `final` tell us where to start, and where we want to end up
	start, final state,
	// `input` is a (possibly empty) list of symbols to apply.
	input []rune,
) bool {
	currentStates := []state{start}
	var wg sync.WaitGroup
	for _, symbol := range input {
		nextStateMap := make(map[state]bool)
		stateChannel := make(chan state, 100)
		for _, st := range currentStates {
			wg.Add(1)
			go func(s state) {
				defer wg.Done()
				for _, next := range transitions(s, symbol) {
					stateChannel <- next
				}
			}(st)
		}
		wg.Wait()
		close(stateChannel)
		for st := range stateChannel {
			nextStateMap[st] = true
		}
		currentStates = make([]state, 0, len(nextStateMap))
		for st := range nextStateMap {
			currentStates = append(currentStates, st)
		}
		if len(currentStates) == 0 {
			return false
		}
	}
	for _, st := range currentStates {
		if st == final {
			return true
		}
	}
	return false
}
