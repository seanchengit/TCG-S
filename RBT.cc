//============================================================================
// Name        : RBT.cpp
// Author      : jessie
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include<stack>
#include "fplan.h"

#include "MyGraph.h"
using namespace std;
#define INFI 1000
#define MAX 100
#define BLACK 1
#define RED 0
#define TT 1
#define SS 0
//#define M 10



class bst {
private:

    struct Node {
        char value;
        bool color;
        Node *leftTree, *rightTree, *parent;
        int width, height;
        int X, Y, X_, Y_;//X,Y means leftdown corner, X_,Y_ means rightup corner

        Node() {
                    color = RED;
                    leftTree = NULL;
                    rightTree = NULL;
                    parent = NULL;
                    value = '0';
                    width = 0; height = 0;
                    X = 0; Y = 0; X_ = 0; Y_ = 0;
                    //cout<<"new red node initiate"<<endl;
                }

       /* Node(int w, int h, int x, int y) {
            color = RED;
            leftTree = NULL;
            rightTree = NULL;
            parent = NULL;
            value = '0';
            cout<<"new red node initiate"<<endl;
        }*/

        Node* grandparent() {
            if (parent == NULL) {
                return NULL;
            }
            return parent->parent;
        }

        Node* uncle() {
            if (grandparent() == NULL) {
                return NULL;
            }
            if (parent == grandparent()->rightTree)
                return grandparent()->leftTree;
            else
                return grandparent()->rightTree;
        }

        Node* sibling() {//brother tree
            if (parent->leftTree == this)
                return parent->rightTree;
            else
                return parent->leftTree;
        }
    };

    void rotate_right(Node *p) {
        Node *gp = p->grandparent();
        Node *fa = p->parent;
        Node *y = p->rightTree;

        fa->leftTree = y;

        if (y != NIL)
            y->parent = fa;
        p->rightTree = fa;
        fa->parent = p;

        if (root == fa)
            root = p;
        p->parent = gp;

        if (gp != NULL) {
            if (gp->leftTree == fa)
                gp->leftTree = p;
            else
                gp->rightTree = p;
        }

    }

    void rotate_left(Node *p) {
        if (p->parent == NULL) {
            root = p;
            return;
        }
        Node *gp = p->grandparent();
        Node *fa = p->parent;
        Node *y = p->leftTree;

        fa->rightTree = y;

        if (y != NIL)
            y->parent = fa;
        p->leftTree = fa;
        fa->parent = p;

        if (root == fa)
            root = p;
        p->parent = gp;

        if (gp != NULL) {
            if (gp->leftTree == fa)
                gp->leftTree = p;
            else
                gp->rightTree = p;
        }
    }

    void inorder(Node *p) {
        if (p == NIL)
        {
            cout << "NIL ";
        	return;
        }

        if (p->leftTree)
            inorder(p->leftTree);

        cout << p->value <<"("<<p->X<<", "<<p->Y<<")";

        if (p->rightTree)
            inorder(p->rightTree);
    }

    void traverseTree(Node *p, char data, int x_, int y_, stack<Node> *node)
        {
    		if (p == NIL)
				return;

			if (p->leftTree)
				traverseTree(p->leftTree, data, x_, y_, node);

			//cout << p->value << " ";

			if(fixed) node->push(*p);
				//traverseTree_Delete(p, data, x_, y_);

			if(p->value == data)	fixed = true;

			if (p->rightTree) traverseTree(p->rightTree, data, x_, y_, node);

        }

    void traverseTree_Delete(Node *p, char data, int x_, int y_)
    {

		if(hv == SS && p->X_ <= x_){
			delete_(p->value);
			//if(p->rightTree)
				//traverseTree_Delete(p->rightTree, data, x_, y_);
		}

		else if(hv == TT && p->Y_ <= y_){
			delete_(p->value);
			//if(p->rightTree)
				//traverseTree_Delete(p->rightTree, data, x_, y_);
		}
    }

    string outputColor(bool color) {
        return color ? "BLACK" : "RED";
    }

    Node* getSmallestChild(Node *p) {
        if (p->leftTree == NIL)
            return p;
        return getSmallestChild(p->leftTree);
    }

    Node *getLargestChild(Node *p)
    {
    	if (p->rightTree == NIL)
			return p;
		return getSmallestChild(p->rightTree);
    }

