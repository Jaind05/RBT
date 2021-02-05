// Dhruv Jain
// 2/4/21
// 1 0 Binary tree




#include <iostream>
#include <cstring>
#include <fstream>
#include <string.h>
#include <math.h>
#include <iomanip>
#include <stdlib.h>



using namespace std;

struct node{
  node* parent;
  int value;
  bool color; // false = 0 = black, true = 1 = red 
  node *left;
  node *right;
};

void printnodes(node* ,int);
node* addnode(node*, node *);
void addnoderecursive(node*, node *);
void addrepairtree(node *);
void deleter(node*);
node* getparent(node*);
node* getsibling(node* );
node* getgrandparent(node* );
node* getuncle(node* );
void rotateleft(node* );
void rotateright(node* );
void deleterecursive(node*);
node *search_node(int);

int main(){
  node *head = NULL;
  node *tempnode = NULL;
  cout << "would you like to read in from a file?" << endl;
  char file[30];
  cin >> file;
  int filereads[100];
  
  if(strcmp(file, "yes") == 0){
    srand (time(NULL));
    cout << "How many nodes would you like?" << endl;
    int numberofstudents = 0;
    cin >> numberofstudents;
    
    streampos size;
    char * memblock;
    char * pch;
    ifstream file ("example.txt", ios::in|ios::binary|ios::ate);
    if (file.is_open()) {
      size = file.tellg();
      memblock = new char [size];
      file.seekg (0, ios::beg);
      file.read (memblock, size);
      file.close();
      
      //  cout << "the entire file content is in memory" << endl;
    }
    pch = strtok (memblock," ,.-");
    int i = 0;
    while (pch != NULL) {
      //cout << pch << endl;
      filereads[i] = atoi(pch);
      pch = strtok (NULL, " ,.-");
      //cout << filereads[i] << endl;
      i++;
    }
    //cout << firstname[numberofstudents] << endl;
    delete[] memblock;
    head = new node;
    head->value = filereads[0];
    head->left = NULL;
    head->right = NULL;
    head->color = false; //0
    head->parent = NULL;

    for (int i = 1; i < numberofstudents; i++){
      if(filereads[i]!=0){
	      cout << filereads[i] << endl;
        tempnode = new node;
        tempnode->value = filereads[i];
        addnode(head, tempnode);
      }
    }
  }
   
  char input[30];
  bool stillrunning = true;
  while (stillrunning){
    if(head == NULL){
      cout << "what value would you like to give to the first node" << endl;
      cout << endl;
      head = new node;
      cin >> head->value;
      head->parent = NULL;
      head->left = NULL;
      head->right = NULL;
      head->color = false; //0
    }
    cout << "would you like to add, print, delete, or quit" << endl;
    cin >> input;
    if(strcmp(input, "add") == 0){
      int temp = 0;
      cout << "What value would you like to put in your new node?" << endl;
      cin >> temp;
      tempnode = new node;
      tempnode->value = temp;
      addnode(head, tempnode);
    }
    else if(strcmp(input, "print") == 0){
      printnodes(head, 0);
    }
    else if(strcmp(input, "delete") == 0){
      int temp = 0;
      cout << "What is the value of the node you would like to delete" << endl; 
      cin >> temp;
      //tempnode = new node;
      tempnode = search_node(temp);
      deleter(tempnode);
    }
    else if(strcmp(input, "quit") == 0){
      stillrunning = false;
    }
  }
}

void printnodes(node *head, int count){
  if(head==NULL){
    return;
  }
  
  count += 7;
  printnodes(head->right, count);
  cout << endl;
  for (int i = 7; i < count; i++){
  cout << "  ";
  }
  cout << head->value << " " << head->color << endl;   // 0 = blackk and 1 = red
  printnodes(head->left, count);
}

node* addnode(node* root, node* n) {
  // Insert new node into the current tree.
  addnoderecursive(root, n);

  // Repair the tree in case any of the 1-0 properties have been violated.
  addrepairtree(n);

  // Find the new root to return.
  root = n;
  while (getparent(root) != NULL) {
    root = getparent(root);
  }
  return root;
}

void addnoderecursive(node *head, node *n){
  if((head->left!=NULL)&&(head->value > n->value)){
    addnode(head->left, n);
  }
  else if((head->right!=NULL)&&(head->value <= n->value)){
    addnode(head->right, n);
  }
  
  if((head->left == NULL)&&(head->value > n-> value)){
    cout << "adding new node on the left"<< endl;
    head->left = n;
  }
  if((head->right==NULL)&&(head->value <= n-> value)){
    cout << "adding new node on the right"<< endl;
    head->right = n;
  }
  n->parent = head;
  n->left = NULL;
  n->right = NULL; 
  n->color = true; //1 = 1
  return;
}

