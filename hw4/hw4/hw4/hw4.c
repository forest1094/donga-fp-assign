/***********************************
	File Structure hw4
	Theme		 : Hashing
	First Update : 13.12.06
	Author		 : ������
	Last Update  : 13.12.07
***********************************/

#include <stdio.h>

#define TABLESIZE 30

/***  Function Part  ***/
void printAll(FILE* f_write, int m, int hashTable[][TABLESIZE]);
void hashing(FILE* f_write, int keys[], int length, int m, int select); 
int linearProbing(int m, int key, int index, int hashTable[][TABLESIZE]);
int quadraticProbing(int m, int key, int index, int hashTable[][TABLESIZE]);

/***  Main Function  ***/
int main(void)
{
	FILE *f_read = NULL, *f_write = NULL;
	
	int keys[TABLESIZE]={'\0',};
	int length=0, i=0;
	int m;	//���� m

	char readFile[] = "hw4.inp";
	char writeFile[] = "hw4.out";

	//// �Է�, ������� ����
	if((f_read = fopen(readFile, "r")) == NULL) 
	{
		printf("Can't open file : %s\n", readFile);
		return 1;
	}
	if((f_write = fopen(writeFile, "w")) == NULL) 
	{
		printf("Can't open file : %s\n", writeFile);
		return 1;
	}

	//// �Է����� �б�
	fscanf(f_read, "%d\n", &m);
	while(1)
	{
		if(-1 == fscanf(f_read, "%d ", &keys[i++]))
		{
			length = i;
			break;
		}
	}
	length--;
	
	//// ��������� ����
	hashing(f_write, keys, length, m, 1);

	//// ��������� ����
	hashing(f_write, keys, length, m, 2);

	fclose(f_read);
	fclose(f_write);
	return 0;
}

/***  Function Define  ***/
void printAll(FILE* f_write, int m, int hashTable[][TABLESIZE])
{
	int i, j;
	
	for(j=0; j<2; j++)
	{
		for(i=0; i<m; i++)
			fprintf(f_write, "%d ", hashTable[j][i]);

		fprintf(f_write, "\n");
	}
}
int linearProbing(int m, int key, int index, int hashTable[][TABLESIZE])
{
	int tempIndex, tempKey;
	
	tempKey = key;

	while(1)
	{
		tempIndex = (++tempKey)%m;

		if(tempIndex == index)				//������ ���������
			return 1;	//full �̸� 1�� ��ȯ

		if(hashTable[1][tempIndex] == '\0')	//�� ������ �߰��� ���
		{
			hashTable[1][tempIndex] = key;
			return 0;	//��������� 0�� ��ȯ
		}
	}	
}

int quadraticProbing(int m, int key, int index, int hashTable[][TABLESIZE])
{
	int tempIndex;
	int i=0;

	while(1)
	{
		i++;
		tempIndex = (index+(2*i*i))%m;

		if(tempIndex == index)				//������ ���������
			return 1;	//full �̸� 1�� ��ȯ
		
		if(hashTable[1][tempIndex] == '\0')	//�� ������ �߰��� ���
		{
			hashTable[1][tempIndex] = key;
			return 0;	//��������� 0�� ��ȯ
		}
	}
}

void hashing(FILE* f_write, int keys[], int length, int m, int select)
{
	int i, index;
	int result;
	int hashTable[2][TABLESIZE] = {'\0',};

	for(i=0; i<length; i++)
	{
		index = keys[i]%m;

		if(hashTable[0][index] == '\0')			//ù��° ������ ����ִ� ���
			hashTable[0][index] = keys[i]; 
		
		else if(hashTable[1][index] == '\0') 	//�ι�° ������ ����ִ� ���
			hashTable[1][index] = keys[i];

		else
		{
			if(select == 1)			//��������� ����
				result = linearProbing(m, keys[i], index, hashTable);
		
			else if(select == 2)	//��������� ����
				result = quadraticProbing(m, keys[i], index, hashTable);
		}

		if(result == 1)		//����üũ
		{
			printf("hash table is full!\n");
			return;
		}
	}
	printAll(f_write, m, hashTable);	//�ؽ����̺� ����Լ�

	return;
}