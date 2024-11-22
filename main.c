#include "openssl/crypto.h"
#include "openssl/sha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct block {
    unsigned char prevHash[SHA256_DIGEST_LENGTH];
    int BlockData;
    struct block *link;
}*head;
void addBlock(int);
void verifyChain();
void alterNthBlock(int, int);
void hackChain();
void hashPrinter(unsigned char[]);
int hashCompare(unsigned char[], unsigned char[]);
void printBlock(struct block *);
void printAllBlocks();

void addBlock(int data)
{
    if (head==NULL) {
        head=malloc(sizeof(struct block));
        SHA256("", sizeof(""), head->prevHash);
        head->BlockData=data;
        return;
    }
    struct block *currentBlock=head;
    while (currentBlock->link)
        currentBlock=currentBlock->link;
    struct block *newBlock=malloc(sizeof(struct block));
    currentBlock->link=newBlock;
    newBlock->BlockData=data;
    SHA256((unsigned char *)currentBlock, sizeof(currentBlock), newBlock->prevHash);
}
void verifyChain()
{
    struct block *currentBlock=head->link;
    struct block *previousBlock=head;
    while (currentBlock) {
        if (memcmp(currentBlock->prevHash, previousBlock->prevHash, SHA256_DIGEST_LENGTH))
            printf("Block chain is invalid\n");
        previousBlock=currentBlock;
        currentBlock=currentBlock->link;
    }
    printf("Block chain is valid\n");
}
void alterNthBlock(int n, int newBlockData)
{
    struct block *currentBlock=head;
    int i=0;
    while (currentBlock) {
        if (i==n) {
            currentBlock->BlockData=newBlockData;
            return;
        }
        i++;
        currentBlock=currentBlock->link;
    }
}
void hackChain()
{
    struct block *currentBlock=head->link;
    while (currentBlock->link)
        currentBlock=currentBlock->link;
    alterNthBlock(1, 1337);
    SHA256((unsigned char *)currentBlock, sizeof(currentBlock), currentBlock->prevHash);
}
void hashPrinter(unsigned char hash[])
{
    for (int i=0; i<SHA256_DIGEST_LENGTH; i++)
        printf("%02x", hash[i]);
    printf("\n");
}
int hashCompare(unsigned char hash1[], unsigned char hash2[])
{
    return memcmp(hash1, hash2, SHA256_DIGEST_LENGTH);
}
void printBlock(struct block *b)
{
    printf("%d\n", b->BlockData);
    hashPrinter(b->prevHash);
    printf("\n");
}
void printAllBlocks()
{
    struct block *currentBlock=head;
    while (currentBlock) {
        printBlock(currentBlock);
        currentBlock=currentBlock->link;
    }
}
int main()
{
    head=NULL;
    int n=10;
    for (int i=1; i<=n; i++)
        addBlock(i*10);
    printf("Original Block Chain\n");
    printAllBlocks();
    verifyChain();
    hackChain();
    printf("After hacking\n");
    printAllBlocks();
    verifyChain();
    return 0;
}
// gcc main.c -o main -lcrypto
// ./main