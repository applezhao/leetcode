struct node
{
    int key;
    int val;
    node* next;
    node* pre;
    node(int key, int val):key(key),val(val),next(NULL),pre(NULL){}
};

class LRUCache {
public:
    LRUCache(int capacity) {
        m_capacity = capacity;
        m_listHead = new node(-1, -1);
        m_listTail = new node(-1, -1);
        m_listHead->next = m_listTail;
        m_listTail->pre = m_listHead;
        m_listLen = 0;
        m_hash.clear();
    }
    
    int get(int key) {
        auto it = m_hash.find(key);
        if(it == m_hash.end() || it->second==NULL)
            return -1;
        
        //exist . we move this node to front and return its value.
        node* tomove = it->second;
        node* pp = tomove->pre;
        node* qq = tomove->next;
        pp->next = qq;
        qq->pre = pp;
        
        //insert to front
        tomove->next = m_listHead->next;
        tomove->pre = m_listHead;
        m_listHead->next = tomove;
        tomove->next->pre = tomove;
        
        return tomove->val;
    }
    
    void put(int key, int value) {
        auto it = m_hash.find(key);
        if(it != m_hash.end() && it->second!=NULL)
        {
            it->second->val = value;
            get(key);
            return;
        }
        if(m_listLen == m_capacity)
        {
            //cout<<"to insert "<<key<<","<<value<<endl;
            
            //remove end
            node* todelete = m_listTail->pre;
            //cout<<"to delete "<<todelete->key<<","<<todelete->val<<endl;
            m_hash[todelete->key] = NULL;
            node* pp = todelete->pre;
            pp->next = m_listTail;
            m_listTail->pre = pp;
            delete todelete;
            
            m_listLen--;
        }
        
        //add to front
        node* toAdd = new node(key, value);
        m_hash[key] = toAdd;
        
        node* qq = m_listHead->next;
        
        toAdd ->next = qq;
        qq->pre = toAdd;
        
        toAdd ->pre = m_listHead;
        m_listHead->next = toAdd;
        m_listLen++;
    }
    
private:
    node* m_listHead;
    node* m_listTail;
    int m_capacity;
    int m_listLen;
    
    unordered_map<int, node*> m_hash;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache obj = new LRUCache(capacity);
 * int param_1 = obj.get(key);
 * obj.put(key,value);
 */