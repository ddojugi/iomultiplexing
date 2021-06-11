all: select client

select: select.c
	g++ -o select.c -pthread

client: client.c
	g++ -o client.c -pthread

clean:
	rm select client
