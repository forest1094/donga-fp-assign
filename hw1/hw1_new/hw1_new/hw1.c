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

////  ���ϰ��� �Լ�   ////
int readFile(FILE * f_r, String arr[]);
	//���� ��ü�� �о Element�� �迭�� �����ϴ� �Լ�
	//1.�о�� ���������� 2.���� ���ϳ����� ������ �迭

////  ť ���� �Լ�  ////
QueueType * createQueue();
int isEmpty(QueueType * Q);
int isFull(QueueType * Q);
void enQueue(QueueType * Q, String item);
String deQueue(QueueType * Q);

////  ������� ���� �Լ� ////
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
	
	/** ���� ���� **/
	if((f_read = fopen(fr_name, "r")) == NULL)	//���� ������ ���� �߻��� �޼��� ���
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

////  ���ϰ��� �Լ� ////
int readFile(FILE * f_r, String arr[])
{
	int i=0;

	for(i=0; i<10; i++)	{
		fscanf(f_r, "%s\n", arr[i].str);	//���ڷ� ���޹��� �ش� ���� �б�
		if(*arr[i].str == '*') {
			*arr[i].str = '\0';
			return i;
		}
	}
	return i;
}

//// ť ���� �Լ�  ////
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
	if(Q->front == Q->rear) return 1;	//ť�� �����ϰ�� 1
	else return 0;						//������ �ƴҰ�� 0�� ��ȯ
}

int isFull(QueueType * Q)
{
	if(Q->rear == qLen-1) 
		return 1;	//ť�� ��������� 1
	else 
		return 0;						//�ƴҰ�� 0�� ��ȯ
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

//// ������� �Լ� ////
int clearBucket(QueueType *Q[], String *arr, int addr)
	//1.����, 2.���ڿ��� ����� �迭, 3.������ ��ġ
	//���� addr ���� ����
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
			//���� ���� �Ʒ��� �������
			if(tempCh[addr] == arr[addr+1].str[0] && tempCh[addr] == arr[addr-1].str[0])
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			//addr++;
		}
		else if ((arr[addr].str[index]!=arr[addr+1].str[index]) && (arr[addr].str[index]==arr[addr-1].str[index])) {
			//���� ���� �Ʒ��� �ٸ����
			if(tempCh[addr] != arr[addr+1].str[0] && tempCh[addr] == arr[addr-1].str[0]) {
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
				addr = clearBucket(Q, arr, start_addr);
			}
			//addr++;
		}
		else if ((arr[addr].str[index]==arr[addr+1].str[index]) && (arr[addr].str[index]!=arr[addr-1].str[index])) {
			//���� �ٸ��� �Ʒ��� �������
			if(tempCh[addr] == arr[addr+1].str[0] && tempCh[addr] != arr[addr-1].str[0]) {
				start_addr = addr;
				enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			}
			//addr++;
		}
		else {
			//�Ѵ� �ٸ����
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

	QueueType * Q[radix];	// bucket�� ���� �迭Q
	
	for(bucket=0; bucket<radix; bucket++)	//0~26�� bucket ����
		Q[bucket] = createQueue();

	//// ù��° index==0 ���� ////
	for(addr=0; addr<count; addr++)
		enQueue(Q[(arr[addr].str[index]-97)], arr[addr]);

	addr = clearBucket(Q, arr, 0);
	addr = 0;
	//---------------------------------//
	//radixSort_n(Q, arr, count, index);

	for(addr=0; addr<count; addr++)
		printf("%s\n", arr[addr].str);
}

