#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "iplfunction.h"
#include "ipllexer.h"
//#define DEBUG


int main(int argc, char *argv[])
{
    int x;                                                          
    
    srand(time(NULL));
    list_head = NULL;       
    listvar_head = NULL;
    listwh_head = NULL;
    if (strcmp(argv[1], "-v") != 0)   /*if argument -v exist the name of the ipl programm is the argv[2] else is the argv[1]*/
    {    
        x = detacher(argv[1]);                                
    }
    else
    {
        x = detacher(argv[2]);
    }
    if (x != 0)
    {
        printf("\nError! Could not detach comments.\n");
        return -1;
    }
    x = lexer();                                                
    if (x != 0)
    {
        printf("\nError! Could not finish lexical analysis.\n");
        return -1;
    }
    #ifdef DEBUG
        printf("\nToken list is: \n");
        printlist(list_head);
    #endif
    x = parser(argc, argv);
    if (x != 0)
    {
        printf("\nError! Could not finish parsing.\n");
        return -1;
    }
    #ifdef DEBUG
        printf("\nIdentifier list is: \n");
        printlist(listvar_head);
        printf("\nWhile list is: \n");
        printwhlist(listwh_head);
        printf("\nArray list is: \n");
        printarraylist(listarray_head);
    #else
        remove("out.txt");
    #endif
    while(list_head != NULL)              /*delete all lists*/
    {
        deletelist(&list_head);
    }
    while(listvar_head != NULL)
    {
        deletelist(&listvar_head);
    }
    while(listwh_head != NULL)
    {
        delete(&listwh_head);
    }
    while(listarray_head != NULL)
    {
        deletearray(&listarray_head, listarray_head->name);
    }
    return 0;
}

