#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct Vertex {
    int value;
    struct Vertex *next;
};
typedef struct Vertex Vertex;

void printGraphAdjacencyList(Vertex **vertices, int numberOfVertices);
void breadFirstTraversal(Queue *q, Vertex **vertices,char *isVisited, int numberOfVertices);

int main(int argc, char **argv)
{
    // Check if the command line arguments are correct
    if (argc != 2) {
        printf("Please run the program by the command - 'program_name input_file'\n");
        return 0;
    }
    
    // Open the input file
    char *inputFileName = argv[1];
    FILE *inputFile = fopen(inputFileName, "r");
    
    // Check if the file is valid
    if (inputFile == NULL) {
        printf("The file %s is invalid. Please check and try again.\n", inputFileName);
        return 0;
    }
    
    // Read the number of vertices and the starting vertex of the BF traversal
    int numberOfVertices; int startingVertex;
    fscanf(inputFile, "%d %d\n", &numberOfVertices, &startingVertex);
    startingVertex--; // The index starts from 0. The inputfile has it from 1. So adjusting by 1.
    
    // Form a adjacency list of the graph
    // (All the index are starting from 0, adjusted by 1 from inputfile)
    // (This is just done in code. The output will again be adjusted to match the indexing in inputfile.)
    Vertex **vertices = malloc(sizeof(Vertex *) * numberOfVertices);
    char *buffer = NULL; size_t bufferSize = 0;
    int n, bytesread; Vertex **v;
    for (int i = 0; i < numberOfVertices; i++) {
        getline(&buffer, &bufferSize, inputFile);
        char *temp = buffer;
        v = vertices + i;
        while (sscanf(buffer, "%d%n", &n, &bytesread) > 0) {
            Vertex *new = malloc(sizeof(Vertex));
            new->value = n-1;
            new->next = NULL;
            *v = new;
            v = &(new->next);
            buffer += bytesread;
        }
        buffer = temp;
        free(temp);
        buffer = NULL;
        bufferSize = 0;
    }
    
    Queue *q;
    newQueue(&q);
    
    int *p = malloc(sizeof(int));
    *p = startingVertex;
    enqueue(q, p);
    
    char *isVisited = calloc(sizeof(char) , numberOfVertices);
    isVisited[*p] = 1;
    breadFirstTraversal(q, vertices, isVisited, numberOfVertices);
    
    return 0;
}

void breadFirstTraversal(Queue *q, Vertex **vertices, char *isVisited, int numberOfVertices) {
    if (isEmpty(q)) {
        // Check if the all the vertices are visited or not.
        // If the graph is not connected, the queue might become empty prematurely
        // Hence we check for any unvisited vertex and start the BFS again from there
        int unvisitedVertex = -1;
        for (int i = 0; i < numberOfVertices; i++) {
            if(!isVisited[i]) {
                unvisitedVertex = i; 
                break;
            }
        }
            
        if (unvisitedVertex != -1) {
            // unvisited vertex found
            int *p = malloc(sizeof(int));
            *p = unvisitedVertex;
            enqueue(q, p);
            isVisited[*p] = 1;
            
            printf("\n"); // Print the traversal of the new component on a newline
            breadFirstTraversal(q, vertices, isVisited, numberOfVertices);
            return;
        } else {
            // There is no other unvisited vertex remaining
            // The BFS is complete . This is the base case of recursion
            return;
        }
    }
    
    int *v = dequeue(q);
    printf("%d ", (*v) + 1); /* Adjusting the indexing scheme of the nodes while showing output
     to match it with the indexing scheme of input file*/
    
    // Add the unvisisted neighbours to the queue
    Vertex *ver = vertices[*v];
    while(ver != NULL) {
        int value = ver->value;
        if (!isVisited[value]) {
            int *p = malloc(sizeof(int));
            *p = value;
            enqueue(q, p);
            isVisited[value] = 1;
        }
        ver = ver->next;
    }
    
    breadFirstTraversal(q, vertices, isVisited, numberOfVertices);
}

void printGraphAdjacencyList(Vertex **vertices, int numberOfVertices) {
    for (int i = 0; i < numberOfVertices; i++) {
        Vertex *v = vertices[i];
        printf("%d - ", i+1);
        while(v != NULL) {
            printf("%d ", v->value + 1);
            v = v->next;
        }
        printf("\n");
    }
}
