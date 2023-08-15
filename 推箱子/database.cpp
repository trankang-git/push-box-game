#include "database.h"
#include <mysql.h>
#include <iostream>

#define db_host "127.0.0.1"
#define db_user "root"
#define db_password "123456"
#define db_name "box_man"
#define db_port 3306

 bool connect_db(MYSQL &mysql){
	//1.初始化数据库句柄
	mysql_init(&mysql);

	//2.设置字符编码
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");

	 //3.连接数据库
	if (mysql_real_connect(&mysql, db_host, db_user, db_password, db_name, db_port, NULL, 0) == NULL) {
		cout << "数据库连接出错，错误原因：" << mysql_error(&mysql)<<endl;
		return false;
	}
	return true;
}


 bool add_user_info(s_userinfo& user) {
	 MYSQL mysql;
	 string sql;
	 int ret;

	 if (!connect_db(mysql)) {
		 mysql_close(&mysql);
		 return false;
	 }

	 sql = "insert into users value('0', '" + user.username+"', md5('" + user.password+"'), 1);";

	 ret = mysql_query(&mysql, sql.c_str());

	 if (ret) {
		 cout << "数据库添加数据失败，失败原因：" << mysql_error(&mysql)<<endl;
		 mysql_close(&mysql);
		 return false;
	 }

	 return true;
 }

 bool fetch_user_info(s_userinfo& user){
	MYSQL mysql;
	string sql;
	bool ret = false;
	MYSQL_RES* res;
	MYSQL_ROW row;

	if (connect_db(mysql) == false) {
		return false;
	}

	sql = "select id, level_id from users where username='" + user.username + "'&&password=md5('" + user.password+"');";
	ret = mysql_query(&mysql, sql.c_str());
	
	if (ret) {
		cout << "数据库查询出错，错误原因：" << mysql_error(&mysql);
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	user.id = atoi(row[0]);
	user.level_id= atoi(row[1]);

	cout << "id:" << row[0] << "	level_id:" << row[1] << endl;

	mysql_free_result(res);
	mysql_close(&mysql);
	return true;
}

bool fetch_level_info(s_levelinfo &level,int level_id) {
	MYSQL mysql;
	string sql;
	bool ret = false;
	MYSQL_RES* res;
	MYSQL_ROW row;
	string str_map_data;

	if (connect_db(mysql) == false) {
		return false;
	}

	sql = "select name,map_row,map_column,map_data,next_level_id from levels where id='" + to_string(level_id) + "';";
	ret = mysql_query(&mysql, sql.c_str());

	if (ret) {
		cout << "数据库查询失败，失败原因：" << mysql_error(&mysql)<<endl;
		mysql_close(&mysql);
		return false;
	}

	res = mysql_store_result(&mysql);
	row = mysql_fetch_row(res);

	if (row == NULL) {
		mysql_free_result(res);
		mysql_close(&mysql);
		return false;
	}

	level.id = level_id;
	level.name = row[0];
	level.map_row = atoi(row[1]);
	level.map_column = atoi(row[2]);
	str_map_data = row[3];
	level.next_level_id = atoi(row[4]);

	if (str_map_data.size() < 1) {
		cout << "地图数据有误，请更改地图" << endl;
		return false;
	}

	int start = 0;
	int end = 0;
	string str_line;
	level.map_data.clear();
	while (1) {
		end = str_map_data.find('|', start);

		if (end == -1)end = str_map_data.size();

		if (start > end) break;

		str_line = str_map_data.substr(start, end - start);

		char* token = NULL;
		char* once = strtok_s((char*)str_line.c_str(), ",", &token);
		vector<int> int_line;

		while (once != NULL) {
			int_line.push_back(atoi(once));
			once = strtok_s(NULL, ",", &token);
		}

		if (int_line.size() != level.map_column) {
			cout << "地图列数出现错误，应为：" << level.map_column << "  实际为：" << int_line.size() << endl;
			return false;
		}

		level.map_data.push_back(int_line);

		start = end + 1;
	}

	if (level.map_data.size() != level.map_row) {
		cout << "地图行数出现错误，应为：" << level.map_row << "  实际为：" << level.map_data.size() << endl;
		return false;
	}

	return true;
}

bool update_usersinfo(s_userinfo &user,int next_level_id){
	MYSQL mysql;
	string sql;
	int ret;

	if (!connect_db(mysql)) {
		mysql_close(&mysql);
		return false;
	}
	
	sql = "update users set level_id = '"+to_string(next_level_id)+"' where id = '"+ to_string(user.id)+"';";
	ret = mysql_query(&mysql, sql.c_str());

	if (ret) {
		cout << "数据库修改数据出错，错误原因：" << mysql_error << endl;
		mysql_close(&mysql);
		return false;
	}

	user.level_id = next_level_id;
	mysql_close(&mysql);
	return true;
}