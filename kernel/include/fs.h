#ifndef HELIUM_FS_H
#define HELIUM_FS_H

/* Helium filesystem definition, heavily ispired from Unix */

#include <stddef.h>
#include <stdint.h>

typedef enum FSNODE_TYPE fsnode_type;

typedef struct DIRCAP dircap;
typedef struct FILECAP filecap;

typedef struct FS_IMPL fsimpl;

typedef struct FILESYS filesys;
typedef struct FSNODE fsnode;

#define FS_NAMELEN (16)
#define FSNODE_NAMELEN (512)

// Types of FS nodes
enum FSNODE_TYPE
{
  FSNODE_FILE,
  FSNODE_DIR,
  FSNODE_LINK
};

// Operations that can be done on a directory
struct DIRCAP
{
  // In filesys, if set, then all fsnodes override caps are ignored
  // even if they have used set to 1.
  // If the filesys' used is set to 0, then the caps in the filesys are just
  // the default, but each fsnode can override them by setting their cap's
  // used to 1.
  uint64_t used:1;

  uint64_t mknode:1; // Can make new subnodes
  uint64_t rmnode:1; // Can remove subnodes
  uint64_t list:1; // Can list subnodes
  uint64_t tellsize:1; // Can tell the size of th dir
  uint64_t link:1; // Can have link subnodes
};

// Operations that can be done on a file
struct FILECAP
{
  // In filesys, if set, then all fsnodes override caps are ignored
  // even if they have used set to 1.
  // If the filesys' used is set to 0, then the caps in the filesys are just
  // the default, but each fsnode can override them by setting their cap's
  // used to 1.
  uint64_t used:1;

  uint64_t read:1; // Can read data from file
  uint64_t write:1; // Can write data to anywhere in the file
  uint64_t append:1; // Can push new data at end of file

  uint64_t rmself:1; // File can be removed, overrides dircap
  uint64_t tellsize:1; // Can tell size of file

  size_t max_size; // Max size of a file, 0 for no max
};

// If a file system cannot write/append to all files, and cannot mknode
// and rmnode from all dirs, then it is called immutable

// Structur containing function pointers to filesystem operations
struct FS_IMPL
{
  // Release any data in fs->ext
  void (*fs_release)(filesys *fs);

  // Called when a node is created
  void (*fsnode_attach)(fsnode *parent, fsnode *child, void *ext);
  // Called when a node is removed
  void (*fsnode_detach)(fsnode *parent, fsnode *child);

  void (*fsnode_link)(fsnode *target, fsnode *link);

  // Reads data from a file into the buffer, cannot read more than
  // specified size, returns actually read bytes
  size_t (*fsfile_read)(fsnode *file, size_t offset, size_t size, void *buf);
  // Writes data from the buffer into the file, cannot write more than
  // specified size, returns actually written bytes
  size_t (*fsfile_write)(fsnode *file, size_t offset, size_t size, void *buf);
  // Appends data frim buffer into the end of file, cannot append more than
  // specified size, returns actually appended bytes
  size_t (*fsfile_append)(fsnode *file, size_t size, void *buf);
  size_t (*fsfile_tellsize)(fsnode *file);

  // Returns the next subnodes of the directory
  // If prev is NULL, return the first subnode
  // If prev is last subnode, returns NULL
  fsnode *(*fsdir_list_next)(fsnode *dir, fsnode *prev);
  size_t (*fsdir_tellsize)(fsnode *dir);
};

// Structure that defines a filesystem
struct FILESYS
{
  char name[FS_NAMELEN];

  // default capabilities
  dircap dir_cap;
  filecap file_cap;

  fsnode *root;

  fsimpl impl;

  // Filesystems can use this pointer as they please
  void *ext;
};

struct FSNODE
{
  char name[FSNODE_NAMELEN];

  filesys *fs;
  fsnode_type type;

  fsnode *parent; // can be NULL for a directory
  fsnode *nsib; // next sibling

  // Each filesystem is free to use this pointer as they wish
  // I imagine it can store info to where the data can be read/written to
  // info about cache if there is any etc etc
  // Filesystems are required to release any memory allocated for this pointer
  // through fsnode_detach
  void *ext;

  // The first node that is a link to this node
  fsnode *flink;

  // Fields that only exist in file xor directories xor links
  union
  {
    struct
    {
      filecap cap;
      size_t size;
    } file;

    struct
    {
      dircap cap;
      fsnode *fchild;
    } dir;

    struct
    {
      // The node this node is a link to
      fsnode *target;

      // The next node that is also a link to `target`
      fsnode *nlink;
    } link;
  };
};

filesys *fs_mount(char *name);
void fs_umount(filesys *fs);

// name functions
int fs_valid_sys_name(char *name);
int fs_valid_node_name(char *name);
int fs_valid_path(char *path);
void fs_makecanonical(char *path, char *opath);
void fs_pathtok(char *path, char *fsname, char **nodes, size_t *len);

// TODO: when processes are implemented, al fs functions should also
// get a pointer to the process making the request
fsnode *fs_search(char *path);

fsnode *fs_dirof(char *path);

#endif