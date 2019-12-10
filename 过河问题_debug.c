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

//请完成函数gamestart()来搜索出至少一个能游戏通关的流程，并将操作过程打印在屏幕上

#include "cross_the_river_game.h"
#include <stdio.h>
#include <string.h>
#define MM(X) memset(X, 0, sizeof(X))

//定义一个用于倒序输出的栈
Stack stack;
int dfstimes = 0;
Human seatA[] = {DAD, MOM, POLICE};
Human seatB[] = {DAD, MOM, POLICE, KILLER, SON, DAUGHTER, NOBODY};

//0:通关;1:需要回退
int dfs(Stage *stage, Album *album, int image)
{
//打印当前bandA的情况
#define AA printf("河岸A:: DAD==%d MOM==%d POLICE==%d KILLER==%d SON==%d DAUGHTER== %d\n", if_who_at(stage, DAD, BANDA), if_who_at(stage, MOM, BANDA), if_who_at(stage, POLICE, BANDA), if_who_at(stage, KILLER, BANDA), if_who_at(stage, SON, BANDA), if_who_at(stage, DAUGHTER, BANDA));
//打印当前bandB的情况
#define BB printf("河岸B:: DAD==%d MOM==%d POLICE==%d KILLER==%d SON==%d DAUGHTER== %d\n", if_who_at(stage, DAD, BANDB), if_who_at(stage, MOM, BANDB), if_who_at(stage, POLICE, BANDB), if_who_at(stage, KILLER, BANDB), if_who_at(stage, SON, BANDB), if_who_at(stage, DAUGHTER, BANDB));
    int t = dfstimes++;
    // printf("\n这是第%d次dfs\n", t);
    int a, b;
    for (a = 0; a < 3; ++a)
    {
        for (b = 0; b < 7; ++b)
        {
            //恢复存档
            stage_restore(stage, album, image);
            switch (ship_ferry(stage, seatA[a], seatB[b]))
            {

            case -2:
            case -3:
            case 1:
                //操作失败，继续循环
                // printf("\n这是第%d次dfs::操作失败,seatA==%d,seatB==%d\n",t,seatA[a], seatB[b]);
                break;

            case 2:
            {
                //找到通关路线，向外传递
                //新建快照，用于传递信息给外层
                int newshop = stage_snapshot(album, stage);

                // printf("\n这是第%d次dfs::通关路线,主驾驶座是%s,副驾驶座是%s\n", t, Human_to_str(seatA[a]), Human_to_str(seatB[b]));
                // AA BB
                stack_push(&stack, seatA[a], seatB[b], newshop);
                return 0;
                break;
            }
            case 0:
                //操作成功，先查重，再进行下一步搜索
                {
                    int check_result = album_check(album, stage, image);
                    if (check_result == image)
                    {
                        // printf("\n这是第%d次dfs::没有重复，向下搜索\n", t);
                        // printf("image==%d\n", image);
                        // printf("check_result==%d\n", check_result);
                        // AA BB
                        //没有重复的情况，向下搜索
                        //先新建快照
                        int newshop = stage_snapshot(album, stage);
                        //向下搜索
                        int dfs_result = dfs(stage, album, newshop);
                        switch (dfs_result)
                        {
                        case 1:
                            break;
                        case 0:
                            //向外传递
                            //恢复存档
                            // stage_restore(stage, album, newshop);
                            // printf("\n这是第%d次dfs::通关路线,主驾驶座是%s,副驾驶座是%s\n", t, Human_to_str(seatA[a]), Human_to_str(seatB[b]));
                            // AA BB
                            stack_push(&stack, seatA[a], seatB[b], newshop);
                            return 0;
                            break;
                        }
                    }
                    else
                    {
                        // printf("\n这是第%d次dfs::重复了，继续循环\n", t);
                        // AA BB
                    }
                    break;
                }
            }
        }
    }
    return 1;
}

int main()
{
    Stage stage;
    Album album;
    gamestart(&stage, &album);

    //先建立一个镜像,保存初始状态的信息
    int image = stage_snapshot(&album, &stage);

    //初始化输出用的栈
    stack_reset(&stack);
    //进入dfs
    dfs(&stage, &album, image);

    int step = 0;
    int end = stack.tail;
    while (step <= end)
    {
        Node node = stack_pop(&stack);
        printf(node.seatB==NOBODY?"\n第%d步：%s ":"\n第%d步：%s 和 %s ", 1+step++, Human_to_str(node.seatA), Human_to_str(node.seatB));
        printf(album.stage[node.album_index].ship.address == 1 ? "过河\n" : "回来\n");
        printf("操作后，两岸的情况是：\n");
        printf("河岸A   :   %d个爸爸  ；  %d个妈妈    ；  %d个警察    ；  %d个土匪    ；  %d个儿子   ；  %d个女儿\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDA), if_who_at(&(album.stage[node.album_index]), MOM, BANDA), if_who_at(&(album.stage[node.album_index]), POLICE, BANDA), if_who_at(&(album.stage[node.album_index]), KILLER, BANDA), if_who_at(&(album.stage[node.album_index]), SON, BANDA), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDA));
        printf("河岸B   :   %d个爸爸  ；  %d个妈妈    ；  %d个警察    ；  %d个土匪    ；  %d个儿子   ；  %d个女儿\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDB), if_who_at(&(album.stage[node.album_index]), MOM, BANDB), if_who_at(&(album.stage[node.album_index]), POLICE, BANDB), if_who_at(&(album.stage[node.album_index]), KILLER, BANDB), if_who_at(&(album.stage[node.album_index]), SON, BANDB), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDB));
    }

    return 0;
}