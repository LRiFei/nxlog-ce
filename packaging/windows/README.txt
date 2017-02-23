Please edit the configuration file after installation. This should be
located under C:\Program Files\nxlog\conf\nxlog.conf or 
C:\Program Files (x86)\nxlog\conf\nxlog.conf depending on your windows
architecture.
You might need to adjust the ROOT folder in the config file, otherwise 
nxlog will not start. After this is done, you should be able to start
the nxlog service from the service manager. Alternatively, the service
can be also started by executing C:\Program Files (x86)\nxlog\nxlog.exe.
Running the executable with the -f command line argument will run it in
foreground if you don't want to run it as a service.

nxlog will write its own messages to the logfile located under 
C:\Program Files (x86)\nxlog\data\nxlog.log
If you have trouble starting or running it, please take a look there.

See the NXLOG Reference Manual for details about configuration and usage.
The manual should be available online at http://nxlog.org/resources
and is also installed under C:\Program Files (x86)\nxlog\doc\

