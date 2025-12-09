#include <bits/stdc++.h>
using namespace std;


struct Node {
    int id;
    int pos;
    bool red;
    Node *l, *r, *p;
    Node(int _id, int _pos){
        id = _id;
        pos = _pos;
        red = true;
        l = r = p = NULL;
    }
};

Node *root = NULL;

bool isRed(Node *n) {
    return n != NULL && n->red;
}

void rotateLeft(Node* &root, Node* x) {
    Node *y = x->r;
    x->r = y->l;
    if(y->l) y->l->p = x;
    y->p = x->p;
    if(!x->p) root = y;
    else if(x == x->p->l) x->p->l = y;
    else x->p->r = y;
    y->l = x;
    x->p = y;
}

void rotateRight(Node* &root, Node* y) {
    Node *x = y->l;
    y->l = x->r;
    if(x->r) x->r->p = y;
    x->p = y->p;
    if(!y->p) root = x;
    else if(y == y->p->l) y->p->l = x;
    else y->p->r = x;
    x->r = y;
    y->p = x;
}

void fixInsert(Node* &root, Node* z) {
    while(z->p && z->p->red) {
        Node* gp = z->p->p;
        if(z->p == gp->l) {
            Node* y = gp->r;
            if(isRed(y)) {
                z->p->red = y->red = false;
                gp->red = true;
                z = gp;
            } else {
                if(z == z->p->r) {
                    z = z->p;
                    rotateLeft(root, z);
                }
                z->p->red = false;
                gp->red = true;
                rotateRight(root, gp);
            }
        } else {
            Node* y = gp->l;
            if(isRed(y)) {
                z->p->red = y->red = false;
                gp->red = true;
                z = gp;
            } else {
                if(z == z->p->l) {
                    z = z->p;
                    rotateRight(root, z);
                }
                z->p->red = false;
                gp->red = true;
                rotateLeft(root, gp);
            }
        }
    }
    root->red = false;
}

Node* insertAircraft(Node* &root, int id, int pos) {
    Node *z = new Node(id, pos);
    Node *y = NULL, *x = root;

    while(x) {
        y = x;
        if(pos < x->pos) x = x->l;
        else x = x->r;
    }
    z->p = y;
    if(!y) root = z;
    else if(pos < y->pos) y->l = z;
    else y->r = z;

    fixInsert(root, z);

    return z;
}


void checkCollision(Node* root, Node* z) {
    int threshold = 50;
    Node *curr = root, *prev = NULL;

    
    while(curr) {
        prev = curr;
        if(z->pos < curr->pos) curr = curr->l;
        else curr = curr->r;
    }

  
    vector<Node*> checkList;
    if(z->p) checkList.push_back(z->p);
    if(z->l) checkList.push_back(z->l);
    if(z->r) checkList.push_back(z->r);

    bool alert = false;
    for(auto c : checkList) {
        if(abs(c->pos - z->pos) <= threshold) {
            cout << " ALERT! Aircraft " << z->id
                 << " near Aircraft " << c->id << "\n";
            alert = true;
        }
    }
    if(!alert)
        cout << "✓ Aircraft " << z->id << " Safe\n";
}

int main() {
    int n;
    cout << "Enter number of aircraft: ";
    cin >> n;

    for(int i = 0; i < n; i++){
        int id, pos;
        cout << "Enter Aircraft ID & Position: ";
        cin >> id >> pos;
        Node* newA = insertAircraft(root, id, pos);
        checkCollision(root, newA);
    }
    return 0;
}
