//
// Created by nianzhechao on 2020/12/7.
//

#ifndef OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H
#define OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H

#endif //OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H

#include "core.h"


//格式化磁盘空间
void format() {
    int i;
    FILE *file;
    total_disk = (char *) malloc(sizeof(char) * (MEM_D_SIZE));
    //申请空间
    fat = (struct FAT *) (total_disk + DISK_SIZE);
    //FAT表起始地址
    fat[0].next = -1;
    fat[0].empty_disk = '1';
    for (i = 1; i < ROOT_DISK_NO - 1; i++) {
        //将存放FAT表的磁盘块号全部标志为已用
        fat[i].next = i + 1;
        fat[i].empty_disk = '1';
    }
    fat[ROOT_DISK_NO].next = -1;
    //存放根目录的磁盘块号
    fat[ROOT_DISK_NO].empty_disk = '1';
    for (i = ROOT_DISK_NO + 1; i < DISK_NUM; i++) {
        fat[i].next = -1;
        fat[i].empty_disk = '0';
    }
    root = (struct FCB *) (total_disk + DISK_SIZE + FAT_SIZE);
    //根目录的地址
    //指向当前目录的目录项
    root[0].sign = 1;
    root[0].first = ROOT_DISK_NO;
    strcpy(root[0].filename, ".");
    root[0].sub_dir = root[0].first;
    root[0].attribute = '1';
    root[0].size = ROOT_DISK_SIZE;
    //指向上一级目录的目录项
    root[1].sign = 1;
    root[1].first = ROOT_DISK_NO;
    strcpy(root[1].filename, "..");
    root[1].sub_dir = root[0].first;
    root[1].attribute = '1';
    root[1].size = ROOT_DISK_SIZE;
    //初始化子目录
    for (i = 2; i < MSD + 2; i++) {
        root[i].sign = 0;
        root[i].first = -1;
        strcpy(root[i].filename, "");
        root[i].sub_dir = -1;
        root[i].attribute = '0';
        root[i].size = 0;
    }
    if ((file = fopen("disk.dat", "wb")) == NULL) {
        printf("不能打开文件 \n");
        return;
    }

    if ((file = fopen("disk.dat", "wb")) == NULL) {
        printf("不能打开文件\n");
        return;
    }
    if (fwrite(total_disk, MEM_D_SIZE, 1, file) != 1)
        //把虚拟磁盘空间保存到磁盘文件中
        printf("失败\n");
    fclose(file);
}

//用户登录
void user_load() {
    char password[10];
    printf("请输入用户名：");
    scanf("%s", &user);
    printf("请输入密码：");
    scanf("%s", &password);
    while (1) {
        if (strcmp(password, "123456") == 0)
            break;
        else {
            printf("密码错误，请重试！\n");
            printf("请输入密码：");
            scanf("%s", &password);
        }
    }
    printf("---------------------------------------------------\n\n");
}


//若之前已经登陆过，则直接进入系统
void enter() {
    FILE *fp;
    int i;
    char name[20];
    total_disk = (char *) malloc(MEM_D_SIZE * sizeof(char));
    if ((fp = fopen("disk.dat", "rb")) == NULL) {
        printf("Error:Cannot open file\n");
        return;
    }
    if (!fread(total_disk, MEM_D_SIZE, 1, fp)) {
        printf("Error:Cannot read file\n");
        exit(0);
    }
    fat = (struct FAT *) (total_disk + DISK_SIZE);
    //找到FAT表地址
    root = (struct FCB *) (total_disk + DISK_SIZE + FAT_SIZE);
    //找到根目录地址
    fclose(fp);

    //将用户打开表进行初始化
    for (i = 0; i < MOFN; i++) {
        strcpy(opening_table.openitem[i].filename, "");
        opening_table.openitem[i].first = -1;
        opening_table.openitem[i].size = 0;
    }
    opening_table.currence_count = 0;
    cur_dir = root; //将根目录设置为当前目录
    currence_dir = (char *) malloc(DIR_MAXSIZE * sizeof(char));
    strcpy(name, strcat(user, ": "));
    strcpy(currence_dir, name);
}

