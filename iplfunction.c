#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "iplfunction.h"
#include "ipllexer.h"

int detacher(char* name)   /*creates a new file without comments*/
{
    FILE *fb;
    FILE *fout;
    char *grammi, *temp;
    int flagerror;
    int ch;

    grammi = malloc(1010 * sizeof(char));  
    if (grammi == NULL)
    {
        printf("Error! Not enought memory");
        return -1;
    }
    fb = fopen(name, "r");     /*open file*/
    if (fb == NULL)
    {
        printf("Error! Could not open file");
        return -1;
    }
    fout = fopen("out.txt", "w");        /*open file for output*/
    if (fout == NULL)
    {
        printf("Error! Could not open file");
        return -1;
    }
    while (feof(fb)==0)    /*while you havent reached end of the file*/
    {
        fgets(grammi, 1010, fb);         /*get a line*/
        flagerror = ferror(fb);        /*check fgets for errors*/
        if (flagerror != 0)
        {
        printf("Error\n");
        return -1;
        }
        ch = '#';
        if (strchr(grammi, ch) != NULL)   /*search the character '#' and return his adress*/
        {
        temp = strchr(grammi, ch);  /*search the character '#' and return it's adress*/
        *temp = '\n';         /*replace character '#' with \n (move to next line)*/
        *(temp+1) = '\0';    /*replace the next character whith end of string*/
        }
        fputs(grammi, fout);      /*write the line to an output file*/
        free (grammi);         /*empty comments after character '#'*/
        grammi = malloc(1010 * sizeof(char));       /*allocate memory for next line*/
        *(grammi)= '\0';        /*replace next character whith end of string*/
    }
    putc('\n', fout);     /*write \n (move to next line) to output file*/
    free(grammi);
    fclose(fb);         /*close file*/
    fclose(fout);      /*close output file*/
    return 0;
}

int token(char *ch)    /*converts type of string to an integer*/
{
    if(strcmp(ch, "\n")==0) 
    {
        return TOKENIZER_CR;
    }
    else if(strcmp(ch, "+")==0) 
    {
        return TOKENIZER_PLUS;
    } 
    else if(strcmp(ch, "-")==0) 
    {
        return TOKENIZER_MINUS;
    } 
    else if(strcmp(ch, "&")==0) 
    {
        return TOKENIZER_AND;
    } 
    else if(strcmp(ch, "|")==0) 
    {
        return TOKENIZER_OR;
    } 
    else if(strcmp(ch, "*")==0) 
    {
        return TOKENIZER_ASTR;
    } 
    else if(strcmp(ch, "/")==0) 
    {
        return TOKENIZER_SLASH;
    } 
    else if(strcmp(ch, "%")==0) 
    {
        return TOKENIZER_MOD;
    }
    else if(strcmp(ch, "<=")==0) 
    {
        return TOKENIZER_LOREQ;
    } 
    else if(strcmp(ch, "<")==0) 
    {
        return TOKENIZER_LT;
    } 
    else if(strcmp(ch, ">=")==0) 
    {
        return TOKENIZER_GOREQ;
    } 
    else if(strcmp(ch, ">")==0) 
    {
        return TOKENIZER_GT;
    } 
    else if(strcmp(ch, "!=")==0) 
    {
        return TOKENIZER_DIFFER;
    } 
    else if(strcmp(ch, "==")==0) 
    {
        return TOKENIZER_EQ;
    } 
    else if(strcmp(ch, "=")==0) 
    {
        return TOKENIZER_REPLACE;
    }
    else if(strcmp(ch, "read")==0) 
    {
        return TOKENIZER_READ;
    }
    else if(strcmp(ch, "writeln")==0) 
    {
        return TOKENIZER_WRITELN;
    }
    else if(strcmp(ch, "write")==0) 
    {
        return TOKENIZER_WRITE;
    }
    else if(strcmp(ch, "else")==0) 
    {
        return TOKENIZER_ELSE;
    }
    else if(strcmp(ch, "if")==0) 
    {
        return TOKENIZER_IF;
    }
    else if(strcmp(ch, "random")==0) 
    {
        return TOKENIZER_RANDOM;
    }
    else if(strcmp(ch, "argumentsize")==0) 
    {
        return TOKENIZER_ARGUMENTSIZE;
    }
    else if(strcmp(ch, "argument")==0) 
    {
        return TOKENIZER_ARGUMENT;
    }
    else if(strcmp(ch, "while")==0) 
    {
        return TOKENIZER_WHILE;
    }
    else if(strcmp(ch, "break")==0) 
    {
        return TOKENIZER_BREAK;
    }
    else if(strcmp(ch, "continue")==0) 
    {
        return TOKENIZER_CONTINUE;
    }
    else if(strcmp(ch, "new")==0) 
    {
        return TOKENIZER_NEW;
    }
    else if(strcmp(ch, "free")==0) 
    {
        return TOKENIZER_FREE;
    }
    else if(strcmp(ch, "size")==0) 
    {
        return TOKENIZER_SIZE;
    }
    return 0;
}



