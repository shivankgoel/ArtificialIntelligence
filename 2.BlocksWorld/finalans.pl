:- dynamic on/2.
:- dynamic hold/1.
:- dynamic ontable/1.
:- dynamic clear/1.
:- dynamic ae/0.


/* 
ismember predicate matches with any object present in the list. 
If the element is not present in the list then it holds false. 
*/

ismember([M|T],M).
ismember([A|B],M):-
	dif(A,[]),
	ismember(B,M).

/* 
Satisfied checks of all goals are already satsified in current state.
In short it checks whether right list is sublist of left list.
*/

satisfied(CurState,[]).

satisfied(CurState,[H|RGoals]):-
	ismember(CurState,H), 
	satisfied(CurState,RGoals).

/*
ispossible just does sanity check so that any block is not 
tried to be stacked/unstacked on itself. 
*/

ispossible(stack(A,B)):- A\==B.
ispossible(unstack(A,B)):- A\==B.
ispossible(_).

/*
remove(A,B,C) removes list B from list A and stores in list C.
*/

remove(State,[],State).
remove(State,[H|T],NewState) :- 
	select(H,State,Temp), 
	remove(Temp,T,NewState).

/*
Applyaction removes the Del list from current state list,
adds the Add list to current state list and store in NewCurState.
*/

applyaction(MediumState,NewCurState,Del,Add):-
	remove(MediumState,Del,TempState),
	append(TempState,Add,NewCurState).

/*
assign_X predicates are helper predicates for give_oprn,
It helps in matching relevant action for a given goal.
*/

assign_unstack(A,B,[unstack(A,B), [ae,clear(A),on(A,B)], [hold(A),clear(B)], [on(A,B),ae]]).
assign_putdown(X,[pd(X), [hold(X)], [ontable(X),ae], [hold(X)]]).
assign_stack(A,B,[stack(A,B), [clear(B),hold(A)], [ae,on(A,B)], [clear(B),hold(A)]]).

/*
give_oprn predicate helps in selecting relevant operation based on 
currentstate and goal to be achieved. We have used various heuristics here. 
*/

give_oprn(Z,List,CurState):-
	dif(Z,ae),
	not(ismember(CurState,ae)),
	give_oprn(ae,List,CurState).

give_oprn(on(A,B),[stack(A,B), [clear(B),hold(A)], [ae,on(A,B)], [clear(B),hold(A)]],CurState).

give_oprn(ontable(X),List,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(ontable(X),[pd(X), [hold(X)], [ontable(X),ae], [hold(X)]],CurState).

give_oprn(clear(X),List,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(hold(X),List,CurState):-
	ismember(CurState,on(Y,X)),
	assign_unstack(Y,X,List).

give_oprn(hold(X),List,CurState):-
	ismember(CurState,on(X,Y)),
	assign_unstack(X,Y,List).

give_oprn(hold(X),[pu(X),[ae,clear(X),ontable(X)], [hold(X)], [ontable(X),ae]],CurState).

give_oprn(ae,List,CurState):-
	ismember(CurState,hold(X)),
	assign_putdown(X,List).



/* 
giveplan predicate tries to achieve all the goals and returns a plan to achieve all the goals.
it's kind of main predicate which are using all other predicates to achieve goal.

It checks if all goals are already satisfied and if not, it finds a goal which is not satisfied,
and tries to satisfy it by finding relevent operation, and in order to do that operation, it 
then tries to satisfy it's preconditions and so on. 
*/

giveplan(CurState,CurState, PendingGoals, []):- 
	satisfied( CurState, PendingGoals). 

giveplan(CurState,FinalState,PendingGoals, PlanList):-
	dif(FinalState,[]),
	dif(PlanList,[]),
	ismember(PendingGoals,X),
	not(ismember(CurState,X)),
	give_oprn(X,[Oprn,Precond,Add,Del],CurState),
	ispossible(Oprn),
	giveplan(CurState,MediumState,Precond,Plan_for_precondition),
	applyaction(MediumState,NewCurState,Del,Add),
	giveplan(NewCurState,FinalState,PendingGoals,Future_plan),
	append(Plan_for_precondition,[Oprn|Future_plan],PlanList).

/*
subsequence(A,B) checks if A is a continuous subsequence of B, 
given size of A is two. It's helper function for repair_plan.
*/

subsequence([], _).   
subsequence([X,Y|T1], [X,Y|T2]).
subsequence(A, [_|T2]) :- subsequence(A, T2).

/*
try_remove(A,B,C) subtracts A from B if B is subsequence , and stores in C.
Else it just copies A to C. It's hlper function for repair_plan. 
*/

try_remove(SubseqList,CurList,AnsList):-
	subsequence(SubseqList,CurList),
	remove(CurList,SubseqList,AnsList).

try_remove(_,CurList,CurList).

/*
repair_plan_help tries to remove all do and undo pairs from plan.
*/
repair_plan_help(PlanList,AnsList):-
	try_remove([stack(A,B),unstack(A,B)],PlanList,Temp1),
	try_remove([unstack(A,B),stack(A,B)],Temp1,Temp2),
	try_remove([pd(A),pu(A)],Temp2,Temp3),
	try_remove([pu(A),pd(A)],Temp3,AnsList).


/*
Decide_further decided if we need to try repairing the plan again.
If in previous try, there is no change then we need not try again.
*/

decide_further(PlanList,Temp1,NewList):-
	dif(PlanList,Temp1),
	repair_plan(Temp1,NewList).

decide_further(PlanList,Temp1,PlanList).


/*
repair_plan repairs PlanList and stores in NewList. 
It gets a plan in Temp1 using repair_plan_help and checks if we need repair_further.
*/
repair_plan(PlanList,NewList):-
	repair_plan_help(PlanList,Temp1),
	decide_further(PlanList,Temp1,NewList).


/*
start_function gets a TempPLan from giveplan and then calls repair_plan on it.
*/
start_function(CurState,FinalState,PendingGoals, PlanList):-
	giveplan(CurState,FinalState,PendingGoals, TempPlanList),
	repair_plan(TempPlanList,PlanList).
