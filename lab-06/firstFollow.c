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
} Rule;

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
            return i;
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
        printf("\tFIRST(%c) = {", table[i].symbol);
        printf("%s}\n", table[i].first);
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
                if(k == n - 1 || if_epsilon(table[x_index].first))
                {
                    char *epsilon = "#";
                    changing = sets_union(table[index].first, epsilon, changing);
                }
            }
        }
    }
    print_first(table, n_s);
}

int main()
{
    FILE *fp;
    char str[MAX];
    int ruleCount = 0;

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
            cfg.rules[strlen(cfg.nonTerminals) - 1].nonTerminal = rule.nonTerminal;
        if(ruleCount == 1)
            cfg.startSymbol = rule.nonTerminal;
        // for inserting production
        for(int i = 0; i < strlen(cfg.nonTerminals) ; i++)
        {
            if(cfg.rules[i].nonTerminal == rule.nonTerminal)
            {
                int j = cfg.rules[i].noOfProductions;
                strcpy(cfg.rules[i].production[j], rule.production[0]);
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

    return 0;
}