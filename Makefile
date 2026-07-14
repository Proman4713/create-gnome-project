default: create-gnome-project

objs=helpers/cgp_errors.o helpers/cgp_utils.o helpers/cgp_args.o helpers/cgp_templates.o
create-gnome-project: $(objs) main.c
	mkdir -p dist
	gcc $^ -o dist/$@ -lcs50 -lcurl

debug: CFLAGS=-g -fdiagnostics-color=always -O0
debug: $(objs) main.c
	mkdir -p dist
	gcc $(CFLAGS) $^ -o dist/$@ -lcs50 -lcurl

clean:
	rm helpers/*.o || true
	rm dist/create-gnome-project || true
	rm dist/debug || true

install:
	echo "Installing for Debian or RHEL systems..."
	( (which apt > /dev/null) && (curl -s https://packagecloud.io/install/repositories/cs50/repo/script.deb.sh | sudo bash) && sudo apt install libcs50 ) || true
	( (which dnf > /dev/null) && (curl -s https://packagecloud.io/install/repositories/cs50/repo/script.rpm.sh | sudo bash) && dnf install libcs50 ) || true