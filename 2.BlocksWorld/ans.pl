/*
ismemberhelp([H|T],M):-
	H == M.
ismemberhelp([H|T],M):-
	ismemberhelp(T,M).

ismember(List,M):-
	dif(List,[]),
	ismemberhelp(List,M).

getmember([H|T],M):-
	dif([H|T],[]),
	M is H.

getmember([H|T],M):-
	getmember(T,M).

*/

ismember([M|T],M).
ismember([A|B],M):-
	dif(A,[]),
	ismember(B,M).

/* If pending goals empty return true*/
satisfied(CurState,[]).

/* If pending goals empty return true*/
satisfied(CurState,[H|RGoals]):-
	ismember(CurState,H), 
	satisfied(CurState,RGoals).

find_operation_forcurstate(pu(X), [ae,clear(X),ontable(X)], [hold(X)], [ontable(X),ae]).
find_operation_forcurstate(pd(X), [hold(X)], [ontable(X),ae], [hold(X)]).
find_operation_forcurstate(stack(A,B), [clear(B),hold(A)], [ae,on(A,B)], [clear(B),hold(A)]).
find_operation_forcurstate(unstack(A,B), [ae,clear(A),on(A,B)], [hold(A),clear(B)], [on(A,B),ae]).

ispossible(stack(A,B)):- A\==B.
ispossible(unstack(A,B)):- A\==B.
ispossible(_).

remove(State,[],State).
remove(State,[H|T],NewState) :- 
	select(H,State,Temp), 
	remove(Temp,T,NewState).

applyaction(X,MediumState,NewCurState,Del,Add):-
	remove(MediumState,Del,TempState),
	append(TempState,Add,NewCurState).




/* If Currently there is no plan then we check if already all goals are satisfied in CurState*/
giveplan(CurState,CurState, PendingGoals, []):- 
	satisfied( CurState, PendingGoals). 

giveplan(CurState,FinalState,PendingGoals, PlanList):-
	ismember(PendingGoals,X),
	not(ismember(CurState,X)),
	find_operation_forcurstate(Oprn,Precond,Add,Del),
	ismember(Add,X),
	ispossible(X),
	giveplan(CurState,MediumState,Precond,Plan_for_precondition),
	applyaction(X,MediumState,NewCurState,Del,Add),
	giveplan(NewCurState,FinalState,PendingGoals,Future_plan),
	append(Plan_for_precondition,[X|Future_plan],PlanList).



/* giveplan([ontable(a),ontable(b),clear(a),clear(b),ae],F,[ontable(b),on(a,b),clear(a),ae],P).*/

/* giveplan([ontable(a),ontable(b),clear(c),clear(b),on(c,a),ae],F,[ontable(c),on(b,c),on(a,b),clear(a),ae],P). */

/*
giveplan([ontable(a),ontable(b),clear(c),clear(b),on(c,a),ae],F,[ontable(b),on(c,b),on(a,c),clear(a),ae],P). 
*/