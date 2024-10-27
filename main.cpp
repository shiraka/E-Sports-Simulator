#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define K 1000
#define LONG 70
struct player   //选手资料结构体
{

    char name[12];
    int score[6];   //1~5号位能力
    int sc; //自信
    float tf[3];    //t天赋
    char tfa[4];
    char rare[4];
    /*
    tt为天梯分，score[1~5]为一号位~五号位能力,tf为学习天赋，sc为自信。
    天梯分为一至五号位能力中，最高的三项的平均分。
    训练安排在哪个位置就会增加哪方面的能力，其他位置对应增加。
    对应号位能力增加公式为( (500*K)/score[i]-500 ) * tf  如果获胜再乘1.5  |  其他号位能力增加公式( (500*K)/score[i]-500 ) * 【胜0.1 负0.2】
    tf[0]核心天赋（对应一、二号位），tf[1]抗压天赋（对应三号位），tf[2]辅助天赋（对应四、五号位）。
    天赋范围[0.5，2.0],极端情况下允许 0.4 和 2.1 。精确至小数点后一位。
    标准天赋：不擅长C 0.5 | 正常B 1.0 | 有天赋A 1.5 | 天才S 2.0 |
    非标准天赋定级区间：C [0.4,0.7] B[0.8,1.2] A[1.3,1.7] S[1.8,2.1]
    失败会降低自信，胜利会增加自信。失败 sc=sc/2 胜利 sc+=(100-sc)/2
    */
};

struct club //俱乐部资料结构体
{
	char name[12];
    char lead[12];
    int money=100;
    int know=0;
	int n=0;
	struct player p[10];
}mc;

struct goods //物品资料结构体
{
    char name[12];
    char in[50];
}goods[]=
{
    {"抽奖券","用于抽奖的消耗品。"},    //0
    {"普通饰品","最常见的饰品。"},
    {"罕见饰品","比较常见的饰品，可以赠送给选手。"},    //2
    {"稀有饰品","相对少见的饰品，可以赠送给选手。"},
    {"神话饰品","比较珍贵的饰品，可以赠送给选手。"},    //4
    {"传说饰品","特别珍贵的饰品，可以赠送给选手。"},
    {"不朽饰品","极其珍贵的饰品，可以赠送给选手。"},    //6
    {"至宝","至宝！是至宝！！！你准备把它送给哪个选手呢？"},
    {"比赛录像","用于举行录像分析会。"}   //8
};
#define GOODS 9
int bag[GOODS]={0,0,0};

int year=2011,month=1,week=1,ap=5,maxap=5,opt,opt2,tnum[6];
bool teamis=false;

void loading(int s);   //读条动画
int tt(int sco[]);  //天梯分数计算公式
int pay();
int R(int a,int b); //[a,b)的随机数
int jcf(struct player p,int a);   //计算比赛个人基础分
void ss();  //分隔符号
void kh(int a);  //空行
void sl(){Sleep(1000);} //等待（）秒
void start();    //开头剧情
void timego();  //时间推移一个月

void menu();    //显示主菜单
void playerList();
void recruit();
void team();
void goodsList();
void recentMatch();
void goOut();
void finishMonth();
void setting();
void save();
void load();
void trigger(); //放在menu开头，用于触发剧情或赛事，并跳转至对应函数
void match();

int main()
{
    srand((unsigned)time(NULL));
    int i,j;
    system("mode con cols=70 lines=20");
    system("title 电竞模拟器 v0.1.0");

    ss();
    for(i=1;i<=28;i++)printf(" ");
    printf("电 竞 模 拟 器 \n");
    for(i=1;i<=26;i++)printf(" ");
    printf("E-Sports Simulator \n");
    ss();
    for(i=1;i<=29;i++)printf(" ");
    printf("1.新的游戏\n");
    for(i=1;i<=29;i++)printf(" ");
    printf("2.读取存档\n");
    kh(12);
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);
    if(opt==2)
    {
        load();
        goto opt1;
    }
    start();
opt1:
    menu();

    system("pause");
    return 0;
}

