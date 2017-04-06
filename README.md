# TLS_Examples
Visual Studio Solution consisting of 2 Projects: TLS_Static and TLS_Dynamic.

These server as examples of Static and Dynamic Thread Local Storage (TLS) Callback creation. This can be used for code execution prior to main(). Compiled and tested for 32 and 64 bit. Tested on Windows 10.

Note: The callback functions simply spawn a Message Box & output to consol debug information to help understand what is happening in memory.

## TLS_Static
Example of adding two TLS callback functions to the binary. No manual modification of the PE is necessary.

## TLS_Dynamic
Example of a static TLS callback dynamically, at run-time, adding a second TLS Callback to the binary. This second callback will be executed directly after the first by the OS Loader. As it is dynamically added at run-time, the second callback will not appear in the PE Headers.
