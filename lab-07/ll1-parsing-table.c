// Roll: CS21B2019
// Name: Devarakonda SLR Siddesh

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

typedef struct {
    char nonTerminal;
    char production[MAX][MAX];
    int noOfProductions;
    char dirsymb[MAX][MAX];
} Rule;

typedef struct {
    char nonTerminal;
    char dirsymb[MAX][MAX];
} dirsymbSet;

typedef struct {
    char startSymbol;
    char nonTerminals[MAX];
    char terminals[MAX];
    Rule rules[MAX];
    int noOfRules;
} CFG;

typedef struct {
    char symbol;
    char first[MAX];
    char follow[MAX];
} firstFollow;

CFG cfg;
firstFollow table[MAX];
dirsymbSet dirsym[MAX];
char parsing_table[MAX][MAX][MAX];

void print_terms()
{
    printf("Terminals    : {");
    for(int i = 0; i < strlen(cfg.terminals) - 1; i++)
        printf("%c, ", cfg.terminals[i]);
    printf("%c}\n", cfg.terminals[strlen(cfg.terminals) - 1]);
}

void print_non_terms()
{
    printf("Non Terminals: {");
    for(int i = 0; i < strlen(cfg.nonTerminals) - 1; i++)
        printf("%c, ", cfg.nonTerminals[i]);
    printf("%c}\n", cfg.nonTerminals[strlen(cfg.nonTerminals) - 1]);
}

void print_cfg()
{
    printf("CFG:\n");
    printf("Start Symbol : %c\n", cfg.startSymbol);
    print_non_terms();
    print_terms();
    printf("Rules:\n");
    for(int i = 0; i < strlen(cfg.nonTerminals); i++)
    {
        printf("\t%c -> ", cfg.rules[i].nonTerminal);
        for(int j = 0; j < cfg.rules[i].noOfProductions - 1; j++)
        {
            printf("%s | ", cfg.rules[i].production[j]);
        }
        printf("%s", cfg.rules[i].production[cfg.rules[i].noOfProductions - 1]);
        printf("\n");
    }
    printf("\n");
}

int insert_non_term(char non_term)
{
    if(strchr(cfg.nonTerminals, non_term) == NULL)
    {
        cfg.nonTerminals[strlen(cfg.nonTerminals)] = non_term;
        return 0;
    }
    return 1;
}

void insert_term(char letter)
{
    if(strchr(cfg.terminals, letter) == NULL)
        cfg.terminals[strlen(cfg.terminals)] = letter;
}

void printTable(Rule table[][MAX], int n_nt, int n_t)
{
    printf("LL(1) Table:\n");
    printf("\t\t");
    for(int i = 0; i < n_t; i++)
    {
        printf("%c\t\t", cfg.terminals[i]);
    }
    printf("\n");
    for(int i = 0; i < n_nt; i++)
    {
        printf("\t%c\t", cfg.nonTerminals[i]);
        for(int j = 0; j < n_t; j++)
        {
            if(table[i][j].production[0][0] != '\0')
                printf("%c->%s\t\t", cfg.nonTerminals[i], table[i][j].production[0]);
            else
                printf("\t\t");
        }
        printf("\n");
    }
}

int index_in_table(firstFollow table[], char symbol, int n_s)
{
    for(int i = 0; i < n_s; i++)
        if(table[i].symbol == symbol)
        {
            return i;
        }
    printf("Trying to find index of %c in ", symbol);
    print_non_terms();
    exit(1);
}

int if_epsilon(char string[])
{
    for(int i = 0; i < strlen(string); i++)
        if(string[i] == '#')
            return 1;
    return 0;
}

char* remove_epsilon(char first[])
{
    char *string = (char*)malloc(sizeof(char) * MAX);
    int l = strlen(first), j = 0;
    for(int i = 0; i < l; i++)
    {
        if(first[i] != '#')
            string[j] = first[i];
        j++;
    }
    return string;
}

