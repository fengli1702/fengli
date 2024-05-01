//PB22111702 李岱峰所有
// IO
#include <iostream>
// std::string
#include <string>
// std::vector
#include <vector>
// std::string 转 int
#include <sstream>
// PATH_MAX 等常量
#include <climits>
// POSIX API
#include <unistd.h>
// wait
#include <sys/wait.h>
// open
#include <fcntl.h>
//str
#include <cstring>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;

std::vector<std::string> split(std::string s, const std::string &delimiter);
void get_redirect(vector<string> commands);
void get_redirect_pro(vector<string> args, string cmd);
void getpwd();//让shell像linux版一样

pid_t run_pid;//标记运行进程号
bool control_c=false;
bool have_pipe=false;
void signcontol(int signum) {//控制信号设置
    control_c=true;
    cout << endl;
    if (run_pid != 0) {
        kill(run_pid, SIGTERM); // 终止当前运行的进程
        cout << endl;
        control_c=false;
        return;
    }
    if(have_pipe==false)
    cout << "$ " << std::flush;
    else{
        have_pipe=false;
    }
}

vector<pid_t> wait_pid; //后台运行进程库

void my_wait(){
    for(auto pid : wait_pid){
        int status;
        waitpid(pid, &status, 0);//等待所有后台运行进程
    }
    wait_pid.clear();
}
//history
vector<string> history;

string echo_root(string dict) {
    struct passwd *pw = getpwnam(dict.c_str());//获取家目录
    if (pw != nullptr) {
        return pw->pw_dir;
    } else {
        return "/"+dict; 
    }
}

