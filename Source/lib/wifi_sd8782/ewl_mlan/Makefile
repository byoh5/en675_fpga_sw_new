

include $(CPU0DIR)/config.mk

LIB	= mlan.o

OBJS	= mlan_11d.o				\
		  mlan_11h.o				\
		  mlan_11n.o				\
		  mlan_11n_aggr.o			\
		  mlan_11n_rxreorder.o		\
		  mlan_cfp.o 				\
		  mlan_cmdevt.o    			\
		  mlan_init.o 				\
		  mlan_join.o 				\
		  mlan_meas.o 				\
		  mlan_misc.o 				\
		  mlan_scan.o 				\
		  mlan_sdio.o 				\
		  mlan_shim.o 				\
		  mlan_sta_cmd.o     		\
		  mlan_sta_cmdresp.o 		\
		  mlan_sta_event.o   		\
		  mlan_sta_ioctl.o   		\
		  mlan_sta_rx.o      		\
		  mlan_sta_tx.o      		\
		  mlan_txrx.o        		\
		  mlan_wmm.o         		\
		  mlan_uap_cmdevent.o       \
		  mlan_uap_ioctl.o          \
		  mlan_uap_txrx.o


all:	$(LIB)

$(LIB):	$(OBJS)
	@$(LD) -r -o $@ $(OBJS)
	@$(AR) rscv libmlan.a $(OBJS)
	
clean:
	@rm -rf $(OBJS) $(LIB) .depend	

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
