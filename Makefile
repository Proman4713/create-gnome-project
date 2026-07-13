default: create-gnome-project

objs=helpers/cgp_errors.o helpers/cgp_utils.o helpers/cgp_args.o
create-gnome-project: $(objs) main.c
	mkdir -p dist
	gcc $^ -o dist/$@ -lcs50 -Wno-discarded-qualifiers

%.o: %.c
	gcc -c -o $@ $^ -Wno-discarded-qualifiers

clean:
	rm helpers/*.o
	rm dist/create-gnome-project