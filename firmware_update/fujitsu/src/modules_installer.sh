#!/bin/sh

#/* Copyright (C) 2013 Fujitsu Technology Solutions
#
#   DeskFlash Modules Installer
#   Program for building and installing the kernel modules needed by deskflash and biosset for linux.
#
#   This script is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License,
#   or (at your option) any later version.
#
#   It is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
#   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
#   for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this file; see the file COPYING. If not, write to the
#   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
#   Boston, MA 02111-1307, USA.
#*/

# configuration variables
if [ -z "$FSC_PREFIX" -o ! -d "$FSC_PREFIX" ] ; then
	FSC_PREFIX=/opt/deskview
fi

POSTFIX_SMBIOS=share/device/lib
POSTFIX_SOB=deskflash/device/lib

if [ -z "$FSC_TEMPDIR" -o ! -d "$FSC_TEMPDIR" ] ; then
	FSC_TEMPDIR=/tmp
fi

# commandline variables
CML_HELP=1
CML_VERSION=0
CML_CLEAN=0
CML_INSTALL=0
CML_BUILD=0
CML_DONTREMOVE=0
CML_VERSION=0
CML_DRIVER_SRC="."
CML_QUIET=""
CML_VERBOSE=""

# misc ... just internal
# TYPE=debug
TYPE=release
DEBUG=0
WRITE_RIGHTS_WARNING=0
CLEAN_TMPDIR=0
IN_DIR=0
THIS=`basename $0`
Version=0.05


writerightswarning() {
	debug "writerightswarning" "entered"
	if [ "$WRITE_RIGHTS_WARNING" = "0" ] ; then
		WRITE_RIGHTS_WARNING=1
		MYecho
		MYecho "You need write rights for the following directories to be"
		MYecho "able to install the modules."
		MYecho
		MYecho "Directories:"
	fi
	debug "writerightswarning" "leave"
}

help() {
	debug "help" "entered"
	MYecho
#	MYecho "$1 -hcbirqvOV container"
	MYecho "$1 -hcbirqvOV"
	MYecho
	MYecho "-h	display this help."
	MYecho "-V	display the version of this script."
	MYecho "-c	clean the modules folder before building respectively installing the modules."
	MYecho "-b	build the modules."
	MYecho "-i	after building the modules, they will be copied to the right folders."
	MYecho "-r	don't remove the extracted files after build and installing."
	MYecho "-q	do it all absolute quiet."
	MYecho "-v	give me excessive output!"
#	MYecho "container	the source container is either the FSC tar.gz or the \"Drivers\"-folder extracted from the tar.gz"
	MYecho
	MYecho
	MYecho "Environment Variable(s) (optional):"
	MYecho "  FSC_PREFIX	this is the folder where the folder structure of deskflash for Linux begins (default: /opt/deskview)"
#	MYecho "  FSC_TEMPDIR	if the drivers source container is the FSC tar.gz then it will be extracted to there (default: /tmp)"
	MYecho
	MYecho "possible Errorlevels of this script are:"
	MYecho "0 = all went fine"
	MYecho "1 = during clean, something went wrong"
	MYecho "2 = during build, something went wrong"
	MYecho "3 = during copying the modules, something went wrong"
	MYecho "4 = you don't have write rights at the install location (default: /opt/deskview, unless FSC_PREFIX is set)"
	MYecho
	debug "help" "leave"
}

version_n_copyrights() {
	debug "version_n_copyrights" "entered"
	MYecho
	MYecho  "DeskFlash Modules Installer version $Version"
	MYecho  "Program for building and installing the modules needed by deskflash for linux."
	MYecho  "Copyright (C) 2013 Fujitsu Technology Solutions GmbH"
	MYecho
	debug "version_n_copyrights" "leave"
}

MYcopy() {
	if [ -z "$CML_QUIET" -a -n "$CML_VERBOSE" ] ; then
		cp $@
	else
		cp $@ 2>&1 1>/dev/null
	fi
	return $?
}

MYecho() {
	if [ -z "$CML_QUIET" ] ; then 
		echo $@ 
	fi
}
debug() {
	if [ "$DEBUG" = "1" ] ; then
		FUNC=$1
		shift
		MSG=$1
		shift
		MYecho -n "[$FUNC] - $MSG"
		if [ "X$1X" != "XX" ] ; then
			MYecho " - Parameter:"
			COUNT=0
			while [ "X$1X" != "XX" ] ; do
				COUNT=$(($COUNT+1))
				MYecho "	$COUNT: '$1'"
				shift
			done
		else
			MYecho
		fi
	fi
}

