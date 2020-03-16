#include <SFML/Graphics.hpp>
#include <iostream>
#include<stdio.h>
#include <windows.h>
#define STAGE_WIDTH 880    //舞台宽度
#define STAGE_HEIGHT 600   //舞台高度
#define MAXLENGTH 100
#define INFO_WIDTH 400
#define POSX  150.0f
#define POSY  400.0f
#define ERROR 0

using namespace std;
using namespace sf;
const int width = STAGE_WIDTH;			//游戏窗口宽度
const int height = STAGE_HEIGHT;			//游戏窗口高度

RenderWindow window(sf::VideoMode(width, height), L"斗bug2.0 ");
Texture ipokers[54];   
Texture ipokersChoose[54];
Texture card;
Texture start;
Texture chupai;
Texture buchu;
Texture background;
Texture bc;
Sprite spokers[54]; //精灵对象
Sprite spokersChoose[54];
Sprite scard;
Sprite sstart;
Sprite schupai;
Sprite sbuchu;
Sprite sbackground;
Sprite sbc;
class Poker;
int dipai[3];//底牌数组
bool isStart = 0;//是否开始
bool isOver = 0;//是否结束
int isTurn;//0为不是你的回合，1为是你的回合可以随意出牌，2为是你的回合但有限制
int turn;//场上的牌是谁的
int playernumber;//是谁的回合
//初始化
void Initial() {
	window.setFramerateLimit(60);
	playernumber = 0;
	isTurn = 1;
	turn = 0;
	isOver = 0;
	if (!card.loadFromFile("images/card/ccard.png")) //加载纹理图片
	{
		cout <<  "card.png没有找到" << endl;
	}
	if (!start.loadFromFile("images/dizhu.png")) 
	{
		cout << "dizhu.png没有找到" << endl;
	}
	if (!chupai.loadFromFile("images/ok.png"))
	{
		cout << "ok.png没有找到" << endl;
	}
	if (!buchu.loadFromFile("images/no.png"))
	{
		cout << "no.png没有找到" << endl;
	}
	if (!background.loadFromFile("images/background2.png"))
	{
		cout << "background.png没有找到" << endl;
	}
	if (!bc.loadFromFile("images/buchu.png"))
	{
		cout << "buchu.png没有找到" << endl;
	}
	for (int i = 0; i < 54; i++)
	{
		char ss[30] = "images/card/card";
		char ssc[30] = "images/cardchoose/card";
		char sss[5] = ".png";
		sprintf_s(ss, "%s%d%s", ss,i, sss);
		sprintf_s(ssc, "%s%d%s", ssc, i, sss);
		if (!ipokers[i].loadFromFile(ss) ) //加载纹理图片
		{
			cout << dec << ss << "没有找到" << endl;
		}
		if (!ipokersChoose[i].loadFromFile(ssc)) //加载纹理图片
		{
			cout << dec << ssc << "没有找到" << endl;
		}
	}
	for (int i = 0; i < 54; i++)
	{
		spokers[i].setTexture(ipokers[i]);
		spokersChoose[i].setTexture(ipokersChoose[i]);
	}
	scard.setTexture(card);
	sstart.setTexture(start);
	schupai.setTexture(chupai);
	sbuchu.setTexture(buchu);
	sbackground.setTexture(background);
	sbc.setTexture(bc);
}
//玩家类
class Player
{
private:
	string name;//玩家名称
	int count[15];//每种牌的数量
	int pai[20];//记录每张牌的编号
	int n;//玩家牌的数量
	bool isChoose[20];
public:
	int number;//玩家编号
	bool canin;//判断是否可以发牌
	bool bc;//是否选择不出
	friend class Poker;//Poker为友元类
	friend int computer(int number);
	friend void computer2(int number);

