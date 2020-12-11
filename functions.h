//
// Created by nianzhechao on 2020/12/7.
//

#ifndef OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H
#define OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H

#endif //OS_VIRTUAL_FILE_SYSTEM_FUNCTIONS_H

#include "core.h"


//��ʽ�����̿ռ�
void format() {
    int i;
    FILE *file;
    total_disk = (char *) malloc(sizeof(char) * (MEM_D_SIZE));
    //����ռ�
    fat = (struct FAT *) (total_disk + DISK_SIZE);
    //FAT����ʼ��ַ
    fat[0].next = -1;
    fat[0].empty_disk = '1';
    for (i = 1; i < ROOT_DISK_NO - 1; i++) {
        //�����FAT��Ĵ��̿��ȫ����־Ϊ����
        fat[i].next = i + 1;
        fat[i].empty_disk = '1';
    }
    fat[ROOT_DISK_NO].next = -1;
    //��Ÿ�Ŀ¼�Ĵ��̿��
    fat[ROOT_DISK_NO].empty_disk = '1';
    for (i = ROOT_DISK_NO + 1; i < DISK_NUM; i++) {
        fat[i].next = -1;
        fat[i].empty_disk = '0';
    }
    root = (struct FCB *) (total_disk + DISK_SIZE + FAT_SIZE);
    //��Ŀ¼�ĵ�ַ
    //ָ��ǰĿ¼��Ŀ¼��
    root[0].sign = 1;
    root[0].first = ROOT_DISK_NO;
    strcpy(root[0].filename, ".");
    root[0].sub_dir = root[0].first;
    root[0].attribute = '1';
    root[0].size = ROOT_DISK_SIZE;
    //ָ����һ��Ŀ¼��Ŀ¼��
    root[1].sign = 1;
    root[1].first = ROOT_DISK_NO;
    strcpy(root[1].filename, "..");
    root[1].sub_dir = root[0].first;
    root[1].attribute = '1';
    root[1].size = ROOT_DISK_SIZE;
    //��ʼ����Ŀ¼
    for (i = 2; i < MSD + 2; i++) {
        root[i].sign = 0;
        root[i].first = -1;
        strcpy(root[i].filename, "");
        root[i].sub_dir = -1;
        root[i].attribute = '0';
        root[i].size = 0;
    }
    if ((file = fopen("disk.dat", "wb")) == NULL) {
        printf("���ܴ��ļ� \n");
        return;
    }

    if ((file = fopen("disk.dat", "wb")) == NULL) {
        printf("���ܴ��ļ�\n");
        return;
    }
    if (fwrite(total_disk, MEM_D_SIZE, 1, file) != 1)
        //��������̿ռ䱣�浽�����ļ���
        printf("ʧ��\n");
    fclose(file);
}

//�û���¼
void user_load() {
    char password[10];
    printf("�������û�����");
    scanf("%s", &user);
    printf("���������룺");
    scanf("%s", &password);
    while (1) {
        if (strcmp(password, "123456") == 0)
            break;
        else {
            printf("������������ԣ�\n");
            printf("���������룺");
            scanf("%s", &password);
        }
    }
    printf("---------------------------------------------------\n\n");
}


//��֮ǰ�Ѿ���½������ֱ�ӽ���ϵͳ
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
    //�ҵ�FAT���ַ
    root = (struct FCB *) (total_disk + DISK_SIZE + FAT_SIZE);
    //�ҵ���Ŀ¼��ַ
    fclose(fp);

    //���û��򿪱���г�ʼ��
    for (i = 0; i < MOFN; i++) {
        strcpy(opening_table.openitem[i].filename, "");
        opening_table.openitem[i].first = -1;
        opening_table.openitem[i].size = 0;
    }
    opening_table.currence_count = 0;
    cur_dir = root; //����Ŀ¼����Ϊ��ǰĿ¼
    currence_dir = (char *) malloc(DIR_MAXSIZE * sizeof(char));
    strcpy(name, strcat(user, ": "));
    strcpy(currence_dir, name);
}

