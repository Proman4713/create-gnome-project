objs = helpers/cgp_errors.o helpers/cgp_utils.o
default: main.c $(objs)
	mkdir -p dist
	gcc main.c $(objs) -o dist/create-gnome-project

clean:
	rm helpers/*.o
	rm dist/create-gnome-project