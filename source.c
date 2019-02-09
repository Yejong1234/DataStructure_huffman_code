// huffman.cpp : Defines the entry point for the console application.
//

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define MAX_ELEMENT 1000
#define MAX_BIT		10
#define MAX_CHAR	20

// Input data for huffman code
typedef struct input_huff {
	char *data;		// Character array (a ~ f)
	int *freq;		// Frequency array
	int size;		// Number of characters
} input_huff;

// Structure for huffman binary tree
typedef struct TreeNode {
	char data;			// Character (a ~ f)
	int key;			// Frequency
	int bits[MAX_BIT];	// Huffman codeword
	int bit_size;		// Huffman codeword's size
	struct TreeNode *l;	// Left child of huffman binary tree
	struct TreeNode *r;	// Right child of huffman binary tree
} TreeNode;

// Structure for bits stream
typedef struct bits_stream {
	int *stream;
	int length;
} bits_stream;

// Elements used in the heap
typedef struct element {
	TreeNode *ptree;
	int key;	// frequency of each character
} element;

// Heap
typedef struct HeapType {
	element heap[MAX_ELEMENT];
	int heap_size;
} HeapType;

int **m_LUT, *m_bit_size;
int	m_char_size = 6;

// Initialization
void init(HeapType *h)
{
	h->heap_size = 0;
}
//
int is_empty(HeapType *h)
{
	if (h->heap_size == 0)
		return 1;
	else
		return 0;
}

void insert_min_heap(HeapType *h, element item)
{
	int i;
	i = ++(h->heap_size);

	// compare it with the parent node in an order from the leaf to the root
	while ((i != 1) && (item.key < h->heap[i / 2].key)) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item; // Insert new node
}

