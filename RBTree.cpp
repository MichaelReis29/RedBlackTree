// Michael Reis
#include <iostream>
#include <cstdlib>

using namespace std;
template <typename keytype, typename valuetype> 
class RBTree {
    private:
        enum Color {
            red, 
            black
        };
        struct Node {
            Color color;
            keytype key;
            valuetype value;
            Node *p;
            Node *left;
            Node *right; 
            int size;
        };
        Node *root;
        Node *nil;
        int treeSize;
        
        void deleteRBTree(Node *x) {
            if (x == nil) {
                return;
            }
            deleteRBTree(x->left);
            deleteRBTree(x->right);
            delete x;
        }

        keytype recurseSelect(Node *x, int i) {
            int r = x->left->size + 1;
            if (i == r) {
                return x->key;
            }
            else if (i < r) {
                return recurseSelect(x->left, i);
            }
            else {
                return recurseSelect(x->right, i - r);
            }
        }
        void recursePreorder(Node *node) {
            if (node == nil) {
                return;
            }
            cout << node->key << " ";
            recursePreorder(node->left);
            recursePreorder(node->right);
        }

        void recurseInorder(Node *node) {
            if (node == nil) {
                return;
            }
            recurseInorder(node->left);
            cout << node->key << " ";
            recurseInorder(node->right);
        }

         void recursePostorder(Node *node) {
            if (node == nil) {
                return;
            }
            recursePostorder(node->left);
            recursePostorder(node->right);
            cout << node->key << " ";
        }
        Node *treeMinimum(Node *node) {
            Node *x = node;
            while (x->left != nil) {
                x = x->left;
            }
            return x;
        }
        Node* treeMaximum(Node* node) {
            Node *x = node;
            while (x->right != nil) {
                x = x->right;
            }
            return x;
        }   
        
        void transplant(Node *u, Node *v) {
            if (u->p == nil) {
                root = v;
            }
            else if (u == u->p->left) {
                u->p->left = v;
            }
            else {
                u->p->right = v;
            }
            v->p = u->p;
        }
        void deleteFixup(Node *x) {
            Node *w;
            while (x != root && x->color == black) {
                if (x == x->p->left) {
                    w = x->p->right;
                    if (w->color == red) {
                        w->color = black;
                        x->p->color = red;
                        leftRotate(x->p);
                        w = x->p->right;
                    }
                    if (w->left->color == black && w->right->color == black) {
                        w->color = red;
                        x = x->p;
                    }
                    else {
                        if (w->right->color == black) {
                            w->left->color = black;
                            w->color = red;
                            rightRotate(w);
                            w = x->p->right;
                        }
                        w->color = x->p->color;
                        x->p->color = black;
                        w->right->color = black;
                        leftRotate(x->p);
                        x = root;
                    }
                }
                else  {
                    w = x->p->left;
                    if (w->color == red) {
                        w->color = black;
                        x->p->color = red;
                        rightRotate(x->p);
                        w = x->p->left;
                    }
                    if (w->right->color == black && w->left->color == black) {
                        w->color = red;
                        x = x->p;
                    }
                    else {
                        if (w->left->color == black) {
                            w->right->color = black;
                            w->color = red;
                            leftRotate(w);
                            w = x->p->left;
                        }
                        w->color = x->p->color;
                        x->p->color = black;
                        w->left->color = black;
                        rightRotate(x->p);
                        x = root;
                    }
                }
            }
            x->color = black;
        }
        void leftRotate(Node *x) {
            Node *y = x->right;
            x->right = y->left;
            if (y->left != nil) {
                y->left->p = x;
            }
            y->p = x->p;
            if (x->p == nil) {
                root = y;
            }
            else if (x == x->p->left) {
                x->p->left = y;
            }
            else {
                x->p->right = y;
            }
            y->left = x;
            x->p = y;
            y->size = x->size;
            x->size = x->left->size + x->right->size + 1;
        }

        void rightRotate(Node *x) {
            Node *y = x->left;
            x->left = y->right;
            if (y->right != nil) {
                y->right->p = x;
            }
            y->p = x->p;
            if (x->p == nil) {
                root = y;
            }
            else if (x == x->p->right) {
                x->p->right = y;
            }
            else {
                x->p->left = y;
            }
            y->right = x;
            x->p = y;
            y->size = x->size;
            x->size = x->left->size + x->right->size + 1;
        }

        void insertFixup(Node *z) {
            while (z->p->color == red) {
                if (z->p == z->p->p->left) {
                    Node *y = z->p->p->right;
                    if (y->color == red) {
                        z->p->color = black;
                        y->color = black;
                        z->p->p->color = red;
                        z = z->p->p;
                    }
                    else {
                        if (z == z->p->right) {
                            z = z->p;
                            leftRotate(z);
                        }
                        z->p->color = black;
                        z->p->p->color = red;
                        rightRotate(z->p->p);
                    }
                }
                else {
                    Node *y = z->p->p->left;
                    if (y->color == red) {
                        z->p->color = black;
                        y->color = black;
                        z->p->p->color = red;
                        z = z->p->p;
                    }
                    else {
                        if (z == z->p->left) {
                            z = z->p;
                            rightRotate(z);
                        }
                        z->p->color = black;
                        z->p->p->color = red;
                        leftRotate(z->p->p);
                    }
                }
            }
            root->color = black;
        }

        void deepCopy(const RBTree &src) {
            nil = new Node;
            nil->color = black;
            nil->p = 0;
            nil->left = 0;
            nil->right = 0;
            nil->size = 0;
            treeSize = src.treeSize;
            root = recurseCopyRBTree(src.root, src.nil, src.nil);
        }

