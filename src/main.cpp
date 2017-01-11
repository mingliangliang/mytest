#include <iostream>
#include <stdio.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <string.h>
#include <yt/util/protocolstream.h>
#include <yt/util/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include "serverconf.h"

using namespace yt;
using namespace std;
int main(int argc,char* argv[])
{
    //¶ÁÈ¡ÅäÖÃÐÅÏ¢
    ServerConf* sconf = ServerConf::Instance();
    if(sconf->Read("./cfg.xml") != 0)
    {
        cout<<"read failed!"<<endl;
        return 0;
    }

    int sockfd;
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        cout << "socket error" << strerror(errno) << endl;
        return 0;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(sconf->m_info.m_port);
    server_addr.sin_addr.s_addr = inet_addr(sconf->m_info.m_ip.c_str());
    if(connect(sockfd,(struct sockaddr *)(&server_addr),sizeof(struct sockaddr)) == -1)
    {
        cout << "connect error " << strerror(errno) << endl;
        return 0;
    }
    cout<<endl<<"begin!"<<endl;
    int seq = 1;
    map<int, Test> &all = ServerConf::Instance()->m_allTest;
	for(map<int, Test>::iterator iter=all.begin(); iter!=all.end(); iter++)
	{
	    if(iter->second.m_iEnable != 1)
            continue;
        for(size_t i = 0; i<iter->second.m_vecJson.size(); i++)
        {
            if(iter->second.m_vecJson[i].m_iTest != 1)
                continue;
            string sendbuf;
            BinaryWriteStream3 stream_w(&sendbuf);
            stream_w.Write(iter->first);
            stream_w.Write(seq++);
            stream_w.Write(iter->second.m_User.c_str(),iter->second.m_User.length());
            stream_w.Write(iter->second.m_vecJson[i].m_Json.c_str(),iter->second.m_vecJson[i].m_Json.length());
            stream_w.Flush();

            struct timeval tvbegin,tvend;
            gettimeofday(&tvbegin,NULL);
            if((send(sockfd,stream_w.GetData(),stream_w.GetSize(),0)) == -1)
            {
                cout << "send error : " << strerror(errno) << endl;
                break;
            }
            int nbytes=0;
            char recvbuf[4096];
            if((nbytes = recv(sockfd,recvbuf,sizeof(recvbuf),0)) <= 0)
            {
                cout << "read error : " << strerror(errno) << endl;
                break;
            }

            BinaryReadStream2 stream(recvbuf,nbytes);
            int cmd;
            if(!stream.Read(cmd))
                break;
            int seq_r;
            if(!stream.Read(seq_r))
                break;
            int ret;
            if(!stream.Read(ret))
                break;
            string retdata;
            size_t datalen;
            if (!stream.Read(&retdata, 0, datalen))
                break;
            cout<<"read cmd: "<<cmd <<endl;
            cout<<"read seq: "<<seq_r <<endl;
            cout<<"read ret: "<<ret <<endl;
            cout<<"send json: "<<iter->second.m_vecJson[i].m_Json.c_str() <<endl;
            cout<<"read json: "<<retdata <<endl;
            cout<<endl;
        }
	}
	cout << "over!" << endl;
    close(sockfd);

    return 0;
}
