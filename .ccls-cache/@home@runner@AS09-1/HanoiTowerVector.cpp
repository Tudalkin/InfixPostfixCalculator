#include <iostream>
#include <vector>
#include<iomanip>
using namespace std;

vector<int> src,dest,temp;
int size;

int pop(int t){
  int n;
  switch(t){
    case 1:
      n=src.back();
      src.pop_back();
      return n;
    case 2:
      n=dest.back();
      dest.pop_back();
      return n;
    case 3:
      n=temp.back();
      temp.pop_back();
      return n;
  }
return 0;
}
void push(int t,int n){
  switch(t){
    case 1: 
      src.push_back(n);
      break;
    case 2:
      dest.push_back(n);
      break;
    case 3: 
      temp.push_back(n);
      break;
  }
}

void display(){
  cout<<"src:";
  for (auto i = src.begin(); i != src.end(); ++i){
    cout << *i << " ";
  }
  cout<<" | dest:";
  for (auto i = dest.begin(); i != dest.end(); ++i){
    cout << *i << " ";
  }
  cout<<" | temp:";
  for (auto i = temp.begin(); i != temp.end(); ++i){
    cout << *i << " ";
  }
  cout<<endl;
}

void toh(int n, int s,int d,int t){
  //cout<<"check\n";
  if(n==1){
    push(d,pop(s));
    display();
    return;
  }
  toh(n-1,s,t,d);
  push(d,pop(s));
  display();
  toh(n-1,t,d,s);  
}

int main() {

  //get user size of tower
  
  cout << "Enter hanoi tower heigth: ";
  cin>>size;

  //values into source and displays
  for(int i=size;i>0;i--){
    src.push_back(i);
  }
  display();

  
  //calls toh function
  toh(size,1,2,3);
}