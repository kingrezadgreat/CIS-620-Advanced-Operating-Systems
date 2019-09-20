

all:
	make server
	make servicemap
	make client
	
	
server: server.o 
	gcc -o server server.o

servicemap: servicemap.go 
	go build servicemap.go

client: client.go 
	go build client.go

clean:
	rm server servicemap client *.o
	