int sets_union(char *src, char *dest, int changed)
{
    int l = strlen(src);
    for(int i = 0; i < strlen(dest); i++)
    {
        if(strchr(src, dest[i]) == NULL)
        {
            src[l] = dest[i];
            changed = 1;
            l++;
        }
    }
    return changed;
}

void print_first(firstFollow table[], int n_s)
{
    printf("First-Sets:\n");
    for(int i = 0; i < n_s; i++)
    {
        printf("\tFIRST(%c) = { ", table[i].symbol);
        for(int j = 0; j < strlen(table[i].first) - 1; j++)
            printf("%c, ", table[i].first[j]);
        printf("%c }\n", table[i].first[strlen(table[i].first) - 1]);
    }
}

void print_follow(firstFollow table[], int n_s)
{
    printf("\nFollow-Sets:\n");
    for(int i = 0; i < n_s; i++)
    {
        printf("\tFOLLOW(%c) = { ", table[i].symbol);
        int n = strlen(table[i].follow);
        for(int j = 0; j < n - 1; j++)
            printf("%c, ", table[i].follow[j]);
        printf("%c }\n", table[i].follow[n - 1]);
    }
    printf("\n");
}

void print_dirsymb(CFG cfg, dirsymbSet dirsym[])
{
    printf("\nDirect Symbol Sets:\n");
    for(int i = 0; i < cfg.noOfRules; i++)
    {
        printf("\t%c", cfg.rules[i].nonTerminal);
        for(int j = 0; j < cfg.rules[i].noOfProductions; j++)
        {
            printf(" -> %s ==> ", cfg.rules[i].production[j]);
            printf("{ ");
            for(int k = 0; k < strlen(dirsym[i].dirsymb[j]) - 1; k++)
                printf("%c, ", dirsym[i].dirsymb[j][k]);
            printf("%c }\n\t ", dirsym[i].dirsymb[j][strlen(dirsym[i].dirsymb[j]) - 1]);
        }
        printf("\n");
    }
}

void first(CFG cfg)
{
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    for(int i = 0; i < n_t; i++)    // for each (a ∈ T) FIRST(a) = {a}; FIRST(#) = {#}; # = epsilon
    {
        table[i].symbol = cfg.terminals[i];
        table[i].first[0] = cfg.terminals[i];
    }
    for(int i = n_t; i < n_s; i++)  // for each (A ∈ N) FIRST(A) = ∅
    {
        table[i].symbol = cfg.nonTerminals[i - n_t];
        strcpy(table[i].first, "");
    }
    int changing = 1;
    while(changing)
    {
        changing = 0;
        for(int i = 0; i < cfg.noOfRules; i++)
        {
            for(int j = 0; j < cfg.rules[i].noOfProductions; j++)
            {
                char nonTerminal = cfg.rules[i].nonTerminal;

                char prod[MAX];
                strcpy(prod, cfg.rules[i].production[j]);

                int index = index_in_table(table, nonTerminal, n_s);
                int k = 0, n = strlen(prod), for_first_x = 1;

                int x_index = index_in_table(table, prod[k], n_s);
                while((if_epsilon(table[x_index].first) && k <= n - 2) || for_first_x)
                {
                    if(for_first_x)
                    {
                        for_first_x = 0;
                        k = -1;
                    }
                    x_index = index_in_table(table, prod[k + 1], n_s);
                    char *x_first_set = remove_epsilon(table[x_index].first);
                    changing = sets_union(table[index].first, x_first_set, changing);
                    k++;
                }
                if(k == n - 1 && if_epsilon(table[x_index].first))
                {
                    char *epsilon = "#";
                    changing = sets_union(table[index].first, epsilon, changing);
                }
            }
        }
    }
    print_first(table, n_s);
}

