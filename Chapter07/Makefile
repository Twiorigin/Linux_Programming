all: application
INCLUDE=/usr/include/gdbm
LIBS=gdbm
LIBS= -lgdbm_compat -lgbm
CFLAGS=
app_ui.o: app_ui.c cd_data.h
	gcc $(CFLAGS) -c app_ui.c
access.o: access.c cd_data.h
	gcc $(CFLAGS) -I$(INCLUDE) -c access.c
application: app_ui.o access.o
	gcc $(CFLAGS) -o application app_ui.o access.o -l$(LIBS)
clean:
	rm -f application *.o
nodbmfiles:
	rm -f *.dir *.pag