int main() {
    signal(SIGINT, signcontol);

  // 不同步 iostream 和 cstdio 的 buffer
  std::ios::sync_with_stdio(false);
  //signal信号
  // 用来存储读入的一行命令
  std::string cmd;
  while (true) {
    have_pipe=false;
    // 打印提示符
//    std::cout << "~";
//    getpwd();

    cout << "$ ";
    std::cout.flush();
    if(!getline(std::cin, cmd)){
        std::cout << "CTRL+D\n";
        return 0;
    }
    
    //保存记录
    history.push_back(cmd);

    // 按空格分割命令为单词
    std::vector<std::string> args = split(cmd, " ");
//-------------------------echo-----------------------------
    if(args[0]=="echo"){
        if(args.size()<=1){
            cout<<"echo命令错误"<<endl;
            continue;
        }
        for(int i=1;i<args.size();i++){
            if(args[i].find("~")==string::npos&&args[i]!="$SHELL"){
                cout<<args[i]<<" ";
            }
            //实现$SHELL
            else if(args[i]=="$SHELL"){
                cout<<getenv("SHELL")<<" ";
            }
            else{
                cout<<args[i].substr(0,args[i].find("~"));
                string root = echo_root(args[i].substr(args[i].find("~")+1,args[i].size()-1));
                cout<<root<<" ";
            }
        }
        cout<<endl;
        continue;
    }
//-------------------------!!重复执行功能----------------------
    
    if(args[0]=="!!"){
        if(history.size()==1){
            cout<<"没有历史命令"<<endl;
            continue;
        }
        cmd=history[history.size()-2];
        args = split(cmd, " ");
    }
    else if(args[0].find("!")!=string::npos){
        int num = 0;
        string num_str = args[0].substr(1,args[0].size()-1);
        if(num_str.empty()){
            cout << "缺少参数\n";
            continue;
        }
        stringstream code_stream(num_str);
        code_stream >> num;
        if(num>history.size()){
            cout<<"没有该条历史记录"<<endl;
            continue;
        }
        cmd=history[num-1];
        args = split(cmd, " ");
    }

//------------------------------------------------------------

    // 没有可处理的命令
    if (args.empty()) {
      continue;
    }
    // 退出
    if (args[0] == "exit") {
      if (args.size() <= 1) {
        return 0;
      }
      
      // std::string 转 int
      std::stringstream code_stream(args[1]);
      int code = 0;
      code_stream >> code;

      // 转换失败
      if (!code_stream.eof() || code_stream.fail()) {
        std::cout << "Invalid exit code\n";
        continue;
      }
      return code;
    }
//-------------------------history--------------------`-------
    if(args[0]=="history"){
        int hisnum = 1;
        if(args.size()>2){
            cout<<"参数过多"<<endl;
            continue;
        }
        else if(args.size()==2){
        std::stringstream number_stream(args[1]);
        number_stream >> hisnum;}
        else{
            cout<<"无参数，默认1"<<endl;
            hisnum=1;
        }

        if(history.size()<hisnum){
            cout<<"历史记录不足"<<endl;
            continue;
        }
        for(int i=history.size()-hisnum;i<history.size();i++){
            cout<< i+1<<"  "<<history[i]<<endl;
        }
        continue;
    }

//-------------------------后台管理----------------------------
    bool runinback=false;
    if(args[args.size()-1]=="&"){
        args.pop_back();
        cmd=cmd.substr(0,cmd.find("&")-1);
        runinback=true;
    }

//-------------------------wait-------------------------------
    if(args[0]=="wait"){
        my_wait();
        continue;
    }

//-------------------------pipline--------------------------
    if(cmd.find("|")!=string::npos){
        have_pipe=true;
        //生成commands列表
        vector<string> coms = split(cmd, " ");
        int i=0;
        vector<vector<char *>> commands;
        vector<vector<string>> commands_string;
        vector<char *> command;
        vector<string> command_string;
        while(i!=coms.size()){
            if(coms[i]=="|"){
                command.push_back(nullptr);  
                commands.push_back(command);
                commands_string.push_back(command_string);
                command.clear();
                command_string.clear();
                i++;
            }
            else{
                command.push_back(coms[i].data());
                command_string.push_back(coms[i]);
                i++;
            }
        }
        command.push_back(nullptr);  
        commands.push_back(command);
        commands_string.push_back(command_string);
        //开多个管道，每个命令1个管道
        int check=0;
        vector<int> pipefds(commands.size()*2,-1);
        for(size_t i=0;i<commands.size();i++){
                if(pipe(pipefds.data()+i*2)==-1){
                    cout << "打开管道失败"<<endl;
                    exit(255);
            }
        }
        pid_t cpid;
        for(size_t i=0;i<commands.size();i++){
            cpid = fork();
            if(cpid==0){//子进程
//            signal(SIGINT, SIG_DFL);
                if(i!=0){
                    if(dup2(pipefds[(i-1)*2],STDIN_FILENO)==-1){
                        cout << "打开管道失败"<<endl;
                        exit(255);
                    }
                }
                if(i!=commands.size()-1){//不是最后一个命令
                    if(dup2(pipefds[i*2+1],STDOUT_FILENO)==-1){
                        cout << "打开管道失败"<<endl;
                        exit(255);
                    }
                }
                for(auto& fd:pipefds){//关闭所有文件描述符
                    close(fd);
                }
              
                for(string now_com : commands_string[i]) //找到出现在管道里的重定向
                  if(now_com==">"||now_com==">>"||now_com=="<") 
                  {   
                    get_redirect(commands_string[i]);//执行"cat -n > out"类似的操作，要执行cat，也要导入out
                    check=1;
                    break;
                  }
                if(check==0)
                    execvp(commands[i][0],commands[i].data());
                exit(255);
                cout<<"命令错误"<<endl;
            }
            else if(cpid<0){
                cout<<"进程错误"<<endl;
            }
        }
        //父进程
        for(auto& fd:pipefds){//关闭所有文件描述符
              close(fd);
        }
        if(runinback){
            wait_pid.push_back(cpid);
            }
        else{
            int status;
            while(wait(nullptr)>0);
            }
        continue;
    }

//-------------------------重定向---------------------------
    if((cmd.find(">")!=string::npos||cmd.find("<")!=string::npos||cmd.find(">>")!=string::npos)&&cmd.find("<<")==string::npos){
        get_redirect(args);
        continue;
    } 
    if(cmd.find("<<")!=string::npos){
        get_redirect_pro(args,cmd);
        continue;
    }
//---------------------------pwd---------------------------shell-
    if (args[0] == "pwd") {
        if (args.size() >= 3||args.size() <= 0) { //错误的参数数目
        std::cout<<"错误的参数"<<std::endl;
        exit(255);
        }
     //设计支持option -L，—P
        char pwd[PATH_MAX];
        if (getcwd(pwd, PATH_MAX) == nullptr) {
            std::cout << "getcwd失败\n";
            continue;

        }
        std::cout << pwd << std::endl;
        continue;
    }
//---------------------------cd-----------------------------
    if (args[0] == "cd") {
      if (args.size() >= 3||args.size() <= 0) { //错误的参数数目
        std::cout<<"错误的参数"<<std::endl;
        exit(255);
        }
      //"cd" 将返回根目录
        if (args.size() == 1) {
            if (chdir("/") < 0) {
            std::cout << "返回根目录失败！"<<std::endl;
            }
            continue;
        }
        else{
            if (chdir(args[1].c_str()) < 0) {
            std::cout << "不存在的目录或切换失败！\n";
            }
            continue;
        }
    }

    // 处理外部命令
    run_pid = fork();

    // std::vector<std::string> 转 char **
    char *arg_ptrs[args.size() + 1];
    for (auto i = 0; i < args.size(); i++) {
      arg_ptrs[i] = &args[i][0];
    }
    // exec p 系列的 argv 需要以 nullptr 结尾
    arg_ptrs[args.size()] = nullptr;

    if (run_pid == 0) {
        setpgrp();
        tcgetpgrp(STDOUT_FILENO);

      // 这里只有子进程才会进入
      // execvp 会完全更换子进程接下来的代码，所以正常情况下 execvp 之后这里的代码就没意义了
      // 如果 execvp 之后的代码被运行了，那就是 execvp 出问题了
      execvp(args[0].c_str(), arg_ptrs);

      // 所以这里直接报错
      exit(255);
    }

    // 这里只有父进程（原进程）才会进入
    setpgid(run_pid, run_pid);
    tcsetpgrp(STDOUT_FILENO, run_pid);
    signal(SIGTTOU,SIG_IGN);

    if(runinback){
        wait_pid.push_back(run_pid);
        run_pid = 0;
    }
    else{
    int status;
    waitpid(run_pid, &status, 0);
    run_pid = 0;}

    tcsetpgrp(STDOUT_FILENO, getpgrp());
  }
}

