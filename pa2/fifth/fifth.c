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

void push(node **head, node* push_node, int* s_size, int* cycle){
    node* ptr = *head;
    if (ptr==NULL){
        *head = push_node;
    } else {
        while (ptr!=NULL){
            if (strcmp(ptr->val,push_node->val)==0){
                *cycle = 1;
                break;
            }
            ptr=ptr->next;
        }
        push_node->next=*head;
        *head = push_node;
    }

    *s_size = *s_size+1;
}

int inDegree(node **array, node* desired_vertex, int* marked_arr, int arr_size){
    int inDeg=0;
    for (int i = 0; i<arr_size; i++){
        if (marked_arr[hash(array[i]->val,array, arr_size)]==0){
            node * ptr = array[i]->next;
            while(ptr!=NULL){
                if (strcmp(ptr->val,desired_vertex->val)==0){
                    inDeg++;
                }
                ptr=ptr->next;
            }
        }
    }
    return inDeg;
}

node* pop(node** array, int arr_size, node **head, int* s_size, int hash_index, int* marked_arr, char*** topological, int** top_index){
    node * ptr = *head;
    node * rtn = createAdjNode(ptr->val, 0);
    *head = (*head)->next;
    free(ptr);
    *s_size = *s_size-1;
    if (marked_arr[hash_index]==0&&inDegree(array,rtn,marked_arr, arr_size)==0){
        strncpy((*topological)[**top_index], rtn->val, 81); 
        **top_index=**top_index+1;
    }
    return rtn;
}

//if push a node that is already in stack than it is a cycle 
//parenthesis arlindaound pointer
char** DFS(node** arr, int arr_size, int *top_index){

    int* marked_arr = calloc(arr_size,sizeof(int));
    int s_size = 1;
    int all_trav = 0;
    int source = 0;
    char** topological_sort = malloc(sizeof(char*)*arr_size);
    for (int i =0; i<arr_size;i++){
        topological_sort[i] = malloc(sizeof(char)*85);
    }

    while (all_trav==0){
        int cycle = 0;
        stack* dfs_stack = malloc(sizeof(stack));
        node* head = createAdjNode(arr[source]->val,0);
        dfs_stack->top=head;
        dfs_stack->bottom=NULL;

        while (s_size!=0){
            int hash_index = hash(dfs_stack->top->val, arr, arr_size);
            node* popped = pop(arr, arr_size, &dfs_stack->top,&s_size, hash_index,marked_arr, &topological_sort, &top_index);
            hash_index = hash(popped->val,arr,arr_size);
            if(marked_arr[hash_index]==0){
                marked_arr[hash_index]=1;
                for (int i = 0; i<arr_size;i++){
                    if(strcmp(arr[i]->val, popped->val)==0){
                        node* ptr = arr[i];
                        while(ptr!=NULL){
                        hash_index = hash(ptr->val, arr,arr_size);
                        if (marked_arr[hash_index]==0&&inDegree(arr,ptr,marked_arr, arr_size)==0){
                            push(&dfs_stack->top, createAdjNode(ptr->val,0), &s_size, &cycle);
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
    return topological_sort;
}

int main(int argc, char *argv[]){

    FILE *fp;
    fp = fopen(argv[1], "r");

    int total;
    char vertices[255];
    int top_index=0;
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

    char** topological = DFS(array, total, &top_index);

    printf("\n");
    if (top_index!=total){
        printf("CYCLE\n");
    } else {
        fp = fopen(argv[2], "r");
        char vertices[255]="";
        while(fscanf(fp, "%s", vertices)!=EOF){
            int* dist_arr = malloc(sizeof(int)*total);
            for (int i=0; i<total; i++){
                dist_arr[i] = INT_MAX;
            }
            dist_arr[hash(vertices, array, total)]=0;

            for (int i =0; i<total; i++){ //going through top sort
                char* u = topological[i];
                int j = 0;
                int hash_u = hash(u, array, total); //index of current topological vertex
                while(strcmp(array[j]->val, u)!=0){
                    j++;
                } //finding vertex adj list in array 
                node * ptr = array[j]->next;
                while(ptr!=NULL){
                    int hash_v = hash(ptr->val, array, total); //index of adjacent node 
                    if (dist_arr[hash_v]>(dist_arr[hash_u])){
                        if (dist_arr[hash_v]>(dist_arr[hash_u]+(ptr->cost))){
                        dist_arr[hash_v] = dist_arr[hash_u]+(ptr->cost);
                        }
                    }
                    ptr=ptr->next;
                }
            }
                for (int i = 0; i<total; i++){
                    int h = hash(array[i]->val, array, total);
                    if (dist_arr[h]==INT_MAX){
                        printf("%s INF\n", array[i]->val);
                    } else {
                        printf("%s %d\n", array[i]->val, dist_arr[h]);
                    }
                } 
                printf("\n"); 
            

            free(dist_arr);
        }
    }

    


    
    
    for (int i =0; i<total; i++){
        freeList(array[i]);
    } 
    
    for (int i =0; i<total; i++){
        free(topological[i]);
    }
    free(topological);
    free(array);
    fclose(fp);
    

    return 0;
}
