关于字符串的两道题目：题目大意是这样的，给定一个字符串S和一个字符串数组dic，问字符串是否能用字符串数组中的字符串拼接起来。字符串数组中的数组是可以重复使用的。139问是否可以，140问各种方案都是什么。
看到这道题目在DP下面，自然而然想到找状态转移方程，但是一直都没有想到比较好的方式。最开始的方式一直是使用类似于暴力的方式进行查找，在dic中找S的字串，一直不成功。直到后来看到别人的解答。。
我最开始的代码
```
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        int len = s.size();
        set<string> dict(wordDict.begin(),wordDict.end());

        string to_match;
        int last = 0;
        while(true){
            int i = 0,j;
            for(j = max(i,last)+1;j <= len;j++){
                to_match = s.substr(i,j-i);
                if(dict.find(to_match) != dict.end()){
                    if(i == 0) last = j;
                    if(j == len) return true;
                    i = j;
                }
                else{
                    if(i==0 && j==len) return false;
                }
            }
        }
    }
};
```
其实我的最开始的思维一直是“从集合中找”，而并没有想到如何记录下哪些有用的信息。下面是后来我看到别人的想法之后的代码
```
bool wordBreak(string s, vector<string>& wordDict) {
    if(wordDict.size() == 0) return false;

    set<string> dict(wordDict.begin(),wordDict.end());
    vector<bool> dp(s.size()+1,false);
    dp[0] = true;

    for(int i = 1;i <= s.size();i++){
        for(int j = i-1;j >= 0;j--){
            if(dp[j]){
                string word = s.substr(j,i-j);
                if(dict.find(word) != dict.end()){
                    dp[i] = true;
                    break;
                }
            }
        }
    }
    return dp[s.size()];
}
```
要记录哪些，上面的代码已经写的一目了然了：字符串S每一个位置上的字符是否是字符串集合中某一个字串的结束。这样就能够使用之前的信息，也即前面的字符串是否能够匹配上，进而通过现在推断出S是否能用字符串集合拼接。
我感觉，从这道题里面，最开始我并没有能够想到要保存哪些信息，一是没有很好地将字符串S“离散化”，没有找到一种合适的方式将S和dic对应起来；二是好久没有做过题了，感觉确实生疏了。加强训练，不断总结才能提高。
