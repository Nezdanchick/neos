NAME=neos
ARCH=x86_64
#i686 x86_64 aarch64
RAM=64M
#mimimum 64M of ram
LOG_FILE=serial.log
DEPLOY_DEVICE=/dev/sda

EFI=$(OUTDIR)/$(TARGET)
OVMF=lib/ovmf/$(ARCH)/OVMF.fd
FLOPPY=$(OUTDIR)$(NAME).img

TARGET=$(NAME).elf
SRC=src
OUTDIR=bin
IMGDIR=image

INC=src/include
SRCS=$(shell find $(SRC) -name '*.c')

include uefi/Makefile

# shortcuts:
h: help;
b: build;
r: run;
d: deploy;
c: clear;

build: mkdir all
	@dd if=/dev/zero of=$(FLOPPY) bs=1k count=1440 status=none
	@mformat -i $(FLOPPY) -f 1440
	@mmd -i $(FLOPPY) ::efi
	@mmd -i $(FLOPPY) ::efi/boot
	@mcopy -i $(FLOPPY) $(EFI) ::/efi/boot/bootx64.efi
	@mcopy -i $(FLOPPY) $(IMGDIR)/* ::
	@echo $(NAME) image created successfully in $(FLOPPY)

run: build
	@echo running $(NAME) $(ARCH)
	@qemu-system-$(ARCH) \
	-drive file=$(FLOPPY),format=raw,if=none,id=floppy \
	-device virtio-blk-pci,drive=floppy,bootindex=0 \
	-bios $(OVMF) \
	-m $(RAM) \
	-vga std \
	-enable-kvm \
	-audiodev sdl,id=snd0 -machine pcspk-audiodev=snd0 \
	-chardev stdio,id=char0,logfile=$(LOG_FILE),signal=off \
	-serial chardev:char0 \
	-no-reboot

deploy: clear build
	@echo trying to deploy to device $(DEPLOY_DEVICE):
	@sudo fdisk -l $(DEPLOY_DEVICE)
	@sudo -k dd if=$(FLOPPY) of=$(DEPLOY_DEVICE)
	@echo deploy to $(DEPLOY_DEVICE) finished successfully

clear: clean
	@rm -f $(LOG_FILE)

mkdir:
	@rsync -r --include '*/' --exclude '*' $(SRC) $(OUTDIR)
	@cp -r $(INC) $(OUTDIR)/$(SRC)

help:
	@printf "\n$(NAME) help:\n \
	h | help ---> show this message\n \
	r | run ----> run in qemu-system-$(ARCH)\n \
	b | build --> compile and create $(FLOPPY) floppy image\n \
	d | deploy -> build and write floppy image to $(DEPLOY_DEVICE)\n \
	c | clear --> clean project (make clean - remove $(OUTDIR) folder)\n\n\n"