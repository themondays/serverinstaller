default:
	#Use modules_installer.sh to compile and install the drivers

install:
	./modules_installer.sh -vbi .

compile:
	./modules_installer.sh -vb .

clean:
	./modules_installer.sh -c .
	rm -f Module.markers modules.order
	rm -f smbios/Sources/modules.order sobproc/Sources/modules.order sobcmos/Sources/modules.order sobbapi/Sources/modules.order sobgabi/Sources/modules.order sobdata/Sources/modules.order

tar:
	./modules_installer.sh -c .
	mkdir -p ../Product/src
	tar -czf ../Product/src/Drivers-src.tgz Makefile COPYING modules_installer.sh _SHARED smbios sob*

obj-y +=  smbios/Sources/ sobbapi/Sources/ sobgabi/Sources/ sobcmos/Sources/ sobdata/Sources/ sobproc/Sources/
