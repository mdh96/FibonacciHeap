// Fibonacci-heap.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
/*
* C++ Program to Implement Fibonacci Heap
*/
#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;
/*
* Node Declaration
*/
struct node
{
	int value = 0;
	int degree;
	node* parent;
	node* child;
	node* left;
	node* right;
	bool marked;
};



/*
* Class Declaration
*/
class FibonacciHeap
{
protected:
	
	int heapSize;
	node *heap;
public:
	FibonacciHeap() {
		heap = NULL;
		heapSize = 0;
	}
	virtual ~FibonacciHeap() {
		if (heap) {
			_deleteAll(heap);
		}
	}
	
	void Union(FibonacciHeap H)
	{
		FibonacciHeap temp;
		temp.heap = H.heap;
		temp.heapSize = H.heapSize;
		while (temp.heap != NULL)
		{
			insert(temp.Extract_Min());
		}
	}

	bool isEmpty() {
		return heap == NULL;
	}

	int getMinimum() {
		return heap->value;
	}


	void decreaseKey(node* n, int value) {
		heap = _decreaseKey(heap, n, value);
	}
	void insert(node* n)
	{
		n->left = n->right = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;

		heap = _merge(heap, n);
	}
	node* find(int value) {
		return _find(heap, value);
	}

	node* _merge(node* a, node* b) {
		if (a == NULL)return b;
		if (b == NULL)return a;
		if (a->value>b->value) {
			node* temp = a;
			a = b;
			b = temp;
		}
		node* an = a->right;
		node* bp = b->left;
		a->right = b;
		b->left = a;
		an->left = bp;
		bp->right = an;
		heapSize++;
		return a;
	}

	void _deleteAll(node* n) {
		if (n != NULL) {
			node* c = n;
			do {
				node* d = c;
				c = c->right;
				_deleteAll(d->child);
				delete d;
			} while (c != n);
		}
	}

	void _addChild(node* parent, node* child) {
		child->left = child->right = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
	}


	node* FibonacciHeap::Extract_Min()
	{
		node* p;
		node* ptr;
		node* z = heap;
		p = z;
		ptr = z;
		if (z == NULL)
			return z;
		node* x;
		node* np;
		x = NULL;
		if (z->child != NULL)
			x = z->child;
		if (x != NULL)
		{
			ptr = x;
			do
			{
				np = x->right;
				(heap->left)->right = x;
				x->right = heap;
				x->left = heap->left;
				heap->left = x;
				if (x->value < heap->value)
					heap = x;
				x->parent = NULL;
				x = np;
			} while (np != ptr);
		}
		(z->left)->right = z->right;
		(z->right)->left = z->left;
		heap = z->right;
		if (z == z->right && z->child == NULL)
			heap = NULL;
		else
		{
			heap = z->right;
			Consolidate();
		}
		heapSize = heapSize - 1;
		return p;
	}

	void FibonacciHeap::Fibonnaci_link(node* y, node* z)
	{
		(y->left)->right = y->right;
		(y->right)->left = y->left;
		if (z->right == z)
			heap = z;
		y->left = y;
		y->right = y;
		y->parent = z;
		if (z->child == NULL)
			z->child = y;
		y->right = z->child;
		y->left = (z->child)->left;
		((z->child)->left)->right = y;
		(z->child)->left = y;
		if (y->value < (z->child)->value)
			z->child = y;
		z->degree++;
	}
	void FibonacciHeap::Consolidate()
	{
		int d, i;
		float f = (log(heapSize)) / (log(2));
		int D = f;
		
		node** A = new node*[D];
		for (i = 0; i <= D; i++)
			A[i] = NULL;
		node* x = heap;
		node* y;
		node* np;
		node* pt = x;
		do
		{
			pt = pt->right;
			d = x->degree;
			while (A[d] != NULL)
			{
				y = A[d];
				if (x->value > y->value)
				{
					np = x;
					x = y;
					y = np;
				}
				if (y == heap)
					heap = x;
				Fibonnaci_link(y, x);
				if (x->right == x)
					heap = x;
				A[d] = NULL;
				d = d + 1;
			}
			A[d] = x;
			x = x->right;
		} while (x != heap);
		heap = NULL;
		for (int j = 0; j <= D; j++)
		{
			if (A[j] != NULL)
			{
				A[j]->left = A[j];
				A[j]->right = A[j];
				if (heap != NULL)
				{
					(heap->left)->right = A[j];
					A[j]->right = heap;
					A[j]->left = heap->left;
					heap->left = A[j];
					if (A[j]->value < heap->value)
						heap = A[j];
				}
				else
				{
					heap = A[j];
				}
				if (heap == NULL)
					heap = A[j];
				else if (A[j]->value < heap->value)
					heap = A[j];
			}
		}
	}

