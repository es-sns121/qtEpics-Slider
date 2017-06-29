# qtEpics Slider 

This application serves as an example of how a simple Qt application can alter 
data on an EPICS v4 record hosted on an EPICS v4 database.

This application also demonstrates how to interperate an EPICS Display structure.
This allows a client to know the context in which a piece of data should be 
displayed (i.e. ranges, format, units, etc.).

## Building

To build the application an installion of EPICS v3 and v4 must be present. This 
application was implemented using EPICS version 3.16 and version 4.5.0.

The appilcation also requires an installation of Qt. The application was implemeted 
using Qt version 4.8.1.

The build system requires a g++ compiler and the Qt moc (meta object compiler) to be
reachable from the $PATH enviornment variable. g++ compiler version 4.4.7 was used
to build this application.

The build system expects certain enviornment variables to be set:

HOST\_OS = The host system OS. An example would be 'Linux'.

EPICS\_HOST\_ARCH = The host system architecture. An example would be 'linux-x86\_64'

EPICS\_V3\_DIR = The path to the topmost directory of the EPICS v3 install.

EPICS\_V4\_DIR = The path to the topmost directory of the EPICS v4 install.

QT\_INC = The path to the 'include' directory of the Qt install.

QT\_LIB = The path to the 'lib' directory of the Qt install.

Assuming all these variables are set and proper installs present, simply type:
	
	> make 

in the topmost directory of the qtEpics Slider project.

## Running the application

To run the application navigate to the 'bin' directory after building.

To start the server, type:

	> ./server

To start the client, type:
	
	> ./client

Note that server must be running for the client to operate. 

Type 'exit' to quit the server.
Click on the exit box of the client window to quit the client.