int add_to_end(struct listnode **ptraddrr, char name1[], int type1, int line1)        
{ 
    int x;

    while (*ptraddrr != NULL)  /* Go to end of list */
    {                                                               
        ptraddrr = &((*ptraddrr)->next);       /* Prepare what we need to change */
    }
    *ptraddrr = malloc(sizeof(struct listnode));             /* Space for new node */
    x = strlen(name1);
    (*ptraddrr)->name = malloc((x+1)*sizeof(char));
    strcpy((*ptraddrr)->name, name1);   
    (*ptraddrr)->type = type1;                                                         
    (*ptraddrr)->line = line1;
    (*ptraddrr)->next = NULL;          /* There is no next element */
    return 0;
}

void printlist(struct listnode *list)        /* Print elements of list */
{ 
    while (list != NULL)   /* Visit list elements up to the end */
    {                                         
        printf("%s--> ", list->name);
        printf("%d--> ", list->type);                                              
        printf("%d--> ", list->line);
        list = list->next;          /* Go to next element */
    }
    printf("NULL\n");       /* Print end of list */
}

int n_th(struct listnode *list, int n, struct listnode *vaddr)       /* Return n-th element of list, if it exists */
{ 
    while (list != NULL)   /* Maybe search up to the end of the list */
    {                                                               
        if (n-- == 1)   /* Did we reach the right element? */
        {                                                          
            strcpy(vaddr->name, list->name);
            vaddr->type = list->type;                                  
            vaddr->line = list->line;
            vaddr->next = list->next;
            return 1;                   /* We found it */
        }
        else
        {
            list = list->next;      /* No, go to next element */
        }
    }
    return 0;       /* Sorry, list is too short */
}

int in(struct listnode *list, char name[])     /*if variable exists returns node else returns 0*/
{ 
    int counter = 0;

    while (list != NULL)     /* Visit list elements up to the end */
    {
        counter++;                                  
        if (strcmp(list->name, name) == 0)   /* Did we find what we are looking for? */  
        {                                             
            return counter;
        }                                           
        else
        {
            list = list->next;      /* No, go to next element */
        }                                           
    }
    return 0;                       
}

int addvalue(struct listnode *list, int n, int value)       
{
    while (list != NULL)                                   
    {  
        if (n-- == 1)
        {                                                   
        list->type = value;                                 
        return 1;                                      
        }
        else
        {
        list = list->next;                                
        } 
    }
    return 0;                                          
}

int add_to_while(struct whnode **ptraddrr, int counter, int type, int true)     
{ 
    struct whnode *templist;

    templist = *ptraddrr;                           /* Save current start of list */
    *ptraddrr = malloc(sizeof(struct whnode));      /* Space for new node */
    (*ptraddrr)->counter = counter;
    (*ptraddrr)->type = type;
    (*ptraddrr)->true = true;                                                  
    (*ptraddrr)->next = templist;             /* Next element is former first */
    return 0;
}

void printwhlist(struct whnode *list)                       /* Print elements of list */
{ 
    while (list != NULL)
    {                                                       /* Visit list elements up to the end */
        printf("%d--> ", list->counter);                    /* Print current element */
        printf("%d--> ", list->type);
        printf("%d--> ", list->true);
        list = list->next;                                  /* Go to next element */
    }
    printf("NULL\n");                                       /* Print end of list */
}

int delete(struct whnode **ptraddr)         /* Delete element from list  if it exists */
{ 
    struct whnode *templist;

    if((*ptraddr) != NULL)
    { 
        templist = *ptraddr;                        /* save address of its node */
        *ptraddr = (*ptraddr)->next;                       
        free(templist);                           /* Free memory for the corresponding node */
        return 1;                   /* We deleted the element */
    }
    return 0;              /* We did't find the element we were looking for */
}