	Player()//构造函数
	{
		n = 0;
		bc = 0;
		for (int i = 0; i < 20; i++)
		{
			pai[i] = 100;
			isChoose[i] = 0;
			count[i] = 0;
		}
		canin = 1;
	}
	~Player() { } //析构函数
	Player(string s, int nu)//带参构造函数
	{
		name = s;
		number = nu;
		n = 0;
		bc = 0;
		for (int i = 0; i < 20; i++)
		{
			pai[i] = 100;
			isChoose[i] = 0;
			count[i] = 0;
		}

	}
	void setname(string s)//设置名字
	{
		name = s;
	}
	//发牌
	void in(int j)
	{
		int h;
		if (j == 52)
			count[13]++;
		else if (j == 53)
			count[14]++;
		else
		{
			h = j / 4;
			count[h]++;
		}
		pai[n] = j;
		n++;
		if (n == 17)
			canin = 0;
	}
	//叫地主
	void start(int j)
	{
		int h;
		if (j == 52)
			count[13]++;
		else if (j == 53)
			count[14]++;
		else
		{
			h = j / 4;
			count[h]++;
		}
		for (int i = n - 1; i > 0; i--)
		{
			if (pai[i] < j)
			{
				pai[i + 1] = j;
				n++;
				break;
			}
			else if (pai[i] > j&&pai[i - 1] > j)
			{
				pai[i + 1] = pai[i];
			}
			else if (pai[i] > j&&pai[i - 1] < j)
			{
				pai[i + 1] = pai[i];
				pai[i] = j;
				n++;
				break;
			}
		}
		if (pai[2] == pai[1])
		{
			pai[1] = pai[0];
			pai[0]=j;
			n++;
		}
	}
	//检测发牌
	void checker()
	{
		cout << "玩家:" << name << "的牌有:" << endl;
		for (int i = 0; i <= 7; i++)
		{
			cout << dec << count[i] << "个" << i + 3 << ",";
		}
		cout << dec << count[8] << "个" << "J" << ",";
		cout << dec << count[9] << "个" << "Q" << ",";
		cout << dec << count[10] << "个" << "K" << ",";
		cout << dec << count[11] << "个" << "A" << ",";
		cout << dec << count[12] << "个" << "2" << ",";
		cout << dec << count[13] << "个" << "小王" << ",";
		cout << dec << count[14] << "个" << "大王" << ",";
		cout << endl;
	}
	//绘制扑克牌
	void Drawcard(float x, float y)
	{
		for (int i = 0; i < n; i++)
		{
			if (isChoose[i] == 0)
			{
				spokers[pai[i]].setPosition(x, y);
				window.draw(spokers[pai[i]]);
			}
			else
			{
				spokersChoose[pai[i]].setPosition(x, y);
				window.draw(spokersChoose[pai[i]]);
			}
			x += 25.0f;
		}
	}
	//画牌背
	void DrawComputer(float x, float y)
	{
		if (isOver)
		{
			for (int i = 0; i < n; i++)
			{
				spokers[pai[i]].setPosition(x, y);
				window.draw(spokers[pai[i]]);
				y += 25.0f;
			}
		}
		else
		{
			for (int i = 0; i < n; i++)
			{
				scard.setPosition(x, y);
				window.draw(scard);
				y += 25.0f;
			}
		}
	}
	//设置选中状态
	void setChoose(int i)
	{
		if (isChoose[i] == 0)
			isChoose[i] = 1;
		else if (isChoose[i] == 1)
			isChoose[i] = 0;
	}
	//出牌
	void out()
	{
		int j = 0;
		for (int i = 0; i < n; i++)
		{
			while (isChoose[i + j] == 1)
			{
				int h;
				if (pai[i+j]== 52)
					count[13]--;
				else if (pai[i + j] == 53)
					count[14]--;
				else
				{
					h = pai[i + j] / 4;
					count[h]--;
				}
				isChoose[i + j] = 0;
				j++;
			}
			pai[i] = pai[i + j];
		}
		n -= j;
	}
	//获得牌的数量
	int  getn()
	{
		return n;
	}

}; Player player[3];

