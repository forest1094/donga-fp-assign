/******************************
	File Structure hw3
	Theme		 : AVL tree
	First Update : 13.11.20
	Author		 : 이정보
	Last Update  : 13.11.24
******************************/

#include <stdio.h>
#include <stdlib.h>
 
#define DATASIZE 100
#define MAX(a, b)	((a) > (b) ? (a) : (b))

typedef struct Node {
	int key;
	struct Node *L_child;
	struct Node *R_child;
}avlNode;

/***  Function Part  ***/
int getHigh(avlNode *node, int height);		// AVL트리의 높이를 구한다
int getBF(avlNode *node, int bf);		// AVL트리의 균형인수를 구한다

avlNode *rotateSingle(avlNode *p, int bf);
avlNode *rotateDouble(avlNode *p, int bf);

avlNode *BFModify(avlNode *node, int bf);
avlNode *avlInsert(avlNode *root,int key);

void avlPreorder(avlNode *root, FILE * f_write);
 
void avlReset(avlNode *root);

/***  Main Function  ***/
int main(void)
{
	avlNode *root = NULL;
	FILE *f_read, *f_write;
	char readFile[] = "hw3.inp";
	char writeFile[] = "hw3.out";

	int data[DATASIZE];
	int i, dataLen;

	if((f_read = fopen(readFile, "r")) == NULL) {
		printf("file open error: %s\n", readFile);
		return 1;
	}
	if((f_write = fopen(writeFile, "w")) == NULL) {
		printf("file open error: %s\n", writeFile);
		return 1;
	}
		
	//// 파일에서 데이터 읽어오기
	i=0;
	while(1)
	{
		if(fscanf(f_read, "%d ", &data[i++]) == -1) {
			dataLen = --i;
			break;
		}
	}
	fclose(f_read);
	
	//// 읽은 데이터 AVL트리에 삽입
	for(i=0; i<dataLen; i++)
	{
		root = avlInsert(root, data[i]);
		avlPreorder(root, f_write);
		fprintf(f_write, "\n");
	}
	//fprintf(f_write, "*");
	fclose(f_write);

	//// AVL트리 초기화
	avlReset(root);

	return 0;
}

/***  Function Define  ***/
avlNode *rotateSingle(avlNode *p, int bf)
{
	avlNode *child;

	if (bf > 1)		//LL로테이션
	{
		child = p->L_child;
 		p->L_child = child->R_child;
		child->R_child = p;
	}
	else			//RR로테이션
	{
		child = p->R_child;
		p->R_child = child->L_child;
		child->L_child = p;
	}

	return child;
}

avlNode *rotateDouble(avlNode *p, int bf)
{
	avlNode *child;

	if (bf > 1)		//LR로테이션
	{
		child = p->L_child;
		p->L_child = rotateSingle(child, -2); //RR
		return rotateSingle(p, 2); //LL

	}
	else	//RL로테이션
	{
		child = p->R_child;
		p->R_child = rotateSingle(child, 2); //LL
		return rotateSingle(p, -2);	//RR
	}
}
 
int getHigh(avlNode *node, int height)
{
	if (node != NULL) {		//왼쪽, 오른쪽 서브트리중 높이가 높은쪽을 반환
		height = MAX(getHigh(node->L_child, height), getHigh(node->R_child, height));
	}
	return ++height;
}
 
int getBF(avlNode *node, int bf)
{
	int height=0;
	if (node == NULL)
		return 0;
	else
		bf = getHigh(node->L_child, height) - getHigh(node->R_child, height);

	return bf;
}
 
avlNode *BFModify(avlNode *node, int bf)
{
	bf = getBF(node, bf);

	if (bf >= 2) {
		if (getBF(node->L_child, bf) > 0)
			return rotateSingle(node, bf);
		else
			return rotateDouble(node, bf);
	}
	else if (bf <= -2) {
		if (getBF(node->R_child, bf) < 0)
			return rotateSingle(node, bf);
		else
			return rotateDouble(node, bf);
	}
	return node;
}
 
avlNode *avlInsert(avlNode *root,int key)
{
	int bf=0;

	if (root == NULL) {
		root = (avlNode*)malloc(sizeof(avlNode));
		if (root == NULL) {
			printf("메모리 할당 오류\n");
			exit(-1);
		}
		root->key = key;
		root->L_child = NULL;
		root->R_child = NULL;
	}
	else if (root->key > key)
		root->L_child = avlInsert(root->L_child,key);
		
	else if (root->key < key)
		root->R_child = avlInsert(root->R_child,key);
		
	root = BFModify(root, bf);
	return root;
}
 
void avlPreorder(avlNode *root, FILE * f_write)
{
	if(root) {
		fprintf(f_write, "%d ",root->key);
		avlPreorder(root->L_child, f_write);
		avlPreorder(root->R_child, f_write);
	}
}

void avlReset(avlNode *root)
{
	if(root) {
		avlReset(root->L_child);
		avlReset(root->R_child);
		free(root);
	}
	
}