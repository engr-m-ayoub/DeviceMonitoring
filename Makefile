BUILDDIR=${PWD}/build
SRCDIR=${PWD}/src
INCDIR=${PWD}/inc
DEPS= ${SRCDIR}/monitoring_server.cpp ${SRCDIR}/monitoring_client.cpp ${INCDIR}/monitoring.h

CC=g++
CFLAGS=-I. -I${INCDIR}

all: clean build

.PHONY: clean
clean:
	rm -rvf $(BUILDDIR)/monitoring_server $(BUILDDIR)/monitoring_client


.PHONY: build
build: $(DEPS)
	mkdir -p $(BUILDDIR)
	cd $(BUILDDIR) && \
	$(CC) $(CFLAGS) $(SRCDIR)/monitoring_server.cpp -o monitoring_server &&\
	$(CC) $(CFLAGS) $(SRCDIR)/monitoring_client.cpp -o monitoring_client