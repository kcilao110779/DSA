#include <stdio.h>

struct trieNode
{
    //   trieNode *parent; //用來記錄父點是誰
    trieNode *children[27];
    bool exist[27] = {false};

    //   char chr; //字母
    // int indexNode; //紀錄是第幾個node
    int level; //紀錄該點的深度
};

void insert(trieNode **index_node, int &i, int &insert_index, char &chr)
{
    trieNode *p = index_node[insert_index];
    int chr_ASCII;
    chr_ASCII = chr - 96;
    if (p->exist[chr_ASCII] != true)
    {
        p->children[chr_ASCII] = index_node[i];
        p->exist[chr_ASCII] = true;   
        index_node[i]->level = p->level + 1;
    }
    else
        index_node[i] = p->children[chr_ASCII];
}

void swap(int &a, int &b)
{
    a ^= b;
    b ^= a;
    a ^= b;
}

int findLCA(trieNode **index_node, int &x, int &y, int **jump)
{   //參考: https://www.itread01.com/content/1545277685.html
    if (index_node[x]->level < index_node[y]->level)
        swap(x, y); //保證x比較深，方便計算
    int dis = index_node[x]->level - index_node[y]->level;
    for (int i = 17; i >= 0; i--)
    { //先跳到等高
        if ((1 << i) & dis)
            x = jump[x][i];
    }
  
    if (index_node[x] == index_node[y])
       return index_node[x]->level;
    
    for (int i = 17; i >= 0; i--)
    {
        if (index_node[jump[x][i]] != index_node[jump[y][i]])
        {
            x = jump[x][i];
            y = jump[y][i];
        }
    }
    
    return index_node[jump[x][0]]->level;
}
int main()
{
    int num, i, j;
    int insert_index;
    int search_cnt;
    char chr;

    scanf("%d", &num);
    trieNode **index_node = new trieNode *[num + 1]; //紀錄每個index對應的node位址
    int **jump = new int *[num + 1];

    for (i = 0; i < num + 1; i++)
    {
        jump[i] = new int[18];
        for (j = 0; j < 18; j++)
        { //初始化跳躍表
            jump[i][j] = 0;
        }
        index_node[i] = new trieNode;
    }

    index_node[0]->level = 0;
    for (i = 1; i < num + 1; i++)
    {
        scanf("%d %c", &insert_index, &chr);
        insert(index_node, i, insert_index, chr);

        jump[i][0] = insert_index;
        for (j = 1; j < 18; j++)
        { //更新跳躍表
            if (jump[insert_index][j - 1] != 0)
                jump[i][j] = jump[jump[i][j - 1]][j - 1];
        }
    }
  
    scanf("%d", &search_cnt);
    int index1, index2;
    int *ansLCA = new int[search_cnt + 1];
    for (i = 1; i < search_cnt + 1; i++)
    {
        scanf("%d %d", &index1, &index2);

        *(ansLCA + i) = (index_node[index1] == index_node[index2]) ? (index_node[index1]->level) : (findLCA(index_node, index1, index2, jump));
       
    }
    for (i = 1; i < search_cnt + 1; i++)
        printf("%d\n", ansLCA[i]);

    return 0;
}
