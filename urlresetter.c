#include <stdio.h>
#include <stdlib.h>
int main(){
    char a;
    printf("���s�����url.html�ɏ������܂ꂽ���X�g���S�č폜����܂��B\n���s���܂���?(y/n) ");
    scanf("%c",&a);
    if(a!='y'&&a!='Y')return 0;
    FILE *fp;
    system("rm url.html");
    fp = fopen("url.html","w");
    fprintf(fp,"<meta charset=\"utf-8\">\n");
    fclose(fp);
    return 0;
}