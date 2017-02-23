if BUILD_MSEVENTLOG
im_mseventlog_LTLIBRARIES	= im_mseventlog.la
im_mseventlog_la_SOURCES	= im_mseventlog.c im_mseventlog.h
im_mseventlog_la_LDFLAGS	= -module -no-undefined -avoid-version
im_mseventlog_la_LIBADD		= $(LIBEVENTLOG) $(LIBNX)
im_mseventlogdir		= $(NX_MODULEDIR)/input
endif
dist_pkgdata_DATA		= im_mseventlog-fields.xml
