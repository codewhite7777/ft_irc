## 아이알씨플러스플러스

irc 채팅 클라이언트 프로그램인 irssi와 호환하는 irc 채팅 서버 프로그램

## 실행 방법

- irssi client
    - 설치 in macOS
        
        ```
        brew install irssi
        ```
        
    - 실헹
        
        ```
        irssi -c <server_ip> -p <server_port> -w <server_pwd> -n <nickname>
        ```
        
- ircserv(this project)
    - 설치
        
        ```
        git clone https://github.com/codewhite7777/ft_irc
        ```
        
    - 컴파일
        
        ```
        make
        ```
        
    - 실행
        
        ```
        ./ircserv <port> <password>
        ```
        

## 시스템 구성도

```
.
├─ main.cpp
├─ Server class
├─ Client class
├─ Channel class
├─ Command class
├─ Protocol class
├─ Singleton class
├─ Chatbot class
└─ utils.cpp
```

