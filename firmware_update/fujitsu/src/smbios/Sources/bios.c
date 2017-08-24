/* Copyright (C) 2001-2014 Fujitsu Technology Solutions
   Joachim Braeuer
   This file is part of smbios

   smbios is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License,
   or (at your option) any later version.

   smbios is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.

   You should have received a copy of the GNU General Public License
   along with smbios; see the file COPYING. If not, write to the
   Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/


/** \file bios.c
 *  DMI-BIOS and SM-BIOS access functions
 *  The functions defined in this file provide low level access to the BIOS
 *  area that is comform to the DMI BIOS Specification or the SM-BIOS
 *  specification, respectively.
 *  
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *	\author Joachim Braeuer
 *	\author Rainer König (refactoring for Linux kernels >= 3.10
 *
 *  \version 0.12
 *  \date January 2014
 */

#ifndef __KERNEL__
#  define __KERNEL__
#endif
#ifndef MODULE
#  define MODULE
#endif

#include <linux/module.h>

#include <linux/kernel.h>	/* ... for 'printk()' */
#include <linux/errno.h>	/* ... error codes */
#include <linux/types.h>	/* ... fixed size types definitions, '__u8'... */
#include <linux/proc_fs.h>	/* ... for 'struct proc_dir_entry' */
#include <linux/string.h>	/* ... for 'strncmp()' */
#include <linux/slab.h>		/* ... for kmalloc, kfree */
#include <linux/version.h>	/* ... conditional compilation for different kernels because of */
							/*     changed proc_fs ABI in Kernel 3.10 */

#include "strgdef.h"        /* ... contains the string definitions for the cooked mode */
#include "bios.h"		    /* ... local declarations for DMI-, SM-BIOS */
#include "cooking.h"	    /* ... local declarations for interpreting DMI- and SM-BIOS types */

// Kernels before 3.7 will produce strange effects when using copy_to_user
// so we definde a safe_memcpy function instead that works on all kernels

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)

#define safe_memcpy(dest,src,count) memcpy(dest,src,count)

#else

#include <linux/uaccess.h>    /* ... for copy_to_user */
#define safe_memcpy(dest,src,count) copy_to_user(dest,src,count)

#endif /* Kernel Version < 3.7.0 */

// CT 07.12.05
// EXPORT_NO_SYMBOLS;
// CT 07.12.05

/*
 *  Global data
 */

/** SM-BIOS, resp. DMI-BIOS /proc/smbios directory */
struct proc_dir_entry *smbios_proc_dir = NULL;
/** SM-BIOS, resp. DMI-BIOS /proc/smbios/raw directory */
struct proc_dir_entry *smbios_raw_proc_dir = NULL;
/** SM-BIOS, resp. DMI-BIOS /proc/smbios/cooked directory */
struct proc_dir_entry *smbios_cooked_proc_dir = NULL;
/** SM-BIOS, resp. DMI-BIOS virtual base address of F-Segment */
void *smbios_base = 0;
/** SM-BIOS entry point structure */
smbios_entry_point_struct *smbios_entry_point = NULL;
/** DMI-BIOS entry point structure */
dmibios_entry_point_struct *dmibios_entry_point = NULL;
/** SM-BIOS, resp. DMI-BIOS structures base address; starting point */
void *smbios_structures_base = NULL;
/** enumeration of SM-BIOS, resp. DMI-BIOS types that do have subtypes */
__u8 smbios_types_with_subtypes[] = { TYPES_WITH_SUBTYPES };
/** contains the SMBIOS Version, e.g. V2.31 */
char smbios_version_string[32];



/* Need to store file opeations globally for things to work */

struct file_operations smbios_version_fops = {
    read : smbios_version_proc
};
struct file_operations bios_read_raw_proc_fops = {
		read : bios_read_raw_proc
};

struct file_operations bios_read_cooked_proc_fops = {
		read : bios_read_cooked_proc
};

/**Private filename store because proc_fs structure went privat in Linux 3.10 */

struct used_proc_filenames {
	char filename[64];                 /* Ugly but quick, just use 64bytes for filenames */
	struct used_proc_filenames *next;
};

/* problem is that the filenames can occur in different directories */
/* so we keep a list on the directories where they go */

struct proc_dir_list {
   struct proc_dir_entry *entry;  /* we keep a pointer to the dir */
   struct used_proc_filenames *filename_store;
   struct proc_dir_list *next;
};

struct proc_dir_list *proc_dir_cache = NULL; /* Hook for the dir_cache */

/*
 *  Functions
 */

/** \fn int register_proc_filename (struct proc_dir_entry *dir, char *newfilename)
  * \brief enters a new filename in our little filename cache
  * \param dir : pointer to directory structure
  * \param newfilename : filename to be registered
  * \return integer if good (==0) or failed
  *
  * this function starts to lookup for the directory cache and then adds the
  * filename to the list
  *
  * \author Rainer König
  * \date January 2014 for refactoring for Kernel >= 3.10
  */