class Poker//打出去的牌类
{
private:
	int pokerN;//牌的张数
	int cla;//牌的类型：1为单，2为双，3为三张，4为炸，5为顺子，6为连对，31为三带一，32为三带二，33为飞机，34为带一的飞机，35为带二的飞机，41为四带一，42为四带二，100为王炸
	int weight;//权重值，比大小用
	int Ppai[20];//存储每张牌的信息
	int Pcount[15];//每种牌的张数
	int fourcount;//有四张牌的种类
	int threecount;
	int twocount;
	int onecount;
public:
	friend int computer(int number);
	Poker()
	{
		fourcount = threecount = twocount = onecount = 0;
		cla = pokerN = weight = 0;
		for (int i = 0; i < 15; i++)
			Pcount[i] = 0;
	}
	Poker(int nn, int ww, int cc, int aa,int pp)
	{
		pokerN = nn;
		weight = ww;
		cla = cc;
		fourcount = threecount = twocount = onecount = 0;
		if (cc == 5)
		{
			int j = 0;
			for (int i = 0; i < pokerN; i++)
			{
				Ppai[i] = player[pp].pai[aa];
				aa+=player[pp].count[ww+j];
				j++;
			}
		}
		else
		{
			for (int i = 0; i < pokerN; i++)
			{
				Ppai[i] = player[pp].pai[aa];
				aa++;
			}
		}
	}
	~Poker() {}
	//统计牌组
	void PokerChecker()
	{
		//统计每种牌的数量
		int a = 0;
		for (int i = 0; i < player[0].n; i++)
		{
			if (player[0].isChoose[i])
			{
				int j = player[0].pai[i];
				int h;
				if (j == 52)
					Pcount[13]++;
				else if (j == 53)
					Pcount[14]++;
				else
				{
					h = j / 4;
					Pcount[h]++;
				}
				Ppai[a] = j;
				pokerN++;
				a++;
			}
		}
		//检查重复牌的数量
		for (int i = 0; i < 15; i++)
		{
			if (Pcount[i] == 1)
			{
				onecount++;
				weight = i;
			}
			else if (Pcount[i] == 2)
			{
				twocount++;
				weight = i;
			}
			else if (Pcount[i] == 3)
			{
				threecount++;
				weight = i;
			}
			else if (Pcount[i] == 4)
			{
				fourcount++;
				weight = i;
			}
		}
	}
	//判断牌型是否合法,生成类型值和权重值
	int getCla()
	{
		if (pokerN == 0)
			return ERROR;
		if (pokerN == 1)
		{
			cla = 1;
		}
		else if (pokerN > 5)
		{
			if (fourcount)
			{
				if (fourcount == 1 && twocount == 1 && pokerN == 6)//四带二
					cla = 42;
				else
					return ERROR;
			}
			else if (threecount)
			{
				if (threecount > 1)
				{
					for (int i = 0; i < 11; i++)
					{
						if (Pcount[i] == 3)
						{
							for (int j = i + 1; j < threecount + i; j++)
							{
								if (Pcount[j] != 3 || j > 11)
									return ERROR;
							}
							weight = i;
							break;
						}
					}
					if (!twocount && !onecount)
						cla = 33;
					else if (threecount == twocount && onecount == 0)
						cla = 35;
					else if ((threecount == onecount && twocount == 0) || (threecount = twocount * 2 && onecount == 0))
						cla = 34;
					else
						return ERROR;
				}
				else
					return ERROR;
			}
			else if (twocount)
			{
				if (twocount >= 3 && onecount == 0)
				{
					for (int i = 0; i < 10; i++)
					{
						if (Pcount[i] == 2)
						{
							for (int j = i + 1; j < twocount + i; j++)
							{
								if (Pcount[j] != 2 || j > 11)
									return ERROR;
							}
							cla = 6;
							weight = i;
							break;
						}
					}
				}
				else
					return ERROR;
			}
			else
			{
				for (int i = 0; i < 11; i++)
				{
					if (Pcount[i] == 1)
					{
						for (int j = i + 1; j < i + onecount; j++)
						{
							if (!Pcount[j] || j > 11)
								return ERROR;
						}
						weight = i;
						cla = 5;
						break;
					}
				}
			}
		}
		else
		{
			if (pokerN == 2)
			{
				if (twocount)
				{
					cla = 2;
				}
				else if (Pcount[13] && Pcount[14])
				{
					weight = 100;
					cla = 100;
				}
				else
					return ERROR;
			}
			else if (pokerN == 3)
			{
				if (threecount)
					cla = 3;
				else
					return ERROR;
			}
			else if (pokerN == 4)
			{
				if (threecount == 1)
				{
					cla = 31;
					for (int i = 0; i < 13; i++)
					{
						if (Pcount[i] == 3)
						{
							weight = i;
							break;
						}
					}
				}
				else if (fourcount == 1)
					cla = 4;
				else
					return ERROR;
			}
			else if (pokerN == 5)
			{
				if (threecount == 1 && twocount == 1)
				{
					cla = 32;
					for (int i = 0; i < 13; i++)
					{
						if (Pcount[i] == 3)
						{
							weight = i;
							break;
						}
					}
				}
				else if (fourcount == 1)
				{
					cla = 41;
					for (int i = 0; i < 13; i++)
					{
						if (Pcount[i] == 4)
						{
							weight = i;
							break;
						}
					}
				}
				else if (twocount == 0 && threecount == 0 && fourcount == 0)//是否顺子
				{
					for (int i = 0; i < 12; i++)
					{
						if (Pcount[i] == 1)
						{
							for (int j = i + 1; j < i + 5; j++)
							{
								if (Pcount[j] == 0)
									return ERROR;
							}
							weight = i;
							cla = 5;
							break;
						}
					}
				}
			}
			else
				return ERROR;
		}
		return cla;
	}
	//画出类里保存的牌型
	void drawPoker(float x, float y)
	{
		for (int i = 0; i < pokerN; i++)
		{
			spokers[Ppai[i]].setPosition(x, y);
			window.draw(spokers[Ppai[i]]);
			x += 25.0f;
		}
	}
	//控制台输出检测
	void Pcheck()
	{
		cout << pokerN << "张牌,类型为" << cla << ",权值为" << weight << endl;
	}
	//获得牌的类型
	int getc()
	{
		return cla;
	}
	//获得权重
	int getw()
	{
		return weight;
	}
	int getpokerN()
	{
		return pokerN;
	}
}; Poker poker; 

