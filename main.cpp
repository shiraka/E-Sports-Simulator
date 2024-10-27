#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define K 1000
#define LONG 70
struct player   //ѡ�����Ͻṹ��
{

    char name[12];
    int score[6];   //1~5��λ����
    int sc; //����
    float tf[3];    //t�츳
    char tfa[4];
    char rare[4];
    /*
    ttΪ���ݷ֣�score[1~5]Ϊһ��λ~���λ����,tfΪѧϰ�츳��scΪ���š�
    ���ݷ�Ϊһ�����λ�����У���ߵ������ƽ���֡�
    ѵ���������ĸ�λ�þͻ������ķ��������������λ�ö�Ӧ���ӡ�
    ��Ӧ��λ�������ӹ�ʽΪ( (500*K)/score[i]-500 ) * tf  �����ʤ�ٳ�1.5  |  ������λ�������ӹ�ʽ( (500*K)/score[i]-500 ) * ��ʤ0.1 ��0.2��
    tf[0]�����츳����Ӧһ������λ����tf[1]��ѹ�츳����Ӧ����λ����tf[2]�����츳����Ӧ�ġ����λ����
    �츳��Χ[0.5��2.0],������������� 0.4 �� 2.1 ����ȷ��С�����һλ��
    ��׼�츳�����ó�C 0.5 | ����B 1.0 | ���츳A 1.5 | ���S 2.0 |
    �Ǳ�׼�츳�������䣺C [0.4,0.7] B[0.8,1.2] A[1.3,1.7] S[1.8,2.1]
    ʧ�ܻή�����ţ�ʤ�����������š�ʧ�� sc=sc/2 ʤ�� sc+=(100-sc)/2
    */
};

struct club //���ֲ����Ͻṹ��
{
	char name[12];
    char lead[12];
    int money=100;
    int know=0;
	int n=0;
	struct player p[10];
}mc;

struct goods //��Ʒ���Ͻṹ��
{
    char name[12];
    char in[50];
}goods[]=
{
    {"�齱ȯ","���ڳ齱������Ʒ��"},    //0
    {"��ͨ��Ʒ","�������Ʒ��"},
    {"������Ʒ","�Ƚϳ�������Ʒ���������͸�ѡ�֡�"},    //2
    {"ϡ����Ʒ","����ټ�����Ʒ���������͸�ѡ�֡�"},
    {"����Ʒ","�Ƚ�������Ʒ���������͸�ѡ�֡�"},    //4
    {"��˵��Ʒ","�ر�������Ʒ���������͸�ѡ�֡�"},
    {"������Ʒ","����������Ʒ���������͸�ѡ�֡�"},    //6
    {"����","��������������������׼�������͸��ĸ�ѡ���أ�"},
    {"����¼��","���ھ���¼������ᡣ"}   //8
};
#define GOODS 9
int bag[GOODS]={0,0,0};

int year=2011,month=1,week=1,ap=5,maxap=5,opt,opt2,tnum[6];
bool teamis=false;

void loading(int s);   //��������
int tt(int sco[]);  //���ݷ������㹫ʽ
int pay();
int R(int a,int b); //[a,b)�������
int jcf(struct player p,int a);   //����������˻�����
void ss();  //�ָ�����
void kh(int a);  //����
void sl(){Sleep(1000);} //�ȴ�������
void start();    //��ͷ����
void timego();  //ʱ������һ����

void menu();    //��ʾ���˵�
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
void trigger(); //����menu��ͷ�����ڴ�����������£�����ת����Ӧ����
void match();