int register_proc_filename (struct proc_dir_entry *dir, char *newfilename)
{
    struct proc_dir_list *dir_pointer; /* Pointer to search for the correct directory */
    struct used_proc_filenames *filename_pointer = NULL;

    dir_pointer = proc_dir_cache;

    /* Search for the directory structure */
    /* at the dir_pointer is pointing to the structure or is NULL (not found) */
    while ((dir_pointer) && (dir_pointer->entry != dir))
    		dir_pointer = dir_pointer->next;

    if (!dir_pointer)  /* we need to add a new directory to the list */
    {
    	dir_pointer = kmalloc(sizeof(struct proc_dir_list), GFP_KERNEL);
    	if (!dir_pointer)
    	{
    		printk(KERN_INFO "kmalloc for dir_pointer failed!");
    	    return (-ENOMEM);
    	}
    	dir_pointer->entry = dir;
    	dir_pointer->filename_store = NULL;
    	dir_pointer->next = proc_dir_cache;
    	proc_dir_cache = dir_pointer;
    }

    /* At this point dir_pointer should point to the correct cache structure */
    filename_pointer = kmalloc(sizeof (struct used_proc_filenames), GFP_KERNEL);
    if (!filename_pointer)
    {
    	printk(KERN_INFO "kmalloc for filename_pointer failed!");
    	return (-ENOMEM);
    }

    /* Copy the filename to our list of used_filenames */
    memcpy(filename_pointer->filename, newfilename, strlen(newfilename));

    /* Link the list */
    filename_pointer->next = dir_pointer->filename_store;
    dir_pointer->filename_store = filename_pointer;

	return (0);
}

/* fn int count_filename_instances (struct proc_dir_entry *dir, char *filenametolookfor)
 * \brief counts the occurences of a file name in the dir directory
 * \param dir : pointer to the directory structure (yes, ugly we just compare pointers)
 * \param filenametolookfor : The filename we are looking for (without instance-number)
 * \return count of instances (0..n)
 *
 * \author Rainer König
 * \date   January 2014 for refactoring for Kernel-Vesions >= 3.10
 */

int count_filename_instances (struct proc_dir_entry *dir, char *filenametolookfor)
{
	int count = 0;
	struct proc_dir_list *dir_pointer; /* Pointer to search for the correct directory */
	struct used_proc_filenames *filename_pointer;

	dir_pointer = proc_dir_cache;

	/* Search for the directory structure */
	    /* at the dir_pointer is pointing to the structure or is NULL (not found) */
	while ((dir_pointer) && (dir_pointer->entry != dir))
		dir_pointer = dir_pointer->next;

	if(!dir_pointer)
	{
		/* did not find the directory, this should not happen */
		return(0);
	}

	filename_pointer = dir_pointer->filename_store;

//	printk(KERN_INFO "Searching for filename %s %u %u", filenametolookfor, dir_pointer, filename_pointer);

	while(filename_pointer)
	{
		/* Compare just for the length of filenametolookfor */
//		printk(KERN_INFO "Filename to compare %s", filename_pointer->filename);
		if (!(strncmp(filename_pointer->filename,filenametolookfor,strlen(filenametolookfor))))
			count++;

		filename_pointer = filename_pointer->next;
	}

	return(count);
}

/** \fn void release_dir_list (struct proc_dir_entry *dir)
  * \brief releases all allocated memory in our directory cache
  * \param dir : Pointer to dir structure
  *
  * \author Rainer König
  * \date   Januar2014 for refactoring for Kernels >= 3.10
 */

void release_dir_list (struct proc_dir_entry *dir)
{
	struct proc_dir_list *dir_pointer, *before_dir_pointer; /* Pointer to search for the correct directory */
	struct used_proc_filenames *filename_pointer, *next_filename_pointer;

	dir_pointer = proc_dir_cache;
	before_dir_pointer = NULL;

	/* Search for the directory structure */
	/* at the dir_pointer is pointing to the structure or is NULL (not found) */
	while ((dir_pointer) && (dir_pointer->entry != dir))
	{
		before_dir_pointer = dir_pointer;
		dir_pointer = dir_pointer->next;
	}

	if (!dir_pointer)
		return ; /* directory pointer not found in tree, nothing to do */

	filename_pointer = dir_pointer->filename_store;

	while(filename_pointer)
	{
		/* This might look weird, but we need to store the next pointer before freeing the memory */
		next_filename_pointer = filename_pointer->next;
		kfree(filename_pointer);
		filename_pointer = next_filename_pointer;
	}

	/* Now release also the dir_list_entry */
	if (dir_pointer == proc_dir_cache)
	{
		/* first element in list is to be freed, so move the list anchor */
		proc_dir_cache = dir_pointer->next;
	}
	else
	{   /* link the entry before to the next one */
		before_dir_pointer->next = dir_pointer->next;
	}
	kfree(dir_pointer);
}

/** \fn smbios_entry_point_struct * smbios_find_entry_point (void *base)
  * \brief find the SM-BIOS entry point structure 
  * \param base virtual base address of physical F-segment address 
  * \return pointer to the SM-BIOS entry point structure
  *
  * this function searches the BIOS F-segement (physical address 0x000f0000
  * to 0x000fffff) for the SM-BIOS entry point signature '_SM_'.
  *
  * \author Markus Lyra
  * \author Thomas Bretthauer
  * \date October 2000
  */
/* returns offset from start value; values < 0 mean failure */
int smbios_find_entry_point (void *base)
{
    char *start = (char *)base;
    char *temp;
    int rv = -1;
    
    /* search for the magic dword - '_SM_� as DWORD formatted -  on paragraph boundaries */
    for (temp = base; temp < start + BIOS_MAP_LENGTH; temp += sizeof(__u32))  // struct is 4 byte aligned
    {
        /* found the identifier ? */
        if (*(__u32*)temp == SMBIOS_MAGIC_DWORD)
        {
            /* check if entry point valid (build checksum) */
            if (!(smbios_check_entry_point (temp)))
            {
            	struct smbios_entry_point_struct *p = (smbios_entry_point_struct*)temp;
                /* fix display of Bios version string *
                 * SMBios version is known as 2.1, 2.2, 2.3 and 2.3.1, never as 2.01 (JB) */
                PDEBUG ("SM-BIOS V%d.%d entry point found at 0x%p\n", p->major_version, p->minor_version, p);
                sprintf(smbios_version_string, "V%d.%d\n", p->major_version, p->minor_version);
                rv = temp - start;
                break;
            }
        }
    }
    return rv;
}


