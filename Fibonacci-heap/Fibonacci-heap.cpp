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
	node *min;
	int heapSize;
public:
	FibonacciHeap() {
		min = NULL;
		heapSize = 0;
	}
	
	void Union(FibonacciHeap *H)
	{
		heapSize = heapSize + H->heapSize;
		if (H->min != NULL) {
			//Get left most and right most node in heap
			node *r1 = min->left;
			node *l1 = min;
			//Get left most and right most node of other heap
			node *r2 = H->min->left;
			node *l2 = H->min;
			if (l1->value > l2->value)
				min = l2;
			else
				min = l1;
			l1->left = r2;
			r2->right = l1;
			l2->left = r1;
			r1->right = l2;
		}
	}

	bool isEmpty() {
		return min == NULL;
	}

	int getMinimum() {
		return min->value;
	}


	/*void decreaseKey(node* n, int value) {
		min = _decreaseKey(min, n, value);
	}*/
	void insert(node* n)
	{
		n->left = n->right = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;

		if (min == NULL) {
			min = n;
			min->left = min;
			min->right = min;
		}
		else if (n != NULL) {
			if (min->value>n->value) {
				node* temp = min;
				min = n;
				n = temp;
			}
			node* an = min->right;
			node* bp = n->left;
			min->right = n;
			n->left = min;
			an->left = bp;
			bp->right = an;
		}
		heapSize++;
	}
	void insert(int value)
	{
		node* n = new node;
		n->value = value;
		insert(n);
	}
	node* find(int value) {
		return _find(min, value);
	}

	void _deleteAll(node* n) {
		if (n != NULL) {
			if (n->value < 0) {
				delete n;
			}
			else {
				node* c = n;
				do {
					node* d = c;
					c = c->right;
					_deleteAll(d->child);
					delete d;
					//c->left = NULL;
				} while (c != n);
			}
		}
	}

	

	node* _merge(node* a, node* b) {
		heapSize++;
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
		return a;
	}

	

	node* FibonacciHeap::Extract_Min()
	{
		node* p;
		node* ptr;
		node* z = min;
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
				(min->left)->right = x;
				x->right = min;
				x->left = min->left;
				min->left = x;
				if (x->value < min->value)
					min = x;
				x->parent = NULL;
				x = np;
			} while (np != ptr);
		}
		(z->left)->right = z->right;
		(z->right)->left = z->left;
		min = z->right;
		if (z == z->right && z->child == NULL)
			min = NULL;
		else
		{
			min = z->right;
			Consolidate();
		}
		heapSize--;
		return p;
	}

	void FibonacciHeap::Consolidate()
	{
		int d, i;
		float f = (log(heapSize)) / (log(2));
		int D = f;

		node** A = new node*[D];
		for (i = 0; i <= D; i++)
			A[i] = NULL;

		node* x = min;
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
				if (y == min)
					min = x;
				Fibonnaci_link(y, x);
				if (x->right == x)
					min = x;
				A[d] = NULL;
				d = d + 1;
			}
			A[d] = x;
			x = x->right;
		} while (x != min);
		min = NULL;
		for (int j = 0; j <= D; j++)
		{
			if (A[j] != NULL)
			{
				A[j]->left = A[j];
				A[j]->right = A[j];
				if (min != NULL)
				{
					(min->left)->right = A[j];
					A[j]->right = min;
					A[j]->left = min->left;
					min->left = A[j];
					if (A[j]->value < min->value)
						min = A[j];
				}
				else
				{
					min = A[j];
				}
				if (min == NULL)
					min = A[j];
				else if (A[j]->value < min->value)
					min = A[j];
			}
		}
	}

	void FibonacciHeap::Fibonnaci_link(node* y, node* z)
	{
		(y->left)->right = y->right;
		(y->right)->left = y->left;
		if (z->right == z)
			min = z;
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

	node* createNode(int num)
	{
		node* n = new node;
		n->value = num;
		n->left = n->right = n;
		n->degree = 0;
		n->marked = false;
		n->child = NULL;
		n->parent = NULL;
		return n;
	}
	void firstEx()
	{
		node* n3 = createNode(3);
		node* n23 = createNode(23);
		node* n7 = createNode(7);
		node* n21 = createNode(21);
		node* n17 = createNode(17);
		node* n24 = createNode(24);
		node* n18 = createNode(18);
		node* n52 = createNode(52);
		node* n38 = createNode(38);
		node* n30 = createNode(30);
		node* n26 = createNode(26);
		node* n39 = createNode(39);
		node* n41 = createNode(41);
		node* n35 = createNode(35);
		node* n46 = createNode(46);
		insert(n3);
		
		_addChild(n3, n38);
		_addChild(n38, n41);
		_addChild(n3, n18);
		_addChild(n18, n39);
		_addChild(n3, n52);
		
		insert(n21);
		insert(n7);
		insert(n23);
		insert(n24);
		_addChild(n26, n35);
		_addChild(n24, n46);
		_addChild(n24, n26);
		insert(n17);
		_addChild(n17, n30);
	}

	void _addChild(node* parent, node* child) {
		child->left = child->right = child;
		child->parent = parent;
		parent->degree++;
		parent->child = _merge(parent->child, child);
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

	void preorder() {
		printf("Heap: {\n");
		if (min == NULL) {
			printf("empty;\n}\n");
			return;
		}
		printf("minimum -> \"%d\";\n", min->value);
		node* c = min;
		do {
			listChildren(c);
			printf("------------------------------------\n");
			c = c->right;
		} while (c != min);
		printf("}\n");
	}

	void listChildren(node* n) {
		printf("\"%d\" degree-> \"%d\";  ", n->value, n->degree);
		printf("\"%d\" right-> \"%d\";\n", n->value, n->right->value);
		//printf("\"%d\" left-> \"%d\";\n", n->value, n->left->value);
		
		if (n->child) {
			node* c = n->child;
			do {
				printf("------------------------------------\n");
				printf("\"%d\" child-> \"%d\";\n", n->value, c->value);
				printf("------------------------------------\n");
				listChildren(c);
				c = c->right;
			} while (c != n->child);
			
		}
		
	}

	void _InorderTree(node*x)
	{
		if(x->child != NULL)
		{
			_InorderTree(x->child);
		}
		if (x->right != min)
		{
			_InorderTree(x->right);
		}
	}
};
void test1() {
	

	int i;
	
	FibonacciHeap x;
	x.firstEx();
	cout << "Before ExtractMin:\n";
	x.preorder();
	cout << "After ExtractMin:\n";
	x.Extract_Min();
	x.preorder();
	cin >> i;
}

int main()
{
	test1();
	return 0;
}

