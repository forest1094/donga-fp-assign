/*
	파일처리 과제2
	대체선택 런 생성
	작성일	: 13.11.11
	최종수정	: 13.11.12
	작성인	: 이정보
*/
#include <stdio.h>
#define BUFSIZE 5
#define DATASIZE 100

/***  구조체 선언  ***/
typedef struct {
	int key;
	int state;
	// state -> normal:0, frozen:1, pop:-1
} bufferNode;

/***  함수 선언  ***/
void breakFrozen(bufferNode Buf[]);
	// frozen상태인 버퍼를 normal로 변환

int checkFrozen(bufferNode Buf[]);
	// 인자로 받은 버퍼의 프로즌 여부 체크
	// 프로즌 안된 버퍼의 인덱스중 가장 낮은값 리턴, 전부 프로즌이면 -1리턴

int popLeastKey(bufferNode Buf[]);
	// 버퍼에서 frozen이 아닌 가장작은 버퍼를 pop
	// pop된 버퍼 state를 -1로

int insertKey(bufferNode Buf[], int data);
	// pop된 버퍼에 data 삽입
	// data가 pop된 버퍼보다 작을경우 해당 버퍼 frozen

void replaceSel(FILE* fw, int data[], int len);
	// 대체선택(작성할 파일, 정렬할 데이터, 데이터 길이)
	// 작성한 런은 fw에 바로 출력

/***  메인 함수  ***/
int main(void)
{
	FILE *fRead, *fWrite;
	int length=0;
	char charTemp = '\0';
	int data[DATASIZE];

	//// 파일 열기
	if((fRead = fopen("hw2.inp", "r")) == NULL)
		return 1;
	if((fWrite = fopen("hw2.out", "w")) == NULL)
		return 1;

	//// 파일 읽어오기
	while(1)
	{
		fscanf(fRead, "%d%c", &data[length++], &charTemp);
		if(charTemp == '\n')
			break;
	}

	//// 입력파일 체크
	if(length < BUFSIZE)
	{
		printf("입력 파일의 길이가 버퍼의 크기보다 작습니다\n");
		return 1;
	}
	
	//// 대체선택
	replaceSel(fWrite, data, length);
	
	//// 파일 닫기
	fclose(fRead);
	fclose(fWrite);
	return 0;
}

/***  함수 구현  ***/
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
	
	for(i=0; i<BUFSIZE; i++)	//normal 상태인 버퍼가 있을때
	{
		if(Buf[i].state == 0)	//normal버퍼중 가장 작은 인덱스값 반환
			return i;	
		check+=Buf[i].state;
	}

	if(check == -BUFSIZE)
		return -2;	// 버퍼가 전부 비어있으면 -2
	else
		return -1;  // 버퍼가 전부 프로즌이면 -1
}

int popLeastKey(bufferNode Buf[])
{
	int i, tempKey, tempIndex;	//tempKey = 버퍼에서 가장 작은값, tempIndex = 가장작은 값의 인덱스
	i = checkFrozen(Buf);

	tempIndex = i;
	tempKey = Buf[i].key;

	for( ; i<BUFSIZE; i++)	//버퍼에서 가장 작은값 탐색
	{
		if((tempKey > Buf[i].key) && Buf[i].state == 0) 
		{
			tempKey = Buf[i].key;
			tempIndex = i;
		}
	}
	Buf[tempIndex].state = -1;		//버퍼에서 가장작은 값을 pop한다
	return tempKey;					
}

int insertKey(bufferNode Buf[], int data)
{
	int i;

	for(i=0; i<BUFSIZE; i++) 
	{
		if(Buf[i].state == -1)			// pop된 자리의 데이터를 찾아서
		{	
			if(Buf[i].key > data)		// pop된 데이터보다 들어오는 삽입데이터가 작으면 frozen
				Buf[i].state = 1;
			else if(Buf[i].key <= data)	// 같거나 크면 normal
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

	// 버퍼에서 BUFSIZE만큼 레코드 판독
	for(index=0; index<BUFSIZE; index++) 
	{
		BUFFER[index].key = data[index];
		BUFFER[index].state = 0;
	}

	// 정렬 시작
	fprintf(fw, "Run%d: ", runNum);
	while(1)
	{
		if(checkFrozen(BUFFER) == -2) {			// 버퍼가 완전히 비어있는경우
			fprintf(fw, "\n*");					// 다음줄에 *출력후 종료
			break;
		}
		else if(checkFrozen(BUFFER) == -1) {	// 버퍼가 완전히 프로즌된 경우
			breakFrozen(BUFFER);				// 프로즌 해제후 다음 런 생성
			fprintf(fw, "\n");
			fprintf(fw, "Run%d: ", ++runNum);
		}
		keyVal = popLeastKey(BUFFER);
		fprintf(fw, "%d ", keyVal);

		if(index<len)	// 남은데이터가 있으면 데이터 추가 삽입
			insertKey(BUFFER, data[index++]);
	}
}