/** \fn dmibios_entry_point_struct * dmibios_find_entry_point (void *base)
  * \brief find the DMI-BIOS entry point structure
  * \param base virtual base address of physical F-segment address
  * \return pointer to the DMI-BIOS entry point structure
  *
  * this function searches the BIOS F-segment (physical address 0x000f0000
  * to 0x000fffff) for the DMI-BIOS entry point signature '_DMI20_NT_'.
	* dmibios_find_entry_point only gets called if we couldn't find a SMBios.
  *
  * \author Thomas Bretthauer
  * \date November 2000
  */

dmibios_entry_point_struct *
dmibios_find_entry_point (void *base)
{
    dmibios_entry_point_struct *entry_point = NULL;	    /** DMI-BIOS entry point */
    unsigned char *temp = NULL;			                /** temp. pointer        */
    unsigned char biossignature[] =		                /** '_DMI20_NT_'         */
                { 0x5f, 0x44, 0x4d, 0x49, 0x32, 0x30, 0x5f, 0x4e, 0x54, 0x5f };

    /* search for the DMI-BIOS signature on character boundary (hm?) */
    for (temp = base;
	       !entry_point && 
				 temp < (__u8 *) base + BIOS_MAP_LENGTH - sizeof (biossignature) - 32;
	       temp++)
	{
        unsigned long *tempdword = (unsigned long *) temp;

        /* found the identifier '_DMI' ?     (beginning of signature) */
        if ((unsigned int)*tempdword == DMIBIOS_MAGIC_DWORD)
        {
	        entry_point = (dmibios_entry_point_struct *) temp;
	
	        PDEBUG ("DMI-BIOS revision %d entry point at 0x%p\n",
		    entry_point->revision, (void*) temp);

            sprintf(smbios_version_string, "V%d\n", entry_point->revision);

	        if (memcmp (temp, biossignature, sizeof (biossignature)) == 0)
	            PDEBUG ("DMI BIOS successfully identified\n");
        }
    }
    return entry_point;
}


/** \fn unsigned char smbios_check_entry_point (void * addr)
 *  \brief checks the entry point structure for correct checksum
 *  \param addr pointer to the entry point structure
 *  \return the checksum of the entry point structure, should be '0'
 *
 *  This function checks the entry point structure for correct checksum.
 *  The checksum is calculated with adding every byte of the structure
 *  to the checksum byte. The entry point structure is considered correct
 *  if the checksum byte is 0.
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */

unsigned char
smbios_check_entry_point (void *addr)
{
    unsigned char *i;
    unsigned char checksum = 0;
    unsigned char length =
      ((smbios_entry_point_struct *) addr)->entry_point_length;


    /* calculate checksum for entry point structure (should be 0) */
    for (i = (unsigned char *) addr; i < (unsigned char *) addr + length; i++)
        checksum += *i;

    return checksum;
}


/** \fn int smbios_type_has_subtype (unsigned char type)
 *  \brief returns whether the specified type does have subtypes
 *  \param type type which is checked for having subtypes
 *  \return 1 if the specified type does have subtypes, 0 otherwise
 * 
 *  This function returns 1 if the specified subtype does have subtypes and 0
 *  if the the specified does not have subtypes. The global array
 *  smbios_types_with_subtypes is searched for the type. The array is
 *  filled statically and must explicitly be modified if new types are 
 *  introduced which have subtypes.
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */

int
smbios_type_has_subtype (unsigned char type)
{
    int i;


    for (i = 0; i < sizeof (smbios_types_with_subtypes); i++)
        if (type == smbios_types_with_subtypes[i])
	        return 1;

    return 0;
}


/** \fn int smbios_get_struct_length (smbios_struct * struct_ptr)
 *  \brief returns the length of the specified SM-BIOS structure 
 *  \param struct_ptr pointer to a SM-BIOS structure
 *  \return length of the structure including optional strings
 *
 *  This function calculates the length of the specified SM-BIOS structure
 *  including any string following the structure.
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */

long
smbios_get_struct_length (smbios_struct * struct_ptr)
{
    /* jump to string list */
    unsigned char *ptr = (unsigned char *) struct_ptr + struct_ptr->length;

    /* search for the end of string list */
    while (ptr[0] != 0x00 || ptr[1] != 0x00)
        ptr++;
    ptr += 2;			/* terminating 0x0000 should be included */

    return (long) ptr - (long) struct_ptr;
}


/** \fn int dmibios_get_struct_length (smbios_struct * struct_ptr)
 *  \brief returns the length of the specified DMI-BIOS structure 
 *  \param struct_ptr pointer to a DMI-BIOS structure
 *  \return length of the structure including optional strings
 *
 *  This functions calculates the length of the specified DMI-BIOS structure
 *  including any string following the structure.
 *
 *  \author Thomas Bretthauer
 *  \date November 2000
 */
int
dmibios_get_struct_length (smbios_struct * struct_ptr)
{
    int len = struct_ptr->length;

    /* jump to the string list */
    unsigned short *ptr = (unsigned short *) ((unsigned char *) struct_ptr + len);

    if (*ptr == 0)
        len++;

    /* search for the end of the string list */
    while (*ptr != 0)
    {
        unsigned char *string;
        int stringlen;

        string = (unsigned char *) smbios_base + *ptr;
        stringlen = strlen (string) + 1;
        len += stringlen;
        ptr++;
    }

    return len + 1;
}