//�û�ע��
void withdraw() {
    FILE *fp;
    if ((fp = fopen("disk.dat", "wb")) == NULL) {
        printf("Error:Cannot open file\n");
        return;
    }
    //�����ݶ�������ļ�disk.dat
    if (!fwrite(total_disk, MEM_D_SIZE, 1, fp)) {
        printf("Error:File write error!\n");
    }
    fclose(fp);
    free(total_disk);
    free(currence_dir);
    return;
}

//���ļ�
int open(char *name) {
    int i, j;
    //�鿴�ļ��Ƿ����
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name))
            break;
    }
    if (i >= MSD + 2) {
        printf("�ļ�������!\n");
        return -1;
    }
    //���ļ�����Ŀ¼
    if (cur_dir[i].attribute == '1') {
        printf("��Ŀ¼���޷���\n");
        return -1;
    }
    //�鿴�ļ��Ƿ��
    for (j = 0; j < MOFN; j++) {
        if (!strcmp(opening_table.openitem[j].filename, name)) {
            printf("�ļ��Ѿ���!\n");
            return -1;
        }
    }
    //������ļ���������
    if (opening_table.currence_count >= MOFN) {
        printf("�ļ��򿪳�������!\n");
        return -1;
    }
    //����һ�������û��򿪱���
    for (j = 0; j < MOFN; j++) {
        if (opening_table.openitem[j].first == -1)
            break;
    }
    //��д����������Ϣ
    opening_table.openitem[j].first = cur_dir[i].first;
    strcpy(opening_table.openitem[j].filename, name);
    opening_table.openitem[j].size = cur_dir[i].size;
    opening_table.currence_count++;
    printf("�򿪳ɹ�!\n");
    return 0;
}

//�ر��ļ�
int close(char *name) {
    int i;
    for (i = 0; i < MOFN; i++) {
        if (!strcmp(opening_table.openitem[i].filename, name))
            break;
    }
    if (i >= MOFN) {
        printf("����!\n");
    }
    //��ո��ļ����û��򿪱��������
    strcpy(opening_table.openitem[i].filename, "");
    opening_table.openitem[i].first = -1;
    opening_table.openitem[i].size = 0;
    opening_table.currence_count--;
    return 0;
}

//�����ļ�
int create(char *name) {
    int i, j;
    if (strlen(name) > 10) {
        //�ļ�����������Ϊ10
        printf("�ļ�����������!\n");
        return -1;
    }
    //����ļ����Ƿ��Ѵ���
    for (j = 2; j < MSD + 2; j++) {
        if (strcmp(cur_dir[j].filename, name) == 0)
            break;
    }
    if (j < MSD + 2) {
        printf("�ļ����Ѵ���!\n");
        return -1;
    }

    //������ǣ���û���������������Ҫ�ҵ�������
    for (i = 2; i < MSD + 2; i++) {
        if (cur_dir[i].first == -1)
            break;
    }
    if (i >= MSD + 2) {
        printf("Ŀ¼������!\n");
        return -1;
    }

    if (opening_table.currence_count >= MOFN) //���ļ�̫��
    {
        printf("���ļ����ѳ�������\n");
        return -1;
    }
    //�ҵ������̿�
    for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
        if (fat[j].empty_disk == '0')
            break;
    }
    if (j >= DISK_NUM)    //û�п����̿�
    {
        printf("���̿ռ�����!\n");
        return -1;
    }
    fat[j].empty_disk = '1';
    //����Ϣ����Ŀ¼����
    strcpy(cur_dir[i].filename, name);
    cur_dir[i].first = j;
    cur_dir[i].size = 0;
    cur_dir[i].sub_dir = j;
    cur_dir[i].attribute = '0';
    return 0;
}

