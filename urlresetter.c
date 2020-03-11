#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shlobj.h>
int main(){
    char a;
    printf("実行するとcataloglist.jsonに書き込まれたリストが全て削除されます。\n続行しますか?(y/n) ");
    scanf("%c",&a);
    if(a!='y'&&a!='Y')return 0;
    char documents[260];
    SHGetSpecialFolderPath(NULL,documents,CSIDL_PERSONAL,0);
    chdir(documents);
    system("mkdir VketCatalogListMaker");
    chdir("./VketCatalogListMaker");
    FILE *fp;
    system("rm cataloglist.json");
    fp = fopen("cataloglist.json","w");
    fprintf(fp,"{\"VketData\": []}");
    fclose(fp);
    return 0;
}