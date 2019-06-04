题目很有意思，不算复杂，主要考察对问题的洞察能力。

最开始我是用的暴力，但是稍微加了一点小聪明

	int maxArea(vector<int>& height) {
	    int most_area = 0;
	    for(int i = 0;i < height.size();i++){
	        for(int j = 0;j < i;j++){
	            if(height[j] < height[i]) most_area = max((i-j)*height[j],most_area);
	            else {most_area = max((i-j)*height[i],most_area);break;}
	        }
	    }
	    return most_area;
	}

能节省一点时间的在else分支的break那里。遍历过程考虑了每一对组合，但是内层循环到第一个比height[i]大的位置就停止，因为就算再往后找也不可能找到比现在这个位置更大的面积。此时，水面的高度主要由height[i]控制，但是宽度已经是最大了，因此此时是第一个比height[i]大的数。再往后走，宽度在减小，高度也不可能比height[i]还要高。


	int maxArea(vector<int>& height) {
	    int most_area = 0,sta = 0,end = height.size()-1;
	    while(sta < end){
	        most_area = max(most_area,min(height[sta],height[end]) * (end-sta));
	        if(height[sta] > height[end]) end--;
	        else sta++;
	    }
	    return most_area;
	}

上面这一份代码只有O(n)的复杂度，显而易见。重点是它看穿了面积增长的本质。面积总归来说还是矩形，面积增长也就两个方法：增加宽度或增加高度。首先把宽度定为数组长度，从两边减小宽度的话，只能朝着让高度增加的方向。

如果恰好碰到两端高度一致，那么随机一个方向就行，可以这样解释：sta和end之间高于height[sta]的数值大于两个，那么一定有分别离sta和end最近的两个，设为nearest_sta和nearest_end。按照遍历过程，可以看到，sta一定会到nearest_sta，end一定会到nearest_end，这个过程中没有一种情况会超过sta和end之间的面积，到了nearest_sta和nearest_end，情况可能会发生变化，但这就是nearest_sta和nearest_end之间的故事了，sta和end已经完成了它们的阶段性使命。
