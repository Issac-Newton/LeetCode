题干和问题乃至解答都非常相似的两道题目（[题目1](https://leetcode.com/problems/combination-sum-iv/),[题目2](https://leetcode.com/problems/coin-change-2/)），两道题目都是求组合的数目，但是一道题目认为{1,2}和{2,1}是两种不同的组合，另一道题目则使用的是我们常见的定义：认为上述案例是相同的。

题目1的解答是这样的:
```
int combinationSum4(vector<int>& nums,int target){
    int len = nums.size();
    vector<unsigned int> dp(target+1,0);
    dp[0] = 1;
    for(int i = 1;i <= target;i++){
        for(int j = 0;j < len;j++){
            if(i >= nums[j]) dp[i] += dp[i-nums[j]];
        }
    }
    return dp[target];
}
```

题目2的解答是这样的：
```
int change(int amount, vector<int>& coins) {
    vector<int> dp(amount+1,0);
    dp[0] = 1;

    for(int coin : coins){
        for(int i = 1;i <= amount;i++){
            if(i >= coin) dp[i] += dp[i-coin];
        }
    }        
    return dp[amount];
}
```

可以看到，两种解答十分相似，从形式上看，只是两个循环的顺序调换了一下。我认为造成不同结果的原因在于硬币之间的“替代”效果。对于两个问题来说，dp数组记录的都是在每道题目中构成数量i的次数。如果硬币是{1、2}，以3为例，那么第一道题中的结果是3：若将3看作是3个1，那么2既可以替代前两个1构成2、1也可以替代后两个1构成1、2。若按照第一题的遍历顺序那就要计算两次。但是在第二题中的遍历顺序，以coin为外层循环变量，相当是说只让2替代其中最后2个1，这样就不会重复计数。