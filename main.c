#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**


                     *---___---*****MOHAMED FARID ABDELAZIZ SALAMA ID:6905*****---___---*
                     *---___---*****ADHAM MOHAMED ALY ABDELHAMID   ID;6744*****---___---*


**/


typedef struct
{
    char name[50];
    int starting_time;
    int remaining_time;
} Process;



typedef struct
{
    Process * data;
    struct Node *next;
} Node;



typedef struct
{
    Node * head;
    Node * tail;
} Queue;



Queue* init()
{
    Queue* q = malloc( sizeof(Queue) );
    q->head = NULL;
    q->tail = NULL;
    return q;
}



Node* newNode(Process *p)
{
    Node *n = malloc( sizeof(Node) );
    n->data = malloc(sizeof(Process) );
    n->data->starting_time = p->starting_time;
    n->data->remaining_time = p->remaining_time;
    strcpy(n->data->name, p->name );
    n->next = NULL;
    return n;
}



void enqueue(Queue * q, Process *p)
{
    Node* n = newNode(p);
    if(q->head==NULL) //Queue is empty
        q->head = q->tail = n;
    else
    {
        q->tail->next = (struct Node *)n;
        q->tail = n;
        q->tail->next = NULL;
    }
}



int isEmpty(Queue *q)
{
    if(q->head==NULL)
        return 1;
    else
        return 0;

}



Process* dequeue(Queue*q)
{
    if(q->head != NULL)
    {
        Process *result;
        Node* temp = q->head;
        result = temp->data;
        q->head = (Node *)temp->next;
        free(temp);
        if(q->head==NULL)
            q->tail=NULL;

        return result;
    }
    return NULL;
}


int getNumberOfLines(char *filename)
{
    FILE *fp = fopen(filename, "r");
    int lineCount = 0;
    char c;

    for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') lineCount ++;

    fclose(fp);
    return lineCount;
}



void destroy(Queue *q)
{
    while(!isEmpty(q)) dequeue(q);
    free(q);
}



void RoundRobin(char* filename)
{
    Queue *q = init();
    int time;
    FILE *fp;

    if(!(fp = fopen(filename, "r")))
    {
        printf("\x1b[31m<!> RoundRobin(char*): Error loading file.\x1b[0m\nAborting RoundRobin...");
        return;
    }
    int processCount = getNumberOfLines(filename);
    Process x[processCount];

    printf("\x1b[32mFile opened successfully.\x1b[0m\n");
    fscanf(fp, "Watching time slots = %d\n", &time);
    printf("TIME : %d\n", time);
    int n = 0;
    while(!feof(fp))
    {
        fscanf(fp, "%s %d %d\n", x[n].name, &x[n].starting_time, &x[n].remaining_time);
        n++;
    }
    fclose(fp);



    for(int j = 0; j < time; j++)
    {
        for(int i = 0; i < processCount; i++)
        {
            if(x[i].starting_time == j)
            {
                enqueue(q, &x[i]);
            }
        }
        if(isEmpty(q))
        {
            printf( "idle  (%d-->%d)\n", j, j+1 );
        }
        else if(q->head->data->remaining_time == 1 )
        {
            printf("%s    (%d-->%d) %s aborts\n", q->head->data->name, j, j + 1, q->head->data->name );
            dequeue(q);
        }
        else if(q->head->data->remaining_time > 1 )
        {
            printf("%s    (%d-->%d)\n", q->head->data->name, j, j + 1 );
            q->head->data->remaining_time = q->head->data->remaining_time - 1 ;
            enqueue(q, q->head->data);
            dequeue(q);
        }
    }
    printf( "stop\n" );
    destroy(q);
}







int main()
{
    system("cls");
    char filename[261];
    puts("Enter file name or Ctrl+Z to exit:");
    puts("----------------------------------");
    while(fgets(filename, 260, stdin) != NULL)
    {
        filename[strlen(filename)-1]='\0';
        if(fopen(filename,"r"))
        {
            puts("Initiating RoundRobin...");
            RoundRobin(filename);
        }
        else
        {
            puts("File Not Found!");
            puts("----------------------------------");
        }
        puts("Enter file name or Ctrl+Z to exit:");
        puts("----------------------------------");
    }

    return 0;
}
