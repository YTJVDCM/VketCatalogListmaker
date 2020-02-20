#include <bits/stdc++.h>
#include <windows.h>//for Windows
#include <unistd.h>
using namespace std;

string GetPathWin();//for Windows

//char *GetPathLinux();//for Linux

int main(int argc,char *argv[]){
    printf("\"%s\"\n",argv[0]);
    printf("\"%s\"\n",argv[1]);
    printf("\"%s\"\n",argv[2]);

    Sleep(1000);
    string url = argv[2];
    
    //for Windows
    string filepath = GetPathWin();

    //for Linux
    //string filepath = GetPathLinux().c_str();
    
    chdir(filepath.c_str());
    string gettitle="runtime\\curl.exe -s -L \""+url+"\"|runtime\\grep.exe -oP \"(?<=<title>)(.+)(?=</title>)\"";
    string getworld="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<span\\sclass=\\\"world_name\\\">)(.+)(?=</span>)\"";
    string getsection="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<span\\sclass=\\\"section_name\\\">)(.+)(?=</span>)\"";

    char titlechar[260];
    char worldchar[256];
    char sectionchar[256];
    string title;
    
    FILE *fp;
    if((fp=popen(gettitle.c_str(),"r"))==NULL){
        title=url;
    }else{
        fgets(titlechar,sizeof(titlechar),fp);
        title=string(titlechar);
    }
    pclose(fp);

    string world;
    if((fp=popen(getworld.c_str(),"r"))==NULL){
        world="Missing";
    }else{
        fgets(worldchar,sizeof(worldchar),fp);
        world=string(worldchar);
    }
    pclose(fp);

    string section;
    if((fp=popen(getsection.c_str(),"r"))==NULL){
        section="Missing";
    }else{
        fgets(sectionchar,sizeof(sectionchar),fp);
        cout << sectionchar << endl;
        section=string(sectionchar);
    }
    pclose(fp);
    
    //for debug
    string file = "C:\\Users\\ytjvd\\Desktop\\VketURLImporter\\url.html";
    //string file = filepath + "url.html";
    fp = fopen(file.c_str(),"a");
    fprintf(fp,"<li><a href=%s target=\"_blank\">%s</a></li>\n",url.c_str(),title.c_str());
    fprintf(fp,"World:%s<br>Section:%s\n",world.c_str(),section.c_str());
    fclose(fp);

    cout << "title: " << title << "world: " << world << "section: " << section << endl;
    getchar();

    return 0;
}

string GetPathWin(){
    string modulePath = "";
    char path[MAX_PATH],drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
    if(GetModuleFileNameA(NULL,path,MAX_PATH)!=0){
        _splitpath_s(path,drive,dir,fname,ext);
        modulePath=string(drive)+string(dir);
    }
    return modulePath;
}

// char *GetPathLinux(){
//     static char modulePath[1024] = {};
//     readlink("/proc/self/exe",modulePath,sizeof(modulePath)-1);
//     return modulePath;
// }