element delete_min_heap(HeapType *h)
{
	int parent, child;
	element item, temp;
	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;
		if (temp.key <= h->heap[child].key) break;
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

// Node generation in binary tree
TreeNode *make_tree(TreeNode *left, TreeNode *right)
{
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
	if (node == NULL) {
		fprintf(stderr, "Memory allocation error\n");
		exit(1);
	}
	node->l = left;
	node->r = right;
	return node;
}

// Binary tree removal
void destroy_tree(TreeNode *root)
{
	if (root == NULL) return;
	destroy_tree(root->l);
	destroy_tree(root->r);
	free(root);
}

// Huffman code generation
element huffman_tree(input_huff *huff)
{
	int i;
	TreeNode *node, *x;
	HeapType heap;
	element e, e1, e2;
	init(&heap);

	int n = huff->size;

	for (i = 0; i < n; i++) {
		node = make_tree(NULL, NULL);
		e.ptree = node;
		node->data = huff->data[i];
		e.key = node->key = huff->freq[i];
		memset(node->bits, 0, sizeof(int)*MAX_BIT);
		node->bit_size = 0;

		insert_min_heap(&heap, e);
	}

	for (i = 1; i < n; i++) {
		// Delete two nodes with minimum values
		e1 = delete_min_heap(&heap);
		e2 = delete_min_heap(&heap);

		// Merge two nodes
		x = make_tree(e1.ptree, e2.ptree);
		e.ptree = x;
		x->data = NULL;
		e.key = x->key = e1.key + e2.key;
		memset(x->bits, 0, sizeof(int)*MAX_BIT);
		x->bit_size = 0;

		insert_min_heap(&heap, e);
	}
	e = delete_min_heap(&heap); // Final Huffman binary tree

	return e;
	//	destroy_tree(e.ptree);
}

// Generate the huffman codeword from the huffman binary tree
// Hint: use the recursion for tree traversal
// input: root node
// output: m_LUT, m_bit_size

int k, j = 0;

void huffman_traversal(TreeNode *node, int temp[], int top){//root node와 임시배열, top을새로 세워주었다.

if (node->l) {//왼쪽 노드가 존재한다면
	temp[top] = 0;//임시 배열에 0을 집어넣어 준다.
	huffman_traversal(node->l, temp, top + 1);//다시 재귀함수를 불러준다. 이 때 top을 증가시킴.

}
if (node->r) {//오른쪽 노드가 존재한다면
	temp[top] = 1;//임시배열에 1을 넣어준다.
	huffman_traversal(node->r, temp, top + 1);//다시 재귀함수.
}
if ((node->l == NULL)&(node->r == NULL)) {//만약 도착한 노드가 리프노드라면
	switch (node->data) {//a=0,b=1 이렇게 지정해주고 난 다음
	case 'a':
		k = 0;
		break;
	case 'b':
		k = 1;
		break;
	case 'c':
		k = 2;
		break;
	case 'd':
		k = 3;;
		break;
	case 'e':
		k = 4;
		break;
	case 'f':
		k = 5;
		break;
	}
	m_bit_size[k] = top;
	for (j = 0; j < top; j++) {//미리 선언한 2차원 배열에 코드를 넣어준다.
		m_LUT[k][j] = temp[j];
	}
}

}

int **mem_2D_int(int row, int col)
{
	int **m2 = (int **)malloc(sizeof(int *)*row);
	for (int i = 0; i < row; i++)
		m2[i] = (int *)malloc(sizeof(int)*col);
	return m2;
}

void print_codeword()
{
	printf("* Huffman codeword\n");
	for (int i = 0; i < m_char_size; i++)
	{
		switch (i) {
		case 0:
			printf("%c: ", 'a');
			break;
		case 1:
			printf("%c: ", 'b');
			break;
		case 2:
			printf("%c: ", 'c');
			break;
		case 3:
			printf("%c: ", 'd');
			break;
		case 4:
			printf("%c: ", 'e');
			break;
		case 5:
			printf("%c: ", 'f');
			break;
		}

		for (int j = 0; j < m_bit_size[i]; j++)
			printf("%d", m_LUT[i][j]);

		printf("\n");
	}

}

// Input: 'str'
// Output: 'bits_stream' (consisting of 0 or 1)
// 'bits_stream' is generated using 'm_LUT' generated by the huffman binary tree
// Return the total length of bits_stream
void huffman_encoding(char *str, bits_stream *bits_str)
{
	int interval = 0;
	int j = 0;//스트링 알파벳 갯수
	int k = 0;
	int l = strlen(str);//스트링 길이
	int arr[9];//알파벳 숫자를 담아두는 곳

	for (j = 0; j < l; j++) {//각각 알파벳에 알맞은 2차원 행 입력
		switch (str[j]) {
		case'a':
			arr[j] = 0;
			break;
		case'b':
			arr[j] = 1;
			break;
		case'c':
			arr[j] = 2;
			break;
		case'd':
			arr[j] = 3;
			break;
		case'e':
			arr[j] = 4;
			break;
		case'f':
			arr[j] = 5;
			break;
		}
	}
	for (j = 0; j < l; j++) {//각각의 코드 bits_Str에 넣어주기
		while (k < m_bit_size[arr[j]]) {
			bits_str->stream[interval++] = m_LUT[arr[j]][k++];
		}
		k = 0;
	}

	printf("\n* Huffman encoding\n");
	printf("total length of bits stream: %d\n", interval);
	printf("bits stream: ");
	for (int i = 0; i < interval; i++)
		printf("%d", bits_str->stream[i]);
	printf("\n");
	bits_str->length = interval;

}

// input: 'bits_stream' and 'total_length'
// output: 'decoded_str'
int huffman_decoding(bits_stream *bits_str, TreeNode *node, char *decoded_str)
{
	int i = 0;
	int j = 0;
	TreeNode *temp = node;
	int index_char = 0;

	for (i = 0; i < bits_str->length; i++) {//트리 따라가기
		if (bits_str->stream[i] == 0) {//만약 배열이 0이 있다면
			temp = temp->l;//왼쪽을 따라간다.
			if (temp->l == NULL && temp->r == NULL) {//만약 leaf 노드에도착했다면
				decoded_str[j++] = temp->data;//문자를 저장해주고
				index_char++;//해독된 문자 count 증가.
				temp = node;//노드는 다시 루트노드로 리셋.
			}
		}

		else if (bits_str->stream[i] == 1) {//만약 배열에 1이 있다면
			temp = temp->r;//오른쪽을 따라간다.
			if (temp->l == NULL && temp->r == NULL) {//똑같이 leat노드에 도착
				decoded_str[j++] = temp->data;
				index_char++;
				temp = node;
			}
		}
	}

	printf("\n* Huffman decoding\n");
	printf("total number of decoded chars: %d\n", index_char);
	printf("decoded chars: ");
	for (int i = 0; i < index_char; i++)
		printf("%c", decoded_str[i]);
	printf("\n");

	return index_char;
}

void main()
{
	int arr[MAX_BIT];
	char data[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
	int freq[] = { 45, 13, 12, 16, 9, 5 };

	input_huff *huff1 = (input_huff *)malloc(sizeof(input_huff));
	huff1->data = data;
	huff1->freq = freq;
	huff1->size = m_char_size;

	// m_LUT: each row corresponds to the codeword for each character
	// m_bit_size: 1D array of codeword size for each character
	// For instance, a = 0, b = 101, ...
	// 1st row of 'm_LUT': 0 0 ... 0
	// 2nd row of 'm_LUT': 1 0 1 ...0
	// m_bit_size = {1, 3, ...}
	m_LUT = mem_2D_int(m_char_size, MAX_BIT);
	m_bit_size = (int *)malloc(sizeof(int)*m_char_size);

	// Generate the huffman binary tree on heap
	// 'element_root': element containing the root node
	element element_root = huffman_tree(huff1);

	// Generate the huffman codeword from the huffman binary tree
	huffman_traversal(element_root.ptree, arr, 0);

	//printf out the huffman codeword
	print_codeword();

	//example of input data
	char str[MAX_CHAR] = { "abacdebaf" };
	//char str[MAX_CHAR] = { "ab" };
	char decoded_str[MAX_CHAR];

	printf("\n* input chars: ");
	for (int i = 0; i < strlen(str); i++)
		printf("%c", str[i]);
	printf("\n");

	//start encoding
	bits_stream *bits_str1 = (bits_stream *)malloc(sizeof(bits_stream));
	bits_str1->stream = (int *)malloc(sizeof(int)*MAX_BIT * MAX_CHAR);
	memset(bits_str1->stream, -1, sizeof(int)*MAX_BIT * MAX_CHAR);
	bits_str1->length = 0;

	huffman_encoding(str, bits_str1);

	//start decoding	
	int decoded_char_length = huffman_decoding(bits_str1, element_root.ptree, decoded_str);
	printf("1n시간 만에 완성했습니다 교수님!!!!!!!!!!!!!!!!!\n");


}
