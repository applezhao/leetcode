class Solution {
public:
    bool canCross(vector<int>& stones) {
        
        if(stones.size() <= 1)
            return true;
        //some case has a big end.
        if(stones[stones.size()-1] > __max(stones.size()))
            return false;
        
        for(size_t i = 0; i < stones.size(); i++)
        {
            m_value2Idx.insert({stones[i], i});
        }
        
        return __can(0, 0, stones);
    }
    
    long long __max(size_t s)
    {
        return s/2*s + s;
    }
private:
    
    
    bool __can(int lastJump, int s, const vector<int>& stones) const
    {
        if(s == stones.size()-1)
            return true;
        if(s == 0)
        {
            int nexts = __valid(s, 1, stones);
            if(nexts != -1)
            {
                if(__can(1, nexts, stones))
                    return true;
            }
            return false;
        }
        
        int nexts = __valid(s, lastJump+1, stones);
        if(nexts != -1)
        {
            if(__can(lastJump+1, nexts, stones))
                return true;
        }
        
        nexts = __valid(s, lastJump, stones);
        if(nexts != -1)
        {
            if(__can(lastJump, nexts, stones))
                return true;
        }
        
        if(lastJump > 1)// 0 is meaningless
        {
            nexts = __valid(s, lastJump-1, stones);
            if(nexts != -1)
            {
                if(__can(lastJump-1, nexts, stones))
                    return true;
            }
        }
        
        return false;
    }
    
    int __valid(int s, int jump, const vector<int>& stones) const
    {
        
        auto it = m_value2Idx.find(stones[s]+jump);
        if(it == m_value2Idx.end())
            return -1;
        return it->second;
    }
private:
    unordered_map<size_t, size_t> m_value2Idx;
};