validateDir() {
	debug "validateDir" "entered"
	if [ ! -d $1 ] ; then
		DIR=`dirname $1`
		while [ ! -d $DIR ] ; do
			DIR=`dirname $DIR`
		done

		if [ ! -w $DIR ] ; then
			writerightswarning
			MYecho $DIR
		else
			mkdir -p $1
		fi
	fi

	if [ ! -w $1 ] ; then
		writerightswarning
		MYecho $1
	fi
	debug "validateDir" "leave"
}

clean() {
	debug "clean" "entered"
	MYecho "cleaning the drivers sources"
	make -C /lib/modules/`uname -r`/build M=`pwd` clean
	rm -f Modules.symvers modules.order Module.markers 2>&1 1>/dev/null
	rm -f */Sources/modules.order
	debug "clean" "leave"
	return 0
}

build() {
	debug "generate" "entered"
	MAKE="make -C /lib/modules/`uname -r`/build M=`pwd`"
	if [ "$2" = "release" -o "$2" = "debug" ] ; then
		type=$2
	else
		type="release"
	fi

	ret=0
	if [ -d $1 ] ; then
		MYecho "building the drivers"

		if [ "$2" = "debug" ]; then
		    MAKE="$MAKE CFLAGS_DBG='-D_DEBUG_'"
		    export CONFIG=DEBUG
		elif [ "$1" = "release" -o "$2" = "release" ]; then
		    export CONFIG=RELEASE
		fi

		${MAKE} FSC_FLAGS=$FSC_FLAGS
		ret=$?
		if [ $ret -ne 0 ] ; then
			debug "generate" "build reported an error" "error: $ret"
		fi

		MYecho "done"
	fi
	debug "generate" "leave"
	return $ret
}

parseparams() {
	debug "parseparams" "entered"
	OPTERR=1
	while getopts "cibhrvqVO" ARGV ; do
		debug "main" "parameter $ARGV recognized"
		case $ARGV in
			h) CML_HELP=1;;
			c) CML_CLEAN=1;;
			b) CML_BUILD=1;;
			i) CML_INSTALL=1;;
			r) CML_DONTREMOVE=1;;
			v) CML_VERBOSE=1;;
			V) CML_VERSION=1;;
			q) CML_QUIET=1;;
			*) help $0;;
		esac
	done
	if [ "$CML_INSTALL" = "1" -o "$CML_BUILD" = "1" -o "$CML_CLEAN" = "1" ] ; then
		CML_HELP=0
	fi

#	eval "PARAM=\${${OPTIND}}"
#	debug "main" "checking the driver source container" "$PARAM"
#	if [ -f "$PARAM" -a "$PARAM = ${PARAM%%.tar.gz}" ] ; then
#			# we found a tar.gz
#			debug "main" "we got a tar.gz" "${PARAM}"
#			CML_DRIVER_SRC=${PARAM}
	
	# if the basename is Drivers we expect that this is the extracted driver source
#	elif [ -d "$PARAM" ] ; then
#			if [ "${PARAM:${#PARAM}-1}" = "/" ] ; then
#				debug "main" "removing last slash" "${PARAM}" "${PARAM:0:${#PARAM}-1}"
#				PARAM=${PARAM:0:${#PARAM}-1}
#			fi
#			debug "main" "we got a Directory" "${PARAM}"
#			CML_DRIVER_SRC=${PARAM}
#	fi
	debug "parseparams" "leave"
}

