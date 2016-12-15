/**
 * Joshua Steward
 * Project 1 Task 2
 * 11/3/2016
 **/


#define FUSE_USE_VERSION  26
#define MAX 1024;
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>


static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";

static const char *planet_str = "Hello Planet Earth!\n";
static const char *planet_path = "/planet";

static const char *nope_str = "LOSER YOU CANT READ ME\n";
static const char *nope_path = "/nope";

char* randstring(size_t length)
{
    static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
    char *randomString = NULL;
    if (length)
    {
        randomString = malloc(sizeof(char) * (length +1));
        if (randomString)
        {
            for (int n = 0;n < length;n++)
            {
                int key = rand() % (int)(sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }
    return randomString;
}

static int hello_getattr(const char *path, struct stat *stbuf)
{
    int res = 0;
    memset(stbuf, 0, sizeof(struct stat));
    if(strcmp(path, "/") == 0)
    {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
    }
    else if(strcmp(path, hello_path) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(hello_str);
    }
    else if(strcmp(path, planet_path) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(planet_str);
    }
        /*
    else if (strcmp(path, rand_path) == 0)
    {
        stbuf->st_mode = S_IFREG | 0444;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(rand_str);
    }
         */
    else if (strcmp(path, nope_path) == 0)
    {
        stbuf->st_mode = S_IFREG | 0000;
        stbuf->st_nlink = 1;
        stbuf->st_size = strlen(nope_str);
    }
    else
        res = -ENOENT;

    return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                         off_t offset, struct fuse_file_info *fi)
{
    (void) offset;
    (void) fi;

    if(strcmp(path, "/") != 0)
        return -ENOENT;

    filler(buf, ".", NULL, 0);
    filler(buf, "..", NULL, 0);
    filler(buf, hello_path + 1, NULL, 0);
    filler(buf, planet_path + 1, NULL, 0);
    filler(buf, nope_path + 1, NULL, 0);
    //filler(buf, rand_path + 1, NULL, 0);

    return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
    if(strcmp(path, hello_path) != 0 && strcmp(path, planet_path) != 0 && strcmp(path, nope_path) != 0 /*&& strcmp(path, rand_path) != 0*/)
        return -ENOENT;

    if (strcmp(path, nope_path) != 0)
    {
        if((fi->flags & 3) != O_RDONLY)
            return -EACCES;
    }
    else
    {
        return -EACCES;
    }


    return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
                      struct fuse_file_info *fi)
{
    //to do still
    //flag 1 for hello file, 0 for planet
    int hello1 = 0;
    int planet1 = 0;
    size_t len;
    (void) fi;
    if (strcmp(path, hello_path) == 0)
    {
        len = strlen(hello_str);
        hello1 = 1;
    }
    else if (strcmp(path, planet_path) == 0)
    {
        len = strlen(planet_str);
        planet1 = 1;
    }
        /*
    else if (strcmp(path, rand_path) == 0)
    {
        len = strlen(rand_str);
    }
         */
    else if (strcmp(path, nope_path) == 0)
    {
        return -EACCES;
    }
    else
    {
        return -ENOENT;
    }


    if (offset < len)
    {
        if (offset + size > len)
        {
            size = len - offset;
        }

        if (hello1)
        {
            memcpy(buf, hello_str + offset, size);
        }
        else if (planet1)
        {
            memcpy(buf, planet_str + offset, size);
        }
            /*
        else
        {
            memcpy(buf, rand_str + offset, size);
        }
             */
    }
    else
    {
        size = 0;
    }

    return size;
}

static struct fuse_operations hello_oper = {
        .getattr   = hello_getattr,
        .readdir = hello_readdir,
        .open   = hello_open,
        .read   = hello_read,
};

int main(int argc, char *argv[])
{
    return fuse_main(argc, argv, &hello_oper, NULL);
}