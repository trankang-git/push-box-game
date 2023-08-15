#include <conio.h>
#include <graphics.h>
#include <iostream>
#include "database.h"

using namespace std;

class pushboxgame
{
public:
	void user_register(void){
		cout << "请输入用户名：";
		cin >> user.username;
		cout << "请输入密码：";
		cin >> user.password;

		if (!add_user_info(user)) {
			cout << "注册失败，请重新进入" << endl;
			system("pause");
			exit(-1);
		}

		fetch_user_info(user);

		cout << "注册成功，按任意键开始游戏" << endl;
		system("pause");
	}

	void user_logon(void){
		bool user_empty=false;
		int logon_num = 4;

		while(!user_empty&& (logon_num--)){
			cout << "请输入用户名：";
			cin >> user.username;
			cout << "请输入密码：";
			cin >> user.password;

			user_empty = fetch_user_info(user);

			if (logon_num != 0 && !user_empty) {
				cout << "用户名或密码错误，请重新输入" << endl;
			}
		}

		if(user_empty){
			cout << "登陆成功，按任意键开始游戏" << endl;
			system("pause");

			fetch_level_info(level, user.level_id);

			if (level.next_level_id < 1) {
				int choice;
				cout << "你已经通关了全部关卡，是否重置?" << endl << "1.重置" << endl << "2.不重置" << endl;
				cin >> choice;

				if (choice == 1) {
					if (!update_usersinfo(user, 1)) {
						cout << "用户信息更新出错，请按任意键退出" << endl;
						system("pause");
						exit(-1);
					}
					cout << "重置成功，按任意键继续" << endl;
					system("pause");
				}
			}
		}

		else {
			cout << "错误次数过多，请重新进入" << endl;
			system("pause");
			exit(-1);
		}
	}
	void IMAGE_get(void)
	{
		loadimage(&backgrand, L"./bmp/blackground.bmp", backgrand_width, backgrand_height, true);

		loadimage(&images[wall], L"./bmp/wall.bmp", bmp_width, bmp_height, true);

		loadimage(&images[floor], L"./bmp/floor.bmp", bmp_width, bmp_height, true);

		loadimage(&images[des], L"./bmp/des.bmp", bmp_width, bmp_height, true);

		loadimage(&images[man], L"./bmp/man.bmp", bmp_width, bmp_height, true);

		loadimage(&images[box], L"./bmp/box.bmp", bmp_width, bmp_height, true);
	}

	void show_map(void)
	{
		if (!fetch_level_info(level, user.level_id)) {
			cout << "地图加载失败，请重新进入" << endl;
			system("pause");
			exit(-1);
		}

		cout << "关卡id：" << level.id << "  关卡名字：" << level.name << endl;
		cout << "地图加载成功，按任意键开始游戏" << endl;
		system("pause");

		int map_line = level.map_data.size();
		int map_column = level.map_data[0].size();

		initgraph(backgrand_width, backgrand_height);

		putimage(0, 0, &backgrand);

		for (int i = 0; i < map_line; i++)
		{
			for (int j = 0; j < map_column; j++)
			{
				if (level.map_data[i][j] == man)
				{
					man_pos.left = j;
					man_pos.top = i;
				}
				putimage(start_left+j* bmp_width, start_top+i* bmp_height, &images[level.map_data[i][j]]);
			}
		}
	}

	void key_control(int Direction)
	{
		POS next_pos;
		POS next_next_pos;

		next_pos.left = man_pos.left + direction_vector[Direction][1];
		next_pos.top = man_pos.top + direction_vector[Direction][0];

		next_next_pos.left= next_pos.left+ direction_vector[Direction][1];
		next_next_pos.top = next_pos.top + direction_vector[Direction][0];

		if(next_pos.top< level.map_data.size()&& next_pos.left< level.map_data[0].size())
		{
			if (level.map_data[next_pos.top][next_pos.left] == floor)
			{
				if (level.map_data[man_pos.top][man_pos.left] == des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
				else level.map_data[man_pos.top][man_pos.left] = floor;

				level.map_data[next_pos.top][next_pos.left] = man;

				putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
				putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man]);

				man_pos = next_pos;
			}

			else if (level.map_data[next_pos.top][next_pos.left] == des)
			{
				if(level.map_data[man_pos.top][man_pos.left]== des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
				else level.map_data[man_pos.top][man_pos.left] = floor;

				level.map_data[next_pos.top][next_pos.left] = des_and_man;

				putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
				putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man], SRCINVERT);

				man_pos = next_pos;
			}


