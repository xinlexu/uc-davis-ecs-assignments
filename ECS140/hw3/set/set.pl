/*combine two lists tehn remove duplicates */
isUnion(Set1, Set2, Union) :-
    append(Set1, Set2, TempUnion),
    rm_duplicates(TempUnion, Union).

/*remove duplicates */
rm_duplicates([], []).
rm_duplicates([H|T], R) :-
    member(H, T), !,
    rm_duplicates(T, R).
rm_duplicates([H|T], [H|R]) :-
    rm_duplicates(T, R).

/*get common elements from two lists */
isIntersection([], _, []).
isIntersection([H|T], Set2, [H|Intersection]) :-
    member(H, Set2), !,
    isIntersection(T, Set2, Intersection).
isIntersection([_|T], Set2, Intersection) :-
    isIntersection(T, Set2, Intersection).

/*check two lists have same elements */
isEqual(Set1, Set2) :-
    subset(Set1, Set2),
    subset(Set2, Set1).

/*check first list elements are all in second list */
subset([], _).
subset([H|T], Set) :-
    member(H, Set),
    subset(T, Set).
