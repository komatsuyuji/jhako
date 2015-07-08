sbin_PROGRAMS = jhako_server

SUBDIRS = \
    expander \
    loader \
    bootstrap \
    executive \
    monitor \
    housekeeper

DIST_SUBDIRS = $(SUBDIRS)

jhako_server_SOURCES = \
    server.c

jhako_server_LDADD = \
    $(top_srcdir)/src/jhako_server/loader/libloader.a \
    $(top_srcdir)/src/jhako_server/expander/libexpander.a \
    $(top_srcdir)/src/jhako_server/bootstrap/libbootstrap.a \
    $(top_srcdir)/src/jhako_server/executive/libexecutive.a \
    $(top_srcdir)/src/jhako_server/monitor/libmonitor.a \
    $(top_srcdir)/src/jhako_server/housekeeper/libhousekeeper.a \
    $(top_srcdir)/src/libs/models/libjhkmodels.a \
    $(top_srcdir)/src/libs/network/libjhknetwork.a \
    $(top_srcdir)/src/libs/basic/libjhkbasic.a \
    $(top_srcdir)/src/libs/common/libjhkcommon.a
