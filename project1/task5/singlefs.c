/**

This is a demonstration program. It implements a FUSE-based file
system that has just one single file at any time. Creating new
files overwrites the single file, so many complex operations will
not work, as they often require temporary files, and this file system
does not support more than one.

This is based on the hellofs.c from the FUSE tutorial.

AJ Bieszczad, 2011

**/

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define _FILE_OFFSET_BITS 64
#define FUSE_USE_VERSION  26
#include <fuse.h>

static char  *file_path      = NULL;
static char  *file_content   = NULL;
static size_t file_size      = 0;

static int
single_getattr(const char *path, struct stat *stbuf)
{
    fprintf(stderr, "GETATTR\n");

    memset(stbuf, 0, sizeof(struct stat));

    if (strcmp(path, "/") == 0) { /* The root directory of our file system. */
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = (file_path == NULL ? 2 : 3); // if we created the file
    } else {
        if (file_path == NULL)
            return -ENOENT;

        if (strcmp(path, file_path) == 0) {
            stbuf->st_mode = S_IFREG | 0666;
            stbuf->st_nlink = 1;
            stbuf->st_size = file_size;
        } else { /* We reject everything else. */
            return -ENOENT;
      }
    }

    return 0;
}

static int
single_open(const char *path, struct fuse_file_info *fi)
{
    fprintf(stderr, "OPEN\n");

    if (file_path == NULL)
        return -ENOENT;
        
    if (strcmp(path, file_path) != 0)
    { // we only recognize one file.
        return -ENOENT;
    }

    return 0;
}

static int
single_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
              off_t offset, struct fuse_file_info *fi)
{
    fprintf(stderr, "READDIR\n");

    if (strcmp(path, "/") != 0)
    { // we only recognize the root directory.
        return -ENOENT;
    }

    filler(buf, ".", NULL, 0);           // current directory (.)
    filler(buf, "..", NULL, 0);          // parent directory (..)
    
    if (file_path != NULL)
        filler(buf, file_path + 1, NULL, 0);

    return 0;
}

static int
single_read(const char *path, char *buf, size_t size, off_t offset,
           struct fuse_file_info *fi)
{
    fprintf(stderr, "READ\n");

    if (file_path == NULL)
        return -ENOENT;

    if (strcmp(path, file_path) != 0) {
        return -ENOENT;
    }

    if (size > file_size) // do not return more than you have
        size = file_size;

    memcpy(buf, file_content, size); // provide the content to the caller

    return size;
}

static int
single_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi)
{
    fprintf(stderr, "WRITE\n");

    if (file_path == NULL)
        return -ENOENT;
        
    if (strcmp(path, file_path) != 0) // allows writes only to the existing file
        return -EINVAL;
        
    if (file_content != NULL) // just overwrite the content
        free(file_content);

    file_content = malloc(size);
    memcpy(file_content, buf, size);
    file_size = size;
    
    return 0;
}

static int
single_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
    fprintf(stderr, "CREATE\n");

    if (file_path != NULL) // get rid of the old file...
        free(file_path);

    if (file_content != NULL)
        free(file_content);
        
    file_content = NULL;
    file_size = 0;

    int len = strlen(path) + 1; //... and create a new one
    file_path = malloc(len);
    strncpy(file_path, path, len);
    
    return 0;   
}

static int
single_mknod(const char *path, mode_t mode, dev_t dev)
{
    fprintf(stderr, "MKNOD\n");

    if (S_ISREG(mode)) // only regular files allowed
        single_create(path, mode, NULL);
	  else
	      return -EINVAL;
	      
	  return 0;
}

static int
single_unlink(const char *path)
{
    fprintf(stderr, "UNLINK\n");

    if (file_path == NULL)
        return -ENOENT;

    if (strcmp(path, file_path) != 0)
        return -ENOENT;
        
    free(file_path);

    if (file_content != NULL)
        free(file_content);
        
    file_path = NULL;
    file_content = NULL;
    file_size = 0;
    
    return 0;
}

static int
single_utime(const char *path, struct utimbuf *ubuf)
{
    fprintf(stderr, "UTIME\n");

    return 0;
}

static int
single_truncate(const char *path, off_t newsize)
{
    fprintf(stderr, "TRUNCATE\n");

    if (file_path == NULL)
        return -ENOENT;

    if (strcmp(path, file_path) != 0)
        return -ENOENT;
        
    char *old_content = file_content;
    file_content = malloc(newsize);
    memcpy(file_content, old_content, newsize);
    free(old_content);
        
    return 0;
}

static int
single_getxattr(const char *path, const char *name, char *value, size_t vlen)
{
    fprintf(stderr, "GETXATTR: %s\n", name);
        
    return 0;
}
        
static int
single_flush(const char *path, struct fuse_file_info *fi)
{
    fprintf(stderr, "FLUSH\n");
        
    return 0;
}

static int
single_fsync(const char *path, int datasync, struct fuse_file_info *fi)
{
    fprintf(stderr, "FSYNC\n");
        
    return 0;
}

static int
single_statfs(const char *path, struct statvfs *statv)
{
    fprintf(stderr, "STATFS\n");
        
    return 0;
}

static int
single_access(const char *path, int mask)
{
    fprintf(stderr, "ACCESS:%x\n", mask);
        
    return 0;
}

static int
single_release(const char *path, struct fuse_file_info *fi)
{
    fprintf(stderr, "RELEASE\n");
        
    return 0;
}

static struct fuse_operations single_filesystem_operations = {
    .getattr  = single_getattr,
    .getxattr = single_getxattr,
    .create   = single_create,
    .mknod    = single_mknod,
    .utime    = single_utime,
    .unlink   = single_unlink,
    .truncate = single_truncate,
    .flush    = single_flush,
    .fsync    = single_fsync,
    .access   = single_access,
    .open     = single_open,
    .statfs   = single_statfs,
    .release  = single_release,
    .read     = single_read,
    .write    = single_write,
    .readdir  = single_readdir,
};

int
main(int argc, char **argv)
{
    return fuse_main(argc, argv, &single_filesystem_operations, NULL);
}
