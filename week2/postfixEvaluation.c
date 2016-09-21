/*
Name : Akshit Kumar
Roll Number : EE14B127
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXLENGTH 100

float stack[MAXLENGTH];
int top = -1;

/*
    Function to check for operator
    @param token : Pointer containing the token
*/
bool isOperator(char *token){
    return (*token == '+'|| *token == '-' || *token == '*' || *token == '/');
}

/* Function to compute the subexpression for an operator and store it back in stack
    @param token : Pointer containing the token
*/
void compute(char *token){
    // Check if the operator has sufficient operands to operate on
    if(!(top>1)){
        printf("Invalid expr. \"%s\" does not have enough operands\n", token);
        exit(0);
    }
    float operand1 = stack[top]; // Pick first operand
    float operand2 = stack[--top]; // Pick second operand
    switch(*token){
        // Perform operation based on operator
        case '+':   stack[top] = operand2 + operand1;
                    break;
        case '-':   stack[top] = operand2 - operand1;
                    break;
        case '/':   stack[top] = operand2 / operand1;
                    break;
        case '*':   stack[top] = operand2 * operand1;
                    break;
    }
}
/*
    Function to validate the token obtained
    @param token : Pointer containing the token
*/
void validate(char *token){
    char *p;
    double d = strtod(token,&p);
    // Check if the expression of the number is legal or not
    if(*p != 0){
        printf("Invalid expr. Illegal tag: %s\n",token);
        exit(0);
    }
    else{
        stack[++top] = atof(token); // if legal, push it to stack
    }
}

/*
    Function to check if the possibility of number is present
    @param token : Pointer containing the token
*/
bool isNumber(char *token){
    char *p;
    double d = strtod(token,&p);
    if(*p != 0){
        return false;
    }
    return true;
}

/*
    Function to check if the number is valid or not
    @param token : Pointer containing the token
*/
bool isValidNumber(char *token){
    char *p;
    double d = strtod(token,&p);
    if(*p != 0 && *token != '\n'){
        printf("Invalid expr. Illegal tag: %s\n",token);
        exit(0);
    }
    return true;
}

/*
    Function to evaluate the postfix expression
    @param postfix_expression : Pointer to the postfix_expression
*/
void evaluate(char *postfix_expression){
    // Tokenize the postfix_expression into individual tokens delimited by space and tab
    char *token = strtok(postfix_expression," \t");
    if(isOperator(token)){
        compute(token);
    }
    else if(isNumber(token)){
        validate(token);
    }
    // Iterate through all the token till you reach the end of the expression
    while(token != NULL){
        if(isOperator(token) && !isNumber(token)){
            compute(token);
        }
        else if(isValidNumber(token)){
            validate(token);
        }
        token = strtok(NULL," \t");
    }
}

int main(){
    char postfix_expression[MAXLENGTH];
    fgets(postfix_expression,MAXLENGTH,stdin); // input the postfix_expression for the console
    // Replace \n included by fgets() by a \0
    char *pos;
    if((pos = strchr(postfix_expression,'\n')) != NULL){
        *pos = '\0';
    }
    evaluate(postfix_expression); // evaluate the expression
    printf("%f\n", stack[top]); // answer of the evaluation if no error are there
    return 0;
}
