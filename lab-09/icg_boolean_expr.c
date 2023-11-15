// CS21B2019
// DEVARAKONDA SLR SIDDESH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct
{
    int key;
    char value[1000];
    int true_label;
    int false_label;
} Expression;

int expression_count, label_count;
Expression *expressions = NULL;
Expression *intermediate_code = NULL;

Expression *initialize_expression_array(int n)
{
    return (Expression *)malloc(sizeof(Expression) * n);
}

int index_of(Expression expr_array[], int n, int key)
{
    for (int i = 0; i < n; i++)
    {
        if (expr_array[i].key == key)
        {
            return i;
        }
    }
    return -1;
}

typedef struct
{
    char data[MAX];
    int top;
    int capacity;
} Stack;

Stack initialize_stack()
{
    Stack stack;
    stack.top = -1;
    stack.capacity = MAX;

    return stack;
}

void push_stack(Stack *stack, char c)
{
    if (stack->top == stack->capacity - 1)
        return;

    stack->data[++stack->top] = c;
}

char pop_stack(Stack *stack)
{
    if (stack->top < 0)
        return '\0';

    char res = stack->data[stack->top];
    stack->data[stack->top--] = '\0';
    return res;
}

char top_stack(Stack stack)
{
    if (stack.top < 0)
        return '\0';

    return stack.data[stack.top];
}

int is_stack_empty(Stack stack)
{
    return (stack.top < 0);
}

int operator_precedence(char c)
{
    switch (c)
    {
    case '|':
        return 1;
    case '&':
        return 2;
    default:
        return 0;
    }
}

int is_boolean_operator(char c)
{
    return (c == '|' || c == '&');
}

int is_comparison_operator(char c)
{
    return (c == '<' || c == '>');
}

char *preprocess_expression(char *infix)
{
    int n = strlen(infix);
    char *new_infix = (char *)calloc(2 * n, 2 * n);
    int j = 0;

    int prev_comp = 0;
    for (int i = 0; i < n; i++)
    {
        if (is_comparison_operator(infix[i]))
        {
            if (prev_comp == 1)
            {
                strcat(new_infix, "&&");
                new_infix[j + 2] = new_infix[j - 1];
                j += 3;
            }
            prev_comp = 1;
        }
        else if (!isalpha(infix[i]))
        {
            prev_comp = 0;
        }
        new_infix[j++] = infix[i];
    }

    return new_infix;
}

char *infix_to_postfix_conversion(char *infix)
{
    int n = strlen(infix);
    Stack stack = initialize_stack();
    char *postfix = (char *)calloc(n + 1, n + 1);
    int j = 0;

    expressions = initialize_expression_array(n);
    expression_count = 0;
    int expression_num = 'A';

    for (int i = 0; i < n; i++)
    {
        if (isalpha(infix[i]))
        {
            // Get the expression string
            // Store it as a key-value pair
            expressions[expression_count].key = expression_num;
            int k = 0;
            while (i < n && !is_boolean_operator(infix[i]))
            {
                expressions[expression_count].value[k++] = infix[i++];
            }
            expressions[expression_count].value[k] = '\0';
            i--;

            printf("%c -> %s\n", expressions[expression_count].key, expressions[expression_count].value);
            expression_count++;

            postfix[j++] = expression_num++;
        }
        else if (infix[i] == '(')
        {
            push_stack(&stack, '(');
        }
        else if (infix[i] == ')')
        {
            while (top_stack(stack) != '(')
            {
                postfix[j++] = pop_stack(&stack);
            }
            pop_stack(&stack); // pop '(' but don't push
        }
        else // operators
        {
            int current_precedence = operator_precedence(infix[i]);
            while (operator_precedence(top_stack(stack)) >= current_precedence)
            {
                postfix[j++] = pop_stack(&stack);
            }
            push_stack(&stack, infix[i]);
            i++;
        }
    }

    while (!is_stack_empty(stack))
    {
        postfix[j++] = pop_stack(&stack);
    }

    return postfix;
}

Expression *generate_condition_code(int expr_key, int *label)
{
    int i = index_of(intermediate_code, label_count, expr_key);
    if (i != -1)
    {
        return &intermediate_code[i];
    }

    int expr_index = index_of(expressions, expression_count, expr_key);
    if (expr_index == -1)
    {
        printf("Invalid expr_key %d\n", expr_key);
        return NULL;
    }
    intermediate_code[label_count].key = expr_key;
    sprintf(intermediate_code[label_count].value, "if %s goto L%d\ngoto L%d\n", expressions[expr_index].value, *label, *label + 1);
    intermediate_code[label_count].true_label = *label;
    intermediate_code[label_count].false_label = *label + 1;
    *label += 2;

    return &intermediate_code[label_count++];
}

void generate_three_address_code(char *postfix)
{
    Stack stack = initialize_stack();
    int n = strlen(postfix);

    intermediate_code = initialize_expression_array(n);
    label_count = 0; // index for intermediate_code

    int label = 1; // label number

    for (int i = 0; i < n; i++)
    {
        if (isalpha(postfix[i]))
        {
            push_stack(&stack, postfix[i]);
        }
        else
        {
            int exp2 = pop_stack(&stack);
            int exp1 = pop_stack(&stack);
            char operator = postfix[i];

            Expression *exp1_code = generate_condition_code(exp1, &label);
            Expression *exp2_code = generate_condition_code(exp2, &label);

            intermediate_code[label_count].key = label_count;
            char *combined_code = intermediate_code[label_count].value;
            if (operator == '&')
            {
                sprintf(combined_code, "%sL%d: %s", exp1_code->value, exp1_code->true_label, exp2_code->value);
                sprintf(combined_code + strlen(combined_code), "L%d: T=1\ngoto L%d\n", exp2_code->true_label, label);
                sprintf(combined_code + strlen(combined_code), "L%d: L%d: T=0\ngoto L%d\n", exp1_code->false_label, exp2_code->false_label, label + 1);
            }
            else if (operator == '|')
            {
                sprintf(combined_code, "%sL%d: %s", exp1_code->value, exp1_code->false_label, exp2_code->value);
                sprintf(combined_code + strlen(combined_code), "L%d: L%d: T=1\ngoto L%d\n", exp1_code->true_label, exp2_code->true_label, label);
                sprintf(combined_code + strlen(combined_code), "L%d: T=0\ngoto L%d\n", exp2_code->false_label, label + 1);
            }
            intermediate_code[label_count].true_label = label;
            intermediate_code[label_count].false_label = label + 1;
            label += 2;

            push_stack(&stack, label_count);
            label_count++;
        }
    }
    char *final_code = intermediate_code[label_count - 1].value;
    sprintf(final_code + strlen(final_code), "L%d: L%d: next code\n", label - 2, label - 1);

    printf("%s", final_code);
}

void intermediate_code_generation(char *infix)
{
    char *new_infix = preprocess_expression(infix);
    printf("Infix: %s\n", new_infix);

    char *postfix = infix_to_postfix_conversion(new_infix);
    printf("Postfix: %s\n", postfix);

    printf("\nIntermediate code with short circuit evaluation:\n");
    generate_three_address_code(postfix);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        argv[1] = "a>b||c<d";
    }

    intermediate_code_generation(argv[1]);

    return 0;
}
