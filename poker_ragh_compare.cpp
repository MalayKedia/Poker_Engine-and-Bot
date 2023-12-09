#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
bool isthereastraightinthisarray(int*a, int s){
	sort(a,a+s);
	int size=0;
	int b[s-1];
	for(int i=0; i<s-1; i++){
		if((a[i+1]-a[i])!=0){
			b[size++]=a[i+1]-a[i];
		}
	}
	int maxconsec1s=0;
	for(int j=0; j<size; j++){
		if (b[j]!=1){
			maxconsec1s=0;
		}
		else maxconsec1s++;
		if (maxconsec1s==4) return true; 
	}
	return false;
}
struct twodubs{
	double a, b;
};
struct card
{
    int suit, value;                          //suit lies between 0 and 3, and value lies between 0 and 12
    card(){
    	suit=0; value=0;
    }
    card(int a, int b){
    	suit=a;
    	value=b;
    }                          //constructor for card
};
int maxsuit(vector<card> setofseven){
	int a[4]; a[0]=0; a[1]=0; a[2]=0; a[3]=0;
	for(int i=0; i<7; i++){
		a[(setofseven[i]).suit]++;
	}
	int max=0, suitofmax;
	for(int i=0; i<4; i++){
		if(max<a[i]){
			max=a[i];
			suitofmax=i;
		}
	}
	return max+10*suitofmax;
}
bool isinthere(card a, vector<card> setofseven){
	bool b = false;
	for(int i=0; i<7; i++){
		b=b||((a.suit==(setofseven[i]).suit)&&(a.value==(setofseven[i]).value));
	}
	return b;
}
bool isvalinthere(int value, vector<card> setofseven){
	bool b = false;
	for(int i=0; i<7; i++){
		b=b||(value==(setofseven[i]).value);
	}
 	return b;
}
int maxnumberofavalue(vector<card> setofseven){
	int a[13]; 
	for(int j=0; j<13; j++){
		a[j]=0;
	}
	for(int i=0; i<7; i++){
		a[(setofseven[i]).value]++;
	}
	int max=0, valueofmax;
	for(int i=0; i<13; i++){
		if(max<a[i]){
			max=a[i];
			valueofmax=i;
		}
	}
	return max+20*valueofmax;
}
int maxnumberofavaluewithonetoavoid(int valuetoavoid, vector<card> setofseven){
	int a[13]; 
	for(int j=0; j<13; j++){
		a[j]=0;
	}
	for(int i=0; i<7; i++){
		a[(setofseven[i]).value]++;
	}
	a[valuetoavoid]=0;
	int max=0, valueofmax;
	for(int i=0; i<13; i++){
		if(max<a[i]){
			max=a[i];
			valueofmax=i;
		}
	}
	return max;
}
int isfullhouse(vector<card> setofseven){
	int x= maxnumberofavalue(setofseven);
	if(x%20<3) return 0;
	else {
		if(maxnumberofavaluewithonetoavoid(x/20, setofseven)>=2){
			return 91;
		}
	}
	return 0;
}
int istwopairs(vector<card> setofseven){
	int x= maxnumberofavalue(setofseven);
	if(x%20<2) return 0;
	else {
		if(maxnumberofavaluewithonetoavoid(x/20, setofseven)>=2){
			return 39+x/20;
		}
	}
	return 0;
}
int isonepair(vector<card> setofseven){
	int x= maxnumberofavalue(setofseven);
	if(x%20==2) return x/20+26;
	else return 0;
}
int ishighcard(vector<card> setofseven){
	int max=0;
	for(int p=0; p<7; p++){
		if((setofseven[p]).value>max) max=(setofseven[p]).value;
	}
	return 13+max;
}
int isfourofakind(vector<card> setofseven){
	int x= maxnumberofavalue(setofseven);
	if(x%20!=4) return 0;
	else return 104+x/20;
}
int isstraight(vector<card> setofseven){
	int a[7];
	for(int k=0; k<7; k++){
		a[k]=(setofseven[k]).value; 
	}
	if(isthereastraightinthisarray(a,7)) return 65;
	else return 0;
}
int isstraightflush(vector<card> setofseven){
	int x= maxsuit(setofseven);
	if(x%10<5){return 0;}
	else{
		int a[7];
		int size=0;
		for(int k=0; k<7; k++){
			if((setofseven[k]).suit==x/10) a[size++]=(setofseven[k]).value; 
		}
		if(isthereastraightinthisarray(a,size)) return 117;
		else return 0; 
	}
}