/** \fn int bios_read_raw__proc (char *page, char **start, off_t off,
 *                                 int count,int *eof, void *data)
 *  \brief called by the kernel whenever a raw proc file is read by an application
 *
 *  \param page
 *  \param start
 *  \param off
 *  \param count
 *  \param eof
 *  \param data
 *  \return bytes returned
 *
 *  This function is called by the kernel whenever an application opens
 *  a file in the proc file system created by this driver.
 *  This routine returns the appropriate data "in the file", i.e. the
 *  binary data of the corresponding SM-BIOS structure.
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \author Rainer König
 *  \date October 2000
 *  \date January 2014 refactored for Kernel 3.10 dependencies
 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

int bios_read_raw_proc (char *page, char **start, off_t off,
		  int count, int *eof, void *data)
{
    int length = smbios_get_struct_length ((smbios_struct *) data);

    /* this function is called by the kernel if a file is read, */
    /* page points to a free page allocated by kernel,          */
    /* data points to the structure associated with the file    */

    if (off >= length)
    {
        *eof = 1;

        return 0;
    }

    if (off + count > length)
    {
        *eof = 1;

        count = length - off;
    }

    /* don't return more than PROC_BLOCK_SIZE bytes */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;

    safe_memcpy (page, data + off, count);

    *start = page;

    return count;
}
#else /* LINUX_VERSION_CODE >= 3.10 from now on */

/* Kernel 3.10 introduced a different ABI for reading proc entries */
/* I assume that *offp holds an offset and that should be updated by this function */
/* Since there is no EOF variable the function should return 0 in case of EOF */

ssize_t bios_read_raw_proc (struct file *filp, char *page, size_t count, loff_t *offp)
{
	void *data;
	int length;
	data = PDE_DATA(file_inode(filp));
	if(!(data))
	{ printk(KERN_INFO "Null data");
	  return (0);
	}
	length = smbios_get_struct_length ((smbios_struct *) data);
	if (*offp >= length) /* EOF detection */
	{ return (0);
	}
	if (*offp + count > length) /* more wanted than there is left */
	{
		count = length - *offp;
	}
    /* don't return more than PROC_BLOCK_SIZE bytes */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;
    safe_memcpy(page, data + *offp, count);
    *offp+=count; /* shift offp to new position */
    return(count);
}
#endif

/** \fn int dmibios_read_raw_proc (char *page, char **start, off_t off,
 *                                 int count,int *eof, void *data)
 *  \brief called by the kernel whenever a proc file is read by an application
 *  \param page
 *  \param start
 *  \param off
 *  \param count
 *  \param eof
 *  \param data
 *  \return bytes returned
 *
 *  This function is called by the kernel whenever an application opens
 *  a file in the proc file system created by this driver.
 *  This routine returns the appropriate data "in the file", i.e. the
 *  binary data of the corresponding DMI-BIOS structure.
 *
 *  \author Thomas Bretthauer
 *  \author Rainer König
 *  \date November 2000
 *  \date January 2014 refactored for Kernel dependencies
 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

int dmibios_read_raw_proc (char *page, char **start, off_t off,
		   int count, int *eof, void *data)
/************************************************************************************
    Changes:    28.10.2001, PT ($$1): replaced GFP_BUFFER by GFP_KERNEL for
                SuSE 7.3 compatibility
******************************************+*****************************************/
{
    /* this function is called by the kernel if a file is read, */
    /* page points to a free page allocated by kernel,          */
    /* data points to the structure associated with the file    */

    unsigned char *scratch;
    unsigned short *stringentry;
    unsigned long bytestoadd;


    int length = dmibios_get_struct_length ((smbios_struct *) data);

    /* build a scratch buffer to simulate a SM-BIOS structure */
    scratch = kmalloc (length + 1, GFP_KERNEL); // ($$1)
    if (scratch == NULL)
    {
        *eof = 1;

        return 0;
    }

    /* copy the DMI structure into this scratch buffer */
    stringentry = (unsigned short *) ((unsigned char *) data +
			  ((smbios_struct *) data)->length);
    bytestoadd = ((smbios_struct *) data)->length;

    memcpy (scratch, data, bytestoadd);

    if (*stringentry == 0)
    {
        unsigned char stringtermination = 0;

        memcpy (&scratch[bytestoadd], &stringtermination,
	      sizeof (stringtermination));
        bytestoadd += sizeof (stringtermination);
    }

    while (*stringentry != 0)
    {
        unsigned char *string;
        int stringlen;

        string = (unsigned char *) smbios_base + *stringentry;
        stringlen = strlen (string) + 1;
        memcpy (&scratch[bytestoadd], string, stringlen);
        bytestoadd += stringlen;
        stringentry++;
    }
    {
        unsigned char stringtermination = 0;

        memcpy (&scratch[bytestoadd], &stringtermination,
	      sizeof (stringtermination));
    }

    if (off >= length)
    {
        *eof = 1;

        return 0;
    }

    if (off + count > length)
    {
        *eof = 1;

        count = length - off;
    }

    /* don't return more than PROC_BLOCK_SIZE bytes! */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;

    safe_memcpy (page, scratch + off, count);

	kfree(scratch);

    *start = page;

    return count;
}
#else /* LINUX_VERSION_CODE >= 3.10 from now on */

/* Kernel 3.10 introduced a different ABI for reading proc entries */
/* I assume that *offp holds an offset and that should be updated by this function */
/* Since there is no EOF variable the function should return 0 in case of EOF */

