/*********************************************************
-  ʱ��    ��11.19
-  �ļ���  ��һԪ����ʽ��ƽ��������洢
-  ����    ��������
*  *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define LH +1   //���
#define EH 0    //�ȸ�
#define RH -1   //�Ҹ�
#define MAXEP 10 //��ߴ���
#define T
typedef struct Data {
	int E;
	double C;
}Data;

//���������Ͷ���
struct tree {
	Data data;            //������
	int bf;               //ƽ������
	struct tree* Lchild;    //����
	struct tree* Rchild;   //�Һ���
};

typedef struct tree Node;
typedef Node* Poly;

static void R_Rotate(Poly* ptr);//����ת

static void L_Rotate(Poly* ptr);//����ת

static void LchildBalance(Poly* ploy);//��ƽ��

static void RchildBalance(Poly* ploy);//��ƽ��
static int InsertAVL(Poly* ploy, Data e, bool* taller);//����
static FILE* OpenFile(const char* filename);//���ļ�
static void CloseFile(FILE* pfile);//�ر��ļ�
static void Inorder(Poly ploy);//ƽ����������������

static void InorderPoly(Poly Poly);//��������ʽ

static void CreatePoly(Poly* Poly, int n);//��������ʽ

static void AddPoly(Poly Poly1, Poly Poly2);//����ʽ�����

static double EvaPoly(Poly Poly, double x);//����ʽ��ֵ

static void DerPloy(Poly Poly, int n);//����ʽ��

static void Order(Poly Poly1, Data d, Poly* resPoly) {
	if (Poly1 != NULL) {
		Data s = { 0 };
		s.C = Poly1->data.C * d.C;
		s.E = Poly1->data.E + d.E;
		bool b = 0;
		InsertAVL(resPoly, s, &b);
		Order(Poly1->Lchild, d, resPoly);
		Order(Poly1->Rchild, d, resPoly);
	}
}

static void MultPoly(Poly Poly1, Poly Poly2, Poly* resPoly) {
	if (Poly2 != NULL) {
		Data d = { 0 };
		d.C = Poly2->data.C;
		d.E = Poly2->data.E;
		Order(Poly1, d, resPoly);
		MultPoly(Poly1, Poly2->Lchild, resPoly);
		MultPoly(Poly1, Poly2->Rchild, resPoly);
	}
}
static void test() {
	Data value;
	Poly Poly1 = NULL, Poly2 = NULL;
	int n = 0;
	printf("���������ʽ������:");
	scanf_s("%d", &n);
	FILE* Write = OpenFile("C:\\Users\\a\\Documents\\c.txt");
	fprintf_s(Write, "\n");
	CloseFile(Write);
	CreatePoly(&Poly1, n);
	//InorderPoly(Poly1);
	CreatePoly(&Poly2, n);
	//InorderPoly(Poly2);
#ifdef T
	double start = clock();
#endif // !T  
	AddPoly(Poly1, Poly2);
#ifdef T
	double finish = clock();
	double duration = (finish - start) / CLOCKS_PER_SEC;
	printf("����ʽ�ӷ������ʱ��Ϊ: %.3f\n", duration);
#endif // !T
	printf("������Ϊ��\n");
	//InorderPoly(Poly1);
#ifdef T
	start = clock();
#endif // !T  
	printf("�������󵼽�����");
	scanf_s("%d", &n);
	DerPloy(Poly1, n);
#ifdef T
	finish = clock();
	duration = (finish - start) / CLOCKS_PER_SEC;
	printf("����ʽ��%d�׵������ʱ��Ϊ: %.3f\n", n, duration);
#endif // !T
	printf("������Ϊ��\n");
	//InorderPoly(Poly1);
	printf("����������ֵ��");
	scanf_s("%d", &n);
#ifdef T
	start = clock();
#endif // !T  
	printf("ֵ = %.3f\n", EvaPoly(Poly1, n));
#ifdef T
	finish = clock();
	duration = (finish - start) / CLOCKS_PER_SEC;
	printf("����ʽ��ֵ�����ʱ��Ϊ: %.3f\n", duration);
#endif // !T
}
static void test2() {
	Data value;
	Poly Poly1 = NULL, Poly2 = NULL;
	int n = 0;
	printf("���������ʽ������:");
	scanf_s("%d", &n);
	CreatePoly(&Poly1, n);
	InorderPoly(Poly1);
	CreatePoly(&Poly2, n);
	InorderPoly(Poly2);
	Poly Poly3 = NULL;
	MultPoly(Poly1, Poly2, &Poly3);
	InorderPoly(Poly3);
}
int main() {
	srand((unsigned int)time(NULL));
	test2();
	return 0;
}

static void R_Rotate(Poly* ptr) {
	Poly lc = (*ptr)->Lchild;   //lcָ���*ptr�����ӵĸ����
	(*ptr)->Lchild = lc->Rchild;  //lc���������ҽ�Ϊ*ptr��������
	lc->Rchild = *ptr;
	*ptr = lc;                 //ptrָ���µĽ��"
}
static void L_Rotate(Poly* ptr) {
	Poly rc = (*ptr)->Rchild;   //rcָ���*ptr���ɺ��ӵĸ����
	(*ptr)->Rchild = rc->Lchild;   //rc���������ҽ�Ϊ*ptr��������
	rc->Lchild = *ptr;
	*ptr = rc;                  //ptrָ���µĽ��
}
static void LchildBalance(Poly* ploy)
{
	Poly lc;
	Poly rd;
	lc = (*ploy)->Lchild;
	switch (lc->bf)
	{
	case LH: {
		(*ploy)->bf = lc->bf = EH;
		R_Rotate(ploy);
		break;
	}
	case RH: {
		rd = lc->Rchild;
		switch (rd->bf)
		{
		case LH: {
			(*ploy)->bf = RH;
			lc->bf = EH;
			break;
		}
		case EH: {
			(*ploy)->bf = lc->bf = EH;
			break;
		}
		case RH: {
			(*ploy)->bf = EH;
			lc->bf = LH;
			break;
		}
		}
		rd->bf = EH;
		L_Rotate(&(*ploy)->Lchild);
		R_Rotate(ploy);
		break;
	}
	}
}
static void RchildBalance(Poly* ploy) {
	Poly lc;
	Poly rd;
	lc = (*ploy)->Rchild;
	switch (lc->bf)
	{
	case RH: {
		(*ploy)->bf = lc->bf = EH;
		L_Rotate(ploy);
		break;
	}
	case LH: {
		rd = lc->Lchild;
		switch (rd->bf)
		{
		case LH: {
			(*ploy)->bf = EH;
			lc->bf = RH;
			break;
		}
		case EH: {
			(*ploy)->bf = lc->bf = EH;
			break;
		}
		case RH: {
			(*ploy)->bf = LH;
			lc->bf = EH;
			break;
		}
		}
		rd->bf = EH;
		R_Rotate(&(*ploy)->Rchild);
		L_Rotate(ploy);
		break;
	}
	}
}
static int InsertAVL(Poly* ploy, Data e, bool* taller) {
	if ((*ploy) == NULL) {
		//����Ϊһ�ÿ���������һ���½ڵ���Ϊ���ڵ�
		(*ploy) = (Poly)malloc(sizeof(Node));
		(*ploy)->bf = EH;
		(*ploy)->data.C = e.C;
		(*ploy)->data.E = e.E;
		(*ploy)->Lchild = NULL;
		(*ploy)->Rchild = NULL;
		*taller = true;
	}
	else if (e.E == (*ploy)->data.E) {
		(*ploy)->data.C += e.C;
		*taller = false;
		return 0;
	}
	else if (e.E < (*ploy)->data.E) {
		//Ӧ�ü�����*ploy����������������
		if (!InsertAVL(&(*ploy)->Lchild, e, taller)) {
			//δ����
			return 0;
		}
		//�Ѳ��뵽*ploy���������в�������������
		if (*taller) {
			//���*ploy��ƽ���
			switch ((*ploy)->bf)
			{
				//ԭ������������������
			case LH: {
				//ƽ������Ϊ-1
				//����
				LchildBalance(ploy);
				*taller = false;
				break;
			}
				   //ԭ��������һ���ߣ�������Ϊ����������������
			case EH: {
				//ƽ������Ϊ0
				(*ploy)->bf = LH;
				*taller = true;
				break;
			}
				   //ԭ�����������������ߣ����ڵȸ�
			case RH: {
				//ƽ������Ϊ1
				(*ploy)->bf = EH;
				*taller = false;
				break;
			}
			}
		}
	}
	else {
		//Ӧ������*ploy���������н�������    
		if (!InsertAVL(&(*ploy)->Rchild, e, taller)) {
			//δ����
			return 0;
		}
		//�Ѳ��뵽*ploy��������������������
		if (*taller) {
			//���*ploy��ƽ���
			switch ((*ploy)->bf)
			{
			case LH: {
				//ԭ�����������������ߣ��������
				(*ploy)->bf = EH;
				*taller = false;
				break;
			}
			case EH: {
				//ԭ�����������Ǹߣ�������Ϊ����������������
				(*ploy)->bf = RH;
				*taller = true;
				break;
			}
			case RH: {
				//ԭ�����������������ߣ���Ҫ������ƽ�⴦�� 
				RchildBalance(ploy);
				*taller = false;
				break;
			}
			}
		}
	}
	return 1;
}
static FILE* OpenFile(const char* filename) {
	FILE* Write;
	errno_t err = fopen_s(&Write, filename, "a+");
	if (err != 0) {
		perror("error");
		exit(0);
	}
	return Write;
}
static void CloseFile(FILE* pfile) {
	fclose(pfile);
}
bool flag = 0, iszero = 0;
static void Inorder(Poly ploy)
{
	if (ploy->Lchild) {
		Inorder(ploy->Lchild);
	}
	//ϵ��Ϊ0
	if (0 == ploy->data.C) {
		iszero++;
		return;
	}
	if (0 == ploy->data.E) {
		//FILE* Write = OpenFile("C:\\Users\\a\\Documents\\c.txt");
		iszero = 0;
		if (0 == flag) {
			if (ploy->data.C > 0) {
				printf("%.2lf+", ploy->data.C);
			}
			else {
				printf("%.2lf+", ploy->data.C);
			}
			flag = 1;
		}
		else {
			if (ploy->data.C > 0) {
				printf("\b+%.2lf+", ploy->data.C);
			}
			else {
				printf("\b%.2lf+", ploy->data.C);
			}
		}
		//CloseFile(Write);
	}
	else {
		iszero = 0;
		if (0 == flag) {
			if (ploy->data.C > 0) {
				if (1 == ploy->data.E) {
					printf("%.2lfx+", ploy->data.C);
				}
				else {
					printf("%.2lfx^%d+", ploy->data.C, ploy->data.E);
				}
			}
			else {
				if (1 == ploy->data.E) {
					printf("%.2lfx+", ploy->data.C);
				}
				else {
					printf("%.2lfx^%d+", ploy->data.C, ploy->data.E);
				}
			}
			flag = 1;
		}
		else {
			if (ploy->data.C > 0) {
				if (1 == ploy->data.E) {
					printf("\b+%.2lfx+", ploy->data.C);
				}
				else {
					printf("\b+%.2lfx^%d+", ploy->data.C, ploy->data.E);
				}
			}
			else {
				if (1 == ploy->data.E) {
					printf("\b%.2lfx+", ploy->data.C);
				}
				else {
					printf("\b%.2lfx^%d+", ploy->data.C, ploy->data.E);
				}
			}
		}
	}
	if (ploy->Rchild)
	{
		Inorder(ploy->Rchild);
	}
}
static void InorderPoly(Poly Poly) {
	flag = 0;
	Inorder(Poly);
	if (iszero) {
		printf("0\n");
	}
	else {
		printf("\b \n");
	}
}
static void CreatePoly(Poly* Poly, int n) {

	printf("��ǰ����ʽΪ��\n");
	Data* p = (Data*)malloc(sizeof(Data) * n);
	bool taller;
	for (int i = 0; i < n; i++)
	{
		//��������
		int b = rand() % 2;
		if (0 == b) {
			b = -1;
		}
		double C = b * rand() % MAXEP;
		int E = rand() % MAXEP + 1;
		p[i].C = C;
		p[i].E = E;
	}
	FILE* Write = OpenFile("C:\\Users\\a\\Documents\\c.txt");
	fprintf_s(Write, "ϵ��\tָ��\n");
#ifdef T
	double start = clock();
#endif // !T  
	for (int i = 0; i < n; i++)
	{
		//��������
		InsertAVL(Poly, p[i], &taller);
		fprintf_s(Write, "%.2f\t %d\n", p[i].C, p[i].E);
	}
#ifdef T
	double finish = clock();
	double duration = (finish - start) / CLOCKS_PER_SEC;
	printf("��������ʽ��ʱ��Ϊ: %.3f\n", duration);
#endif // !T
	CloseFile(Write);
}
static void AddPoly(Poly Poly1, Poly Poly2) {
	if (Poly2->Lchild) {
		AddPoly(Poly1, Poly2->Lchild);
	}
	if (0 == Poly2->data.E) {
		bool t;
		InsertAVL(&Poly1, Poly2->data, &t);
	}
	else {
		bool t;
		InsertAVL(&Poly1, Poly2->data, &t);
	}
	if (Poly2->Rchild) {
		AddPoly(Poly1, Poly2->Rchild);
	}
}
static double EvaPoly(Poly Poly, double x) {
	if (Poly->Lchild && Poly->Rchild) {
		return Poly->data.C * pow(x, Poly->data.E) + EvaPoly(Poly->Lchild, x) + EvaPoly(Poly->Rchild, x);
	}
	else if (Poly->Lchild && NULL == Poly->Rchild) {
		return Poly->data.C * pow(x, Poly->data.E) + EvaPoly(Poly->Lchild, x);
	}
	else if (NULL == Poly->Lchild && Poly->Rchild) {
		return Poly->data.C * pow(x, Poly->data.E) + EvaPoly(Poly->Rchild, x);
	}
	else {
		return Poly->data.C * pow(x, Poly->data.E);
	}
}
static void DerPloy(Poly Poly, int n) {
	if (0 == n) {
		return;
	}
	if (Poly->Lchild) {
		DerPloy(Poly->Lchild, n);
	}
	if (n > Poly->data.E) {
		Poly->data.E = 0;
		Poly->data.C = 0;
	}
	else if (n == Poly->data.E) {
		Poly->data.C *= Poly->data.E;
		Poly->data.E = 0;
	}
	else {
		int i = n;
		while (i--) {
			Poly->data.C *= Poly->data.E;
			Poly->data.E -= 1;
		}
	}
	if (Poly->Rchild) {
		DerPloy(Poly->Rchild, n);
	}
}