    void delete_child_(Node *p, char data) {//ccc
		if (p->leftTree)
			delete_child_(p->leftTree, data);

		if(p->value == data)
		{
			if (p->rightTree == NIL) {
				delete_one_child_(p);
				return;
			}
				Node *smallest = getSmallestChild(p->rightTree);
				swap(p->value, smallest->value);
				swap(p->X, smallest->X);
				swap(p->Y, smallest->Y);
				swap(p->X_, smallest->X_);
				swap(p->Y_, smallest->Y_);
				swap(p->width, smallest->width);
				swap(p->height, smallest->height);
				delete_one_child_(smallest);
				return;
		}
		if(p->rightTree)
			delete_child_(p->rightTree, data);
    }

    bool delete_child(Node *p, char data) {
        if (p->value > data) {
            if (p->leftTree == NIL) {
                return false;
            }
            return delete_child(p->leftTree, data);
        } else if (p->value < data) {
            if (p->rightTree == NIL) {
                return false;
            }
            return delete_child(p->rightTree, data);
        } else if (p->value == data) {
            if (p->rightTree == NIL) {
                delete_one_child(p);
                return true;
            }
            Node *smallest = getSmallestChild(p->rightTree);
            swap(p->value, smallest->value);
            //swap(p->value, smallest->value);
			swap(p->X, smallest->X);
			swap(p->Y, smallest->Y);
			swap(p->X_, smallest->X_);
			swap(p->Y_, smallest->Y_);
			swap(p->width, smallest->width);
			swap(p->height, smallest->height);
            delete_one_child(smallest);

            return true;
        }
    }

    void delete_one_child_(Node *p) {//ccc
            Node *child = p->leftTree == NIL ? p->rightTree : p->leftTree;
            if (p->parent == NULL && p->leftTree == NIL && p->rightTree == NIL) {
                p = NULL;
                root = p;
                return;
            }

            if (p->parent == NULL) {
                delete  p;
                child->parent = NULL;
                root = child;
                root->color = BLACK;
                return;
            }

            if (p->parent->leftTree == p) {
                p->parent->leftTree = child;
            } else {
                p->parent->rightTree = child;
            }
            child->parent = p->parent;

            if (p->color == BLACK) {
                if (child->color == RED) {
                    child->color = BLACK;
                } else
                    delete_case(child);
            }
            delete p;
        }

    void delete_one_child(Node *p) {
        Node *child = p->leftTree == NIL ? p->rightTree : p->leftTree;
        if (p->parent == NULL && p->leftTree == NIL && p->rightTree == NIL) {
            p = NULL;
            root = p;
            return;
        }

        if (p->parent == NULL) {
            delete  p;
            child->parent = NULL;
            root = child;
            root->color = BLACK;
            return;
        }

        if (p->parent->leftTree == p) {
            p->parent->leftTree = child;
        } else {
            p->parent->rightTree = child;
        }
        child->parent = p->parent;

        if (p->color == BLACK) {
            if (child->color == RED) {
                child->color = BLACK;
            } else
                delete_case(child);
        }
        delete p;
    }

    void delete_case(Node *p) {
        if (p->parent == NULL) {
            p->color = BLACK;
            return;
        }
        if (p->sibling()->color == RED) {
            p->parent->color = RED;
            p->sibling()->color = BLACK;
            if (p == p->parent->leftTree)
                rotate_left(p->sibling());
            else
                rotate_right(p->sibling());
        }
        if (p->parent->color == BLACK && p->sibling()->color == BLACK
                && p->sibling()->leftTree->color == BLACK && p->sibling()->rightTree->color == BLACK) {
            p->sibling()->color = RED;
            delete_case(p->parent);
        } else if (p->parent->color == RED && p->sibling()->color == BLACK
                && p->sibling()->leftTree->color == BLACK && p->sibling()->rightTree->color == BLACK) {
            p->sibling()->color = RED;
            p->parent->color = BLACK;
        } else {
            if (p->sibling()->color == BLACK) {
                if (p == p->parent->leftTree && p->sibling()->leftTree->color == RED
                        && p->sibling()->rightTree->color == BLACK) {
                    p->sibling()->color = RED;
                    p->sibling()->leftTree->color = BLACK;
                    rotate_right(p->sibling()->leftTree);
                } else if (p == p->parent->rightTree && p->sibling()->leftTree->color == BLACK
                        && p->sibling()->rightTree->color == RED) {
                    p->sibling()->color = RED;
                    p->sibling()->rightTree->color = BLACK;
                    rotate_left(p->sibling()->rightTree);
                }
            }
            p->sibling()->color = p->parent->color;
            p->parent->color = BLACK;
            if (p == p->parent->leftTree) {
                p->sibling()->rightTree->color = BLACK;
                rotate_left(p->sibling());
            } else {
                p->sibling()->leftTree->color = BLACK;
                rotate_right(p->sibling());
            }
        }
    }

