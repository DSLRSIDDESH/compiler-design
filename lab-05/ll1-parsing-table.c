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

CFG cfg;

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

char *next_terms(CFG *cf, char *prod)
{
    char next[MAX];
    memset(next, 0, sizeof(next));
    if (isupper(prod[0]))
    {
        int n = strchr(cf->nonTerminals, prod[0]) - cf->nonTerminals;
        for (int i = 0; i < cf->rules[n].noOfProductions; i++)
        {
            strcat(next, next_terms(cf, cf->rules[n].production[i]));
        }
    }
    else
    {
        next[0] = prod[0];
    }
    char *ret = malloc(sizeof(next));
    strcpy(ret, next);
    return ret;
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

void ll1_table()
{
    // Table
    int n_nt = strlen(cfg.nonTerminals);
    int n_t = strlen(cfg.terminals);
    int n_r = cfg.noOfRules;
    Rule table[n_nt][MAX];
    
    int total = 0;
    // For all rules with a non-terminal
    for (int i = 0; i < n_r; i++)
    {
        // For all productions of the rule
        for (int j = 0; j < cfg.rules[i].noOfProductions; j++)
        {
            char *next = next_terms(&cfg, cfg.rules[i].production[j]);
            for (int k = 0; k < strlen(next); k++)
            {
                int n = strchr(cfg.nonTerminals, cfg.rules[i].nonTerminal) - cfg.nonTerminals;
                int m = strchr(cfg.terminals, next[k]) - cfg.terminals;
                strcpy(table[n][m].production[0], cfg.rules[i].production[j]);
                total++;
            }
        }
    }
    printTable(table, n_nt, n_t);
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
            if(!isupper(rule.production[0][i]))
            {
                insert_term(rule.production[0][i]);
            }
        }
    }
    cfg.noOfRules = ruleCount;
    print_cfg();
    ll1_table();

    return 0;
}