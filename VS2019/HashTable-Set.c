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
//2^20�η���С�ļ���
typedef struct valueType {
	int value;
}ValueType;
typedef struct Pair {
	char : 5;
	char _bool : 1;		//����ɹ�
	char isZero : 1;	//�Ƿ�Ϊ0Ԫ��
	ValueType value;	//ֵ
	KeyType key;		//��
}Pair;
typedef struct Elem {
	struct Elem* next;
	Pair pair;
}Elem;
typedef struct HashTable {
	Elem* pElem;
	double lc;				//װ������	
	int elemNum;			//Ԫ�ظ���
	int length;				//��
}Node, * Set;

typedef struct Sign {
	int n;
	Set set;
}Sign;

//���ϵĴ���
Set CreateSet();
//Pair�Ĳ��룬����ֵΪ<Position,bool>
Pair PairInsert(Set set, const ValueType x);
//���ϴ洢��չ
//Set ExpandSet(Set* set);
//��������Pair��ֵ
void PairSwap(Pair* pair1, Pair* pair2);
//Pair��ɾ��������ֵΪ<Position,bool>
Pair PairErase(Set set, const ValueType x);
//��������
void PrintSet(Set set);
//formatΪ��ʽ����
//format == p����ӡ�������Ԫ��
//format == s��������Ԫ�ش���PairArray����
Sign OperateSet(Set set, const char* format, Pair PairArray[], Set set2);
//�����������ϵĲ����������¼���
Set Union(Set opSet1, Set opSet2);
//�����������ϵĽ����������¼���
Set Intersection(Set opSet1, Set opSet2);
//�����������ϵĲ�������¼���
Set Difference(Set opSet1, Set opSet2);
//���ټ���
void DestorySet(Set set);

//���Ժ���
void test1(int);
void test2();

