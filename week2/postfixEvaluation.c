#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXLENGTH 100

float stack[MAXLENGTH];
int top = -1;
bool validNumber = true;

bool isOperator(char *token){
    return (*token == '+'|| *token == '-' || *token == '*' || *token == '/');
}

void compute(char *token){
    if(!(top>1)){
        printf("Invalid expr. \"%s\" does not have enough operands\n", token);
        exit(0);
    }
    float operand1 = stack[top];
    float operand2 = stack[--top];
    switch(*token){
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

void validate(char *token){
    char *p;
    double d = strtod(token,&p);
    if(*p != 0){
        printf("Invalid expr. Illegal tag: %s\n",token);
        exit(0);
    }
    else{
        stack[++top] = atof(token);
    }
}

bool isNumber(char *token){
    char *p;
    double d = strtod(token,&p);
    if(*p != 0){
        return false;
    }
    return true;
}

bool isValidNumber(char *token){
    char *p;
    double d = strtod(token,&p);
    if(*p != 0 && *token != '\n'){
        printf("Invalid expr. Illegal tag: %s\n",token);
        exit(0);
    }
    return true;
}

void evaluate(char *postfix_expression){
    const char delimiter[2] = " ";
    char *token = strtok(postfix_expression," \t");
    if(isOperator(token)){
        compute(token);
    }
    else if(isNumber(token)){
        validate(token);
    }
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
    fgets(postfix_expression,MAXLENGTH,stdin);
    char *pos;
    if((pos = strchr(postfix_expression,'\n')) != NULL){
        *pos = '\0';
    }
    evaluate(postfix_expression);
    printf("%f\n", stack[top]);
    return 0;
}
