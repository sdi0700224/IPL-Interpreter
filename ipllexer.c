#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "iplfunction.h"
#include "ipllexer.h"
//#define DEBUG

int isKeyword(char buffer[])					/*fanction isKeyword finds keywords*/
{
	char keywords[13][20] = {"read", "write", "writeln", "else", "if", "random", "argument", "size", "while", "break", "continue", "new", "free"};
	int i, flag = 0;
	
	for(i = 0; i < 13; ++i){
		if(strcmp(keywords[i], buffer) == 0)	/*if its the keyword 'argument'*/
		{
			flag = 1;							/*kane to flag 1*/
			if (i == 6)							/*if its the keyword 'argument'*/
			{
				flag = 2;
			}
			if (i == 7)							/*if its the keyword 'size'*/
			{
				flag = 3;
			}
			break;
		}
	}
	return flag;
}

int lexer(void) 			/*function lexer converts the ipl program into a sequence of tokens*/
{
	char ch, chbuf[2], buffer[105], tempbuffer[105], opbuf[3], operators[] = {"+-*/%=!<>"};
	FILE *fp;
	int i,j=0; 
	int counter = 1, flag = 0; /*flag is 1 when buffer[] has letter*/
	int flag2 = 0;    /*flag2 is 1 when tempbuffer[] is "argument" */
	int flag3 = 0;
	int flagop =0;
	int tabflag = 0;

	for(int i=0; i<3; i++)
	{
		opbuf[i] = '\0';
	}
	fp = fopen("out.txt","r");  /*open file*/
	if(fp == NULL)
	{
		printf("Error while opening the file\n");
		return(0);
	}
	while((ch = fgetc(fp)) != EOF)  /*get eatch character till end of file*/
	{
		flag3 = 0;
   		for(i = 0; i < 9; ++i)		/*check if its operator*/
		{
   			if(ch == operators[i])
			{
				tabflag = 1;
				if (flag2 == 1)
				{
					#ifdef DEBUG
						printf("%s is keyword, line %d\n", tempbuffer, counter);
					#endif
					add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
					flag2 = 0;
				}
				flagop = 1;
				flag3 = 1;
				opbuf[0] = ch;
				if (flagop == 1)
				{
					ch = fgetc(fp);
					for(i = 0; i < 9; ++i)
					{
						if (ch == operators[i])
						{
							opbuf[1] = ch;
							#ifdef DEBUG
								printf("%s is operator, line %d\n", opbuf, counter);
							#endif
							if(j > 0)
							{
								printf("Error! Identifiers, literals and keywords should not include operators");
								return -1;
							}
							add_to_end(&list_head, opbuf, TOKENIZER_OPERATOR, counter);
							flagop = 0;
						}
					}
					if (flagop != 0)
					{
						#ifdef DEBUG	
							printf("%c is operator, line %d\n", opbuf[0], counter);
						#endif
						opbuf[1] = '\0';
						if(j > 0)
						{
							printf("Error! Identifiers, literals and keywords should not include operators");
							return -1;
						}
						add_to_end(&list_head, opbuf, TOKENIZER_OPERATOR, counter);
					}
				}
				flagop = 1;
			}
   		}
		if ((ch == '\t') && (j == 0) && (tabflag == 0))
			{
				#ifdef DEBUG
					printf("tab is depthcounter, line %d\n", counter);
				#endif
				add_to_end(&list_head,"\t", TOKENIZER_TAB, counter);
			}	
   		if(isalnum(ch) || ch == '_' || ch == '[' || ch == ']' )			/*check if ch is number or letter*/
		{
			tabflag = 1;
   			buffer[j++] = ch;		/*store ch in buffer[] and increase j for next character*/
			if (isalpha(ch))		/*if its letter*/
			{
				flag = 1;
			}
   		}
   		else if((ch == ' ' || ch == '\n' || ch == '\t') && (j != 0)) 		/*if its space, tab or \n(next line) after character*/
		{
			buffer[j] = '\0';	/*end of string*/
			j = 0;									
			if ((flag2 == 1) && (isKeyword(buffer) != 3) && (ch != ' ' || ch != '\n' || ch != '\t'))
			{
				#ifdef DEBUG
					printf("%s is keyword, line %d\n", tempbuffer, counter);
				#endif
				add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
				flag2 = 0;
			}
			if(isKeyword(buffer) == 1)		/*check if its keyword*/
			{
				#ifdef DEBUG
					printf("%s is keyword, line %d\n", buffer, counter);
				#endif
				add_to_end(&list_head, buffer, TOKENIZER_KEYWORD, counter);
			}
			else if(isKeyword(buffer) == 2)			/*if buffer[] is keyword "argument"*/
			{
				for (i = 0; i < 105 ; i++)
				{
					tempbuffer[i] = buffer[i];
				}
				flag2 = 1;
			}
			else if(isKeyword(buffer) == 3)		/*if buffer[] is keyword "size" */
			{
				if (flag2 == 1)  /*token "argument size"*/
				{
					strcat(tempbuffer, buffer);
					#ifdef DEBUG
						printf("%s is keyword, line %d\n", tempbuffer, counter);
					#endif
					add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
					flag2 = 0;
				}
				else		/*token "size"*/
				{
					#ifdef DEBUG
						printf("%s is keyword, line %d\n", buffer, counter);
					#endif
					add_to_end(&list_head, buffer, TOKENIZER_KEYWORD, counter); 
					flag2 = 0;
				}
			}
			else if (flag == 1) 	/*if buffer[] has letter is identifier*/
			{
				#ifdef DEBUG
					printf("%s is identifier, line %d\n", buffer, counter);
				#endif
				if(isalpha(*buffer) == 0)  /*if first character isn't letter*/
				{
					printf("Error! Identifier should start with letter");
					return -1;
				}
				add_to_end(&list_head, buffer, TOKENIZER_IDENTIFIER, counter);
			}
			else	/*if buffer is letter free*/
			{
				#ifdef DEBUG
					printf("%s is number, line %d\n", buffer, counter);
				#endif
				add_to_end(&list_head, buffer, TOKENIZER_NUMBER, counter);
			}
			if (ch == '\t' && tabflag == 0) 
			{
				printf("tab is depthcounter, line %d\n", counter);
				add_to_end(&list_head,"\t", TOKENIZER_TAB, counter);
			}
			for (i = 0 ; i < 105 ; i++)
			{
				buffer[i] = ' ';  /*empty buffer*/
			}
			flag = 0;
   		}
		else   /*other characters */
		{
			if (flag3 != 1 && ch != ' ' && ch != '\t' && ch != '\n')
			{
				tabflag = 1;
				buffer[j] = '\0';	/*close string*/
				j = 0;															
				if ((flag2 == 1) && (isKeyword(buffer) != 3))
				{
					#ifdef DEBUG
						printf("%s is keyword, line %d\n", tempbuffer, counter);
					#endif
					add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
					flag2 = 0;
				}	
				if(isKeyword(buffer) == 1)	/*check if its keyword*/
				{
					#ifdef DEBUG
						printf("%s is keyword, line %d\n", buffer, counter);
					#endif
					add_to_end(&list_head, buffer, TOKENIZER_KEYWORD, counter);
				}
				else if(isKeyword(buffer) == 2)				
				{
					for (i = 0; i < 105 ; i++)
					{
						tempbuffer[i] = buffer[i];
					}
					flag2 = 1;
				}
				else if(isKeyword(buffer) == 3)							
				{
					if (flag2 == 1)
					{
						strcat(tempbuffer, buffer);
						#ifdef DEBUG
							printf("%s is keyword, line %d\n", tempbuffer, counter);
						#endif
						add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
						flag2 = 0;
					}
					else
					{
						#ifdef DEBUG
							printf("%s is identifier, line %d\n", buffer, counter);
						#endif
						if(isalpha(*buffer) == 0)
						{
							printf("Error! Identifier should start with letter");
							return -1;
						}
						add_to_end(&list_head, buffer, TOKENIZER_IDENTIFIER, counter);
						flag2 = 0;
					}
				}
				else if (flag == 1)												
				{
					#ifdef DEBUG
						printf("%s is identifier, line %d\n", buffer, counter);
					#endif
					if(isalpha(*buffer) == 0)
					{
						printf("Error! Identifier should start with letter");
						return -1;
					}
					add_to_end(&list_head, buffer, TOKENIZER_IDENTIFIER, counter);
				}
				else																
				{
					#ifdef DEBUG
						printf("%s is number, line %d\n", buffer, counter);
					#endif
					add_to_end(&list_head, buffer, TOKENIZER_NUMBER, counter);
				}
				for (i = 0 ; i < 105 ; i++)
				{
					buffer[i] = ' ';
				}
				flag = 0;
				#ifdef DEBUG
					printf("%c is wrongvalue, line %d\n", ch, counter);
				#endif
				chbuf[0]= ch;
				chbuf[1]= '\0';
				printf("Error! Symbol that is not allowed in IPL");
				return -1;
				add_to_end(&list_head, chbuf, TOKENIZER_ERROR, counter);
			}
		}
		if (ch == '\n')		
		{
			#ifdef DEBUG
				printf ("end of line %d\n", counter);
			#endif
			add_to_end(&list_head, "\n", TOKENIZER_ENDOFLINE, counter);
			counter++;   /*count line*/
			tabflag = 0;
		}
	}
	if (j != 0) /*check if buffer is not empty*/
	{
		buffer[j] = '\0';	/*close string*/
		j = 0;															
		if(isKeyword(buffer) == 1)													
		{
			#ifdef DEBUG
				printf("%s is keyword, line %d\n", buffer, counter);
			#endif
			add_to_end(&list_head, buffer, TOKENIZER_KEYWORD, counter);
		}
		else if(isKeyword(buffer) == 2)											
		{
			for (i = 0; i < 105 ; i++)
			{
				tempbuffer[i] = buffer[i];
			}
			flag2 = 1;
		}
		else if(isKeyword(buffer) == 3)												
		{
			if (flag2 == 1)
			{
				strcat(tempbuffer, buffer);
				#ifdef DEBUG
					printf("%s is keyword, line %d\n", tempbuffer, counter);
				#endif
				add_to_end(&list_head, tempbuffer, TOKENIZER_KEYWORD, counter);
				flag2 = 0;
			}
			else
			{
				#ifdef DEBUG
					printf("%s is identifier, line %d\n", buffer, counter);
				#endif
				if(isalpha(*buffer) == 0)
				{
					printf("Error! Identifier should start with letter");
					return -1;
				}
				add_to_end(&list_head, buffer, TOKENIZER_IDENTIFIER, counter);
				flag2 = 0;
			}
		}
		else if (flag == 1)																
		{
			#ifdef DEBUG
				printf("%s is identifier, line %d\n", buffer, counter);
			#endif
			if(isalpha(*buffer) == 0)
			{
				printf("Error! Identifier should start with letter");
				return -1;
			}
			add_to_end(&list_head, buffer, TOKENIZER_IDENTIFIER, counter);
		}
		else																			
		{
			#ifdef DEBUG
				printf("%s is number, line %d\n", buffer, counter);
			#endif
			add_to_end(&list_head, buffer, TOKENIZER_NUMBER, counter);
		}
	}
	#ifdef DEBUG
		printf ("end of line %d\n", counter);
	#endif
	add_to_end(&list_head, "\n", TOKENIZER_ENDOFLINE, counter);
	add_to_end(&list_head, "EOF", EOF, counter); 
	fclose(fp);
	return 0;
}