int parser(int argc, char *argv[]) 
{                                  
    struct listnode *adr, *adrvar;                          
    int tokennum;                                        
    char x[105];                                            
    int j, k;                                               
    int x1, y1;
    int counter = 0;               /*counter counts nodes*/
    int curdepth = 0;              /*curdepth counts current depth*/
    int tempcounter;               
    int tabcount = 0;              /*tabcount counts tabs of its line*/
    int diff;                    
    int flag = 1;
    int elflag = 0;
    char *tempname;

    adr = malloc(sizeof(struct listnode));
    adr->name = malloc(105*sizeof(char));
    n_th(list_head, 1, adr);                        /*returns content of n-th node*/
    adrvar = malloc(sizeof(struct listnode));
    adrvar->name = malloc(105*sizeof(char));
    n_th(listvar_head, 1, adrvar);
    while(adr->next != NULL)                
    {
        tokennum = -1;
        counter++; 
        n_th(list_head, counter, adr);
        if (adr->type == TOKENIZER_ENDOFLINE)
        {
            continue;
        }
        tabcount = 0;
        if(adr->type == TOKENIZER_TAB)                
        {
            tabcount++;     /*count tabs*/
            while (1 == 1)
            {
                counter++;
                n_th(list_head, counter, adr);
                if (adr->type != TOKENIZER_TAB)
                {
                    break;
                }
                tabcount++;
            }
            if(adr->type == TOKENIZER_ENDOFLINE)
            {
                continue;
            }
        }
        else
        {
            tabcount = 0;
        }
        if ((tabcount == curdepth) || ((tabcount > curdepth) && listwh_head->true == 0))
        {
            if ((tabcount > 0) && (listwh_head->true == 0))
            {
                while (1 == 1)
                {                   /*ignore tokens*/
                    counter++;
                    n_th(list_head, counter, adr);  
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        break;
                    }
                }
            }
            if (token(adr->name) == TOKENIZER_BREAK)
            {
                if (strcmp(argv[1], "-v") == 0)
                {   
                    printline(adr->line);
                }
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type == TOKENIZER_ENDOFLINE)   /*if after break there is no numer*/
                {
                    j = 1;    
                }
                else if(adr->type == TOKENIZER_NUMBER)
                {
                    j = atoi(adr->name);
                    counter++;
                    n_th(list_head, counter, adr);
                    if(adr->type != TOKENIZER_ENDOFLINE)
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected end of line or number after break");
                    return -1;
                }
                if (j <= 0)
                {
                    printf("Error! Break expected a positive number");
                    return -1;
                }
                while(j > 0)
                { 
                    if((j > 0) && (listwh_head == NULL))
                    {
                        printf("Error! Break depth is bigger than while depth");
                        return -1;
                    }                   
                    while((listwh_head->type == TOKENIZER_IF) || (listwh_head->type == TOKENIZER_ELSE))
                    {
                        curdepth--;
                        delete(&listwh_head);
                        if((j > 0) && (listwh_head == NULL))
                        {
                            break;
                        }
                    }
                    if((j > 0) && (listwh_head == NULL))
                    {
                        printf("Error! Break depth is bigger than while depth");
                        return -1;
                    }
                    if(listwh_head->type == TOKENIZER_WHILE)
                    {
                        if(j != 1)
                        {
                            curdepth--;
                            delete(&listwh_head);
                            j--;
                        }
                        else
                        {
                            changewh(listwh_head, 1, 0);
                            j--;
                        }
                    }
                }
            }
            if (token(adr->name) == TOKENIZER_CONTINUE)
            {
                if (strcmp(argv[1], "-v") == 0)
                {   
                    printline(adr->line);
                }
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type == TOKENIZER_ENDOFLINE)
                {
                    j = 1;
                }
                else if(adr->type == TOKENIZER_NUMBER)
                {
                    j = atoi(adr->name);
                    counter++;
                    n_th(list_head, counter, adr);
                    if(adr->type != TOKENIZER_ENDOFLINE)
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected end of line or number after continue");
                    return -1;
                }
                if (j <= 0)
                {
                    printf("Error! Continue expected a positive number");
                    return -1;
                }
                while(j > 0)
                { 
                    if((j > 0) && (listwh_head == NULL))
                    {
                        printf("Error! Continue depth is bigger than while depth");
                        return -1;
                    }                   
                    while((listwh_head->type == TOKENIZER_IF) || (listwh_head->type == TOKENIZER_ELSE))
                    {
                        curdepth--;
                        delete(&listwh_head);   /*delete if or else*/
                        if((j > 0) && (listwh_head == NULL))
                        {
                            printf("Error! Continue depth is bigger than while depth");
                            return -1;
                        }
                    }
                    if((j > 0) && (listwh_head == NULL))
                    {
                        printf("Error! Continue depth is bigger than while depth");
                        return -1;
                    }
                    if(listwh_head->type == TOKENIZER_WHILE)
                    {
                        if(j != 1)
                        {
                            curdepth--;
                            delete(&listwh_head);   /*delete while*/
                            j--;
                        }
                        else
                        {
                            if (listwh_head->true == 0)   /*if while statment is false*/
                            {
                                delete(&listwh_head);    /*delete while*/
                                curdepth --;
                            }
                            else if ((listwh_head->true == 1))   /*if while statment is true*/
                            {
                                flag = statement(listwh_head->counter);     /*check while statment and save it to flag*/
                                if (flag == 1)    /*if while statment is true*/
                                {
                                    counter = (listwh_head->counter);
                                    while (1 == 1)
                                    {
                                        counter++;
                                        n_th(list_head, counter, adr); 
                                        if (adr->type == TOKENIZER_ENDOFLINE)
                                        {
                                            if (strcmp(argv[1], "-v") == 0)
                                            {
                                                printline(adr->line);
                                            }
                                            break;
                                        }
                                    }
                                    break;
                                }
                                else
                                {
                                    if (strcmp(argv[1], "-v") == 0)   /* if argv[1] is -v print while*/
                                    {
                                        n_th(list_head, listwh_head->counter, adr);
                                        printline(adr->line);              
                                        n_th(list_head, counter, adr);
                                    }
                                    delete(&listwh_head);
                                    curdepth --;
                                }
                            }
                            j--;
                        }
                    }
                }
            }
        }
        else if (tabcount != curdepth)
        {
            if (tabcount > curdepth)
            {
                printf("Error! Line depth is bigger than instruction depth");
                return -1;
            }
            else
            {
                diff = curdepth - tabcount;
                while (diff > 0)
                {
                    if ((listwh_head->type == TOKENIZER_IF) || (listwh_head->type == TOKENIZER_ELSE))
                    {
                        elflag = 0;
                        if ((listwh_head->true == 0) && (listwh_head->type == TOKENIZER_IF))
                        {
                            elflag = 1;
                        }
                        delete(&listwh_head);    /*delete if*/
                        curdepth --;
                        if ((token(adr->name) == TOKENIZER_ELSE) && (tabcount == curdepth))  /*if else after if*/
                        {
                            curdepth++;
                            if(elflag == 1)   /*if if statment was false */
                            {
                                add_to_while(&listwh_head, counter, TOKENIZER_ELSE, 1);
                            }
                            else
                            {
                                add_to_while(&listwh_head, counter, TOKENIZER_ELSE, 0);
                            }
                            while (1 == 1)
                            {
                                counter++;
                                n_th(list_head, counter, adr); 
                                if (adr->type == TOKENIZER_ENDOFLINE)
                                {
                                    if (strcmp(argv[1], "-v") == 0)
                                    {   
                                        printline(adr->line);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                    else if (listwh_head->true == 0)  /*if while statment was false*/
                    {
                        delete(&listwh_head);  /*delete it*/
                        curdepth --;
                    }
                    else if ((listwh_head->true == 1))  /*if while statement was true*/
                    {
                        flag = statement(listwh_head->counter);
                        if (flag == 1)
                        {
                            counter = (listwh_head->counter);
                            while (1 == 1)
                            {
                                counter++;
                                n_th(list_head, counter, adr); 
                                if (adr->type == TOKENIZER_ENDOFLINE)
                                {
                                    if (strcmp(argv[1], "-v") == 0)
                                    {
                                        printline(adr->line);
                                    }
                                    break;
                                }
                            }
                            break;
                        }
                        else
                        {
                            if (strcmp(argv[1], "-v") == 0)
                            {
                                n_th(list_head, listwh_head->counter, adr);
                                printline(adr->line);
                                n_th(list_head, counter, adr);
                            }
                            delete(&listwh_head);
                            curdepth --;
                        }
                    }
                    diff--;
                }
            }
        }
        if(adr->type == TOKENIZER_KEYWORD)     /*check if its keyword*/
        {
            tokennum = token(adr->name);
            if(tokennum == TOKENIZER_ARGUMENTSIZE)   /*if token is argument size*/
            {
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type == TOKENIZER_IDENTIFIER)   /*if after argument size is identifier*/
                {
                    if((strchr(adr->name, '[') == NULL) && (strchr(adr->name, ']') == NULL))   /*if its variable (not array)*/
                    {   
                        y1 = checkarray(listarray_head, adr->name);  /*check if variable name is already used as an array*/
                        if(y1 == 0)
                        {
                            if (strcmp(argv[1], "-v") == 0)
                            {
                                changevar(listvar_head, adr->name, (argc-3));
                            }
                            else
                            {
                                changevar(listvar_head, adr->name, (argc-2));
                            }
                        }
                        else
                        {
                            printf("Error! Variable name is already used as an array");
                            return -1;
                        }
                    }
                    else  /*if its array*/
                    {
                        tempname = adr->name; 
                        x1 = arrayseperator(&tempname);
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            y1 = changearray(listarray_head, tempname, x1, (argc-3));
                        }
                        else
                        {
                            y1 = changearray(listarray_head, tempname, x1, (argc-2));
                        }
                        if(y1 == 0)
                        {
                            printf("Error! Target array does not exist");
                            return -1;
                        }
                        free(tempname);
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (strcmp(argv[1], "-v") == 0)
                    {
                        printline(adr->line);
                    }
                    if (adr->type != TOKENIZER_ENDOFLINE)       
                    {
                        printf("Error! Expected end of line");
                        return -1;
                    }
                }
                else
                {
                    printf("\nError! Expected identifier after argument size\n");
                    return -1;
                }
            }
            else if(tokennum == TOKENIZER_ARGUMENT)   
            {
                counter++;
                n_th(list_head, counter, adr);
                if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER)) 
                {
                    k = arrayparse(adr->name);
                    if(k == RAND_MAX)   /*if its error*/
                    {
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_IDENTIFIER)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            x1 = 3;
                        }
                        else
                        {
                            x1 = 2;
                        }
                        if(argc >= (k+x1))      /*check if we have the right amound of arguments*/
                        {
                            if((strchr(adr->name, '[') == NULL) && (strchr(adr->name, ']') == NULL))
                            {   
                                y1 = checkarray(listarray_head, adr->name);
                                if(y1 == 0)
                                {
                                    if (strcmp(argv[1], "-v") == 0)
                                    {
                                        changevar(listvar_head, adr->name, atoi(argv[k+2]));
                                    }
                                    else
                                    {  
                                        changevar(listvar_head, adr->name, atoi(argv[k+1]));
                                    }
                                }
                                else
                                {
                                    printf("Error! Variable name is already used as an array");
                                    return -1;
                                }
                            }
                            else
                            {
                                tempname = adr->name;
                                x1 = arrayseperator(&tempname);
                                if(y1 == 0)
                                {
                                    if (strcmp(argv[1], "-v") == 0)
                                    {
                                        y1 = changearray(listarray_head, tempname, x1, atoi(argv[k+2]));
                                    }
                                    else
                                    {  
                                        y1 = changearray(listarray_head, tempname, x1, atoi(argv[k+1]));
                                    }
                                    free(tempname);
                                }
                                if(y1 == 0)
                                {
                                    printf("Error! Target array does not exist");
                                    return -1;
                                }
                            }
                            counter++;
                            n_th(list_head, counter, adr);
                            if (strcmp(argv[1], "-v") == 0)
                            {
                                printline(adr->line);
                            }
                            if (adr->type != TOKENIZER_ENDOFLINE)
                            {
                                printf("Error! Expected end of line");
                                return -1;
                            }
                        }
                        else
                        {
                            printf("\nError! Not enough arguments\n");
                            return -1;
                        }
                    }
                    else
                    {
                        printf("\nError! Expected identifier after argument number\n");
                        return -1;
                    }
                }
                else
                {
                    printf("\nError! Expected number after argument\n");
                    return -1;
                }
            }
            else if((tokennum == TOKENIZER_WRITE) || (tokennum == TOKENIZER_WRITELN))
            {
                counter++;
                n_th(list_head, counter, adr);
                if((adr->type == TOKENIZER_IDENTIFIER) || (adr->type == TOKENIZER_NUMBER))
                {
                    j = arrayparse(adr->name);
                    if(j == RAND_MAX)
                    {
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if(adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (tokennum == TOKENIZER_WRITE)
                        {
                            printf("%d ", j);
                        }
                        else
                        {
                            printf("%d\n", j);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after identifier/number");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected idintifier or number after write/writeln");
                    return -1;
                }
            }
            else if (tokennum == TOKENIZER_READ)
            {
                counter++;
                n_th(list_head, counter, adr);
                if (adr->type == TOKENIZER_IDENTIFIER)
                {
                    if (strcmp(argv[1], "-v") == 0)
                    {
                        printline(adr->line);
                    }
                    scanf("%d", &j);
                    strcpy(x, adr->name);
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))  /*if its not array*/
                        {   
                            y1 = checkarray(listarray_head, x);
                            if(y1 == 0)
                            {
                                changevar(listvar_head, x, j);
                            }
                            else
                            {
                                printf("Error! Variable name is already used as an array");
                                return -1;
                            }
                        }
                        else  /*if its array*/
                        {
                            tempname = x;
                            x1 = arrayseperator(&tempname);
                            y1 = changearray(listarray_head, tempname, x1, j);
                            free(tempname);
                            if(y1 == 0)
                            {
                                printf("Error! Target array does not exist");
                                return -1;
                            }
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after identifier");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected identifier after read");
                    return -1;
                }
            }
            else if(tokennum == TOKENIZER_RANDOM)
            {
                counter++;
                n_th(list_head, counter, adr);
                if (adr->type == TOKENIZER_IDENTIFIER)
                {
                    j = rand();
                    while(j < 0)  /*make sure j is positive*/
                    {
                        j = rand();
                    }
                    strcpy(x, adr->name);                    
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type != TOKENIZER_ENDOFLINE)
                    {
                        printf("Error! Expected end of line after identifier");
                        return -1;
                    }
                    if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                    {   
                        y1 = checkarray(listarray_head, x);
                        if(y1 == 0)
                        {
                            changevar(listvar_head, x, j);
                        }
                        else
                        {
                            printf("Error! Variable name is already used as an array");
                            return -1;
                        }
                    }
                    else
                    {
                        tempname = x;
                        x1 = arrayseperator(&tempname);
                        y1 = changearray(listarray_head, tempname, x1, j);
                        free(tempname);
                        if(y1 == 0)
                        {
                            printf("Error! Target array does not exist");
                            return -1;
                        }
                    }
                }
                else
                {
                    printf("Error! Expected identifier after random");
                    return -1;
                }
            }
            else if (tokennum == TOKENIZER_WHILE)
            {
                tempcounter = counter;
                counter++;
                n_th(list_head, counter, adr);
                if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                {
                    j = arrayparse(adr->name);
                    if(j == RAND_MAX)
                    {
                        return -1;
                    } 
                }
                else
                {
                    printf("Error! Expected number or identifier after while");
                    return -1;
                }
                counter++;
                n_th(list_head, counter, adr);
                if (token(adr->name) == TOKENIZER_EQ)   /*if token is "==" */
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j == k)   /*check if while statment is true*/
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else    /*check if while statment is false*/
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_DIFFER)    /*if token is "!=" (same process as before)*/
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j != k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_LOREQ)   /*if token is "<=" (same process as before)*/
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j <= k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }          
                }
                else if(token(adr->name) == TOKENIZER_LT)   /*if token is "<" (same process as before)*/
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j < k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }                        
                }
                else if(token(adr->name) == TOKENIZER_GOREQ)   /*if token is ">=" (same process as before)*/
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j >= k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_GT)    /*if token is ">" (same process as before)*/
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j > k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_WHILE, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected statement after identifier or number");
                    return -1;
                }
            }
            else if (tokennum == TOKENIZER_IF)   /*if token is "if" ,same process as while*/
            {
                tempcounter = counter;
                counter++;
                n_th(list_head, counter, adr);
                if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                {
                    j = arrayparse(adr->name);
                    if(j == RAND_MAX)
                    {
                        return -1;
                    } 
                }
                else
                {
                    printf("Error! Expected number or identifier after while");
                    return -1;
                }
                counter++;
                n_th(list_head, counter, adr);
                if (token(adr->name) == TOKENIZER_EQ)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j == k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_DIFFER)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j != k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_LOREQ)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j <= k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }          
                }
                else if(token(adr->name) == TOKENIZER_LT)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j < k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }                        
                }
                else if(token(adr->name) == TOKENIZER_GOREQ)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j >= k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else if(token(adr->name) == TOKENIZER_GT)
                {
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_NUMBER) || (adr->type == TOKENIZER_IDENTIFIER))
                    {
                        k = arrayparse(adr->name);
                        if(k == RAND_MAX)
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        printf("Error! Expected number or identifier after statement");
                        return -1;
                    }
                    counter++;
                    n_th(list_head, counter, adr);
                    if (adr->type == TOKENIZER_ENDOFLINE)
                    {
                        if (strcmp(argv[1], "-v") == 0)
                        {
                            printline(adr->line);
                        }
                        if (j > k)
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 1);
                        }
                        else
                        {
                            curdepth++;
                            add_to_while(&listwh_head, tempcounter, TOKENIZER_IF, 0);
                        }
                    }
                    else
                    {
                        printf("Error! Expected end of line after number");
                        return -1;
                    }
                }
                else
                {
                    printf("Error! Expected statement after identifier or number");
                    return -1;
                }
            }
            else if(tokennum == TOKENIZER_ELSE)   /*check if token is "else" without if,print error*/
            {
                printf("Error! Expected if part before else");
                return -1;
            }
            else if(tokennum == TOKENIZER_NEW)    
            {
                counter++;
                n_th(list_head, counter, adr);
                if (adr->type == TOKENIZER_IDENTIFIER)
                {
                    tempname = adr->name;
                    y1 = arrayseperator(&tempname);
                    if(y1 == RAND_MAX)
                    {
                        return -1;
                    }
                    else if(y1 <= 0)
                    {
                        printf("Error! Array size when declared should be a possitve integer");
                        return -1;
                    }
                    else if(in(listvar_head, tempname) != 0)   /*if name of array exists as variable print error*/
                    {
                        printf("Error! Array name is already declared as variable name");
                        return -1;
                    }
                    x1 = add_to_array(&listarray_head, tempname, y1);
                    free(tempname);
                    counter++;
                    n_th(list_head, counter, adr);
                    if(adr->type != TOKENIZER_ENDOFLINE)
                    {
                        printf("Error! Expected end of line after identifier/number");
                        return -1;
                    }
                    if (strcmp(argv[1], "-v") == 0)
                    {   
                        printline(adr->line);
                    }
                }
                else
                {
                    printf("Error! Expected array identifier after new");
                    return -1;
                }
            }
            else if(tokennum == TOKENIZER_FREE)
            {
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type != TOKENIZER_IDENTIFIER)
                {
                    printf("Error! Expected identifier after free");
                    return -1;
                }
                x1 = deletearray(&listarray_head, adr->name);   /*delete array*/
                if(x1 == 0)
                {
                    printf("Error! Array does not exist");
                    return -1;
                }
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type != TOKENIZER_ENDOFLINE)
                {
                    printf("Error! Expected end of line after identifier/number");
                    return -1;
                }
                if (strcmp(argv[1], "-v") == 0)
                {   
                    printline(adr->line);
                }
            }
            else if(tokennum == TOKENIZER_SIZE)
            {
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type != TOKENIZER_IDENTIFIER)
                {
                    printf("Error! Expected identifier after free");
                    return -1;
                }
                x1 = checkarray(listarray_head, adr->name);
                if(x1 == 0)
                {
                    printf("Error! Array does not exist");
                    return -1;
                }
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type != TOKENIZER_IDENTIFIER)
                {
                    printf("Error! Expected identifier after size");
                    return -1;
                }
                if((strchr(adr->name, '[') == NULL) && (strchr(adr->name, ']') == NULL))
                {   
                    y1 = checkarray(listarray_head, adr->name);
                    if(y1 == 0)
                    {
                        changevar(listvar_head, adr->name, x1);
                    }
                    else
                    {
                        printf("Error! Variable name is already used as an array");
                        return -1;
                    }
                }
                else
                {
                    tempname = adr->name;
                    j = arrayseperator(&tempname);
                    y1 = changearray(listarray_head, tempname, j, x1);
                    free(tempname);
                    if(y1 == 0)
                    {
                        printf("Error! Target array does not exist");
                        return -1;
                    }
                }
                counter++;
                n_th(list_head, counter, adr);
                if(adr->type != TOKENIZER_ENDOFLINE)
                {
                    printf("Error! Expected end of line after identifier/number");
                    return -1;
                }
                if (strcmp(argv[1], "-v") == 0)
                {   
                    printline(adr->line);
                }
            }
        }
        else if(adr->type == TOKENIZER_IDENTIFIER)
        {
            strcpy(x, adr->name);
            counter++;
            n_th(list_head, counter, adr);
            tokennum = token(adr->name);
            if (tokennum == TOKENIZER_REPLACE)   /*if token is "="*/
            {
                counter++;
                n_th(list_head, counter, adr);
                if((adr->type == TOKENIZER_IDENTIFIER) || (adr->type == TOKENIZER_NUMBER))
                {
                    j = arrayparse(adr->name);
                    counter++;
                    n_th(list_head, counter, adr);
                    if ((adr->type == TOKENIZER_OPERATOR) || (adr->type == TOKENIZER_ENDOFLINE))
                    {
                        if(adr->type == TOKENIZER_ENDOFLINE)
                        {
                            if (strcmp(argv[1], "-v") == 0)
                            {
                                printline(adr->line);
                            }
                            if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                            {   
                                y1 = checkarray(listarray_head, x);
                                if(y1 == 0)
                                {
                                    changevar(listvar_head, x, j);
                                }
                                else
                                {
                                    printf("Error! Variable name is already used as an array");
                                    return -1;
                                }
                            }
                            else
                            {
                                tempname = x;
                                x1 = arrayseperator(&tempname);
                                y1 = changearray(listarray_head, tempname, x1, j);
                                free(tempname);
                                if(y1 == 0)
                                {
                                    printf("Error! Target array does not exist");
                                    return -1;
                                }
                            }
                        }
                        else
                        {
                            tokennum = token(adr->name);
                            counter++;
                            n_th(list_head, counter, adr);
                            if((adr->type == TOKENIZER_IDENTIFIER) || (adr->type == TOKENIZER_NUMBER))
                            {
                                k = arrayparse(adr->name);
                                counter++;
                                n_th(list_head, counter, adr);
                                if(adr->type == TOKENIZER_ENDOFLINE)
                                {
                                    if (strcmp(argv[1], "-v") == 0)
                                    {
                                        printline(adr->line);
                                    }
                                    if(tokennum == TOKENIZER_PLUS)   /*if token is "+"*/
                                    {
                                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))  /*if its not array*/
                                        {   
                                            y1 = checkarray(listarray_head, x);
                                            if(y1 == 0)
                                            {
                                                changevar(listvar_head, x, (j + k));
                                            }
                                            else
                                            {
                                                printf("Error! Variable name is already used as an array");
                                                return -1;
                                            }
                                        }
                                        else  /*if its array*/
                                        {
                                            tempname = x;
                                            x1 = arrayseperator(&tempname);
                                            y1 = changearray(listarray_head, tempname, x1, (j + k));
                                            free(tempname);
                                            if(y1 == 0)
                                            {
                                                printf("Error! Target array does not exist");
                                                return -1;
                                            }
                                        }
                                    }
                                    else if(tokennum == TOKENIZER_MINUS)   /*if token is "-"*/
                                    {                                                    
                                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                                        {   
                                            y1 = checkarray(listarray_head, x);
                                            if(y1 == 0)
                                            {
                                                changevar(listvar_head, x, (j - k));
                                            }
                                            else
                                            {
                                                printf("Error! Variable name is already used as an array");
                                                return -1;
                                            }
                                        }
                                        else
                                        {
                                            tempname = x;
                                            x1 = arrayseperator(&tempname);
                                            y1 = changearray(listarray_head, tempname, x1, (j - k));
                                            free(tempname);
                                            if(y1 == 0)
                                            {
                                                printf("Error! Target array does not exist");
                                                return -1;
                                            }
                                        }
                                    }
                                    else if(tokennum == TOKENIZER_ASTR)  /*if token is "*" */
                                    {                                                    
                                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                                        {   
                                            y1 = checkarray(listarray_head, x);
                                            if(y1 == 0)
                                            {
                                                changevar(listvar_head, x, (j * k));
                                            }
                                            else
                                            {
                                                printf("Error! Variable name is already used as an array");
                                                return -1;
                                            }
                                        }
                                        else
                                        {
                                            tempname = x;
                                            x1 = arrayseperator(&tempname);
                                            y1 = changearray(listarray_head, tempname, x1, (j * k));
                                            free(tempname);
                                            if(y1 == 0)
                                            {
                                                printf("Error! Target array does not exist");
                                                return -1;
                                            }
                                        }
                                    }
                                    else if(tokennum == TOKENIZER_SLASH)  /*if token is "/" */
                                    {                                                    
                                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                                        {   
                                            y1 = checkarray(listarray_head, x);
                                            if(y1 == 0)
                                            {
                                                changevar(listvar_head, x, (j / k));
                                            }
                                            else
                                            {
                                                printf("Error! Variable name is already used as an array");
                                                return -1;
                                            }
                                        }
                                        else
                                        {
                                            tempname = x;
                                            x1 = arrayseperator(&tempname);
                                            y1 = changearray(listarray_head, tempname, x1, (j / k));
                                            free(tempname);
                                            if(y1 == 0)
                                            {
                                                printf("Error! Target array does not exist");
                                                return -1;
                                            }
                                        }
                                    }
                                    else if(tokennum == TOKENIZER_MOD)  /*if token is "%" */
                                    {                                                    
                                        if((strchr(x, '[') == NULL) && (strchr(x, ']') == NULL))
                                        {   
                                            y1 = checkarray(listarray_head, x);
                                            if(y1 == 0)
                                            {
                                                changevar(listvar_head, x, (j % k));
                                            }
                                            else
                                            {
                                                printf("Error! Variable name is already used as an array");
                                                return -1;
                                            }
                                        }
                                        else
                                        {
                                            tempname = x;
                                            x1 = arrayseperator(&tempname);
                                            y1 = changearray(listarray_head, tempname, x1, (j % k));
                                            free(tempname);
                                            if(y1 == 0)
                                            {
                                                printf("Error! Target array does not exist");
                                                return -1;
                                            }
                                        }
                                    }
                                    tokennum = -1;
                                    
                                }
                                else
                                {
                                    printf("Error! Expected end of line after identifier/number");
                                    return -1;
                                }
                            }
                            else
                            {
                                printf("\nError! Expected identintifier or number after operator\n");
                                return -1;
                            }
                        }
                    }
                    else
                    {
                        printf("\nError! Expecter end of line or operator after identifier\n");
                        return -1;
                    } 
                    
                }
                else
                {
                    printf("\nError! Expected identintifier or number after replacement operator\n");
                    return -1;
                }
            }
            else
            {
                printf("\nError! Expecter replacement operator\n");
                return -1;
            }
        }
        else if (adr->type == TOKENIZER_OPERATOR)  /*if line starts with operator*/
        {
            printf("Error! Line should not start with operator");
            return -1;
        }
        else if (adr->type == TOKENIZER_NUMBER)  /*if line starts with number*/
        {
            printf("Error! Line should not start with number");
            return -1;
        }      
    }
    free(adr->name);
    free(adr);
    free(adrvar->name);
    free(adrvar);
    return 0;
}