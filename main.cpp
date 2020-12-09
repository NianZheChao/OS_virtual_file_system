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
        printf("是否进行格式化?(y/n)");
        scanf("%c", &ch);
        if (ch == 'y') {
            format();
            printf("格式化成功! \n");
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
    //如之前已登录过，直接进入
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
            case 0: //用户注销
                withdraw();
                return 0;
            case 1: //创建文件
                scanf("%s", name);
                flag = create(name);
                if (flag == 0)
                    printf("创建文件成功!\n");
                printf("%s>", currence_dir);
                break;
            case 2://打开文件
                scanf("%s", name);
                flag = open(name);
                if (flag == 0)
                    printf("打开文件成功!\n");
                printf("%s>", currence_dir);
                break;
            case 3://关闭文件
                scanf("%s", name);
                flag = close(name);
                if (flag == 0)
                    printf("关闭文件成功!\n");
                printf("%s>", currence_dir);
                break;
            case 4://删除文件
                scanf("%s", name);
                flag = del(name);
                if (flag == 0)
                    printf("删除文件成功!\n");
                printf("%s>", currence_dir);
                break;
            case 5://创建子目录
                scanf("%s", name);
                flag = mkdir(name);
                if (flag == 0)
                    printf("创建目录成功!\n");
                printf("%s>", currence_dir);
                break;
            case 6://删除子目录
                scanf("%s", name);
                flag = rmdir(name);
                if (flag == 0)
                    printf("删除目录成功!\n");
                printf("%s>", currence_dir);
                break;
            case 7://显示当前子目录
                ls();
                printf("%s>", currence_dir);
                break;
            case 8://更改当前目录
                scanf("%s", name);
                flag = cd(name);
                printf("%s>", currence_dir);
                break;
            case 9://格式化
                format();
                printf("格式化成功\n");
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
