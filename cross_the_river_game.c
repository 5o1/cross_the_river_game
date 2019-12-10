/*
��Ϸ����:
��λ��1���ְ֣�1�����裬2�����ӣ�2��Ů����1�����ˣ�1������
���أ�1���Ӱ�A��1������1���Ӱ�B
��ʼ��ȫԱ�ںӰ�A
Ŀ�꣺ȫԱ�ںӰ�B
�ְ֡����衢������Կ��ƴ�
��1������2��
���û�����裬�ְֲ��ܺ�Ů��ͬʱ�ںӰ�A����ͬʱ�ڴ��ϣ���ͬʱ�ںӰ�B
���û�аְ֣����費�ܺͶ���ͬʱ�ںӰ�A����ͬʱ�ڴ��ϣ���ͬʱ�ںӰ�B
���û�о��죬���˲��ܺͰְֻ��������ӻ�Ů��ͬʱ�ںӰ�A����ͬʱ�ڴ��ϣ���ͬʱ�ںӰ�B
*/

#ifndef CROSS_THE_RIVER_GAME_H
#include "cross_the_river_game.h"
#define CROSS_THE_RIVER_GAME_H
#endif

#ifndef __STRING_H__
#include <string.h>
#define __STRING_H__
#endif

int stage_reset(Stage *stage)
{
    //���ж������Ƿ����
    if (!stage)
    {
        return -1; //-1��*stageΪNULL
    }

    Human who;
    if (stage)
    {
        stage->ship.address = BANDA;      //��.λ�ó�ʼ���ںӰ�A
        stage->ship.seatA = NOBODY;       //��.����ʻ����ʼ��Ϊ���˳���
        stage->ship.seatB = NOBODY;       //��.����ʻ����ʼ��Ϊ���˳���
        stage->band[BANDA][DAD] = 1;      //�Ӱ�A��1���ְ�
        stage->band[BANDA][MOM] = 1;      //�Ӱ�A��1������
        stage->band[BANDA][POLICE] = 1;   //�Ӱ�A��1������
        stage->band[BANDA][SON] = 2;      //�Ӱ�A��2������
        stage->band[BANDA][DAUGHTER] = 2; //�Ӱ�A��2��Ů��
        stage->band[BANDA][KILLER] = 1;   //�Ӱ�A��1������
        for (who = NOBODY; who < _Human_END; ++who)
        {
            stage->band[BANDB][who] = 0; //�Ӱ�B��һ��
        }
    }
    return 0; //0���ɹ�
}

int ship_ferry(Stage *stage, const Human seatA, const Human seatB)
{
    //���ж������Ƿ����
    if (!stage)
    {
        return -1; //*stageΪNULL
    }
    if (seatA != DAD && seatA != MOM && seatA != POLICE)
    {
        return -2; //����ʻ������
    }
    if (seatA == seatB)
    {
        if (stage->band[stage->ship.address][seatA] - 2 < 0)
        {
            return -3; //�ϴ����˲�����
        }
    }
    else if (seatB == NOBODY)
    {
        if (stage->band[stage->ship.address][seatA] - 1 < 0)
        {
            return -3; //�ϴ����˲�����
        }
    }
    else
    {
        if (stage->band[stage->ship.address][seatA] - 1 < 0 || stage->band[stage->ship.address][seatB] - 1 < 0)
        {
            return -3; //�ϴ����˲�����
        }
    }

    //��ѡ��ĵ�λ���ϴ�
    stage->ship.seatA = seatA;
    --(stage->band[stage->ship.address][seatA]);
    stage->ship.seatB = seatB;
    if (seatB != NOBODY)
    {
        --(stage->band[stage->ship.address][seatB]);
    }

    //��һ��judge
    switch (stage_judge(stage))
    {
    case 0: //���·���
        break;
    case 1:       //��Ϸͨ��
        return 2; //2����Ϸͨ��
        break;
    case 2:       //��Ϸʧ��
        return 1; //1����Ϸʧ��
        break;
    case -1:       //��ָ̨��ΪNULL
        return -1; //-1��*stageΪNULL
        break;
    }

    //����
    stage->ship.address = !(stage->ship.address);

    //�Ѵ��ϵĵ�λ���´�
    ++(stage->band[stage->ship.address][seatA]);
    stage->ship.seatA = NOBODY;
    if (seatB != NOBODY)
    {
        ++(stage->band[stage->ship.address][seatB]);
    }
    stage->ship.seatB = NOBODY;

    //��һ��judge
    switch (stage_judge(stage))
    {
    case 0: //���·���
        break;
    case 1:       //��Ϸͨ��
        return 2; //2����Ϸͨ��
        break;
    case 2:       //��Ϸʧ��
        return 1; //1����Ϸʧ��
        break;
    case -1:       //��ָ̨��ΪNULL
        return -1; //-1����ָ̨��ΪNULL
        break;
    }

    //���·���������0�ɹ�
    return 0;
}

