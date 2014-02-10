all:
	g++ -I /usr/local/boost_1_55_0/ main.cpp -std=gnu++11 -o main -lboost_system -lboost_filesystem -lboost_regex

clean:
	rm main