void addrepairtree(node* n) {
  if (getparent(n) == NULL) {
    n->color = 0;  //0
  } else if (getparent(n)->color == 0) {
     return;
  } else if (getuncle(n) != NULL && getuncle(n)->color == 1) {  //1
    getparent(n)->color = 0;  //0
    getuncle(n)->color = 0; //0
    getgrandparent(n)->color = 1; //1
    addrepairtree(getgrandparent(n));
  } else {
    node* p = getparent(n);
    node* g = getgrandparent(n);
    if (n == p->right && p == g->left) {
      rotateleft(p);
      n = n->left;
    } else if (n == p->left && p == g->right) {
      rotateright(p);
      n = n->right;
    }
   
    if (n == p->left) {
      rotateright(g);
    } else {
      rotateleft(g);
    }
    p->color = 0; //0
    g->color = 1; //1
  }
  return;
}

node* getparent(node* n) {
  // parent is null for the root node.
  if (n == NULL)
    return NULL;
  else
    return n->parent;
}

node* getgrandparent(node* n) {
  // returns NULL if this is root or child of root
  return getparent(getparent(n));
}

node* getsibling(node* n) {
  node* p = getparent(n);

  // No parent
  if (p == NULL) {
    return NULL;
  }

  if (n == p->left) {
    return p->right;
  } else {
    return p->left;
  }
}

node* getuncle(node* n) {
  node* p = getparent(n);
  // No parent means no uncle
  return getsibling(p);
}

void rotateleft(node* n) {
  node* nnew = n->right;
  node* p = getparent(n);
  if(nnew == NULL) return;  // Since the leaves of a 1-0 tree are empty they cannot become internal nodes.
  n->right = nnew->left;
  nnew->left = n;
  n->parent = nnew;
  // Handle other child/parent pointers.
  if (n->right != NULL) {
    n->right->parent = n;
  }

  // Initially n could be the root.
  if (p != NULL) {
    if (n == p->left) {
      p->left = nnew;
    } else if (n == p->right) {
      p->right = nnew;
    }
  }
  nnew->parent = p;
  return;
}

void rotateright(node* n) {
  node* nnew = n->left;
  node* p = getparent(n);
  if(nnew == NULL)return;  // Since the leaves of a 1-0 tree are empty,
                            // they cannot become internal nodes.

  n->left = nnew->right;
  nnew->right = n;
  n->parent = nnew;

  // Handle other child/parent pointers.
  if (n->left != NULL) {
    n->left->parent = n;
  }

  // Initially n could be the root.
  if (p != NULL) {
    if (n == p->left) {
      p->left = nnew;
    } else if (n == p->right) {
      p->right = nnew;
    }
  }
  nnew->parent = p;
  return;
}

void replacenode(node* n, node* child) {
  child->parent = n->parent;
  if (n == n->parent->left) {
    n->parent->left = child;
  } else {
    n->parent->right = child;
  }
  return;
}

node * search_node(int value){
  node * temp;
  //Todo find teh node by walking through RBT */
  return temp;
}

void deleter(node* n) {
  // Precondition: n has at most one non-leaf child.
  node* child = (n->right == NULL) ? n->left : n->right;
  if(child == NULL)return;

  replacenode(n, child);
  if (n->color == 0) {
    if (child->color == 1) {
      child->color = 0; //BLACK
    } else {
      deleterecursive(child);
    }
  }
  delete(n);
  return;
}

void deleterecursive(node *n){
//Case 1
  if (n->parent != NULL) {
    node* s = getsibling(n);

    if (s->color == 1) {
      n->parent->color = 1;
      s->color = 0;
      if (n == n->parent->left) {
        rotateleft(n->parent);
      } else {
        rotateright(n->parent);
      }
    }
    if ((n->parent->color == 0) && (s->color == 0) &&
        (s->left->color == 0) && (s->right->color == 0)) {
      s->color = 1;
      //call the first Case recursively
      deleterecursive(n->parent);
    } else {
      if ((n->parent->color == 1) && (s->color == 0) &&
          (s->left->color == 0) && (s->right->color == 0)) {
        s->color = 1;
        n->parent->color = 0;
      } else {
        // This if statement is unecissary, due to case 2 (even though case 2 changed
        // the sibling to a sibling's child, the sibling's child can't be 1, since
        // no 1 parent can have a 1 child).
      if (s->color == 0) {
        // The following statements just force the 1 to be on the left of the
        // left of the parent, or right of the right, so case six will rotate
          // correctly.
          if ((n == n->parent->left) && (s->right->color == 0) &&
          (s->left->color == 1)) {
            // This last test is uneccessary too due to cases 2-4.
            s->color = 1;
            s->left->color = 0;
            rotateright(s);
        } else if ((n == n->parent->right) && (s->left->color == 0) &&
          (s->right->color == 1)) {
            // This last test is unecessary too due to cases 2-4.
            s->color = 1;
            s->right->color = 0;
            rotateleft(s);
          }
        }
        s->color = n->parent->color;
        n->parent->color = 0;

        if (n == n->parent->left) {
          s->right->color = 0;
          rotateleft(n->parent);
        } else {
          s->left->color = 0;
          rotateright(n->parent);
        }
      }
    }
  }
  return;
}
