struct ele
{
    int val;
    mutable size_t count;
    ele(int val):val(val),count(1){}
};

struct valLess
{
    bool operator()(const ele& a, const ele& b)  const
    {
        return a.val < b.val;
    }
};

struct countGreater
{
    bool operator()(const ele& a, const ele& b)  const
    {
        return a.count > b.count;
    } 
};
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        set<ele, valLess> stat;
        for(auto& n : nums)
        {
            ele target(n);
            auto it = stat.find(target);
            if(it == stat.end())
            {
                stat.insert(target);
            }
            else
            {
                (*it).count++;
            }
        }
        // for(auto& ii : stat)
        //     cout<<ii.val<<":"<<ii.count<<endl;
        vector<ele> sorted(stat.begin(), stat.end());
        sort(sorted.begin(), sorted.end(), countGreater());
        
        vector<int> ret;
        for(int i=0; i<k; i++)
        {
            ret.push_back(sorted[i].val);
        }
         
        return ret;
    }
};