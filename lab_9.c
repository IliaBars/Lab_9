#include <stdio.h>
#include <stdlib.h>

#define HASH_TABLE_SIZE 37

// RecordType
struct RecordType
{
    int		id;
    char	name;
    int		order;
};


// Fill out this structure
struct HashType {
    struct HashNode* head;
};

struct HashNode {
    struct RecordType *data;
    struct HashNode *next;
};

// Compute the hash function
int hash(int x) {
    int index = x % HASH_TABLE_SIZE; // Mode
    return index;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;

    for (i=0;i<hashSz;++i)
    {
        // if index is occupied with any records, print all
        struct HashNode *node = pHashArray[i].head;
        if (node != NULL) {
            printf("index %d ", i);

            while (1) {
                printf("-> %d, %c, %d ", node->data->id, node->data->name, node->data->order);
                if (node->next == NULL) break;
                node = node->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);
    // Your hash implementation
    struct HashType arr[HASH_TABLE_SIZE] = {0};

    // Fill elements
    for (int i = 0; i < recordSz; i++)
    {
        int record_id = pRecords[i].id;
        int index = hash(record_id);

        struct HashNode* newNode = malloc(sizeof(struct HashNode));
        newNode->data = &pRecords[i];
        newNode->next = NULL;

        // If empty
        if (arr[index].head == NULL) {
            arr[index].head = newNode;
        } else {
            // Add to the end of the list
            struct HashNode *lastNode = arr[index].head;
            while (1) {
                if (lastNode->next == NULL) break;
                lastNode = lastNode->next;
            }
            lastNode->next = newNode;
        }
    }

    displayRecordsInHash(arr, HASH_TABLE_SIZE);

    return 0;
}