void follow(CFG cfg)
{
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    for(int i = 0; i < n_t; i++)    // for each (a ∈ T) FIRST(a) = ∅
    {
        table[i].symbol = cfg.terminals[i];
        strcpy(table[i].follow, "");
    }
    for(int i = n_t; i < n_s; i++)  // for each (A ∈ N) FIRST(A) = ∅
    {
        table[i].symbol = cfg.nonTerminals[i - n_t];
        strcpy(table[i].follow, "");
    }
    for(int i = 0; i < n_s; i++)    // FOLLOW(S) = {$}
    {
        if(table[i].symbol == cfg.startSymbol)
            table[i].follow[0] = '$';
    }
    int changing = 1;
    while(changing)
    {
        changing = 0;
        for(int i = 0; i < cfg.noOfRules; i++)
        {
            for(int j = 0; j < cfg.rules[i].noOfProductions; j++)
            {
                char nonTerminal = cfg.rules[i].nonTerminal;
                char *prod = remove_epsilon(cfg.rules[i].production[j]);
                if(strlen(prod) == 0)
                    continue;

                int index = index_in_table(table, nonTerminal, n_s);
                int k = 0, n = strlen(prod), for_first_x = 1;

                int x_index = index_in_table(table, prod[strlen(prod)-1], n_s);
                changing = sets_union(table[x_index].follow, table[index].follow, changing);

                char *rest = table[index].follow;
                for(int k = strlen(prod)-1; k >= 1; k--)
                {
                    int x_i_index = index_in_table(table, prod[k], n_s);
                    char *x_i_first_set = table[x_i_index].first;
                    char x_i_1 = prod[k - 1];
                    int x_i_1_index = index_in_table(table, prod[k - 1], n_s);
                    if(if_epsilon(x_i_first_set))
                    {
                        char *x_i_first_set = remove_epsilon(table[x_i_index].first);
                        changing = sets_union(table[x_i_1_index].follow, x_i_first_set, changing);
                        changing = sets_union(table[x_i_1_index].follow, rest, changing);
                    }
                    else
                    {
                        changing = sets_union(table[x_i_1_index].follow, x_i_first_set, changing);
                    }
                    rest = table[x_i_index].follow;
                }
            }
        }
    }
    print_follow(table, n_s);
}

void dirsymb_set(CFG cfg) {
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    for (int i = 0; i < cfg.noOfRules; i++) 
    {
        for (int j = 0; j < cfg.rules[i].noOfProductions; j++) 
        {
            char dirsymb[MAX] = "";
            char production[MAX];
            char nonTerm;
            int epsilon_exists = 0;
            int include_follow = 0;

            nonTerm = cfg.rules[i].nonTerminal;
            strcpy(production, cfg.rules[i].production[j]);

            for (int k = 0; k < strlen(production); k++) 
            {
                if (k == 0 || epsilon_exists) {
                    if (islower(production[k]) || production[k] == '$') 
                    {
                        char str[2] = { production[k], '\0' };
                        sets_union(dirsymb, str, 0);
                        break;
                    } 
                    else if (isupper(production[k])) 
                    {
                        int index = index_in_table(table, production[k], n_s);
                        char *first = table[index].first;
                        epsilon_exists = if_epsilon(first);
                        if (epsilon_exists)
                            first = remove_epsilon(first);
                        sets_union(dirsymb, first, 0);
                    } else if (production[k] == '#')
                        epsilon_exists = 1;
                }
            }
            if (epsilon_exists)
                include_follow = 1;
            if (include_follow)
            {
                int index = index_in_table(table, nonTerm, n_s);
                sets_union(dirsymb, table[index].follow, 0);
            }
            strcpy(dirsym[i].dirsymb[j], dirsymb);
        }
    }
    print_dirsymb(cfg, dirsym);
}

void printParsingTable(char table[][MAX][MAX], int n_nt, int n_t)
{
    printf("LL(1) Parsing Table:\n");
    printf("\t\t");
    for(int i = 0; i < n_t; i++)
    {
        printf("%c\t\t\t", cfg.terminals[i]);
    }
    printf("\n");
    for(int i = 0; i < n_nt; i++)
    {
        printf("\t%c\t", cfg.nonTerminals[i]);
        for(int j = 0; j < n_t; j++)
        {
            if(strcmp(table[i][j], "") != 0)
                printf("%c->%s\t\t\t", cfg.nonTerminals[i], table[i][j]);
            else
                printf("\t\t\t");
        }
        printf("\n");
    }
}

