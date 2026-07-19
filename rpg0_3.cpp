//这个版本——0.3  我打算给技能组增加冷却时间
//然而 在这个版本中出现了很大的BUG————技能在第一回合不生效
//卡了我三天  完全没有进展   好在是AI告诉我 是allac这一系列函数的for循环的条件写错了  我写成了i<buffpack.size()  而不是i2<buffpack.size()；
//现在已修改   目前来看技能可以很好地生效
#include<iostream>
#include<string>
#include<vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>
#include <memory>
#include <algorithm>
#include <random>

using namespace std;

template <typename t>
void rent(int i,vector<t>& anker);//丢弃函数 用来丢弃任意容器中第i项
class character;
class buff;

//技能定义
class skill{
    public:
    string name;
    string inform;
    void setskillname(string sname, string sinform);
    void showskillinform();
    virtual void use(character& player,character& anm) = 0;
    virtual ~skill() = default;
    };
void skill::setskillname(string sname,string sinform){
    name = sname;
    inform = sinform;
}
//////////////////////
void skill::showskillinform(){
    cout<<name<<":"<<inform<<endl;
}

//定义BUFF//////////////////
class buff {
    public:
        string name;
        double adAdd = 0;
        double apAdd = 0;
        double acAdd = 0;
        double pcAdd = 0;
        double ddAdd = 0;

        double adMul = 1.0;
        double acMul = 1.0;
        double ddMul = 1.0;
        double apMul = 1.0;
        double pcMul = 1.0;

        int turns = 0;

        void setbuff(string sname,double ada,double apa,double aca,double pca,double dda,double adm,double apm,double acm,double pcm,double ddm,double turn){
            name = sname;
            adAdd = ada;
            apAdd = apa;
            acAdd = aca;
            pcAdd = pca;
            ddAdd = dda;
            adMul = adm;
            apMul = apm;
            acMul = acm;
            pcMul = pcm;
            ddMul = ddm;
            turns = turn;
        }
};

//武器定义
class weapon{
    public:
    string name;
    double wad;
    double wap;//武器法强加值
    double wac;//武器护甲加值
    double wpc;//武器魔抗加值
    double wdd;//武器敏捷
    void setweapon(string name,double ad,double ap,double ac,double pc,double dd);
    void showweapon();
};
//武器设定基础全定义
void weapon::setweapon(string wname,double ad,double ap,double ac,double pc,double dd){
    name = wname;wad = ad;wap = ap;wac = ac;wpc = pc;wdd = dd;
}
//展示武器详细参数
void weapon::showweapon(){
    cout<<name<<": "<<"攻击力:"<<wad<<"法强:"<<wap<<"护甲加值:"<<wac<<"魔抗:"<<wpc<<"武器敏捷:"<<wdd<<endl;
}
//角色的基础定义
class character{
    public:
    void setcharacter(string sname,double shp,double sad,double sap, double sac,double spc,double sdd);
    string name;
    double maxhp = 100;//最大生命值
    double hp = 100;
    double ad = 10;
    double ap = 0;
    double ac = 20;//护甲
    double pc = 20;
    double dd = 5;//敏捷
    vector<weapon> warmon;//装备栏
    vector<weapon> bag;//背包
    vector<shared_ptr<skill>> qvq;//技能
    vector<buff> buffpack;
    void attack(character& anm,double danmage);
    void put_weapon();
    void showwarmon();
    void showbag();
    void showcharacter();
    void showskillpack(character& anm);
    double alldd();
    double allad();//计算总攻击力
    double allac();
    double allap();//计算总法强
    double allpc();//计算总魔抗
};