//初始发牌
void fapai()
{
	player[0].setname("player1");
	srand(time(NULL));
	do                          //生成底牌
	{
		for (int i = 0; i < 3; i++)
			dipai[i] = rand() % 54;
	} while (dipai[0] == dipai[1] || dipai[0] == dipai[2] || dipai[1] == dipai[2]);

	for (int i = 0; i < 54; i++)     //发牌
	{
		int a;
		if (i != dipai[0] && i != dipai[1] && i != dipai[2] && (player[0].canin == 1 || player[1].canin == 1 || player[2].canin == 1))
		{
			a = rand() % 3;
			if (player[a].canin == 1)
				player[a].in(i);
			else
				i--;
		}
	}
/*for (int i = 0; i < 3; i++)
	{
		player[i].checker();
	}
	for (int i = 0; i < 3; i++)
		cout << dec << dipai[i] << " ";*/
	window.clear();
	sbackground.setPosition(0.0f, 0.0f);
	window.draw(sbackground);
	for (int i = 0; i < 3; i++)
	{
		scard.setPosition(POSX + 100.0f + 130.0f*i, 10.0f);
		window.draw(scard);
	}
	sstart.setPosition(350.0f, 350.0f);
	window.draw(sstart);
	player[0].Drawcard(POSX, POSY);
	player[1].DrawComputer(POSX-130.0f, 10.0f);
	player[2].DrawComputer(POSX+600.0f, 10.0f);
}

