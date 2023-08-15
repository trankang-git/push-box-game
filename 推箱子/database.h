#pragma once
#include <string>
#include <vector>

using namespace std;

struct s_userinfo
{
	int id;
	string username;
	string password;
	int level_id;

};

struct s_levelinfo
{
	int id;
	string name;
	int map_row;
	int map_column;
	vector<vector<int>> map_data;
	int next_level_id;
};

bool add_user_info(s_userinfo& user);
bool fetch_user_info(s_userinfo& user);
bool fetch_level_info(s_levelinfo& level, int level_id);
bool update_usersinfo(s_userinfo& user, int next_level_id);