//回合BUFF管理//////////////
void buffmanage(character& chrac) {
    for (int i = 0; i < chrac.buffpack.size(); i++) {
        chrac.buffpack[i].turns--;

        if (chrac.buffpack[i].turns < 0) {
            cout << chrac.buffpack[i].name << " 失效了" << endl;
            chrac.buffpack.erase(chrac.buffpack.begin() + i);
            i--;
        }
    }
}
//////初始化技能///////////////////////////////
class sheild : public skill{
    public:
    sheild() {
        setskillname("骑士之盾", "使你的护甲值提高50,持续3回合");
    }
    void use(character& player,character& anm){
        buff sheildbuff;
        sheildbuff.setbuff("骑士之盾",0,0,50,0,0,1,1,1,1,1,3);
        player.buffpack.push_back(sheildbuff);
        cout << "当前护甲：" << player.allac() << endl;
    }
};
//冰刺技能
class bingci : public skill{
    public:
    bingci() {
        setskillname("冰刺", "对敌人造成50点伤害");
    }
    void use(character& player,character& anm){
        double damage = 1.0*player.allap();
        player.attack(anm,damage);
    }
};
//冰凤展翅技能
class bingfengzhanchi : public skill{
    public:
    bingfengzhanchi() {
        setskillname("冰凤展翅", "使自己的敏捷提高6,持续1回合");
    }
    void use(character& player,character& anm){
        buff bingfengbuff;
        bingfengbuff.setbuff("冰凤展翅",0,0,0,0,6,1,1,1,1,1,1);
        player.buffpack.push_back(bingfengbuff);
        cout << "当前闪避：" << player.alldd() << endl;
    }
};
//普通攻击技能
class normalattack : public skill{
    public:
    normalattack() {
        setskillname("普通攻击", "对敌人造成100%攻击力伤害");
    }
    void use(character& player,character& anm){
        double damage = 1.0*player.allad();
        player.attack(anm,damage);
    }
};
//容器内部查找////////////////////////////////////////////////////
template <typename t>
t* findt(string targetname,vector<t>& anker);

void character::setcharacter(string sname,double shp,double sad,double sap, double sac,double spc,double sdd){
    name = sname;
    hp = shp;
    ad = sad;
    ap = sap;
    ac = sac;
    pc = spc;
    dd = sdd;
} 


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
//使用技能函数
void useskill(vector<shared_ptr<skill>>& qvq, character& player, character& anm) {
    int i = 0;
    cout << "你要用第几个技能：" << endl;
    cin >> i;

    if (i <= 0 || i > qvq.size()) {
        cout << "没有这个技能" << endl;
        return;
    }
    cout<<player.name<<"使用了技能: "<<qvq[i - 1]->name<<endl;
    qvq[i - 1]->use(player, anm);
}
//展示技能函数
void character::showskillpack(character& anm){
    int i;
    cout<<name<<"的技能槽"<<endl<<endl;
    for(i=0;i<qvq.size();i++){
        cout<<"第"<<i+1<<"个:"<<endl;
        (*qvq[i]).showskillinform();
        cout<<endl;
    }
    cout<<"0.关闭 1.选择技能"<<endl;
    string p;
    cin>>ws;
    getline(cin,p);
    if(p=="0"){

    }
    else if(p=="1"){
        useskill(qvq,*this,anm);
    }
}


//展示角色属性函数
void character::showcharacter(){
    cout<<name
        <<" 生命值 = "<<hp
        <<" 攻击力 = "<<allad()
        <<" 法强 = "<<allap()
        <<" 护甲 = "<<allac()
        <<" 魔抗 = "<<allpc()
        <<" 闪避 = "<<alldd()
        <<endl;

}

//掷骰子
int rollDice() {
    return rand() % 20 + 1;
}

vector<character> characters;

//丢弃     丢弃第i-1项
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
    int i2 = 0;
    double sum = 0 + dd;
    for(int i=0;i<warmon.size();i++){
        sum += warmon[i].wdd;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum += buffpack[i2].ddAdd;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum *= buffpack[i2].ddMul;
    }
    return sum;
}
//总攻击力计算
double character::allad(){
    int i = 0;
    int i2 = 0;
    int i3 = 0;
    double sum = ad + 0;
    for(int i=0;i<warmon.size();i++){
        sum += warmon[i].wad;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum += buffpack[i2].adAdd;
    }
    for(int i3=0;i3<buffpack.size();i3++){
        sum *= buffpack[i3].adMul;
    }
    return sum;
}
//总法强
double character::allap(){
    int i = 0;
    int i2 = 0;
    int i3 = 0;
    double sum = ap + 0;
    for(int i=0;i<warmon.size();i++){
        sum += warmon[i].wap;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum += buffpack[i2].apAdd;
    }
    for(int i3=0;i3<buffpack.size();i3++){
        sum *= buffpack[i3].apMul;
    }
    return sum;
}
//总护甲计算
double character::allac(){
    int i = 0;
    int i2 = 0;
    int i3 = 0;
    double sum = ac + 0;
    for(int i=0;i<warmon.size();i++){
        sum += warmon[i].wac;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum += buffpack[i2].acAdd;
    }
    for(int i3=0;i3<buffpack.size();i3++){
        sum *= buffpack[i3].acMul;
    }
    return sum;
}
//总魔抗计算
double character::allpc(){
    int i = 0;
    int i2 = 0;
    int i3 = 0;
    double sum = pc + 0;
    for(int i=0;i<warmon.size();i++){
        sum += warmon[i].wpc;
    }
    for(int i2=0;i2<buffpack.size();i2++){
        sum += buffpack[i2].pcAdd;
    }
    for(int i3=0;i3<buffpack.size();i3++){
        sum *= buffpack[i3].pcMul;
    }
    return sum;
}

