// CS21B2019 DSLR SIDDESH
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1000
#define MAX_TOKEN_LENGTH 100
int commentState = 0;

int kwCount = 0;
int idCount = 0;
int opCount = 0;
int scCount = 0;
int cnCount = 0;
int stCount = 0;

char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned",
    "void", "volatile", "while"
};
int num_keywords = sizeof(keywords) / sizeof(keywords[0]);

typedef enum {
    KEYWORD,
    IDENTIFIER,
    CONSTANT
} TokenType;

int isKeyword(char* token) {
    for (int i = 0; i < num_keywords; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isOperator(char ch) {
    char operators[] = "+-*/%=<>&|^!~?:";
    for (int i = 0; i < strlen(operators); i++) {
        if (ch == operators[i]) {
            return 1;
        }
    }
    return 0;
}

int isSpecialCharacter(char ch) {
    char special_characters[] = "{}[]();,";
    for (int i = 0; i < strlen(special_characters); i++) {
        if (ch == special_characters[i]) {
            return 1;
        }
    }
    return 0;
}

int isWhitespace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n' || ch == '#';
}

char* removeComments(char *str)
{
    int n = strlen(str);
    for(int i=0; i<n; i++)
    {
        switch (commentState)
        {
            case 0:
                if(str[i] == '/')
                    commentState = 1;
                else if(str[i] == '"')
                    commentState = 2;
                else if(str[i] == '#')
                {
                    str[i] = '\0';
                    commentState = 3;
                }
                break;
            case 1:
                if(str[i] == '/') {
                    str[i-1] = str[i] = ' ';
                    commentState = 3;
                }
                else if(str[i] == '*') {
                    str[i-1] = str[i] = ' ';
                    commentState = 4;
                }
                else
                    commentState = 0;
                break;
            case 2:
                if(str[i] == '"')
                    commentState = 0;
                break;
            case 3:
                if(str[i] == '\n' || str[i] == '\0')
                    commentState = 0;
                str[i] = ' ';
            case 4:
                if(str[i] == '*')
                    commentState = 5;
                str[i] = ' ';
                break;
            case 5:
                if(str[i] == '/'){
                    str[i-1] = str[i] = ' ';
                    commentState = 0;
                }
                else if(str[i] == '*')
                    commentState = 5;
                else
                    commentState = 4;
                str[i] = ' ';
                break;
        }
        if(commentState == 3)
        {
            commentState = 0;
            break;
        }
    }
    return str;
}


TokenType getTokenType(char *token){
    if(isKeyword(token)){
        return KEYWORD;
    }
    else if(isdigit(token[0])){
        int i=0;
        while(token[i]!='\0'){
            if(!isdigit(token[i]) && token[i]!='.'){
                return IDENTIFIER;
            }
            i++;
        }
        return CONSTANT;
    }
    else{
        return IDENTIFIER;
    }
}

int main(int argc, char *argv[]){
    if(argc != 2){
        printf("Invalid number of arguments\n");
        return 0;
    }

    FILE *ifp, *ofp;
    char line[MAX_LINE_LENGTH];
    char token[MAX_TOKEN_LENGTH];

    ifp = fopen(argv[1], "r");
    ofp = fopen("output.txt", "w");

    if(ifp == NULL || ofp == NULL)
    {
        printf("Can't open the file %s", argv[1]);
        return 0;
    }
    while(fgets(line, MAX_LINE_LENGTH, ifp))
    {
        int k = 0;
        while(line[k] == ' ')
            k++;
        if(line[k] == '/' && line[k+1] == '/')
            continue;
        char* str = removeComments(line);
        int i = 0;
        while(str[i] != '\0')
        {
            if(isWhitespace(str[i]))
            {
                i++;
                continue;
            }
            else if(isOperator(str[i]))
            {
                int j = 0;
                token[j++] = str[i++];
                while(isOperator(str[i]))
                {
                    token[j++] = str[i++];
                }
                token[j] = '\0';
                fprintf(ofp, "%s : operator\n", token);
                opCount++;
            }
            else if(isSpecialCharacter(str[i]))
            {
                fprintf(ofp, "%c : special character\n", str[i]);
                i++;
                scCount++;
            }
            else if(str[i] == '"')
            {
                int j = 0;
                token[j++] = str[i++];
                while(str[i] != '"')
                {
                    token[j++] = str[i++];
                }
                token[j++] = str[i++];
                token[j] = '\0';
                fprintf(ofp, "%s : string\n", token);
                stCount++;
            }
            else
            {
                int j=0;
                while(!isWhitespace(str[i]) && !isOperator(str[i]) && !isSpecialCharacter(str[i]) && str[i]!='\0')
                {
                    token[j++] = str[i++];
                }
                token[j] = '\0';
                TokenType token_type = getTokenType(token);
                switch(token_type)
                {
                    case KEYWORD:
                        fprintf(ofp, "%s : keyword\n", token);
                        kwCount++;
                        break;
                    case IDENTIFIER:
                        fprintf(ofp, "%s : identifier\n", token);
                        idCount++;
                        break;
                    case CONSTANT:
                        fprintf(ofp, "%s : constant\n", token);
                        cnCount++;
                        break;
                }
            }
        }
    }

    printf("\nOutput is written to output.txt\n");
    printf("Keywords           : %d\n", kwCount);
    printf("Identifiers        : %d\n", idCount);
    printf("Operators          : %d\n", opCount);
    printf("Special Characters : %d\n", scCount);
    printf("Constants          : %d\n", cnCount);
    printf("Strings            : %d\n", stCount);
    printf("Total              : %d\n", kwCount+idCount+opCount+scCount+cnCount+stCount);

    return 0;
}