//动态绘制
void logic()
{
	window.clear();
	sbackground.setPosition(0.0f, 0.0f);
	sbuchu.setPosition(300.0f, 350.0f);
	schupai.setPosition(420.0f, 350.0f);
	window.draw(sbackground);
	if (playernumber==0&&!isOver)
	{
		window.draw(schupai);
		if (player[2].bc == 1)
		{
			sbc.setPosition(POSX, POSY - 220.0f);
			window.draw(sbc);
		}
		if (turn==2)
		{
			poker.drawPoker(POSX , POSY - 220.0f);
			window.draw(sbuchu);
		}
		else if (turn == 1)
		{
			poker.drawPoker(POSX + 25.0f*(19-poker.getpokerN()), POSY - 220.0f);
			window.draw(sbuchu);
		}
		else if (turn == 0)
		{
			poker.drawPoker(POSX + 100.0f, POSY - 210.0f);
		}
	}
	else
	{
		if (turn == 2)
		{
			poker.drawPoker(POSX, POSY - 220.0f);
		}
		else if (turn == 1)
		{
			poker.drawPoker(POSX + 25.0f*(19 - poker.getpokerN()), POSY - 220.0f);
		}
		else if (turn == 0)
		{
			poker.drawPoker(POSX + 100.0f, POSY - 210.0f);
		}
	}
	for (int i = 0; i < 3; i++)
	{
		spokers[dipai[i]].setPosition(POSX + 100.0f + 130.0f*i, 10.0f);
		window.draw(spokers[dipai[i]]);
	}
	if (playernumber != 1&&player[1].bc == 1)
	{
		sbc.setPosition(POSX + 500.0f, POSY - 220.0f);
		window.draw(sbc);
	}
	else if (playernumber!=0&&player[0].bc == 1)
	{
		sbc.setPosition(POSX+150.0f, POSY - 50.0f);
		window.draw(sbc);
	}
	player[0].Drawcard(POSX, POSY);
	player[2].DrawComputer(POSX - 130.0f, 10.0f);
	player[1].DrawComputer(POSX + 600.0f, 10.0f);
	window.display();
}

