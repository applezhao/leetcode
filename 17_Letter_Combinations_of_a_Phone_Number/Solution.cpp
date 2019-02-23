class Solution {
public:
    vector<string> letterCombinations(string digits) {
        m_len = digits.size();
        m_result.clear();
        m_digits = digits;
        if(m_len==0)
            return m_result;
        
        __prepareButtonMapping();
        __letterComb();
        return m_result;
    }
private:
    void __prepareButtonMapping()
    {
        m_button = {"abc", "def", "ghi", "jkl", "mon", "pqrs", "tuv", "wxyz"};
    }
    void __letterComb()
    {
        if(m_current.size()==m_len)
        {
            m_result.push_back(m_current);
            return;
        }
        
        string currentButton = m_button[m_digits[m_current.size()]-'2'];
        for(auto& c : currentButton)
        {
            m_current += c;
            __letterComb();
            m_current.pop_back();
        }
    }
private:
    vector<string> m_button;
    vector<string> m_result;
    string m_current;
    
    int m_len;
    string m_digits;
};