ssize_t dmibios_read_raw_proc (struct file *filp, char *page, size_t count, loff_t *offp)
{
	void *data;
	int length;

    unsigned char *scratch;
    unsigned short *stringentry;
    unsigned long bytestoadd;

	data = PDE_DATA(file_inode(filp));
	if(!(data))
	{ printk (KERN_INFO "Null data");
	  return (0);
	}
    /* this function is called by the kernel if a file is read, */
    /* page points to a free page allocated by kernel,          */
    /* data points to the structure associated with the file    */

    length = dmibios_get_struct_length ((smbios_struct *) data);

    /* build a scratch buffer to simulate a SM-BIOS structure */
    scratch = kmalloc (length + 1, GFP_KERNEL); // ($$1)
    if (scratch == NULL)
    {
        return 0;
    }
    /* copy the DMI structure into this scratch buffer */
    stringentry = (unsigned short *) ((unsigned char *) data +
			  ((smbios_struct *) data)->length);
    bytestoadd = ((smbios_struct *) data)->length;

    memcpy (scratch, data, bytestoadd);

    if (*stringentry == 0)
    {
        unsigned char stringtermination = 0;

        memcpy (&scratch[bytestoadd], &stringtermination,
	      sizeof (stringtermination));
        bytestoadd += sizeof (stringtermination);
    }

    while (*stringentry != 0)
    {
        unsigned char *string;
        int stringlen;

        string = (unsigned char *) smbios_base + *stringentry;
        stringlen = strlen (string) + 1;
        memcpy (&scratch[bytestoadd], string, stringlen);
        bytestoadd += stringlen;
        stringentry++;
    }
    {
        unsigned char stringtermination = 0;

        memcpy (&scratch[bytestoadd], &stringtermination,
	      sizeof (stringtermination));
    }
	if (*offp >= length) /* EOF detection */
	{ return (0);
	}
	if (*offp + count > length) /* more wanted than there is left */
	{
		count = length - *offp;
	}
    /* don't return more than PROC_BLOCK_SIZE bytes */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;
    safe_memcpy(page, scratch + *offp, count);
    kfree(scratch);
    *offp+=count; /* shift offp to new position */
    return(count);
}
#endif

/** \fn int smbios_version_proc (char *page, char **start, off_t off,
 *                                    int count,int *eof, void *data)
 *  \brief called by the kernel whenever a proc file is read by an application
 *  \param page
 *  \param start
 *  \param off
 *  \param count
 *  \param eof
 *  \param data
 *  \return bytes returned
 *
 *  This function is called by the kernel whenever an application opens
 *  a file in the proc file system created by this driver.
 *  This routine returns the appropriate data "in the file", i.e. the
 *  interpreted data of the corresponding DMI-BIOS structure.
 *
 *  \author Thomas Bretthauer
 *  \author Rainer König
 *  \date January 2001
 *  \data January 2014 refactoring for Kernel dependencies
 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

int smbios_version_proc (char *page, char **start, off_t off,
		   int count, int *eof, void *data)
{
    /* this function is called by the kernel if a file is read, */
    /* page points to a free page allocated by kernel,          */
    /* data points to the structure associated with the file    */

    unsigned char *scratch;
	unsigned int length;


    /* build the buffer with the information to present */
    scratch = (char *) data;
    length = strlen (scratch);

    if (scratch == NULL)
    {
        *eof = 1;

        return 0;
    }

    if (off >= length)
    {
        *eof = 1;

        return 0;
    }

    if (off + count > length)
    {
        *eof = 1;

        count = length - off;
    }

    /* don't return more than PROC_BLOCK_SIZE bytes! */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;

    safe_memcpy(page, scratch + off, count);

    *start = page;

    return count;
}
#else /* LINUX_VERSION_CODE >= 3.10 from now on */

/* Kernel 3.10 introduced a different ABI for reading proc entries */
/* I assume that *offp holds an offset and that should be updated by this function */
/* Since there is no EOF variable the function should return 0 in case of EOF */

ssize_t smbios_version_proc (struct file *filp, char *page, size_t count, loff_t *offp)
{
	void *data;
	int length;
	data = PDE_DATA(file_inode(filp));
	if(!(data))
	{ printk (KERN_INFO "Null data");
	  return (0);
	}
	length = strlen(data);
	if (*offp >= length) /* EOF detection */
	{ return (0);
	}
	if (*offp + count > length) /* more wanted than there is left */
	{
		count = length - *offp;
	}
    /* don't return more than PROC_BLOCK_SIZE bytes */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;
    safe_memcpy(page, data + *offp, count);
    *offp+=count; /* shift offp to new position */
    return(count);
}

#endif


/** \fn int dmibios_read_cooked_proc (char *page, char **start, off_t off,
 *                                    int count,int *eof, void *data)
 *  \brief called by the kernel whenever a proc file is read by an application
 *  \param page
 *  \param start
 *  \param off
 *  \param count
 *  \param eof
 *  \param data
 *  \return bytes returned
 *
 *  This function is called by the kernel whenever an application opens
 *  a file in the proc file system created by this driver.
 *  This routine returns the appropriate data "in the file", i.e. the
 *  interpreted data of the corresponding DMI-BIOS structure.
 *
 *  \author Thomas Bretthauer
 *  \author Rainer König
 *  \date January 2001
 *  \date January 2014 refactored for Kernel dependencies
 */

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

int bios_read_cooked_proc (char *page, char **start, off_t off,
		   int count, int *eof, void *data)
{
    /* this function is called by the kernel if a file is read, */
    /* page points to a free page allocated by kernel,          */
    /* data points to the structure associated with the file    */

    unsigned char *scratch;
	unsigned int length;

    /* build the buffer with the information to present */
    scratch = bios_cook((smbios_struct *) data, &length);

    if (scratch == NULL)
    {
        *eof = 1;

        return 0;
    }

    if (off >= length)
    {
        *eof = 1;

        return 0;
    }

    if (off + count > length)
    {
        *eof = 1;

        count = length - off;
    }

    /* don't return more than PROC_BLOCK_SIZE bytes! */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;

    safe_memcpy (page, scratch + off, count);

    kfree(scratch);

    *start = page;

    return count;
}

