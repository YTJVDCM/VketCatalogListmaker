#include <stdio.h>
#include <stdlib.h>
int main(){
    char a;
    printf("���s�����cataloglist.json�ɏ������܂ꂽ���X�g���S�č폜����܂��B\n���s���܂���?(y/n) ");
    scanf("%c",&a);
    if(a!='y'&&a!='Y')return 0;
    FILE *fp;
    system("rm cataloglist.json");
    fp = fopen("cataloglist.json","w");
    fprintf(fp,"{\"VketData\": []}");
    fclose(fp);
    return 0;
}