    void insert(Node *p, char data) {
        if (p->value >= data) {
            if (p->leftTree != NIL)
                insert(p->leftTree, data);
            else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = p;
                p->leftTree = tmp;
                insert_case(tmp);
            }
        } else {
            if (p->rightTree != NIL)
                insert(p->rightTree, data);
            else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = p;
                p->rightTree = tmp;
                insert_case(tmp);
            }
        }

    }
/*
    void insert_rb(Node *p, char data, int w, int h, int X, int Y){//ccc
    	if(hv == S)
    	{
    		if(p->X_ <= X)
    		{
    			if(p->rightTree != NIL)	insert_rb(p->rightTree, data, w, h, X, Y);
    			else
    			{
    				Node *tmp = new Node();
    				char D[M];
    				int i=0;
    				while(i++<M)	D[i] = '\0';
    				tmp->value = data;
    				tmp->width = w; tmp->height = h;
    				tmp->X = X; tmp->Y = Y;
    				tmp->X_ = X + w; tmp->Y_ = Y + h;
    				tmp->leftTree = tmp->rightTree = NIL;
    				tmp->parent = p;
    				p->rightTree = tmp;
    				insert_case(tmp);
    				fixed = false;
    				traverseTime = 1;
					//while(traverseTime){
					traverseTree(data, tmp->X_, tmp->Y_, name);

    			}
    		}
    		else if(p->Y_ <= Y)
    		{
    			if(p->leftTree != NIL)	insert_rb(p->leftTree, data, w, h, X, Y);
				else
				{
					Node *tmp = new Node();
					tmp->value = data;
					tmp->width = w; tmp->height = h;
					tmp->X = X; tmp->Y = Y;
					tmp->X_ = X + w; tmp->Y_ = Y + h;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->leftTree = tmp;
					insert_case(tmp);
					fixed = false;
					traverseTime = 1;
					//while(traverseTime){
						traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTime--;
					//}
				}
    		}


    	}
    	else if(hv == T)
    	{
    		if(p->Y_ <= Y)
			{
				if(p->rightTree != NIL)	insert_rb(p->rightTree, data, w, h, X, Y);
				else
				{
					Node *tmp = new Node();
					tmp->value = data;
					tmp->width = w; tmp->height = h;
					tmp->X = X; tmp->Y = Y;
					tmp->X_ = X + w; tmp->Y_ = Y + h;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->rightTree = tmp;
					insert_case(tmp);
					fixed = false;
					traverseTime = 1;
					//while(traverseTime){
						traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTime--;
					//}
				}
			}
			else if(p->X_ <= X)
			{
				if(p->leftTree != NIL)	insert_rb(p->leftTree, data, w, h, X, Y);
				else
				{
					Node *tmp = new Node();
					tmp->value = data;
					tmp->width = w; tmp->height = h;
					tmp->X = X; tmp->Y = Y;
					tmp->X_ = X + w; tmp->Y_ = Y + h;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->leftTree = tmp;
					insert_case(tmp);
					fixed = false;
					traverseTime = 1;
					//while(traverseTime){
						traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTree(data, tmp->X_, tmp->Y_);
						//traverseTime--;
					//}
				}
			}

    	}

    }
*/
void insert_rb_(Node *p, Module_Info *m, matrixgraph *Ch){
	if(hv == SS)
	{
		if(p->value == 's')//first
		{
			m->x = p->X + p->width;//update every time
			if(p->rightTree != NIL)	insert_rb_(p->rightTree, m, Ch);
			else{
				Node *tmp = new Node();
				tmp->value = m->name;
				tmp->width = m->width; tmp->height = m->height;
				tmp->X = m->x; tmp->Y = m->y;
				m->rx = tmp->X_ = m->x + m->width;
				m->ry = tmp->Y_ =m->y + m->height;

				tmp->leftTree = tmp->rightTree = NIL;
				tmp->parent = p;
				p->rightTree = tmp;
				insert_case(tmp);
				fixed = false;
				stack<Node> node;//store node need to be delete
				traverseTree(m->name, tmp->X_, tmp->Y_, &node);
				while(!node.empty())
				{
					traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
					node.pop();
				}
				return;
			}
		}
		else
		{
			if(getRelation(Ch, getNum(p->value), getNum(m->name)))
			{
				m->x = p->X + p->width;//ccc
				if(p->rightTree != NIL)  insert_rb_(p->rightTree, m, Ch);
				else
				{
					Node *tmp = new Node();
					tmp->value = m->name;
					tmp->width = m->width; tmp->height = m->height;
					tmp->X = m->x; tmp->Y = m->y;
					m->rx = tmp->X_ = m->x + m->width;
					m->ry = tmp->Y_ = m->y + m->height;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->rightTree = tmp;
					insert_case(tmp);
					fixed = false;
					stack<Node> node;
					traverseTree(m->name, tmp->X_, tmp->Y_, &node);
					while(!node.empty())
					{
						traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
						node.pop();
					}
					return;
				}
			}
			else
			{
				m->y=p->Y + p->height;
				if(p->leftTree != NIL)  insert_rb_(p->leftTree, m, Ch);
				else
				{
					Node *tmp = new Node();
					tmp->value = m->name;
					tmp->width = m->width; tmp->height = m->height;
					tmp->X = m->x; tmp->Y = m->y;
					m->rx = tmp->X_ = m->x + m->width;
					m->ry = tmp->Y_ = m->y + m->height;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->leftTree = tmp;
					insert_case(tmp);
					fixed = false;
					stack<Node> node;
					traverseTree(m->name, tmp->X_, tmp->Y_, &node);
					while(!node.empty())
					{
						traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
						node.pop();
					}
					return;
				}
			}
		}
	}

	if(hv == TT)
	{
		if(p->value == 't')//first
		{
			m->y = p->Y + p->height;//update every time
			if(p->rightTree != NIL)	insert_rb_(p->rightTree, m, Ch);
			else{
				Node *tmp = new Node();
				tmp->value = m->name;
				tmp->width = m->width; tmp->height = m->height;
				tmp->X = m->x; tmp->Y = m->y;
				m->rx = tmp->X_ = m->x + m->width;
				m->ry = tmp->Y_ =m->y + m->height;
				tmp->leftTree = tmp->rightTree = NIL;
				tmp->parent = p;
				p->rightTree = tmp;
				insert_case(tmp);
				fixed = false;
				stack<Node> node;
				traverseTree(m->name, tmp->X_, tmp->Y_, &node);
				while(!node.empty())
				{
					traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
					node.pop();
				}
				return;
			}
		}
		else
		{
			if(getRelation(Ch, getNum(p->value), getNum(m->name)))
			{
				m->x=p->X+p->width;
				if(p->leftTree != NIL)  insert_rb_(p->leftTree, m, Ch);
				else
				{
					Node *tmp = new Node();
					tmp->value = m->name;
					tmp->width = m->width; tmp->height = m->height;
					tmp->X = m->x; tmp->Y = m->y;
					m->rx = tmp->X_ = m->x + m->width;
					m->ry = tmp->Y_ = m->y + m->height;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->leftTree = tmp;
					insert_case(tmp);
					fixed = false;
					stack<Node> node;
					traverseTree(m->name, tmp->X_, tmp->Y_, &node);
					while(!node.empty())
					{
						traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
						node.pop();
					}
					return;
				}
			}
			else
			{
				m->y=p->Y + p->height;
				if(p->rightTree != NIL)  insert_rb_(p->rightTree, m, Ch);
				else
				{
					Node *tmp = new Node();
					tmp->value = m->name;
					tmp->width = m->width; tmp->height = m->height;
					tmp->X = m->x; tmp->Y = m->y;
					m->rx = tmp->X_ = m->x + m->width;
					m->ry = tmp->Y_ = m->y + m->height;
					tmp->leftTree = tmp->rightTree = NIL;
					tmp->parent = p;
					p->rightTree = tmp;
					insert_case(tmp);
					fixed = false;
					stack<Node> node;
					traverseTree(m->name, tmp->X_, tmp->Y_, &node);
					while(!node.empty())
					{
						traverseTree_Delete(&(node.top()), m->name, tmp->X_, tmp->Y_);
						node.pop();
					}
					return;
				}
			}
		}
	}

}

    void insert_case(Node *p) {
        if (p->parent == NULL) {
            root = p;
            p->color = BLACK;
            return;
        }
        if (p->parent->color == RED) {
            if (p->uncle()->color == RED) {
                p->parent->color = p->uncle()->color = BLACK;
                p->grandparent()->color = RED;
                insert_case(p->grandparent());
            } else {
                if (p->parent->rightTree == p && p->grandparent()->leftTree == p->parent) {
                    rotate_left(p);
                    rotate_right(p);
                    p->color = BLACK;
                    p->leftTree->color = p->rightTree->color = RED;
                } else if (p->parent->leftTree == p && p->grandparent()->rightTree == p->parent) {
                    rotate_right(p);
                    rotate_left(p);
                    p->color = BLACK;
                    p->leftTree->color = p->rightTree->color = RED;
                } else if (p->parent->leftTree == p && p->grandparent()->leftTree == p->parent) {
                    p->parent->color = BLACK;
                    p->grandparent()->color = RED;
                    rotate_right(p->parent);
                } else if (p->parent->rightTree == p && p->grandparent()->rightTree == p->parent) {
                    p->parent->color = BLACK;
                    p->grandparent()->color = RED;
                    rotate_left(p->parent);
                }
            }
        }
    }

    void DeleteTree(Node *p) {
        if (!p || p == NIL) {
            return;
        }
        DeleteTree(p->leftTree);
        DeleteTree(p->rightTree);
        delete p;
    }
