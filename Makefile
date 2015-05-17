build:
	gcc wget.c -g -Wall -o myclient
clean:
	rm -r myclient www.google.ro
run:
	./myclient http://www.google.ro/?gws_rd=cr&amp;ei=e8pYVaHuJsbfU6uhgYgC
