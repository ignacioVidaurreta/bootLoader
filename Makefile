IMAGE = Image/x64BareBonesImage.qcow2

QEMU = /usr/bin/qemu-system-x86_64
QEMUOPTS = -hda $(IMAGE) -m 512 -soundhw pcspk -monitor stdio
QEMU_GDB = -s -S
QEMU_NO_GRAPH = -nographic 

.PHONY: bootloader image collections kernel userland all clean go kill gdb 

all:  bootloader kernel userland image
	chmod a+rwx Image/*

# Run and wait for gdb in terminal mode
run: 
	$(QEMU) $(QEMUOPTS) $(QEMU_GDB) 

# Run without waiting
go: 
	$(QEMU) $(QEMUOPTS) 

# Close qemu
kill:
	killall $(QEMU)

# gdb launching
gdb:
	gdb

bootloader:
	$(MAKE) -C Bootloader/ all

kernel:
	$(MAKE) -C Kernel/ all

userland:
	$(MAKE) -C Userland/ all

image: kernel bootloader userland
	$(MAKE) -C Image/ all

clean:
	$(MAKE) clean -C Bootloader/ 
	$(MAKE) clean -C Image/ 
	$(MAKE) clean -C Kernel/ 
	$(MAKE) clean -C Userland/ 
