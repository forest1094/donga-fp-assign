/***********************************
	File Structure hw4
	Theme		 : Hashing
	First Update : 13.12.06
	Author		 : 이정보
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
	int m;	//제수 m

	char readFile[] = "hw4.inp";
	char writeFile[] = "hw4.out";

	//// 입력, 출력파일 열기
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

	//// 입력파일 읽기
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
	
	//// 선형조사법 수행
	hashing(f_write, keys, length, m, 1);

	//// 이차조사법 수행
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

		if(tempIndex == index)				//완전히 가득찬경우
			return 1;	//full 이면 1을 반환

		if(hashTable[1][tempIndex] == '\0')	//빈 버켓을 발견한 경우
		{
			hashTable[1][tempIndex] = key;
			return 0;	//정상종료시 0을 반환
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

		if(tempIndex == index)				//완전히 가득찬경우
			return 1;	//full 이면 1을 반환
		
		if(hashTable[1][tempIndex] == '\0')	//빈 버켓을 발견한 경우
		{
			hashTable[1][tempIndex] = key;
			return 0;	//정상종료시 0을 반환
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

		if(hashTable[0][index] == '\0')			//첫번째 버켓이 비어있는 경우
			hashTable[0][index] = keys[i]; 
		
		else if(hashTable[1][index] == '\0') 	//두번째 버켓이 비어있는 경우
			hashTable[1][index] = keys[i];

		else
		{
			if(select == 1)			//선형조사법 수행
				result = linearProbing(m, keys[i], index, hashTable);
		
			else if(select == 2)	//이차조사법 수행
				result = quadraticProbing(m, keys[i], index, hashTable);
		}

		if(result == 1)		//오류체크
		{
			printf("hash table is full!\n");
			return;
		}
	}
	printAll(f_write, m, hashTable);	//해시테이블 출력함수

	return;
}