			else if (level.map_data[next_pos.top][next_pos.left] == box)
			{
				if (next_next_pos.top < level.map_data.size() && next_next_pos.left < level.map_data[0].size())
				{
					if (level.map_data[next_next_pos.top][next_next_pos.left] == floor)
					{
						if (level.map_data[man_pos.top][man_pos.left] == des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
						else level.map_data[man_pos.top][man_pos.left] = floor;

						level.map_data[next_pos.top][next_pos.left] = man;
						level.map_data[next_next_pos.top][next_next_pos.left] = box;

						putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
						putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man]);
						putimage(start_left + next_next_pos.left * bmp_width, start_top + next_next_pos.top * bmp_height, &images[box]);

						man_pos = next_pos;
					}
					else if (level.map_data[next_next_pos.top][next_next_pos.left] == des)
					{
						if (level.map_data[man_pos.top][man_pos.left] == des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
						else level.map_data[man_pos.top][man_pos.left] = floor;

						level.map_data[next_pos.top][next_pos.left] = man;
						level.map_data[next_next_pos.top][next_next_pos.left] = hit;

						putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
						putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man]);
						putimage(start_left + next_next_pos.left * bmp_width, start_top + next_next_pos.top * bmp_height, &images[box]);

						man_pos = next_pos;
					}
				}
			}

			else if (level.map_data[next_pos.top][next_pos.left] == hit)
			{
				if (next_next_pos.top < level.map_data.size() && next_next_pos.left < level.map_data[0].size())
				{
					if (level.map_data[next_next_pos.top][next_next_pos.left] == floor)
					{
						if (level.map_data[man_pos.top][man_pos.left] == des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
						else level.map_data[man_pos.top][man_pos.left] = floor;

						level.map_data[next_pos.top][next_pos.left] = des_and_man;

						level.map_data[next_next_pos.top][next_next_pos.left] = box;

						putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
						putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[des]);
						putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man], SRCINVERT);
						putimage(start_left + next_next_pos.left * bmp_width, start_top + next_next_pos.top * bmp_height, &images[box]);

						man_pos = next_pos;
					}
					else if (level.map_data[next_next_pos.top][next_next_pos.left] == des)
					{
						if (level.map_data[man_pos.top][man_pos.left] == des_and_man)	level.map_data[man_pos.top][man_pos.left] = des;
						else level.map_data[man_pos.top][man_pos.left] = floor;

						level.map_data[next_pos.top][next_pos.left] = des_and_man;

						level.map_data[next_next_pos.top][next_next_pos.left] = hit;

						putimage(start_left + man_pos.left * bmp_width, start_top + man_pos.top * bmp_height, &images[level.map_data[man_pos.top][man_pos.left]]);
						putimage(start_left + next_pos.left * bmp_width, start_top + next_pos.top * bmp_height, &images[man], SRCINVERT);
						putimage(start_left + next_next_pos.left * bmp_width, start_top + next_next_pos.top * bmp_height, &images[box]);

						man_pos = next_pos;
					}
				}
			}
		}

	}

	bool is_success(void)
	{
		for (int i = 0; i < level.map_data.size(); i++)
		{
			for (int j = 0; j < level.map_data[0].size(); j++)
			{
				if (level.map_data[i][j] == des|| level.map_data[i][j]==des_and_man)return false;
			}
		}
		return true;
	}

	void game_over(int game_over_type)
	{
		settextcolor(WHITE);
		RECT rec = { 0,0, backgrand_width ,backgrand_height };
		settextstyle(20, 0, _T("宋体"));
		if (game_over_type == success_once)drawtext(_T("恭喜通关，按任意键继续下一关！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else if(game_over_type==success_all)drawtext(_T("达成全通关，按任意键退出！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		else if(game_over_type == quit)drawtext(_T("按任意键退出！"), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	void game_control(void)
	{
		char key;
		while (1)
		{
			if (_kbhit())
			{
				key = _getch();
				if (key == 'w')
				{
					key_control(UP);
				}
				else if (key == 'a')
				{
					key_control(LEFT);
				}
				else if (key == 's')
				{
					key_control(DOWN);
				}
				else if (key == 'd')
				{
					key_control(RIGHT);
				}
				else if (key == 'q')
				{
					game_over(false);
					system("pause");
					exit(0);
				}
				if (is_success())
				{
					if (level.next_level_id < 1) {
						game_over(success_all);
						system("pause");
						exit(0);
					}

					game_over(success_once);
					closegraph();
					if (!update_usersinfo(user, level.next_level_id)) {
						cout << "用户信息更新出错，请按任意键退出" << endl;
						system("pause");
						exit(-1);
					}
					break;
				}
			}
			Sleep(100);
		}
	}

	void pushboxgame_main()
	{
		int choice;
		cout << "请选择：" << endl << "1.注册" << endl << "2.登录" << endl;
		cin >> choice;

		if (choice == 1)user_register();
		if (choice == 2)user_logon();

		IMAGE_get();

		while (1) {
			//cleardevice();
			show_map();
			game_control();
		}
	}

	s_userinfo user;
	s_levelinfo level;

	int backgrand_width = 800;	//整个背景的宽度
	int backgrand_height = 650;	//整个背景的高度

	int bmp_width = 50;	//一张图片的宽度
	int bmp_height = 50;	//一张图片的宽度

	int start_left=100;
	int start_top=100;

	enum thing_type
	{
		wall,
		floor,
		des,
		man,
		box,
		thing_type_sum,
		hit,
		des_and_man
	};

	IMAGE backgrand;
	IMAGE images[thing_type_sum];

	//墙：0，地板：1，箱子目的地：2，人：3，箱子：4，箱子命中目标：5
	//vector<vector<int>> map;
		/*= {
		{0,0,0,0,1,0,0,0,0,0,0,0},
		{0,1,0,1,1,1,1,1,1,1,0,0},
		{0,1,4,1,1,2,1,0,2,1,0,0},
		{0,1,0,1,1,1,0,0,1,1,1,0},
		{0,1,0,2,1,1,1,4,1,1,1,0},
		{0,1,1,1,1,3,1,1,1,4,1,0},
		{0,1,2,1,1,4,1,1,1,1,1,0},
		{0,1,0,0,1,0,1,1,0,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0} 
	};*/
	//0,0,0,0,1,0,0,0,0,0,0,0|0,1,1,1,1,1,1,1,1,1,1,0|0,1,4,1,1,2,1,1,2,1,1,0|0,1,1,1,1,1,1,1,1,1,1,0|0,1,1,2,1,1,1,4,1,1,1,0|0,1,1,1,1,3,1,1,1,4,1,0|0,1,2,1,1,4,1,1,1,1,1,0|0,1,1,1,1,1,1,1,1,1,1,0|0,0,0,0,0,0,0,0,0,0,0,0
	struct POS
	{
		int left;
		int top;
	};

	POS man_pos;

	vector<vector<int>> direction_vector = { {-1,0},{0,-1},{1,0},{0,1} };

	enum direction_type
	{
		UP,
		LEFT,
		DOWN,
		RIGHT
	};

	enum game_over_type {
		quit,
		success_once,
		success_all
	};
};




int main()
{
	pushboxgame test;
	test.pushboxgame_main();

	system("pause");
	return 0;
}