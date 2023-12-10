#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <string>
#include <array>
#define STRING_SIZE 255	
#define MAX_PROCESS	24				//����ȡQQ����24
#define PUBLIC_BUFFER_SIZE 270		//�����ڴ滺������С
#define BUF_SIZE 4096               //QQ�ṹ����󳤶�
struct QQinfo
{
	std::string pid;
	std::string port;
	std::string pt_locak_token;
	std::string account;
	std::string sex;
	std::string name;
	std::string qqkey;
	std::string skey;
	std::string p_skey;
};

class QQKEY
{
public:
	std::string exec(const char* cmd);//�ܵ�����
	BOOL IsProcessNameMatch(const std::string& processName, const std::string& matchName);//�Ƿ�ΪQQ����
	std::string ProcessIdToName(DWORD processId);//PIDȡ·��    
	BOOL AdjustPrivilege();	//��Ȩ

private:
	//������
	TCHAR pName[STRING_SIZE];		//Ŀ�������
	DWORD target_pid[MAX_PROCESS];	//Ŀ�����pid
};