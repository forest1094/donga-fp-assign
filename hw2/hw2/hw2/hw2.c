/*
	����ó�� ����2
	��ü���� �� ����
	�ۼ���	: 13.11.11
	��������	: 13.11.12
	�ۼ���	: ������
*/
#include <stdio.h>
#define BUFSIZE 5
#define DATASIZE 100

/***  ����ü ����  ***/
typedef struct {
	int key;
	int state;
	// state -> normal:0, frozen:1, pop:-1
} bufferNode;

/***  �Լ� ����  ***/
void breakFrozen(bufferNode Buf[]);
	// frozen������ ���۸� normal�� ��ȯ

int checkFrozen(bufferNode Buf[]);
	// ���ڷ� ���� ������ ������ ���� üũ
	// ������ �ȵ� ������ �ε����� ���� ������ ����, ���� �������̸� -1����

int popLeastKey(bufferNode Buf[]);
	// ���ۿ��� frozen�� �ƴ� �������� ���۸� pop
	// pop�� ���� state�� -1��

int insertKey(bufferNode Buf[], int data);
	// pop�� ���ۿ� data ����
	// data�� pop�� ���ۺ��� ������� �ش� ���� frozen

void replaceSel(FILE* fw, int data[], int len);
	// ��ü����(�ۼ��� ����, ������ ������, ������ ����)
	// �ۼ��� ���� fw�� �ٷ� ���

/***  ���� �Լ�  ***/
int main(void)
{
	FILE *fRead, *fWrite;
	int length=0;
	char charTemp = '\0';
	int data[DATASIZE];

	//// ���� ����
	if((fRead = fopen("hw2.inp", "r")) == NULL)
		return 1;
	if((fWrite = fopen("hw2.out", "w")) == NULL)
		return 1;

	//// ���� �о����
	while(1)
	{
		fscanf(fRead, "%d%c", &data[length++], &charTemp);
		if(charTemp == '\n')
			break;
	}

	//// �Է����� üũ
	if(length < BUFSIZE)
	{
		printf("�Է� ������ ���̰� ������ ũ�⺸�� �۽��ϴ�\n");
		return 1;
	}
	
	//// ��ü����
	replaceSel(fWrite, data, length);
	
	//// ���� �ݱ�
	fclose(fRead);
	fclose(fWrite);
	return 0;
}

/***  �Լ� ����  ***/
void breakFrozen(bufferNode Buf[])
{
	int i;
	for(i=0; i<BUFSIZE; i++)
	{
		if(Buf[i].state == 1)
			Buf[i].state = 0;
	}
}

int checkFrozen(bufferNode Buf[])
{
	int i;
	int	check=0;
	
	for(i=0; i<BUFSIZE; i++)	//normal ������ ���۰� ������
	{
		if(Buf[i].state == 0)	//normal������ ���� ���� �ε����� ��ȯ
			return i;	
		check+=Buf[i].state;
	}

	if(check == -BUFSIZE)
		return -2;	// ���۰� ���� ��������� -2
	else
		return -1;  // ���۰� ���� �������̸� -1
}

int popLeastKey(bufferNode Buf[])
{
	int i, tempKey, tempIndex;	//tempKey = ���ۿ��� ���� ������, tempIndex = �������� ���� �ε���
	i = checkFrozen(Buf);

	tempIndex = i;
	tempKey = Buf[i].key;

	for( ; i<BUFSIZE; i++)	//���ۿ��� ���� ������ Ž��
	{
		if((tempKey > Buf[i].key) && Buf[i].state == 0) 
		{
			tempKey = Buf[i].key;
			tempIndex = i;
		}
	}
	Buf[tempIndex].state = -1;		//���ۿ��� �������� ���� pop�Ѵ�
	return tempKey;					
}

int insertKey(bufferNode Buf[], int data)
{
	int i;

	for(i=0; i<BUFSIZE; i++) 
	{
		if(Buf[i].state == -1)			// pop�� �ڸ��� �����͸� ã�Ƽ�
		{	
			if(Buf[i].key > data)		// pop�� �����ͺ��� ������ ���Ե����Ͱ� ������ frozen
				Buf[i].state = 1;
			else if(Buf[i].key <= data)	// ���ų� ũ�� normal
				Buf[i].state = 0;
			
			Buf[i].key = data;
		}
	}
	return 0;
}
void replaceSel(FILE* fw, int data[], int len)
{
	int index, keyVal;
	int runNum = 1;
	bufferNode BUFFER[BUFSIZE]={'\0',};

	// ���ۿ��� BUFSIZE��ŭ ���ڵ� �ǵ�
	for(index=0; index<BUFSIZE; index++) 
	{
		BUFFER[index].key = data[index];
		BUFFER[index].state = 0;
	}

	// ���� ����
	fprintf(fw, "Run%d: ", runNum);
	while(1)
	{
		if(checkFrozen(BUFFER) == -2) {			// ���۰� ������ ����ִ°��
			fprintf(fw, "\n*");					// �����ٿ� *����� ����
			break;
		}
		else if(checkFrozen(BUFFER) == -1) {	// ���۰� ������ ������� ���
			breakFrozen(BUFFER);				// ������ ������ ���� �� ����
			fprintf(fw, "\n");
			fprintf(fw, "Run%d: ", ++runNum);
		}
		keyVal = popLeastKey(BUFFER);
		fprintf(fw, "%d ", keyVal);

		if(index<len)	// ���������Ͱ� ������ ������ �߰� ����
			insertKey(BUFFER, data[index++]);
	}
}