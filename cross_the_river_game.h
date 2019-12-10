


//����
typedef enum Human
{
    NOBODY = 0, //������:NOBODY
    DAD,        //��
    MOM,        //��
    SON,        //����
    DAUGHTER,   //Ů��
    POLICE,     //����
    KILLER,     //����
    _Human_END, //����λ��:END
} Human;

//�Ӱ�
typedef enum Band
{
    BANDA = 0, //�Ӱ�A
    BANDB = 1, //�Ӱ�B
} Band;

//��
typedef struct Ship
{
    Band address; //��λ�ڵ�λ��
    Human seatA;  //��ʻ����ֻ����DAD,MOM,POLICE����
    Human seatB;  //����ʻ��������ʣ�µ������˳���
} Ship;

//��̨:�������ͺӰ����Լ��Ӱ��ϵ����
typedef struct Stage
{
    Ship ship;
    int band[2][_Human_END]; //band[0]==�Ӱ�A,band[1]==�Ӱ�B,�������͵�����ֱ���ڵ�����
} Stage;

//���ռ���;�ڲ���������Ŀ�������;��������2^8�����;
typedef struct Album
{
#define _MAX_ALBUM 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2
    Stage stage[_MAX_ALBUM];
    int len;
#undef _MAX_ALBUM
} Album;

//���ڴ�ӡ�����ջ�Ľڵ�
typedef struct Node
{
    Human seatA;
    Human seatB;
    int album_index;
} Node;

//���ڴ�ӡ�����ջ
typedef struct Stack
{
    Node node[300];
    int tail;
} Stack;


//��ʼ����Ϸ��̨;0:�ɹ�;-1:*stageΪNULL;
int stage_reset(Stage *stage);

//�����ƶ����԰�;seatA����ΪNOBODY;seatB����ΪNOBODY;0:�����ɹ�;-1:*stageΪNULL;-2:seatA����;-3:seatA��seatB������;1:��Ϸʧ��;2:��Ϸͨ��;
int ship_ferry(Stage *stage, const Human seatA, const Human seatB);

//�о�;0:���·���;-1:*stageΪNULL;1:��Ϸͨ��;2:��Ϸʧ��;
int stage_judge(const Stage *stage);

//who�ںӰ�band������
int if_who_at(const Stage *stage, const Human who, const Band band);

//��������seatA����˭
Human ship_who_at_seatA(const Stage *stage);

//��������seatB����˭
Human ship_who_at_seatB(const Stage *stage);

//��������album��ʼ��;-1:*albumΪNULL
int album_reset(Album *album);

//�������;-1:*albumΪNULL;-2:*stageΪNULL;���򷵻ر���Ŀ��յ��±�
int stage_snapshot(Album *album, const Stage *stage);

//�ȶԿ���;-1:*albumΪNULL;-2:*stageΪNULL;except:û���ظ�;���򷵻غ����ظ����Ǹ������±�
int album_check(const Album *album, const Stage *stage,const int except);

//��ʼģ����Ϸ����;-1:*stageΪNULL;-2:*albumΪNULL;
int gamestart(Stage *stage, Album *album);

//�ָ��浵
int stage_restore(Stage *stage,const Album *album,const int index);

//��Humanת�����ַ���
char *Human_to_str(const Human human);

//��ʼ����ӡջ
void stack_reset(Stack *stack);

//push��ӡջ;����ʣ��ڵ���
int stack_push(Stack *stack,const Human seatA,const Human seatB,const int album_index);

//pop��ӡջ;����ʣ��ڵ���
Node stack_pop(Stack *stack);