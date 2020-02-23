#include <bits/stdc++.h>
#include "picojson.h"
using namespace std;
int main(){

    //後で消す奴ら
    string url="https://www.v-market.work/v3/user/728";
    string title="バーチャルマーケット | セシル変身プロジェクト";
    string vket="バーチャルマーケット３";
    string world="ネオ渋谷-Day";
    string section="ネオ道玄坂下";


    picojson::value v;
    ifstream json;
    string jsondata="";
    json.open("cataloglist.json");
    if(json.fail()){
        system("touch cataloglisst.json");
        json.open("cataloglist.json");
    }
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


    return 0;
}