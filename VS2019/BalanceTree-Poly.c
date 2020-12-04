/*********************************************************
-  时间    ：11.19
-  文件名  ：一元多项式的平衡二叉树存储
-  作者    ：马昭阳
*  *******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define LH +1   //左高
#define EH 0    //等高
#define RH -1   //右高
#define MAXEP 10 //最高次数
#define T
typedef struct Data {
	int E;
	double C;
}Data;

//二叉树类型定义
struct tree {
	Data data;            //数据域
	int bf;               //平衡因子
	struct tree* Lchild;    //左孩子
	struct tree* Rchild;   //右孩子
};

typedef struct tree Node;
typedef Node* Poly;

static void R_Rotate(Poly* ptr);//右旋转

static void L_Rotate(Poly* ptr);//左旋转

static void LchildBalance(Poly* ploy);//左平衡

static void RchildBalance(Poly* ploy);//右平衡
static int InsertAVL(Poly* ploy, Data e, bool* taller);//插入
static FILE* OpenFile(const char* filename);//打开文件
static void CloseFile(FILE* pfile);//关闭文件
static void Inorder(Poly ploy);//平衡二叉树的中序遍历

static void InorderPoly(Poly Poly);//遍历多项式

static void CreatePoly(Poly* Poly, int n);//创建多项式

static void AddPoly(Poly Poly1, Poly Poly2);//多项式的相加

static double EvaPoly(Poly Poly, double x);//多项式求值

static void DerPloy(Poly Poly, int n);//多项式求导

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
	printf("请输入多项式的项数:");
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
	printf("多项式加法运算的时间为: %.3f\n", duration);
#endif // !T
	printf("计算结果为：\n");
	//InorderPoly(Poly1);
#ifdef T
	start = clock();
#endif // !T  
	printf("请输入求导阶数：");
	scanf_s("%d", &n);
	DerPloy(Poly1, n);
#ifdef T
	finish = clock();
	duration = (finish - start) / CLOCKS_PER_SEC;
	printf("多项式求%d阶导运算的时间为: %.3f\n", n, duration);
#endif // !T
	printf("计算结果为：\n");
	//InorderPoly(Poly1);
	printf("请输入带入的值：");
	scanf_s("%d", &n);
#ifdef T
	start = clock();
#endif // !T  
	printf("值 = %.3f\n", EvaPoly(Poly1, n));
#ifdef T
	finish = clock();
	duration = (finish - start) / CLOCKS_PER_SEC;
	printf("多项式求值运算的时间为: %.3f\n", duration);
#endif // !T
}
static void test2() {
	Data value;
	Poly Poly1 = NULL, Poly2 = NULL;
	int n = 0;
	printf("请输入多项式的项数:");
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
	Poly lc = (*ptr)->Lchild;   //lc指向的*ptr的左孩子的根结点
	(*ptr)->Lchild = lc->Rchild;  //lc的右子树挂接为*ptr的左子树
	lc->Rchild = *ptr;
	*ptr = lc;                 //ptr指向新的结点"
}
static void L_Rotate(Poly* ptr) {
	Poly rc = (*ptr)->Rchild;   //rc指向的*ptr的由孩子的根结点
	(*ptr)->Rchild = rc->Lchild;   //rc的左子树挂接为*ptr的右子树
	rc->Lchild = *ptr;
	*ptr = rc;                  //ptr指向新的结点
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
		//该树为一棵空树，创建一个新节点作为根节点
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
		//应该继续在*ploy的左子树进行搜索
		if (!InsertAVL(&(*ploy)->Lchild, e, taller)) {
			//未插入
			return 0;
		}
		//已插入到*ploy的左子树中并且左子树长高
		if (*taller) {
			//检查*ploy的平衡度
			switch ((*ploy)->bf)
			{
				//原本左子树比右子树高
			case LH: {
				//平衡因子为-1
				//左旋
				LchildBalance(ploy);
				*taller = false;
				break;
			}
				   //原本左右树一样高，现在因为左子树长高树长高
			case EH: {
				//平衡因子为0
				(*ploy)->bf = LH;
				*taller = true;
				break;
			}
				   //原本右子树比左子树高，现在等高
			case RH: {
				//平衡因子为1
				(*ploy)->bf = EH;
				*taller = false;
				break;
			}
			}
		}
	}
	else {
		//应继续在*ploy的右子树中进行搜索    
		if (!InsertAVL(&(*ploy)->Rchild, e, taller)) {
			//未插入
			return 0;
		}
		//已插入到*ploy的右子树且右子树长高
		if (*taller) {
			//检查*ploy的平衡度
			switch ((*ploy)->bf)
			{
			case LH: {
				//原本左子树比右子树高，现在相等
				(*ploy)->bf = EH;
				*taller = false;
				break;
			}
			case EH: {
				//原来左右子树登高，现在因为右子树长高树长高
				(*ploy)->bf = RH;
				*taller = true;
				break;
			}
			case RH: {
				//原本右子树比左子树高，需要做右旋平衡处理 
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
	//系数为0
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

	printf("当前多项式为：\n");
	Data* p = (Data*)malloc(sizeof(Data) * n);
	bool taller;
	for (int i = 0; i < n; i++)
	{
		//生成数据
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
	fprintf_s(Write, "系数\t指数\n");
#ifdef T
	double start = clock();
#endif // !T  
	for (int i = 0; i < n; i++)
	{
		//插入数据
		InsertAVL(Poly, p[i], &taller);
		fprintf_s(Write, "%.2f\t %d\n", p[i].C, p[i].E);
	}
#ifdef T
	double finish = clock();
	double duration = (finish - start) / CLOCKS_PER_SEC;
	printf("创建多项式的时间为: %.3f\n", duration);
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