        Node * recurseCopyRBTree(Node *root, Node *p, Node *oNil) {
            if (root == oNil) {
                return nil;
            }
            Node *z = new Node;
            z->key = root->key;
            z->value = root->value;
            z->color = root->color;
            z->size = root->size;
            z->p = (p = oNil) ? nil : p;

            z->left = recurseCopyRBTree(root->left, z, oNil);
            z->right = recurseCopyRBTree(root->right, z, oNil);

            return z;
        }

    public:
    RBTree() {
        nil = new Node;
        nil->color = black;
        nil->p = 0;
        nil->left = 0;
        nil->right = 0;
        nil->size = 0;
        root = nil;
        treeSize = 0;

    }
    RBTree(keytype k[], valuetype V[], int s) {
        treeSize = 0;
        nil = new Node;
        nil->color = black;
        nil->p = NULL;
        nil->left = NULL;
        nil->right = NULL;
        nil->size = 0;
        root = nil;
        for (int i = 0; i < s; i++) {
            insert(k[i], V[i]);
        }
    }

    ~RBTree() {
        deleteRBTree(root);
        delete nil;
    }

    RBTree<keytype, valuetype>& operator=(const RBTree<keytype, valuetype> &src) {
        if (this != &src) {
            deleteRBTree(root);
            deepCopy(src);
        }
        return *this;


    }
    RBTree(const RBTree<keytype, valuetype> &src) {
        deepCopy(src);
    }

    void insert(keytype k, valuetype v) {
        Node *z = new Node;
        z->key = k;
        z->value = v;
        z->size = 1;
        Node *y = nil;
        Node *x = root;

        while (x != nil) {
            y = x;
            x->size++;
            if (z->key < x->key) {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }
        z->p = y;
        if (y == nil) {
            root = z;
        }
        else if (z->key < y->key) {
            y->left = z;
        }
        else {
            y->right = z;
        }
        z->left = nil;
        z->right = nil;
        z->color = red;
        insertFixup(z);
        treeSize++;
    }




    valuetype * search(keytype k) {
        Node *temp = root;
        while (temp != nil) {
            if (temp->key == k) {
                return &temp->value;
            }
            if (temp->key < k) {
                temp = temp->right;
            }
            else if (temp->key > k) {
                temp = temp->left;
            }
        }
        return NULL;
    }
    int remove(keytype k) {
        Node *z = root;
        while (z != nil) {
            if (z->key == k) {
                break;
            }
            if (z->key < k) {
                z = z->right;
            }
            else if (z->key > k) {
                z = z->left;
            }
        }
        
        if (z == nil) {
            return 0;
        } 
        Node *s=z->p;
        
        while (s != nil) {
            
            s->size--;
            s = s->p;
        }
        Node *y = z;
        z = y;
        Node *x;
        Color yOrigColor = y->color;
        if (z->left == nil) {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == nil) {
            x = z->left;
            transplant(z, z->left);
        }
        else {
            y = treeMaximum(z->left);
            yOrigColor = y->color;
            x = y->left;
            if (y->p == z) {
                x->p = y;
            }
            else {
                transplant(y, y->left);
                y->left = z->left;
                y->left->p = y;

                Node *s = x->p;
                while (s != nil && s != y) {
                    s->size--;
                    s = s->p;
                }
            }
            transplant(z, y);
            y->right = z->right;
            y->right->p = y;
            y->color = z->color;  
            y->size = y->left->size + y->right->size + 1;
        }
        if (yOrigColor == black) {
            deleteFixup(x);
        }
        treeSize--;
        return 1;
    }
    int rank(keytype k) {
        Node *x = root;
        while (x != nil) {
            if (x->key == k) {
                break;
            }
            if (x->key < k) {
                x = x->right;
            }
            else if (x->key > k) {
                x = x->left;
            }
        }
        if (x == nil) {
            return 0;
        }
        int r = x->left->size + 1;
        Node *y = x;
        while (y != root) {
            if (y == y->p->right) {
                r += y->p->left->size + 1;
            }
            y = y->p;
        }
        return r;
    }
    keytype select(int pos) {
        return recurseSelect(root, pos);
    }
    keytype *successor(keytype k) {
        Node *z = root;
        while (z != nil) {
            if (z->key == k) {
                break;
            }
            if (z->key < k) {
                z = z->right;
            }
            else if (z->key > k) {
                z = z->left;
            }
        }
        if (z == nil) {
            return NULL;
        }
       if (z->right != nil) {
            Node *x = z->right;
            while (x->left != nil) {
                x = x->left;
            }
            return &x->key;
       }
       Node *y = z->p;
       while (y != nil &&  z == y->right) {
           z = y;
           y = y->p;
       }
       if (y == nil) {
           return NULL;
       }
       else {
           return &y->key;
       }
    }
    keytype *predecessor(keytype k) {
        Node *z = root;
        while (z != nil) {
            if (z->key == k) {
                break;
            }
            if (z->key < k) {
                z = z->right;
            }
            else if (z->key > k) {
                z = z->left;
            }
        }
        if (z == nil) {
            return NULL;
        }
       if (z->left!= nil) {
            Node *x = z->left;
            while (x->right != nil) {
                x = x->right;
            }
            return &x->key;
       }
       Node *y = z->p;
       while (y != nil &&  z == y->left) {
           z = y;
           y = y->p;
       }
       if (y == nil) {
           return NULL;
       }
       else {
           return &y->key;
       }
    }
    int size() {
        return treeSize;
    }
    void preorder() {
        recursePreorder(root);
        cout << endl;
    }
    void inorder() {
        recurseInorder(root);
        cout << endl;
    }
    void postorder() {
        recursePostorder(root);
        cout << endl;
    }
    void printk(int k) {
       for (int i = 1; i <= k; i++) {
           cout << recurseSelect(root, i) << " ";
       }
       cout << endl;
    }
} ;
