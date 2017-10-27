#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

const int one_level = 0;
const int one_row = 1;
const int one_col = 1;
const int min_max_depth = 3;

/*
This is the class that stores coordinates of points in form of triplets.
*/

class triplet{
	public:
	int a;
	int b;
	int c;

	triplet(){
		a=-1;
		b=-1;
		c=-1;
	}

	triplet(int a1, int b1, int c1){
		a = a1;
		b = b1;
		c = c1;
	}

	void printme(){
		cout<<a<<" "<<b<<" "<<c;
	}
};


/*
Function that generates magic cube
*/
void generate_cube( int cube [3][3][3],triplet my_map[28],int level=2,int row=1, int col=1 ){

	for(int i=1;i<=27;i++){
		//Store the ith number at given location
		cube[level][row][col]= i;

		//Hash map to map each number to it's location in the cube(coordinates)
		my_map[i].a=level;
		my_map[i].b=row;
		my_map[i].c=col;

		//The section that finds the next position to puth i+1th element
		if(i%9 == 0){
			//Drop Level Case
			level--;
			if(level == -1)level=2;
		}
		else{
			//Increase Level
			level++;
			if(level == 3)level=0;

			//Right Case
			if(i%3 !=0 ){
				col++;
				if(col==3)col=0;
			}
			//Ahead Case
			else{
				row--;
				if(row==-1)row=2;
			}
		}
	}
}


