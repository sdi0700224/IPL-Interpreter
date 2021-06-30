struct arraynode;
struct listnode;
struct whnode;

int add_to_end(struct listnode**, char[], int, int);
int detacher(char*);
int token(char*);
void printlist(struct listnode*);
int n_th(struct listnode*, int, struct listnode *);
int parser(int, char*[]);
int in(struct listnode *, char[]);
int addvalue(struct listnode*, int, int);
int add_to_while(struct whnode**, int, int, int);
void printwhlist(struct whnode*);
int delete(struct whnode**);
int statement(int);
int printline(int);
int changewh(struct whnode*, int, int);
int add_to_array(struct arraynode**, char[], int);
void printarraylist(struct arraynode*);
int deletelist(struct listnode **ptraddr);
int deletearray(struct arraynode**, char[]);
int changearray(struct arraynode*, char[], int, int);
int checkarray(struct arraynode*, char[]);
int arrayreturnv(struct arraynode*, char[], int);
int allnumbers(char[]);
int arrayparse(char[]);
int arrayseperator(char**);
int changevar(struct listnode*, char[], int);

struct listnode
{
    char *name;
    int type;
    int line;
    struct listnode *next;
}*list_head, *listvar_head; 

struct whnode
{
    int counter;
    int type;
    int true;
    struct whnode *next;
}*listwh_head;

struct arraynode
{
    char *name;
    int *array;
    int size;
    struct arraynode *next;
}*listarray_head;