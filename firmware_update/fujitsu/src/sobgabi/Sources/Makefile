EXTRA_CFLAGS += -I$(src) -I$(src)/../../_SHARED/Inc $(CFLAGS_DBG) $(FSC_FLAGS) -DKERNELVERSION=\"$(KERNELVERSION)\"


obj-m += sobgabi.o
sobgabi-y := sobgabimain.o dispatch.o gabi.o


# this does not work yet
#$(obj)/sobmain.o: $(src)/../../_SHARED_GPL/Cls/sobmain.c
#	 $(CC) -c $(c_flags) $(CFLAGS_MODULE) $(EXTRA_CFLAGS) $(src)/../../_SHARED_GPL/Cls/sobmain.c
