//
// Copyright (c) 2008, Wei Mingzhi. All rights reserved.
//
// Use, redistribution and modification of this code is unrestricted as long as this
// notice is preserved.
//

#ifndef CONFIG_H
#define CONFIG_H

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "1.9"
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN 256
#endif

#ifndef _XBOX // Use config manager on Xbox
extern char cfgfile[256];
#endif

#ifdef _MSC_VER
#pragma warning (disable:4133)
#pragma warning (disable:4142)
#pragma warning (disable:4244)
#pragma warning (disable:4996)
#pragma warning (disable:4018)
#pragma warning (disable:4761)
#endif

#endif //CONFIG_H