#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strLen 4
#define qLen 10
#define radix 26

typedef struct {
	char str[strLen];
}String;

typedef struct {
	String Queue[qLen];
	int front, rear;
}QueueType;

////  파일관련 함수   ////
int readFile(FILE * f_r, String arr[]);
	//파일 전체를 읽어서 Element형 배열에 저장하는 함수
	//1.읽어올 파일포인터 2.읽은 파일내용을 저장할 배열

////  큐 관련 함수  ////
QueueType * createQueue();
int isEmpty(QueueType * Q);
int isFull(QueueType * Q);
void enQueue(QueueType * Q, String item);
String deQueue(QueueType * Q);

////  기수정렬 관련 함수 ////
int clearBucket(QueueType *Q[], String *arr, int addr);
void radixSort(String *arr, int count);

int main(void)
{
	FILE *f_read, *f_write;
	
	int i;
	int count;
	
	char fr_name[] = "hw1.inp"; 
	char fw_name[] = "hw1.out";
	String arr[qLen]={'\0'};
	
	/** 파일 열기 **/
	if((f_read = fopen(fr_name, "r")) == NULL)	//파일 여는중 오류 발생시 메세지 출력
		printf("can't file open!: %s\n", fr_name);
		
	if((f_write = fopen(fw_name, "w")) == NULL)
		printf("can't file open!: %s\n", fw_name);

	count = readFile(f_read, arr);
	printf("%d\n", count);

	radixSort(arr, count);

	for(i=0; i<count; i++) fprintf(f_write, "%s\n", arr[i].str);

	fclose(f_read);
	fclose(f_write);
	return 0;
}

////  파일관련 함수 ////
int readFile(FILE * f_r, String arr[])
{
	int i=0;

	for(i=0; i<10; i++)	{
		fscanf(f_r, "%s\n", arr[i].str);	//인자로 전달받은 해당 파일 읽기
		if(*arr[i].str == '*') {
			*arr[i].str = '\0';
			return i;
		}
	}
	return i;
}

//// 큐 관련 함수  ////
QueueType * createQueue()
{
	QueueType * Q;
	Q = (QueueType *)malloc(sizeof(QueueType));
	Q->front = -1;
	Q->rear = -1;

	return Q;
}

int isEmpty(QueueType * Q)
{
	if(Q->front == Q->rear) return 1;	//큐가 공백일경우 1
	else return 0;						//공백이 아닐경우 0을 반환
}

int isFull(QueueType * Q)
{
	if(Q->rear == qLen-1) 
		return 1;	//큐가 가득찰경우 1
	else 
		return 0;						//아닐경우 0을 반환
}

void enQueue(QueueType * Q, String item)
{
	if(isFull(Q)) exit(1);
	else 
	{
		Q->rear++;
		Q->Queue[Q->rear] = item;
	}
}

String deQueue(QueueType * Q)
{
	if(isEmpty(Q)) exit(1);
	else
	{
		Q->front++;
		return Q->Queue[Q->front];
	}
}

//// 기수정렬 함수 ////
int clearBucket(QueueType *Q[], String *arr, int addr)
	//1.버켓, 2.문자열이 저장될 배열, 3.저장할 위치
	//현재 addr 값을 리턴
{
	int bucket;

	for(bucket=0; bucket<radix; bucket++) {
		while(!isEmpty(Q[bucket]))
			arr[addr++] = deQueue(Q[bucket]); 
	}
	for(bucket=0; bucket<radix; bucket++) {
		Q[bucket]->front = -1;
		Q[bucket]->rear = -1;
	}
	return addr;
}

void radixSort_n(QueueType *Q[], String *arr, int count, int index)
{
	char tempCh[10];
	int i;
	int start_addr=0, addr=0, temp_addr=0;
	
	if (index+1==strLen-1)
		return;
		
	for(i=0; i<count; i++)
		tempCh[i] = arr[i].str[0];

	for(addr=0; addr<count-1; addr++) {
		if ((arr[addr].str[index]==arr[addr+1].str[index]) && (arr[addr].str[index]==arr[addr-1].str[index])) {
			//위도 같고 아래도 같은경우
			if(tempCh[addr] == arr[addr+1].str[0] && tempCh[addr] == arr[addr-1].str[0])
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			//addr++;
		}
		else if ((arr[addr].str[index]!=arr[addr+1].str[index]) && (arr[addr].str[index]==arr[addr-1].str[index])) {
			//위는 같고 아래는 다른경우
			if(tempCh[addr] != arr[addr+1].str[0] && tempCh[addr] == arr[addr-1].str[0]) {
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
				addr = clearBucket(Q, arr, start_addr);
			}
			//addr++;
		}
		else if ((arr[addr].str[index]==arr[addr+1].str[index]) && (arr[addr].str[index]!=arr[addr-1].str[index])) {
			//위는 다르고 아래는 같은경우
			if(tempCh[addr] == arr[addr+1].str[0] && tempCh[addr] != arr[addr-1].str[0]) {
				start_addr = addr;
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			}
			//addr++;
		}
		else {
			//둘다 다른경우
			temp_addr = clearBucket(Q, arr, start_addr);
			//addr++;
		}
	}
	radixSort_n(Q, arr, count, ++index);
	return;
}
void radixSort(String *arr, int count)
{
	int bucket, addr;
	int index = 0;

	QueueType * Q[radix];	// bucket로 사용될 배열Q
	
	for(bucket=0; bucket<radix; bucket++)	//0~26의 bucket 생성
		Q[bucket] = createQueue();

	//// 첫번째 index==0 정렬 ////
	for(addr=0; addr<count; addr++)
		enQueue(Q[(arr[addr].str[index]-97)], arr[addr]);

	addr = clearBucket(Q, arr, 0);
	addr = 0;
	//---------------------------------//
	//radixSort_n(Q, arr, count, index);

	for(addr=0; addr<count; addr++)
		printf("%s\n", arr[addr].str);
}

