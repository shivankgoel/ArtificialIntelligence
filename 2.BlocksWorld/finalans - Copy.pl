:- dynamic on/2.
:- dynamic hold/1.
:- dynamic ontable/1.
:- dynamic clear/1.
:- dynamic ae/0.

set_prolog_flag(answer_write_options,[max_depth(0)]).


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

applyaction(MediumState,NewCurState,Del,Add):-
	remove(MediumState,Del,TempState),
	append(TempState,Add,NewCurState).

assign_unstack(A,B,[unstack(A,B), [ae,clear(A),on(A,B)], [hold(A),clear(B)], [on(A,B),ae]]).
assign_putdown(X,[pd(X), [hold(X)], [ontable(X),ae], [hold(X)]]).
assign_stack(A,B,[stack(A,B), [clear(B),hold(A)], [ae,on(A,B)], [clear(B),hold(A)]]).

give_oprn(Z,List,PendingGoals,CurState):-
	dif(Z,ae),
	not(ismember(CurState,ae)),
	give_oprn(ae,List,PendingGoals,CurState).

give_oprn(on(A,B),[stack(A,B), [clear(B),hold(A)], [ae,on(A,B)], [clear(B),hold(A)]],PendingGoals,CurState).

give_oprn(ontable(X),List,PendingGoals,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(ontable(X),[pd(X), [hold(X)], [ontable(X),ae], [hold(X)]],PendingGoals,CurState).

give_oprn(clear(X),List,PendingGoals,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(hold(X),List,PendingGoals,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(hold(X),List,PendingGoals,CurState):-
	ismember(CurState,on(X,Y)),
	assign_unstack(X,Y,List).

give_oprn(hold(X),[pu(X),[ae,clear(X),ontable(X)], [hold(X)], [ontable(X),ae]],PendingGoals,CurState).

/*
give_oprn(ae,List,PendingGoals):-
	hold(X),
	call(hold(X)),
	ismember(PendingGoals,on(X,Y)),
	clear(Y),
	assign_stack(X,Y,List).
*/


give_oprn(ae,List,PendingGoals,CurState):-
	ismember(CurState,hold(X)),
	assign_putdown(X,List).


assert_all([]).
assert_all([H|T]):-
	assert(H),
	assert_all(T).

retract_all([]).
retract_all([H|T]):-
	retract(H),
	retract_all(T).
	



/* If Currently there is no plan then we check if already all goals are satisfied in CurState*/
giveplan(CurState,CurState, PendingGoals, []):- 
	satisfied( CurState, PendingGoals). 

giveplan(CurState,FinalState,PendingGoals, PlanList):-
	dif(FinalState,[]),
	dif(PlanList,[]),
	ismember(PendingGoals,X),
	not(ismember(CurState,X)),
	give_oprn(X,[Oprn,Precond,Add,Del],PendingGoals,CurState),
	ispossible(Oprn),
	giveplan(CurState,MediumState,Precond,Plan_for_precondition),
	applyaction(MediumState,NewCurState,Del,Add),
	assert_all(Add),
	retract_all(Del),
	giveplan(NewCurState,FinalState,PendingGoals,Future_plan),
	append(Plan_for_precondition,[Oprn|Future_plan],PlanList).


subsequence([], _).   
subsequence([X,Y|T1], [X,Y|T2]).
subsequence(A, [_|T2]) :- subsequence(A, T2).

try_remove(SubseqList,CurList,AnsList):-
	subsequence(SubseqList,CurList),
	remove(CurList,SubseqList,AnsList).

try_remove(_,CurList,CurList).

repair_plan_help(PlanList,AnsList):-
	try_remove([stack(A,B),unstack(A,B)],PlanList,Temp1),
	try_remove([unstack(A,B),stack(A,B)],Temp1,Temp2),
	try_remove([pd(A),pu(A)],Temp2,Temp3),
	try_remove([pu(A),pd(A)],Temp3,AnsList).
	
decide_further(PlanList,Temp1,NewList):-
	dif(PlanList,Temp1),
	repair_plan(Temp1,NewList).

decide_further(PlanList,Temp1,PlanList).

repair_plan(PlanList,NewList):-
	repair_plan_help(PlanList,Temp1),
	decide_further(PlanList,Temp1,NewList).


start_function(CurState,FinalState,PendingGoals, PlanList):-
	assert_all(CurState),
	giveplan(CurState,FinalState,PendingGoals, TempPlanList),
	repair_plan(TempPlanList,PlanList).


/*
start_function(CurState,FinalState,PendingGoals, PlanList):-
	assert_all(CurState),
	giveplan(CurState,FinalState,PendingGoals, PlanList).
*/


/* start_function([ontable(a),ontable(b),clear(a),clear(b),ae],F,[ontable(b),on(a,b),clear(a),ae],P).*/

/* start_function([ontable(a),ontable(b),clear(c),clear(b),on(c,a),ae],F,[ontable(c),on(b,c),on(a,b),clear(a),ae],P). */

/*
start_function([ontable(a),ontable(b),clear(c),clear(b),on(c,a),ae],F,[ontable(b),on(c,b),on(a,c),clear(a),ae],P). 
*/

/*

start_function(
[ontable(a),on(b,a),on(c,b),clear(c),ontable(d),clear(d),ae],
F,
[ontable(d),on(c,d),on(b,c),on(a,b),clear(a),ae],
P
).

*/

/*
start_function(
[ontable(a),on(c,a),clear(c),ontable(b),clear(b),ae],
F,
[on(a,b),on(b,c)],
P
).

*/

/*
start_function(
[ontable(a),on(c,a),clear(d),ontable(b),clear(b),on(d,c),ae],
F,
[on(a,b),on(b,d),on(d,c),ontable(c),clear(a)],
P
).

*/


/*
start_function(
[ontable(a),on(c,a),clear(d),ontable(b),clear(b),on(d,c),ae],
F,
[ontable(c),on(d,c),on(b,d),on(a,b),clear(a),ae],
P
).

*/