	node* _cut(node* heap, node* n) {
		if (n->right == n) {
			n->parent->child = NULL;
		}
		else {
			n->right->left = n->left;
			n->left->right = n->right;
			n->parent->child = n->right;
		}
		n->right = n->left = n;
		n->marked = false;
		return _merge(heap, n);
	}

	node* _decreaseKey(node* heap, node* n, int value) {
		if (n->value<value)return heap;
		n->value = value;
		if (n->value<n->parent->value) {
			heap = _cut(heap, n);
			node* parent = n->parent;
			n->parent = NULL;
			while (parent != NULL && parent->marked) {
				heap = _cut(heap, parent);
				n = parent;
				parent = n->parent;
				n->parent = NULL;
			}
			if (parent != NULL && parent->parent != NULL)parent->marked = true;
		}
		return heap;
	}

	node* _find(node* heap, int value) {
		node* n = heap;
		if (n == NULL)return NULL;
		do {
			if (n->value == value)return n;
			node* ret = _find(n->child, value);
			if (ret)return ret;
			n = n->right;
		} while (n != heap);
		return NULL;
	}

	void dump() {
		printf("digraph G {\n");
		if (heap == NULL) {
			printf("empty;\n}\n");
			return;
		}
		printf("minimum -> \"%d\" [constraint=false];\n", heap->value);
		node* c = heap;
		do {
			_dumpChildren(c);
			c = c->right;
		} while (c != heap);
		printf("}\n");
	}

	void _dumpChildren(node* n) {
		printf("\"%d\" right-> \"%d\";\n", n->value, n->right->value);
		printf("\"%d\" left-> \"%d\";\n", n->value, n->left->value);
		if (n->marked)printf("\"%d\" marked;\n", n->value);
		if (n->parent) {
			printf("\"%d\" parent-> \"%d\";\n", n->value, n->parent->value);
		}
		if (n->child) {
			node* c = n->child;
			do {
				printf("\"%d\" child-> \"%d\";\n", n->value, c->value);
				_dumpChildren(c);
				c = c->right;
			} while (c != n->child);
		}
	}
};
void test() {
	FibonacciHeap h;
	FibonacciHeap g;
	node* twentythree = new node;
	twentythree->value = 23;
	node* seven = new node;
	seven->value = 7;

	node* twentyone = new node;
	twentyone->value = 21;
	node* eight = new node;
	eight->value = 8;

	h.insert(twentythree);
	h.insert(seven);
	g.insert(twentyone);
	g.insert(eight);
	//g.insert(40);
	//g.insert(45);
	FibonacciHeap x;
	x.Union(g);
	//g.Extract_Min();
	//x.insert(g.heap);
	/*h.insert(3);
	h.insert(18);
	h.insert(39);
	h.insert(52);
	h.insert(38);
	h.insert(41);
	h.insert(17);
	h.insert(30);
	h.insert(24);
	h.insert(26);
	h.insert(35);
	h.insert(46);*/
	//h.Extract_Min();
	//h.removeMinimum();
	//h.insert(5);
	//h.insert(7);
	//h.removeMinimum();
	//h.insert(2);
	/*node* nine = h.insert(90);
	h.removeMinimum();
	h.removeMinimum();
	h.removeMinimum();
	for (int i = 0; i<20; i += 2)h.insert(30 - i);
	for (int i = 0; i<4; i++)h.removeMinimum();
	for (int i = 0; i<20; i += 2)h.insert(30 - i);
	h.insert(23);
	for (int i = 0; i<7; i++)h.removeMinimum();
	h.decreaseKey(nine, 1);
	h.decreaseKey(h.find(28), 2);
	h.decreaseKey(h.find(23), 3);*/
	int i;
	x.dump();
	cin >> i;
}

int main()
{
	test();
	return 0;
}