//攻击命中判定与伤害计算总函数(2*dd,ac,damage,yuanhp)
void character::attack(character& anm,double danmage){
    int dice = rollDice();
    if(dice+alldd()/3 >= anm.alldd()||dice==20){
        double truedanmage = danmage*100.0/(anm.allac() + 100.0);
        anm.hp -= truedanmage;
        cout<<name<<"造成了"<<truedanmage<<"点伤害"<<"  ";
        cout<<anm.name<<"还剩"<<anm.hp<<endl<<endl;
    }
    else{
        cout<<"未命中！"<<endl;
    }
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
//打乱vector顺序函数
template <typename T>
void daluan(vector<T>& nums) {
    static random_device rd;
    static mt19937 rng(rd());

    shuffle(nums.begin(), nums.end(), rng);
}
//敌人行为函数
void anm_action(character& anm,character& player){
    daluan(anm.qvq);
    cout<<anm.name<<"使用了技能: "<<anm.qvq[0]->name<<endl;
    anm.qvq[0]->use(anm,player);
}
//
void reallbattle(character& player,character& anm){
        cout<<"战斗开始!"<<endl<<endl;
        this_thread::sleep_for(chrono::seconds(1));
        while(check(player,anm)){
            cout<<"你的回合！"<<endl<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            cout<<"1.普通攻击  2.查看背包 3.释放技能 4.查看敌人 5.查看玩家属性"<<endl<<endl;
            string re;
            cin>>ws;
            getline(cin,re);
            if(re=="1"){
                player.attack(anm,player.allad());
            }
            else if(re=="2"){
                player.showbag();
                continue;
            }
            else if(re=="3"){
                player.showskillpack(anm);
            }
            else if(re=="4"){
                anm.showcharacter();
                continue;
            }
            else if(re=="5"){
                player.showcharacter();
                continue;
            }
            else{
                cout<<"输入错误"<<endl;
                continue;
            }
            if(check(player,anm)==0){
                break;
            }
            this_thread::sleep_for(chrono::seconds(1));
            //敌人回合
            cout<<anm.name<<"回合"<<endl<<endl;
            this_thread::sleep_for(chrono::seconds(1));
            anm_action(anm,player);
            buffmanage(player);
            buffmanage(anm);
        }
    }
//战斗函数
void battlestar(character& player,vector<character> anms){
    daluan(anms);
    int i = 0;
    for(i=0;i<anms.size();i++){
       cout<<endl<<"你遇到了"<< anms[i].name<<"  ";
       this_thread::sleep_for(chrono::seconds(1));
       cout<<"是否前去:1.是 2.否"<<endl;
       string s;
       cin>>ws;
       getline(cin,s);
       if(s=="1"){
            reallbattle(player,anms[i]);
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
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    srand(time(0));
    //初始化技能/////////////////////
    
    /////初始化敌人//////////////////////////////////////////////
    character anm1;
    anm1.setcharacter("雪怪",30,5,0,5,5,1);
    weapon dabang;
    dabang.setweapon("大棒",10,0,0,0,0);
    weapon suozijia;
    suozijia.setweapon("皮甲",0,0,3,0,0);
    anm1.warmon.push_back(dabang);
    anm1.warmon.push_back(suozijia);
    anm1.qvq.push_back(make_shared<normalattack>());
    anms1.push_back(anm1);//anms1是嚎哭深渊怪物集合
    ////////////////////////////////////////
    character anm2;
    anm2.setcharacter("冰鸟",20,5,10,1,1,10);
    weapon binshuangzhijie;
    binshuangzhijie.setweapon("冰霜之戒",0,20,0,0,0);
    anm2.warmon.push_back(binshuangzhijie);
    anm2.qvq.push_back(make_shared<bingci>());
    anm2.qvq.push_back(make_shared<normalattack>());
    anm2.qvq.push_back(make_shared<bingfengzhanchi>());
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
            w1.setweapon("木剑",3,0,0,0,0);
            newcharacter.warmon.push_back(w1);
            characters.push_back(newcharacter);
            sheild sheild1;
            newcharacter.qvq.push_back(make_shared<sheild>());
            cout<<"你的名字叫"<<newcharacter.name<<endl;
            gamestar(newcharacter);

        }

    }
    
}