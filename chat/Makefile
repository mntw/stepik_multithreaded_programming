all: select poll echo 
echo: 
	gcc -o echo tcp_echo_server.c
select:
	g++ -std=c++11 -o SelectEchoServer multiConnectionalEchoServer_select.cpp 
poll:
	g++ -std=c++11 -o PollEchoServer multiConnectionalEchoServer_poll.cpp 
clean:
	rm SelectEchoServer
	rm PollEchoServer 
	rm echo 
