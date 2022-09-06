#include "TaskThread.h"
#include "Util.h"

#include <unordered_map>
#include <string>

using namespace std;

unordered_map<int, User *> handlerToUser;
extern list<ServerStream *> g_StreamPool;
int TaskThread::processCmd(ACE_SOCK_Stream Stream, string cmd)
{
    cout << "--------------" << cmd << "--------------" << endl;
    if (cmd.substr(0,4) == "USER")
    {
        if(processUSER(Stream, cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
            return 0;
        }
    }else if (cmd.substr(0,4) == "PASS")
    {
        if(processPASS(Stream, cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
            return 0;
        }
    }else if (cmd.substr(0,4) == "SYST"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processSYST(Stream, cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,3) == "CWD")
    {
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(this->processCWD(Stream, cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,4) == "QUIT")
    {
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processQUIT(Stream)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,4) == "RETR")
    {
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processRETR(Stream, cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }

    }else if (cmd.substr(0,4) == "LIST"){
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processLIST(Stream,cmd)==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,4) == "PORT")
    {
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if( processPORT(Stream,cmd)==-1 ){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }

    }
    else if (cmd.substr(0,4) == "STOR")
    {
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processSTOR(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,4) == "XPWD")
    {
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processXPWD(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
     }
    else if (cmd.substr(0,4) == "AUTH")
    {
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processAUTH(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if (cmd.substr(0,3) == "PWD")
    {
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processPWD(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    else if(cmd.substr(0,4) == "PASV"){
        if(this->isUserLoginValid(Stream)==-1){
            return 0;
        }
        if(processPASV(Stream, cmd) ==-1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0,4) == "CDUP"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processCDUP(Stream) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0,3) == "MKD"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processMKD(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0,4) == "SIZE"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processSIZE(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0, 4) == "DELE"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processDELE(Stream,cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0, 4) == "RNFR"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processRNFR(Stream, cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }else if(cmd.substr(0, 4) == "RNTO"){
        if(this->isUserLoginValid(Stream) == -1){
            return 0;
        }
        if(processRNTO(Stream, cmd) == -1){
            ACE_DEBUG((LM_DEBUG,"ERROR! process USER CMD\n"));
        }
    }
    
    return 0;
}

int TaskThread::isUserLoginValid(ACE_SOCK_STREAM &Stream)
{
        if (handlerToUser.find(Stream.get_handle()) == handlerToUser.end() || handlerToUser[Stream.get_handle()]->getStatus() == 0)
        {
            Stream.send(USR_UNLOGIN, ACE_OS::strlen(USR_UNLOGIN), 0);
            return -1;
        } 
        return 0;
}
int TaskThread::svc(void)
{
    ACE_Message_Block *Msg;
    while (1)
    {
        getq(Msg);
        ACE_Data_Block *Data_Block = Msg->data_block();
        MsgData *pData = reinterpret_cast<MsgData *>(Data_Block->base());
        if (0 == pData->getDataFlog())
        {
            std::list<ServerStream *>::iterator it;
            for (it = g_StreamPool.begin(); it != g_StreamPool.end(); ++it)
            {
                if (get_handle() == (*it)->get_handle())
                {
                    g_StreamPool.erase(it);
                    delete *it;
                    break;
                }
            }
            return 0;
        }
        char strBuffer[MAX_BUFF_SIZE];
        ACE_OS::memset(strBuffer, 0, sizeof(strBuffer));
        ACE_OS::memcpy(strBuffer, pData->Data, sizeof(strBuffer));
        if (this->processCmd(*(pData->getIOHandle()), string(pData->Data)) == -1)
        {
            ACE_DEBUG((LM_ERROR, "error!\n"));
            ACE_OS::exit(-1);
        }

        Msg->release();
    }
    return 0;
}

int TaskThread::processUSER(ACE_SOCK_Stream &Stream, string &cmd){
        User *user = new User;
        string u_name = cmd.substr(5,cmd.size());
        trim(u_name);
        cout<<u_name<<endl;
        user->setUserName(u_name);
        if(Stream.send(PWD_REQUIRE, ACE_OS::strlen(PWD_REQUIRE), 0)==-1){
            return -1;
        }   
        ACE_DEBUG((LM_DEBUG, "wait for password\n"));
        handlerToUser[Stream.get_handle()] = user;
        return 0;
}

int TaskThread::processPASS(ACE_SOCK_Stream &Stream, string &cmd)
{
        if(handlerToUser.find(Stream.get_handle()) == handlerToUser.end()){
            return -1;
        }

        string file_pwd = handlerToUser[Stream.get_handle()]->getpwd(handlerToUser[Stream.get_handle()]->getUserName());

        string password = cmd.substr(5,cmd.size());

        trim(password);

        if (file_pwd == password)
        {
            handlerToUser[Stream.get_handle()]->setStatus(1);

            Stream.send(LOGIN_SUCCESS, ACE_OS::strlen(LOGIN_SUCCESS));
            return 0;
        }
        else
        {
            if(Stream.get_handle() == -1){
                cout<<"通道已经关闭"<<endl;
            }
            ACE_DEBUG((LM_DEBUG, "user %s login failed! \n", handlerToUser[Stream.get_handle()]->getUserName()));
            if (handlerToUser.find(Stream.get_handle()) != handlerToUser.end())
            {
                User *del = handlerToUser[Stream.get_handle()];
                handlerToUser.erase(Stream.get_handle());
                delete del;
            }
            Stream.send(USR_LOGI_FAILED, ACE_OS::strlen(USR_LOGI_FAILED));
            return 0;
        }
}

int TaskThread::processSYST(ACE_SOCK_Stream &Stream, string &cmd)
{
    if(Stream.send(SYST_CMD, ACE_OS::strlen(SYST_CMD))==-1){
        return -1;
    }
    return 0;
}

int TaskThread::processPASV(ACE_SOCK_Stream &Stream, string &cmd)
{
    User* user = handlerToUser[Stream.get_handle()];

    ACE_INET_Addr *file_addr = new ACE_INET_Addr;
    ACE_SOCK_Acceptor *file_accept = new ACE_SOCK_Acceptor; 
    int pPort;
    for(pPort = BEGINPORT; pPort < ENDPORT; pPort++){
        if(file_addr->set(pPort) == -1){
            continue;
        }
        if(file_accept->open(*file_addr)==-1){
            continue;
        }
    }
    if(pPort>=ENDPORT){
        Stream.send(PORT_EXHAUSTED, ACE_OS::strlen(PORT_EXHAUSTED));
    }
    std::stringstream ipAndPort;
    ipAndPort<<PASV_CMD<<"("<<DEFAULT_IP<<","<<(pPort>>8)<<","<<(pPort & 0xFF)<<").";
    Stream.send(ipAndPort.str().c_str(), ACE_OS::strlen(ipAndPort.str().c_str()),0);
    user->setPort(pPort);
    user->setIP(DEFAULT_IP);
    return 0;
}

int TaskThread::processPORT(ACE_SOCK_Stream &Stream, string &cmd)
{
    string ip_cmd = cmd.substr(5,cmd.size());
    trim(ip_cmd);
    string ip_addr = ip_cmd;

    int port1 = 0;
    int port2 = 0;
    int comma_counter = 0;
    int num_index = 0;
    int i = ip_addr.size()-1;

    while (!(comma_counter == 1 && ip_addr[i] == ','))
    {
        if (ip_addr[i] == ',')
        {
            comma_counter++;
            num_index = 0;
        }
        else // process number
        {
            double radix = 10;
            if (comma_counter == 0)
            {
                // the last segment of the ip address
                int curr_cha_number = ip_addr[i] - '0';

                port1 += int(curr_cha_number * pow(radix, num_index));

                num_index++;
            }
            else
            {
                int curr_cha_number = ip_addr[i] - '0';
                port2 += int(curr_cha_number * pow(radix, num_index));
                num_index++;
            }
        }
        i--;
    }

    int addr_len = ip_addr.size();
    for (int j = 0; j < addr_len; j++)
        if (ip_addr[j] == ',')
            ip_addr[j] = '.';
    ip_addr[i] = 0;
    handlerToUser[Stream.get_handle()]->setIP(ip_addr);
    cout<<ip_addr<<endl;
    int port = port2 * 256 + port1;
    handlerToUser[Stream.get_handle()]->setPort(port);
    cout<<port<<endl;
    Stream.send(PORT_COMMAND_SUCCESS, ACE_OS::strlen(PORT_COMMAND_SUCCESS), 0);
    ACE_DEBUG((LM_DEBUG, "user %s PORT command successful\n", handlerToUser[Stream.get_handle()]->getUserName()));
}

int TaskThread::processCWD(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string dir_cmd = cmd.substr(4,cmd.size());
    trim(dir_cmd);
    string upper_flag = "..";
    if (dir_cmd == upper_flag){
        this->processCDUP(Stream);
    }else{
        string toDir;
        if(!dir_cmd.empty()&&(dir_cmd[0] == '//'||dir_cmd[0] == '/')){
            toDir = dir_cmd;
        }else{
            toDir.append(currentUser->getCurrentDir()).append(dir_cmd);
        }
        ACE_stat state;
        if(ACE_OS::stat(toDir.c_str(),&state)<0){
            Stream.send(DIR_NOT_FIND,ACE_OS::strlen(DIR_NOT_FIND),0);
            ACE_DEBUG((LM_DEBUG,"user %s directory invalid\n",currentUser->getUserName()));
            return -1;
        }else{
            currentUser->setCurrentDir(toDir.append("/"));
            Stream.send(CWD_SUCCESS,ACE_OS::strlen(CWD_SUCCESS));
            ACE_DEBUG((LM_DEBUG,"user %s change directory\n",currentUser->getUserName()));
        }
    }
    return 0;
}

int TaskThread::processQUIT(ACE_SOCK_Stream &Stream)
{
    string q_cmd = "221 Goodbye\n";
    if(Stream.send(q_cmd.c_str(), ACE_OS::strlen(q_cmd.c_str()), 0) ==-1){
        return -1;
    }
    if (handlerToUser.find(Stream.get_handle()) != handlerToUser.end())
    {
        User *del = handlerToUser[Stream.get_handle()];
        handlerToUser.erase(Stream.get_handle());
        delete del;
    }
    return 0;
}

int TaskThread::processRETR(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string file_name = currentUser->getCurrentDir();

    file_name.append(cmd.substr(5,cmd.size()));
    trim(file_name);
    int len = file_name.size();

    FilesCtr fileCtr;
    if (!fileCtr.fileExist(file_name.c_str())){
        Stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
        ACE_DEBUG((LM_DEBUG,"user %s requested for an invalid file\n",currentUser->getUserName()));
        return -1;
    }else{
        //transfer file
        ACE_SOCK_Stream *data_stream = new ACE_SOCK_Stream();
        ACE_SOCK_Connector *data_con = new ACE_SOCK_Connector();
        ACE_INET_Addr *inet_add = new ACE_INET_Addr();
        inet_add->set(currentUser->getPort(),currentUser->getIP().c_str());

        if(data_con->connect(*data_stream,*inet_add)==-1){
            Stream.send(DATA_CONN_FAILED,ACE_OS::strlen(DATA_CONN_FAILED),0);
            ACE_DEBUG((LM_DEBUG,"user %s connection error.\n",currentUser->getUserName()));
            return -1;
        }
        FILE *fp;
        fp = ACE_OS::fopen(file_name.c_str(), "rb");
        if (fp == nullptr){
            Stream.send(OPEN_FILE_FAILED, ACE_OS::strlen(OPEN_FILE_FAILED), 0);
            ACE_DEBUG((LM_DEBUG,"user %s open file failed\n",currentUser->getUserName()));
            return -1;
        }

        ACE_OS::fseek(fp, 0L, 2);
        int len = ACE_OS::ftell(fp);
        ACE_OS::fseek(fp, 0L, 0);

        char *file_block = new char[FILE_DATA_BLOCK];
        int read_len = ACE_OS::fread(file_block, 1, FILE_DATA_BLOCK, fp);
        file_block[read_len] = 0;
        int data_read = 1024;
        ACE_DEBUG((LM_DEBUG, "opening file: %s\n", file_name));
        std::string str("150 Opening ASCII mode data connection for ");
        str.append(file_name);
        str.append(".\n");
        Stream.send(str.c_str(),ACE_OS::strlen(str.c_str()),0);
        data_stream->send(file_block,ACE_OS::strlen(file_block), 0);
        while(data_read<len){
            read_len = ACE_OS::fread(file_block, 1, FILE_DATA_BLOCK, fp);
            data_read += read_len;
            data_stream->send(file_block, read_len, 0);
        }
        data_stream->close();
        Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
        ACE_OS::fclose(fp);
        delete data_con;
        delete data_stream;
        delete inet_add;
        delete[] file_block;
        ACE_DEBUG((LM_DEBUG,"user %s download file %s.\n",currentUser->getUserName(),file_name));
        return 0;
    }
}

int TaskThread::processLIST(ACE_SOCK_Stream &Stream, string &cmd)
{
    Stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
    ACE_SOCK_Stream *data_stream = new ACE_SOCK_STREAM();
    ACE_SOCK_Connector *data_conn = new ACE_SOCK_Connector();
    ACE_INET_Addr *inet_addr = new ACE_INET_Addr();
    User *currentUser = handlerToUser[Stream.get_handle()];
    inet_addr->set(currentUser->getPort(), currentUser->getIP().c_str());
    if (data_conn->connect(*data_stream, *inet_addr) == -1)
    {
        Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
        return -1;
    }
    DIR *dir;//用于存放opendir()成功时的返回值
	struct dirent *file;//用于存放readir()的返回值
	struct stat info;//目录项描述结构体
	if(!(dir = opendir(currentUser->getCurrentDir().c_str()))){
		//打开目录失败
		cout<<"fail to read"<<currentUser->getCurrentDir().c_str();
		return -1;
	}
	struct passwd *userinfo;
	struct group *groupinfo;
	while((file = readdir(dir))!= NULL){
		//文件无文件名只有后缀
		if(strncmp(file->d_name, ".", 1) == 0)
			continue;
		strcpy(filename[filenum++],file->d_name);
		//获取文件信息，存储在info中
		stat(file->d_name,&info); 
		userinfo = getpwuid(info.st_uid);//所有者用户名 
		groupinfo = getgrgid(info.st_gid);//所有者所在组用户名
		char pw[11];
		power(info.st_mode,pw);//文件权限信息
        string file_info;
        long long fz = info.st_size;
		file_info.append(pw).append("\t").append((userinfo != nullptr ? userinfo->pw_name: ""))
            .append("  ").append(groupinfo != nullptr ?groupinfo->gr_name : "").append("  ");
        stringstream ss;
        ss<<fz<<"\t";
        file_info.append(ss.str());
        file_info+= 4 + ctime(&info.st_mtime);
        trimToParentDir(file_info);
		file_info.append("\t").append(file->d_name).append("\n");
		data_stream->send(file_info.c_str(),file_info.size(),0);
	}
	closedir(dir);
    data_stream->close();
    Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
    delete data_conn;
    delete data_stream;
    delete inet_addr;
    return 0;
}

int TaskThread::processSTOR(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *user = handlerToUser[Stream.get_handle()];
    Stream.send(OPEN_ASCII_MODE, ACE_OS::strlen(OPEN_ASCII_MODE), 0);
    string f_name = cmd.substr(5,cmd.size());
    trim(f_name);
    int leng = f_name.size();

    string file_path(user->getCurrentDir());
    file_path.append("/");
    file_path.append(f_name);
    char f_path[COMMAND_SIZE];
    ACE_OS::strcpy(f_path, file_path.c_str());

    FILE *fp;
    fp = ACE_OS::fopen(f_path, "wb+");

    ACE_SOCK_Stream *data_stream = new ACE_SOCK_Stream();
    ACE_SOCK_Connector *data_con = new ACE_SOCK_Connector();
    ACE_INET_Addr *inet_add = new ACE_INET_Addr();
    inet_add->set(user->getPort(), user->getIP().c_str());

    if (data_con->connect(*data_stream, *inet_add) == -1)
    {
        Stream.send(DATA_CONN_FAILED, ACE_OS::strlen(DATA_CONN_FAILED), 0);
        ACE_DEBUG((LM_DEBUG, "user %s connection error.\n", user->getUserName()));
        return -1;
    }

    char *rec_buf = new char[FILE_DATA_BLOCK];

    int rec_counter = data_stream->recv(rec_buf, REC_BLOCK);

    if (rec_counter < REC_BLOCK)
        rec_buf[rec_counter] = 0;

    int total_count = rec_counter;

    while (rec_counter != -1 && rec_counter == REC_BLOCK)
    {
        rec_counter = data_stream->recv(rec_buf, REC_BLOCK);
        ACE_OS::fwrite(rec_buf, 1, rec_counter, fp);
        total_count += rec_counter;
        if (rec_counter < REC_BLOCK)
            break;
    }
    rec_buf[total_count] = 0;

    ACE_OS::fclose(fp);
    data_stream->close();

    Stream.send(TRANS_COMPELETE, ACE_OS::strlen(TRANS_COMPELETE), 0);
    //ACE_DEBUG((LM_DEBUG, "user %s upload file %s in the current directory.\n", user->getUserName(), f_path));

    delete data_stream;
    delete data_con;
    delete inet_add;
    delete[] rec_buf;
    return 0;
}

int TaskThread::processXPWD(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *user = handlerToUser[Stream.get_handle()];
    char *dir_root = new char[COMMAND_SIZE];
    std::string str("257");
    str.append(user->getCurrentDir());
    str.append(" is current directory.\r\n");
    ACE_OS::strcpy(dir_root, str.c_str());
    if(Stream.send(dir_root, ACE_OS::strlen(dir_root), 0)==-1){
        return -1;
    }
    ACE_DEBUG((LM_DEBUG, "user %s display current directory\n", user->getUserName()));

    delete[] dir_root;
    return 0;
}

int TaskThread::processAUTH(ACE_SOCK_Stream &Stream, string &cmd)
{
    if(Stream.send(AUTH_CMD, ACE_OS::strlen(AUTH_CMD), 0)==-1){
        return -1;
    }
    return 0;
}

int TaskThread::processPWD(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *user = handlerToUser[Stream.get_handle()];
    string currentDir;
    currentDir.append(PWD_CMD).append(user->getCurrentDir()).append(" is the current directory.\r\n");
    Stream.send(currentDir.c_str(), currentDir.size(), 0);
    return 0;
}

int TaskThread::processSIZE(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if(dir[0] != '/'){
        dir = currentUser->getCurrentDir() + dir;
    }
    if(opendir(dir.c_str()) == NULL) //判断目录
	{
        Stream.send(DIR_NOT_FIND, ACE_OS::strlen(DIR_NOT_FIND), 0);
        return 0;
	}
    struct stat statbuf;
	stat(dir.c_str(), &statbuf);
	size_t filesize = statbuf.st_size;
    string res =SIZE_CMD + filesize;
    Stream.send(res.c_str(), res.size(), 0);
    return 0;
}

int TaskThread::processCDUP(ACE_SOCK_Stream &Stream)
{
    User *currentUser = handlerToUser[Stream.get_handle()]; 
    if (currentUser->getCurrentDir() == ROOT_DIR){
        Stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
    }else{
        string currentdir = currentUser->getCurrentDir();
        int length = currentdir.size()-2;
        while (currentUser->getCurrentDir()[length] != '/' && currentUser->getCurrentDir()[length] != '//' && length >= 0)
        {
            length--;
        }
        currentUser->setCurrentDir(currentdir.substr(0,length).append("/"));
        Stream.send(CWD_SUCCESS, ACE_OS::strlen(CWD_SUCCESS));
    }
    ACE_DEBUG((LM_DEBUG, "user %s modified directory!\n", currentUser->getUserName()));
    return 0;
}

int TaskThread::processMKD(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string newDir = cmd.substr(4,cmd.size());
    trim(newDir);
    if(newDir[0] !='/'&&newDir.size()>1){
        newDir = currentUser->getCurrentDir() + newDir;
    }
    if(mk_dir(newDir.c_str())==-1){
        Stream.send(MkDIR_ERRROE, ACE_OS::strlen(MkDIR_ERRROE), 0);
        return 0;
    }
    string resMessage = MKDIR_CMD + newDir +" created.\n";
    Stream.send(resMessage.c_str(), resMessage.size(), 0);
    return 0;
}

int TaskThread::processRNFR(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if(dir[0] !='/'&&dir.size()>1){
        dir = currentUser->getCurrentDir() + dir;
    }
    FILE *fp;
    fp = ACE_OS::fopen(dir.c_str(), "r");
    if (fp == nullptr){
        Stream.send(RNFR_FAILED, ACE_OS::strlen(RNFR_FAILED), 0);
        return 0;
    }
    currentUser->setRenamefrom(dir);
    Stream.send(RNFR_CMD,ACE_OS::strlen(RNFR_CMD),0);
    return 0;
}

int TaskThread::processRNTO(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if(dir[0] !='/'&&dir.size()>1){
        dir = currentUser->getCurrentDir() + dir;
    }
    if (rename(currentUser->getRenameFrom().c_str(),dir.c_str()) < 0){
        Stream.send(RNTO_FAILED,ACE_OS::strlen(RNTO_FAILED), 0);
        return 0;
    }
    Stream.send(RENAME_SUCCESS, ACE_OS::strlen(RENAME_SUCCESS), 0);
    return 0;
}

int TaskThread::processDELE(ACE_SOCK_Stream &Stream, string &cmd)
{
    User *currentUser = handlerToUser[Stream.get_handle()];
    string dir = cmd.substr(5,cmd.size());
    trim(dir);
    if(dir[0] != '/'){
        dir = currentUser->getCurrentDir() + dir;
    }
    FILE *fp;
    fp = ACE_OS::fopen(dir.c_str(), "r");
    if (fp == nullptr){
        Stream.send(DELETE_FAILE, ACE_OS::strlen(DELETE_FAILE), 0);
        return 0;
    }
    if(remove(dir.c_str())!=0){
        Stream.send(DELETE_FAILE, ACE_OS::strlen(DELETE_FAILE), 0);
        return 0;
    }
    Stream.send(DELETE_SUCCESS, ACE_OS::strlen(DELETE_SUCCESS), 0);
    return 0;
}