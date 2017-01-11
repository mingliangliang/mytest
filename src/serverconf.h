#pragma once
#include <map>
#include <string>
#include <set>
#include <string.h>
#include <vector>

using namespace std;
class TiXmlElement;

struct Info
{
    string m_ip;
    int m_port;
    Info()
    {
        m_ip = "127.0.0.1";
    }
};

struct ParaJson
{
    string m_Json;
    int m_iTest;
};

struct Test
{
	int m_iEnable;
	int m_iCmd;
	string m_User;
	vector<ParaJson> m_vecJson;
};


class ServerConf
{
public:
	static ServerConf* Instance()
	{
		static ServerConf sconf;
		return &sconf;
	}
	int Read(const char* filename);
private:
	ServerConf(){}
	~ServerConf();
	bool ReadInfoCfg(TiXmlElement *root);
	int ReadAllTestCfg(TiXmlElement *root);
	int ReadJsonNode(TiXmlElement *node , Test &stat);
public:
	string m_cfgfilename;
	Info m_info;
	map<int, Test> m_allTest;
};

