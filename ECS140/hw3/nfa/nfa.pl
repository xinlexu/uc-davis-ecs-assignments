:- [transitions].

/*if input empty then start needs to be final */
reachable(State, State, []).

/*if symbols left then process each */
reachable(StartState, FinalState, [Symbol|Rest]) :-
    transition(StartState, Symbol, NextStates),  % get next states for symbol
    member(NextState, NextStates),               % check each next state
    reachable(NextState, FinalState, Rest).      % recurse to check rest