//ɾ���ļ�
int del(char *name) {
    int i, j;
    int cur_item; //���ڴ洢Ҫɾ���ļ���cur_dir�е��±�
    int item;    //�洢Ҫɾ���ļ�����ʼ�̿��
    int temp;
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name)) {
            cur_item = i;
            break;
        }
    }

    if (i >= MSD + 2) {
        printf("������ļ���������!\n");
        return -1;
    }
    if (cur_dir[cur_item].attribute != '0') {
        printf("ɾ���Ĳ����ļ�!\n");
        return -1;
    }
    //����ļ��Ǵ�״̬������ɾ��
    for (j = 0; j < MOFN; j++) {
        if (strcmp(opening_table.openitem[j].filename, name) == 0) {
            printf("�ļ����ڴ�״̬������ɾ��!\n");
            return -1;
        }
    }
    item = cur_dir[cur_item].first;
    //�ͷŴ��ļ���ռ�õ�FAT��Ŀռ�
    while (item != -1) {
        temp = fat[item].next;
        fat[item].next = -1;
        fat[item].empty_disk = '0';
        item = temp;
    }
    //�ͷŴ��ļ���ռ�õ�Ŀ¼��
    cur_dir[cur_item].sign = 0;
    cur_dir[cur_item].first = -1;
    strcpy(opening_table.openitem[cur_item].filename, "");
    cur_dir[cur_item].sub_dir = -1;
    cur_dir[cur_item].attribute = '0';
    cur_dir[cur_item].size = 0;
    return 0;
}

