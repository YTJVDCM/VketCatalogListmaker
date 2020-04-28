#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <windows.h>//for Windows
#include <unistd.h>
#include <shlobj.h>
#include "header\picojson.h"
using namespace std;

string GetPathWin();//for Windows

int splitlength=0;
std::vector<std::string> split(const std::string &str, char sep);

//char *GetPathLinux();//for Linux

int main(int argc,char *argv[]){
    printf("\"%s\"\n",argv[0]);
    printf("\"%s\"\n",argv[1]);
    //printf("\"%s\"\n",argv[2]);

    string url = argv[1];
    
    //for Windows
    string filepath = GetPathWin();

    //for Linux
    //string filepath = GetPathLinux().c_str();
    
    FILE *fp;
    char titlechar[260]="";
    char vketchar[260]="";
    char worldchar[256]="";
    char groupchar[256]="";
    char sectionchar[256]="";
    char headerchar[256]="";
    char iconchar[256]="";
    string title;
    string vket;
    string world;
    string group;
    string section;
    string header;
    string icon;
    char mydocuments[260];
    SHGetSpecialFolderPath(NULL,mydocuments,CSIDL_PERSONAL,0);

    string jsonpath=string(mydocuments)+"\\VketJSON\\cataloglist.json";
    
    int acs = access(jsonpath.c_str(),0);
    printf("%d",acs);
    //ファイルの存在チェック(なければ作成)
    if(acs==-1){
        string mkdir="mkdir "+string(mydocuments)+"\\VketJSON";
        system(mkdir.c_str());
        fp = fopen(jsonpath.c_str(),"w");
        fprintf(fp,"{\"VketData\": []}");
        fclose(fp);
    }

    //探索用コマンド群

    int chkvket3=0;
    chdir(filepath.c_str());
    string checkvket3="runtime\\curl.exe -s -L \""+url+"\"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//title[2]) -";
    string checkvkettitle="runtime\\curl.exe -s -L \""+url+"\"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//title[1]) -";
    fp=popen(checkvket3.c_str(),"r");
    char chkvketchar[256];
    fgets(chkvketchar,sizeof(chkvketchar),fp);
    if(strcmp(chkvketchar,"バーチャルマーケット３")==0)chkvket3=1;
    pclose(fp);

    fp=popen(checkvkettitle.c_str(),"r");
    char chkvkettitle[256];
    fgets(chkvkettitle,sizeof(chkvkettitle),fp);
    pclose(fp);
    vector<string> splited = split(string(chkvkettitle),':');
    
    //for Vket3
    if(chkvket3!=0){
        printf("This catalog is Vket3.\n");
        string gettitle="runtime\\curl.exe -s -L \""+url+"\"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//div[@class='user_name']/text()) -";
        string getworld="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//span[@class='world_name']/text()) -";
        string getsection="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//span[@class='section_name']/text()) -";
        string getheader="runtime\\curl.exe -s -L "+url+"|runtime\\grep.exe -oP \"(?<=<section\\sclass=\\\"user_header\\\"\\sstyle=\\\"background-image:\\surl\\()(.+)(?=\\)\\\">)\"";
        string geticon="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//img[@class='user_icon']/@src) -";
    
        //こっからデータ取得
        //タイトル
        if((fp=popen(gettitle.c_str(),"r"))==NULL){
        title=url;
        }else{
            fgets(titlechar,sizeof(titlechar),fp);
            fgets(vketchar,sizeof(vketchar),fp);
            title=string(titlechar);
            if(strcmp(titlechar,"")==0)title="Missing\n";
        }
        pclose(fp);

        //ワールド
        if((fp=popen(getworld.c_str(),"r"))==NULL){
            world="Missing";
        }else{
            fgets(worldchar,sizeof(worldchar),fp);
            world=string(worldchar);
            if(strcmp(worldchar,"")==0)world="Missing\n";
        }
        pclose(fp);

        //エリア
        if((fp=popen(getsection.c_str(),"r"))==NULL){
            section="Missing";
        }else{
            fgets(sectionchar,sizeof(sectionchar),fp);
            section=string(sectionchar);
        if(strcmp(sectionchar,"")==0)section="Missing\n";
        }
        pclose(fp);

        //ヘッダー画像URL
        if((fp=popen(getheader.c_str(),"r"))==NULL){
        header="Missing";
        }else{
            fgets(headerchar,sizeof(headerchar),fp);
            header=string(headerchar);
            if(strcmp(headerchar,"")==0)header="Missing";
        }
        pclose(fp);

        //アイコン画像URL
        if((fp=popen(geticon.c_str(),"r"))==NULL){
            icon="Missing";
        }else{
            fgets(iconchar,sizeof(iconchar),fp);
            icon=string(iconchar);
            if(strcmp(iconchar,"")==0)icon="Missing";
        }
        pclose(fp);
        
        vket="バーチャルマーケット3";
        //ここまでデータ取得

        //グループタグ(Vケット3では存在しないので強制Missing)
        group="Missing";

        printf("Data acquisition completed!\n");
        //JSONデータ作成
        picojson::value v;
        ifstream json;
        string jsondata="";
        json.open(jsonpath.c_str());
        while(!json.eof()){
            string work = "";
            json >> work;
            jsondata += work;
        }
        json.close();
        string err=parse(v,jsondata);
        if(!err.empty()){
            cerr<<picojson::get_last_error()<<endl;
            return 1;
        }
        cout << v << endl;
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
        maindata.insert(make_pair("Group",picojson::value(group)));
        maindata.insert(make_pair("Section",picojson::value(section)));
        maindata.insert(make_pair("Icon",picojson::value(icon)));
        maindata.insert(make_pair("Header",picojson::value(header)));
        vketdata.push_back(picojson::value(maindata));

        string jsonout = picojson::value(o).serialize();

        ofstream jsonput;
        jsonput.open(jsonpath.c_str());
        if(!jsonput){
            cout << "ファイル書き込み失敗" << endl;
            return 1;
        }
        jsonput << jsonout << endl;
        jsonput.close();
        //ここまでデータ作成

        //デバッグ用出力
        cout << "Title: " << title << "Vket:" << vket << "World: " << world << "Section: " << section << endl;


    }else if(splited[splitlength-1]==" バーチャルマーケット4"){
        //for Vket 4
        string gettitle="curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul \"normalize-space(//span[@class='circle-card__circle_name']/text())\" -";
        string getarea="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul \"normalize-space(//span[@class='circle-card__section']/text())\" -|runtime\\sed.exe \"s/ - /\\\\n/g\"";
        string getheader="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//img[@class='circle-card__header-image']/@src) -";
        string geticon="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//img[@class='circle-card__icon']/@src) -";
    
        printf("This catalog is Vket4.\n");
    
        //こっからデータ取得
        //タイトル
        if((fp=popen(gettitle.c_str(),"r"))==NULL){
        title=url;
        }else{
            fgets(titlechar,sizeof(titlechar),fp);
            title=string(titlechar);
            if(strcmp(titlechar,"")==0)title="Missing\n";
        }
        pclose(fp);

        //ワールド,グループ,エリア
        if((fp=popen(getarea.c_str(),"r"))==NULL){
        world="Missing";
        group="Missing";
        section="Missing";
        }else{
            fgets(worldchar,sizeof(worldchar),fp);
            fgets(groupchar,sizeof(groupchar),fp);
            fgets(sectionchar,sizeof(sectionchar),fp);
            world=string(worldchar);
            group=string(groupchar);
            section=string(sectionchar);
            if(strcmp(worldchar,"")==0)world="Missing\n";
            if(strcmp(groupchar,"")==0)group="Missing\n";
            if(strcmp(sectionchar,"")==0)section="Missing\n";
        }
        pclose(fp);

        //ヘッダー画像URL
        if((fp=popen(getheader.c_str(),"r"))==NULL){
        header="Missing";
        }else{
            fgets(headerchar,sizeof(headerchar),fp);
            header=string(headerchar);
            if(strcmp(headerchar,"")==0)header="Missing";
        }
        pclose(fp);

        //アイコン画像URL
        if((fp=popen(geticon.c_str(),"r"))==NULL){
            icon="Missing";
        }else{
            fgets(iconchar,sizeof(iconchar),fp);
            icon=string(iconchar);
            if(strcmp(iconchar,"")==0)icon="Missing";
        }
        pclose(fp);

        vket="バーチャルマーケット4";
        //ここまでデータ取得

        printf("Data acquisition completed!\n");
        //JSONデータ作成
        picojson::value v;
        ifstream json;
        string jsondata="";
        json.open(jsonpath.c_str());
        while(!json.eof()){
            string work = "";
            json >> work;
            jsondata += work;
        }
        json.close();
        string err=parse(v,jsondata);
        if(!err.empty()){
            cerr<<picojson::get_last_error()<<endl;
            return 1;
        }
        cout << v << endl;
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
        maindata.insert(make_pair("Group",picojson::value(group)));
        maindata.insert(make_pair("Section",picojson::value(section)));
        maindata.insert(make_pair("Icon",picojson::value(icon)));
        maindata.insert(make_pair("Header",picojson::value(header)));
        vketdata.push_back(picojson::value(maindata));

        string jsonout = picojson::value(o).serialize();

        ofstream jsonput;
        jsonput.open(jsonpath.c_str());
        if(!jsonput){
            cout << "ファイル書き込み失敗" << endl;
            return 1;
        }
        jsonput << jsonout << endl;
        jsonput.close();
        //ここまでデータ作成

        //デバッグ用出力
        cout << "Title: " << title << "Vket:" << vket << "World: " << world << "Group: " << group << "Section: " << section << endl;
    }else{
        //for Vket β
        string gettitle="curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul \"normalize-space(//meta[@property='og:title']/@content)\" -";
        string geticon="runtime\\curl.exe -s -L "+url+"|runtime\\xmllint.exe --nowarning --noblanks --html --xpath 2>nul normalize-space(//meta[@property='og:image']/@content) -";
    
        printf("This catalog is Vketβ or others.\n");
    
        //こっからデータ取得
        //タイトル
        if((fp=popen(gettitle.c_str(),"r"))==NULL){
        title=url;
        }else{
            fgets(titlechar,sizeof(titlechar),fp);
            title=string(titlechar);
            if(strcmp(titlechar,"")==0)title="Missing\n";
        }
        pclose(fp);

        //ワールド,グループ,エリア
        world="Missing";
        group="Missing";
        section="Missing";

        //ヘッダー画像URL
        header="Missing";

        //アイコン画像URL
        if((fp=popen(geticon.c_str(),"r"))==NULL){
            icon="Missing";
        }else{
            fgets(iconchar,sizeof(iconchar),fp);
            icon=string(iconchar);
            if(strcmp(iconchar,"")==0)icon="Missing";
        }
        pclose(fp);

        vket="バーチャルマーケットβ";
        //ここまでデータ取得

        printf("Data acquisition completed!\n");
        //JSONデータ作成
        picojson::value v;
        ifstream json;
        string jsondata="";
        json.open(jsonpath.c_str());
        while(!json.eof()){
            string work = "";
            json >> work;
            jsondata += work;
        }
        json.close();
        string err=parse(v,jsondata);
        if(!err.empty()){
            cerr<<picojson::get_last_error()<<endl;
            return 1;
        }
        cout << v << endl;
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
        maindata.insert(make_pair("Group",picojson::value(group)));
        maindata.insert(make_pair("Section",picojson::value(section)));
        maindata.insert(make_pair("Icon",picojson::value(icon)));
        maindata.insert(make_pair("Header",picojson::value(header)));
        vketdata.push_back(picojson::value(maindata));

        string jsonout = picojson::value(o).serialize();

        ofstream jsonput;
        jsonput.open(jsonpath.c_str());
        if(!jsonput){
            cout << "ファイル書き込み失敗" << endl;
            return 1;
        }
        jsonput << jsonout << endl;
        jsonput.close();
        //ここまでデータ作成
    }
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

std::vector<std::string> split(const std::string &str, char sep)
{
    splitlength=0;
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
        splitlength++;
    }
    return v;
}

// char *GetPathLinux(){
//     static char modulePath[1024] = {};
//     readlink("/proc/self/exe",modulePath,sizeof(modulePath)-1);
//     return modulePath;
// }