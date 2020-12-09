//
// Created by nianzhechao.
//

#ifndef OS_VIRTUAL_FILE_SYSTEM_CORE_H
#define OS_VIRTUAL_FILE_SYSTEM_CORE_H

#endif //OS_VIRTUAL_FILE_SYSTEM_CORE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define DISK_SIZE 1024                                      //���̿��С 1024
#define DISK_NUM 1024                                       //���̿����� 1024
#define MEM_D_SIZE 1024*1024                                //�ܴ��̿ռ� 1M
#define FAT_SIZE DISK_NUM*sizeof(struct FAT)                //FAT���С 1024*8
#define ROOT_DISK_NO FAT_SIZE/DISK_SIZE+1                   //��Ŀ¼��ʼ�̿�� 9
#define ROOT_DISK_SIZE sizeof(struct FCB)                   //��Ŀ¼��С 36
#define DIR_MAXSIZE 1024                                    //·����󳤶�
#define MSD 5                                               //�����Ŀ¼��
#define MOFN 5                                              //����ļ����


//�ļ����ƿ�
typedef struct FCB {
    char filename[10];                                      //�ļ���Ŀ¼��
    char attribute;                                         //���ԣ�Ŀ¼�����ļ���Ŀ¼��1���ļ���0
    int first;                                              //��ʼ�̿��
    int size;                                               //�ļ���Ŀ¼�Ĵ�С
    int sub_dir;                                            //��Ŀ¼��ʼ�̿��
    int sign;                                               //1�Ǹ�Ŀ¼��0���Ǹ�Ŀ¼
} fcb[MSD + 2];

//�ļ������
struct FAT {
    int next;                                               //ָ����һ���̿��
    char empty_disk;                                        //���̿��Ƿ���У�0��ʾΪ����
};

//�û����ļ���
struct opened_table {
    struct open_table {
        char filename[10];                                  //�ļ���
        int first;                                          //��ʼ�̿��
        int size;                                           //�ļ��Ĵ�С
    } openitem[MOFN];
    int currence_count;                                     //��ǰ���ļ�������
};

//ȫ�ֱ���
struct FAT *fat;                                            //fat������
struct FCB *root;                                           //��Ŀ¼����
struct FCB *cur_dir;                                        //��¼��ǰĿ¼������
struct opened_table opening_table;                          //���ļ���
int fd = -1;                                                //�ļ��򿪱�����
char *currence_dir;                                         //��¼��ǰ·��
char *total_disk;                                           //���������ʼ��ַ
char user[10];                                              //�û���

//��������
void format();

void user_load();

void enter();

void withdraw();

int open(char *name);

int close(char *name);

int create(char *name);

int del(char *name);

int mkdir(char *name);

int rmdir(char *name);

void ls();

int cd(char *name);

void print();