#else /* LINUX_VERSION_CODE >= 3.10 from now on */

/* Kernel 3.10 introduced a different ABI for reading proc entries */
/* I assume that *offp holds an offset and that should be updated by this function */
/* Since there is no EOF variable the function should return 0 in case of EOF */

ssize_t bios_read_cooked_proc (struct file *filp, char *page, size_t count, loff_t *offp)
{
	void *data;
	data = PDE_DATA(file_inode(filp));
	if(!(data))
	{ printk (KERN_INFO "Null data");
	  return (0);
	}

	unsigned char *scratch;
	unsigned int length;

    /* build the buffer with the information to present */
    scratch = bios_cook((smbios_struct *) data, &length);

 	if (*offp >= length) /* EOF detection */
	{ return (0);
	}
	if (*offp + count > length) /* more wanted than there is left */
	{
		count = length - *offp;
	}
    /* don't return more than PROC_BLOCK_SIZE bytes */
    if (count > PROC_BLOCK_SIZE)
        count = PROC_BLOCK_SIZE;
    safe_memcpy(page, scratch + *offp, count);
    kfree(scratch);
    *offp+=count; /* shift offp to new position */
    return(count);
}

#endif

/** \fn int smbios_make_version_entry (struct proc_dir_entry *smbiosdir)
 *  \brief makes a directory entry for the proc file system
 *  \param smbiosdir pointer to proc directory where the files should be created in
 *  \return -ENOMEM if not enough memory, 0 otherwise
 *
 *  This function creates the smbios_version file in the proc file system.
 *
 *  \author Joachim Braeuer
 *  \author Rainer König
 *  \date October 2000
 *  \date January 2014 refactored for Kernel dependencies
 */

int
smbios_make_version_entry (struct proc_dir_entry *smbiosdir)
{
    struct proc_dir_entry *new_entry;
	char local_filename[64];

	
    /* file name */
    strcpy(local_filename, "smbios_version");

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

    /* create file */
    if (!(new_entry = create_proc_entry (local_filename, S_IFREG | S_IRUGO, smbiosdir)))
	       return -ENOMEM;

    /* set data to the beginning of the string */
    new_entry->data = smbios_version_string;
    /* set the read function for this file */
    new_entry->read_proc = smbios_version_proc;
    /* set the file size */
    new_entry->size=strlen(smbios_version_string);

#else /* LINUX_VERSION_CODE >= 3.10 from now on */

    if(!(new_entry = proc_create_data (local_filename,
    									S_IFREG | S_IRUGO,
    									smbiosdir,
    									&smbios_version_fops,
    									smbios_version_string)))
    	return (-ENOMEM);
    proc_set_size(new_entry, strlen(smbios_version_string));

#endif

    return 0;
}


/** \fn int smbios_make_dir_entries (struct proc_dir_entry *smbiosdir,
 *                  struct proc_dir_entry *rawdir, struct proc_dir_entry *cookeddir)
 *  \brief makes the directory entries for the proc file system
 *  \param smbiosdir pointer to proc directory where the files should be created in (/proc/smbios)
 *  \param rawdir pointer to proc directory where the files should be created in (/proc/smbios/raw)
 *  \param cookeddir pointer to proc directory where the files should be created in (/proc/smbios/cooked)
 *  \return -ENOMEM if not enough memory, 0 otherwise
 * 
 *  This function creates the files in the proc file system. Therefore, SM-BIOS
 *  is scanned and for every type found a file name will be created with the
 *  name of the form "type[-subtype].instance".
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */

int
smbios_make_dir_entries (struct proc_dir_entry *smbiosdir, struct proc_dir_entry *rawdir, struct proc_dir_entry *cookeddir)
{
    int i;
    unsigned int raw_name_length = 0;	
    char raw_name[12];                      /* e.g. 0.0 for structure type 0 , first instance */
    unsigned int readable_name_length = 0;	
    char readable_name[64];                 /* e.g. Bios.0 for structure type 0 , first instance */
    smbios_struct *struct_ptr = smbios_structures_base;


    /*
     *  for every SMBIOS structure do ...
     */
    for (i = 0; i < smbios_entry_point->no_of_structures; i++)
    {
        /*
         *  generate an unique name for the file:  "type[-subtype].count"
         */
        if (smbios_type_has_subtype (((smbios_struct *) struct_ptr)->type))
        {
            /* name will contain the raw file name, it equals the structure type (e.g. 1 for Type 1).
             * readable_name contains the interpreted file name (e.g. System for Type 1)
             */
	        raw_name_length = sprintf (raw_name, "%d-%d", struct_ptr->type, struct_ptr->subtype);
            readable_name_length = smbios_get_readable_name_ext(readable_name, struct_ptr);
        }
        else
        {
	        raw_name_length = sprintf (raw_name, "%d", struct_ptr->type);
            readable_name_length = smbios_get_readable_name(readable_name, struct_ptr);
        }

      
        /*
         *  create the file
         */

        /*
         * rawname, rawdirectory, pointer to dmibios structure, raw mode
         */
        make_file_entries (raw_name, rawdir, struct_ptr, FILE_MODE_RAW);
        /*
         * rawname, cooked directory, pointer to dmibios structure, cooked mode
         */
        make_file_entries (raw_name, cookeddir, struct_ptr, FILE_MODE_COOKED);
        /*
         * cookedname, smbiosdirectory, pointer to dmibios structure, cooked mode
         */
        make_file_entries (readable_name, smbiosdir, struct_ptr, FILE_MODE_COOKED);

        /*
         *  go to the next structure
         */
        struct_ptr =
	       (smbios_struct *) ((unsigned char *) struct_ptr + smbios_get_struct_length(struct_ptr));
    }

    return 0;
}


