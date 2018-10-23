set arch i386:x86-64:intel
target remote :1234
add-symbol-file Kernel/kernel.elf 0x100000
dir Kernel/
add-symbol-file Userland/0000-sampleCodeModule.elf 0x400000
dir Userland/SampleCodeModule/
b main
c
