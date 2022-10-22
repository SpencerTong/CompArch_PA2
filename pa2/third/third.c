#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


typedef struct node {
    char val[80];
    struct node* next;
} node;

typedef struct queue {
    node *Qhead;
    node *Qtail;

} queue;

int hash(char* vert, node **array, int arr_size){
    int hash = 0;
    for (int i = 0; i<arr_size; i++){
        if (strcmp(array[i]->val,vert)==0){
            hash = i;
        }
    }
    return hash;
}

void freeList(node *head)
{
    node *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}


node* createAdjNode(char* c){
    node* rtn = malloc(sizeof(node));
    strcpy(rtn->val,c);
    rtn->next = NULL;
    return rtn;
}

void addEdge(node*** arr, char* edge1, char* edge2, int arr_size){
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,edge1)==0){ //found correct node
                node* currentNode = arr[0][i];
                node* nextNode = arr[0][i]->next;
                while(nextNode!=NULL&&(strcmp(nextNode->val,edge2)<0)){ //alpha is smaller 
                    currentNode = nextNode;
                    nextNode = nextNode->next;
                }
                currentNode->next = createAdjNode(edge2);
                currentNode->next->next = nextNode;
            
        } 
        if (strcmp(arr[0][i]->val,edge2)==0){
            node* currentNode = arr[0][i];
                node* nextNode = arr[0][i]->next;
                while(nextNode!=NULL&&(strcmp(nextNode->val,edge1)<0)){ //alpha is smaller 
                    currentNode = nextNode;
                    nextNode = nextNode->next;
                }
                currentNode->next = createAdjNode(edge1);
                currentNode->next->next = nextNode;
        }
    } 
}

void enqueue(node **head, node* insert, int* q_size){
    if (*head==NULL){
        *head = insert;
    } else{
        node *ptr = *head;
        while(ptr->next!=NULL){
        ptr = ptr->next;
        }
        ptr->next = insert;
    }
    *q_size = *q_size+1;
}

node* dequeue(node **head, int* q_size){
    node *temp = *head;
    *head = (*head)->next;
    *q_size=*q_size-1;
    printf("%s ",temp->val);
    node* rtn = createAdjNode(temp->val);
    free(temp);
    return rtn;
}

void BFS(char* desired_vert, node** arr, int arr_size){
    
    int* marked = calloc(arr_size, sizeof(int)); 
    queue* Queue = malloc(sizeof(queue));
    node * head = createAdjNode(desired_vert);
    marked[hash(desired_vert,arr, arr_size)]=1;
    Queue->Qhead = head;
    Queue->Qtail = NULL;
    Queue->Qhead->next=Queue->Qtail;
    int q_size = 1;


    while (q_size!=0){
        node* rtn = dequeue(&Queue->Qhead, &q_size);
        for (int i = 0; i<arr_size; i++){
            if (strcmp(arr[i]->val, rtn->val)==0){
                node *ptr = arr[i];
                while(ptr!=NULL){
                int hash_index = hash(ptr->val,arr, arr_size);
                if (marked[hash_index]==0){
                    enqueue(&Queue->Qhead, createAdjNode(ptr->val) ,&q_size);
                    marked[hash_index] = 1;
                }
                ptr=ptr->next;
                }
            }
        }
        free(rtn);
    }

    free(Queue);
    free(marked);
    //add vertex to queue, find in array, mark as visited, traverse adj list if not visited then add 
    //dequeue(returns val of node), check if visited, 

    //enqueue once outside, then while (queue not empty), dequeue and look at all of its neighbors, if not marked you enqueue 
    // outer loop dequeues, inner loop enqueues 

  



}


int main (int argc, char *argv[]){

    FILE * fp;
    fp = fopen(argv[1], "r");
    char vertexes[255];
    int x;
    fscanf(fp,"%d",&x);
    node** array = malloc(x*sizeof(node));

    for (int i =0; i<x; i++){
        fscanf(fp,"%s",vertexes);
        array[i] = createAdjNode(vertexes);
    }


    char edge1[80];
    char edge2[80];
    while (fscanf(fp,"%s %s", edge1, edge2)!=EOF){
        addEdge(&array, edge1, edge2, x);
    }

    fp = fopen(argv[2], "r");

    char source[80];

    while(fscanf(fp, "%s", source)!=EOF){
        BFS(source, array, x);
        printf("\n");
    }


    for (int i =0; i<x; i++){
        freeList(array[i]);
    }
    
    free(array);
    fclose(fp);
    

    return 0; 
}