/** \fn int dmibios_make_dir_entries (struct proc_dir_entry *smbiosdir,
 *                  struct proc_dir_entry *rawdir, struct proc_dir_entry *cookeddir)
 *  \brief makes the directory entries for the proc file system
 *  \param smbiosdir pointer to proc directory where the files should be created in (/proc/smbios)
 *  \param rawdir pointer to proc directory where the files should be created in (/proc/smbios/raw)
 *  \param cookeddir pointer to proc directory where the files should be created in (/proc/smbios/cooked)
 *  \return -ENOMEM if not enough memory, 0 otherwise
 *
 *  This function creates the files in the proc file system. Therefore, DMI-BIOS
 *  is scanned and for every type found a file name will be created with the
 *  name of the form "type[-subtype].count".
 *
 *  \author Thomas Bretthauer
 *  \date November 2000
 */

int
dmibios_make_dir_entries (struct proc_dir_entry *smbiosdir, struct proc_dir_entry *rawdir, struct proc_dir_entry *cookeddir)
{
    dmibios_table_entry_struct *dmi_table_entry;
    smbios_struct *struct_ptr;
    unsigned int raw_name_length = 0;	
    char raw_name[12];                      /* e.g. 0.0 for structure type 0 , first instance */
    unsigned int readable_name_length = 0;	
    char readable_name[64];                 /* e.g. Bios.0 for structure type 0 , first instance */


    /* dmi_table_entry is the intermediate table that contains the offsets to the
     * dmi bios structures.
     */
    dmi_table_entry = dmibios_entry_point->entry;

    /*
     * for every intermediate structure do ...
     */
    while (dmi_table_entry->size != 0)
    {
        /* structure = entry point base + offset from intermediate table
         * struct_ptr contains a pointer to one single dmi bios structure
         */
        struct_ptr = smbios_base + dmi_table_entry->handle;

        /*
         * generate a unique name for the file:  "type[-subtype].count"
         */
        if (smbios_type_has_subtype (((smbios_struct *) struct_ptr)->type))
	    {
            /* name will contain the raw file name, it equals the structure type (e.g. 1 for Type 1).
             * readable_name contains the interpreted file name (e.g. System for Type 1)
             */
            raw_name_length = sprintf (raw_name, "%d-%d", struct_ptr->type, struct_ptr->subtype);
            readable_name_length = smbios_get_readable_name_ext(readable_name, struct_ptr);
        }
        else
        {
	        raw_name_length = sprintf (raw_name, "%d", struct_ptr->type);
            readable_name_length = smbios_get_readable_name(readable_name, struct_ptr);
        }

    
        /*
         * create the files
         */

        /*
         * rawname, rawdirectory, pointer to dmibios structure, raw mode
         */
        make_file_entries (raw_name, rawdir, struct_ptr, FILE_MODE_RAW);
        /*
         * rawname, cooked directory, pointer to dmibios structure, cooked mode
         */
        make_file_entries (raw_name, cookeddir, struct_ptr, FILE_MODE_COOKED);
        /*
         * cookedname, smbiosdirectory, pointer to dmibios structure, cooked mode
         */
        make_file_entries (readable_name, smbiosdir, struct_ptr, FILE_MODE_COOKED);

      
        /*
         * go to the next structure
         */
        dmi_table_entry++;
    }

    return 0;
}


/** \fn void smbios_destroy_dir_entries (struct proc_dir_entry *dir)
 *  \brief destroys the created files in the given proc directory
 *  \param dir directory of the files to be deleted
 *
 *  \author Markus Lyra
 *  \author Thomas Bretthauer
 *  \date October 2000
 */
void
smbios_destroy_dir_entries (struct proc_dir_entry *dir)
{

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

	struct proc_dir_entry *entry_ptr;

    for (entry_ptr = dir->subdir; entry_ptr; entry_ptr = dir->subdir)
        remove_proc_entry (entry_ptr->name, dir);

#else /* LINUX_VERSION_CODE >= 3.10 from now on */

    proc_remove(dir);
    release_dir_list(dir); /* Free our small filename cache */

#endif

}


/** \fn unsigned int smbios_get_readable_name_ext (char *name, smbios_struct *struct_ptr)
 *  \brief converts a smbios type with subtype (e.g. 0-0) into a readable type name.
 *  \param name
 *  \param struct_ptr
 *
 *  proprietary Fujitsu Siemens Code. not supported so far. therfore it returns the raw name.
 *
 *  \author Joachim Braeuer
 *  \date October 2000
 */

unsigned int
smbios_get_readable_name_ext(char *name, smbios_struct *struct_ptr)
{
    return sprintf (name, "%d-%d", struct_ptr->type, struct_ptr->subtype);
}


/** \fn unsigned int smbios_get_readable_name (char *name, smbios_struct *struct_ptr)
 *  \brief converts a smbios type with subtype (e.g. 0) into a readable type name (e.g. Bios).
 *  \param name [OUT]-Param. contains the readable SMBIOS Type name
 *  \param struct_ptr [IN]-Param. contains the raw SMBIOS Type
 *
 *  Just a big case that maps values to strings defined in strgdef.h.
 *
 *  \author Joachim Braeuer
 *  \date October 2000
 */

