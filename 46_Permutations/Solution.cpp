class Solution {
public:
    vector<vector<int>> permute(vector<int>& nums) {
        //stl
        /*
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        do
        {
            res.push_back(nums);
        }while(next_permutation(nums.begin(), nums.end()));
        
        return res;
        */
        
        //23 32 123 213 231 132 312 321 
        m_result.clear();
        m_used.clear();
        m_used.resize(nums.size(), false);
        m_nums = nums;
        s = m_nums.size();
        
        __permute();
        nums = m_nums;
        return m_result;
    }
private:    
    void __permute()
    {
        if(m_current.size() == s)
        {
            m_result.push_back(m_current);
            return;
        }
        
        for(size_t i=0; i<s; i++)
        {
            if(!m_used[i])
            {
                m_used[i] = true;
                m_current.push_back(m_nums[i]);
                __permute();
                m_current.pop_back();
                m_used[i]=false;
            }
        }
    }
private:
    vector<bool> m_used;
    vector<int> m_nums;
    vector<int> m_current;
    vector<vector<int>> m_result;
    size_t s;
};