#include <stdio.h> 
#include <stdlib.h>
#include <string.h>


typedef struct node {
    char val[80];
    struct node* next;
    int cost;
} node;

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

void addEdge(node*** arr, char* edge1, char* edge2, int arr_size, int cost){
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,edge1)==0){ //found correct node
                node* currentNode = arr[0][i];
                node* nextNode = arr[0][i]->next;
                while(nextNode!=NULL&&(strcmp(nextNode->val,edge2)<0)){ //alpha is smaller 
                    currentNode = nextNode;
                    nextNode = nextNode->next;
                }
                currentNode->next = createAdjNode(edge2, cost);
                currentNode->next->next = nextNode;
            break;
        } 
    } 
}

void inDegree(char* desired_vert, node*** arr, int arr_size){
    int degree = 0;
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,desired_vert)!=0){
            node* ptr = arr[0][i]->next;
            while(ptr!=NULL){
                if (strcmp(ptr->val,desired_vert)==0){
                    degree++;
                    break;
                }
                ptr = ptr->next;
            }
        }
    }
    printf("%d\n", degree);
}

void outDegree(char* desired_vert, node*** arr, int arr_size){
    int degree = 0;
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,desired_vert)==0){
            node* ptr = arr[0][i]->next;
            while(ptr!=NULL){
                degree++;
                ptr = ptr->next;
            }
            break;
        }
    }
    printf("%d\n", degree);

}


void adjacencyQ(char* desired_vert, node*** arr, int arr_size){
    for (int i = 0; i<arr_size; i++){
        if (strcmp(arr[0][i]->val,desired_vert)==0){
            node* ptr = arr[0][i]->next;
            while(ptr!=NULL){
                printf("%s ",ptr->val);
                ptr = ptr->next;
            }
            break;
        }
    }
    printf("\n");
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
        array[i] = createAdjNode(vertexes, 0);
    }


    char edge1[80];
    char edge2[80];
    double cost;
    while (fscanf(fp,"%s %s %lf", edge1, edge2, &cost)!=EOF){
        addEdge(&array, edge1, edge2, x, cost);
    }

    fp = fopen(argv[2], "r");

    char query[80];
    char desired_vert[80];

    while(fscanf(fp, "%s %s", query, desired_vert)!=EOF){
        if (query[0]=='a'){
            adjacencyQ(desired_vert, &array, x);
        } else if (query[0]=='i'){
            inDegree(desired_vert, &array, x);
        } else if (query[0]=='o'){
            outDegree(desired_vert,&array, x);
        }
    }


    for (int i =0; i<x; i++){
        freeList(array[i]);
    }
    
    free(array);
    fclose(fp);
    

    

    return 0; 
}