unsigned int
smbios_get_readable_name(char *name, smbios_struct *struct_ptr)
{
    switch(struct_ptr->type)
    {
        case 0: return sprintf (name, "%s", RD_BIOS);
        case 1: return sprintf (name, "%s", RD_SYSTEM);
		case 2: return sprintf (name, "%s", RD_BASEBOARD);
		case 3: return sprintf (name, "%s", RD_ENCLOSURE);
		case 4: return sprintf (name, "%s", RD_PROCESSOR);
		case 5: return sprintf (name, "%s", RD_MEMCTRL);
		case 6: return sprintf (name, "%s", RD_MEMMOD);
		case 7: return sprintf (name, "%s", RD_CACHE);
		case 8: return sprintf (name, "%s", RD_PORT);
		case 9: return sprintf (name, "%s", RD_SLOT);
		case 10: return sprintf (name, "%s", RD_ONBOARD);
		case 11: return sprintf (name, "%s", RD_OEMSTRINGS);
		case 12: return sprintf (name, "%s", RD_SYSTEMCONFIG);
		case 13: return sprintf (name, "%s", RD_BIOSLANG);
		case 14: return sprintf (name, "%s", RD_GROUPASSOC);
		case 15: return sprintf (name, "%s", RD_EVENTLOG);
		case 16: return sprintf (name, "%s", RD_MEMARRAY);
		case 17: return sprintf (name, "%s", RD_MEMDEV);
		case 18: return sprintf (name, "%s", RD_32MEMERR);
		case 19: return sprintf (name, "%s", RD_MEMMAPPEDADR);
		case 20: return sprintf (name, "%s", RD_MEMMAPPEDDEV);
		case 21: return sprintf (name, "%s", RD_POINTINGDEV);
		case 22: return sprintf (name, "%s", RD_BATTERY);
		case 23: return sprintf (name, "%s", RD_RESET);
		case 24: return sprintf (name, "%s", RD_SECURITY);
		case 25: return sprintf (name, "%s", RD_PWRCTRL);
		case 26: return sprintf (name, "%s", RD_VOLTAGE);
		case 27: return sprintf (name, "%s", RD_COOLINGDEV);
		case 28: return sprintf (name, "%s", RD_TEMP);
		case 29: return sprintf (name, "%s", RD_CURRENT);
		case 30: return sprintf (name, "%s", RD_RMTACCESS);
		case 31: return sprintf (name, "%s", RD_BIS);
		case 32: return sprintf (name, "%s", RD_BOOT_INFO);
		case 33: return sprintf (name, "%s", RD_64MEMERR);
		case 34: return sprintf (name, "%s", RD_MANAGDEV);
		case 35: return sprintf (name, "%s", RD_MANAGDEVCOMP);
		case 36: return sprintf (name, "%s", RD_MANAGDEVTHRESH);
		case 37: return sprintf (name, "%s", RD_MEMCHANNEL);
		case 38: return sprintf (name, "%s", RD_IPMI);
		case 39: return sprintf (name, "%s", RD_PWRSUP);
		case 126: return sprintf (name, "%s", RD_INACTIVE);
		case 127: return sprintf (name, "%s", RD_EOT);
		default: return sprintf (name, "%d", struct_ptr->type);
    }
}


/** \fn int make_file_entries (char *filename, struct proc_dir_entry *dir, smbios_struct *struct_ptr, int mode)
 *  \brief creates a file in a given /proc directory
 *  \param filename name of the file to create
 *  \param dir /proc directory where the file should be created
 *  \param struct_ptr the raw smbios structure related to the filename
 *  \param mode indicates if we need the cooked or the raw mode
 *
 *  creates a file in a given /proc directory
 *
 *  \author Joachim Braeuer
 *  \date October 2000
 */

int
make_file_entries (char *filename, struct proc_dir_entry *dir, smbios_struct *struct_ptr, int mode)
{
    struct proc_dir_entry *new_entry;
    struct proc_dir_entry *entry_ptr;
    int count;
    long size;
	char local_filename[64];
    struct file_operations *fops_ptr;

	strcpy(local_filename, filename);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

	count = 0;
    for (entry_ptr = dir->subdir; entry_ptr; entry_ptr = entry_ptr->next)
	    if (!(strncmp (local_filename, entry_ptr->name, strlen(local_filename))))
	        count++;

#else

	/* In Kernel 3.10 the structure proc_dir_entry is no longer public */
    /* look for the number of file entries with the above name. if we find to instances
     * we have now a third instance. count contains the new instance number
     */
    
    count = count_filename_instances(dir, local_filename);

#endif

    /*
     * name contains now <rawname>.<instance> (e.g. 1.1 for the second instance of Type 1 structure)
     * the same is valid for readable_name. it contains the readable name and instance for our structure.
     * <readable_name>.<instance> (e.g. System.1)
     * this will be the file names for the /proc file system.
     */
    sprintf (local_filename + strlen(local_filename), ".%d", count);
    PDEBUG("About to create file name: %s ...\n", local_filename);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,10,0)

    if (!(new_entry = create_proc_entry (local_filename, S_IFREG | S_IRUGO, dir)))
        return -ENOMEM;

    /* set data pointer to the raw smbios structure */
    new_entry->data = struct_ptr;

    /* set the read function for this file */
    if(mode == FILE_MODE_RAW)
    {
        new_entry->read_proc = bios_read_raw_proc;
        new_entry->size=smbios_get_struct_length(struct_ptr);
    }

    else 
    {
        new_entry->read_proc = bios_read_cooked_proc;
        new_entry->size = 0;
    }
#else /* LINUX_VERSION_CODE >= 3.10 from now on */

    if (mode == FILE_MODE_RAW)
    {
    	fops_ptr = &bios_read_raw_proc_fops;
    	size=smbios_get_struct_length(struct_ptr);
    }
    else
    {
    	fops_ptr = &bios_read_cooked_proc_fops;
    	size = 0;
    }

    if(!(new_entry = proc_create_data (local_filename,
    									S_IFREG | S_IRUGO,
    									dir,
    									fops_ptr,
    									struct_ptr)))
    	return (-ENOMEM);
    proc_set_size(new_entry, size);
    register_proc_filename(dir, local_filename);

#endif

    return 0;
}


