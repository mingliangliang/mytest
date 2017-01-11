#include "serverconf.h"
#include "yt/xml/tinyxml.h"

//////////////////////////////////////////////////////////////////
//ServerConf
//////////////////////////////////////////////////////////////////
ServerConf::~ServerConf()
{
}

bool ServerConf::ReadInfoCfg(TiXmlElement *root)
{
    TiXmlElement *mysqlcfg = root->FirstChildElement("info");
	if (!mysqlcfg)
	{
	    cout<<"info not exists!"<<endl;
	    return -1;
	}

	const char* str = mysqlcfg->Attribute("ip");
	if(!str)
	{
		cout<<"read ip error"<<endl;
	}
	else
	{
        m_info.m_ip = str;
	}
	cout<<"ip = "<<m_info.m_ip.c_str()<<endl;

	str = mysqlcfg->Attribute("port",&m_info.m_port);
	if(!str)
	{
		cout<<"read port error"<<endl;
		return false;
	}
	cout<<"port = "<<m_info.m_port<<endl;

	return true;
}

int ServerConf::ReadJsonNode(TiXmlElement *node , Test &test)
{
    TiXmlElement *http = node->FirstChildElement("json");
    if (!http)
	{
	    cout<<"json not exists!"<<endl;
	    return 0;
	}
    while(http)
	{
	    ParaJson json;
        const char *str = http->Attribute("parajson");
        if(!str)
        {
            cout<<"READ parajson failed."<<endl;
            return -1;
        }
        json.m_Json = str;
        //cout<<"parajson = "<<json.m_Json.c_str()<<endl;

        str = http->Attribute("test",&json.m_iTest);
        if(!str)
        {
            cout<<"READ test failed."<<endl;
            return -1;
        }
        //cout<<"test = "<<json.m_iTest<<endl;

        test.m_vecJson.push_back(json);
        http = http->NextSiblingElement("json");
	}
	return 0;
}

int ServerConf::ReadAllTestCfg(TiXmlElement *root)
{
	TiXmlElement *sqlinfo = root->FirstChildElement("info");
	if (!sqlinfo)
	{
	    cout<<"info not exists!"<<endl;
	    return -1;
	}

	TiXmlElement *subsqlinfo = sqlinfo->FirstChildElement("subinfo");
	while(subsqlinfo)
	{
		Test test;

        const char *str = subsqlinfo->Attribute("cmd" , &test.m_iCmd);
		if(!str)
        {
            cout<<"read cmd error"<<endl;
            return -1;
        }
        else
        {
            cout<<"cmd = "<<test.m_iCmd<<endl;
        }

		str = subsqlinfo->Attribute("enable",&test.m_iEnable);
        str = subsqlinfo->Attribute("user");
        if(!str)
            cout<<"read user error"<<endl;
        else
            test.m_User = str;

        if(-1 == ReadJsonNode(subsqlinfo , test))
        {
            cout<<"ReadJsonNode failed. cmd: "<<test.m_iCmd<<endl;
            return -1;
        }

        m_allTest[test.m_iCmd] = test;

		subsqlinfo = subsqlinfo->NextSiblingElement("subinfo");
	}
	return 0;
}

int ServerConf::Read(const char* filename)
{
	TiXmlDocument doc(filename);
	bool ret = doc.LoadFile();
	if (!ret)
	{
		cout<<"load file error!"<<filename<<endl;
		return -1;
	}
	m_cfgfilename = filename;
	TiXmlElement *root = doc.FirstChildElement("config");
	if(!root)
	{
		cout<<"config not exists!"<<endl;
		return -1;
	}
    if(-1 == ReadInfoCfg(root))
		return -1;
	if(-1 == ReadAllTestCfg(root))
		return -1;

    return 0;
}


