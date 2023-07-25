// Reading the contents from input.txt and writing them to output.txt file
#include <stdio.h>
int main() 
{
    FILE* ifp;
    FILE* ofp;
    char str[100];

    ifp = fopen("input.txt", "r");
    ofp = fopen("output.txt", "w");

    if(ifp == NULL)
        printf("Input file can't be opened");
    else if(ofp == NULL)
        printf("Error! while creating file");
    else
    {
        while(fgets(str, 100, ifp))
        {
            fputs(str, ofp);
        }
        printf("File created successfully and written.\n");
    }

    fclose(ifp);
    fclose(ofp);

    return 0;
}