#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

void sort_vertex(char*** arr, int size){
    char temp[80];
    for (int i=0; i<size; i++){
        for (int j=0; j<size-1-i;j++){
            if (strcmp((*arr)[j],(*arr)[j+1])>0){
                strcpy(temp, (*arr)[j]);
                strcpy((*arr)[j], (*arr)[j+1]);
                strcpy((*arr)[j+1], temp);
            }
        }
    } 
}

typedef struct node {
    char val[80];
    struct node* next;
    int cost;
} node;

typedef struct stack {
    node *top;
    node *bottom;

} stack;

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

node* createAdjNode(char* c, double cost){
    node* rtn = malloc(sizeof(node));
    strcpy(rtn->val,c);
    rtn->next = NULL;
    rtn->cost=cost;
    return rtn;
}

void addEdge(node*** arr, char* edge1, char* edge2, int arr_size, double cost){
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,edge1)==0){ //found correct node
                node* currentNode = arr[0][i];
                node* nextNode = arr[0][i]->next;
                while(nextNode!=NULL&&(strcmp(nextNode->val,edge2)>0)){ //alpha is smaller 
                    currentNode = nextNode;
                    nextNode = nextNode->next;
                }
                currentNode->next = createAdjNode(edge2, cost);
                currentNode->next->next = nextNode;
            break;
        } 
    }  
}

void push(node **head, node* push_node, int* s_size){
    node* ptr = *head;
    if (ptr==NULL){
        *head = push_node;
    } else {
        push_node->next=*head;
        *head = push_node;
    }

    *s_size = *s_size+1;
}
//top of stack is head in LL
node* pop(node **head, int* s_size, int hash_index, int* marked_arr){
    node * ptr = *head;
    node * rtn = createAdjNode(ptr->val, 0);
    *head = (*head)->next;
    free(ptr);
    *s_size = *s_size-1;

    if (marked_arr[hash_index]==0){
    printf("%s ", rtn->val);
    }
    return rtn;
}

void DFS(node** arr, int arr_size){

    int* marked_arr = calloc(arr_size,sizeof(int));
    int s_size = 1;
    int all_trav = 0;
    int source = 0;

    while (all_trav==0){
        stack* dfs_stack = malloc(sizeof(stack));
        node* head = createAdjNode(arr[source]->val,0);
        dfs_stack->top=head;
        dfs_stack->bottom=NULL;

        while (s_size!=0){
            int hash_index = hash(dfs_stack->top->val, arr, arr_size);
            node* popped = pop(&dfs_stack->top,&s_size, hash_index,marked_arr);
            hash_index = hash(popped->val,arr,arr_size);
            if(marked_arr[hash_index]==0){
                marked_arr[hash_index]=1;
                for (int i = 0; i<arr_size;i++){
                    if(strcmp(arr[i]->val, popped->val)==0){
                        node* ptr = arr[i];
                        while(ptr!=NULL){
                        hash_index = hash(ptr->val, arr,arr_size);
                        if (marked_arr[hash_index]==0){
                            push(&dfs_stack->top, createAdjNode(ptr->val,0), &s_size);
                        }
                        ptr=ptr->next;
                        }
                    }
                }

            } 
            free(popped);
        }
        for (int i =0; i<arr_size;i++){
            if (marked_arr[i]==0){
                all_trav=0;
                source = i;
                s_size=1;
                break;
            } else {
                all_trav = 1;
            }
        }
    free(dfs_stack);
    }
    free(marked_arr);
}

int main (int argc, char *argv[]){

    FILE * fp;
    fp = fopen(argv[1], "r");
    char vertexes[255];
    int x;
    fscanf(fp,"%d",&x);
    node** array = malloc(x*sizeof(node));
    char** arr_to_be_sorted = malloc(x*sizeof(char*));

    
    for (int i =0; i<x; i++){
        fscanf(fp,"%s",vertexes);
        arr_to_be_sorted[i] = malloc(sizeof(char)*80);
        strcpy(arr_to_be_sorted[i],vertexes);
    }

    sort_vertex(&arr_to_be_sorted, x);

    for (int i =0; i<x; i++){
        array[i] = createAdjNode(arr_to_be_sorted[i],0);
        free(arr_to_be_sorted[i]);
    } 
    free(arr_to_be_sorted); 


    char edge1[80];
    char edge2[80];
    double cost;
    while (fscanf(fp,"%s %s %lf", edge1, edge2, &cost)!=EOF){
        addEdge(&array, edge1, edge2, x, cost);
    }


    DFS(array, x); 


    for (int i =0; i<x; i++){
        freeList(array[i]);
    } 
    
    free(array); 
    fclose(fp);
    

    return 0; 
}
