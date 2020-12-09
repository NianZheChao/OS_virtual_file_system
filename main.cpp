#include "functions.h"


int main() {
    FILE *fp;
    char ch;
    char a[100];
    char code[10][10];
    char name[10];
    int i, flag;
    user_load();
    //fflush(stdin);
    if ((fp = fopen("disk.dat", "rb")) == NULL) {
        printf("�Ƿ���и�ʽ��?(y/n)");
        scanf("%c", &ch);
        if (ch == 'y') {
            format();
            printf("��ʽ���ɹ�! \n");
        } else
            return 0;
    }

    strcpy(code[0], "withdraw");
    strcpy(code[1], "create");
    strcpy(code[2], "open");
    strcpy(code[3], "close");
    strcpy(code[4], "del");
    strcpy(code[5], "mkdir");
    strcpy(code[6], "rmdir");
    strcpy(code[7], "ls");
    strcpy(code[8], "cd");
    strcpy(code[9], "format");
    //��֮ǰ�ѵ�¼����ֱ�ӽ���
    enter();
    print();
    printf("%s>", currence_dir);

    while (1) {
        scanf("%s", a);
        for (i = 0; i < 11; i++) {
            if (!strcmp(code[i], a))
                break;
        }
        switch (i) {
            case 0: //�û�ע��
                withdraw();
                return 0;
            case 1: //�����ļ�
                scanf("%s", name);
                flag = create(name);
                if (flag == 0)
                    printf("�����ļ��ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 2://���ļ�
                scanf("%s", name);
                flag = open(name);
                if (flag == 0)
                    printf("���ļ��ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 3://�ر��ļ�
                scanf("%s", name);
                flag = close(name);
                if (flag == 0)
                    printf("�ر��ļ��ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 4://ɾ���ļ�
                scanf("%s", name);
                flag = del(name);
                if (flag == 0)
                    printf("ɾ���ļ��ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 5://������Ŀ¼
                scanf("%s", name);
                flag = mkdir(name);
                if (flag == 0)
                    printf("����Ŀ¼�ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 6://ɾ����Ŀ¼
                scanf("%s", name);
                flag = rmdir(name);
                if (flag == 0)
                    printf("ɾ��Ŀ¼�ɹ�!\n");
                printf("%s>", currence_dir);
                break;
            case 7://��ʾ��ǰ��Ŀ¼
                ls();
                printf("%s>", currence_dir);
                break;
            case 8://���ĵ�ǰĿ¼
                scanf("%s", name);
                flag = cd(name);
                printf("%s>", currence_dir);
                break;
            case 9://��ʽ��
                format();
                printf("��ʽ���ɹ�\n");
                printf("---------------------------------------------------\n\n");
                enter();
                print();
                printf("%s>", currence_dir);
                break;
            default:
                printf("\n Error!\n The command is wrong! \n");
                printf("%s>", currence_dir);
                break;
        }
    }
    return 0;

}
