/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() {}

    Node(int _val, Node* _left, Node* _right, Node* _next) {
        val = _val;
        left = _left;
        right = _right;
        next = _next;
    }
};
*/
class Solution {
public:
    Node* connect(Node* root) {
        Node * fakeHead = new Node();

        Node* currHead = root; //maintain a head pointer for current level. This list will be as a queue for level traversal
        while(currHead!=NULL&&currHead->left!=NULL)
        {
            Node* p = currHead;
            Node* nextLevel = fakeHead;
            Node* q = nextLevel;
            while(p!=NULL)
            {
                q->next = p->left;
                q=q->next;
                q->next = p->right;
                q=q->next;
                
                p=p->next;
            }
            q->next = NULL;
            currHead = nextLevel->next;
        }
        delete fakeHead;
        return root;
    }
};