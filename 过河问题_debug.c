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

//����ɺ���gamestart()������������һ������Ϸͨ�ص����̣������������̴�ӡ����Ļ��

#include "cross_the_river_game.h"
#include <stdio.h>
#include <string.h>
#define MM(X) memset(X, 0, sizeof(X))

//����һ�����ڵ��������ջ
Stack stack;
int dfstimes = 0;
Human seatA[] = {DAD, MOM, POLICE};
Human seatB[] = {DAD, MOM, POLICE, KILLER, SON, DAUGHTER, NOBODY};

//0:ͨ��;1:��Ҫ����
int dfs(Stage *stage, Album *album, int image)
{
//��ӡ��ǰbandA�����
#define AA printf("�Ӱ�A:: DAD==%d MOM==%d POLICE==%d KILLER==%d SON==%d DAUGHTER== %d\n", if_who_at(stage, DAD, BANDA), if_who_at(stage, MOM, BANDA), if_who_at(stage, POLICE, BANDA), if_who_at(stage, KILLER, BANDA), if_who_at(stage, SON, BANDA), if_who_at(stage, DAUGHTER, BANDA));
//��ӡ��ǰbandB�����
#define BB printf("�Ӱ�B:: DAD==%d MOM==%d POLICE==%d KILLER==%d SON==%d DAUGHTER== %d\n", if_who_at(stage, DAD, BANDB), if_who_at(stage, MOM, BANDB), if_who_at(stage, POLICE, BANDB), if_who_at(stage, KILLER, BANDB), if_who_at(stage, SON, BANDB), if_who_at(stage, DAUGHTER, BANDB));
    int t = dfstimes++;
    // printf("\n���ǵ�%d��dfs\n", t);
    int a, b;
    for (a = 0; a < 3; ++a)
    {
        for (b = 0; b < 7; ++b)
        {
            //�ָ��浵
            stage_restore(stage, album, image);
            switch (ship_ferry(stage, seatA[a], seatB[b]))
            {

            case -2:
            case -3:
            case 1:
                //����ʧ�ܣ�����ѭ��
                // printf("\n���ǵ�%d��dfs::����ʧ��,seatA==%d,seatB==%d\n",t,seatA[a], seatB[b]);
                break;

            case 2:
            {
                //�ҵ�ͨ��·�ߣ����⴫��
                //�½����գ����ڴ�����Ϣ�����
                int newshop = stage_snapshot(album, stage);

                // printf("\n���ǵ�%d��dfs::ͨ��·��,����ʻ����%s,����ʻ����%s\n", t, Human_to_str(seatA[a]), Human_to_str(seatB[b]));
                // AA BB
                stack_push(&stack, seatA[a], seatB[b], newshop);
                return 0;
                break;
            }
            case 0:
                //�����ɹ����Ȳ��أ��ٽ�����һ������
                {
                    int check_result = album_check(album, stage, image);
                    if (check_result == image)
                    {
                        // printf("\n���ǵ�%d��dfs::û���ظ�����������\n", t);
                        // printf("image==%d\n", image);
                        // printf("check_result==%d\n", check_result);
                        // AA BB
                        //û���ظ����������������
                        //���½�����
                        int newshop = stage_snapshot(album, stage);
                        //��������
                        int dfs_result = dfs(stage, album, newshop);
                        switch (dfs_result)
                        {
                        case 1:
                            break;
                        case 0:
                            //���⴫��
                            //�ָ��浵
                            // stage_restore(stage, album, newshop);
                            // printf("\n���ǵ�%d��dfs::ͨ��·��,����ʻ����%s,����ʻ����%s\n", t, Human_to_str(seatA[a]), Human_to_str(seatB[b]));
                            // AA BB
                            stack_push(&stack, seatA[a], seatB[b], newshop);
                            return 0;
                            break;
                        }
                    }
                    else
                    {
                        // printf("\n���ǵ�%d��dfs::�ظ��ˣ�����ѭ��\n", t);
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

    //�Ƚ���һ������,�����ʼ״̬����Ϣ
    int image = stage_snapshot(&album, &stage);

    //��ʼ������õ�ջ
    stack_reset(&stack);
    //����dfs
    dfs(&stage, &album, image);

    int step = 0;
    int end = stack.tail;
    while (step <= end)
    {
        Node node = stack_pop(&stack);
        printf(node.seatB==NOBODY?"\n��%d����%s ":"\n��%d����%s �� %s ", 1+step++, Human_to_str(node.seatA), Human_to_str(node.seatB));
        printf(album.stage[node.album_index].ship.address == 1 ? "����\n" : "����\n");
        printf("����������������ǣ�\n");
        printf("�Ӱ�A   :   %d���ְ�  ��  %d������    ��  %d������    ��  %d������    ��  %d������   ��  %d��Ů��\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDA), if_who_at(&(album.stage[node.album_index]), MOM, BANDA), if_who_at(&(album.stage[node.album_index]), POLICE, BANDA), if_who_at(&(album.stage[node.album_index]), KILLER, BANDA), if_who_at(&(album.stage[node.album_index]), SON, BANDA), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDA));
        printf("�Ӱ�B   :   %d���ְ�  ��  %d������    ��  %d������    ��  %d������    ��  %d������   ��  %d��Ů��\n", if_who_at(&(album.stage[node.album_index]), DAD, BANDB), if_who_at(&(album.stage[node.album_index]), MOM, BANDB), if_who_at(&(album.stage[node.album_index]), POLICE, BANDB), if_who_at(&(album.stage[node.album_index]), KILLER, BANDB), if_who_at(&(album.stage[node.album_index]), SON, BANDB), if_who_at(&(album.stage[node.album_index]), DAUGHTER, BANDB));
    }

    return 0;
}