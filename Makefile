objs = main.o global.o interface.o io.o linklist.o service.o tty.o \
       ui_account.o frame.o ui_main.o ui_play.o ui_query.o \
	   ui_sale.o ui_schedule.o ui_seat.o ui_studio.o
args = -std=c11
tmms: $(objs)
	gcc $(args) -o ttms $(objs)

main.o:
	gcc -c main.c $(args)
global.o:
	gcc -c global.c $(args)
interface.o:
	gcc -c interface.c $(args)
io.o:
	gcc -c io.c $(args)
linklist.o:
	gcc -c linklist.c $(args)
service.o:
	gcc -c service.c $(args)
tty.o:
	gcc -c tty.c $(args)
ui_account.o:
	gcc -c ui_account.c $(args)
frame.o:
	gcc -c frame.c $(args)
ui_main.o:
	gcc -c ui_main.c $(args)
ui_play.o:
	gcc -c ui_play.c $(args)
ui_query.o:
	gcc -c ui_query.c $(args)
ui_sale.o:
	gcc -c ui_sale.c $(args)
ui_schedule.o:
	gcc -c ui_schedule.c $(args)
ui_seat.o:
	gcc -c ui_seat.c $(args)
ui_studio.o:
	gcc -c ui_studio.c $(args)

clean:
	-rm $(objs)