int main() {
	int n = 0;
	printf("��������Լ���Ԫ�ص����ֵ:");
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
	printf("��������Ҫ���Ե�Ԫ�ظ�����ϵͳ���Զ�Ϊ����������\n");
	scanf_s("%d", &n);
	printf("���潫��ЩԪ�ز���Set1\n");
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
	printf("����ʱ��Ϊ��%.3f\n", duration);

	printf("����������Ҫ���Ե�Ԫ�ظ�����ϵͳ���Զ�Ϊ����������\n");
	scanf_s("%d", &n);
	printf("���潫��ЩԪ�ز���Set2");
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
	printf("����ʱ��Ϊ��%.3f\n\n", duration);

	printf("���㽻��");
	start = clock();
	set3 = Intersection(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("����ʱ��Ϊ��%.3f\n\n", duration);
	DestorySet(set3);

	set3 = CreateSet();
	printf("���㲢��");
	start = clock();
	set3 = Union(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("����ʱ��Ϊ��%.3f\n\n", duration);
	DestorySet(set3);

	set3 = CreateSet();
	printf("����");
	start = clock();
	set3 = Difference(set1, set2);
	finish = clock();
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
#ifdef P
	PrintSet(set3);
#endif // P
	printf("����ʱ��Ϊ��%.3f\n\n", duration);

	printf("ɾ��set3Ԫ��\n");
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
	printf("����ʱ��Ϊ��%.3f\n", duration);

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
		printf("������Ԫ��\n");
		Sign sign = { 0 };
		sign.n = 0;
		return sign;
	}
	//�¼���
	Set Nset = NULL;
	//nΪ��������ļ�¼������OutTimeΪ��������ļ�¼������I��¼�������Ҫ�ڶ�������ȫ��Ԫ�أ�
	int n = 0, OutTime = 1, I = 0;
	Elem* p = set->pElem;
	int length = set->length;
	if (!strcmp(format, "u")) {
		//����ȡlengthΪ��С��Ԫ�ظ���
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
		//�ڶ��˱�����һ������
		if (j == 1) {
			p = set2->pElem;
		}
		if (I == 1 && j == 1) {
			length = set2->length;
		}
		for (int i = 0; i < length; ++i) {
			//0Ԫ��
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
				//������
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
			//����0Ԫ��
			else {
				//��λ����Ԫ��
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
					//������
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
	KeyType position = HashFunction(x.value);		//����hash�����ҵ�ɢ�е�ַ

	//��λ������Ԫ��
	if (set->pElem[position].pair.isZero == -1) {
		//Ԫ�����
		if (set->pElem[position].pair.value.value == x.value) {
			//Ԫ����Ȳ���ʧ��
			Pair pair = { 0 };
			pair._bool = 0;
			pair.key = position;
			return pair;
		}
		//Ԫ�ز���
		else {
			//�������������ظ�Ԫ��
			//����
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
					//Ԫ�����
					if (pelem->pair.value.value == x.value) {
						//Ԫ����Ȳ���ʧ��
						Pair pair = { 0 };
						pair._bool = 0;
						pair.key = position;
						return pair;
					}
					else {
						//��¼���һ��Ԫ�ص�λ��
						if (pelem->next == NULL) {
							last = pelem;
						}
						pelem = pelem->next;
					}
				}
				//δ����ͬԪ��
				//�����Ԫ��
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
	//��λ�ò�����Ԫ��
	else {
		//��ǰλ����Ԫ�أ����Դ�ֵ
		if (set->pElem[position].pair.value.value == 0) {
			//����Ԫ����Ϊ0Ԫ��
			if (x.value == 0) {
				set->pElem[position].pair.value = x;
				set->pElem[position].pair.key = position;
				//bool��Ϊ1����ʾ��λ����0Ԫ��
				set->pElem[position].pair.isZero = 1;
			}
			//����Ԫ�ز���0Ԫ��
			else {
				set->pElem[position].pair.value = x;
				set->pElem[position].pair.key = position;
			}
			Pair pair = { 0 };
			pair.key = position;
			pair._bool = 1;
			pair.value = set->pElem[position].pair.value;
			//Ԫ�ظ���+1��װ�����Ӹı�
			set->elemNum++;
			set->lc = set->elemNum * 1.0 / set->length;
			return pair;
		}
		//��ǰλ����Ԫ��
		else {
			//Ԫ�����
			if (set->pElem[position].pair.value.value == x.value) {
				//Ԫ����Ȳ���ʧ��
				Pair pair = { 0 };
				pair._bool = 0;
				pair.key = position;
				return pair;
			}
			//Ԫ�ز���
			else {
				//�������������ظ�Ԫ��
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
						//Ԫ�����
						if (pelem->pair.value.value == x.value) {
							//Ԫ����Ȳ���ʧ��
							Pair pair = { 0 };
							pair._bool = 0;
							pair.key = position;
							return pair;
						}
						else {
							//��¼���һ��Ԫ�ص�λ��
							if (pelem->next == NULL) {
								last = pelem;
							}
							pelem = pelem->next;
						}
					}
					//δ����ͬԪ��
					//�����Ԫ��
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
	//ɾ��Ԫ��Ϊ0Ԫ��
	if (x.value == 0) {
		//��Ԫ����ֱ����������
		if (set->pElem[0].pair.isZero == -1) {
			//������
			if (set->pElem[0].next == NULL) {
				set->pElem[0].pair.isZero = 0;
				//�ı�Ԫ�ظ���
				set->elemNum--;
				//����װ������
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
			//������
			else {
				//����һ��λ�õ�Ԫ��ֵ����������������һ��Ԫ��ɾ��
				Elem* p = set->pElem[0].next;
				Elem* q = p->next;
				PairSwap(&set->pElem[0].pair, &(p->pair));
				free(p);
				set->pElem[0].next = q;
				//�ı�Ԫ�ظ���
				set->elemNum--;
				//����װ������
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
		}
		//0Ԫ�ز���������
		else {
			Elem* p = set->pElem[0].next;
			Elem* q = set->pElem[0].next;
			while (p) {
				if (p->pair.value.value == 0) {
					q->next = p->next;
					free(p);
					//�ı�Ԫ�ظ���
					set->elemNum--;
					//����װ������
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
	//ɾ����Ԫ�ز�Ϊ0
	else {
		KeyType position = HashFunction(x.value);		//����hash�����ҵ�ɢ�е�ַ
		//ɾ��λ����������
		if (set->pElem[position].pair.value.value == x.value) {
			//������
			if (set->pElem[position].next == NULL) {
				set->pElem[position].pair.value.value = 0;
				//�ı�Ԫ�ظ���
				set->elemNum--;
				//����װ������
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[0].pair.key;
				pair._bool = 1;
				return pair;
			}
			//������
			else {
				//����һ��λ�õ�Ԫ��ֵ����������������һ��Ԫ��ɾ��
				Elem* p = set->pElem[position].next;
				Elem* q = p->next;
				PairSwap(&set->pElem[position].pair, &(p->pair));
				free(p);
				set->pElem[position].next = q;
				//�ı�Ԫ�ظ���
				set->elemNum--;
				//����װ������
				set->lc = set->elemNum * 1.0 / set->length;
				Pair pair;
				pair.key = set->pElem[position].pair.key;
				pair._bool = 1;
				return pair;
			}
		}
		//ɾ��λ�ò���������
		else {
			Elem* p = set->pElem[position].next;
			Elem* q = set->pElem[position].next;
			while (p) {
				if (p->pair.value.value == x.value) {
					q->next = p->next;
					free(p);
					//�ı�Ԫ�ظ���
					set->elemNum--;
					//����װ������
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
