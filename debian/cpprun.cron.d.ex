#
# Regular cron jobs for the cpprun package.
#
0 4	* * *	root	[ -x /usr/bin/cpprun_maintenance ] && /usr/bin/cpprun_maintenance