/*
The function which prints all three front faces of cube.
Surface parameter tells which face to print among 3 front faces.
*/
void print_front(int cube[3][3][3], int surface=1){
	int row = 2-surface+1;
	for(int level=2;level>=0;level--){
		for(int col=0;col<=2;col++){
			cout<<cube[level][row][col]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}


/*
The function which prints all three top faces of cube.
Surface parameter tells which face to print among 3 top faces.
*/
void print_top(int cube[3][3][3], int surface=1){
	int level = 2-surface+1;
	for(int row=0;row<=2;row++){
		for(int col=0;col<=2;col++){
			cout<<cube[level][row][col]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

/*
The function which prints all three side faces of cube (from the right).
Surface parameter tells which face to print among 3 side faces.
*/

void print_right(int cube[3][3][3], int surface=1){
	int col = 2-surface+1;
	for(int row=0;row<=2;row++){
		for(int level=2;level>=0;level--){
			cout<<cube[level][row][col]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}

/*
The function which prints the two diagonal faces of cube.
Surface parameter tells which face to print among 2 diagonal faces.
*/

void print_diag(int cube[3][3][3], int surface=1){

	if(surface==1){
		for(int level=2;level>=0;level--){
			for(int row=0;row<=2;row++){
				cout<<cube[level][row][row]<<" ";
			}
			cout<<"\n";
		}
	}
	else{
		for(int level=2;level>=0;level--){
			for(int row=2;row>=0;row--){
				cout<<cube[level][row][2-row]<<" ";
			}
			cout<<"\n";
		}
	}

	cout<<"\n";
}

/*
The function which prints all eleven faces of cube.
*/
void print_cube(int cube[3][3][3]){

    cout<<"Front \n";
    print_front(cube,1);
    cout<<"Middle Front \n";
    print_front(cube,2);
    cout<<"Back \n";
    print_front(cube,3);

 	cout<<"Right \n";
    print_right(cube,1);
    cout<<"Middle Surface Right \n";
    print_right(cube,2);
    cout<<"Left \n";
    print_right(cube,3);

    cout<<"Top \n";
    print_top(cube,1);
    cout<<"Middle Top \n";
    print_top(cube,2);
    cout<<"Bottom \n";
    print_top(cube,3);

    cout<<"Diagonal 1 \n";
    print_diag(cube,1);
    cout<<"Diagonal 2 \n";
    print_diag(cube,2);

}

/*
Function to find distance between two points in 3-D space.
Helper function to check 3 points in 3-D space are collinear.
*/
float find_distance(triplet t1,triplet t2){

	return 0.0+sqrt(((t1.a-t2.a)*(t1.a-t2.a))+
				((t1.b-t2.b)*(t1.b-t2.b))+
				((t1.c-t2.c)*(t1.c-t2.c)));
}

/*
Function to check if the given three points(triplets) in 3-D space are collinear.
*/
bool is_collinear_distance(triplet t1, triplet t2, triplet t3){
	float d1 = find_distance(t1,t2);
	float d2 = find_distance(t2,t3);
	float d3 = find_distance(t1,t3);
	return d1+d2==d3 || d2+d3==d1 || d1+d3==d2;
}

/*
Function that iterates over all possible combinations of 3-tuples and find anomalous points.
Vector v stores anomaly where sum=42 but points are not collinear.
Vector v2 stores anomaly where sum != 42 but points are collinear.
*/

void find_anomaly(int cube[3][3][3], vector<triplet*> &v,vector<triplet*> &v2, triplet my_map[28]){

	for(int i=1;i<=27;i++){
		for(int j=i+1;j<=27;j++){
			for(int k=j+1;k<=27;k++){

				if(i+j+k==42){
					//If they are not collinear then anomaly
					if(!is_collinear_distance(my_map[i],my_map[j],my_map[k])){
						v.push_back (new triplet(i,j,k));
					}
				}
				else{
					//Case when sum is not 42 but still collinear
					if(is_collinear_distance(my_map[i],my_map[j],my_map[k])){
						v2.push_back (new triplet(i,j,k));
					}
				}
			}
		}
	}
}

/*
Function to compare two 3-tuples co-ordinate wise.
*/
bool compare_in_order(int a1,int a2,int a3,int b1,int b2,int b3){
	return (a1==b1) && (a2==b2) && (a3==b3);
}

/*
Function to check that given 3 numbers are actually collinear in our original cube;
by taking into account the two type of anomalies.
*/
bool is_collinear(int a, int b, int c, vector<triplet*> anom1,vector<triplet*> anom2){

	bool temp;

	if(a+b+c == 42){
		//If sum is 42 we check in list of anomaly type-I as they still might not be collinear
		for(int i=0;i<anom1.size();i++){
			int a_1 = (*anom1[i]).a;
			int b_1 = (*anom1[i]).b;
			int c_1 = (*anom1[i]).c;
			if(a==a_1){
				if(b==b_1 || b==c_1){
					return false;
				}
			}
			if(a==b_1){
				if(b==a_1 || b==c_1){
					return false;
				}
			}
			if(a==c_1){
				if(b==b_1 || b==a_1){
					return false;
				}
			}
		}
		temp = true;
	}
	else{
		//Even if sum is not 42 we check in second list of anomaly to confirm they are not collinear
		for(int i=0;i<anom2.size();i++){
			int a_1 = (*anom2[i]).a;
			int b_1 = (*anom2[i]).b;
			int c_1 = (*anom2[i]).c;
			if(compare_in_order(a,b,c,a_1,b_1,c_1))return true;
			if(compare_in_order(a,c,b,a_1,b_1,c_1))return true;
			if(compare_in_order(b,a,c,a_1,b_1,c_1))return true;
			if(compare_in_order(b,c,a,a_1,b_1,c_1))return true;
			if(compare_in_order(c,a,b,a_1,b_1,c_1))return true;
			if(compare_in_order(c,b,a,a_1,b_1,c_1))return true;
		}
		temp = false;
	}

	return temp;
}

/*
Funnction that calculates score(benefit) heuristically at any given state of the game
*/
int calc_score(vector<triplet*> anom1,vector<triplet*> anom2,vector<int> amove,vector<int> bmove){


		int score = 0;

		int a_size = amove.size();
		//cout<<"a_size"<<a_size;
		for(int i=0;i<a_size;i++){
			for(int j=i+1;j<a_size;j++){
				for(int k=j+1;k<a_size;k++){
					//If we have collinear points, we have a positive score
                    if(is_collinear(amove[i],amove[j],amove[k],anom1,anom2)){
						score  = score + 5 ;
					}

				}
			}
		}

		int b_size = bmove.size();
		for(int i=0;i<b_size;i++){
			for(int j=i+1;j<b_size;j++){
				for(int k=j+1;k<b_size;k++){
					//If opponent(human) has collinear point that's a negative score for computer
					if(is_collinear(bmove[i],bmove[j],bmove[k],anom1,anom2)){
						score  = score - 5;
					}

				}
			}
		}
		return score;
}

/*
Function that gives us the best possible score we can obtain by searching upto certain depth in the graph.
*/
int min_max(int depth, bool max_turn, bool empty_pos[28],vector<int> amove,vector<int> bmove,vector<triplet*> anom1,vector<triplet*> anom2){

	if(depth>min_max_depth){
		return calc_score(anom1,anom2,amove,bmove);
	}

	if(max_turn){
		int best_score = INT_MIN;

		for(int i=1;i<=27;i++){
			if(empty_pos[i]){
				empty_pos[i] = false;
				amove.push_back(i);
				best_score = max(best_score,min_max(depth+1,!max_turn,empty_pos,amove,bmove,anom1,anom2));
				empty_pos[i] = true;
				amove.pop_back();
			}
		}

		return best_score-depth;
	}
	else{

		int best_score = INT_MAX;

		for(int i=1;i<=27;i++){
			if(empty_pos[i]){
				empty_pos[i] = false;
				bmove.push_back(i);
				best_score = min(best_score,min_max(depth+1,!max_turn,empty_pos,amove,bmove,anom1,anom2));
				//cout<<"best score = "<<best_score;
				empty_pos[i] = true;
				bmove.pop_back();
			}
		}
		return best_score-depth;
	}
}


/*
Returns the best number to be played at using min_max function heuristic scores over all empty positions.
*/
int find_best_move(bool empty_pos[28],vector<int> amove,vector<int> bmove,vector<triplet*> anom1,vector<triplet*> anom2, int t_moves){

	int best_move;
	int best_score = INT_MIN;

	for(int i=1;i<=27;i++){

		if(empty_pos[i]){
			empty_pos[i] = false;
			amove.push_back(i);
			//cout<<"jhgkvk "<<i<<" is empty \n";
			int temp;
			if(t_moves == 17 || t_moves == 18 )
                temp = min_max(2,false,empty_pos,amove,bmove,anom1,anom2);
			else if(t_moves == 19 || t_moves == 20 )
                temp = min_max(3,false,empty_pos,amove,bmove,anom1,anom2);
			else
                temp = min_max(1,false,empty_pos,amove,bmove,anom1,anom2);
			//cout<<"jhgkvk "<<i<<" is empty after min_max\n";
			if(temp>=best_score){
				best_score = temp;
				best_move = i;
			}
			empty_pos[i] = true;
			amove.pop_back();
		}
	}
	return best_move;
}


/*
Increase a player points(win counter) if the new move (pos) is forming more linear triplets.
*/
void inc_counter(vector<int> v_move,int pos,int &count,vector<triplet*> anom1,vector<triplet*> anom2){

	int size = v_move.size();
	for(int i=0;i<size;i++){
		for(int j=i+1; j<size;j++){
			if(is_collinear(v_move[i],v_move[j],pos,anom1,anom2)){
				cout<<"Formed! "<<v_move[i]<<" "<<v_move[j]<<" "<<pos<<"\n";
				count++;
			}
		}
	}

}


int main(){

    int cube[3][3][3];

    //Array that stores maaping of number to it's co-ordinates in 3-D cube
    triplet my_map[28];

    //Initialize the cube and print it
	generate_cube(cube,my_map,one_level,one_row,one_col);
	print_cube(cube);

	//Vectors to store 2 types of anomalies
	vector<triplet*> anom1;
	vector<triplet*> anom2;

	//Finding and printing anomalies
	find_anomaly(cube,anom1,anom2,my_map);
	cout<<"Anomaly 1 \n";
	for(int i=0;i<anom1.size();i++){
		(*anom1[i]).printme();
		cout<<"\n";
	}


	cout<<"Anomaly 2 \n";
	for(int i=0;i<anom2.size();i++){
		(*anom2[i]).printme();
		cout<<"\n";
	}

	//Vector to store seq of moves by PC
	vector<int> amove;
	//Vector to store seq of moves by human
	vector<int> bmove;
	bool empty_pos[28];
	for(int i=0;i<=27;i++){
		empty_pos[i] = true;
	}

	//variable to store PC score
	int a_count = 0;
	//variable to score human score
	int b_count = 0;
	int t_moves=0;
	char play_first;

	cout<<"Do you want to play first y/n ? \n";
	cin >> play_first;
	if(play_first != 'y'){
		//We play in center in first move
	    int pos = cube[1][1][1];
		cout<< "I play "<<pos<<" \n";
		inc_counter(amove,pos,a_count,anom1,anom2);
		amove.push_back(pos);
		empty_pos[pos]=false;
		t_moves++;
	}

	while(t_moves <=20){

			int pos = -1;

			cout << "Enter position to play \n";
			cin>> pos;

			//Loop to check if input entered is valid and not occupied
			while(!empty_pos[pos] || pos<1 || pos>27){
				cout<<"Position already occupied or invalid, play again! \n";
				cin>>pos;
			}
			//Increase Human points if linear triplet formed
			inc_counter(bmove,pos,b_count,anom1,anom2);
			bmove.push_back(pos);
			empty_pos[pos] = false;
			t_moves++;
			t_moves++;
			cout<< "Your Score : "<<b_count<<" My Score : "<<a_count<<" \n";

			if(t_moves <=20){
	            pos = find_best_move(empty_pos,amove,bmove,anom1,anom2,t_moves);
				cout<< "I play "<<pos<<" \n";
				//Increase PC points if linear triplet formed
				inc_counter(amove,pos,a_count,anom1,anom2);
				amove.push_back(pos);
				empty_pos[pos]=false;
				t_moves++;
				cout<< "Your Score : "<<b_count<<" My Score : "<<a_count<<" \n";
			}

	}

	//Produce the results
	if(a_count > b_count){
		cout<<"I win \n";
	}
	else if(a_count < b_count){
		cout<< "Congrats! You win \n";
	}else{
		cout <<"We are draw \n";
	}

	return 0;
}