int stage_judge(const Stage *stage)
{
    //���ж������Ƿ����
    if (!stage)
    {
        return -1;
    }

    //�ж��Ƿ���Ϸͨ��
    if (if_who_at(stage, DAD, BANDB) == 1 && if_who_at(stage, MOM, BANDB) == 1 && if_who_at(stage, POLICE, BANDB) == 1 && if_who_at(stage, KILLER, BANDB) == 1 && if_who_at(stage, SON, BANDB) == 2 && if_who_at(stage, DAUGHTER, BANDB) == 2)
    {
        return 1;
    }

    //�ж��Ƿ���Ϸʧ��
    Band whichband;
    for (whichband = BANDA; whichband <= BANDB; ++whichband)
    {
        if (if_who_at(stage, DAD, whichband) == 0 && if_who_at(stage, MOM, whichband) > 0 && if_who_at(stage, SON, whichband) > 0)
        {
            return 2;
        }
        if (if_who_at(stage, MOM, whichband) == 0 && if_who_at(stage, DAD, whichband) > 0 && if_who_at(stage, DAUGHTER, whichband) > 0)
        {
            return 2;
        }
        if (if_who_at(stage, POLICE, whichband) == 0 && if_who_at(stage, KILLER, whichband) > 0 && (if_who_at(stage, DAD, whichband) + if_who_at(stage, MOM, whichband) + if_who_at(stage, SON, whichband) + if_who_at(stage, DAUGHTER, whichband)) > 0)
        {
            return 2;
        }
    }
    if (ship_who_at_seatA(stage) == DAD && ship_who_at_seatB(stage) == DAUGHTER)
    {
        return 2;
    }
    if (ship_who_at_seatA(stage) == MOM && ship_who_at_seatB(stage) == SON)
    {
        return 2;
    }
    if (ship_who_at_seatB(stage) == KILLER && ship_who_at_seatA(stage) != POLICE)
    {
        return 2;
    }
    return 0;
}

int if_who_at(const Stage *stage, const Human who, const Band whichband)
{
    return stage->band[whichband][who];
}

Human ship_who_at_seatA(const Stage *stage)
{
    return stage->ship.seatA;
}
Human ship_who_at_seatB(const Stage *stage)
{
    return stage->ship.seatB;
}

int album_reset(Album *album)
{
    //�ȼ�������Ƿ����
    if (!album)
    {
        return -1; //*albumΪNULL
    }
    memset(album, 0, sizeof(Album));
    return 0;
}

//�������
int stage_snapshot(Album *album, const Stage *stage)
{
    //�ȼ�������Ƿ����
    if (!stage)
    {
        return -2; //*stageΪNULL
    }
    if (!album)
    {
        return -1; //*albumΪNULL
    }
    memcpy(&(album->stage[album->len]), stage, sizeof(Stage));
    return (album->len)++;
}

int album_check(const Album *album, const Stage *stage,const int except)
{
    //�ȼ�������Ƿ����
    if (!stage)
    {
        return -2; //*stageΪNULL
    }
    if (!album)
    {
        return -1; //*albumΪNULL
    }

    int index;
    for (index = 0; index < album->len; ++index)
    {
        if (index == except)
        {
            continue;
        }
        if (memcmp(&(album->stage[index]), stage, sizeof(Stage)) == 0)
        {
            return index; //�����±�
        }
    }
    return except; //����except
}

int gamestart(Stage *stage, Album *album)
{
    if (!stage)
    {
        return -1;
    }
    if (!album)
    {
        return -1;
    }
    stage_reset(stage);
    album_reset(album);
}

int stage_restore(Stage *stage,const Album *album,const int index)
{
    memcpy(stage, &(album->stage[index]), sizeof(Stage));
    return 0;
}

char *Human_to_str(const Human human)
{
    switch (human)
    {
    case DAD:
        return "�ְ�";
    case MOM:
        return "����";
    case POLICE:
        return "����";
    case KILLER:
        return "����";
    case DAUGHTER:
        return "Ů��";
    case SON:
        return "����";
    case NOBODY:
        return "���Ų�����";
    }
}

void stack_reset(Stack *stack)
{
    memset(stack, 0, sizeof(Stack));
    stack->tail = -1;
    return;
}

int stack_push(Stack *stack, const Human seatA, const Human seatB, const int album_index)
{
    ++(stack->tail);
    stack->node[stack->tail].seatA = seatA;
    stack->node[stack->tail].seatB = seatB;
    stack->node[stack->tail].album_index = album_index;
    return stack->tail;
}

Node stack_pop(Stack *stack)
{
    return stack->node[(stack->tail)--];
}