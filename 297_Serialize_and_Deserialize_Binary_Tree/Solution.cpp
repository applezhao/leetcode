/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        string temp = "";
        __serialize(root, temp);
        return temp;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) 
    {
        
        return __deserialize(data);
    }
private:
    void __serialize(TreeNode* root, string& res)
    {
        if(root == NULL)
        {
            res += "*";
            return;
        }
        res += to_string(root->val);
        res += ",";
        
        __serialize(root->left, res);
        res += ",";
        
        __serialize(root->right, res);
        return;
    }
    
    TreeNode* __deserialize(string& data)
    {
        if(data.size()==0)
            return NULL;
        if(data[0]=='*')
        {
            if(data.size()==1)
                data = "";
            else
                data = data.substr(2);
            return NULL;
        }
        size_t found = data.find_first_of(",");
        if(found==string::npos)//last node
        {
            TreeNode* node = new TreeNode(stoi(data));
            data = "";
            return node;
        }
        
        string nodeval = data.substr(0, found);
        TreeNode* node = new TreeNode(stoi(nodeval));
        
        data = data.substr(found+1);

        TreeNode* left = __deserialize(data);
        node->left = left;
        
        TreeNode* right = __deserialize(data);
        node->right = right;
        
        return node;
    }
};

// Your Codec object will be instantiated and called as such:
// Codec codec;
// codec.deserialize(codec.serialize(root));