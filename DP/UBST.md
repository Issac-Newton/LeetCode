关于二叉搜索树的两道题目：95号和96号

两道题的题干是相同的：给定一个整数，问从1-n所有整数构成的不同的二叉搜索树；题96需要的是结果的数量，题95问的是每个结果都是什么（这里的题序有些问题，如果能先看到96题，对95题的解题思路有帮助 Emmm.. 因为我先看到的是96题，所以我就先做的是这道题。）

定义的BST结构是这样的：
```
	struct TreeNode {
	     int val;
	     TreeNode *left;
	     TreeNode *right;
	     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
	};
```
我首先的思路并没有和DP产生多大关联，因为最开始我并不觉得可以从K之前的结果简单地生成k的结果（当然，如果我想到了95题的思路那就是另一回事了）。所以，最开始我的思路比较粗暴，想着先生成一个只有左侧链的BST，然后对每个左孩子实施右旋。这样的办法是可行的，但在正确性上以及可实施性上有一些问题。首先是正确性，这种方法会产生重复的BST，当然，也可以把新生成的BST和之前生成的所有BST进行对比进行结果筛查，但比较耗时间：粗略估计，n比较大的时候，实践复杂度远超O(n2)，这一点其实就已经可以否决这种方法。另外则是该题从给出的BST定义上看就不适合做右旋这种操作，进行旋转比较合适的结构是在BST的定义中添加上指向父节点的指针，这样能够在进行右旋的时候快速找到父节点和祖父节点。

所以，上述方法不可行，我想到了使用分治的方法进行生成。其实对于n，最终结果无非就是让1-n依次当根节点。选定根节点的数之后，左子树和右子树的所有的数就确定了，然后在左子树和右子树上分别进行递归。最终的代码如下：
```
	vector<TreeNode*> generate(int from,int to){
	    vector<TreeNode*> result;

	    if(from > to){result.push_back(NULL);}
	    else{
	        for(int i = from;i <= to;i++){
	            vector<TreeNode*> Right = generate(i+1,to);
	            vector<TreeNode*> Left = generate(from,i-1);

	            for(auto lc : Left){
	                for(auto rc : Right){
	                    TreeNode* root = new TreeNode(i);
	                    root->left = lc;
	                    root->right = rc;
	                    result.push_back(root);
	                }
	            }
	        }	
	    }
	    return result;
	}
	vector<TreeNode*> generateTrees(int n) {
	    if(n == 0) return vector<TreeNode*>(0);
	    return generate(1,n);
	}
```

这题放在了dp的title下面，没有一种dp的做法总觉得不太对劲。。。

后来，我看到了一份代码，瞬间开朗
```
	vector<TreeNode*> generateTrees(int n) {
        if (!n) {
            return {};
        }
        std::vector<std::vector<TreeNode *>> dp(n + 1);
        dp[0].push_back(nullptr);
        for (int i = 1; i <= n; ++i) {
            for (int mid = 1; mid <= i; ++mid) {
                for (auto ltree : dp[mid - 1]) {
                    for (auto rtree : dp[i - mid]) {
                        TreeNode *node = new TreeNode(mid);
                        node->left = ltree;
                        node->right = clone(rtree, mid);
                        dp[i].push_back(node);
                    }
                }
            }
        }
        return std::move(dp[n]);
    }
    
    TreeNode *clone(TreeNode *node, int offset)
    {
        if (!node) {
            return nullptr;
        }
        TreeNode *new_node = new TreeNode(node->val + offset);
        new_node->left = clone(node->left, offset);
        new_node->right = clone(node->right, offset);
        return new_node;
    }
```
这种解法就是使用k之前的结果来产生k的结果。我是在看完上面代码之后才看到的96题，瞬间感觉真滴是先做了第二问才做的第一问。

上面能够使用dp的一个点在于：对于一个给定的数n来说，不同的BST个数和n没有关系，只跟BST上的节点个数有关系。所以当根节点的值是mid时，左子树的每个形状就能够在dp[k-1]中找到，右子树的所有形状能够在dp[i-mid]中找到。


PS：

**C++使用技巧**

使用new关键字分配的变量在堆中，所以在函数调用中使用new不用担心野指针的问题。具体情况是这样的：如果返回值和指针有关，并且指针指向的是函数中的局部变量，那么会造成野指针的产生。另外，malloc也是在栈中分配空间。