public:

    bst() {
        NIL = new Node();
        NIL->color = BLACK;
        root = NULL;
        fixed = false;
        //hv = S;
    }

    ~bst() {
        if (root)
            DeleteTree(root);
        delete NIL;
    }

    void inorder() {
        if (root == NULL)
            return;
        inorder(root);
        cout << endl;
    }

    /*Node* getNode(char data)
    {
    	if(root == NULL)
    		return NIL;
    	getNode(root, data);
    }
*/
 /*bool getNode(Node *T, char data, Node &p){

      if(!T) {
        p=f;
        return false;
      }
      else if (data== T->value) {
        p=T;
        return true;
      }
      else if (data < T->value)
        return getNode(T->leftChild, data, T, p);
      else
        return getNode(T->rightChild, data, T, p);
    }*/
/*
    void insert(char x, int w, int h, int X, int Y) {
        if (root == NULL) {
            root = new Node();
            root->color = BLACK;
            root->leftTree = root->rightTree = NIL;
            root->value = x;
            if(x == 's')
            {
            	hv = S;
            	root->X = root->Y = 0;
            	root->X_ = 0; root->Y_ = INFI;
            }
            else if(x == 't')
            {
            	hv = T;
            	root->X = root->Y = 0;
            	root->X_ = INFI; root->Y_ = 0;
            }
        } else {
        	insert_rb(root, x, w, h, X, Y);
        }

    }
*/

    void insertHead(char x, int w, int h, matrixgraph *Ch){
        		if (root == NULL) {
                   root = new Node();
                   root->color = BLACK;
                   root->leftTree = root->rightTree = NIL;
                   root->value = x;
                   if(root->value == 's')
                   {
                   	hv = SS;
                   	root->X = root->Y = 0;
                   	root->X_ = 0; root->Y_ = INFI;
                   }
                   else if(root->value == 't')
                   {
                   	hv = TT;
                   	root->X = root->Y = 0;
                   	root->X_ = INFI; root->Y_ = 0;
                   }
               } /*else {
               	insert_rb_(root, x, w, h, 0, 0, Ch);
               }*/
           }

    void insert_(module *m, matrixgraph *Ch){
    		if (root == NULL) {
               root = new Node();
               root->color = BLACK;
               root->leftTree = root->rightTree = NIL;
               root->value = m->name;
               if(root->value == 's')
               {
               	hv = SS;
               	root->X = root->Y = 0;
               	root->X_ = 0; root->Y_ = INFI;
               }
               else if(root->value == 't')
               {
               	hv = TT;
               	root->X = root->Y = 0;
               	root->X_ = INFI; root->Y_ = 0;
               }
           } else {
           	insert_rb_(root, m, Ch);
           }
       }

void moduleMerge(vector<module> *Gamma, matrixgraph *ch1, matrixgraph *ch2)
{
	addModule(ch1, ch2);
	int i = 0;
	//insertHead('t', 0, 0, ch1);//starting construct t tree.
	while(i<Gamma->size())
	{
		insert_(&(Gamma->at(i)), ch1);
		i++;
	}
}

void moduleMerge(queue<module> *gamma, matrixgraph *ch1, matrixgraph *ch2)
{
	addModule(ch1, ch2);
	//insertHead('t', 0, 0, ch1);//starting construct t tree.
	while(!gamma->empty())
	{
		insert_(&(gamma->front()), ch1);
		gamma->pop();
	}
}


    void traverseTree(char data, int x_, int y_, stack<Node> *node)
    {
    	if(root == NULL)
    		return;
    	else
    		//while(traverseTime){
    		traverseTree(root, data, x_, y_, node);

    		//}
    }

    void delete_(char data) {
    		if(root == NULL)
    			return;
            delete_child_(root, data);
        }

    bool delete_value(char data) {
        return delete_child(root, data);
    }
private:
    Node *root, *NIL;
    bool hv,fixed;
    int traverseTime;
};
