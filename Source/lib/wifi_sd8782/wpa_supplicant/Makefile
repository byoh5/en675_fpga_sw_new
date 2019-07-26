
include $(CPU0DIR)/config.mk

#CONFIG_IEEE8021X_EAPOL=n
#CONFIG_EAP_PSK=n
#CONFIG_GEN_RAND_ISSAC=n
#CONFIG_L2_PACKET=y /* WPA SUPPLICANT can handle eaop message from wifi */
CONFIG_NO_WPA=n


#/*
#    CONFIG_CTRL_IFACE CFLAGS support wpa client to control supplicant
#*/


#-DINTERNAL_AES -DINTERNAL_SHA1 -DINTERNAL_MD5 
CFLAGS += -DCONFIG_NO_STDOUT_DEBUG -DCONFIG_CTRL_IFACE  -DCONFIG_NO_WPA_SM
LIB	= wpa_lib.o

#	    =md4.o\
#		 md5.o\
#		 rc4.o\
#		 sha1.o\
#		 des.o\
#		 aes_wrap.o\
#        tls_none.o\
#sudo apt-get install autoconf

OBJS =  driver_wext.o\
        common.o\
        drivers.o\
        config.o\
        eloop.o\
        wpa_supplicant.o\
        os_none.o\
        events.o\
        wl_tools.o
#//        ctrl_iface.o\
#//        ctrl_iface_shell.o\
#//        wpa_cli.o\
#//        config_file.o\
         		
ifdef CONFIG_IEEE8021X_EAPOL
OBJS += eap.o eapol_sm.o eap_methods.o
endif

ifdef CONFIG_EAP_PSK
CFLAGS += -DEAP_PSK
OBJS += eap_psk.o eap_psk_common.o
endif

ifdef CONFIG_GEN_RAND_ISSAC
CFLAGS += -DISSAC
OBJS += isaac.o
endif

ifdef CONFIG_L2_PACKET
OBJS += l2_packet.o
CFLAGS += -DL2_PACKET
endif


ifeq ($(CONFIG_NO_WPA),n)
CFLAGS += -DCONFIG_NO_WPA_SM
OBJS += wpa.o
else
CFLAGS += -DCONFIG_NO_WPA -DCONFIG_NO_WPA2
endif


		  
all:	$(LIB)

$(LIB):	$(OBJS)
	@$(LD) -r -o $@ $(OBJS)
	
clean:
	@rm -rf $(OBJS) $(LIB) .depend	

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
