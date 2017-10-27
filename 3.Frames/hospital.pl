frame(hospital,[nil,country(default("India")),phone("No Default Phone"),address("No Default Address"),labs(["No Default Labs"]),wards(["No Default Wards"]),doctors(["No Default Doctors"])]).
frame(child_hospital,[ako(hospital),age(value(0,12),demon(if_needed))]).
frame(heart_hospital,[ako(hospital)]).
frame(lab,[ part_of(hospital), pathologies(["No Default Pathologies"]), x_rays(["No Default XRays"]) ] ).
frame(pathology,[ part_of(lab), incharge("No Default Incharge"),tests(["No Default Tests"]) ] ).
frame(x_ray,[ part_of(lab), incharge("No Default Incharge") ] ).

frame(aiims,[inst(hospital),phone(value(9871849039)),address(value("Central Delhi")),labs([aiims_lab]),wards([aiims_ward]),doctors(["A","B","C"])]).
frame(aiims_lab,[inst(lab),part_of(aiims),pathologies([aiims_pahology]),x_rays([aiims_x_ray])]).
frame(aiims_pathology,[part_of(aiims_lab),incharge(value("Karan")),tests([ecg, blood_test, urine])).
frame(aiims_x_ray,[part_of(aiims_lab),incharge(value("Shivank"))]).

frame(kalawati,[inst(child_hospital),phone(value(9871849039)),address(value("1021 new delhi")),dirctor(value("Dr. Mike"))]).

frame(escort,[inst(heart_hospital),phone(value(9871849039)),address(value("faridabad"))]).



giveslotnamevalue(country(default(X)),country,X).
giveslotnamevalue(country(value(X)),country,X).
giveslotnamevalue(phone(default(X)),phone,X).
giveslotnamevalue(phone(value(X)),phone,X).
giveslotnamevalue(address(default(X)),address,X).
giveslotnamevalue(address(value(X)),address,X).
giveslotnamevalue(incharge(value(X)),incharge,X).

/*
X are lists 
*/

giveslotnamevalue(wards(X),wards,X).
giveslotnamevalue(labs(X),labs,X).
giveslotnamevalue(pathologies(X),pathologies,X).
giveslotnamevalue(x_rays(X),x_rays,X).
giveslotnamevalue(tests(X),tests,X).
giveslotnamevalue(doctors(X),doctors,X).


search(Y,[],Svalue):-fail,!.

search(Y,[M|N],Svalue):-
	giveslotnamevalue(M,Name,Svalue),
	Name == Y,!.

search(Y,[M|N],Svalue):-
	search(Y,N,Svalue).


find(X,Y,Value) :- 
	frame(X,Z),
	search(Y,Z,Value),!.

find(X,Y,Value) :-
	frame(X,[inst(Z)|_]),
	find(Z,Y,Value),!.

find(X,Y,Value) :-
	frame(X,[ako(Z)|_]),
	find(Z,Y,Value),!.

find(X,Y,Value) :-
	frame(X,[part_of(Z)|_]),
	find(Z,Y,Value),!.