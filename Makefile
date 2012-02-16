SUBDIRS = libvanred/src vanred

all install uninstall clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir $@; \
	done
