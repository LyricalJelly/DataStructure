#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <time.h>
#define P
#define KeyType int
//2^20次方大小的集合
typedef struct valueType {
	int value;
}ValueType;
typedef struct Pair {
	char : 5;
	char _bool : 1;		//插入成功
	char isZero : 1;	//是否为0元素
	ValueType value;	//值
	KeyType key;		//键
}Pair;
typedef struct Elem {
	struct Elem* next;
	Pair pair;
}Elem;
typedef struct HashTable {
	Elem* pElem;
	double lc;				//装填因子	
	int elemNum;			//元素个数
	int length;				//表长
}Node, * Set;

typedef struct Sign {
	int n;
	Set set;
}Sign;

//集合的创建
Set CreateSet();
//Pair的插入，返回值为<Position,bool>
Pair PairInsert(Set set, const ValueType x);
//集合存储扩展
//Set ExpandSet(Set* set);
//交换两个Pair的值
void PairSwap(Pair* pair1, Pair* pair2);
//Pair的删除，返回值为<Position,bool>
Pair PairErase(Set set, const ValueType x);
//遍历集合
void PrintSet(Set set);
//format为格式参数
//format == p：打印输出集合元素
//format == s：将集合元素存入PairArray数组
Sign OperateSet(Set set, const char* format, Pair PairArray[], Set set2);
//计算两个集合的并集，返回新集合
Set Union(Set opSet1, Set opSet2);
//计算两个集合的交集，返回新集合
Set Intersection(Set opSet1, Set opSet2);
//计算两个集合的差集，返回新集合
Set Difference(Set opSet1, Set opSet2);
//销毁集合
void DestorySet(Set set);

//测试函数
void test1(int);
void test2();

int main() {
	int n = 0;
	printf("请输入测试集合元素的最大值:");
	scanf_s("%d", &n);
	test1(n);
	return 0;
}