int isroyalflush(vector<card> setofseven){
 	int x= maxsuit(setofseven);
 	if(x%10<5){return 0;}
 	else{
 		if(isinthere(card(x/10, 12),setofseven)&&isinthere(card(x/10, 11),setofseven)&&isinthere(card(x/10, 10),setofseven)&&isinthere(card(x/10, 9),setofseven)&&isinthere(card(x/10, 8),setofseven)){
 			return 130;
 		}
 		else return 0;
 	}
}
int isthreeofakind(vector<card> setofseven){
	int x= maxnumberofavalue(setofseven);
	if(x%20<3) return 0;
	else return 52+x/20;
}
int isflush(vector<card> setofseven){
	if(maxsuit(setofseven)%10>=5){
		return 78;	
	}
	else return 0;
}
int rankk(vector<card> setofseven){
	if(isroyalflush(setofseven)) return isroyalflush(setofseven);
	else if(isstraightflush(setofseven)) return isstraightflush(setofseven);
	else if(isfourofakind(setofseven)) return isfourofakind(setofseven);
	else if(isfullhouse(setofseven)) return isfullhouse(setofseven);
	else if(isflush(setofseven)) return isflush(setofseven);
	else if(isstraight(setofseven)) return isstraight(setofseven);
	else if(isthreeofakind(setofseven)) return isthreeofakind(setofseven);
	else if(istwopairs(setofseven)) return istwopairs(setofseven);
	else if(isonepair(setofseven)) return isonepair(setofseven);
	else return ishighcard(setofseven);
}

//twodubs calcprob(vector<card> commcards, vector<card> playercards, vector<card> deck){

//}
void removecard(vector<card> *deck, card a){
	for(int b=0; b<(*deck).size(); b++){
		if((a.suit==((*deck)[b]).suit)&&(a.value==((*deck)[b]).value)){
			(*deck).erase((*deck).begin()+b);
			break;
		}
	}
}
int main(){
	vector<card> comcards,deck, playcards;
	int win=0,draw=0, lose=0;
	double probofwin=0, probofdraw=0;
	vector<card> *poi=&deck, *po=&comcards;
	for(int i=0; i<4; i++){
		for(int j=0; j<13; j++){
			deck.push_back(card(i,j));
		}
	}
	comcards.push_back(card(1,4));
 	comcards.push_back(card(1,5));
	comcards.push_back(card(0,6));
	// comcards.push_back(card(2,1));
	// comcards.push_back(card(0,0));
	// comcards.push_back(card(2,12));
	// comcards.push_back(card(0,8));
	playcards.push_back(card(1,3));
	playcards.push_back(card(1,7));
	//for the purpose of time testing
	
	removecard(poi, comcards[0]);
	removecard(poi, comcards[0]);
	removecard(poi, comcards[0]);
	removecard(poi, playcards[0]);
	removecard(poi, playcards[1]);
	for(int i=1; i<47; i++){
		for(int g=0; g<i; g++){
			card a=deck[i],b=deck[g];
			comcards.push_back(a);
			comcards.push_back(b);
			comcards.push_back(playcards[0]);
			comcards.push_back(playcards[1]);
			int c= rankk(comcards);
			comcards.erase(comcards.begin()+6);
			comcards.erase(comcards.begin()+5);
			for(int p=1; p<47; p++){
				if ((p==i)||(p==g)) continue;
				for(int u=0; u<p; u++){
					if ((u==i)||(u==g)) continue;
					comcards.push_back(deck[p]);
					comcards.push_back(deck[u]);
					int d =rankk(comcards);
					if(d<c) win++;
					if(d==c) {
						draw++;
						//cout<<d;
	}
					if(d>c) lose++;
					removecard(po, deck[p]);
					removecard(po, deck[u]);
				}
			}
			removecard(po, a);
			removecard(po, b);
		}
	}
	//cout<<deck.size();
	//cout<<rankk(comcards);
	cout<<win<<" "<<" "<<draw<<" "<<lose<<'\n';
	//cout<<maxnumberofavalue(comcards);
	//cout<<maxsuit(comcards);
	//cout<<isinthere(card(0,9), comcards);
}