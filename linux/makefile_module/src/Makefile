#
# Rules for building sub-modules
#

# ----------------------------------------------------------------------------
# common rules
# ----------------------------------------------------------------------------
ROOT_PATH := ..

include $(ROOT_PATH)/gcc.mk

# ----------------------------------------------------------------------------
# sub-modules
# ----------------------------------------------------------------------------
SUBDIRS := cjson

# ----------------------------------------------------------------------------
# building rules
# ----------------------------------------------------------------------------
.PHONY: all install clean install_clean $(SUBDIRS) _all _install _clean _install_clean

all:
	$(Q)$(MAKE) _all TARGET=all

install:
	$(Q)$(MAKE) _install TARGET=install

clean:
	$(Q)$(MAKE) _clean TARGET=clean

install_clean:
	$(Q)$(MAKE) _install TARGET=install_clean

$(SUBDIRS):
	$(Q)$(MAKE) $(S) -C $@ $(TARGET)

_all: $(SUBDIRS)

_install: $(SUBDIRS)

_clean: $(SUBDIRS)

_install_clean: $(SUBDIRS)
