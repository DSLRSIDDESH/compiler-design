// CS21B2019 DEVARAKONDA SLR SIDDESH

/* 
Context Free Grammar :
    S -> ABC
    A -> aA | a
    B -> bB | b
    C -> cC | c
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 1000

char token, string[MAX];
int ind = 0;

void accept()
{
    printf("Status   : String accepted\n");
    exit(0);
}

void reject()
{
    printf("Status   : String rejected\n");
    exit(0);
}

void get_token()
{
    token = string[ind++];
}

void S(), A(), B(), C();

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Provide string as an argument in command line!!\n");
        exit(1);
    }

    printf("Rules : \n");
    printf("\tS -> ABC\n\tA -> aA | a\n\tB -> bB | b\n\tC -> cC | c\n\n");
    printf("Language : {a^n b^m c^k | n, m, k >= 1}\n");
    printf("String   : %s\n", argv[1]);

    strcpy(string, argv[1]);
    int l = strlen(string);

    string[l++] = '$';
    string[l] = '\0';

    get_token();
    S();
    if (token == '$')
        accept();
    else
        reject();

    return 0;
}

void S()
{
    A();
    B();
    C();
}

void A()
{
    switch (token)
    {
        case 'a':
            get_token();
            A();
            break;
        case 'b':
            return;
        default:
            reject();
    }
}

void B()
{
    switch (token)
    {
        case 'b':
            get_token();
            B();
            break;
        case 'c':
            return;
        default:
            reject();
    }
}

void C()
{
    switch (token)
    {
        case 'c':
            get_token();
            C();
            break;
    }
}