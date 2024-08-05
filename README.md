# Virtual File System

## overview

I developed Vritual File System (VFS) layer. The system provides an abstraction layer that bridges driver and filesystem implementations with a modern architecture.
The system is designed to be used in operating system development and low level embedded environments.

Using the VFS, I implemented two filesystems to be used on a Linux machine:
* procfs: control and retrieve info about running processes and related data (analogous to Linux procfs).
* sysfs: control and retrieve info about the kernel's internal structures and hardware (analogous to Linux sysfs).
In addition to those, I implemented a RAM based virtual disk driver (analogous to Linux ramfs/tmpfs).

I implemented this entire project from scratch in pure C and C++ with no standard libraries. This includes:
* Polymorphism and Inheritance
* Sophisticated Algorithms and efficient data structures regarding strings and low level data
* Unionized data (replacing std::variant, std::any, std::optional)
* Static & Dynamic memory management (replacing smart pointers)
* Data serialization, encoding and hashing

## system features

the design for the system was inspired primarily from the legacy Unix/Linux filesystem hierarchy. the architecture is similar, with newer and modern abstractions.
speaking technically, the virtual file system architecture supports:
* files and directories
* soft links
* hardlinking nodes
* mounts
* drivers
