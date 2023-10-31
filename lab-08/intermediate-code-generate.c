// CS21B2019
// DEVARAKONDA SLR SIDDESH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct Stack {
    int top;
    unsigned capacity;
    char* array;
};

struct Stack* createStack(unsigned capacity) 
{
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (char*)malloc(stack->capacity * sizeof(char));
    return stack;
}

int isStackEmpty(struct Stack* stack) 
{
    return stack->top == -1;
}

char peek(struct Stack* stack)
{
    if (!isStackEmpty(stack)) 
    {
        return stack->array[stack->top];
    }
    return '$'; 
}

void push(struct Stack* stack, char item) 
{
    stack->array[++stack->top] = item;
}

char pop(struct Stack* stack) 
{
    if (!isStackEmpty(stack)) 
    {
        return stack->array[stack->top--];
    }
    return '$'; 
}

int isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

int getOperatorPrecedence(char ch)
{
    if (ch == '+' || ch == '-')
        return 1;
    else if (ch == '*' || ch == '/')
        return 2;
    return 0;
}

char* infixToPostfix(char* infixExpression) 
{
    int i, j;
    struct Stack* stack = createStack(strlen(infixExpression));
    if (!stack) 
    {
        printf("Memory error\n");
        return NULL;
    }
    
    char* postfixExpression = (char*)malloc((strlen(infixExpression) + 1) * sizeof(char));
    if (!postfixExpression) 
    {
        printf("Memory error\n");
        free(stack);
        return NULL;
    }

    for (i = 0, j = 0; infixExpression[i]; i++) 
    {
        if (isalnum(infixExpression[i]))
            postfixExpression[j++] = infixExpression[i];
        else if (infixExpression[i] == '(')
            push(stack, infixExpression[i]);
        else if (infixExpression[i] == ')') 
        {
            while (peek(stack) != '$' && peek(stack) != '(')
                postfixExpression[j++] = pop(stack);
            if (peek(stack) == '(')
                pop(stack);
        } else 
        {
            while (!isStackEmpty(stack) && getOperatorPrecedence(infixExpression[i]) <= getOperatorPrecedence(peek(stack)))
                postfixExpression[j++] = pop(stack);
            push(stack, infixExpression[i]);
        }
    }

    while (!isStackEmpty(stack))
        postfixExpression[j++] = pop(stack);

    postfixExpression[j] = '\0';
    
    free(stack);
    
    return postfixExpression;
}

void generateThreeAddressCode(char * postfixExpression) 
{
    struct Stack * expressionStack = createStack(strlen(postfixExpression));
    int tempVariable = 0;

    for(int i = 0; i < strlen(postfixExpression); i++) 
    {
        if(!isOperator(postfixExpression[i])) 
        {
            push(expressionStack, postfixExpression[i]);
        }
        else 
        {
            tempVariable++;
            char operand1 = pop(expressionStack);
            char operand2 = pop(expressionStack);

            if((operand1 >= 'a' && operand1 <= 'z') && (operand2 >= 'a' && operand2 <= 'z')) 
            {
                printf("t%d = %c %c %c \n", tempVariable, operand2, postfixExpression[i], operand1);
            }
            else 
            {
                if(operand1 >= 'a' && operand1 <= 'z') 
                {
                    printf("t%d = t%c %c %c \n", tempVariable, operand2, postfixExpression[i], operand1);
                }
                else if(operand2 >= 'a' && operand2 <= 'z') 
                {
                    printf("t%d = %c %c t%c \n", tempVariable, operand2, postfixExpression[i], operand1);
                }
                else 
                {
                    printf("t%d = t%c %c t%c \n", tempVariable, operand2, postfixExpression[i], operand1);
                }
            }

            tempVariable = tempVariable + '0';
            push(expressionStack, tempVariable);
            tempVariable = tempVariable - '0';
        }
    }
}

int main() {
    char infixExpression[100];
    
    printf("Enter an infix expression: ");
    scanf("%s", infixExpression);
    
    char *postfixExpression = infixToPostfix(infixExpression);
    generateThreeAddressCode(postfixExpression);

    return 0;
}