//电脑有限制情况下出牌，参数为电脑号码
int computer(int number)
{
	int aa = 0;
	int i;
	bool c = 0;
	if (poker.cla == 1)
	{
		for (i = poker.weight + 1; i < 12; i++)
		{
			if (player[number].count[i] == 1)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(1, i, 1, aa, number);
				player[number].setChoose(aa);
				poker = cpoker;
				cpoker.~Poker();
				return 1;
			}
		}
		for (i = poker.weight + 1; i < 15; i++)
		{
			if (player[number].count[i])
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(1, i, 1, aa, number);
				player[number].setChoose(aa);
				poker = cpoker;
				return 1;
			}
		}

	}
	else if (poker.cla == 2)
	{
		if (poker.weight < 13)
		{
			for (i = poker.weight + 1; i < 11; i++)
			{
				if (player[number].count[i] == 2)
				{
					for (int j = 0; j < i; j++)
					{
						aa += player[number].count[j];
					}
					Poker cpoker(2, i, 2, aa, number);
					player[number].setChoose(aa);
					player[number].setChoose(aa + 1);
					poker = cpoker;
					cpoker.~Poker();
					return 1;
				}
			}
			for (i = poker.weight + 1; i < 13; i++)
			{
				if (player[number].count[i]>1)
				{
					for (int j = 0; j < i; j++)
					{
						aa += player[number].count[j];
					}
					Poker cpoker(2, i, 2, aa, number);
					player[number].setChoose(aa);
					player[number].setChoose(aa + 1);
					poker = cpoker;
					cpoker.~Poker();
					return 1;
				}
			}		
		}
	}
	else if (poker.cla == 3)
	{
		for (i = poker.weight + 1; i < 13; i++)
		{
			if (player[number].count[i] == 3)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(3, i, 3, aa, number);
				player[number].setChoose(aa);
				player[number].setChoose(aa + 1);
				player[number].setChoose(aa + 2);
				poker = cpoker;
				cpoker.~Poker();
				return 1;
			}
		}
	}
	else if (poker.cla == 4)
	{
		for (int i = poker.weight+1; i < 13; i++)
		{
			if (player[number].count[i] == 4)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(4, i, 4, aa, number);
				for (int x = 0; x < 4; x++)
				{
					player[number].setChoose(aa + x);
				}
				poker = cpoker;
				return 1;
			}
		}
		if (player[number].count[13] == 1 && player[number].count[14] == 1)
		{
			Poker cpoker(2, 100, 100, player[number].n - 2, number);
			player[number].setChoose(player[number].n - 1);
			player[number].setChoose(player[number].n - 2);
			poker = cpoker;
			return 1;
		}
		return 0;
	}
	else if (poker.cla == 5 && (poker.pokerN + poker.weight) < 12)
	{
		for (i = poker.weight + 1; i < (13-poker.pokerN); i++)
		{
			if (player[number].count[i] < 3&& player[number].count[i]>0)
			{
				for (int j =  1; j < poker.pokerN; j++)
				{
					if (player[number].count[i+j] == 0 || player[number].count[i+j] > 3)
					{
						c = 1;
						break;
					}
				}
				if (c == 0)
				{
					for (int j = 0; j < i; j++)
					{
						aa += player[number].count[j];
					}
					Poker cpoker(poker.pokerN, i, 5, aa, number);
					for (int j = 0; j < poker.pokerN; j++)
					{
						player[number].setChoose(aa);
						aa += player[number].count[i + j];
					}
					poker = cpoker;
					return 1;
				}
			}
		}

	}
	else if (poker.cla == 31)
	{
		for (i = poker.weight + 1; i < 11; i++)
		{
			if (player[number].count[i] == 3)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(3, i, 31, aa, number);
				player[number].setChoose(aa);
				player[number].setChoose(aa + 1);
				player[number].setChoose(aa + 2);
				aa = 0;
				for (i = 0; i < 12; i++)
				{
					if (player[number].count[i] == 1)
					{
						for (int j = 0; j < i; j++)
						{
							aa += player[number].count[j];
						}
						cpoker.Ppai[3] = player[number].pai[aa];
						cpoker.pokerN = 4;
						player[number].setChoose(aa);
						poker = cpoker;
						cpoker.~Poker();
						return 1;
					}
				}
				return 0;

			}
		}
		for (i = poker.weight + 1; i < 13; i++)
		{
			if (player[number].count[i] >= 3)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(3, i, 31, aa, number);
				player[number].setChoose(aa);
				player[number].setChoose(aa + 1);
				player[number].setChoose(aa + 2);
				aa = 0;
				for (i = 0; i < 12; i++)
				{
					if (player[number].count[i] == 1)
					{
						for (int j = 0; j < i; j++)
						{
							aa += player[number].count[j];
						}
						cpoker.Ppai[3] = player[number].pai[aa];
						cpoker.pokerN = 4;
						player[number].setChoose(aa);
						poker = cpoker;
						cpoker.~Poker();
						return 1;
					}
				}
				return 0;

			}
		}

	}
	else if (poker.cla == 32)
	{
	for (i = poker.weight + 1; i < 11; i++)
	{
		if (player[number].count[i] == 3)
		{
			for (int j = 0; j < i; j++)
			{
				aa += player[number].count[j];
			}
			Poker cpoker(3, i, 32, aa, number);
			player[number].setChoose(aa);
			player[number].setChoose(aa + 1);
			player[number].setChoose(aa + 2);
			aa = 0;
			for (i = 0; i < 12; i++)
			{
				if (player[number].count[i] == 2)
				{
					for (int j = 0; j < i; j++)
					{
						aa += player[number].count[j];
					}
					cpoker.Ppai[3] = player[number].pai[aa];
					cpoker.Ppai[4] = player[number].pai[aa+1];
					cpoker.pokerN = 5;
					player[number].setChoose(aa);
					player[number].setChoose(aa+1);
					poker = cpoker;
					cpoker.~Poker();
					return 1;
				}
			}
			return 0;
		}
	}
	for (i = poker.weight + 1; i < 13; i++)
	{
		if (player[number].count[i] == 3)
		{
			for (int j = 0; j < i; j++)
			{
				aa += player[number].count[j];
			}
			Poker cpoker(3, i, 32, aa, number);
			player[number].setChoose(aa);
			player[number].setChoose(aa + 1);
			player[number].setChoose(aa + 2);
			aa = 0;
			for (i = 0; i < 12; i++)
			{
				if (player[number].count[i] == 2)
				{
					for (int j = 0; j < i; j++)
					{
						aa += player[number].count[j];
					}
					cpoker.Ppai[3] = player[number].pai[aa];
					cpoker.Ppai[4] = player[number].pai[aa + 1];
					cpoker.pokerN = 5;
					player[number].setChoose(aa);
					player[number].setChoose(aa + 1);
					poker = cpoker;
					cpoker.~Poker();
					return 1;
				}
			}
			return 0;
		}
	}
	}
	if (player[0].n < 5&&turn==0&&poker.cla!=100)
	{
	for (int i = 0; i < 13; i++)
	{
		if (player[number].count[i] == 4)
		{
			for (int j = 0; j < i; j++)
			{
				aa += player[number].count[j];
			}
			Poker cpoker(4, i, 4, aa, number);
			for (int x = 0; x < 4; x++)
			{
				player[number].setChoose(aa + x);
			}
			poker = cpoker;
			return 1;
		}
	}
	if (player[number].count[13] == 1 && player[number].count[14] == 1)
	{
		Poker cpoker(2, 100, 100, player[number].n-2, number);
		player[number].setChoose(player[number].n - 1);
		player[number].setChoose(player[number].n - 2);
		poker = cpoker;
		return 1;
	}
	return 0;
 }
	else
	return 0;
}
//电脑主动出牌
void computer2(int number)
{
	int a;
	int aa = 0;
	if (player[0].getn() == 1)
	{
		for (int i = 0; i < 13; i++)
		{
			if ((a= player[number].count[i])>1)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(a, i, a, aa, number);
				for (int x = 0; x < a; x++)
				{
					player[number].setChoose(aa + x);
				}
				poker = cpoker;
				return;
			}
		}
		for (int i = 14; i >=0; i--)
		{
			if (player[number].count[i] == 1)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(1, i,1, aa, number);
				player[number].setChoose(aa);
				poker = cpoker;
				return;
			}
		}
	}
	else if (player[0].getn() == 2)
	{
		for (int i =0; i <15; i++)
		{
			if ((a=player[number].count[i]) == 1||a==3)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(a, i, a, aa, number);
				for (int x = 0; x < a; x++)
				{
					player[number].setChoose(aa + x);
				}
				poker = cpoker;
				return;
			}

		}
		for (int i = 0; i <15; i++)
		{
			if (player[number].count[i] == 2)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(1, i, 1, aa, number);
				player[number].setChoose(aa);
				poker = cpoker;
				return;
			}
		}
		for (int i = 0; i < 15; i++)
		{
			if (player[number].count[i] == 4)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(4, i, 4, aa, number);
				for (int x = 0; x < 4; x++)
				{
					player[number].setChoose(aa+x);
				}
				poker = cpoker;
				return;
			}
		}
	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			if ((a = player[number].count[i]) != 0 && a < 4)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(a, i, a, aa, number);
				for (int x = 0; x < a; x++)
				{
					player[number].setChoose(aa + x);
				}
				poker = cpoker;
				return;
			}
		}
		for (int i = 0; i < 15; i++)
		{
			if (player[number].count[i] == 4)
			{
				for (int j = 0; j < i; j++)
				{
					aa += player[number].count[j];
				}
				Poker cpoker(4, i, 4, aa, number);
				for (int x = 0; x < 4; x++)
				{
					player[number].setChoose(aa + x);
				}
				poker = cpoker;
				return;
			}
		}
	}
}
//电脑出牌总控
void control()
{
	Sleep(1000);
	if (playernumber == 1)
		{
			if (turn == 1)
			{
				computer2(1);
				player[1].out();
				player[1].bc = 0;
				//cout << "电脑1出牌" << endl;
				if (!player[1].getn())
				{
					isOver = 1;
				}
				else
				{
					playernumber = 2;
				}
				logic();
			}
			else if (turn == 0)
			{
				if (computer(1))
				{
					player[1].out();
					player[1].bc = 0;
					//cout << "电脑1出牌" << endl;
					turn = 1;
					if (!player[1].getn())
					{
						isOver = 1;
					}
					else
					{
						playernumber = 2;
					}
					logic();
				}
				else
				{
					playernumber = 2;
					player[1].bc = 1;
					//cout << "电脑1不出" << endl;
				}
			}
			else
			{
				playernumber = 2;
				player[1].bc = 1;
				//cout << "电脑1不出" << endl;
			}
		}
	Sleep(1000);
	if (playernumber == 2)
	{
			if (turn == 2)
			{
				computer2(2);
				player[2].out();
				//cout << "电脑2出牌" << endl;
				if (!player[2].getn())
				{
					isOver = 1;
				}
				else
				{
					playernumber = 0;
				}
				logic();
			}
			else if (!(turn==1&&(poker.getw()>10||poker.getc()==4))&&computer(2))
			{
				player[2].out();
				player[2].bc = 0;
				turn = 2;
				//cout << "电脑2出牌" << endl;
				if (!player[2].getn())
				{
					isOver = 1;
				}
				else
				{
					playernumber = 0;
				}
				logic();
			}
			else
			{
				playernumber = 0;
				player[2].bc = 1;
				//cout << "电脑2不出" << endl;
				logic();
			}
			//Sleep(1000);
		}
}

