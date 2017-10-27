#include <iostream>
#include <vector>
using namespace std;

char pc='x';
char human = 'o';


void generate_cube( int cube [3][3][3],int level=2,int row=1, int col=1 ){

	for(int i=1;i<=27;i++){
		cube[level][row][col]= i;
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

class triplet{
	public:
	int a;
	int b;
	int c;

	triplet(int a1, int b1, int c1){
		a = a1;
		b = b1;
		c = c1;
	}

	void printme(){
		cout<<a<<" "<<b<<" "<<c;
	}
};

void find_anomaly_a(int cube[3][3][3], vector<triplet*> &v){
	//Find in front faces
	for(int row=0;row<=2;row++){
		if(cube[0][row][0]+cube[1][row][2]+cube[2][row][1] == 42){
			v.push_back (new triplet(cube[0][row][0],cube[1][row][2],cube[2][row][1]));
		}
		if(cube[0][row][2]+cube[1][row][0]+cube[2][row][1] == 42){
			v.push_back(new triplet(cube[0][row][2],cube[1][row][0],cube[2][row][1]));
		}
		if(cube[2][row][0]+cube[1][row][1]+cube[2][row][2] == 42){
			v.push_back(new triplet(cube[2][row][0],cube[1][row][1],cube[2][row][2]));
		}
		if(cube[2][row][2]+cube[0][row][1]+cube[1][row][0] == 42){
			v.push_back(new triplet(cube[2][row][2],cube[0][row][1],cube[1][row][0]));
		}
	}

	//Find in right faces
	for(int col=0;col<=2;col++){
		if(cube[0][0][col]+cube[1][2][col]+cube[2][1][col] == 42){
			v.push_back(new triplet(cube[0][0][col],cube[1][2][col],cube[2][1][col]));
		}
		if(cube[0][2][col]+cube[1][0][col]+cube[2][1][col] == 42){
			v.push_back(new triplet(cube[0][2][col],cube[1][0][col],cube[2][1][col]));
		}
		if(cube[2][0][col]+cube[1][1][col]+cube[2][2][col] == 42){
			v.push_back(new triplet(cube[2][0][col],cube[1][1][col],cube[2][2][col]));
		}
		if(cube[2][2][col]+cube[0][1][col]+cube[1][0][col] == 42){
			v.push_back(new triplet(cube[2][2][col],cube[0][1][col],cube[1][0][col]));
		}
	}


	//Find in top faces
	for(int level=0;level<=2;level++){
		if(cube[level][0][0]+cube[level][1][2]+cube[level][2][1] == 42){
			v.push_back(new triplet(cube[level][0][0],cube[level][1][2],cube[level][2][1]));
		}
		if(cube[level][0][2]+cube[level][1][0]+cube[level][2][1] == 42){
			v.push_back(new triplet(cube[level][0][2],cube[level][1][0],cube[level][2][1]));
		}
		if(cube[level][2][0]+cube[level][1][1]+cube[level][2][2] == 42){
			v.push_back(new triplet(cube[level][2][0],cube[level][1][1],cube[level][2][2]));
		}
		if(cube[level][2][2]+cube[level][0][1]+cube[level][1][0] == 42){
			v.push_back(new triplet(cube[level][2][2],cube[level][0][1],cube[level][1][0]));
		}
	}

}


void find_anomaly_b(int cube[3][3][3], vector<triplet*> &v){

	for(int level=2;level>=0;level--){
		if(cube[level][0][0]+cube[level][1][1]+cube[level][2][2] != 42){
			v.push_back(new triplet(cube[level][0][0],cube[level][1][1],cube[level][2][2]));
		}
	}

	for(int level=2;level>=0;level--){
		if(cube[level][0][2]+cube[level][1][1]+cube[level][2][0] != 42){
			v.push_back(new triplet(cube[level][0][2],cube[level][1][1],cube[level][2][0]));
		}
	}


	for(int row=0;row<=2;row++){
		for(int col=0;col<=2;col++){
			if(cube[0][row][col]+cube[1][1][1]+cube[2][2-row][2-col] != 42){
				//cout<<"test "<<cube[0][row][col]+cube[1][1][1]+cube[0][2-row][2-col]<<"\n";
				v.push_back(new triplet(cube[0][row][col],cube[1][1][1],cube[2][2-row][2-col]));
			}		
		}
	}
	
}

int main(){

    int cube[3][3][3];
	generate_cube(cube,2,1,1);
	print_cube(cube);

	vector<triplet*> anom1;
	vector<triplet*> anom2;
	find_anomaly_a(cube,anom1);
	cout<<"Anomaly 1 \n";
	for(int i=0;i<anom1.size();i++){
		(*anom1[i]).printme();
		cout<<"\n";
	}

	find_anomaly_b(cube,anom2);
	cout<<"Anomaly 2 \n";
	for(int i=0;i<anom2.size();i++){
		(*anom2[i]).printme();
		cout<<"\n";
	}

	return 0;
}


int calc_score(int cube[3][3][3], int symbol[3][3][3]){


	int score = 0;

	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[a][b][c];
			for(int c=0; c<=2; c++){
				if(symbol[a][b][c] == '_'){
					line = false;
					break;
				}
				else if(symbol[a][b][c] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}

	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[c][a][b];
			for(int c=0; c<=2; c++){
				if(symbol[c][a][b] == '_'){
					line = false;
					break;
				}
				else if(symbol[c][a][b] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}


	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[b][c][a];
			for(int c=0; c<=2; c++){
				if(symbol[b][c][a] == '_'){
					line = false;
					break;
				}
				else if(symbol[b][c][a] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}

	for(int a=0; a<=2; a++){
		if(symbol[a][0][0] == symbol[a][1][1]  && symbol[a][1][1] == symbol[a][2][2] && symbol[a][0][0] != '_'){
			if(symbol[a][0][0] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[a][0][2] == symbol[a][1][1]  && symbol[a][1][1] == symbol[a][2][0] && symbol[a][0][2] != '_'){
			if(symbol[a][0][2] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][a][0] == symbol[1][a][1]  && symbol[1][a][1] == symbol[2][a][2] && symbol[0][a][0] != '_'){
			if(symbol[0][a][0] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][a][2] == symbol[1][a][1]  && symbol[1][a][1] == symbol[2][a][0] && symbol[0][a][2] != '_'){
			if(symbol[0][a][2] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][0][a] == symbol[1][1][a]  && symbol[1][1][a] == symbol[2][2][a] && symbol[0][0][a] != '_'){
			if(symbol[0][0][a] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][2][a] == symbol[1][1][a]  && symbol[1][1][a] == symbol[2][0][a] && symbol[0][2][a] != '_'){
			if(symbol[0][2][a] == pc) 
				score += 10;
			else score -= 10;
		}
	}

	for(int row=0;row<=2;row++){
		for(int col=0;col<=2;col++){
			if(row+col %2 == 0){
				if(symbol[0][row][col]!= '_' && symbol[0][row][col]==symbol[1][1][1] && symbol[1][1][1]==symbol[2][2-row][2-col]){
					if(symbol[0][row][col] == pc) 
					score += 10;
					else score -= 10;								
				}
			}		
		}
	}

	return score;

}





int calc_score(int cube[3][3][3], int symbol[3][3][3]){


	int score = 0;

	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[a][b][c];
			for(int c=0; c<=2; c++){
				if(symbol[a][b][c] == '_'){
					line = false;
					break;
				}
				else if(symbol[a][b][c] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}

	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[c][a][b];
			for(int c=0; c<=2; c++){
				if(symbol[c][a][b] == '_'){
					line = false;
					break;
				}
				else if(symbol[c][a][b] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}


	for(int a=0; a<=2; a++){
		for(int b=0; b<=2; b++){
			bool line = true;
			char test = symbol[b][c][a];
			for(int c=0; c<=2; c++){
				if(symbol[b][c][a] == '_'){
					line = false;
					break;
				}
				else if(symbol[b][c][a] != test){
					line = false;
					break;	
				}
			}
			if(line){
				if(test == pc) score += 10;
				else score -= 10;			
			}
		}
	}

	for(int a=0; a<=2; a++){
		if(symbol[a][0][0] == symbol[a][1][1]  && symbol[a][1][1] == symbol[a][2][2] && symbol[a][0][0] != '_'){
			if(symbol[a][0][0] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[a][0][2] == symbol[a][1][1]  && symbol[a][1][1] == symbol[a][2][0] && symbol[a][0][2] != '_'){
			if(symbol[a][0][2] == pc) 
				score += 10;
			else score -= 10;
		}
				if(symbol[0][a][0] == symbol[1][a][1]  && symbol[1][a][1] == symbol[2][a][2] && symbol[0][a][0] != '_'){
			if(symbol[0][a][0] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][a][2] == symbol[1][a][1]  && symbol[1][a][1] == symbol[2][a][0] && symbol[0][a][2] != '_'){
			if(symbol[0][a][2] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][0][a] == symbol[1][1][a]  && symbol[1][1][a] == symbol[2][2][a] && symbol[0][0][a] != '_'){
			if(symbol[0][0][a] == pc) 
				score += 10;
			else score -= 10;
		}
		if(symbol[0][2][a] == symbol[1][1][a]  && symbol[1][1][a] == symbol[2][0][a] && symbol[0][2][a] != '_'){
			if(symbol[0][2][a] == pc) 
				score += 10;
			else score -= 10;
		}
	}

	for(int row=0;row<=2;row++){
		for(int col=0;col<=2;col++){
			if(row+col %2 == 0){
				if(symbol[0][row][col]!= '_' && symbol[0][row][col]==symbol[1][1][1] && symbol[1][1][1]==symbol[2][2-row][2-col]){
					if(symbol[0][row][col] == pc) 
					score += 10;
					else score -= 10;								
				}
			}		
		}
	}

	return score;

}


int min_max(int depth, bool max_turn, int symbol[3][3][3]){

	if(depth>3){
		return calc_score(cube,symbol);
	}

	if(max_turn){
		int best_score = INT_MIN;

		for(int i=0;i<=2;i++){
			for(int j=0;j<=2;j++){
				for(int k=0;k<=2;k++){
					if(symbol[i][j][k] == '_'){

						symbol[i][j][k] == pc;
						best_score = max(best_score,min_max(depth+1,!max_turn,symbol));
						symbol[i][j][k] == '_';

					}
				}
			}		
		}
		return best_score;
	}
	else{

		int best_score = INT_MAX;

		for(int i=0;i<=2;i++){
			for(int j=0;j<=2;j++){
				for(int k=0;k<=2;k++){
					if(symbol[i][j][k] == '_'){

						symbol[i][j][k] == human ;
						best_score = min(best_score,min_max(depth+1,!max_turn,symbol));
						symbol[i][j][k] == '_';

					}
				}
			}		
		}
		return best_score;
	}
}

triplet find_best_move(int symbol[3][3][3]){

	triplet best_move;
	int best_score = INT_MIN;

	for(int i=0;i<=2;i++){
		for(int j=0;j<=2;j++){
			for(int k=0;k<=2;k++){
				if(symbol[i][j][k] == '_'){
					symbol[i][j][k] = pc;
					int temp =min_max(1,false,symbol);
					if(temp>best_score){
						best_score = temp;
						best_move.a=i;
						best_move.b=j;
						best_move.c=k;
					}
					symbol[i][j][k] = '_';
				}
			}
		}
	}
	return best_move; 
}


bool is_collinear(int a, int b, int c, vector<triplet*> anom1,vector<triplet*> anom2){

	bool temp;

	if(a+b+c == 42){
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
		for(int i=0;i<anom2.size();i++){
			int a_1 = (*anom2[i]).a;
			int b_1 = (*anom2[i]).b;
			int c_1 = (*anom2[i]).c;
			if(a==a_1){
				if(b==b_1 || b==c_1){
					return true;
				}
			}
			if(a==b_1){
				if(b==a_1 || b==c_1){
					return true;
				}
			}
			if(a==c_1){
				if(b==b_1 || b==a_1){
					return true;
				}
			}
		}
		temp = false;	
	}

	return temp;
}

int calc_score(vector<triplet*> anom1,vector<triplet*> anom2,vector<int> amove,vector<int> bmove){

		int score = 0;

		int a_size = amove.size();
		for(int i=0;i<a_size;i++){
			for(int j=i+1;j<a_size;i++){
				for(int k=j+1;k<a_size;k++){

					if(is_collinear(amove[i],amove[j],amove[k],anom1,anom2)){
						score  += 10;
					}

				}
			}
		}

		int b_size = bmove.size();
		for(int i=0;i<b_size;i++){
			for(int j=i+1;j<b_size;i++){
				for(int k=j+1;k<b_size;k++){

					if(is_collinear(bmove[i],bmove[j],bmove[k],anom1,anom2)){
						score  -= 10;
					}

				}
			}
		}

		return score;

}

int find_best_move(bool empty_pos[28],vector<int> amove,vector<int> bmove,vector<triplet*> anom1,vector<triplet*> anom2){

	int best_move;
	int best_score = INT_MIN;

	for(int i=1;i<=27;i++){

		if(empty_pos[i]){
			empty_pos[i] = false;
			amove.push_back(i);
			int temp = min_max(1,false,empty_pos,amove,bmove,anom1,anom2);
			if(temp>best_score){
				best_score = temp;
				best_move = i;
			}
			empty_pos[i] = true;
			amove.pop_back();
		}
	}
	return best_move; 
}


int min_max(int depth, bool max_turn, bool empty_pos[28],vector<int> amove,vector<int> bmove,vector<triplet*> anom1,vector<triplet*> anom2){

	if(depth>3){
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

		return best_score;
	}
	else{

		int best_score = INT_MAX;

		for(int i=1;i<=27;i++){
			if(empty_pos[i]){
				empty_pos[i] = false;
				bmove.push_back(i);
				best_score = min(best_score,min_max(depth+1,!max_turn,empty_pos,amove,bmove,anom1,anom2));
				empty_pos[i] = true;
				bmove.pop_back();
			}
		}
		return best_score;
	}
}