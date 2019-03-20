#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2009";
	static const double UBUNTU_VERSION_STYLE = 9.09;
	
	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 0;
	static const long BUILD = 1186;
	static const long REVISION = 6654;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2273;
	#define RC_FILEVERSION 2,0,1186,6654
	#define RC_FILEVERSION_STRING "2, 0, 1186, 6654\0"
	static const char FULLVERSION_STRING[] = "2.0.1186.6654";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 86;
	

}
#endif //VERSION_h
