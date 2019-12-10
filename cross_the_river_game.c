/*
游戏规则:
单位：1个爸爸，1个妈妈，2个儿子，2个女儿，1个土匪，1个警察
场地：1个河岸A，1个船，1个河岸B
初始：全员在河岸A
目标：全员在河岸B
爸爸、妈妈、警察可以控制船
船1次能载2人
如果没有妈妈，爸爸不能和女儿同时在河岸A，或同时在船上，或同时在河岸B
如果没有爸爸，妈妈不能和儿子同时在河岸A，或同时在船上，或同时在河岸B
如果没有警察，土匪不能和爸爸或妈妈或儿子或女儿同时在河岸A，或同时在船上，或同时在河岸B
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
    //先判断输入是否错误
    if (!stage)
    {
        return -1; //-1：*stage为NULL
    }

    Human who;
    if (stage)
    {
        stage->ship.address = BANDA;      //船.位置初始化在河岸A
        stage->ship.seatA = NOBODY;       //船.主驾驶座初始化为无人乘坐
        stage->ship.seatB = NOBODY;       //船.副驾驶座初始化为无人乘坐
        stage->band[BANDA][DAD] = 1;      //河岸A有1个爸爸
        stage->band[BANDA][MOM] = 1;      //河岸A有1个妈妈
        stage->band[BANDA][POLICE] = 1;   //河岸A有1个警察
        stage->band[BANDA][SON] = 2;      //河岸A有2个儿子
        stage->band[BANDA][DAUGHTER] = 2; //河岸A有2个女儿
        stage->band[BANDA][KILLER] = 1;   //河岸A有1个土匪
        for (who = NOBODY; who < _Human_END; ++who)
        {
            stage->band[BANDB][who] = 0; //河岸B无一人
        }
    }
    return 0; //0：成功
}

int ship_ferry(Stage *stage, const Human seatA, const Human seatB)
{
    //先判断输入是否错误
    if (!stage)
    {
        return -1; //*stage为NULL
    }
    if (seatA != DAD && seatA != MOM && seatA != POLICE)
    {
        return -2; //主驾驶座错误
    }
    if (seatA == seatB)
    {
        if (stage->band[stage->ship.address][seatA] - 2 < 0)
        {
            return -3; //上船的人不存在
        }
    }
    else if (seatB == NOBODY)
    {
        if (stage->band[stage->ship.address][seatA] - 1 < 0)
        {
            return -3; //上船的人不存在
        }
    }
    else
    {
        if (stage->band[stage->ship.address][seatA] - 1 < 0 || stage->band[stage->ship.address][seatB] - 1 < 0)
        {
            return -3; //上船的人不存在
        }
    }

    //将选择的单位扔上船
    stage->ship.seatA = seatA;
    --(stage->band[stage->ship.address][seatA]);
    stage->ship.seatB = seatB;
    if (seatB != NOBODY)
    {
        --(stage->band[stage->ship.address][seatB]);
    }

    //第一次judge
    switch (stage_judge(stage))
    {
    case 0: //无事发生
        break;
    case 1:       //游戏通关
        return 2; //2：游戏通关
        break;
    case 2:       //游戏失败
        return 1; //1：游戏失败
        break;
    case -1:       //舞台指针为NULL
        return -1; //-1：*stage为NULL
        break;
    }

    //过河
    stage->ship.address = !(stage->ship.address);

    //把船上的单位扔下船
    ++(stage->band[stage->ship.address][seatA]);
    stage->ship.seatA = NOBODY;
    if (seatB != NOBODY)
    {
        ++(stage->band[stage->ship.address][seatB]);
    }
    stage->ship.seatB = NOBODY;

    //第一次judge
    switch (stage_judge(stage))
    {
    case 0: //无事发生
        break;
    case 1:       //游戏通关
        return 2; //2：游戏通关
        break;
    case 2:       //游戏失败
        return 1; //1：游戏失败
        break;
    case -1:       //舞台指针为NULL
        return -1; //-1：舞台指针为NULL
        break;
    }

    //无事发生，返回0成功
    return 0;
}

int stage_judge(const Stage *stage)
{
    //先判断输入是否错误
    if (!stage)
    {
        return -1;
    }

    //判断是否游戏通关
    if (if_who_at(stage, DAD, BANDB) == 1 && if_who_at(stage, MOM, BANDB) == 1 && if_who_at(stage, POLICE, BANDB) == 1 && if_who_at(stage, KILLER, BANDB) == 1 && if_who_at(stage, SON, BANDB) == 2 && if_who_at(stage, DAUGHTER, BANDB) == 2)
    {
        return 1;
    }

    //判断是否游戏失败
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
    //先检测输入是否错误
    if (!album)
    {
        return -1; //*album为NULL
    }
    memset(album, 0, sizeof(Album));
    return 0;
}

//保存快照
int stage_snapshot(Album *album, const Stage *stage)
{
    //先检测输入是否错误
    if (!stage)
    {
        return -2; //*stage为NULL
    }
    if (!album)
    {
        return -1; //*album为NULL
    }
    memcpy(&(album->stage[album->len]), stage, sizeof(Stage));
    return (album->len)++;
}

int album_check(const Album *album, const Stage *stage,const int except)
{
    //先检测输入是否错误
    if (!stage)
    {
        return -2; //*stage为NULL
    }
    if (!album)
    {
        return -1; //*album为NULL
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
            return index; //返回下标
        }
    }
    return except; //返回except
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
        return "爸爸";
    case MOM:
        return "妈妈";
    case POLICE:
        return "警察";
    case KILLER:
        return "土匪";
    case DAUGHTER:
        return "女儿";
    case SON:
        return "儿子";
    case NOBODY:
        return "空着不坐人";
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