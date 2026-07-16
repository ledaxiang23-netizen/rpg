#include<iostream>
#include<string>
#include<vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

using namespace std;
//技能定义
class skill{
    public:
    string name;
    };

//武器定义
class weapon{
    public:
    string name;
    double wad;
    double wac;//武器护甲加值
    double wdd;//武器敏捷
    void setweapon(string name,double ad,double ac,double dd);
    void showweapon();
};
//武器设定基础全定义
void weapon::setweapon(string wname,double ad,double ac,double dd){
    name = wname;wad = ad;wac = ac;wdd = dd;
}
//展示武器详细参数
void weapon::showweapon(){
    cout<<name<<": "<<"攻击力:"<<wad<<"护甲加值:"<<wac<<"武器敏捷:"<<wdd<<endl;
}
//角色的基础定义
class character{
    public:
    void setcharacter(string sname,double shp,double sad,double sac,double sdd);
    string name;
    double maxhp = 100;//最大生命值
    double hp = 100;
    double ad = 10;
    double ac = 20;//护甲
    double dd = 5;//敏捷
    vector<weapon> warmon;//装备栏
    vector<weapon> bag;//背包
    vector<skill> qvq;//技能
    void attack(character& anm,double danmage);
    void put_weapon();
    void showwarmon();
    void showbag();
    void showcharacter();
    double alldd();
    double allad();//计算总攻击力
};

template <typename t>
t* findt(string targetname,vector<t>& anker);

void character::setcharacter(string sname,double shp,double sad,double sac,double sdd){
    name = sname;
    hp = shp;
    ad = sad;
    ac = sac;
    dd = sdd;
} 

template <typename t>
void rent(int i,vector<t>& anker);

//装备武器函数
void character::put_weapon(){
    cout<<"你想装备第几件武器:"<<endl;
    int i;
    cin>>ws;
    cin>>i;
    warmon.push_back(bag[i-1]);
    cout<<bag[i-1].name<<"已装备"<<endl;
    rent(i,bag);
}

void character::showwarmon(){
    int i;
    cout<<name<<"的装备栏"<<endl;
    for(i=0;i<warmon.size();i++){
        cout<<"第"<<i+1<<"件:"<<endl;
        warmon[i].showweapon();
    }
}

void character::showbag(){
    int i;
    cout<<name<<"的背包"<<endl<<endl;
    for(i=0;i<bag.size();i++){
        cout<<"第"<<i+1<<"件:"<<endl;
        bag[i].showweapon();
        cout<<endl;
    }
    cout<<"0.关闭背包 1.装备武器"<<endl;
    string p;
    cin>>ws;
    getline(cin,p);
    if(p=="0"){

    }
    else if(p=="1"){
        put_weapon();
    }
}

void character::showcharacter(){
    cout<<name<<" 生命值 = "<<hp<<" 攻击力 = "<<ad<<" 护甲 = "<<ac<<" 闪避 = "<<dd<<endl;
}

//掷骰子
int rollDice() {
    return rand() % 20 + 1;
}

vector<character> characters;

//丢弃
template <typename t>
void rent(int i,vector<t>& anker){
    anker.erase(anker.begin() + i - 1);

}

//容器内部查找////////////////////////////////////////////////////
template <typename t>
t* findt(string targetname,vector<t>& anker){
    int i = 0;
    int p = 0;
    for(i=0;i<anker.size();i++){
        if(anker[i].name == targetname){
            return &anker[i];
            p = 1;
            break;
        }
    }
    if(p==0){
        return nullptr;
    }
    return nullptr;
}
//总敏捷值计算
double character::alldd(){
    int i = 0;
    int sum = 0;
    for(i=0;i<warmon.size();i++){
        sum += warmon[i].wdd;
    }
    return dd + sum;
}
//总攻击力计算
double character::allad(){
    int i = 0;
    int sum = 0;
    for(i=0;i<warmon.size();i++){
        sum += warmon[i].wad;
    }
    return dd + sum;
}
//攻击命中判定与伤害计算总函数
void character::attack(character& anm,double danmage){
    int dice = rollDice();
    if(dice+alldd()/3 >= anm.alldd()||dice==20){
        double truedanmage = danmage*100.0/(anm.ac + 100.0);
        anm.hp -= truedanmage;
        cout<<name<<"造成了"<<truedanmage<<"点伤害"<<"  ";
        cout<<anm.name<<"还剩"<<anm.hp<<endl<<endl;
    }
    else{
        cout<<"未命中！"<<endl;
    }
    this_thread::sleep_for(chrono::seconds(2));
}