int main()
{
    srand((unsigned)time(NULL));
    int i,j;
    system("mode con cols=70 lines=20");
    system("title �羺ģ���� v0.1.0");

    ss();
    for(i=1;i<=28;i++)printf(" ");
    printf("�� �� ģ �� �� \n");
    for(i=1;i<=26;i++)printf(" ");
    printf("E-Sports Simulator \n");
    ss();
    for(i=1;i<=29;i++)printf(" ");
    printf("1.�µ���Ϸ\n");
    for(i=1;i<=29;i++)printf(" ");
    printf("2.��ȡ�浵\n");
    kh(12);
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
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

//��������
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
    printf("����:\n�ϰ����������ˣ����������ǵ羺���ֲ����ŵ����ӡ�\n");getchar();
    printf("�ϰ�������������Ϊ����ļ�ĵ�һ��ѡ���ǡ�\n");getchar();
    printf("����:\n��ã������������Ҵ�DOTA1�����ڣ�����3000�֣��ó�����λ��\n");getchar();
    printf("��:\n������á��ҽС���\n");getchar();
    system("cls");
    ss();
    printf("��������������:\n");
    gets(mc.lead);
    ss();
    printf("������羺���ֲ�������:\n");
    gets(mc.name);
    loading(1);
    sl();
    getchar();
    system("cls");
    printf("%s:\n��ã��ҽ�%s,��ӭ����%s��\n",mc.lead,mc.lead,mc.name);getchar();
    strcpy(mc.p[0].name,"����");
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

    printf("�³�Ա���룡\n");
    ss();
    printf("%-12s%6s%6s%6s%6s%6s%6s%6s %s\n","ѡ������","����","����","1��λ","2��λ","3��λ","4��λ","5��λ","��/��/��");
    printf("%-12s%6d%6d", mc.p[0].name , mc.p[0].sc , tt(mc.p[0].score) );
    for(i=1;i<=5;i++)
        printf("%6d",mc.p[0].score[i]);
    printf("  %c/%c/%c    %-3s\n",mc.p[0].tfa[0],mc.p[0].tfa[1],mc.p[0].tfa[2],mc.p[0].rare);
    ss();
    system("pause");
    system("cls");
    printf("����:\n�õģ���ô����������Ϥһ���������ĵط���\n");getchar();
    system("cls");
    printf("||| %4d��%2d�� |||                     | �ж�: %2d/%-2d | �ʽ�:%6d K |\n",year,month,ap,maxap,mc.money);
    ss();
    printf("��%s�羺���ֲ��� - ������:%s\n",mc.name,mc.lead);
    printf("��Ա��:%2d   Ӱ����:%5d   �½Ṥ��:%dk\n",mc.n,mc.know,pay());
    ss();
    getchar();
    printf("����:\n�������İ칫�ң�������������鿴�����ֲ��ĸ�����Ϣ�����Դ�����ֲ��ĸ�������\n");getchar();
    printf("ÿ����ٰ���ϰ��������ļ����ͳ�Ա��ʱ�򶼻�����һ�����ж�������\n");getchar();
    printf("ÿ����ֻ��5���ж��������þ�ʱ����ѡ��4.�������ˡ����������¡�\n");getchar();
    printf("���ˣ�ʣ�µľͽ������ˣ�%s\n",mc.lead);getchar();
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
    printf("||| %4d��%2d��-��%d�� |||               | �ж�: %2d/%-2d | �ʽ�:%6d K |\n",year,month,week,ap,maxap,mc.money);
    ss();
    printf("��%s�羺���ֲ��� - ������:%s\n",mc.name,mc.lead);
    printf(" ��Ա����%-2d     Ӱ������%-5d     �½Ṥ�ʣ�%-2dk\n",mc.n,mc.know,pay());
    ss();
    printf("  1.ѡ���б�\n  2.��Ʒһ��\n  3.��������\n  4.��ȥ����\n  5.��Ϣ\n\n  0.����\n");
    kh(5);
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
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
            printf("�� ĩ �� ��\n");
            ss();
            int out,in;
            out=pay()+3;
            in=mc.know/100;
            printf("  -����֧��Ա������ %dK ,ˮ�緿�� %dK ��\n",pay(),3);
            printf("  -���¹������ %dK ��\n",mc.know/100);
            ss();
            if(out>in)
                printf("  �ϼ�֧�� %dK",out-in);
            else
                printf("  �ϼ����� %dK",in-out );
            printf(" | �ʽ�: %dK => ",mc.money);
            mc.money=mc.money-out+in;
            printf("%dK\n",mc.money);
            ss();
            kh(1);
            if(out>=in || mc.money<20 )
            {
                printf("�ʽ��飺\n");
                if(out>=in+10)
                    printf("    *����������ĩ����֧���ϴ󣬿���ͨ���ü������Ա�����ٹ���֧����Ҳ������߾��ֲ���Ӱ��������߹�����롣\n");
                if(mc.money<20)
                    printf("    *�����ʽ��Ѿ�����2��Ԫ(20k)���뾡������½����������������𣬷������ľ��ֲ����ܽ������Ʋ���\n");
            }
            kh(1);
            system("pause");
            if(mc.money<=10 && baoXian)
            {
                baoXian=false;
                getchar();
                system("cls");
                printf("���֣�\n���ţ��ҵ��ϰ塣������һ������Ϣ�����Ǿ��ֲ����ʽ��Ѿ���ʣ�޼��ˡ�\n");getchar();
                printf("%s��\n�ҷ�������һ�����⣬������ֻ��Ҫһ�������ʽ��������������\n",mc.lead);getchar();
                printf("���֣�\n���ȱ𼱣����ٸ����������Ϣ��Ͷ�ʷ��������ǵľ��ֲ�����Ǳ��������������ע��10��(100k)�����ǻ������������Ʋ���\n");getchar();
                printf("���֣�\n������Ҳ�����̫�磬���ǲ���ÿ�ζ���ô���˵ġ�����غúþ�Ӫ����Ҫ�������Ƶ�Σ�������ˡ�\n");getchar();
                system("cls");
                ss();
                printf("���100K�ʽ�\n");
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
            printf("һ�ܹ�ȥ��...\n��Ϣ������þ������棡\n");
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
    printf("||| %4d��%2d��-��%d�� |||               | �ж�: %2d/%-2d | �ʽ�:%6d K |\n",year,month,week,ap,maxap,mc.money);;
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("ѡ �� �� ��\n");
    ss();
    printf("%-12s%6s%6s%6s%6s%6s%6s%6s %s\n","ѡ������","����","����","1��λ","2��λ","3��λ","4��λ","5��λ","��/��/��");
    for(i=0;i<mc.n;i++)
    {
        printf("%-12s%6d%6d", mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) );
        for(j=1;j<=5;j++)
            printf("%6d",mc.p[i].score[j]);
        printf("  %c/%c/%c    %-3s\n",mc.p[i].tfa[0],mc.p[i].tfa[1],mc.p[i].tfa[2],mc.p[i].rare);
    }
    ss();
    kh(10-mc.n);
    printf("/ 1.��ϰ�� / 2.��� / 3.��ļѡ�� / 4.���ѡ�� / 0.���� /\n");
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
    scanf("%d",&opt);

    switch(opt)
    {
    case 1:
        system("cls");
        if(mc.n<5)
        {
            ss();
            printf("�Բ������ľ��ֲ�ѡ��С��5�ˣ��޷��ٰ���ϰ����\n");
            ss();
            system("pause");
            goto pl;
        }
        ss();
        printf("��ϰ��\n    -����2���ж�����\n");
        ss();
        printf("/  1.ȷ��  /  2.ȡ��  /\n");\
        scanf("%d",&opt);
        if(opt==1)
        {
            if(ap<2)
            {
                system("cls");
                ss();
                printf("�ж��������㣡\n");
                ss();
                system("pause");
                goto pl;
            }
            ap-=2;
            system("cls");
            printf("���ɳ������ǰ����ѡ�ֲ���\n");
            ss();
            printf("%-6s%-12s%6s%6s%6s\n","��λ","ѡ������","����","���ݷ�","��λ����");
            for(i=0;i<mc.n;i++)
            {
                printf("%-1d��λ %-12s%6d%6d%6d %6s\n",i+1,mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) ,mc.p[i].score[i+1],mc.p[i].rare);
            }
            ss();
            printf("/ 1.ȷ�� / 2.�޸ı�� /\n");
            scanf("%d",&opt);
            if(opt==2)
            {
                team();
            }
            /*������ʼ*/
            system("cls");
            for(i=0;i<5;i++)
            {
                thfen+=JCF(mc.p[i],i+1);
                yyfen+=JCF(mc.p[i],i+1);
            }
            thfen/=5;yyfen/=5;
            printf("��Ի����֣�%d ҹ������֣�%d\n",thfen,yyfen);
            thfen=thfen*R(10,20)/10;yyfen=yyfen*R(10,20)/10;
            printf("���ʵ�ʷ��ӣ�%d ҹ��ʵ�ʷ��ӣ�%d\n",thfen,yyfen);
            if(thfen>=yyfen)
            {
                printf("�ҷ���ʤ��\n");
                ss();
                for(i=0;i<5;i++)
                {
                    adds=( (500*K)/ mc.p[i].score[i+1] - 50 ) * 1.5;
                    if(i<2) adds*=mc.p[i].tf[0] ;
                    else if(i==2) adds*=mc.p[i].tf[1] ;
                    else adds*=mc.p[i].tf[2] ;
                    mc.p[i].score[i+1]+=adds;
                    printf("%s.%s��%d��λ�������� %d ����������С��������\n",mc.name,mc.p[i].name,i+1,adds);
                    for(j=1;j<=5;j++)//������λС������
                    {
                        mc.p[i].score[j]+=( (500*K)/ mc.p[i].score[j] - 50 ) * 0.2;
                    }
                }
                system("pause");
                playerList();
            }
            else
            {
                printf("�з���ʤ��\n");
                ss();
                for(i=0;i<5;i++)
                {
                    adds=( (500*K)/ mc.p[i].score[i+1] - 50 );
                    if(i<2) adds*=mc.p[i].tf[0] ;
                    else if(i==2) adds*=mc.p[i].tf[1] ;
                    else adds*=mc.p[i].tf[2] ;
                    mc.p[i].score[i+1]+=adds;
                    printf("%s.%s��%d��λ�������� %d ����������С��������\n",mc.name,mc.p[i].name,i+1,adds);
                    for(j=1;j<=5;j++)//������λС������
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
        printf("ѡ �� �� ��\n");
        ss();
        printf("  ѡ���б��ǰ����ѡ�ּ�Ϊ����ѡ�֣���˳��ֱ�Ϊһ��λ������λ������������λ��\n����ȷ���Ĳ���ѡ�ְ��ŵ�ǰ��˳λ���ɡ�\n");
        ss();
        system("pause");
        team();
        break;

    case 3:
        system("cls");
        ss();
        printf("��ļѡ��\n    -����2���ж�����\n    -����1k��������\n");
        if(mc.know<100)
            printf("   *Ӱ��������100����ļ��R\n");
        if(mc.know<400)
            printf("   *Ӱ��������400����ļ��SR\n");
        if(mc.know<1000)
            printf("   *Ӱ��������1000����ļ��SSR\n");
        ss();
        printf("/  1.ȷ��  /  2.ȡ��  /\n");\
        scanf("%d",&opt);
        if(opt==1)
        {
            if(ap<2 || mc.money<1)
            {
                system("cls");
                ss();
                printf("�ж��������ʽ��㣡\n");
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
        printf("��������Ҫ��͵�ѡ�ֵ����\n");
        ss();
        printf("%-12s%6s%6s%6s%6s%6s%6s%6s%6s%4s\n","ѡ������","����","����","1��λ","2��λ","3��λ","4��λ","5��λ","","���");
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
            printf("��� %s ��%s��\n    -����1���ж�����\n",mc.p[opt-1].name,mc.p[opt-1].rare);
            ss();
            printf("/ 1.ȷ�� / 0.ȡ�� /");
            scanf("%d",&opt2);
            if(opt2==1);
            {
                for(i=opt-1;i<mc.n-1;i++)
                    mc.p[i]=mc.p[i+1];
                strcpy(mc.p[i].name,"���");
                mc.n--;
            }
        }
        else
        {
            ss();
            printf("�����ǻ��������ѹ��û���ѡ�֣�\n");
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
        {"���м���",0,2000,1000,600,800,500,100,0.5,0.6,0.5,"CCC","N"},
        {"����Ϸ����",0,2000,1500,1000,1000,1000,100,0.5,0.5,0.5,"CCC","N"},
        {"ONE",0,1500,500,500,500,500,100,0.5,0.6,0.5,"CCC","N"},
        {"����",0,1000,1000,1500,1500,1500,100,0.5,0.5,0.5,"CCC","N"},
        {"cc",0,1000,1000,1500,1000,2000,100,0.6,0.5,0.5,"CCC","N"},
        {"��·ȫ��",0,1000,1000,2000,1000,1500,100,0.6,0.5,0.5,"CCC","N"},
        {"������·",0,1000,500,500,1500,2000,100,0.5,0.5,0.6,"CCC","N"},
        {"����",0,1000,500,500,1000,2000,100,0.5,0.5,0.6,"CCC","N"},
        {"���ȷ���",0,1000,1000,1500,1000,1000,100,0.6,0.5,0.5,"CCC","N"},
        {"���Ʒ���",0,1000,1000,1000,2000,1000,100,0.5,0.6,0.5,"CCC","N"},
        /*�ӻ�Ԥ��*/
        {"�������",0,2000,1500,500,1000,500,100,0.6,0.5,0.5,"CCC","N"},
        {"������һ",0,1500,1500,500,500,400,100,0.5,0.5,0.5,"CCC","N"},
        {"����ȫ��",0,1000,1000,500,500,400,100,0.5,0.5,0.5,"CCC","N"},
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
        {"��������",0,1500,1000,1000,1500,2000,100,0.5,0.5,1.0,"CCB","R"},

    };
    struct player sr[5]=
    {
        {"��������",0,5000,8000,3000,4000,4000,100,1.5,1.0,1.0,"ABB","SR"},
        {"Shadow",0,8000,5000,4000,4000,3000,100,1.5,1.0,1.0,"ABB","SR"},
        {"ddc",0,5000,4000,4000,3000,3000,100,1.5,1.0,1.0,"ABB","SR"},
        {"Xiao8",0,3000,3000,6000,5000,4000,100,1.0,1.0,1.5,"BAA","SR"},
        {"FAITH",0,3000,3000,4000,5000,6000,100,1.0,1.0,1.5,"BBA","SR"},

    };
    struct player ssr[9]=
    {
        {"09����",0,4000,7000,6000,4000,5000,100,2.0,1.5,1.5,"SAS","SSR"},
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
        printf("��һ��������һ�Ӧ�������Ϣ���£�\n");
        ss();
        printf("���id:%-12s��%s��\n",n[s].name,n[s].rare);
        ss();
        printf("| ���ݷ���:%5d�� | ����:%4d |\n",tt(n[s].score),n[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d��λ:%5d�� ", i , n[s].score[i] );
        }
        printf("\n\n");

        printf("�츳:\n| �����츳:%c | ��ѹ�츳:%c | �����츳:%c |\n",n[s].tfa[0],n[s].tfa[1],n[s].tfa[2]);
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
        printf("��һ����һ�Ӧ�������Ϣ���£�\n");
        ss();
        printf("���id:%-12s��%s��\n",r[s].name,r[s].rare);
        ss();
        printf("| ���ݷ���:%5d�� | ����:%4d |\n",tt(r[s].score),r[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d��λ:%5d�� ", i , r[s].score[i] );
        }
        printf("\n\n");

        printf("�츳:\n| �����츳:%c | ��ѹ�츳:%c | �����츳:%c |\n",r[s].tfa[0],r[s].tfa[1],r[s].tfa[2]);
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
        printf("��һ��ְҵѡ�ֻ�Ӧ��ѡ����Ϣ���£�\n");
        ss();
        printf("���id:%-12s��%s��\n",sr[s].name,sr[s].rare);
        ss();
        printf("| ���ݷ���:%5d�� | ����:%4d |\n",tt(sr[s].score),sr[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d��λ:%5d�� ", i , sr[s].score[i] );
        }
        printf("\n\n");

        printf("�츳:\n| �����츳:%c | ��ѹ�츳:%c | �����츳:%c |\n",sr[s].tfa[0],sr[s].tfa[1],sr[s].tfa[2]);
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
        printf("��һ��ְҵѡ�ֻ�Ӧ��ѡ����Ϣ���£�\n");
        ss();
        printf("���id:%-12s��%s��\n",ssr[s].name,ssr[s].rare);
        ss();
        printf("| ���ݷ���:%5d�� | ����:%4d |\n",tt(ssr[s].score),ssr[s].sc);
        for(i=1;i<=5;i++)
        {
            printf("%d��λ:%5d�� ", i , ssr[s].score[i] );
        }
        printf("\n\n");

        printf("�츳:\n| �����츳:%c | ��ѹ�츳:%c | �����츳:%c |\n",ssr[s].tfa[0],ssr[s].tfa[1],ssr[s].tfa[2]);
    }
    else    //25%
    {
        ss();
        printf("���ź�,������ļ���˻�Ӧ��\n");
        if(mc.know<400)
        {
            ss();
            printf("   *��߾��ֲ���Ӱ��������������ļ��Ӧ�ļ��ʣ����л�����ļ��ϡ��ѡ�֡�\n");
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
        printf("  ǰ����Ϊ����ѡ��\n");
        ss();
        printf("%-12s%6s%6s%6s%6s%6s%6s%6s%6s%4s\n","ѡ������","����","����","1��λ","2��λ","3��λ","4��λ","5��λ","","���");
        for(i=0;i<mc.n;i++)
        {
            printf("%-12s%6d%6d", mc.p[i].name , mc.p[i].sc , tt(mc.p[i].score) );
            for(j=1;j<=5;j++)
                printf("%6d",mc.p[i].score[j]);
            printf("   %-3s%3d\n",mc.p[i].rare,i+1);
        }
        ss();
        printf("����������ź���˳λ��(����0�������)\n");
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
    printf("�����ɣ�");
    kh(1);
    system("pause");
}
void goodsList()
{
    int i,j=0;
    system("cls");
    ss();
    printf("||| %4d��%2d��-��%d�� |||               | �ж�: %2d/%-2d | �ʽ�:%6d K |\n",year,month,week,ap,maxap,mc.money);;
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("�� Ʒ һ ��\n");
    ss();
    printf("%-6s%-12s%6s     |%-40s\n","���","����","����","��Ʒ����");
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
    printf("/ 1.�����ڴ� / 0.���� /\n");
    scanf("%d",&opt);
}
void setting()
{
    int i;
    system("cls");
    ss();
    printf("||| %4d��%2d�� |||                        | �ʽ�:%6dK | ����:%4d |\n",year,month,mc.money,mc.know);
    ss();
    ss();
    for(i=1;i<=33;i++) printf(" ");
    printf("�� ��\n");
    ss();
    printf("  1.������Ϸ\n  2.�������\n  3.�˻ر���\n");
    kh(12);
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
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
    //��ʾ�浵��Ϣʱ������������ֲ�������
    //ѡ��浵��浵��
    int i,opt;
    bool a=true,b=true,c=true;
    FILE *mc1,*mc2,*mc3,*p1,*p2,*p3,*bag1,*bag2,*bag3;
    mc1=fopen("save1\\myclub.dat","rb");mc2=fopen("save2\\myclub.dat","rb");mc3=fopen("save3\\myclub.dat","rb");
    p1=fopen("save1\\progress.dat","rb");p2=fopen("save2\\progress.dat","rb");p3=fopen("save3\\progress.dat","rb");
    system("cls");
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("�� �� �� ��\n");
    for(i=1;i<=32;i++)printf(" ");
    printf("S A V E \n");
    ss();

    ss();
    struct club lc;
    int lyear,lmonth;
    //��ʾ�浵1
    if(mc1==NULL)
    {
        a=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc1);
        fscanf(p1,"%d %d",&lyear,&lmonth);
    }
    printf("�浵1:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //��ʾ�浵2
    if(mc2==NULL)
    {
        b=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc2);
        fscanf(p2,"%d %d",&lyear,&lmonth);
    }
    printf("�浵2:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //��ʾ�浵3
    if(mc3==NULL)
    {
        c=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc3);
        fscanf(p3,"%d %d",&lyear,&lmonth);
    }
    printf("�浵3:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    fclose(mc1);fclose(mc2);fclose(mc3);
    fclose(p1);fclose(p2);fclose(p3);
    //ѡ��浵
    kh(3);
    printf("/ 1.����浵1 / 2.����浵2 / 3.����浵3 / 0.�������˵� /\n");
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
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
    //��ʾ�浵��Ϣʱ����һ�ζ���������ֲ�������
    //ѡ��浵��ڶ��ζ���������ȫ�ֱ�����
    int i,opt;
    bool a=true,b=true,c=true;
    FILE *mc1,*mc2,*mc3,*p1,*p2,*p3,*bag1,*bag2,*bag3;
    mc1=fopen("save1\\myclub.dat","rb");mc2=fopen("save2\\myclub.dat","rb");mc3=fopen("save3\\myclub.dat","rb");
    p1=fopen("save1\\progress.dat","rb");p2=fopen("save2\\progress.dat","rb");p3=fopen("save3\\progress.dat","rb");
lo:
    system("cls");
    ss();
    for(i=1;i<=30;i++)printf(" ");
    printf("�� ȡ �� ��\n");
    for(i=1;i<=32;i++)printf(" ");
    printf("L O A D \n");
    ss();

    ss();
    struct club lc;
    int lyear,lmonth;
    //��ʾ�浵1
    if(mc1==NULL)
    {
        a=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc1);
        fscanf(p1,"%d %d",&lyear,&lmonth);
    }
    printf("�浵1:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //��ʾ�浵2
    if(mc2==NULL)
    {
        b=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc2);
        fscanf(p2,"%d %d",&lyear,&lmonth);
    }
    printf("�浵2:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    //��ʾ�浵3
    if(mc3==NULL)
    {
        c=false;
        strcpy(lc.name,"��");
        strcpy(lc.lead,"��");
        lyear=0;lmonth=0;lc.money=0;lc.know=0;lc.n=0;
    }
    else
    {
        fread(&lc,sizeof(club),1,mc3);
        fscanf(p3,"%d %d",&lyear,&lmonth);
    }
    printf("�浵3:");
    printf("��%s���ֲ� - %s��\n",lc.name,lc.lead);
    printf("| ��Ϸʱ��: %4d��%2d�� | ",lyear,lmonth);
    printf("�ʽ�:%6dK | ����:%4d | ��Ա:%2d |\n",lc.money,lc.know,lc.n);
    ss();
    fclose(mc1);fclose(mc2);fclose(mc3);
    fclose(p1);fclose(p2);fclose(p3);
    //ѡ��浵
    kh(3);
    printf("/ 1.��ȡ�浵1 / 2.��ȡ�浵2 / 3.��ȡ�浵3 / 0.�������˵� /\n");
    ss();
    printf("�����Ӧ����ѡ��ѡ��: ");
    scanf("%d",&opt);
    switch(opt)
    {
    case 1:
        if(a==false)
        {
            system("cls");
            ss();printf("\n    ����һ���յĴ浵!\n\n");ss();
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
            ss();printf("\n    ����һ���յĴ浵!\n\n");ss();
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
            ss();printf("\n    ����һ���յĴ浵!\n\n");ss();
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
        printf("TI%d - %d����������");
        ss();
        printf("");
        if(teamis==false)
        {
            printf("�㻹û��һ����ӣ����Ƚ���һ�����");
            team();
        }

    }
}
void match()
{

}
