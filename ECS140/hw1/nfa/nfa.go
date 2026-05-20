package nfa

// A state in the NFA is labeled by a single integer.
type state uint

// TransitionFunction tells us, given a current state and some symbol, which
// other states the NFA can move to.
// Deterministic automata have only one possible destination state,
// but we're working with non-deterministic automata.
type TransitionFunction func(st state, act rune) []state

func Reachable(transitions TransitionFunction, start, final state, input []rune) bool {
	if len(input) == 0 {
		return start == final
	}
	nextStates := transitions(start, input[0])
	for _, nextState := range nextStates {
		if Reachable(transitions, nextState, final, input[1:]) {
			return true
		}
	}
	return false
}
