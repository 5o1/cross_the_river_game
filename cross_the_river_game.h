


//人物
typedef enum Human
{
    NOBODY = 0, //无名氏:NOBODY
    DAD,        //爹
    MOM,        //娘
    SON,        //儿子
    DAUGHTER,   //女儿
    POLICE,     //警察
    KILLER,     //土匪
    _Human_END, //结束位置:END
} Human;

//河岸
typedef enum Band
{
    BANDA = 0, //河岸A
    BANDB = 1, //河岸B
} Band;

//船
typedef struct Ship
{
    Band address; //船位于的位置
    Human seatA;  //驾驶座，只允许DAD,MOM,POLICE乘坐
    Human seatB;  //副驾驶座，允许剩下的所有人乘坐
} Ship;

//舞台:包含船和河岸（以及河岸上的人物）
typedef struct Stage
{
    Ship ship;
    int band[2][_Human_END]; //band[0]==河岸A,band[1]==河岸B,各个类型的人物分别存在的数量
} Stage;

//快照集合;内部包含保存的快照序列;最多可能有2^8种情况;
typedef struct Album
{
#define _MAX_ALBUM 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2
    Stage stage[_MAX_ALBUM];
    int len;
#undef _MAX_ALBUM
} Album;

//用于打印结果的栈的节点
typedef struct Node
{
    Human seatA;
    Human seatB;
    int album_index;
} Node;

//用于打印结果的栈
typedef struct Stack
{
    Node node[300];
    int tail;
} Stack;


//初始化游戏舞台;0:成功;-1:*stage为NULL;
int stage_reset(Stage *stage);

//将船移动到对岸;seatA不能为NOBODY;seatB可以为NOBODY;0:操作成功;-1:*stage为NULL;-2:seatA错误;-3:seatA或seatB不存在;1:游戏失败;2:游戏通关;
int ship_ferry(Stage *stage, const Human seatA, const Human seatB);

//判决;0:无事发生;-1:*stage为NULL;1:游戏通关;2:游戏失败;
int stage_judge(const Stage *stage);

//who在河岸band的数量
int if_who_at(const Stage *stage, const Human who, const Band band);

//返回坐在seatA的是谁
Human ship_who_at_seatA(const Stage *stage);

//返回坐在seatB的是谁
Human ship_who_at_seatB(const Stage *stage);

//快照序列album初始化;-1:*album为NULL
int album_reset(Album *album);

//保存快照;-1:*album为NULL;-2:*stage为NULL;否则返回保存的快照的下标
int stage_snapshot(Album *album, const Stage *stage);

//比对快照;-1:*album为NULL;-2:*stage为NULL;except:没有重复;否则返回和其重复的那个快照下标
int album_check(const Album *album, const Stage *stage,const int except);

//开始模拟游戏进程;-1:*stage为NULL;-2:*album为NULL;
int gamestart(Stage *stage, Album *album);

//恢复存档
int stage_restore(Stage *stage,const Album *album,const int index);

//把Human转换成字符串
char *Human_to_str(const Human human);

//初始化打印栈
void stack_reset(Stack *stack);

//push打印栈;返回剩余节点数
int stack_push(Stack *stack,const Human seatA,const Human seatB,const int album_index);

//pop打印栈;返回剩余节点数
Node stack_pop(Stack *stack);