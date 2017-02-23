if BUILD_MSEVENTLOG
im_msvistalog_LTLIBRARIES	= im_msvistalog.la
im_msvistalog_la_SOURCES	= im_msvistalog.c im_msvistalog.h winevt.h
im_msvistalog_la_LDFLAGS	= -module -no-undefined -avoid-version
im_msvistalog_la_LIBADD		= -lwevtapi -lole32 $(LIBNX)
im_msvistalogdir		= $(NX_MODULEDIR)/input
endif
dist_pkgdata_DATA		= im_msvistalog-fields.xml

