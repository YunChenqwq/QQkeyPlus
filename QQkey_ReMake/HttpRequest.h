#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>
#include <vector>
#include <WinSock.h>
#include <iostream>
#include<wininet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "wininet.lib")
class HttpRequest {
private:
    HINTERNET m_hHttpRequest;

public:
    HttpRequest() : m_hHttpRequest(NULL) {}

    void InitializeURLComponentsA(URL_COMPONENTSA& crackedURL, const char* URL_STRING) {
        char* szHostName = new char[128];
        char* szUrlPath = new char[256];
        crackedURL.dwStructSize = sizeof(URL_COMPONENTSA);
        crackedURL.lpszHostName = szHostName;
        crackedURL.dwHostNameLength = 128;
        crackedURL.lpszUrlPath = szUrlPath;
        crackedURL.dwUrlPathLength = 256;
        InternetCrackUrlA(URL_STRING, (DWORD)strlen(URL_STRING), 0, &crackedURL);
    }
    BOOL SendHttpRequest(HINTERNET hInternet, const char* hostName, const char* urlPath,int port,LPSTR lpHeaders) {
        HINTERNET hHttpSession = InternetConnectA(hInternet, hostName, port, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        m_hHttpRequest = HttpOpenRequestA(hHttpSession, "GET", urlPath, NULL, "", NULL, INTERNET_FLAG_SECURE, 0);
        DWORD leng;
        // 发送HTTP请求
        if (lpHeaders == NULL)
        {
            leng = sizeof(lpHeaders);
        }
        else leng = strlen(lpHeaders);  //莫名奇妙的bug 玄学修复
        return HttpSendRequestA(m_hHttpRequest, lpHeaders,leng, NULL, 0);
    }

    DWORD GetHttpStatusCode() {
        DWORD dwRetCode = 0;
        DWORD dwSizeOfRq = sizeof(DWORD);
        HttpQueryInfo(m_hHttpRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &dwRetCode, &dwSizeOfRq, NULL);
        return dwRetCode;
    }

    void ReadHttpHeaders(char* lpHeaderBuffer, DWORD dwSizeOfRq) {
        // 读取整个Headers
        DWORD dwSize = dwSizeOfRq;
        HttpQueryInfo(m_hHttpRequest, HTTP_QUERY_RAW_HEADERS, lpHeaderBuffer, &dwSize, NULL);
    }
};


#endif // HTTPREQUEST_H
