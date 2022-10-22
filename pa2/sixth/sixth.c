#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

typedef struct Queue {
    node* head;
} queue;

int dequeue (queue** q){
    if ((*q)->head==NULL){
        return -1;
    }
    node* temp = (*q)->head;
    (*q)->head = (*q)->head->next;
    free(temp);
    return 1;
}

void enqueue (queue** q, node* insert){
    if ((*q)->head==NULL){
        (*q)->head = insert;
        return;
    }
    
    insert->next = (*q)->head;
    (*q)->head = insert;
}

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
                while(nextNode!=NULL&&(strcmp(nextNode->val,edge2)<0)){ //ascending order
                    currentNode = nextNode;
                    nextNode = nextNode->next;
                }
                currentNode->next = createAdjNode(edge2, cost);
                currentNode->next->next = nextNode;
            break;
        } 
    }  
}


void Dijkstra(node ** arr, int arr_size, char* source){

    int* visited_arr = calloc(arr_size, sizeof(int));
    int* dist_arr = calloc(arr_size, sizeof(int));
    queue* q = malloc(sizeof(queue));
    q->head = NULL;

    for (int i =0; i<arr_size; i++){
        dist_arr[i] = INT_MAX;
    }

    int source_index = hash(source, arr, arr_size);
    dist_arr[source_index] = 0;
    enqueue(&q,createAdjNode(source,0));

     while (dequeue(&q)!=-1){
        node * ptr = arr[source_index];
        while(ptr!=NULL){
            int adj_index = hash(ptr->val, arr, arr_size);
            if (visited_arr[adj_index]==0){
                if ((dist_arr[source_index]+ptr->cost)<dist_arr[adj_index]){
                    dist_arr[adj_index] = (dist_arr[source_index]+ptr->cost);
                    enqueue(&q, createAdjNode(ptr->val,0));
                }
            }
            ptr=ptr->next;
        }

        visited_arr[source_index] = 1;
        int next_index;
        int next_node = INT_MAX;
        for (int i = 0; i<arr_size; i++){
            if (dist_arr[i]<next_node&&visited_arr[i]==0){
                next_node = dist_arr[i];
                next_index = i;
            }
        }

        source_index = next_index;
        visited_arr[source_index] = 1; 

    }  

    for (int i = 0; i< arr_size; i++){
        if (dist_arr[i]==INT_MAX){
            printf("%s INF\n", arr[i]->val);
        } else {
            printf("%s %d\n", arr[i]->val, dist_arr[i]);
        }
    }
    printf("\n");
    



    free(visited_arr);
    free(dist_arr);
    free(q);

}


int main(int argc, char *argv[]){

    FILE *fp;
    fp = fopen(argv[1], "r");

    int total;
    char vertices[255];
    fscanf(fp,"%d", &total);

    node** array = malloc(total*sizeof(node));
    char** arr_to_be_sorted = malloc(total*sizeof(char*));

    
    for (int i =0; i<total; i++){
        fscanf(fp,"%s",vertices);
        arr_to_be_sorted[i] = malloc(sizeof(char)*80);
        strcpy(arr_to_be_sorted[i],vertices);
    }

    sort_vertex(&arr_to_be_sorted, total);

    for (int i =0; i<total; i++){
        array[i] = createAdjNode(arr_to_be_sorted[i],0);
        free(arr_to_be_sorted[i]);
    } 
    free(arr_to_be_sorted); 


    char edge1[80];
    char edge2[80];
    double cost;
    while (fscanf(fp,"%s %s %lf", edge1, edge2, &cost)!=EOF){
        addEdge(&array, edge1, edge2, total, cost);
    }

    fp = fopen(argv[2], "r");
    char* source = malloc(sizeof(char)*80);
    while (fscanf(fp,"%s", source)!=EOF){
        Dijkstra(array, total, source);
    }

    

    free(source);
   

   

    
    
    for (int i =0; i<total; i++){
        freeList(array[i]);
    } 
    
    free(array);
    fclose(fp);
    

    return 0;
}
