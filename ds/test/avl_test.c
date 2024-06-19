#include <stdio.h>	/* printf */

#include "bst.h"
#include "test_utility.h"



void TestIsEmpty();
void TestInsert();
void TestSize();
void TestRemove();
void TestFind();
void TestForEach();




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






int main()
{
	
	TestIsEmpty();
	TestInsert();
	TestSize();
	TestRemove();
	TestFind();
	TestForEach();
	
	Test();
	
	return 0;
}




static int CompareInt(const void *item, const void *data_to_compare)
{
 	return (*(int *)item - *(int *)data_to_compare);
}
