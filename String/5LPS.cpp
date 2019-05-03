class Solution {
public:
    string longestPalindrome(string s) {
        if(s == "") return "";
        int max_len = 1,sta = 0;
        vector<vector<int> >palin;
        for(int i = 0;i < s.size();i++){
            vector<int> temp;
            for(int j = 0;j < s.size();j++) temp.push_back(i == j);
            palin.push_back(temp);
        }
        
        for(int i = 1;i <= s.size() - 1;i++){
            for(int j = 0;j+i < s.size();j++){
                if(s[j] == s[j+i]){
                    if(i > 1){
                        if(palin[j+1][i+j-1]){
                            if(i + 1 > max_len){
                                sta = j;max_len = i + 1;
                            }
                            palin[j][j+i] = 1;
                        }
                    }
                    else {
                        palin[j][j+1] = 1;
                        sta = j;
                        max_len = 2;
                    }
                }
            }
        }
        
        return s.substr(sta,max_len);
    }
};