main() {
	debug "main" "entered"

	parseparams $@

	if [ "$CML_HELP" = "1" -o "$CML_VERSION" = "1" ] ; then
		CML_QUIET=""
	fi

	if [ "$CML_VERSION" = "1" ] ; then
		MYecho $Version
		return 0
	fi

	version_n_copyrights

	if [ "$CML_HELP" = "1" ] ; then
		debug "main" "calling help sub"
		help $THIS
	else
		if [ "$CML_INSTALL" = "1" ] ; then
			MYecho "validating destination folders..."
			debug "main" "calling validateDir sub" "$FSC_PREFIX/$POSTFIX_SMBIOS"
			validateDir $FSC_PREFIX/$POSTFIX_SMBIOS
			debug "main" "calling validateDir sub" "$FSC_PREFIX/$POSTFIX_SOB"
			validateDir $FSC_PREFIX/$POSTFIX_SOB
#			debug "main" "calling validateDir sub" "$FSC_TEMPDIR"
#			validateDir $FSC_TEMPDIR

			if [ "$WRITE_RIGHTS_WARNING" = "1" ] ; then 
				MYecho "failed"
			else
				MYecho "done"
			fi
		fi

		if [ ! "$WRITE_RIGHTS_WARNING" = "1" -o "$CML_INSTALL" = "0" ] ; then 
			debug "main" "going into build line" "$CML_DRIVER_SRC"
			if [ "$TYPE" = "" ] ; then
				TYPE="release"
			fi

			if [ "$CML_CLEAN" = "1" ] ; then
				if [ -n "$CML_DRIVER_SRC" ] ; then
					if [ ! -d "$CML_DRIVER_SRC" ] ; then
						debug "main" "doing no clean, because of tar.gz source container" "$CML_DRIVER_SRC"
						MYecho "We got the FSC tar.gz as Source container, so no need to clear anything."
					else
						debug "main" "calling clean sub" "$CML_DRIVER_SRC"
						clean $CML_DRIVER_SRC
						ret=$?
						if [ $ret -ne 0 ] ; then
							debug "main" "clean sub returned an error" "error: $ret"
							return 1
						fi
					fi
				else
					MYecho
					debug "main" "could not clean, no driver source container given" "$CML_DRIVER_SRC"
					MYecho "ERROR: could not clean, no driver source container given"
					help $THIS
					CML_BUILD=0
					CML_INSTALL=0
					CLEAN_TMPDIR=0
				fi
			else
				debug "main" "clean not wanted or no driver source container given" "$CML_DRIVER_SRC"
			fi

			if [ -f "$CML_DRIVER_SRC" ] ; then
				MYecho "extracting driver sources to $FSC_TEMPDIR"
				tar -xzf $CML_DRIVER_SRC -C $FSC_TEMPDIR
				debug "main" "extracted" "$CML_DRIVER_SRC" "$FSC_TEMPDIR" "tar return: $?"
				if [ "$?" = "0" ] ; then
					CLEAN_TMPDIR=1
					CML_DRIVER_SRC=$FSC_TEMPDIR/Drivers
					debug "main" "new driver source container" "$CML_DRIVER_SRC"
					MYecho "done"
				else
					MYecho
					debug "main" "extraction failed." "$CML_DRIVER_SRC"
					MYecho "ERROR: the drivers source tar.gz could not be extracted to $FSC_TEMPDIR"
					help $THIS
					CML_BUILD=0
					CML_INSTALL=0
					CLEAN_TMPDIR=0
				fi
			fi

			if [ "$CML_BUILD" = "1" ] ; then
				if [ -n "$CML_DRIVER_SRC" -a -d "$CML_DRIVER_SRC" ] ; then
					debug "main" "calling generate sub" "$CML_DRIVER_SRC"
					build $CML_DRIVER_SRC $TYPE
					ret=$?
					if [ $ret -ne 0 ] ; then
						debug "main" "build returned an error" "error: $ret"
						return 2
					fi
				else
					MYecho
					debug "main" "could not build, no driver source container given" "$CML_DRIVER_SRC"
					MYecho "ERROR: could not build, no driver source container given"
					help $THIS
					CML_INSTALL=0
				fi
			else
				debug "main" "build not wanted"
			fi

			if [ "$CML_INSTALL" = "1" ] ; then
				if [ -n "$CML_DRIVER_SRC" -a -d "$CML_DRIVER_SRC" ] ; then
					if [ -e $CML_DRIVER_SRC/sobbapi/Sources/sobbapi.ko ] ; then
						MYecho "copying the drivers"
						for i in $CML_DRIVER_SRC/s*/Sources/*.ko ; do
							if [ `basename $i` = "smbios.ko" ] ; then
								MYcopy -v $i $FSC_PREFIX/$POSTFIX_SMBIOS
							else	
								MYcopy -v $i $FSC_PREFIX/$POSTFIX_SOB
							fi
							ret=$?
							if [ $ret -ne 0 ] ; then
								debug "main" "copy returned an error" "error: $ret"
								return 3
							fi
						done
						MYecho "done"
					else
						MYecho
						debug "main" "no modules to be copied, forgot to build?" "$CML_DRIVER_SRC"
						MYecho "WARNING: no modules to be copied, forgot to build?"
						help $THIS
					fi
				else
					MYecho
					debug "main" "could not install, no driver source container given" "$CML_DRIVER_SRC"
					MYecho "ERROR: could not install, no driver source container given"
					help $THIS
				fi
			fi
			if [ "$CLEAN_TMPDIR" = "1" -a -d "$FSC_TEMPDIR/Drivers" ] ; then
				if [ "$CML_DONTREMOVE" = "0" ] ; then
					MYecho "removing temporary folder $FSC_TEMPDIR/Drivers"
					rm -r $FSC_TEMPDIR/Drivers
					MYecho "done"
				else
					MYecho "The extracted files will not be removed, as you requested."
					MYecho "You are responsible to removing it, if they are not needed anymore."
					MYecho "They are placed: $FSC_TEMPDIR/Drivers"
				fi
			fi
		else
			MYecho
			return 4
		fi
	fi
	debug "main" "leave"
	return 0
}

main $@
exit $?
