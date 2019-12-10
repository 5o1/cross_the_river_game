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

#include <stdio.h>
#include <string.h>
#include "cross_the_river_game.h"

//全局变量
Stack stack;

//0:通关;1:需要回退
int dfs(Stage *stage, Album *album, const int image)
{
    static const Human seatA[] = {DAD, MOM, POLICE};
    static const Human seatB[] = {DAD, MOM, POLICE, KILLER, SON, DAUGHTER, NOBODY};
    int a, b;
    for (a = 0; a < 3; ++a)
    {
        for (b = 0; b < 7; ++b)
        {
            //恢复存档
            stage_restore(stage, album, image);
            switch (ship_ferry(stage, seatA[a], seatB[b]))
            {
            case 2: //找到通关路线，向外传递
            {
                //新建快照，用于传递信息给外层
                int newshop = stage_snapshot(album, stage);
                stack_push(&stack, seatA[a], seatB[b], newshop);
                return 0;
            }
            case 0:                                            //操作成功，先查重，再进行下一步搜索
                if (album_check(album, stage, image) == image) //没有重复的情况，向下搜索
                {
                    //先新建快照
                    int newshop = stage_snapshot(album, stage);
                    //向下搜索
                    switch (dfs(stage, album, newshop))
                    {
                    case 0:
                        //向外传递
                        stack_push(&stack, seatA[a], seatB[b], newshop);
                        return 0;
                    }
                }
                break;
            }
        }
    }
    return 1;
}

int main()
{
    Stage stage;
    Album album;

    //初始化游戏场景和快照序列
    gamestart(&stage, &album);
    //初始化输出用的栈
    stack_reset(&stack);
    //新建一个镜像用于保存初始状态
    int image = stage_snapshot(&album, &stage);

    //开始dfs
    dfs(&stage, &album, image);

    //打印结果
    int step = 0;
    int end = stack.tail;
    while (step <= end)
    {
        Node node = stack_pop(&stack);
        printf("\n第%d步:\n", 1 + step++);
        printf("\t爸爸\t妈妈\t警察\t土匪\t儿子\t女儿\n");
        printf("河岸A\t  %d\t  %d\t  %d\t  %d\t  %d\t  %d\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDA), if_who_at(&(album.stage[node.album_index]), MOM, BANDA), if_who_at(&(album.stage[node.album_index]), POLICE, BANDA), if_who_at(&(album.stage[node.album_index]), KILLER, BANDA), if_who_at(&(album.stage[node.album_index]), SON, BANDA), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDA));
        if(album.stage[node.album_index].ship.address == 1){
            if(node.seatA==DAD || node.seatB== DAD){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
            if(node.seatA==MOM || node.seatB== MOM){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
            if(node.seatA==POLICE || node.seatB== POLICE){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
            if(node.seatA==KILLER || node.seatB== KILLER){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
            if(node.seatA==SON || node.seatB== SON){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
            if(node.seatA==DAUGHTER || node.seatB== DAUGHTER){
                printf("\t↓↓↓");
            }else{
                printf("\t");
            }
        }else{
            if(node.seatA==DAD || node.seatB== DAD){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
            if(node.seatA==MOM || node.seatB== MOM){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
            if(node.seatA==POLICE || node.seatB== POLICE){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
            if(node.seatA==KILLER || node.seatB== KILLER){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
            if(node.seatA==SON || node.seatB== SON){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
            if(node.seatA==DAUGHTER || node.seatB== DAUGHTER){
                printf("\t↑↑↑");
            }else{
                printf("\t");
            }
        }
        printf("\n");
        printf("河岸B\t  %d\t  %d\t  %d\t  %d\t  %d\t  %d\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDB), if_who_at(&(album.stage[node.album_index]), MOM, BANDB), if_who_at(&(album.stage[node.album_index]), POLICE, BANDB), if_who_at(&(album.stage[node.album_index]), KILLER, BANDB), if_who_at(&(album.stage[node.album_index]), SON, BANDB), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDB));
    }

    return 0;
}