#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shlobj.h>
int main(){
    char documents[260];
    SHGetSpecialFolderPath(NULL,documents,CSIDL_PERSONAL,0);
    chdir(documents);
    chdir("./VketCatalogListMaker");
    FILE *fp;
    system("rm cataloglist.json");
    fp = fopen("cataloglist.json","w");
    fprintf(fp,"{\"VketData\": []}");
    fclose(fp);
    return 0;
}