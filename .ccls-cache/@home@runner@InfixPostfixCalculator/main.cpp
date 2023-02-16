// Test pattern:  ((2-1)*2^2^3+8/4)
//1-2*3/2^1^2+3

#include<iostream>
#include<cmath>
#include<stack>
#include<iomanip>

using namespace std;

int  calc(char token, int num1, int num2);
bool hasPrec(char op1, char op2);
int  getWeight( char op );
bool in2postfix( deque<char> &operatorSTK, string &infixQ, string &postfixQ );

int  postfixCalc( string postfixQ );

int main() {
    // Naming Convention: All containers has a pending STK or Q 
    // In this program we used 3 containers:
    string infix;      // infix expression
    string infixQ;     // infix Queue
    string postfixQ;   // postfix Queue
    deque<char> operatorSTK; // for resolving (^, *, /, +, -) In-Coming Priority 

    int result;
    // holding the current operators to comapre with next In-Coming one
    
    // In this program, we choose to use string as a char Q.
    //     string q; 
    // See m11/usingStringAsQ.cpp for how STL string class supports
    //     show(), enQ(), deQ(), front(), rear()


    cout << "Please enter an INFIX expression: ";
    getline(cin, infix); 
    cout << "InFix Expression: " << infix;
    for(char c:infix) {
        if( isdigit(c) || c=='^' || c=='(' || c==')' ||
            c=='+' || c=='-' || c=='*' || c=='/' )
        infixQ += c;
    }
    
    // cout << "\nContents of infix Queue: " << infixQ;
    
    if( !in2postfix( operatorSTK, infixQ, postfixQ ) ) {
        cout << "failed the conversion to postfix!";
        exit(EXIT_FAILURE);
    }
    
    cout << "\nThe postfix expression is " << postfixQ;
    // cin.get();
    result = postfixCalc(postfixQ);
    cout << "\nAnswer: " << result; //display the result
        
    return 0;
}

bool in2postfix( deque<char> &operatorSTK, string &infixQ, string &postfixQ ) {
    bool result = true;
    cout << endl
         << setw(15) << left << "operatorSTK"
         << setw(20) << right << "postfixQ"
         << setw(20) << right << "infixQ\n";

    // Infix to Postfix Conversion, parsing left to right:
    while(true){ 
// Location 1. put out debug tracer for operatorSTK, infixQ, postfixQ
// cout << "\n operatorSTK: "; for(auto x:operatorSTK) cout << x;
// cout << "  <<<  infixQ: " << infixQ;
// cout << "  <<<  postfixQ: " << postfixQ << endl;

for(auto x:operatorSTK) cout << x;
for(int i=0; i<(15 - operatorSTK.size()); i++) cout << ' ';
cout << setw(20) << right << postfixQ
     << setw(20) << right << infixQ << endl;
     
        if( infixQ.empty() ) break; // infix Q is empty - done!

        // parsing the infixQ left to right, reading the front of infixQ
        char in = infixQ[0]; infixQ.erase(0,1);
        if( '0'<=in && in<='9') // If an operand: output it into the postfix postfixQ
        { postfixQ += in; continue; }

        switch( in ) {
            case '(': // push into the operator operatorSTK
                operatorSTK.push_front(in); break;
                
            case ')':  // pop operator operatorSTK to postfix postfixQ
                       // until matching ‘(‘ is popped from the operatorSTK
                while(true) {
                    char op = operatorSTK.front(); operatorSTK.pop_front();
                    if( op == '(' ) break;
                    postfixQ += op ;  // add to postfix
                } break;
                
            // To determine the ICP, i.e. In-coming vs OperatorStk.front()
            case '+': case '-': case '*': case '/': case '^': 
            {
                // If the operatorSTK.front() is empty or top of the Operator operatorSTK is   
                //   ‘(‘ lower precedence, then push onto Operator operatorSTK
                char op = operatorSTK.front();
                while( !operatorSTK.empty() && hasPrec(op, in ) ) { // ICP In-Coming Priority
                    postfixQ += op;  operatorSTK.pop_front();
                    op = operatorSTK.front();
                }
                operatorSTK.push_front(in); 
                break;
            }
            
            default:
                cout << "Unknown symbol " << in << " encountered!\n";
                result = false;
                break;
        }// end of switch
        
        if(in=='#') break;
    } // end of while true for infix to postfix
    
    // the end of infixQ
    // pop remaining operator operatorSTK to postfix postfixQ
    while( !operatorSTK.empty() ) { 
        char op = operatorSTK.front(); operatorSTK.pop_front();
        postfixQ += op ; 
    } 

// Location 2. 
// cout << "\n operatorSTK: "; for(auto x:operatorSTK) cout << x;
// cout << "  <<<  infixQ: " << infixQ;
// cout << "  <<<  postfixQ: " << postfixQ << endl;
    return result;
}

int postfixCalc( string postfixQ ) {
    stack<int> intSTK; // An Integer Stack calculator Stack
    int result, left_op, right_op, step = 1; 
    
    // While postfixQ is not empty do
    while( !postfixQ.empty() ) {
        char ch = postfixQ[0]; postfixQ.erase(0,1);
        
        // When ch is an operand, 
        // push the numerical value of op to intSTK 
        if( '0'<=ch && ch<='9') intSTK.push(ch-'0');
        else if(ch=='+' || ch== '-' ||
                ch=='*' || ch== '/' || ch=='^') {
                    
            // pop Stack intSTK right_op on top of left_op
            right_op = intSTK.top(); intSTK.pop();
            left_op = intSTK.top(); intSTK.pop();
            result = calc( ch, left_op, right_op );
            intSTK.push(result);
            
cout << "\nStep " << step << ": " 
     << result << " = " << left_op << ch << right_op;
// cout << " | postfixQ: " << postfixQ;
// cout << " | intSTK: " << intSTK;
            
            step++;
        }
    }
    return result;
}

// Two argumented Calculator
int calc(char token, int num1, int num2){
    int result;
    switch(token) {
        case '*': result = num1 * num2; break;
        case '/': result = num1 / num2; break;
        case '+': result = num1 + num2; break;
        case '-': result = num1 - num2; break;
        case '^': result = pow(num1, num2); break;
    }
    return result;
}

// Compare ISP: In-Stack Priority Weight
bool hasPrec(char op1, char op2) { 
    int op_pr1 = getWeight(op1);
    int op_pr2 = getWeight(op2);
    if(op_pr1 == op_pr2) {
        if(op1 == '^') return false;
        return true;
    }
    return op_pr1 >= op_pr2 ? true:false;
}

// Determine ISP: In-Stack Priority Weight
int getWeight( char op ) { 
    int w = -1;
    switch (op) {
        case '+':
        case '-': w = 1; break;
        case '*':
        case '/': w = 2; break;
        case '^': w = 3; break;
    }
    return w;
}
void display_stack_postfix_queue(deque < char > operatorSTK, string postfixQ){

  cout <<"OP Stack: ";

  deque < char > tempoperatorSTK = operatorSTK;

  for (auto x:operatorSTK) {

    cout <<tempoperatorSTK.back();

    tempoperatorSTK.pop_back();
  }
}