// CS21B2019 DSLR SIDDESH
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int main(int argc, char *argv[])
{
    FILE* fptr;
    char str[1000];
    int lines = 0, words = 0, chars = 0;
    int vowels = 0, consonants = 0, digits = 0, others = 0;

    fptr = fopen(argv[1], "r");
    if(fptr == NULL)
        printf("Can't open the file!!");
    else
    {
        while(fgets(str, 1000, fptr))
        {
            lines++;
            chars += strlen(str);
            for(int i=0; i<strlen(str); i++)
            {
                if(isalpha(str[i]))
                {
                    if(str[i]=='a' || str[i]=='e' || str[i]=='i' || str[i]=='o' || str[i]=='u' || str[i]=='A' || str[i]=='E' || str[i]=='I' || str[i]=='O' || str[i]=='U')
                        vowels++;
                    else
                        consonants++;
                }
                else if(isdigit(str[i]))
                    digits++;
                else
                    others++;
            }
            char *token = strtok(str, "\n\t ");
            while (token != NULL) 
            {
                words++;
                token = strtok(NULL, "\n\t ");
            }
        }
        printf("\nNumber of lines      : %d\n",lines);
        printf("Number of words      : %d\n",words);
        printf("Number of characters : %d\n",chars);
        printf("Number of vowels     : %d\n",vowels);
        printf("Number of consonants : %d\n",consonants);
        printf("Number of digits     : %d\n",digits);
        printf("Number of others     : %d\n",others);
        printf("Total                : %d\n", vowels + consonants + digits + others);
    }
    fclose(fptr);
    return 0;
}