//其他函数
int tt(int sco[])
{
    int fen[6];
    int i,j,t,k;
    for(i=0;i<6;i++)
        fen[i]=sco[i];

    for(i=1;i<5;i++)
    {
        k=i;
        for(j=i+1;j<=5;j++)
        {
            if(fen[k]>fen[j])
            {
                k=j;
            }
        }
        if(k!=i)
        {
            t=fen[i];
            fen[i]=fen[k];
            fen[k]=t;
        }
    }
    t=(fen[3]+fen[4]+fen[5])/3;
    return t;
}
int pay()
{
    int i,ttf,sum=0;
    for(i=0;i<mc.n;i++)
    {
        ttf=tt(mc.p[i].score);
        if(ttf<2000)
            sum+=2;
        else if(ttf<4000)
            sum+=3;
        else if(ttf<6000)
            sum+=4;
        else if(ttf<8000)
            sum+=5;
        else
            sum+=6;
    }
    return sum;
}
int JCF(struct player p,int a)
{
    int f;
    f=( p.score[a] + tt(p.score) ) * ( 100 + p.sc )/400;
    return f;
}
void loading(int s)
{
    int i;
    for(i=0;i<LONG;i++)
        printf("_");
    printf("\n");
    for(i=0;i<LONG;i++)
    {
        Sleep(s*1000/LONG);
        printf("*");
    }
    printf("\n");
}
void ss()
{
    int i;
    for(i=0;i<LONG;i++)
        printf("=");
    printf("\n");
}
void kh(int a)
{
    int i;
    for(i=1;i<=a;i++)
        printf("\n");
}
void start()
{
    int i;
    char a;
    system("cls");
    sl();
    getchar();
    printf("助手:\n老板您终于来了，今天是我们电竞俱乐部开张的日子。\n");getchar();
    printf("老板您看，这是我为您招募的第一批选手们。\n");getchar();
    printf("辣鸡:\n你好，我是辣鸡。我从DOTA1打到现在，天梯3000分，擅长二号位。\n");getchar();
    printf("我:\n辣鸡你好。我叫……\n");getchar();
    system("cls");
    ss();
    printf("请输入您的名字:\n");
    gets(mc.lead);
    ss();
    printf("请输入电竞俱乐部的名字:\n");
    gets(mc.name);
    loading(1);
    sl();
    getchar();
    system("cls");
    printf("%s:\n你好，我叫%s,欢迎加入%s。\n",mc.lead,mc.lead,mc.name);getchar();
    strcpy(mc.p[0].name,"辣鸡");
    mc.p[0].score[1]=1000;
    mc.p[0].score[2]=4000;
    mc.p[0].score[3]=2500;
    mc.p[0].score[4]=2500;
    mc.p[0].score[5]=2000;
    mc.p[0].sc=80;
    mc.p[0].tf[0]=1.0;mc.p[0].tfa[0]='B';
    mc.p[0].tf[1]=0.5;mc.p[0].tfa[1]='C';
    mc.p[0].tf[2]=1.0;mc.p[0].tfa[2]='B';
    strcpy(mc.p[0].rare,"R");
    mc.n++;

    printf("新成员加入！\n");
    ss();
    printf("%-12s%6s%6s%6s%6s%6s%6s%6s %s\n","选手名字","自信","天梯","1号位","2号位","3号位","4号位","5号位","核/抗/辅");
    printf("%-12s%6d%6d", mc.p[0].name , mc.p[0].sc , tt(mc.p[0].score) );
    for(i=1;i<=5;i++)
        printf("%6d",mc.p[0].score[i]);
    printf("  %c/%c/%c    %-3s\n",mc.p[0].tfa[0],mc.p[0].tfa[1],mc.p[0].tfa[2],mc.p[0].rare);
    ss();
    system("pause");
    system("cls");
    printf("助手:\n好的，那么我来带您熟悉一下您工作的地方。\n");getchar();
    system("cls");
    printf("||| %4d年%2d月 |||                     | 行动: %2d/%-2d | 资金:%6d K |\n",year,month,ap,maxap,mc.money);
    ss();
    printf("【%s电竞俱乐部】 - 负责人:%s\n",mc.name,mc.lead);
    printf("成员数:%2d   影响力:%5d   月结工资:%dk\n",mc.n,mc.know,pay());
    ss();
    getchar();
    printf("助手:\n这是您的办公室，您可以在这里查看到俱乐部的各种信息，可以处理俱乐部的各种事务。\n");getchar();
    printf("每次你举办练习赛或是招募、解雇成员的时候都会消耗一定的行动点数。\n");getchar();
    printf("每个月只有5点行动点数，用尽时可以选择“4.本月终了”来结束本月。\n");getchar();
    printf("好了，剩下的就交给你了，%s\n",mc.lead);getchar();
}
void timego()
{
    week++;
    if(week==5)
    {
        week=1;
        month++;
        if(month==13)
        {
            month=1;
            year++;
        }
    }
}
void menu()
{
    int i,j;
    bool baoXian=true;

    system("cls");
    ss();
    printf("||| %4d年%2d月-第%d周 |||               | 行动: %2d/%-2d | 资金:%6d K |\n",year,month,week,ap,maxap,mc.money);
    ss();
    printf("【%s电竞俱乐部】 - 负责人:%s\n",mc.name,mc.lead);
    printf(" 成员数：%-2d     影响力：%-5d     月结工资：%-2dk\n",mc.n,mc.know,pay());
    ss();
    printf("  1.选手列表\n  2.物品一览\n  3.近期赛事\n  4.出去走走\n  5.休息\n\n  0.设置\n");
    kh(5);
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);
    switch(opt)
    {
    case 1:
        playerList();
        break;
    case 2:
        goodsList();

    case 3:
        break;
    case 4:
        break;
    case 5:
        if(week==4)
        {
            system("cls");
            ss();
            for(i=1;i<=30;i++) printf(" ");
            printf("月 末 结 算\n");
            ss();
            int out,in;
            out=pay()+3;
            in=mc.know/100;
            printf("  -本月支付员工工资 %dK ,水电房租 %dK 。\n",pay(),3);
            printf("  -本月广告收入 %dK 。\n",mc.know/100);
            ss();
            if(out>in)
                printf("  合计支出 %dK",out-in);
            else
                printf("  合计收入 %dK",in-out );
            printf(" | 资金: %dK => ",mc.money);
            mc.money=mc.money-out+in;
            printf("%dK\n",mc.money);
            ss();
            kh(1);
            if(out>=in || mc.money<20 )
            {
                printf("资金建议：\n");
                if(out>=in+10)
                    printf("    *本月您的月末结算支出较大，可以通过裁减多余成员来减少工资支出，也可以提高俱乐部的影响力来提高广告收入。\n");
                if(mc.money<20)
                    printf("    *您的资金已经低于2万元(20k)，请尽快提高月结收入或争夺比赛奖金，否则您的俱乐部可能将面临破产！\n");
            }
            kh(1);
            system("pause");
            if(mc.money<=10 && baoXian)
            {
                baoXian=false;
                getchar();
                system("cls");
                printf("助手：\n天呐，我的老板。这里有一个坏消息，我们俱乐部的资金已经所剩无几了。\n");getchar();
                printf("%s：\n我发誓这是一场意外，我现在只需要一点额外的资金便可以力挽狂澜。\n",mc.lead);getchar();
                printf("助手：\n你先别急，我再告诉你个好消息。投资方觉得我们的俱乐部很有潜力，今天上午又注资10万(100k)。我们还不至于立刻破产。\n");getchar();
                printf("助手：\n不过你也别高兴太早，我们不会每次都这么走运的。请务必好好经营，不要再让类似的危机出现了。\n");getchar();
                system("cls");
                ss();
                printf("获得100K资金！\n");
                ss();
                mc.money+=100;
                system("pause");
            }
            ap=maxap;
        }
        else
        {
            system("cls");
            ss();
            printf("一周过去了...\n休息后，您变得精力充沛！\n");
            ss();
            kh(2);
            system("pause");
        }
        timego();
        break;
    default:
        setting();
    }
    menu();
}
void playerList()
{
pl:
    int thfen=0,yyfen=0,adds;
    int i,j;
    system("cls");
    ss();
    printf("||| %4d年%2d月-第%d周 |||               | 行动: %2d/%-2d | 资金:%6d K |\n",year,month,week,ap,maxap,mc.money);;
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("选 手 列 表\n");
    ss();
    printf("%-12s%6s%6s%6s%6s%6s%6s%6s %s\n","选手名字","自信","天梯","1号位","2号位","3号位","4号位","5号位","核/抗/辅");
    for(i=0;i<mc.n;i++)
    {
        printf("%-12s%6d%6d", mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) );
        for(j=1;j<=5;j++)
            printf("%6d",mc.p[i].score[j]);
        printf("  %c/%c/%c    %-3s\n",mc.p[i].tfa[0],mc.p[i].tfa[1],mc.p[i].tfa[2],mc.p[i].rare);
    }
    ss();
    kh(10-mc.n);
    printf("/ 1.练习赛 / 2.编队 / 3.招募选手 / 4.解雇选手 / 0.返回 /\n");
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);

    switch(opt)
    {
    case 1:
        system("cls");
        if(mc.n<5)
        {
            ss();
            printf("对不起，您的俱乐部选手小于5人，无法举办练习赛！\n");
            ss();
            system("pause");
            goto pl;
        }
        ss();
        printf("练习赛\n    -消耗2点行动点数\n");
        ss();
        printf("/  1.确定  /  2.取消  /\n");\
        scanf("%d",&opt);
        if(opt==1)
        {
            if(ap<2)
            {
                system("cls");
                ss();
                printf("行动点数不足！\n");
                ss();
                system("pause");
                goto pl;
            }
            ap-=2;
            system("cls");
            printf("将派出队伍的前五名选手参赛\n");
            ss();
            printf("%-6s%-12s%6s%6s%6s\n","定位","选手名字","自信","天梯分","号位能力");
            for(i=0;i<mc.n;i++)
            {
                printf("%-1d号位 %-12s%6d%6d%6d %6s\n",i+1,mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) ,mc.p[i].score[i+1],mc.p[i].rare);
            }
            ss();
            printf("/ 1.确定 / 2.修改编队 /\n");
            scanf("%d",&opt);
            if(opt==2)
            {
                team();
            }
            /*比赛开始*/
            system("cls");
            for(i=0;i<5;i++)
            {
                thfen+=JCF(mc.p[i],i+1);
                yyfen+=JCF(mc.p[i],i+1);
            }
            thfen/=5;yyfen/=5;
            printf("天辉基础分：%d 夜宴基础分：%d\n",thfen,yyfen);
            thfen=thfen*R(10,20)/10;yyfen=yyfen*R(10,20)/10;
            printf("天辉实际发挥：%d 夜宴实际发挥：%d\n",thfen,yyfen);
            if(thfen>=yyfen)
            {
                printf("我方获胜！\n");
                ss();
                for(i=0;i<5;i++)
                {
                    adds=( (500*K)/ mc.p[i].score[i+1] - 50 ) * 1.5;
                    if(i<2) adds*=mc.p[i].tf[0] ;
                    else if(i==2) adds*=mc.p[i].tf[1] ;
                    else adds*=mc.p[i].tf[2] ;
                    mc.p[i].score[i+1]+=adds;
                    printf("%s.%s的%d号位能力提升 %d ，其他能力小幅提升！\n",mc.name,mc.p[i].name,i+1,adds);
                    for(j=1;j<=5;j++)//其他号位小幅增加
                    {
                        mc.p[i].score[j]+=( (500*K)/ mc.p[i].score[j] - 50 ) * 0.2;
                    }
                }
                system("pause");
                playerList();
            }
            else
            {
                printf("敌方获胜！\n");
                ss();
                for(i=0;i<5;i++)
                {
                    adds=( (500*K)/ mc.p[i].score[i+1] - 50 );
                    if(i<2) adds*=mc.p[i].tf[0] ;
                    else if(i==2) adds*=mc.p[i].tf[1] ;
                    else adds*=mc.p[i].tf[2] ;
                    mc.p[i].score[i+1]+=adds;
                    printf("%s.%s的%d号位能力提升 %d ，其他能力小幅提升！\n",mc.name,mc.p[i].name,i+1,adds);
                    for(j=1;j<=5;j++)//其他号位小幅增加
                    {
                        mc.p[i].score[j]+=( (500*K)/ mc.p[i].score[j] - 50 ) * 0.1;
                    }
                }
                system("pause");
                playerList();
            }
        }

    case 2:
        system("cls");
        ss();
        for(i=1;i<=30;i++)printf(" ");
        printf("选 手 编 队\n");
        ss();
        printf("  选手列表的前五名选手即为参赛选手，按顺序分别为一号位、二号位……最后是五号位。\n将你确定的参赛选手安排到前五顺位即可。\n");
        ss();
        system("pause");
        team();
        break;

    case 3:
        system("cls");
        ss();
        printf("招募选手\n    -消耗2点行动点数\n    -花费1k用于宣传\n");
        if(mc.know<100)
            printf("   *影响力大于100可招募到R\n");
        if(mc.know<400)
            printf("   *影响力大于400可招募到SR\n");
        if(mc.know<1000)
            printf("   *影响力大于1000可招募到SSR\n");
        ss();
        printf("/  1.确定  /  2.取消  /\n");\
        scanf("%d",&opt);
        if(opt==1)
        {
            if(ap<2 || mc.money<1)
            {
                system("cls");
                ss();
                printf("行动点数或资金不足！\n");
                ss();
                system("pause");
                goto pl;
            }
            ap-=2;
            mc.money-=1;
            recruit();
        }
        else
        {

        }
    case 4:
        c4:
        system("cls");
        printf("输入你想要解雇的选手的序号\n");
        ss();
        printf("%-12s%6s%6s%6s%6s%6s%6s%6s%6s%4s\n","选手名字","自信","天梯","1号位","2号位","3号位","4号位","5号位","","序号");
        for(i=0;i<mc.n;i++)
        {
            printf("%-12s%6d%6d", mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) );
            for(j=1;j<=5;j++)
                printf("%6d",mc.p[i].score[j]);
            printf("   %-3s%3d\n",mc.p[i].rare,i+1);
        }
        scanf("%d",&opt);
        if(opt<=mc.n)
        {
            ss();
            system("cls");
            ss();
            printf("解雇 %s 【%s】\n    -消耗1点行动点数\n",mc.p[opt-1].name,mc.p[opt-1].rare);
            ss();
            printf("/ 1.确定 / 0.取消 /");
            scanf("%d",&opt2);
            if(opt2==1);
            {
                for(i=opt-1;i<mc.n-1;i++)
                    mc.p[i]=mc.p[i+1];
                strcpy(mc.p[i].name,"解雇");
                mc.n--;
            }
        }
        else
        {
            ss();
            printf("你怕是活在梦里，你压根没这个选手！\n");
            ss();
            system("pause");
            goto c4;
        }
    default:
        menu();
    }
    goto pl;
}
void recruit()
{
    //N 0.5 R 0.5->1.0 SR 1.0->1.5 SSR 1.5->2.0
    //
    int i,j,ra,s;
    struct player n[13]=
    {
        {"忘切假腿",0,2000,1000,600,800,500,100,0.5,0.6,0.5,"CCC","N"},
        {"这游戏真难",0,2000,1500,1000,1000,1000,100,0.5,0.5,0.5,"CCC","N"},
        {"ONE",0,1500,500,500,500,500,100,0.5,0.6,0.5,"CCC","N"},
        {"大龙",0,1000,1000,1500,1500,1500,100,0.5,0.5,0.5,"CCC","N"},
        {"cc",0,1000,1000,1500,1000,2000,100,0.6,0.5,0.5,"CCC","N"},
        {"三路全崩",0,1000,1000,2000,1000,1500,100,0.6,0.5,0.5,"CCC","N"},
        {"新手上路",0,1000,500,500,1500,2000,100,0.5,0.5,0.6,"CCC","N"},
        {"我买鸡",0,1000,500,500,1000,2000,100,0.5,0.5,0.6,"CCC","N"},
        {"风杖风杖",0,1000,1000,1500,1000,1000,100,0.6,0.5,0.5,"CCC","N"},
        {"推推风杖",0,1000,1000,1000,2000,1000,100,0.5,0.6,0.5,"CCC","N"},
        /*坑货预定*/
        {"人形外挂",0,2000,1500,500,1000,500,100,0.6,0.5,0.5,"CCC","N"},
        {"国服第一",0,1500,1500,500,500,400,100,0.5,0.5,0.5,"CCC","N"},
        {"带飞全场",0,1000,1000,500,500,400,100,0.5,0.5,0.5,"CCC","N"},
    };
    struct player r[11]=
    {
        {"hubaba",0,2500,2000,2000,1000,1500,100,1.0,0.5,0.5,"BCC","R"},
        {"FangHa",0,2000,1500,1000,1000,500,100,1.0,0.5,0.5,"BCC","R"},
        {"Black Tech",0,2000,2000,2000,1000,1500,100,1.0,0.5,0.5,"BCC","R"},
        {"miaojun",0,1000,2000,1500,1000,1000,100,1.0,0.5,0.5,"BCC","R"},
        {"p.shadow",0,1000,1000,3000,1500,1000,100,0.5,1.0,0.5,"CBC","R"},
        {"KKK",0,1000,1000,3000,2000,2000,100,0.5,1.0,1.0,"CBC","R"},
        {"KuaiX",0,1000,1000,2000,1000,1000,100,0.5,1.0,0.5,"CBC","R"},
        {"AnBanLion",0,1500,1500,2500,2000,2000,100,0.5,1.0,1.0,"CBB","R"},
        {"Shiraka",0,1000,1000,1000,2000,2000,100,0.5,0.5,1.5,"CCA","R"},
        {"TTT",0,1500,1000,1500,2000,1000,100,0.5,0.5,1.0,"CCB","R"},
        {"包鸡包眼",0,1500,1000,1000,1500,2000,100,0.5,0.5,1.0,"CCB","R"},

    };
    struct player sr[5]=
    {
        {"跳刀跳刀",0,5000,8000,3000,4000,4000,100,1.5,1.0,1.0,"ABB","SR"},
        {"Shadow",0,8000,5000,4000,4000,3000,100,1.5,1.0,1.0,"ABB","SR"},
        {"ddc",0,5000,4000,4000,3000,3000,100,1.5,1.0,1.0,"ABB","SR"},
        {"Xiao8",0,3000,3000,6000,5000,4000,100,1.0,1.0,1.5,"BAA","SR"},
        {"FAITH",0,3000,3000,4000,5000,6000,100,1.0,1.0,1.5,"BBA","SR"},

    };
    struct player ssr[9]=
    {
        {"09伍声",0,4000,7000,6000,4000,5000,100,2.0,1.5,1.5,"SAS","SSR"},
        {"HAO",0,6000,3000,4000,6000,3000,100,2.0,1.5,1.5,"SAA","SSR"},
        {"BANANA",0,6000,6000,6000,6000,6000,100,2.0,2.0,1.5,"SSA","SSR"},
        {"YYF",0,4000,6000,7000,4000,5000,100,1.5,2.0,1.5,"ASA","SSR"},
        {"CHUAN",0,4000,7000,6000,4000,5000,100,2.0,1.5,1.5,"SAA","SSR"},
        {"ZHOU",0,7000,4000,5000,4500,5500,100,2.0,1.5,1.5,"SAA","SSR"},
        {"430",0,4000,7000,6000,4000,5000,100,2.0,1.5,1.5,"SAA","SSR"},
        {"BurNing",0,9000,6000,4000,4000,6000,100,2.0,1.5,2.0,"SAS","SSR"},
        {"SanSheng",0,6000,6000,4000,4000,9000,100,1.5,1.5,2.0,"AAS","SSR"},

    };
    system("cls");
    ra=R(1,101);
    // N 60% | R 25% | SR 10% | SSR 5%
    if(ra<=40)  //40%
    {
    theN:
        s=R(0,13);
        for(i=0;i<mc.n;i++)
            if(mc.p[i].name==n[s].name)
                goto theN;
        mc.p[mc.n]=n[s];
        mc.n++;
        printf("有一名新手玩家回应，玩家信息如下：\n");
        ss();
        printf("玩家id:%-12s【%s】\n",n[s].name,n[s].rare);
        ss();
        printf("| 天梯分数:%5d分 | 自信:%4d |\n",tt(n[s].score),n[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d号位:%5d分 ", i , n[s].score[i] );
        }
        printf("\n\n");

        printf("天赋:\n| 核心天赋:%c | 抗压天赋:%c | 辅助天赋:%c |\n",n[s].tfa[0],n[s].tfa[1],n[s].tfa[2]);
    }
    else if(ra<=60 && mc.know>=100) //20%
    {
    theR:
        s=R(0,11);
        for(i=0;i<mc.n;i++)
            if(mc.p[i].name==r[s].name)
                goto theR;
        mc.p[mc.n]=r[s];
        mc.n++;
        printf("有一名玩家回应，玩家信息如下：\n");
        ss();
        printf("玩家id:%-12s【%s】\n",r[s].name,r[s].rare);
        ss();
        printf("| 天梯分数:%5d分 | 自信:%4d |\n",tt(r[s].score),r[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d号位:%5d分 ", i , r[s].score[i] );
        }
        printf("\n\n");

        printf("天赋:\n| 核心天赋:%c | 抗压天赋:%c | 辅助天赋:%c |\n",r[s].tfa[0],r[s].tfa[1],r[s].tfa[2]);
    }
    else if(ra<=70 && mc.know>=400) //10%
    {
    theSR:
        s=R(0,5);
        for(i=0;i<mc.n;i++)
            if(mc.p[i].name==sr[s].name)
                goto theSR;
        mc.p[mc.n]=sr[s];
        mc.n++;
        printf("有一名职业选手回应，选手信息如下：\n");
        ss();
        printf("玩家id:%-12s【%s】\n",sr[s].name,sr[s].rare);
        ss();
        printf("| 天梯分数:%5d分 | 自信:%4d |\n",tt(sr[s].score),sr[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d号位:%5d分 ", i , sr[s].score[i] );
        }
        printf("\n\n");

        printf("天赋:\n| 核心天赋:%c | 抗压天赋:%c | 辅助天赋:%c |\n",sr[s].tfa[0],sr[s].tfa[1],sr[s].tfa[2]);
    }
    else if(ra<=70 && mc.know>=1000 )   //5%
    {
    theSSR:
        s=R(0,9);
        for(i=0;i<mc.n;i++)
            if(mc.p[i].name==ssr[s].name)
                goto theSSR;
        mc.p[mc.n]=ssr[s];
        mc.n++;
        printf("有一名职业选手回应，选手信息如下：\n");
        ss();
        printf("玩家id:%-12s【%s】\n",ssr[s].name,ssr[s].rare);
        ss();
        printf("| 天梯分数:%5d分 | 自信:%4d |\n",tt(ssr[s].score),ssr[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d号位:%5d分 ", i , ssr[s].score[i] );
        }
        printf("\n\n");

        printf("天赋:\n| 核心天赋:%c | 抗压天赋:%c | 辅助天赋:%c |\n",ssr[s].tfa[0],ssr[s].tfa[1],ssr[s].tfa[2]);
    }
    else    //25%
    {
        ss();
        printf("很遗憾,本次招募无人回应。\n");
        if(mc.know<400)
        {
            ss();
            printf("   *提高俱乐部的影响力可以增加招募回应的几率，更有机会招募到稀有选手。\n");
        }
    }
    ss();
    system("pause");
    playerList();
}
void team()
{
    int i,j,a,b;
    struct player t;
    do{
        system("cls");
        printf("  前五名为参赛选手\n");
        ss();
        printf("%-12s%6s%6s%6s%6s%6s%6s%6s%6s%4s\n","选手名字","自信","天梯","1号位","2号位","3号位","4号位","5号位","","序号");
        for(i=0;i<mc.n;i++)
        {
            printf("%-12s%6d%6d", mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) );
            for(j=1;j<=5;j++)
                printf("%6d",mc.p[i].score[j]);
            printf("   %-3s%3d\n",mc.p[i].rare,i+1);
        }
        ss();
        printf("输入两个编号呼唤顺位：(输入0结束编队)\n");
        scanf("%d",&a);
        if(a!=0){
            scanf("%d",&b);
            if(b!=0){
                if(a!=b&&a<=mc.n&&b<=mc.n)
                {
                    t=mc.p[a-1];
                    mc.p[a-1]=mc.p[b-1];
                    mc.p[b-1]=t;
                }
            }
        }
    }while(a&&b);
    system("cls");
    printf("编队完成！");
    kh(1);
    system("pause");
}
void goodsList()
{
    int i,j=0;
    system("cls");
    ss();
    printf("||| %4d年%2d月-第%d周 |||               | 行动: %2d/%-2d | 资金:%6d K |\n",year,month,week,ap,maxap,mc.money);;
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("物 品 一 览\n");
    ss();
    printf("%-6s%-12s%6s     |%-40s\n","序号","名称","数量","物品描述");
    //for(i=0;i<LONG;i++)
      //  printf("-");
    for(i=0;i<GOODS;i++)
    {
        if(bag[i]>0)
        {
            j++;
            printf("%-6d%-12s%6d     |%-40s\n",j,goods[i].name,bag[i],goods[i].in);
        }
    }
    ss();
    printf("/ 1.敬请期待 / 0.返回 /\n");
    scanf("%d",&opt);
}
void setting()
{
    int i;
    system("cls");
    ss();
    printf("||| %4d年%2d月 |||                        | 资金:%6dK | 声望:%4d |\n",year,month,mc.money,mc.know);
    ss();
    ss();
    for(i=1;i<=33;i++) printf(" ");
    printf("设 置\n");
    ss();
    printf("  1.继续游戏\n  2.保存进度\n  3.退回标题\n");
    kh(12);
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);
    switch (opt)
    {
    case 1:
        menu();break;
    case 2:
        save();break;
    case 3:
        main();break;
    default:
        menu();
    }
    save();
}
void save()
{
    //显示存档信息时，读档，存入局部变量。
    //选择存档后存档。
    int i,opt;
    bool a=true,b=true,c=true;
    FILE *mc1,*mc2,*mc3,*p1,*p2,*p3,*bag1,*bag2,*bag3;
    mc1=fopen("save1\\myclub.dat","rb");mc2=fopen("save2\\myclub.dat","rb");mc3=fopen("save3\\myclub.dat","rb");
    p1=fopen("save1\\progress.dat","rb");p2=fopen("save2\\progress.dat","rb");p3=fopen("save3\\progress.dat","rb");
    system("cls");
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("保 存 进 度\n");
    for(i=1;i<=32;i++)printf(" ");
    printf("S A V E \n");
    ss();

    ss();
    struct club lc;
    int lyear,lmonth;
    //显示存档1
    if(mc1==NULL)
    {
        a=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc1);
        fscanf(p1,"%d %d",&lyear,&lmonth);
    }
    printf("存档1:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //显示存档2
    if(mc2==NULL)
    {
        b=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc2);
        fscanf(p2,"%d %d",&lyear,&lmonth);
    }
    printf("存档2:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //显示存档3
    if(mc3==NULL)
    {
        c=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc3);
        fscanf(p3,"%d %d",&lyear,&lmonth);
    }
    printf("存档3:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    fclose(mc1);fclose(mc2);fclose(mc3);
    fclose(p1);fclose(p2);fclose(p3);
    //选择存档
    kh(3);
    printf("/ 1.存入存档1 / 2.存入存档2 / 3.存入存档3 / 0.返回主菜单 /\n");
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);
    switch(opt)
    {
    case 1:
        system("md save1");
        mc1=fopen("save1\\myclub.dat","wb");
        p1=fopen("save1\\progress.dat","wb");
        bag1=fopen("save1\\bag.dat","wb");
        fwrite(&mc,sizeof(club),1,mc1);
        fprintf(p1,"%d %d %d %d",year,month,ap,maxap);
        for(i=0;i<GOODS;i++)
            fwrite(&bag[i],sizeof(int),1,bag1);
        fclose(mc1);
        fclose(p1);
        fclose(bag1);
        break;
    case 2:
        system("md save2");
        mc2=fopen("save2\\myclub.dat","wb");
        p2=fopen("save2\\progress.dat","wb");
        bag2=fopen("save2\\bag.dat","wb");
        fwrite(&mc,sizeof(club),1,mc2);
        fprintf(p2,"%d %d %d %d",year,month,ap,maxap);
        for(i=0;i<GOODS;i++)
            fwrite(&bag[i],sizeof(int),1,bag2);
        fclose(mc2);
        fclose(p2);
        fclose(bag2);
        break;
    case 3:
        system("md save3");
        mc3=fopen("save3\\myclub.dat","wb");
        p3=fopen("save3\\progress.dat","wb");
        bag3=fopen("save3\\bag.dat","wb");
        fwrite(&mc,sizeof(club),1,mc3);
        fprintf(p3,"%d %d %d %d",year,month,ap,maxap);
        for(i=0;i<GOODS;i++)
            fwrite(&bag[i],sizeof(int),1,bag3);
        fclose(mc3);
        fclose(p3);
        fclose(bag3);
        break;
    default:
        break;
    }
}

void load()
{
    //显示存档信息时，第一次读档，存入局部变量。
    //选择存档后第二次读档，存入全局变量。
    int i,opt;
    bool a=true,b=true,c=true;
    FILE *mc1,*mc2,*mc3,*p1,*p2,*p3,*bag1,*bag2,*bag3;
    mc1=fopen("save1\\myclub.dat","rb");mc2=fopen("save2\\myclub.dat","rb");mc3=fopen("save3\\myclub.dat","rb");
    p1=fopen("save1\\progress.dat","rb");p2=fopen("save2\\progress.dat","rb");p3=fopen("save3\\progress.dat","rb");
lo:
    system("cls");
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("读 取 存 档\n");
    for(i=1;i<=32;i++)printf(" ");
    printf("L O A D \n");
    ss();

    ss();
    struct club lc;
    int lyear,lmonth;
    //显示存档1
    if(mc1==NULL)
    {
        a=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc1);
        fscanf(p1,"%d %d",&lyear,&lmonth);
    }
    printf("存档1:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //显示存档2
    if(mc2==NULL)
    {
        b=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc2);
        fscanf(p2,"%d %d",&lyear,&lmonth);
    }
    printf("存档2:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //显示存档3
    if(mc3==NULL)
    {
        c=false;
        strcpy(lc.name,"空");
        strcpy(lc.lead,"空");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc3);
        fscanf(p3,"%d %d",&lyear,&lmonth);
    }
    printf("存档3:");
    printf("【%s俱乐部 - %s】\n",lc.name,lc.lead);
    printf("| 游戏时间: %4d年%2d月 | ",lyear,lmonth);
    printf("资金:%6dK | 声望:%4d | 成员:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    fclose(mc1);fclose(mc2);fclose(mc3);
    fclose(p1);fclose(p2);fclose(p3);
    //选择存档
    kh(3);
    printf("/ 1.读取存档1 / 2.读取存档2 / 3.读取存档3 / 0.返回主菜单 /\n");
    ss();
    printf("输入对应数字选择选项: ");
    scanf("%d",&opt);
    switch(opt)
    {
    case 1:
        if(a==false)
        {
            system("cls");
            ss();printf("\n    这是一个空的存档!\n\n");ss();
            kh(14);
            system("pause");
            goto lo;
        }
        mc1=fopen("save1\\myclub.dat","rb");
        p1=fopen("save1\\progress.dat","rb");
        bag1=fopen("save1\\bag.dat","rb");
        fread(&mc,sizeof(club),1,mc1);
        fscanf(p1,"%d %d %d %d",&year,&month,&ap,&maxap);
        for(i=0;i<GOODS;i++)
                fread(&bag[i],sizeof(int),1,bag1);
        fclose(mc1);
        fclose(p1);
        fclose(bag1);
        break;
    case 2:
        if(b==false)
        {
            system("cls");
            ss();printf("\n    这是一个空的存档!\n\n");ss();
            kh(14);
            system("pause");
            goto lo;
        }
        mc2=fopen("save2\\myclub.dat","rb");
        p2=fopen("save2\\progress.dat","rb");
        bag2=fopen("save2\\bag.dat","rb");
        fread(&mc,sizeof(club),1,mc2);
        fscanf(p2,"%d %d %d %d",&year,&month,&ap,&maxap);
        for(i=0;i<GOODS;i++)
            fread(&bag[i],sizeof(int),1,bag2);
        fclose(mc2);
        fclose(p2);
        fclose(bag3);
        break;
    case 3:
        if(c==false)
        {
            system("cls");
            ss();printf("\n    这是一个空的存档!\n\n");ss();
            kh(14);
            system("pause");
            goto lo;
        }
        mc3=fopen("save3\\myclub.dat","rb");
        p3=fopen("save3\\progress.dat","rb");
        bag3=fopen("save3\\bag.dat","rb");
        fread(&mc,sizeof(club),1,mc3);
        fscanf(p3,"%d %d %d %d",&year,&month,&ap,&maxap);
        for(i=0;i<GOODS;i++)
            fread(&bag[i],sizeof(int),1,bag3);
        fclose(mc3);
        fclose(p3);
        fclose(bag3);
        break;
    default:
        main();
    }
}
int R(int a,int b)
{
    int t;
	return (rand()%(b-a))+a;
}
int TI=0;
void trigger()
{
    if(month==8)
    {
        TI++;
        system("cls");
        ss();
        printf("TI%d - %d国际邀请赛");
        ss();
        printf("");
        if(teamis==false)
        {
            printf("你还没有一个编队，请先建立一个编队");
            team();
        }

    }
}
void match()
{

}
