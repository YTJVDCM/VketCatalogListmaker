#include <stdio.h>
#include <stdlib.h>
int main(){
    char a;
    printf("実行するとurl.htmlに書き込まれたリストが全て削除されます。\n続行しますか?(y/n) ");
    scanf("%c",&a);
    if(a!='y'&&a!='Y')return 0;
    FILE *fp;
    system("rm url.html");
    fp = fopen("url.html","w");
    fprintf(fp,"<meta charset=\"utf-8\">\n");
    fclose(fp);
    return 0;
}