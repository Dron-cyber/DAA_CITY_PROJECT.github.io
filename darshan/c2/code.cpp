#include <bits/stdc++.h>
using namespace std;


struct Node {
    int angle, intensity, height;
    Node *left, *right;
    Node(int a, int i) : angle(a), intensity(i), height(1), left(NULL), right(NULL) {}
};

int getHeight(Node* n) {
    return n ? n->height : 0;
}

int getBalance(Node* n) {
    return getHeight(n->left) - getHeight(n->right);
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;
    x->right = y;
    y->left = T2;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;
    y->left = x;
    x->right = T2;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

Node* insertAVL(Node* root, int angle, int intensity) {
    if (!root) return new Node(angle, intensity);

    if (intensity < root->intensity)
        root->left = insertAVL(root->left, angle, intensity);
    else
        root->right = insertAVL(root->right, angle, intensity);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && intensity < root->left->intensity) return rotateRight(root);
    if (balance < -1 && intensity > root->right->intensity) return rotateLeft(root);
    if (balance > 1 && intensity > root->left->intensity) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (balance < -1 && intensity < root->right->intensity) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

Node* findMax(Node* root) {
    if (!root) return NULL;
    while (root->right) root = root->right;
    return root;
}

int main() {
    Node* root = NULL;
    int n;
    cout << "Enter number of angle readings: ";
    cin >> n;

    cout << "Enter <angle intensity> pairs:\n";
    for (int i = 0; i < n; i++) {
        int angle, intensity;
        cin >> angle >> intensity;
        root = insertAVL(root, angle, intensity);
    }

    Node* best = findMax(root);
    cout << "\nBest Tilt Angle: " << best->angle
         << " degrees (Intensity: " << best->intensity << ")\n";

    return 0;
}
