先贴代码，下面是自己调通之后的代码

    class Solution {
    public:
        string convert(string s, int numRows) {
            int len = s.size();
            string result = "";
            int divide = max(numRows,2 * (numRows - 1));
            for(int i = 0;i < numRows;i++){
                int j = i;
                if(i == 0 || i == (numRows-1)){
                    while(j < len){
                        result.push_back(s[j]);
                        j += divide;
                    }
                }
                else{
                    int remainder = i % divide;
                    while(j < len){
                        result.push_back(s[j]);
                        int n_j = j + int(abs(divide - remainder - remainder));
                        if(n_j < len) result.push_back(s[n_j]);
                        j = j + divide;
                    }
                }
            }
            return result;
        }
    };

上面代码在运行之后空间使用上还好，但是时间上要14ms。

上面代码复杂度已经是O(n)，而且这道题无论如何也得遍历所有的字母才能得到答案。

看了解答区的代码，感觉自己写的太挫了。。

    class Solution {
    public:
        string convert(string s, int numRows) {
            if(numRows == 1) return s;
            
            int len = s.size();
            string result = "";
            int divide = 2 * (numRows - 1);  //对于“循环节”的理解上并没有太大区别
            for(int i = 0;i < numRows;i++){
                for(int j = 0;j + i < len;j += divide){
                    result.push_back(s[j + i]);
                    if(i!=0 && i!=numRows-1 && j+divide-i<len)   //这句是差别最大的地方
                        result.push_back(s[j+divide-i]);
                }
            }
            return result;
        }
    };

其实感觉自己最开始的代码和后来这个代码的区别还是在于：**对非两端的行的字母下标查找上理解不同** 

下面这份代码计算中间每个字母的递推式要比我的更简单。 `j+divide-i` 这里的j并不是一个循环节中的第一个字符所在的位置，`j+i`才是第一个字符所在的位置。所以，`j+divide-i` 相当于是 `j+i+divide-2i`, `divide-2i`就是一个循环节中第一个字符和第二个字符相差的距离了。

提交了以下，空间复杂度上略有改进，用的时间到了12ms，这在这道题上算是一个天堑了。。

我看了一下排名，发现还有8ms的，代码如下：

    static auto speedup = []() __attribute__((always_inline)) noexcept { ios::sync_with_stdio(0); cin.tie(0); cout.tie(0); return 0; }();

    class Solution {
    public:
        std::string convert(std::string s, int numRows) {
            if (s.size() <= numRows || numRows <= 1) {
                return s;
            }
            
            const int sz = s.size();
            const int triangleLength = 2*(numRows - 1),
                      triangleCount = (sz / triangleLength) + (sz % triangleLength > 0 ? 1 : 0);
            
            std::string result(sz, 0);
            
            for (int row = 0, r = 0; row < numRows; ++row) {
                for (int t = 0; t < triangleCount; ++t) {
                    int index = row + t*triangleLength;
                    if (index < sz) {
                        result[r++] = s[index];
                        if (row > 0 && row < numRows - 1) {
                            index += (numRows - row - 1)*2;
                            if (index < sz) {
                                result[r++] = s[index];
                            }
                        }
                    }
                }
            }
            
            return result;
        }
    };

逻辑上并没有什么大的区别，但是最上面的写的那段加速的代码实在是高。

最上面那段是关闭使用cin、cout的缓冲区同步的，使用cin，cout这类流IO方式是要输出到缓冲区中再到终端。但是scanf，printf这类原生的输入输出不会经过缓冲区，因此要快一些。

ios::sync_with_stdio(false)可以关闭这个同步，因此要快一些。

下面代码另外特别到位的一点在于写的代码十分规范，
[参考](https://www.cnblogs.com/PrayG/p/5749832.html)