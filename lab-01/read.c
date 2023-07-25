// Reading from the input.txt file
#include <stdio.h>
int main() 
{
    FILE* ifp;
    char str[100];

    ifp = fopen("input.txt", "r");
    if(ifp == NULL)
        printf("File can't be opened");
    else 
    {
        while(fgets(str, 100, ifp))
        {
            printf("%s", str);
        }
    }

    fclose(ifp);

    return 0;
}