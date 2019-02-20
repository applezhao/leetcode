/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) 
    {
        if(preorder.size() == 0)
            return NULL;
        return __build(preorder, 0, preorder.size()-1, inorder, 0, preorder.size()-1);
    }
    
    TreeNode* __build(const vector<int>& preorder, int sp, int ep, const vector<int>& inorder, int si, int ei)
    {
        if(sp > ep)
            return NULL;
        
        TreeNode* root = new TreeNode(preorder[sp]);
        
        if(sp == ep)
        {
            return root;
        }
        
        int rootIdxInorder = distance(inorder.begin(), find(inorder.begin()+si, inorder.begin()+ei, preorder[sp]));
        
        TreeNode* leftRoot = __build(preorder, sp+1, sp+rootIdxInorder-si, inorder, si, rootIdxInorder-1);
        root->left = leftRoot;
        
        TreeNode* rightRoot = __build(preorder, sp+rootIdxInorder-si+1, ep, inorder, rootIdxInorder+1, ei);
        root->right = rightRoot;
        return root;
    }
};