vector<character> anms1;
//存活状态判定
int check(character& player,character& anm){
    if(player.hp > 0 && anm.hp > 0){
        return 1;
    }
    else{
        return 0;
    }
}
//胜利后结算函数
void playervectory(character& player,character& anm){
    cout<<"掉落了装备: ";
    this_thread::sleep_for(chrono::seconds(1));
    anm.showwarmon();
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"是否要拾取？"<<endl<<"1.是  2.否"<<endl;
    string choice;
    getline(cin,choice);
    if(choice == "1"){
        int i = 0;
        for(i=0;i<anm.warmon.size();i++){
            player.bag.push_back(anm.warmon[i]);
            cout<<"你拾取了装备:"<<anm.warmon[i].name<<endl;
        }
    }

}
void gamemap(character& player);
//战斗函数
void battlestar(character& player,vector<character> anms){
    int i = 0;
    for(i=0;i<anms.size();i++){
       cout<<endl<<"你遇到了"<< anms[i].name<<"  ";
       this_thread::sleep_for(chrono::seconds(1));
       cout<<"战斗开始!"<<endl<<endl;
       this_thread::sleep_for(chrono::seconds(1));
       while(check(player,anms[i])){
            cout<<"你的回合！"<<endl<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            cout<<"1.普通攻击  2.查看背包"<<endl<<endl;
            string re;
            cin>>ws;
            getline(cin,re);
            if(re=="1"){
                player.attack(anms[i],player.allad());
            }
            else if(re=="2"){
                player.showbag();
                continue;
            }
            if(check(player,anms[i])==0){
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));
            //敌人回合
            cout<<anms[i].name<<"回合"<<endl<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            anms[i].attack(player,anms[i].allad());
       }
       ////////战斗结果判定////////
       if(player.hp<=0){
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"你死了"<<endl;
        break;
       }
       else{
        this_thread::sleep_for(chrono::seconds(1));
        cout<<"你击败了"<<anms[i].name<<endl;
        this_thread::sleep_for(chrono::seconds(1));
        playervectory(player,anms[i]);
       }
    }
}

void gamemap(character& player){
    cout<<"你要去哪里"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout<<"1.嚎哭深渊  2.扭曲丛林  3.比尔吉沃特"<<endl;
    string taddress;
    getline(cin,taddress);
    if(taddress == "1"){
        battlestar(player,anms1);
    }
}

void gamestar(character& player){
    while(player.hp>0){
        cout<<"0.退出 1.地图 2.装备栏 3.人物属性 4.背包"<<endl;
        string p;
        cin>>ws;
        getline(cin,p);
        if(p=="1"){
            gamemap(player);
        }
        else if(p=="4"){
            player.showbag();
        }
        else if(p=="3"){
            player.showcharacter();
        }
        else if(p=="0"){
            break;
        }
        else if(p=="2"){
            player.showwarmon();
        }
    }
}


int main(){
    srand(time(0));
    /////初始化敌人//////////////////////////////////////////////
    character anm1;
    anm1.setcharacter("雪怪",30,5,5,1);
    weapon dabang;
    dabang.setweapon("大棒",10,0,0);
    weapon suozijia;
    suozijia.setweapon("皮甲",0,3,0);
    anm1.warmon.push_back(dabang);
    anm1.warmon.push_back(suozijia);
    anms1.push_back(anm1);//anms1是嚎哭深渊怪物集合
    ////////////////////////////////////////
    character anm2;
    anm2.setcharacter("冰鸟",20,5,1,7);
    weapon binshuangzhijie;
    binshuangzhijie.setweapon("冰霜之戒",20,0,0);
    anm2.warmon.push_back(binshuangzhijie);
    anms1.push_back(anm2);//anms1是嚎哭深渊怪物集合
    ///////////////////////////////////////////////////////////
    int p = 99;
    while(p!=0)
    {
        cout<<"0.退出  1.创建角色  2.选择角色"<<endl;
        cin>>p;
        if(p==1){
            cout<<"请为你的角色命名"<<endl;
            character newcharacter;
            cin.ignore();
            getline(cin,newcharacter.name);
            weapon w1;
            w1.setweapon("木剑",3,0,0);
            newcharacter.warmon.push_back(w1);
            characters.push_back(newcharacter);
            cout<<"你的名字叫"<<newcharacter.name<<endl;
            gamestar(newcharacter);

        }

    }
    
}