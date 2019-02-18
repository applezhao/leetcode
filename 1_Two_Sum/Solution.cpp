class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        multimap<int, size_t> numsMap;
        for(size_t i = 0; i < nums.size(); i++)
        {
            numsMap.insert(make_pair(nums[i], i));
        }
        
        vector<int> ret(2, -1);
        for(auto item = numsMap.begin(); item != numsMap.end(); item++)
        {
            int tofind = target - item->first;
            auto found = numsMap.lower_bound(tofind);
            if(found == item)
                found++;
            if(found != numsMap.end() && found->first == tofind)
            {
                ret[0] = item->second;
                ret[1] = found->second;
                return ret;
            }
        }
        return ret;
    }
};