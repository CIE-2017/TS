#include <stdio.h>

 

void main()

{

    FILE *fptr;

    char name[20];

    int password;


 

    /*  open for writing */

    fptr = fopen("emp.rec", "w");

 

    if (fptr == NULL)

    {

        printf("File does not exists \n");

        return;

    }

    printf("Enter the name \n");

    scanf("%s", name);

    fprintf(fptr, "Name    = %s\n", name);

    printf("Enter the password\n");

    scanf("%d", &password);

    fprintf(fptr, "Password     = %d\n", password);
    
    fclose(fptr);

    }
//https://www.sanfoundry.com/c-program-create-file-store-information/



