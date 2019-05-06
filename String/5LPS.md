题意简单易懂，就是在一个字符串中找到一个最长的回文子串。

**子串**和**回文串**的定义不再赘述。

Approach1：
对每个位置的子串都进行验证，对每个位置都进行一次遍历需要O(n^2)

如果使用遍历的方式验证子串是不是回文串，最终的时间复杂度是O(n^2)

但是可以考虑回文串的性质而减少回文串验证的时间复杂度。

最开始考虑用dp，代码如下

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

但是效果不是特别好，内存、时间都用的多。

![first_runtime](first_runtime.jpg)

![first_memory](first_memory.jpg)

**后来查了一下，dp中的状态矩阵可以使用二进制简化，更改之后的代码如下：**

	class Solution {
	public:
	    string longestPalindrome(string s) {
		if(s == "") return "";
		int bit_len = s.size()/32,bit_p,bit_r;
		bit_len += ((s.size() % 32) > 0);
		int max_len = 1,sta = 0;
		vector<vector<unsigned int> >palin;
		for(int i = 0;i < s.size();i++){
		    vector<unsigned int> temp(bit_len,0);
		    bit_p = i >> 5;bit_r = i % 32;
		    temp[bit_p] = 1 << bit_r;
		    palin.push_back(temp);
		}

		for(int i = 1;i <= s.size() - 1;i++){
		    for(int j = 0;j+i < s.size();j++){
			if(s[j] == s[j+i]){
			    if(i > 1){
				bit_p = (i+j-1) / 32;bit_r = (i+j-1) % 32;
				if((palin[j+1][bit_p] & (1<<bit_r)) > 0){
				    if(i + 1 > max_len){
					sta = j;max_len = i + 1;
				    }
				    bit_p = (j+i)/32;bit_r = (i+j)%32;
				    palin[j][bit_p] |= (1<<(bit_r));
				}
			    }
			    else {
				bit_p = (j+1) / 32;bit_r = (j+1) % 32;
				palin[j][bit_p] |= (1<<bit_r);
				sta = j;
				max_len = 2;
			    }
			}
		    }
		}

		return s.substr(sta,max_len);
	    }
	};

改了之后的效果如下：

![second_runtime](second_runtime.jpg)

![second_memory](second_memory.jpg)

更改的方式主要是将每个存状态的变量由int转为了bit，节省了将近32倍的空间

而且因为位运算得天独厚的优势，运行时间也得到了加成

第t位设置为1的方式是 `x = x | (1 << t)` 验证第t位是不是1的方式是`x & (1 << t) > 0`

原理虽然不难，但是改的时候也遇到了一个很难发现的问题，如果使用'int'来当作*bit*的容器

因为`int`使用补码来存嘛，所以如果最高是1会发生溢出，讲道理来说这对验证是不是为1并没有什么影响，但是结果不对

后来我改成`unsigned int`才过

PS. 突然想到为什么使用`int`对上述方案不行，因为上面程序的判断方式是 **大于**,如果`int`的最高位正好是1，判断当然不成立。
可以将上面改为 **不等于0**,因为`int`使用的是补码，0的表示形式只有一种，不会引发其他问题。

好多事不做一遍真的不知道是什么样子呀

Approach 2:
在上面的基础上进行进一步分析，上面使用二维数组的目的是记录一个子串是不是回文串，这样能够让包含这个子串的更长的子串进行判断时更省时。如果不用数组，那么每个子串的分析就会变成O(n)，这是一个难以承受的代价。

其实回文串有一些性质，它的“中心”来自于一个字符或者是两个字符，一个字符的情况不用多说，两个字符代表这两个字符相同如:baab,“中心”是 “aa”。既然知道了这一性质，可以利用它来对程序进行修改。

	class Solution {
	public:
	    string longestPalindrome(string s) {
	        if(s.size() <= 1) return s;
	        int max_len = 1,sta = 0,len = s.size();
	        for(int i = 0;i < len-1;i++){
	            int j = 0;
	            while(i-j>=0 && i+j<len && s[i-j]==s[i+j]) j++;
	            if(2*(j-1)+1 > max_len){
	                max_len = 2*(j-1) + 1; sta = i-j+1;
	            }
	            if(s[i] == s[i+1]){
	                j = 0;
	                while(i-j>=0 && i+1+j<len && s[i-j]==s[i+1+j]) j++;
	                if(2*(j-1)+2 > max_len){
	                    max_len = 2*(j-1) + 2; sta = i-j+1;
	                }
	            }
	        }
	        return s.substr(sta,max_len);
	    }
	};

上面是现在修改的最精简的一个版本了。

改进效果如下：
![third_runtime](third_runtime.jpg)
![third_memory](third_memory.jpg)

PS.后来看到一个DL的代码，遍历的时候直接跳过了相同的子串，这样就没有“中心”是一个字符还是两个字符的区别了，之后在这个子串两边进行回文串检测，真是高呀。。

