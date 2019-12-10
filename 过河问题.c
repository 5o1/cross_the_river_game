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

#include <stdio.h>
#include <string.h>
#include "cross_the_river_game.h"

//ȫ�ֱ���
Stack stack;

//0:ͨ��;1:��Ҫ����
int dfs(Stage *stage, Album *album, const int image)
{
    static const Human seatA[] = {DAD, MOM, POLICE};
    static const Human seatB[] = {DAD, MOM, POLICE, KILLER, SON, DAUGHTER, NOBODY};
    int a, b;
    for (a = 0; a < 3; ++a)
    {
        for (b = 0; b < 7; ++b)
        {
            //�ָ��浵
            stage_restore(stage, album, image);
            switch (ship_ferry(stage, seatA[a], seatB[b]))
            {
            case 2: //�ҵ�ͨ��·�ߣ����⴫��
            {
                //�½����գ����ڴ�����Ϣ�����
                int newshop = stage_snapshot(album, stage);
                stack_push(&stack, seatA[a], seatB[b], newshop);
                return 0;
            }
            case 0:                                            //�����ɹ����Ȳ��أ��ٽ�����һ������
                if (album_check(album, stage, image) == image) //û���ظ����������������
                {
                    //���½�����
                    int newshop = stage_snapshot(album, stage);
                    //��������
                    switch (dfs(stage, album, newshop))
                    {
                    case 0:
                        //���⴫��
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

    //��ʼ����Ϸ�����Ϳ�������
    gamestart(&stage, &album);
    //��ʼ������õ�ջ
    stack_reset(&stack);
    //�½�һ���������ڱ����ʼ״̬
    int image = stage_snapshot(&album, &stage);

    //��ʼdfs
    dfs(&stage, &album, image);

    //��ӡ���
    int step = 0;
    int end = stack.tail;
    while (step <= end)
    {
        Node node = stack_pop(&stack);
        printf("\n��%d��:\n", 1 + step++);
        printf("\t�ְ�\t����\t����\t����\t����\tŮ��\n");
        printf("�Ӱ�A\t  %d\t  %d\t  %d\t  %d\t  %d\t  %d\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDA), if_who_at(&(album.stage[node.album_index]), MOM, BANDA), if_who_at(&(album.stage[node.album_index]), POLICE, BANDA), if_who_at(&(album.stage[node.album_index]), KILLER, BANDA), if_who_at(&(album.stage[node.album_index]), SON, BANDA), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDA));
        if(album.stage[node.album_index].ship.address == 1){
            if(node.seatA==DAD || node.seatB== DAD){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==MOM || node.seatB== MOM){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==POLICE || node.seatB== POLICE){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==KILLER || node.seatB== KILLER){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==SON || node.seatB== SON){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==DAUGHTER || node.seatB== DAUGHTER){
                printf("\t������");
            }else{
                printf("\t");
            }
        }else{
            if(node.seatA==DAD || node.seatB== DAD){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==MOM || node.seatB== MOM){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==POLICE || node.seatB== POLICE){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==KILLER || node.seatB== KILLER){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==SON || node.seatB== SON){
                printf("\t������");
            }else{
                printf("\t");
            }
            if(node.seatA==DAUGHTER || node.seatB== DAUGHTER){
                printf("\t������");
            }else{
                printf("\t");
            }
        }
        printf("\n");
        printf("�Ӱ�B\t  %d\t  %d\t  %d\t  %d\t  %d\t  %d\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDB), if_who_at(&(album.stage[node.album_index]), MOM, BANDB), if_who_at(&(album.stage[node.album_index]), POLICE, BANDB), if_who_at(&(album.stage[node.album_index]), KILLER, BANDB), if_who_at(&(album.stage[node.album_index]), SON, BANDB), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDB));
    }

    return 0;
}