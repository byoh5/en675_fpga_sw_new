

include $(CPU0DIR)/config.mk
 
LIB	= wificfg.o

#CFLAGS += -DDEBUG_LEVEL1

OBJS	= sdio.o         \
        timer.o          \
        ewl_os.o
        
MLIBS   = moal_sdio_mmc.o\
        moal_main.o      \
        moal_shim.o      \
        moal_priv.o      \
        moal_ioctl.o     \
        moal_wext.o      \
        moal_uap.o       \
        moal_uap_priv.o  \
        moal_uap_wext.o  \
        uaputil.o        \
        uapcmd.o         \
        ewl.o            \
        ewl_wext.o       \
        ewl_passphrase.o \
        ewlap.o			 \
        ../mlan/mlan_11d.o\
	../mlan/mlan_11h.o\
	../mlan/mlan_11n.o\
	../mlan/mlan_11n_aggr.o\
	../mlan/mlan_11n_rxreorder.o\
	../mlan/mlan_cfp.o\
	../mlan/mlan_cmdevt.o\
	../mlan/mlan_init.o\
	../mlan/mlan_join.o\
	../mlan/mlan_meas.o\
	../mlan/mlan_misc.o\
	../mlan/mlan_scan.o\
	../mlan/mlan_sdio.o\
	../mlan/mlan_shim.o\
	../mlan/mlan_sta_cmd.o\
	../mlan/mlan_sta_cmdresp.o\
	../mlan/mlan_sta_event.o\
	../mlan/mlan_sta_ioctl.o\
	../mlan/mlan_sta_rx.o\
	../mlan/mlan_sta_tx.o\
	../mlan/mlan_txrx.o\
	../mlan/mlan_wmm.o\
	../mlan/mlan_uap_cmdevent.o\
	../mlan/mlan_uap_ioctl.o\
	../mlan/mlan_uap_txrx.o        

all:	$(LIB)

$(LIB):	$(OBJS) $(MLIBS)
	@$(LD) -r -o $@ $(OBJS)
	@$(AR) rscv libmewl.a $(MLIBS)

clean:
	@rm -rf $(OBJS) $(LIB) $(MLIBS) libmewl.a .depend	

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
