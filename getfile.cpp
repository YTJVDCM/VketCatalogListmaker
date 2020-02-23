#include <bits/stdc++.h>
#include <windows.h>//for Windows
#include <unistd.h>
#include "header\picojson.h"
using namespace std;

string GetPathWin();//for Windows

//char *GetPathLinux();//for Linux

int main(int argc,char *argv[]){
    printf("\"%s\"\n",argv[0]);
    printf("\"%s\"\n",argv[1]);
    printf("\"%s\"\n",argv[2]);

    string url = argv[2];
    
    //for Windows
    string filepath = GetPathWin();

    //for Linux
    //string filepath = GetPathLinux().c_str();
    
    chdir(filepath.c_str());
    string gettitle="runtime\\curl.exe -s -L \""+url+"\"|runtime\\grep.exe -oP \"(?<=<title>)(.+)(?=</title>)\"";
    string getworld="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<span\\sclass=\\\"world_name\\\">)(.+)(?=</span>)\"";
    string getsection="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<span\\sclass=\\\"section_name\\\">)(.+)(?=</span>)\"";
    string getheader="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<section\\sclass=\\\"user_header\\\"\\sstyle=\\\"background-image:\\surl\\()(.+)(?=\\)\\\">)\"";
    string geticon="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<img\\ssrc=\\\")(.+)(?=\\\"\\sclass=\\\"user_icon\\\">)\"";
    

    char titlechar[260]="";
    char vketchar[260]="";
    char worldchar[256]="";
    char sectionchar[256]="";
    char headerchar[256]="";
    char iconchar[256]="";
    string title;
    string vket;
    
    FILE *fp;
    if((fp=popen(gettitle.c_str(),"r"))==NULL){
        title=url;
        vket="None";
    }else{
        fgets(titlechar,sizeof(titlechar),fp);
        fgets(vketchar,sizeof(vketchar),fp);
        title=string(titlechar);
        vket=string(vketchar);
        if(strcmp(titlechar,"")==0)title="Missing\n";
        if(strcmp(vketchar,"")==0)vket="Missing\n";
    }
    pclose(fp);

    string world;
    if((fp=popen(getworld.c_str(),"r"))==NULL){
        world="Missing";
    }else{
        fgets(worldchar,sizeof(worldchar),fp);
        world=string(worldchar);
        if(strcmp(worldchar,"")==0)world="Missing\n";
    }
    pclose(fp);

    string section;
    if((fp=popen(getsection.c_str(),"r"))==NULL){
        section="Missing";
    }else{
        fgets(sectionchar,sizeof(sectionchar),fp);
        section=string(sectionchar);
        if(strcmp(sectionchar,"")==0)section="Missing\n";
    }
    pclose(fp);

    string header;
    if((fp=popen(getheader.c_str(),"r"))==NULL){
        header="image/noimage.png";
    }else{
        fgets(headerchar,sizeof(headerchar),fp);
        header=string(headerchar);
        if(strcmp(headerchar,"")==0)header="image/noimage.png";
    }
    pclose(fp);

    string icon;
    if((fp=popen(geticon.c_str(),"r"))==NULL){
        icon="image/noimage.png";
    }else{
        fgets(iconchar,sizeof(iconchar),fp);
        icon=string(iconchar);
        if(strcmp(iconchar,"")==0)icon="image/noimage.png";
    }
    pclose(fp);
    
    picojson::value v;
    ifstream json;
    string jsondata="";
    json.open("cataloglist.json");
    while(!json.eof()){
        string work = "";
        json >> work;
        jsondata += work;
    }
    json.close();
    string err=parse(v,jsondata);//エラー判定だよ、ついでにパースもするらしいよ。パースって何だ。
    if(!err.empty()){
        cerr<<picojson::get_last_error()<<endl;
        return 1;
    }
    cout << v << endl;//出力するよ。
    if(!v.is<picojson::object>()){
        cerr << "JSON is not an Object" << endl;
        return 1;
    }
    picojson::object& o = v.get<picojson::object>();
    for(picojson::object::const_iterator i=o.begin();i!=o.end();++i)cout<<i->first<<" "<<i->second<<endl;
    
    picojson::value& c = o["VketData"];
    picojson::array& vketdata = c.get<picojson::array>();
    picojson::object maindata;

    maindata.insert(make_pair("Title",picojson::value(title)));
    maindata.insert(make_pair("URL",picojson::value(url)));
    maindata.insert(make_pair("Vket",picojson::value(vket)));
    maindata.insert(make_pair("World",picojson::value(world)));
    maindata.insert(make_pair("Section",picojson::value(section)));
    maindata.insert(make_pair("Icon",picojson::value(icon)));
    maindata.insert(make_pair("Header",picojson::value(header)));
    vketdata.push_back(picojson::value(maindata));

    string jsonout = picojson::value(o).serialize();

    ofstream jsonput;
    jsonput.open("cataloglist.json");
    if(!jsonput){
        cout << "ファイル書き込み失敗" << endl;
        return 1;
    }
    jsonput << jsonout << endl;
    jsonput.close();

    cout << "Title: " << title << "Vket:" << vket << "World: " << world << "Section: " << section << endl;

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