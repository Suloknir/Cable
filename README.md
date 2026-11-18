# **Cable**
## Brief description:
Cable is a `header-only` library written in C that allows you to easily print nicely formatted tables. You provide table dimensions, headers, data and Cable takes care of the rest (proper column widths, formatting etc.). 

## Usage
* Place _Cable.h_ file in your project directory. You don't need any other files.
* Include _Cable.h_ anywhere you want to use it in your code. 
* Define _CABLE_IMPLEMENTATION_ macro **only once** in your project. You **must** do that before you include _Cable.h_ as shown below:

```C
/*
* 'define' and 'include' must be in this order.
*/
#define CABLE_IMPLEMENTATION //do this once for project
#include "Cable.h"
```

* Examples of how to use library are shown in `example.c`.