//获取鼠标输入
void input()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::EventType::Closed)
		{
			window.close();
		}
	}

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		//cout << "left pressed" << endl;
		Vector2i position = sf::Mouse::getPosition();
		Vector2i positionw = window.getPosition();

		if (!isStart)
		{
			if (380.0f + positionw.y <= position.y &&410.0f + positionw.y >= position.y&&350.0f + positionw.x <= position.x&&440.0f + positionw.x >= position.x)
			{
				//cout << "游戏开始" << endl;
				for (int i = 0; i < 3; i++)
				{
					player[0].start(dipai[i]);
				}
				isStart = 1;
				logic();
			}
		}
		else
		{
			if (playernumber==0&&!isOver)
			{
				int a = player[0].getn();
				//点击位置在手牌区域
				if (POSY + 30.0f + positionw.y <= position.y &&POSY + positionw.y + 180.0f >= position.y)
				{
					int p = (position.x - positionw.x - POSX) / 25.0f;
					if (p >= 0 && p < a)
					{
						//cout << "第" << p + 1 << "张牌被点击了" << endl;
						player[0].setChoose(p);
						logic();
					}
					else if (p >= a && p < a + 4)
					{
						//cout << "第" << a << "张牌被点击了" << endl;
						player[0].setChoose(a - 1);
						logic();
					}
				}
				//点击了出牌
				if (380.0f + positionw.y <= position.y &&410.0f + positionw.y >= position.y&&420.0f + positionw.x <= position.x&&510.0f + positionw.x >= position.x)
				{
					//cout << "判断出牌" << endl;
					Poker bpoker;
					bpoker.PokerChecker();
					int c = bpoker.getCla();
					//bpoker.Pcheck();
					//cout <<"牌型为"<< c << endl;
					if (c)
					{
						if (turn == 0 || c == 100 || (c == 4 && poker.getc() != 100 && poker.getc() != 4))
						{
							poker = bpoker;
							bpoker.~Poker();
							//cout << "出牌" << endl;
							turn = 0;
							player[0].out();
							player[0].bc = 0;
							if (!player[0].getn())
							{
								isOver = 1;
								logic();
							}
							else
							{
								playernumber = 1;
								logic();
								control();
							}
						}
						else if (bpoker.getc() == poker.getc() && bpoker.getw() > poker.getw())
						{
							poker = bpoker;
							bpoker.~Poker();
							//cout << "出牌" << endl;
							turn = 0;
							player[0].out();
							player[0].bc = 0;
							if (!player[0].getn())
							{
								isOver = 1;
								logic();
							}
							else
							{
								playernumber = 1;
								logic();
								control();
							}
						}
					}
				}
				//点击了不出
				if (380.0f + positionw.y <= position.y &&410.0f + positionw.y >= position.y&&300.0f + positionw.x <= position.x&&390.0f + positionw.x >= position.x&&turn!=0)
				{
					//cout << "不出" << endl;
					playernumber = 1;
					player[0].bc = 1;
					logic();
					control();
				}
			}
		}		
	}
}

int main()
{
	Initial();
	fapai();
	window.display();
	while (window.isOpen())
	{
		input();
	}
	//system("pause");
	return 0;
}