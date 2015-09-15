#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define strLen 4
#define qLen 10
#define radix 26

//////  ����ü ����  //////
typedef struct {
	char str[strLen];
}String;

typedef struct {
	String Queue[qLen];
	int front, rear;
}QueueType;

//////  ���ϰ��� �Լ�  //////
int readFile(FILE * f_r, String arr[]);
	// ���� ��ü�� �о Element�� �迭�� �����ϴ� �Լ�
	// �����ؾ� �Ǵ� ���ڵ� ���� ��ȯ
	// 1.�о�� ���������� 2.���� ���ϳ����� ������ �迭

//////  ť ���� �Լ�  //////
QueueType * createQueue();
int isEmpty(QueueType * Q);
int isFull(QueueType * Q);
void enQueue(QueueType * Q, String item);
String deQueue(QueueType * Q);

//////  ������� ���� �Լ�  //////
int clearBucket(QueueType *Q[], String *arr, int addr);
	// ���� ��ü�� deQ�ؼ� arr�� ����
	// ���� ������ ���۵Ǿ� �ϴ� addr ��ȯ
	// 1.��ü����, 2.deQ�Ѱ� ������ ���ڿ��迭, 3.deQ�ؾ� �ϴ� arr[]�� ��ġ
int radixSort_n(QueueType *Q[], String *arr, int count, int index);
	// 2������ radix����
	// ���� �ϸ鼭 ����� ����Ƚ�� ��ȯ
	// 1.��ü����, 2.������ ���ڿ��迭, 3.������ ���ڿ�����, 4.������ ���ڿ� ����ȣ
int radixSort(QueueType *Q[], String *arr, int count);
	// 1������ �� radixSort_n ȣ���ؼ� 2������ ����
	// radixSort_n ���� ���� �ϸ鼭 ����� ����Ƚ�� ��ȯ
	// 1.��ü����, 2.������ ���ڿ��迭, 3.������ ���ڿ�����

//////  main�Լ�  //////
int main(void)
{
	//// ���� ���� ////
	FILE *f_read, *f_write;
	
	int i;
	int count, num;					// count: ������ ���ڿ��� ����, num: ��������� ����Ƚ��
	QueueType * Q[radix];			// bucket�� ���� �迭Q
	String arr[qLen]={'\0'};		// ������ ���ڿ��� ���� ����ü �迭 arr	
	char fr_name[] = "hw1.inp"; 
	char fw_name[] = "hw1.out";
	
	//// ���� ���� ////
	if((f_read = fopen(fr_name, "r")) == NULL) {	// ���� ������ ���� �߻��� �޼��� ���
		printf("can't file open!: %s\n", fr_name);
		return 1;
	}
	if((f_write = fopen(fw_name, "w")) == NULL) {
		printf("can't file open!: %s\n", fw_name);
		return 1;
	}
	count = readFile(f_read, arr);
	
	//// ���� ���� ////
	for(i=0; i<radix; i++)	// 0~26�� ���� Q[i]����
		Q[i] = createQueue();
	
	//// ���� ���� ////
	num = radixSort(Q, arr, count);
	
	//// ���� ��� ////
	for(i=0; i<count; i++) fprintf(f_write, "%s\n", arr[i].str);
	fprintf(f_write, "%d\n", num);
	fprintf(f_write, "%c", '*');

	//// ���� ���� ////
	fclose(f_read);
	fclose(f_write);

	return 0;
}

//////  ���ϰ��� �Լ� //////
int readFile(FILE * f_r, String arr[])
{
	int i=0;

	for(i=0; i<10; i++)	{
		fscanf(f_r, "%s\n", arr[i].str);	// ���ڷ� ���޹��� �ش� ���� �б�
		if(*arr[i].str == '*') {			// '*'������ �����б� ����
			*arr[i].str = '\0';
			return i;
		}
	}
	return i;
}

////// ť ���� �Լ�  //////
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
		return 1;						//ť�� ��������� 1
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

////// ������� ���� �Լ� //////
int clearBucket(QueueType *Q[], String *arr, int addr)
{
	int bucket;

	for(bucket=0; bucket<radix; bucket++) {		// arr[addr]�� deQ
		while(!isEmpty(Q[bucket]))
			arr[addr++] = deQueue(Q[bucket]); 
	}
	for(bucket=0; bucket<radix; bucket++) {		// Q�ʱ�ȭ
		Q[bucket]->front = -1;
		Q[bucket]->rear = -1;
	}
	return addr-1;
}
int radixSort_n(QueueType *Q[], String *arr, int count, int index)
{
	String arrTemp[10];
	
	int before, after;			// before: arr[addr]�� �� ���� ���ڿ���, after: arr[addr]�� �� �Ʒ��� ���ڿ��� 
	int num=0;					// num: ������� ���� �� enQ, deQ�Ǵ� Ƚ��
	int start_addr=0, addr=0;	// start_addr: enQ�� ���۵Ǵ� ����, addr: ���� ����
	int size = index+1;			// size: ���ؾߵǴ� ���ڿ� ����

	if (size==strLen-1)
		return num;
	
	for(addr=0; addr<count; addr++)		// arr[i]���� size��ŭ arrTemp[i]�� ����
		strncpy(arrTemp[addr].str, arr[addr].str, size);
	
	for(addr=0; addr<count; addr++)
	{
		before=strncmp(arrTemp[addr].str, arrTemp[addr-1].str, size);
		after=strncmp(arrTemp[addr].str, arrTemp[addr+1].str, size);

		if(before==0 && after==0){
			// ���� ���� �Ʒ��� ������
			// addr��° arr�� �ִ� str[index+1]�� ��ť
			enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
		}
		else if(before==0 && after!=0){
			// ���� ���� �Ʒ��� �ٸ���
			enQueue(Q[(arr[addr].str[index+1]-97)], arr[addr]);
			addr = clearBucket(Q, arr, start_addr);
			num++;
		}
		else if(before!=0 && after==0){
			// ���� �ٸ��� �Ʒ��� ������
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
	
	//// ù��° index==0 ���� ////
	for(addr=0; addr<count; addr++)
		enQueue(Q[(arr[addr].str[index]-97)], arr[addr]);

	addr = clearBucket(Q, arr, 0);
	num += radixSort_n(Q, arr, count, index);
	return num;
}