// 经典的 cpp string split 实现
// https://stackoverflow.com/a/14266139/11691878
std::vector<std::string> split(std::string s, const std::string &delimiter) {
  std::vector<std::string> res;
  size_t pos = 0;
  std::string token;
  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    res.push_back(token);
    s = s.substr(pos + delimiter.length());
  }
  res.push_back(s);
  return res;
}
void getpwd(){
    char pwd[PATH_MAX];
    if (getcwd(pwd, PATH_MAX) == nullptr) {
            std::cout << "getcwd failed\n";
    }
    std::cout << pwd ;
}
void get_redirect(vector<string> commands){//"cat -n 10> out" -> "cat" "-n" "10>" "out"
    int fd[32];//最多32个文件描述符
    int fdstd[32];//支持数字描述符
    vector<string> fdname1,fdname2,fdname3; //文件名
    int fdna1=0,fdna2=0,fdna3=0;

    int location_=0;int code = 0;
    for(string now_com : commands){ //找到出现在管道里的重定向
        
        if(now_com.find(">>")!=string::npos) 
        {   //写流
            fdname1.push_back(commands[location_+1]);
        }
        else if(now_com.find(">")!=string::npos) 
        {   //写流
            fdname2.push_back(commands[location_+1]);   
        }
        else if(now_com.find("<")!=string::npos&&now_com.find("<<")==string::npos) 
        {   //读流
            fdname3.push_back(commands[location_+1]);  
        }
        location_++;
    }
    int all_file=fdname1.size()+fdname2.size()+fdname3.size();

    for(string now_com : commands){ //找到出现在管道里的重定向
        if(now_com.find(">>")!=string::npos) 
        {   //写流
            if(now_com.substr(0,now_com.find(">>"))!=""){
                //支持数字文件描述符
                stringstream code_stream(now_com.substr(0,now_com.find(">>")));
                code_stream >> code;
                fdstd[fdna1]=code;
            }
            else
                fdstd[fdna1]=1;
            fdna1++;
        }
        else if(now_com.find(">")!=string::npos) 
        {   //写流
            if(now_com.substr(0,now_com.find(">"))!=""){
                //支持数字文件描述符
                stringstream code_stream(now_com.substr(0,now_com.find(">")));
                code_stream >> code;
                fdstd[fdname1.size()+fdna2]=code;
            }
            else
                fdstd[fdname1.size()+fdna2]=1;
             fdna2++;
        }
        else if(now_com.find("<")!=string::npos&&now_com.find("<<")==string::npos) 
        {   //读流
            if(now_com.substr(0,now_com.find("<"))!=""){
                //支持数字文件描述符
                stringstream code_stream(now_com.substr(0,now_com.find(">")));
                code_stream >> code;
                fdstd[fdname1.size()+fdname2.size()+fdna3]=code;
            }
            else
                fdstd[fdname1.size()+fdname2.size()+fdna3]=0;
            fdna3++;
        }
    }

    //---------------
    if(all_file>31){
      cout<<"输入的文件过多(>31)"<<endl;
      exit(255);
    }
    for(int i=0;i<fdname1.size();i++){//">> out"
        fd[i]=open(fdname1[i].c_str(),O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd[i] == -1) {
            cout<<"打开文件失败"<<endl;
            exit(255);
        }
    }
    for(int i=0;i<fdname2.size();i++){//"> out"
        fd[i+fdname1.size()]=open(fdname2[i].c_str(),O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd[i+fdname1.size()] == -1) {
            cout<<"打开文件失败"<<endl;
            exit(255);
        }
    }
    for(int i=0;i<fdname3.size();i++){//"< out"
        fd[i+fdname1.size()+fdname2.size()]=open(fdname3[i].c_str(),O_RDONLY, 0644);
        if (fd[i+fdname1.size()+fdname2.size()] == -1) {
            cout<<"打开文件失败"<<endl;
            exit(255);
        }
    }
    //在这里开始执行指令“cat” “-n”
    //考虑命令只会出现在该句句首

    pid_t pid = fork();
    if(pid == 0){
        for(int i=0;i<all_file;i++){
            if(i<fdname1.size()){
                if (dup2(fd[i], fdstd[i]) == -1) {
                    cout<<"重定向文件失败"<<endl;
                    exit(255);
                }
            }
            else if(i<fdname1.size()+fdname2.size()){
                if (dup2(fd[i], fdstd[i]) == -1) {
                    cout<<"重定向文件失败"<<endl;
                    exit(255);
                }
            }
            else{
                if (dup2(fd[i], fdstd[i]) == -1) {
                    cout<<"重定向文件失败"<<endl;
                    exit(255);
                }
            }
        }
        for(int i=0;i<all_file;i++)//关闭文件描述符
        close(fd[i]);

        //得到三种操作符之前的命令
        vector<string> args;
        for(auto commder: commands){
            if(commder.find("<")!=string::npos||commder.find(">>")!=string::npos||commder.find(">")!=string::npos){
                break;
            }
            else{
                args.push_back(commder);
            }
        }

        //vector<string>转 vector<char*>
        vector<char*> args_cstr;
        for (const string& arg : args) {
            args_cstr.push_back(const_cast<char*>(arg.c_str()));
        }
        args_cstr.push_back(nullptr);

        execvp(args_cstr[0], args_cstr.data());
        
    }
    else if(pid<0){
        cout<<"进程错误"<<endl;
    
    }
    else if(pid>0){
        wait(nullptr);
        for(int i=0;i<all_file;i++)//关闭文件描述符
        close(fd[i]);
    }
    else{
      cout<<"pid进程错误"<<endl;
    }

    return;
}
void get_redirect_pro(vector<string> args, string cmd){  //解决"cat -n << EOF"
    vector<string>  commands;
    string get_arg;
    int location=0;
    for(auto get_ : args){
        if(get_.find("<<")!=string::npos){
            get_arg = args[location+1];
            break;
        }
        commands.push_back(get_);
        location++;
    }
    //获得结束字符串

    string text;
    char** cmd_char=new char*[100];
    int num_size=0;
    cout<<">";
    getline(cin, cmd);
    while(cmd!=get_arg){
        text=cmd;    text+="\n";
        cmd_char[num_size]=new char[text.size()+1];
        strcpy(cmd_char[num_size],text.c_str());
        cout<<">";
        getline(cin, cmd);
        num_size++;
    }
    vector<char*> args_cstr;
    for (const string& arg : commands) {
        args_cstr.push_back(const_cast<char*>(arg.c_str()));
        }
    args_cstr.push_back(nullptr);
    // 创建管道
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        cout << "创建管道失败" << endl;
        exit(255);
    }

    pid_t pid = fork();
    if (pid == -1) {
        cout << "创建管道中进程失败" << endl;
        exit(255);
    } else if (pid == 0) { // 子进程
        // 关闭管道的写入端
        close(pipe_fd[1]);
        // 将管道的读取端重定向到标准输入
        dup2(pipe_fd[0], STDIN_FILENO);
        // 关闭多余的文件描述符
        close(pipe_fd[0]);

        // 执行命令
        execvp(args_cstr[0], args_cstr.data());
        
    } else { // 父进程
        // 关闭管道的读取端
        close(pipe_fd[0]);
        // 写入文本块到管道的写入端
        char* text;
        for (int i=0;i<num_size;i++) {
            text = cmd_char[i];
            ssize_t copy_ = write(pipe_fd[1], text, strlen(text));
            if (copy_ == -1) {
                cout << "写入管道失败" << endl;
                 exit(255);
            }
        }
        // 关闭管道的写入端
        close(pipe_fd[1]);
        wait(nullptr);
    }
    for (int i = 0; i < 100; ++i) {
    delete[] cmd_char[i];
    }
    delete[] cmd_char;
    return ;
}