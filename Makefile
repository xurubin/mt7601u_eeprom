all: mt7601 kmod

mt7601: mt7601.o

kmod:
	make -C /lib/modules/`uname -r`/build M=$(PWD)/mt7601u