//用户注销
void withdraw() {
    FILE *fp;
    if ((fp = fopen("disk.dat", "wb")) == NULL) {
        printf("Error:Cannot open file\n");
        return;
    }
    //把内容读入磁盘文件disk.dat
    if (!fwrite(total_disk, MEM_D_SIZE, 1, fp)) {
        printf("Error:File write error!\n");
    }
    fclose(fp);
    free(total_disk);
    free(currence_dir);
    return;
}

//打开文件
int open(char *name) {
    int i, j;
    //查看文件是否存在
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name))
            break;
    }
    if (i >= MSD + 2) {
        printf("文件不存在!\n");
        return -1;
    }
    //是文件还是目录
    if (cur_dir[i].attribute == '1') {
        printf("是目录，无法打开\n");
        return -1;
    }
    //查看文件是否打开
    for (j = 0; j < MOFN; j++) {
        if (!strcmp(opening_table.openitem[j].filename, name)) {
            printf("文件已经打开!\n");
            return -1;
        }
    }
    //如果打开文件超出限制
    if (opening_table.currence_count >= MOFN) {
        printf("文件打开超出限制!\n");
        return -1;
    }
    //查找一个空闲用户打开表项
    for (j = 0; j < MOFN; j++) {
        if (opening_table.openitem[j].first == -1)
            break;
    }
    //填写表项的相关信息
    opening_table.openitem[j].first = cur_dir[i].first;
    strcpy(opening_table.openitem[j].filename, name);
    opening_table.openitem[j].size = cur_dir[i].size;
    opening_table.currence_count++;
    printf("打开成功!\n");
    return 0;
}

//关闭文件
int close(char *name) {
    int i;
    for (i = 0; i < MOFN; i++) {
        if (!strcmp(opening_table.openitem[i].filename, name))
            break;
    }
    if (i >= MOFN) {
        printf("错误!\n");
    }
    //清空该文件的用户打开表项的内容
    strcpy(opening_table.openitem[i].filename, "");
    opening_table.openitem[i].first = -1;
    opening_table.openitem[i].size = 0;
    opening_table.currence_count--;
    return 0;
}

//创建文件
int create(char *name) {
    int i, j;
    if (strlen(name) > 10) {
        //文件名长度限制为10
        printf("文件名超出限制!\n");
        return -1;
    }
    //检查文件名是否已存在
    for (j = 2; j < MSD + 2; j++) {
        if (strcmp(cur_dir[j].filename, name) == 0)
            break;
    }
    if (j < MSD + 2) {
        printf("文件名已存在!\n");
        return -1;
    }

    //如果不是，则没有重名的情况，需要找到空闲区
    for (i = 2; i < MSD + 2; i++) {
        if (cur_dir[i].first == -1)
            break;
    }
    if (i >= MSD + 2) {
        printf("目录项已满!\n");
        return -1;
    }

    if (opening_table.currence_count >= MOFN) //打开文件太多
    {
        printf("打开文件数已超出限制\n");
        return -1;
    }
    //找到空闲盘块
    for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
        if (fat[j].empty_disk == '0')
            break;
    }
    if (j >= DISK_NUM)    //没有空闲盘块
    {
        printf("磁盘空间已满!\n");
        return -1;
    }
    fat[j].empty_disk = '1';
    //将信息填入目录项中
    strcpy(cur_dir[i].filename, name);
    cur_dir[i].first = j;
    cur_dir[i].size = 0;
    cur_dir[i].sub_dir = j;
    cur_dir[i].attribute = '0';
    return 0;
}