void parsingTable()
{
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int check = 0;

    for(int i = 0; i < n_nt; i++)
    {
        for(int j = 0; j < n_t; j++)
        {
            strcpy(parsing_table[i][j], "");
        }
    }
    for(int i = 0; i < cfg.noOfRules; i++)
    {
        for(int j = 0; j < n_t; j++)
        {
            for(int k = 0; k < cfg.rules[i].noOfProductions; k++)
            {
                if(strchr(dirsym[i].dirsymb[k], cfg.terminals[j]) != NULL)
                {   
                    if(strcmp(parsing_table[i][j],"") == 0)
                        strcpy(parsing_table[i][j], cfg.rules[i].production[k]);
                    else
                    {
                        check = 1;
                        strcat(parsing_table[i][j], " | ");
                        strcat(parsing_table[i][j], cfg.rules[i].production[k]);
                    }
                }
            }
        }
    }
    printParsingTable(parsing_table, n_nt, n_t);
    if(check)
    {
        printf("\n\n[STATUS] Grammar is not LL(1)\n");
        exit(1);
    }
}

void parsing(char s[])
{
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int n_s = n_nt + n_t;

    char stack[MAX] = "";
    int top = -1;
    stack[++top] = cfg.startSymbol;

    int i = 0;
    while(stack[top] != '$')
    {
        char x = stack[top];
        char a = s[i];
        if(x == a)
        {
            top--;
            i++;
        }
        else if(isupper(x))
        {
            int i = strchr(cfg.nonTerminals, x) - cfg.nonTerminals;
            int j = strchr(cfg.terminals, a) - cfg.terminals;
            if(strcmp(parsing_table[i][j], "") == 0)
            {
                printf("\n[STATUS] String is rejected\n");
                exit(1);
            }
            else
            {
                top--;
                char *prod = parsing_table[i][j];
                for(int k = strlen(prod) - 1; k >= 0; k--)
                {
                    stack[++top] = prod[k];
                }
            }
        }
        else
        {
            printf("\n[STATUS] String is rejected\n");
            exit(1);
        }
    }
    if(stack[top] == '$' && i == strlen(s))
        printf("\n[STATUS] String is accepted\n");
    else
        printf("\n[STATUS] String is rejected\n");
}

int main()
{
    FILE *fp;
    char str[MAX];
    int ruleCount = 0;
    char string[MAX];

    fp = fopen("CFG", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    while(fgets(str, MAX, fp))
    {
        ruleCount++;
        Rule rule;

        rule.nonTerminal = strtok(str, " =\n")[0];
        strcpy(rule.production[0], strtok(NULL, " =\n"));
        // for inserting non terminal
        int exist = insert_non_term(rule.nonTerminal);
        if(!exist)
        {
            cfg.rules[strlen(cfg.nonTerminals) - 1].nonTerminal = rule.nonTerminal;
        }
        if(ruleCount == 1)
            cfg.startSymbol = rule.nonTerminal;
        // for inserting production
        for(int i = 0; i < strlen(cfg.nonTerminals) ; i++)
        {
            if(cfg.rules[i].nonTerminal == rule.nonTerminal)
            {
                int j = cfg.rules[i].noOfProductions;
                strcpy(cfg.rules[i].production[j], rule.production[0]);
                strcpy(cfg.rules[i].dirsymb[j], "");
                cfg.rules[i].noOfProductions++;
                break;
            }
        }
        // for inserting terminals
        for(int i = 0; i < strlen(rule.production[0]); i++)
        {
            if(!isupper(rule.production[0][i])) //  && rule.production[0][i] != '#'
            {
                insert_term(rule.production[0][i]);
            }
        }
    }
    cfg.noOfRules = ruleCount;
    print_cfg();
    first(cfg);
    follow(cfg);
    dirsymb_set(cfg);
    parsingTable();

    printf("\n\nEnter the string : ");
    scanf("%s", string);

    parsing(string);

    return 0;
}