#include <stdio.h>	/* printf */

#include "avl.h"

/*
#include "test_utility.h"



void TestIsEmpty();
void TestInsert();
void TestSize();
void TestRemove();
void TestFind();
void TestForEach();
*/

static int CompareInt(const void *item, const void *data_to_compare);

int main()
{
	/*
	TestIsEmpty();
	TestInsert();
	TestSize();
	TestRemove();
	TestFind();
	TestForEach();
	Test();
	*/
	
	int x1 = 1, x2 = 2, x3 = 3, x4 = 4, x5 =5, x6 = 6, x7 = 7, x10 = 10, x15 = 15, x11 = 11, x12 = 12, x13 = 13;
	int status = 0;
	
	avl_t *avl = AVLCreate(CompareInt);
	printf("is empty? %d\n", AVLIsEmpty(avl));
	printf("tree size = %lu\n", AVLCount(avl));
	
	status = AVLInsert(avl, &x2);
	
	printf("is empty? %d\n", AVLIsEmpty(avl));
	printf("tree size = %lu\n", AVLCount(avl));
	
	status = AVLInsert(avl, &x3);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x1);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x4);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x5);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x6);
	printf("tree size = %lu\n", AVLCount(avl));
	
	status = AVLInsert(avl, &x7);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x15);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x10);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x13);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x12);
	printf("tree size = %lu\n", AVLCount(avl));
	status = AVLInsert(avl, &x11);
	printf("tree size = %lu\n", AVLCount(avl));
	
	
	printf("searching: %d\n", *(int *)AVLFind(avl, &x1));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x2));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x3));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x4));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x5));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x6));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x7));
	printf("searching: %d\n", *(int *)AVLFind(avl, &x15));
	
	
	
	AVLDestroy(avl);
	
	return 0;
}




static int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}

/*

void TestIsEmpty()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int res = BSTIsEmpty(tree);
	
	TestInt("TestIsEmpty", __FILE__, __LINE__, 1, res);
	
	BSTDestroy(tree);
}

void TestInsert()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int is_empty_res = 0;
	int data = 5;
	int *get_data_res = NULL;
	bst_iterator_t insert_res = BSTInsert(tree, &data);
	bst_iterator_t begin_res = BSTBegin(tree);
	
	get_data_res = BSTGetData(insert_res);
	
	TestInt("TestInsert", __FILE__, __LINE__, data, *get_data_res);
	
	get_data_res = BSTGetData(begin_res);
	
	TestInt("TestInsert", __FILE__, __LINE__, data, *get_data_res);
	
	is_empty_res = BSTIsEmpty(tree);
	
	TestInt("TestInsert", __FILE__, __LINE__, 0, is_empty_res);
	
	BSTDestroy(tree);
}


void TestSize()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int data[] = {4, 20, 9, 35, 6, 2};
	size_t size_res = 0;
	size_t i = 0;
	
	for (; i < sizeof(data) / sizeof(int); ++i)
	{
		BSTInsert(tree, data + i);
	}
	
	size_res = BSTSize(tree);
	
	TestSizeT("TestSize", __FILE__, __LINE__, 6, size_res);
	
	BSTDestroy(tree);
}

void TestRemove()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int data[] = {4, 20, 9, 35, 6, 2};
	bst_iterator_t runner = NULL;
	int *remove_res = NULL;
	size_t i = 0;
	size_t size_res = 0;
	
	for (; i < sizeof(data) / sizeof(int); ++i)
	{
		BSTInsert(tree, data + i);
	}
	
	runner = BSTNext(BSTNext(BSTNext(BSTNext(BSTBegin(tree)))));
	remove_res = (int *)BSTRemove(runner);
	size_res = BSTSize(tree);
	
	TestInt("TestRemove", __FILE__, __LINE__, 20, *remove_res);
	TestSizeT("TestRemove", __FILE__, __LINE__, 5, size_res);
	
	BSTDestroy(tree);
}

(tree);
}

void TestFind()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int data[] = {4, 20, 9, 35, 6, 2};
	bst_iterator_t find_res = {0};
	int *get_data_res = NULL;
	size_t i = 0;
	
	for (; i < sizeof(data) / sizeof(int); ++i)
	{
		BSTInsert(tree, data + i);
	}
	
	find_res = BSTFind(tree, data);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 4, *get_data_res);
	
	find_res = BSTFind(tree, data + 1);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 20, *get_data_res);
	
	find_res = BSTFind(tree, data + 2);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 9, *get_data_res);
	
	find_res = BSTFind(tree, data + 3);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 35, *get_data_res);
	
	find_res = BSTFind(tree, data + 4);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 6, *get_data_res);
	
	find_res = BSTFind(tree, data + 5);
	get_data_res = BSTGetData(find_res);
	
	TestInt("TestFind", __FILE__, __LINE__, 2, *get_data_res);
	
	BSTDestroy(tree);
}

static int AddTen(void *element_data, void *input_data)
{
	(void)input_data;
	
	*(int *)element_data += 10;
	
	return 0;
}

void TestForEach()
{
	bst_t *tree = BSTCreate(&CompareInt);
	int data[] = {4, 20, 9, 35, 6, 2};
	int *get_data_res = NULL;
	bst_iterator_t runner = NULL;
	size_t i = 0;
	
	for (; i < sizeof(data) / sizeof(int); ++i)
	{
		BSTInsert(tree, data + i);
	}
	
	BSTForEach(tree, &AddTen, NULL);

	runner = BSTBegin(tree);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 12, *get_data_res);
	
	runner = BSTNext(runner);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 14, *get_data_res);
	
	runner = BSTNext(runner);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 16, *get_data_res);
	
	runner = BSTNext(runner);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 19, *get_data_res);
	
	runner = BSTNext(runner);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 30, *get_data_res);
	
	runner = BSTNext(runner);
	get_data_res = BSTGetData(runner);
	
	TestInt("TestNext", __FILE__, __LINE__, 45, *get_data_res);
	
	BSTDestroy(tree);
}


*/




