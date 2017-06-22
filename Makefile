objs = main.o ttms_datastruct.o ttms_io.o ttms_tty.o ttms_ui.o ttms_global.o ttms_ui_func.o ttms_srv.o \
	ttms_ui_studio.o ttms_ui_account.o ttms_ui_play.o ttms_ui_seat.o ttms_ui_sale.o \
	ttms_ui_schedule.o ttms_ui_query.o
args = -std=c11
tmms: $(objs)
	gcc $(args) -o ttms $(objs)

main.o: main.c
	gcc -c main.c $(args)
ttms_global.o: ttms_global.c
	gcc -c ttms_global.c $(args)
ttms_datastruct.o: ttms_datastruct.c include/ttms_datastruct.h include/ttms_stddef.h
	gcc -c ttms_datastruct.c $(args)
ttms_io.o: ttms_io.c
	gcc -c ttms_io.c $(args)
ttms_ui.o: ttms_ui.c
	gcc -c ttms_ui.c $(args)
ttms_tty.o: ttms_tty.c
	gcc -c ttms_tty.c $(args)
ttms_ui_func.o: ttms_ui_func.c
	gcc -c ttms_ui_func.c $(args)
ttms_ui_studio.o: ttms_ui_studio.c
	gcc -c ttms_ui_studio.c $(args)
ttms_srv.o: ttms_srv.c
	gcc -c ttms_srv.c $(args)
ttms_ui_account.o: ttms_ui_account.c
	gcc -c ttms_ui_account.c $(args)
ttms_ui_play.o: ttms_ui_play.c
	gcc -c ttms_ui_play.c $(args)
ttms_ui_seat.o: ttms_ui_seat.c
	gcc -c ttms_ui_seat.c $(args)
ttms_ui_sale.o: ttms_ui_sale.c
	gcc -c ttms_ui_sale.c $(args)
ttms_ui_schedule.o: ttms_ui_schedule.c
	gcc -c ttms_ui_schedule.c $(args)
ttms_ui_query.o:  ttms_ui_query.c
	gcc -c ttms_ui_query.c $(args)

clean:
	-rm $(objs)
