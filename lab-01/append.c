// Without effecting the contents of output.txt, append some sentences to output.txt
#include <stdio.h>
int main() 
{
    FILE* ofp;

    ofp = fopen("output.txt", "a");
    if(ofp == NULL)
        printf("File can't be opened");
    else 
    {
        char *str = "Appending this line to output.txt\n";
        fputs(str, ofp);
        printf("Appended successfully to output.txt\n");
    }

    fclose(ofp);

    return 0;
}