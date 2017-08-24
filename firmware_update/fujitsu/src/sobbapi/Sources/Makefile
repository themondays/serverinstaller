EXTRA_CFLAGS += -I$(src) -I$(src)/../../_SHARED/Inc $(CFLAGS_DBG) $(FSC_FLAGS) -DKERNELVERSION=\"$(KERNELVERSION)\"


obj-m += sobbapi.o
sobbapi-y := dispatch.o biosapi.o sobmain.o apic.o


# this does not work yet
#$(obj)/sobmain.o: $(src)/../../_SHARED_GPL/Cls/sobmain.c
#	 $(CC) -c $(c_flags) $(CFLAGS_MODULE) $(EXTRA_CFLAGS) $(src)/../../_SHARED_GPL/Cls/sobmain.c
