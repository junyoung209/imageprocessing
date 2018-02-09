#include "stdafx.h"
#include <Stdio.h>
#include <stdlib.h>
#include <string>
#include "BinaryTree.h"
using namespace std;

BTreeNode * MakeBTreeNode(void)
{
	BTreeNode* nd = (BTreeNode*)malloc(sizeof(BTreeNode));
	nd->left = NULL;
	nd->right = NULL;
	return nd;
}

BTData GetData(BTreeNode* bt)
{
	return bt->data;
}

void SetData(BTreeNode * bt, BTData data) {
	bt->data = data;
}

BTreeNode * GetLeftSubTree(BTreeNode* bt) {
	return bt->left;
}

BTreeNode *  GetRightSubTree(BTreeNode * bt) {
	return bt->right;
}


void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub) {
	if (main->left != NULL)
		free(main->left);

	main->left = sub;
}
void MakeRightSubTree(BTreeNode * main, BTreeNode * sub) {
	if (main->right != NULL)
		free(main->right);

	main->right = sub;
}

void PreorderTraverse(BTreeNode* bt, VisitFuncPtr1 action, char* outputstr, int* index) {

	if (bt == NULL) {
		return;
	}
	action(bt->data, index, outputstr);
	PreorderTraverse(bt->left, action, outputstr, index);
	PreorderTraverse(bt->right, action, outputstr, index);
}

void InorderTraverse(BTreeNode * bt, VisitFuncPtr1 action, char* outputstr, int* index) {

	if (bt == NULL) {
		return;
	}
	InorderTraverse(bt->left, action, outputstr, index);
	action(bt->data, index, outputstr);
	InorderTraverse(bt->right, action, outputstr, index);
}

void PostorderTraverse(BTreeNode * bt, VisitFuncPtr1 action, char* outputstr, int* index) {

	if (bt == NULL) {
		return;
	}
	PostorderTraverse(bt->left, action, outputstr, index);
	PostorderTraverse(bt->right, action, outputstr, index);
	action(bt->data, index, outputstr);
}

void DecryptionPreorderTraverse(BTreeNode *bt1, VisitFuncPtr2 action, char* inputstr, int* index) {
	//int length = calLength(inputstr);
	if (bt1 == NULL) {
		return;
	}
	action(inputstr, index, bt1);
	DecryptionPreorderTraverse(bt1->left, action, inputstr, index);
	DecryptionPreorderTraverse(bt1->right, action, inputstr, index);

}

void DecryptionInorderTraverse(BTreeNode * bt1, VisitFuncPtr2 action, char* inputstr, int* index) {
	if (bt1 == NULL) {
		return;
	}
	DecryptionInorderTraverse(bt1->left, action, inputstr, index);
	action(inputstr, index, bt1);
	DecryptionInorderTraverse(bt1->right, action, inputstr, index);

}

void DecryptionPostorderTraverse(BTreeNode * bt1, VisitFuncPtr2 action, char* inputstr, int* index) {
	if (bt1 == NULL) {
		return;
	}
	DecryptionPostorderTraverse(bt1->left, action, inputstr, index);
	DecryptionPostorderTraverse(bt1->right, action, inputstr, index);
	action(inputstr, index, bt1);

}


void DeleteTree(BTreeNode * bt) {
	if (bt == NULL)
		return;

	DeleteTree(bt->left);
	DeleteTree(bt->right);

	printf("del tree data: %c\n", bt->data);
	free(bt);
}

int calLength(char* str) {
	int length = 0;
	char* p_str = str;
	while (1) {
		if (*p_str == '\0')
			break;
		else {
			length++;
			p_str++;
		}
	}

	return length;
}
char* Encryption(char* inputstr, char* outputstr, int key) {
	int length = calLength(inputstr);
	int number = 0;
	BTreeNode * Node_arr[100];


	//initiate Node
	for (int i = 1; i <= length; i++) {
		Node_arr[i] = MakeBTreeNode();
		SetData(Node_arr[i], inputstr[i - 1]);

	}
	//set the tree

	if (length % 2 == 0) {
		number = length / 2;
		for (int root = 1; root <= number; root++) {


			MakeLeftSubTree(Node_arr[root], Node_arr[root * 2]);

			if (root != number) {
				MakeRightSubTree(Node_arr[root], Node_arr[(root * 2) + 1]);
			}


		}


	}
	else {
		int number = (length - 1) / 2;
		for (int root = 1; root <= number; root++) {


			MakeLeftSubTree(Node_arr[root], Node_arr[root * 2]);

			MakeRightSubTree(Node_arr[root], Node_arr[(root * 2) + 1]);


		}

	}


	int index = 0;




	//key encryption
	switch (key)
	{
	case 1:
		PreorderTraverse(Node_arr[1], GetCharData, outputstr, &index);
		break;
	case 2:
		InorderTraverse(Node_arr[1], GetCharData, outputstr, &index);
		break;
	case 3:
		PostorderTraverse(Node_arr[1], GetCharData, outputstr, &index);
		break;
	default:
		break;
	}

	outputstr[index] = NULL;

	return outputstr;
}

char* Decryption(char * inputstr, char * outputstr, int key) {
	int length = calLength(inputstr);
	int number = 0;
	BTreeNode * Node_arr[100];
	//BTreeNode * Node_arr2[100];
	//initiate Node
	for (int i = 1; i <= length; i++) {
		Node_arr[i] = MakeBTreeNode();
		//Node_arr2[i] = MakeBTreeNode();
		//SetData(Node_arr[i], 'a');
	}

	//set the tree

	if (length % 2 == 0) {
		number = length / 2;
		for (int root = 1; root <= number; root++) {
			MakeLeftSubTree(Node_arr[root], Node_arr[root * 2]);

			if (root != number) {
				MakeRightSubTree(Node_arr[root], Node_arr[(root * 2) + 1]);
			}
		}
	}
	else {
		int number = (length - 1) / 2;
		for (int root = 1; root <= number; root++) {
			MakeLeftSubTree(Node_arr[root], Node_arr[root * 2]);

			MakeRightSubTree(Node_arr[root], Node_arr[(root * 2) + 1]);

		}
	}

	int index = 1;

	switch (key) {
	case 1:
		DecryptionPreorderTraverse(Node_arr[1], SetInputData, inputstr, &index);
		break;
	case 2:
		DecryptionInorderTraverse(Node_arr[1], SetInputData, inputstr, &index);
		break;
	case 3:
		DecryptionPostorderTraverse(Node_arr[1], SetInputData, inputstr, &index);
		break;
	default:
		break;
	}


	for (int i = 1; i <= length; i++) {

		outputstr[i - 1] = Node_arr[i]->data;
	}

	outputstr[index - 1] = NULL;

	return outputstr;
}


void SetInputData(char * inputstr, int* index, BTreeNode *bt) {
	bt->data = inputstr[*index - 1];
	*index = *index + 1;
}

void ShowCharData(char data) {
	printf("%c", data);
}
void GetCharData(char data, int* index, char* outputstr) {
	outputstr[*index] = data;
	*index = *index + 1;
}