int statement(int counter)            /*checks if while statment is true or faulse (same process as while in parser)*/
{
    struct listnode *adr, *adrvar;
    int j, k;
    
    adr = malloc(sizeof(struct listnode));
    adr->name = malloc(105*sizeof(char));
    n_th(list_head, 1, adr);
    adrvar = malloc(sizeof(struct listnode));
    adrvar->name = malloc(105*sizeof(char));
    n_th(listvar_head, 1, adrvar);
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j == k)
            {
                return 1;
            }
            else
            {
                return 0;
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j != k)
            {
                return 1;
            }
            else
            {
                return 0;
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j <= k)
            {
                return 1;
            }
            else
            {
                return 0;
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j < k)
            {
                return 1;
            }
            else
            {
                return 0;
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j >= k)
            {
                return 1;
            }
            else
            {
                return 0;
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
            free(adr->name);
            free(adr);
            free(adrvar->name);
            free(adrvar);
            if (j > k)
            {
                return 1;
            }
            else
            {
                return 0;
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

int printline(int linenumber)
{
    FILE *file = fopen("out.txt", "r");
    int count = 1;

    if ( file != NULL )
    {
        char line[1010]; /* or other suitable maximum line size */
        while (fgets(line, sizeof line, file) != NULL) /* read a line */
        {
            if (count == linenumber)
            {
                printf("\t%2d:        %s", count, line);
                fclose(file);
                return 0;
            }
            else
            {
                count++;
            }
        }
        fclose(file);
    }
    else
    {
        printf("Error! Could not open out.txt");
        return -1;
    }
    return 0;
}

int changewh(struct whnode *list, int n, int true)      /*change true statment to while*/
{ 
    while (list != NULL)                                           
    {  
        if (n-- == 1)
        {                                                      
        list->true = true ;                                    
        return 1;                                                 
        }
        else
        {
        list = list->next;                                 
        } 
    }
    return 0;                                                  
}

int add_to_array(struct arraynode **ptraddrr, char name[], int size)        
{ 
    int x, i;
    struct arraynode *templist;

    templist = *ptraddrr;                                                   
    *ptraddrr = malloc(sizeof(struct arraynode));              /* Space for new node */
    x = strlen(name);
    (*ptraddrr)->name = malloc((x+1)*sizeof(char));
    strcpy((*ptraddrr)->name, name);
    (*ptraddrr)->array = malloc((size)*sizeof(int));
    for (i = 0; i < size; i++)
    {
        *((*ptraddrr)->array + i) = 0;
    }
    (*ptraddrr)->size = size;              
    (*ptraddrr)->next = templist;                                  
    return 0;
}

void printarraylist(struct arraynode *list)          /* Print elements of list */
{ 
    while (list != NULL)   /* Visit list elements up to the end */
    {                                      
        int i;
        printf("%s--> ", list->name);
        printf(" ");
        for(i = 0; i < list->size-1; i++)
        {
            printf("%d, ", *(list->array + i));
        }
        printf("%d ", *(list->array + list->size -1));
        printf("-->");                                                   
        printf("%d\n", list->size);
        list = list->next;                /* Go to next element */
    }
    printf("NULL\n");          /* Print end of list */
}

int deletelist(struct listnode **ptraddr)          /* Delete element from list if it exists */
{ 
    struct listnode *templist;

    if((*ptraddr) != NULL)
    { 
        templist = *ptraddr;                /*  save address of its node */
        free((*ptraddr)->name);
        *ptraddr = (*ptraddr)->next;        /* Bypass deleted element */
        free(templist);                     /* Free memory for the corresponding node */
        return 1;                           /* We deleted the element */
    }
    return 0;           /* We did't find the element we were looking for */
}

int deletearray(struct arraynode **ptraddr, char name[])         /*delete a node from array list*/      
{ 
    struct arraynode *templist;
    
    while ((*ptraddr) != NULL)                                              
    {  
        if (strcmp((*ptraddr)->name, name) == 0)                            
        {
            templist = *ptraddr;                                            
            free((*ptraddr)->name);
            free((*ptraddr)->array);
            *ptraddr = (*ptraddr)->next;                                    
            free(templist);                                                 
            return 1;                                                   
        }
        else
        { 
            ptraddr = &((*ptraddr)->next);          /* Prepare what we might change */
        }
    }
    return 0;         /* We did't find the element we were looking for */
}

int changearray(struct arraynode *list, char name[], int i, int value)     /*change value of given name and index*/ 
{ 
    while (list != NULL)                                                        
    {  
        if (strcmp(list->name, name) == 0)                                     
        {                                    /*found the array*/ 
            *(list->array + i) = value;
            return 1;
        }
        else
        { 
            list = list->next;                                                 
        }
    }
    return 0;        /*coudnt find the array*/                                                           
}

int checkarray(struct arraynode *list, char name[])      /*if array exists returns array size else 0*/   
{ 
    while (list != NULL)               /* Visit list elements up to the end */
    {  
        if (strcmp(list->name, name) == 0)      /* Did we find what to delete? */
        {
            return list->size;   /*return array size*/
        }
        else
        { 
            list = list->next;        /* Prepare what we might change */
        }
    }
    return 0;           /* We did't find the element we were looking for */
}

int arrayreturnv(struct arraynode *list, char name[], int i)            
{ 
    while (list != NULL)              /* Visit list elements up to the end */
    {  
        if (strcmp(list->name, name) == 0)        /* Did we find what to delete? */
        {
            return *(list->array + i);
        }
        else
        { 
            list = list->next;          /* Prepare what we might change */
        }
    }
    return 0;         /* We did't find the element we were looking for */
}

int allnumbers(char numbers[])    /*check if srting has numbers only*/
{
    char *endptr;

    strtol(numbers, &endptr, 10);
    if (*endptr != '\0' || endptr == numbers) 
    {
        return 0;
    } 
    else 
    {
        return 1;
    }
}

int arrayparse(char name[])  /*takes a string witch can be number ,variable or array and returns its value*/
{
    struct listnode *adrvar;
    char ch;
    char* token;
    char name1[strlen(name) + 1];
    char *temp, *temp2;
    adrvar = malloc(sizeof(struct listnode));
    adrvar->name = malloc(105*sizeof(char));
    n_th(listvar_head, 1, adrvar);
    strcpy(name1, name);
    int x;

    if((strchr(name, '[') == NULL) && (strchr(name, ']') == NULL))
    {
        if(allnumbers(name) == 1)   /*check if its number*/
        {
            free(adrvar->name);
            free(adrvar);
            return (atoi(name));
        }
        else if(isalpha(name[0]) == 0)  /*check if first character of variable is a letter*/
        {
            printf ("Error! Identifier name should start with a letter");
            return RAND_MAX;
        }
        else    /*if its variable*/
        {
            if(in(listvar_head, name) == 0)    /*if it doesnt exists*/
            {
                free(adrvar->name);
                free(adrvar);
                return 0;
            }
            else    /*if exists*/
            {
                n_th(listvar_head, in(listvar_head, name), adrvar);
                x = adrvar->type;
                free(adrvar->name);
                free(adrvar);
                return x;
            }
        }
    }
    else  /*if its array*/
    {
        token = strtok(name1, "[");
        token = strtok(NULL, "[");    
        temp = malloc((strlen(name)+1)*sizeof(char));
        temp2 = temp;
        ch = '[';
        temp = (strchr(name, ch));
        temp++;
        strcpy(token, temp);
        free(temp2);
        if (checkarray(listarray_head, name1) == 0)
        {
            printf("Error! Undefined array.");
            return RAND_MAX;
        }
        else
        {
            x = strlen(name);
            if(name[x - 1] != ']')
            {
                printf("Error! Wrong use of array symbols - expected ] at the end\n");
                return RAND_MAX;
            }
            x = strlen(token);
            token[x - 1] = '\0';
            x = arrayparse(token);
            if (x == RAND_MAX)
            {
                return x;
            }
            free(adrvar->name);
            free(adrvar);
            if((x < 0) || (x >= checkarray(listarray_head, name1)))
            {
                printf("Error! Non existend array element");
                return RAND_MAX;
            }
            return (arrayreturnv(listarray_head, name1, x));
        }
    }
}

int arrayseperator(char **name)   /*seperate array name and calculate index*/
{
    char *tempname, *token1, *temp, *temp2, ch;
    int x1, y1;
    
    tempname = malloc((strlen((*name))+1)*sizeof(char));
    strcpy(tempname, (*name));
    token1 = strtok(tempname, "[");    
    token1 = strtok(NULL, "[");     /*seperate name and index*/
    temp = malloc((strlen((*name))+1)*sizeof(char));
    temp2 = temp;
    ch = '[';
    temp = (strchr((*name), ch));
    temp++;
    strcpy(token1, temp);
    free(temp2);
    x1 = strlen((*name));
    if((*name)[x1 - 1] != ']')
    {
        printf("Error! Wrong use of array symbols - expected ] at the end\n");
        return -1;
    }
    x1 = strlen(token1);
    token1[x1 - 1] = '\0';
    if(strchr(tempname, ']') != NULL)
    {
        printf("Error! Wrong array declaration");
        return -1;
    }
    y1 = arrayparse(token1);
    if(y1 == RAND_MAX)
    {
        return -1;
    }
    if((x1 = checkarray(listarray_head, tempname)))
    {
        if((y1 < 0) || (y1 >= x1))
        {
            printf("Error! Non existend array element");
            return RAND_MAX;
        }
    }
    *name = tempname;
    return y1;
}

int changevar(struct listnode *list, char name[], int value)      /*if variable exists change its value else creates it */  
{ 
    while (list != NULL)            /* Maybe search up to the end of the list */
    {  
        if (strcmp(list->name, name) == 0)    /* Did we reach the right element? */
        {                                                          
            list->type = value;      /*change value*/ 
            return 1;                /* Yes, return it */                        
        }
        else
        {
            list = list->next;        /* No, go to next element */
        } 
    }
    add_to_end(&listvar_head, name, value, 0);    /*if it doesnt exist create it*/
    return 0;        
}