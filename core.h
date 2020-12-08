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
/*

#define DISK_SIZE 1024                          //磁盘块大小
#define DISK_NUM 1024                           //磁盘块数量
#define MEM_D_SIZE 1024*1024                    //总磁盘空间
#define FAT_SIZE DISK_NUM*sizeof(struct FAT)    //FAT表大小 1024 * 8


//todo:unfinished define

typedef struct FCB{
    char filename[10];                          //文件或目录名
    int inodeAddr;                              //inode结点地址
};
//todo:fcb数组

typedef struct inode{
    int attribute;                              //目录为1，文件为0
    int first_disk;                             //起始盘块号
    int file_size;                              //文件或目录大小
    int sub_dir;                                //子目录起始盘块号
    int is_root;                                //根目录为1，否则为0
};

typedef struct FAT{
    int next;                                   //下一个盘块号
    int is_empty;                               //盘块空闲为0
};

//todo:用户打开文件表

//全局变量
struct FAT *fat;
struct FCB *root;
*/


#define DISK_SIZE 1024                                      //磁盘块的大小
#define DISK_NUM 1024                                       //磁盘块的数量
#define MEM_D_SIZE 1024*1024                                //总磁盘空间为1M
#define FAT_SIZE DISK_NUM*sizeof(struct FAT)                //FAT表大小 1024*8
#define ROOT_DISK_NO FAT_SIZE/DISK_SIZE+1                   //根目录起始盘块号 9
#define ROOT_DISK_SIZE sizeof(struct FCB)                   //根目录大小 36
#define DIR_MAXSIZE 1024                                    //路径最大长度为
#define MSD 5                                               //最大子目录数
#define MOFN 5                                              //最大文件深度为


//文件控制块
typedef struct FCB
{
    char filename[10];                                      //文件或目录名
    char attribute;                                         //属性，目录还是文件，目录：1；文件：0
    int first;                                              //起始盘块号
    int size;                                               //文件或目录的大小
    int sub_dir;                                            //子目录起始盘块号
    int sign;                                               //1是根目录，0不是根目录
}fcb[MSD+2];

//文件分配表
struct FAT
{
    int next;                                               //指向下一个盘块号
    char empty_disk;                                        //磁盘块是否空闲，0表示为空闲
};

//用户打开文件表
struct opened_table
{
    struct open_table
    {
        char filename[10];                                  //文件名
        int first;                                          //起始盘块号
        int size;                                           //文件的大小
    }openitem[MOFN];
    int currence_count;                                     //当前打开文件的数量
};

//全局变量
struct FAT *fat;                                            //fat表数组
struct FCB *root;                                           //根目录数组
struct FCB *cur_dir;                                        //记录当前目录的数组
struct opened_table opening_table;                          //打开文件表
int fd=-1;                                                  //文件打开表的序号
char *currence_dir;                                         //记录当前路径
char *total_disk;                                           //虚拟磁盘起始地址
char user[10];			                                    //用户名

//函数声明
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