//删除文件
int del(char *name) {
    int i, j;
    int cur_item; //用于存储要删除文件在cur_dir中的下标
    int item;    //存储要删除文件的起始盘块号
    int temp;
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name)) {
            cur_item = i;
            break;
        }
    }

    if (i >= MSD + 2) {
        printf("输入的文件名不存在!\n");
        return -1;
    }
    if (cur_dir[cur_item].attribute != '0') {
        printf("删除的不是文件!\n");
        return -1;
    }
    //如果文件是打开状态，则不能删除
    for (j = 0; j < MOFN; j++) {
        if (strcmp(opening_table.openitem[j].filename, name) == 0) {
            printf("文件处于打开状态，不能删除!\n");
            return -1;
        }
    }
    item = cur_dir[cur_item].first;
    //释放此文件所占用的FAT表的空间
    while (item != -1) {
        temp = fat[item].next;
        fat[item].next = -1;
        fat[item].empty_disk = '0';
        item = temp;
    }
    //释放此文件所占用的目录项
    cur_dir[cur_item].sign = 0;
    cur_dir[cur_item].first = -1;
    strcpy(opening_table.openitem[cur_item].filename, "");
    cur_dir[cur_item].sub_dir = -1;
    cur_dir[cur_item].attribute = '0';
    cur_dir[cur_item].size = 0;
    return 0;
}

//创建目录
int mkdir(char *name) {
    int i, j;
    struct FCB *cur_mkdir;
    if (strlen(name) > 10) {
        printf("名字超出长度限制!\n");
        return -1;
    }
    for (j = 2; j < MSD + 2; j++) {
        if (strcmp(cur_dir[j].filename, name) == 0) {
            printf("该名字已存在!\n");
            break;
        }
    }
    for (i = 2; i < MSD + 2; i++) //如果有空闲目录项
    {
        if (cur_dir[i].first == -1)
            break;
    }
    if (i >= MSD + 2) {
        printf("目录容量已满\n");
        return -1;
    }
    //找到空闲磁盘块
    for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
        if (fat[j].empty_disk == '0') {
            fat[j].empty_disk = '1'; //将该空闲块设置为已分配
            break;
        }
    }
    if (j >= DISK_NUM) {
        printf("磁盘空间已满!\n");
        return -1;
    }
    //填写目录项
    strcpy(cur_dir[i].filename, name);
    cur_dir[i].first = j;
    cur_dir[i].size = ROOT_DISK_SIZE;
    cur_dir[i].sub_dir = j;
    cur_dir[i].attribute = '1';

    //初始化目录
    cur_mkdir = (struct FCB *) (total_disk + cur_dir[i].first * DISK_SIZE);
    //目录的地址
    //指向当前目录的目录项
    cur_mkdir[0].sign = 0;
    cur_mkdir[0].first = cur_dir[i].first;
    strcpy(cur_mkdir[0].filename, ".");
    cur_mkdir[0].sub_dir = cur_mkdir[0].first;
    cur_mkdir[0].attribute = '1';
    cur_mkdir[0].size = ROOT_DISK_SIZE;

    //指向上一级目录的目录项
    cur_mkdir[1].sign = cur_dir[0].sign;
    cur_mkdir[1].first = cur_dir[0].first;
    strcpy(cur_mkdir[1].filename, "..");
    cur_mkdir[1].sub_dir = cur_mkdir[1].first;
    cur_mkdir[1].attribute = '1';
    cur_mkdir[1].size = ROOT_DISK_SIZE;

    //子目录都初始化为空
    for (i = 2; i < MSD + 2; i++) {
        cur_mkdir[i].sign = 0;
        cur_mkdir[i].first = -1;
        strcpy(cur_mkdir[i].filename, "");
        cur_mkdir[i].sub_dir = -1;
        cur_mkdir[i].attribute = '0';
        cur_mkdir[i].size = 0;
    }

    return 0;
}

