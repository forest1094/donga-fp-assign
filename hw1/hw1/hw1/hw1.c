#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strLen 4
#define qLen 10
#define radix 26

//////  구조체 선언  //////
typedef struct {
	char str[strLen];
}String;

typedef struct {
	String Queue[qLen];
	int front, rear;
}QueueType;

//////  파일관련 함수  //////
int readFile(FILE * f_r, String arr[]);
	// 파일 전체를 읽어서 Element형 배열에 저장하는 함수
	// 정렬해야 되는 레코드 갯수 반환
	// 1.읽어올 파일포인터 2.읽은 파일내용을 저장할 배열

//////  큐 관련 함수  //////
QueueType * createQueue();
int isEmpty(QueueType * Q);
int isFull(QueueType * Q);
void enQueue(QueueType * Q, String item);
String deQueue(QueueType * Q);

//////  기수정렬 관련 함수  //////
int clearBucket(QueueType *Q[], String *arr, int addr);
	// 버켓 전체를 deQ해서 arr에 저장
	// 다음 연산이 시작되야 하는 addr 반환
	// 1.전체버켓, 2.deQ한걸 저장할 문자열배열, 3.deQ해야 하는 arr[]의 위치
int radixSort_n(QueueType *Q[], String *arr, int count, int index);
	// 2열부터 radix정렬
	// 정렬 하면서 시행된 연산횟수 반환
	// 1.전체버켓, 2.정렬한 문자열배열, 3.정렬할 문자열갯수, 4.정렬할 문자열 열번호
int radixSort(QueueType *Q[], String *arr, int count);
	// 1열정렬 및 radixSort_n 호출해서 2열부터 정렬
	// radixSort_n 포함 정렬 하면서 시행된 연산횟수 반환
	// 1.전체버켓, 2.정렬한 문자열배열, 3.정렬할 문자열갯수

//////  main함수  //////
int main(void)
{
	//// 변수 선언 ////
	FILE *f_read, *f_write;
	
	int i;
	int count, num;					// count: 정렬할 문자열의 갯수, num: 기수정렬의 연산횟수
	QueueType * Q[radix];			// bucket로 사용될 배열Q
	String arr[qLen]={'\0'};		// 정렬할 문자열이 들어가는 구조체 배열 arr	
	char fr_name[] = "hw1.inp"; 
	char fw_name[] = "hw1.out";
	
	//// 파일 열기 ////
	if((f_read = fopen(fr_name, "r")) == NULL) {	// 파일 여는중 오류 발생시 메세지 출력
		printf("can't file open!: %s\n", fr_name);
		return 1;
	}
	if((f_write = fopen(fw_name, "w")) == NULL) {
		printf("can't file open!: %s\n", fw_name);
		return 1;
	}
	count = readFile(f_read, arr);
	
	//// 버켓 생성 ////
	for(i=0; i<radix; i++)	// 0~26의 버켓 Q[i]생성
		Q[i] = createQueue();
	
	//// 정렬 시작 ////
	num = radixSort(Q, arr, count);
	
	//// 파일 출력 ////
	for(i=0; i<count; i++) fprintf(f_write, "%s\n", arr[i].str);
	fprintf(f_write, "%d\n", num);
	fprintf(f_write, "%c", '*');

	//// 파일 종료 ////
	fclose(f_read);
	fclose(f_write);

	return 0;
}

//////  파일관련 함수 //////
int readFile(FILE * f_r, String arr[])
{
	int i=0;

	for(i=0; i<10; i++)	{
		fscanf(f_r, "%s\n", arr[i].str);	// 인자로 전달받은 해당 파일 읽기
		if(*arr[i].str == '*') {			// '*'읽으면 파일읽기 종료
			*arr[i].str = '\0';
			return i;
		}
	}
	return i;
}

////// 큐 관련 함수  //////
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
		return 1;						//큐가 가득찰경우 1
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

////// 기수정렬 관련 함수 //////
int clearBucket(QueueType *Q[], String *arr, int addr)
{
	int bucket;

	for(bucket=0; bucket<radix; bucket++) {		// arr[addr]에 deQ
		while(!isEmpty(Q[bucket]))
			arr[addr++] = deQueue(Q[bucket]); 
	}
	for(bucket=0; bucket<radix; bucket++) {		// Q초기화
		Q[bucket]->front = -1;
		Q[bucket]->rear = -1;
	}
	return addr-1;
}
int radixSort_n(QueueType *Q[], String *arr, int count, int index)
{
	String arrTemp[10];
	
	int before, after;			// before: arr[addr]과 그 위쪽 문자열비교, after: arr[addr]과 그 아래쪽 문자열비교 
	int num=0;					// num: 기수정렬 진행 중 enQ, deQ되는 횟수
	int start_addr=0, addr=0;	// start_addr: enQ가 시작되는 지점, addr: 현재 지점
	int size = index+1;			// size: 비교해야되는 문자열 길이

	if (size==strLen-1)
		return num;
	
	for(addr=0; addr<count; addr++)		// arr[i]에서 size만큼 arrTemp[i]에 복사
		strncpy(arrTemp[addr].str, arr[addr].str, size);
	
	for(addr=0; addr<count; addr++)
	{
		before=strncmp(arrTemp[addr].str, arrTemp[addr-1].str, size);
		after=strncmp(arrTemp[addr].str, arrTemp[addr+1].str, size);

		if(before==0 && after==0){
			// 위도 같고 아래도 같을때
			// addr번째 arr에 있는 str[index+1]을 인큐
			enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
		}
		else if(before==0 && after!=0){
			// 위는 같고 아래는 다를때
			enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			addr = clearBucket(Q, arr, start_addr);
			num++;
		}
		else if(before!=0 && after==0){
			// 위는 다르고 아래는 같을때
			start_addr = addr;
			enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
		}
	}

	num += radixSort_n(Q, arr, count, ++index);
	return num;
}
int radixSort(QueueType *Q[], String *arr, int count)
{
	int addr;
	int index = 0;
	int num=1;
	
	//// 첫번째 index==0 정렬 ////
	for(addr=0; addr<count; addr++)
		enQueue(Q[(arr[addr].str[index]-97)], arr[addr]);

	addr = clearBucket(Q, arr, 0);
	num += radixSort_n(Q, arr, count, index);
	return num;
}