#include <iostream>
#include <string.h>
#include <stdio.h>

using namespace std;
//===========================================================================
//函数功能：输出zigzag数组
//知识点1:zigzag用于对JPEG图像压缩算法
//知识点2:通过malloc/free来动态分配一个二维数组的内存
//知识点3:zigzag数组的算法，主要思想是分为上三角和下三角二种情况分析
//              其中下三角的情况最为复杂。
//===========================================================================
int ouputZigzag()
{
    int n;//要分配数组的维数
    int cross = 0;//第cross条对角线，对角线从0开始计数
    //这里如果不初始化为0，系统会崩溃，至今未解，只能学到经验：以后每个变量在定义的时候都要赋初值。
    int count = 0;//通过每次+1来计算zigzag数组的每个元素的具体数值
    printf("\n\t\t\t\t【zigzag】\n");
    printf("\n请输入要求zigzag数组的维数:");
    scanf("%d", &n);
    printf("\n\n");

    //定义一个指向指针的指针。
    //先给这个指向指针的指针分配n个sizeof(int)的空间
    int **a = (int**)malloc(sizeof(int)*n);
    //如果这个指向指针的指针分配内存失败，返回。
    if (a == NULL)
        return 0;
    //轮询给之前分配的n个指针每个都再分配n个sizeof(int)的内存
    //从而通过malloc的方法分配一个二维数组的内存
    for (int i = 0; i < n; i++)
    {
        *(a+i) = (int*)malloc(sizeof(int)*n);
        if (*(a + i) == NULL)//如果分配失败，
        {
            while (--i > 0)
            {
                free(a[i]);//先释放*(a+i)的内存
            }
            free(a);//再释放**a的内存
            return 0;
        }
    }
    //上三角(包括对角线)
    //关键2点：分第偶数条对角线和第奇数条对角线 & row+col=cross
    for (cross = 0; cross < n; cross++)
    {
        if (cross % 2 == 0)//偶对角线
        {
            for (int col = 0; col <= cross; col++)//关系row+col=cross
            {
                a[cross - col][col] = count++;
            }
        }
        else if (cross % 2 != 0)//奇对角线
        {
            for (int row = 0; row <= cross; row++)
            {
                a[row][cross - row] = count++;
            }
        }
    }
    //下三角
    //以对角线(n-1)条cross为中心线,则设右下方与之相邻的对角线为n-2,接着右下依次为n-3,n-4,....,1,0.
    //下三角的数组a[i][j]中的i,j的取值范围为【1，n-1】  即i,j取值的下限为1
    //因此这里可以定义一个关键的变量 limit=1；
    //而刚好i+j可以与limit联系起来:i+j=(n-1)+limit
    //因此对于一个cross，可以用limit这个关键变量来完成
    //通过limt可以限制每个cross须要填充的目标的个数 &  通过limt来移动到下一个cross
    //即limt可以将每个cross与每个cross须要填充的目标的个数联系起来
    int limit = 1;//关键点的变量 ！！！！
    for (cross = (n - 1) - 1; cross >= 0; cross--)
    {
        if (cross % 2 == 0)
        {
            for (int row = n - 1; row >= limit; row--)
            {
                a[row][n - 1 + limit - row] = count++;
            }
            limit++;
        }
        if (cross % 2 != 0)
        {
            for (int col = n - 1; col >= limit; col--)
            {
                a[n - 1 + limit - col][col] = count++;
            }
            limit++;
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%5d", a[i][j]);
        }
        cout << endl;
    }
    printf("\n\n\t\t\t\t\t\t\t----coded by 我不是斗哥\n");
    return 0;
}
int main()
{
    ouputZigzag();
    system("pause");
}