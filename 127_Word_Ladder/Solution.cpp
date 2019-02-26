class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        bool isEndInDict = false;
        bool isBeginInDict = false;
        for(auto& i : wordList)
        {
            if(i==endWord)
            {
                isEndInDict = true;
            }
            if(i==beginWord)
            {
                isBeginInDict = true;
            }
        }
        if(!isEndInDict)
            return 0;
        __endWord = endWord;
        __words = wordList;
        if(!isBeginInDict)
            __words.push_back(beginWord);
        __visited.clear();
        __visited.resize(__words.size(), false);
        
        queue<int> curr;
        //curr.push();
        for(int i=0; i<__words.size(); i++)
        {
            if(__words[i]==beginWord)
            {
                __visited[i]=true;
                curr.push(i);
                break;
            }
        }
        //__visited[__words.size()-1]=true;
        __len = 1;
        __BFS(curr);
        return __len;
    }
    
    bool __hasEdge(const string& a, const string& b)
    {
        int diff = 0;
        for(int i=0; i<a.size(); i++)
        {
            if(a[i]!=b[i])
                diff++;
        }
        return diff==1;
    }
    
    void __BFS(queue<int>& currentDepth)
    {
        if(currentDepth.empty())
        {
            __len = 0;
            return;
        }
        __len++;
        queue<int> nextDepth;
        while(!currentDepth.empty())
        {
            string curr = __words[currentDepth.front()];
            currentDepth.pop();
            
            for(int i=0; i<__words.size(); i++)
            {
                if(!__visited[i]&&__hasEdge(curr, __words[i]))
                {
                    if(__words[i]==__endWord)
                        return;
                    __visited[i]=true;
                    nextDepth.push(i);
                }
            }
        }
        __BFS(nextDepth);
    }
private:
    int __len;
    vector<bool> __visited;
    vector<string> __words;
    string __endWord;
    
};