//����Ŀ¼
int mkdir(char *name) {
    int i, j;
    struct FCB *cur_mkdir;
    if (strlen(name) > 10) {
        printf("���ֳ�����������!\n");
        return -1;
    }
    for (j = 2; j < MSD + 2; j++) {
        if (strcmp(cur_dir[j].filename, name) == 0) {
            printf("�������Ѵ���!\n");
            break;
        }
    }
    for (i = 2; i < MSD + 2; i++) //����п���Ŀ¼��
    {
        if (cur_dir[i].first == -1)
            break;
    }
    if (i >= MSD + 2) {
        printf("Ŀ¼��������\n");
        return -1;
    }
    //�ҵ����д��̿�
    for (j = ROOT_DISK_NO + 1; j < DISK_NUM; j++) {
        if (fat[j].empty_disk == '0') {
            fat[j].empty_disk = '1'; //���ÿ��п�����Ϊ�ѷ���
            break;
        }
    }
    if (j >= DISK_NUM) {
        printf("���̿ռ�����!\n");
        return -1;
    }
    //��дĿ¼��
    strcpy(cur_dir[i].filename, name);
    cur_dir[i].first = j;
    cur_dir[i].size = ROOT_DISK_SIZE;
    cur_dir[i].sub_dir = j;
    cur_dir[i].attribute = '1';

    //��ʼ��Ŀ¼
    cur_mkdir = (struct FCB *) (total_disk + cur_dir[i].first * DISK_SIZE);
    //Ŀ¼�ĵ�ַ
    //ָ��ǰĿ¼��Ŀ¼��
    cur_mkdir[0].sign = 0;
    cur_mkdir[0].first = cur_dir[i].first;
    strcpy(cur_mkdir[0].filename, ".");
    cur_mkdir[0].sub_dir = cur_mkdir[0].first;
    cur_mkdir[0].attribute = '1';
    cur_mkdir[0].size = ROOT_DISK_SIZE;

    //ָ����һ��Ŀ¼��Ŀ¼��
    cur_mkdir[1].sign = cur_dir[0].sign;
    cur_mkdir[1].first = cur_dir[0].first;
    strcpy(cur_mkdir[1].filename, "..");
    cur_mkdir[1].sub_dir = cur_mkdir[1].first;
    cur_mkdir[1].attribute = '1';
    cur_mkdir[1].size = ROOT_DISK_SIZE;

    //��Ŀ¼����ʼ��Ϊ��
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

//ɾ��Ŀ¼
int rmdir(char *name) {
    int i, j, item;
    struct FCB *temp_dir;
    //��鵱ǰĿ¼�������޸�Ŀ¼
    for (i = 2; i < MSD + 2; i++) {
        if (!strcmp(cur_dir[i].filename, name))
            break;
    }
    if (i >= MSD + 2) {
        printf("��Ŀ¼������!\n");
        return -1;
    }
    if (cur_dir[i].attribute != '1') {
        printf("ɾ���Ĳ���Ŀ¼!\n");
        return -1;
    }
    //�ж�Ҫɾ����Ŀ¼������Ŀ¼
    temp_dir = (struct FCB *) (total_disk + cur_dir[i].sub_dir * DISK_SIZE);
    for (j = 2; j < MSD + 2; j++) {
        if (temp_dir[j].sub_dir != -1) {
            printf("��Ŀ¼������Ŀ¼������ɾ����Ŀ¼\n");
            return -1;
        }
    }
    //�ҵ���ʼ�̿��,�������ͷ�
    item = cur_dir[i].first;
    fat[item].empty_disk = '0';

    //�޸�Ŀ¼��
    cur_dir[i].sign = 0;
    cur_dir[i].first = -1;
    strcpy(cur_dir[i].filename, "");
    cur_dir[i].sub_dir = -1;
    cur_dir[i].attribute = '0';
    cur_dir[i].size = 0;
    return 0;
}

//��ʾ��ǰ��Ŀ¼������
void ls() {
    int i;
    for (i = 2; i < MSD + 2; i++) {
        if (cur_dir[i].first != -1) //���������Ŀ¼
        {
            printf("%s\t", cur_dir[i].filename);
            if (cur_dir[i].attribute == '0') {
                printf("\t<�ļ�>\t");
                printf("\t��ʼ�̿��:%d\n", cur_dir[i].first);

            } else {
                printf("\t<Ŀ¼>\t");
                printf("\t��ʼ�̿��:%d\n", cur_dir[i].first);
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
    //Ϊ·�����ַ���ռ�
    temp = (char *) malloc(DIR_MAXSIZE * sizeof(char));
    for (i = 0; i < (int) strlen(str); i++)
        temp[i] = str[i];
    temp[i] = '\0';
    //���Ҹ���Ŀ¼�Ƿ��ڵ�ǰĿ¼��
    for (j = 0; j < MSD + 2; j++) {
        if (!strcmp(temp_dir[j].filename, temp))
            break;
    }
    free(temp);//�ͷ��������ʱ�ռ�
    //û�����Ŀ¼
    if (j >= MSD + 2) {
        printf("·������ȷ\n");
        return -1;
    }
    item = temp_dir[j].first;
    //��ǰĿ¼�ڴ�����λ��
    temp_dir = (struct FCB *) (total_disk + item * DISK_SIZE);
    if (strcmp("..", name) == 0) {
        if (cur_dir[j - 1].sign != 1) //����ϼ�Ŀ¼���Ǹ�Ŀ¼
        {
            point = strchr(currence_dir, '\\');
            //�����ַ���currence_dir���״γ����ַ�\ ��λ��
            while (point != NULL) {
                point1 = point + 1; //��ȥ'\'��ռ�Ŀռ�,��¼�´β��ҵ���ʼ��ַ
                point = strchr(point1, '\\');
            }
            *(point1 - 1) = '\0'; //����һ��Ŀ¼ɾ��
        }
    } else {
        currence_dir = strcat(currence_dir, "\\"); //�޸ĵ�ǰĿ¼
        currence_dir = strcat(currence_dir, name);
    }
    cur_dir = temp_dir; //����ǰĿ¼ȷ������
    return 0;
}

void print() {
    printf("\n\n**********************�����ļ�ϵͳ���***********************\n\n");
    printf("\t\t\t�����ʽ˵��\n\n");
    printf("\t\tcd	        ���ĵ�ǰĿ¼\n");
    printf("\t\tformat		��ʽ�����̿ռ�\n");
    printf("\t\tmkdir		������Ŀ¼\n");
    printf("\t\trmdir		ɾ����Ŀ¼\n");
    printf("\t\tls	        ��ʾ��ǰĿ¼����Ŀ¼\n");
    printf("\t\tcreate	        �����ļ�\n");
    printf("\t\tdel	        ɾ���ļ�\n");
    printf("\t\topen	        ���ļ�\n");
    printf("\t\tclose		�ر��ļ�\n");
    printf("\t\twithdraw	�û�ע��\n");
    printf("**************************************************************\n");
}