//删除目录
int rmdir(char *name) {
    int i, j, item;
    struct FCB *temp_dir;
    //检查当前目录项中有无该目录
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name))
            break;
    }
    if (i >= MSD + 2) {
        printf("该目录不存在!\n");
        return -1;
    }
    if (cur_dir[i].attribute != '1') {
        printf("删除的不是目录!\n");
        return -1;
    }
    //判断要删除的目录有无子目录
    temp_dir = (struct FCB *) (total_disk + cur_dir[i].sub_dir * DISK_SIZE);
    for (j = 2; j < MSD + 2; j++) {
        if (temp_dir[j].sub_dir != -1) {
            printf("该目录下有子目录，请先删除子目录\n");
            return -1;
        }
    }
    //找到起始盘块号,并将其释放
    item = cur_dir[i].first;
    fat[item].empty_disk = '0';

    //修改目录项
    cur_dir[i].sign = 0;
    cur_dir[i].first = -1;
    strcpy(cur_dir[i].filename, "");
    cur_dir[i].sub_dir = -1;
    cur_dir[i].attribute = '0';
    cur_dir[i].size = 0;
    return 0;
}

//显示当前子目录的内容
void ls() {
    int i;
    for (i = 2; i < MSD + 2; i++) {
        if (cur_dir[i].first != -1) //如果存在子目录
        {
            printf("%s\t", cur_dir[i].filename);
            if (cur_dir[i].attribute == '0') {
                printf("\t<文件>\t");
                printf("\t起始盘块号:%d\n", cur_dir[i].first);

            } else {
                printf("\t<目录>\t");
                printf("\t起始盘块号:%d\n", cur_dir[i].first);
            }
        }
    }
}

int cd(char *name) {
    int i, j, item;
    char *str;
    char *temp, *point, *point1;
    struct FCB *temp_dir;
    temp_dir = cur_dir;
    str = name;
    if (!strcmp("\\", name)) {
        cur_dir = root;
        strcpy(currence_dir, "Root:");
        return 0;
    }
    //为路径名字分配空间
    temp = (char *) malloc(DIR_MAXSIZE * sizeof(char));
    for (i = 0; i < (int) strlen(str); i++)
        temp[i] = str[i];
    temp[i] = '\0';
    //查找该子目录是否在当前目录中
    for (j = 0; j < MSD + 2; j++) {
        if (!strcmp(temp_dir[j].filename, temp))
            break;
    }
    free(temp);//释放申请的临时空间
    //没有这个目录
    if (j >= MSD + 2) {
        printf("路径不正确\n");
        return -1;
    }
    item = temp_dir[j].first;
    //当前目录在磁盘中位置
    temp_dir = (struct FCB *) (total_disk + item * DISK_SIZE);
    if (strcmp("..", name) == 0) {
        if (cur_dir[j - 1].sign != 1) //如果上级目录不是根目录
        {
            point = strchr(currence_dir, '\\');
            //查找字符串currence_dir中首次出现字符\ 的位置
            while (point != NULL) {
                point1 = point + 1; //减去'\'所占的空间,记录下次查找的起始地址
                point = strchr(point1, '\\');
            }
            *(point1 - 1) = '\0'; //将上一级目录删除
        }
    } else {
        currence_dir = strcat(currence_dir, "\\"); //修改当前目录
        currence_dir = strcat(currence_dir, name);
    }
    cur_dir = temp_dir; //将当前目录确定下来
    return 0;
}

void print() {
    printf("\n\n**********************虚拟文件系统设计***********************\n\n");
    printf("\t\t\t命令格式说明\n\n");
    printf("\t\tcd	        更改当前目录\n");
    printf("\t\tformat		格式化磁盘空间\n");
    printf("\t\tmkdir		创建子目录\n");
    printf("\t\trmdir		删除子目录\n");
    printf("\t\tls	        显示当前目录的子目录\n");
    printf("\t\tcreate	        创建文件\n");
    printf("\t\tdel	        删除文件\n");
    printf("\t\topen	        打开文件\n");
    printf("\t\tclose		关闭文件\n");
    printf("\t\twithdraw	用户注销\n");
    printf("**************************************************************\n");
}