void test1(int Max) {
	Set set1 = CreateSet();
	Set set2 = CreateSet();
	Set set3 = CreateSet();
	ValueType v = { 0 };
	double duration = 0;
	int n = 0;
	printf("请输入需要测试的元素个数，系统将自动为您生成数据\n");
	scanf_s("%d", &n);
	printf("下面将这些元素插入Set1\n");
	clock_t start, finish;
	start = clock();
	for (int i = 0; i < n; ++i) {
		v.value = rand() % Max;
		PairInsert(set1, v);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set1);
#endif // P
	printf("花费时间为：%.3f\n", duration);

	printf("请再输入需要测试的元素个数，系统将自动为您生成数据\n");
	scanf_s("%d", &n);
	printf("下面将这些元素插入Set2");
	start = clock();
	for (int i = 0; i < n; ++i) {
		v.value = rand() % Max;
		PairInsert(set2, v);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set2);
#endif // P
	printf("花费时间为：%.3f\n\n", duration);

	printf("计算交集");
	start = clock();
	set3 = Intersection(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("花费时间为：%.3f\n\n", duration);
	DestorySet(set3);

	set3 = CreateSet();
	printf("计算并集");
	start = clock();
	set3 = Union(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("花费时间为：%.3f\n\n", duration);
	DestorySet(set3);

	set3 = CreateSet();
	printf("计算差集");
	start = clock();
	set3 = Difference(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("花费时间为：%.3f\n\n", duration);

	printf("删除set3元素\n");
	start = clock();
	for (int i = 0; i < n; ++i) {
		v.value = rand() % Max;
		printf("%d ", v.value);
		PairErase(set3, v);
	}
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	printf("\n");
#ifdef P
	PrintSet(set3);
#endif // P
	printf("花费时间为：%.3f\n", duration);

}

void test2() {
	srand((unsigned int)time(NULL));
	Set set = CreateSet();
	int n = 0;
	scanf_s("%d", &n);
	for (int i = 0; i < n; ++i) {
		ValueType v = { 0 };
		v.value = rand() % 100000;
		Pair pair = { 0 };
		pair = PairInsert(set, v);
		printf("%d ", pair.value.value);
	}
	printf("\n\n\n");
	PrintSet(set);
	system("pause");
}

Sign OperateSet(Set set, const char* format, Pair PairArray[], Set set2) {
	if (set->elemNum == 0) {
		printf("集合无元素\n");
		Sign sign = { 0 };
		sign.n = 0;
		return sign;
	}
	//新集合
	Set Nset = NULL;
	//n为存入数组的记录变量，OutTime为其他运算的记录变量，I记录求差集（差集需要第二个集合全部元素）
	int n = 0, OutTime = 1, I = 0;
	Elem* p = set->pElem;
	int length = set->length;
	if (!strcmp(format, "u")) {
		//并集取length为最小的元素个数
		length = set->length < set2->length ? set->length : set2->length;
		Nset = CreateSet();
		assert(Nset != NULL);
		OutTime = 2;
	}
	else if (!strcmp(format, "d")) {
		Nset = CreateSet();
		assert(Nset != NULL);
		OutTime = 2;
		I = 1;
	}
	for (int j = 0; j < OutTime; ++j) {
		//第二趟遍历另一个集合
		if (j == 1) {
			p = set2->pElem;
		}
		if (I == 1 && j == 1) {
			length = set2->length;
		}
		for (int i = 0; i < length; ++i) {
			//0元素
			if (p[i].pair.isZero == -1) {
				if (!strcmp(format, "p")) {
					printf("%d ", p[i].pair.value.value);
				}
				else if (!strcmp(format, "s")) {
					assert(PairArray != NULL);
					PairArray[n++] = p[i].pair;
				}
				else if (!strcmp(format, "u")) {
					PairInsert(Nset, p[i].pair.value);
				}
				else if (!strcmp(format, "d")) {
					if (j == 1) {
						PairErase(Nset, p[i].pair.value);
					}
					else {
						PairInsert(Nset, p[i].pair.value);
					}
				}
				//有链子
				if (p[i].next != NULL) {
					Elem* tp = p[i].next;
					while (tp) {
						if (!strcmp(format, "p")) {
							printf("%d ", p[i].pair.value.value);
						}
						else if (!strcmp(format, "s")) {
							assert(PairArray != NULL);
							PairArray[n++] = p[i].pair;
						}
						else if (!strcmp(format, "u")) {
							PairInsert(Nset, p[i].pair.value);
						}
						else if (!strcmp(format, "d")) {
							if (j == 1) {
								PairErase(Nset, p[i].pair.value);
							}
							else {
								PairInsert(Nset, p[i].pair.value);
							}
						}
						tp = tp->next;
					}
				}
			}
			//不是0元素
			else {
				//该位置有元素
				if (p[i].pair.value.value != 0) {
					if (!strcmp(format, "p")) {
						printf("%d ", p[i].pair.value.value);
					}
					else if (!strcmp(format, "s")) {
						assert(PairArray != NULL);
						PairArray[n++] = p[i].pair;
					}
					else if (!strcmp(format, "u")) {
						PairInsert(Nset, p[i].pair.value);
					}
					else if (!strcmp(format, "d")) {
						if (j == 1) {
							PairErase(Nset, p[i].pair.value);
						}
						else {
							PairInsert(Nset, p[i].pair.value);
						}
					}
					//有链子
					if (p[i].next != NULL) {
						Elem* tp = p[i].next;
						while (tp) {
							if (!strcmp(format, "p")) {
								printf("%d ", p[i].pair.value.value);
							}
							else if (!strcmp(format, "s")) {
								assert(PairArray != NULL);
								PairArray[n++] = p[i].pair;
							}
							else if (!strcmp(format, "u")) {
								PairInsert(Nset, p[i].pair.value);
							}
							else if (!strcmp(format, "d")) {
								if (j == 1) {
									PairErase(Nset, p[i].pair.value);
								}
								else {
									PairInsert(Nset, p[i].pair.value);
								}
							}
							tp = tp->next;
						}
					}
				}
			}
		}
	}

	printf("\n");
	Sign sign = { 0 };
	sign.n = n;
	sign.set = Nset;
	return sign;
}
Set CreateSet() {
	Set set = (Set)calloc(1, sizeof(Node));
	set->length = pow(2, 24);//8380001
	set->pElem = (Elem*)calloc(set->length, sizeof(Elem));
	set->lc = set->elemNum * 1.0 / set->length;
	return set;
}
void DestorySet(Set set) {
	free(set->pElem);
	free(set);
}
Pair PairInsert(Set set, const ValueType x) {
	KeyType position = HashFunction(x.value);		//调用hash函数找到散列地址

	//该位置是零元素
	if (set->pElem[position].pair.isZero == -1) {
		//元素相等
		if (set->pElem[position].pair.value.value == x.value) {
			//元素相等插入失败
			Pair pair = { 0 };
			pair._bool = 0;
			pair.key = position;
			return pair;
		}
		//元素不等
		else {
			//遍历链表，查找重复元素
			//无链
			if (set->pElem[position].next == NULL) {
				set->pElem[position].next = (Elem*)calloc(1, sizeof(Elem));
				set->pElem[position].next->pair.value = x;
				if (x.value == 0) {
					set->pElem[position].next->pair.isZero = 1;
				}
				set->pElem[position].next->pair.key = position;
				set->elemNum++;
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair = { 0 };
				pair._bool = 1;
				pair.key = position;
				return pair;
			}
			else {
				Elem* pelem = set->pElem[position].next;
				Elem* last = pelem;
				while (pelem) {
					//元素相等
					if (pelem->pair.value.value == x.value) {
						//元素相等插入失败
						Pair pair = { 0 };
						pair._bool = 0;
						pair.key = position;
						return pair;
					}
					else {
						//记录最后一个元素的位置
						if (pelem->next == NULL) {
							last = pelem;
						}
						pelem = pelem->next;
					}
				}
				//未有相同元素
				//插入该元素
				Elem* pnew = (Elem*)calloc(1, sizeof(Elem));
				pnew->pair.value = x;
				if (x.value == 0) {
					pnew->pair.isZero = 1;
				}
				pnew->pair.key = position;
				if (last != NULL) {
					last->next = pnew;
				}
				else {
					pelem = pnew;
				}
				set->elemNum++;
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair = { 0 };
				pair._bool = 1;
				pair.key = position;
				return pair;
			}

		}
		Pair pair = { 0 };
		pair._bool = 0;
		pair.key = position;
		return pair;
	}
	//该位置不是零元素
	else {
		//当前位置无元素，可以存值
		if (set->pElem[position].pair.value.value == 0) {
			//插入元素是为0元素
			if (x.value == 0) {
				set->pElem[position].pair.value = x;
				set->pElem[position].pair.key = position;
				//bool变为1，表示该位置是0元素
				set->pElem[position].pair.isZero = 1;
			}
			//插入元素不是0元素
			else {
				set->pElem[position].pair.value = x;
				set->pElem[position].pair.key = position;
			}
			Pair pair = { 0 };
			pair.key = position;
			pair._bool = 1;
			pair.value = set->pElem[position].pair.value;
			//元素个数+1，装填因子改变
			set->elemNum++;
			set->lc = set->elemNum * 1.0 / set->length;
			return pair;
		}
		//当前位置有元素
		else {
			//元素相等
			if (set->pElem[position].pair.value.value == x.value) {
				//元素相等插入失败
				Pair pair = { 0 };
				pair._bool = 0;
				pair.key = position;
				return pair;
			}
			//元素不等
			else {
				//遍历链表，查找重复元素
				if (set->pElem[position].next == NULL) {
					set->pElem[position].next = (Elem*)calloc(1, sizeof(Elem));
					set->pElem[position].next->pair.value = x;
					if (x.value == 0) {
						set->pElem[position].next->pair.isZero = 1;
					}
					set->pElem[position].next->pair.key = position;
					set->elemNum++;
					set->lc = set->elemNum * 1.0 / set->length;
					Pair pair = { 0 };
					pair._bool = 1;
					pair.key = position;
					return pair;
				}
				else {
					Elem* pelem = set->pElem[position].next;
					Elem* last = pelem;
					while (pelem) {
						//元素相等
						if (pelem->pair.value.value == x.value) {
							//元素相等插入失败
							Pair pair = { 0 };
							pair._bool = 0;
							pair.key = position;
							return pair;
						}
						else {
							//记录最后一个元素的位置
							if (pelem->next == NULL) {
								last = pelem;
							}
							pelem = pelem->next;
						}
					}
					//未有相同元素
					//插入该元素
					Elem* pnew = (Elem*)calloc(1, sizeof(Elem));
					pnew->pair.value = x;
					if (x.value == 0) {
						pnew->pair.isZero = 1;
					}
					pnew->pair.key = position;
					if (last != NULL) {
						last->next = pnew;
					}
					else {
						pelem = pnew;
					}
					set->elemNum++;
					set->lc = set->elemNum * 1.0 / set->length;
					Pair pair = { 0 };
					pair._bool = 1;
					pair.key = position;
					return pair;
				}
			}
		}
	}
}
static int HashFunction(KeyType key) {
	return (key % 8380001);
}
void PairSwap(Pair* pair1, Pair* pair2) {
	Pair tpair = *pair1;
	*pair1 = *pair2;
	*pair2 = tpair;
}
void PrintSet(Set set) {
	OperateSet(set, "p", NULL, NULL);
}
Pair PairErase(Set set, const ValueType x) {
	//删除元素为0元素
	if (x.value == 0) {
		//零元素在直接在数组中
		if (set->pElem[0].pair.isZero == -1) {
			//无链子
			if (set->pElem[0].next == NULL) {
				set->pElem[0].pair.isZero = 0;
				//改变元素个数
				set->elemNum--;
				//计算装填因子
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
			//有链子
			else {
				//将下一个位置的元素值换到这里来，将下一个元素删除
				Elem* p = set->pElem[0].next;
				Elem* q = p->next;
				PairSwap(&set->pElem[0].pair, &(p->pair));
				free(p);
				set->pElem[0].next = q;
				//改变元素个数
				set->elemNum--;
				//计算装填因子
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
		}
		//0元素不在数组中
		else {
			Elem* p = set->pElem[0].next;
			Elem* q = set->pElem[0].next;
			while (p) {
				if (p->pair.value.value == 0) {
					q->next = p->next;
					free(p);
					//改变元素个数
					set->elemNum--;
					//计算装填因子
					set->lc = set->elemNum * 1.0 / set->length;
					Pair pair;
					pair.key = set->pElem[0].pair.key;
					pair._bool = 1;
					return pair;
				}
				else {
					q = p;
					p = p->next;
				}
			}
		}
	}
	//删除的元素不为0
	else {
		KeyType position = HashFunction(x.value);		//调用hash函数找到散列地址
		//删除位置在数组中
		if (set->pElem[position].pair.value.value == x.value) {
			//无链子
			if (set->pElem[position].next == NULL) {
				set->pElem[position].pair.value.value = 0;
				//改变元素个数
				set->elemNum--;
				//计算装填因子
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
			//有链子
			else {
				//将下一个位置的元素值换到这里来，将加一个元素删除
				Elem* p = set->pElem[position].next;
				Elem* q = p->next;
				PairSwap(&set->pElem[position].pair, &(p->pair));
				free(p);
				set->pElem[position].next = q;
				//改变元素个数
				set->elemNum--;
				//计算装填因子
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[position].pair.key;
				pair._bool = 1;
				return pair;
			}
		}
		//删除位置不在数组中
		else {
			Elem* p = set->pElem[position].next;
			Elem* q = set->pElem[position].next;
			while (p) {
				if (p->pair.value.value == x.value) {
					q->next = p->next;
					free(p);
					//改变元素个数
					set->elemNum--;
					//计算装填因子
					set->lc = set->elemNum * 1.0 / set->length;
					Pair pair;
					pair.key = set->pElem[position].pair.key;
					pair._bool = 1;
					return pair;
				}
				else {
					q = p;
					p = p->next;
				}
			}
		}
		Pair pair;
		pair.key = position;
		pair._bool = 0;
		return pair;
	}

}
Set Union(Set opSet1, Set opSet2) {
	Sign sign = { 0 };
	sign = OperateSet(opSet1, "u", NULL, opSet2);
	return sign.set;
}
Set Intersection(Set opSet1, Set opSet2) {
	Set set = CreateSet();
	Pair* PairArray1 = (Pair*)malloc(opSet1->elemNum * sizeof(Pair));
	OperateSet(opSet1, "s", PairArray1, NULL);
	Pair* PairArray2 = (Pair*)malloc(opSet2->elemNum * sizeof(Pair));
	OperateSet(opSet2, "s", PairArray2, NULL);

	for (int i = 0; i < opSet1->elemNum; i++) {
		for (int j = 0; j < opSet2->elemNum; ++j) {
			if (PairArray1[i].value.value == PairArray2[j].value.value) {
				PairInsert(set, PairArray1[i].value);
			}
		}
	}
	free(PairArray1);
	free(PairArray2);
	PairArray1 = PairArray2 = NULL;
	return set;
}
Set Difference(Set opSet1, Set opSet2) {
	Sign sign = { 0 };
	sign = OperateSet(opSet1, "d", NULL, opSet2);
	return sign.set;
}
