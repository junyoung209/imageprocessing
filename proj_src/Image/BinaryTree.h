#pragma once
#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

typedef char BTData;


typedef struct _bTreeNode
{
	BTData data;
	struct _bTreeNode * left;
	struct _bTreeNode * right;

}BTreeNode;


BTreeNode * MakeBTreeNode(void);
BTData GetData(BTreeNode * bt);
void SetData(BTreeNode* bt, BTData data);

BTreeNode * GetLeftSubTree(BTreeNode * bt);
BTreeNode * GetRightSubTree(BTreeNode * bt);


void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub);
void MakeRightSubTree(BTreeNode * main, BTreeNode * sub);

typedef void VisitFuncPtr1(BTData data, int* index, char* str);
typedef void VisitFuncPtr2(char* str, int* index, BTreeNode *bt);

void PreorderTraverse(BTreeNode * bt, VisitFuncPtr1 action, char* outputstr, int* index);
void InorderTraverse(BTreeNode * bt, VisitFuncPtr1 action, char* outputstr, int* index);
void PostorderTraverse(BTreeNode * bt, VisitFuncPtr1 action, char* outputstr, int* index);

void DecryptionPreorderTraverse(BTreeNode * bt, VisitFuncPtr2 action, char* inputstr, int* index);
void DecryptionInorderTraverse(BTreeNode * bt, VisitFuncPtr2 action, char* inputstr, int* index);
void DecryptionPostorderTraverse(BTreeNode * bt, VisitFuncPtr2 action, char* inputstr, int* index);


int calLength(char* str);
void ShowCharData(char data);

char* Encryption(char* inputstr, char* outputstr, int key);

char* Decryption(char * inputstr, char * outputstr, int key);

void SetInputData(char * inputstr, int* index, BTreeNode *bt);
void GetCharData(char data, int* index, char* outputstr);
void DeleteTree(BTreeNode * bt);



#endif // !__BINARY_TREE_H__


