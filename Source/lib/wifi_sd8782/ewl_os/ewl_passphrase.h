
//#include "types.h"

/** Type definition of mlan_ds_passphrase for MLAN_OID_SEC_CFG_PASSPHRASE */
typedef struct ewl_passphrase
{
	u8 *ssid;

	/**
	 * ssid_len - Length of the SSID
	 */
	size_t ssid_len;

	/**
	 * bssid - BSSID
	 *
	 * If set, this network block is used only when associating with the AP
	 * using the configured BSSID
	 */
	u8 bssid[6];

	/**
	 * bssid_set - Whether BSSID is configured for this network
	 */
	int bssid_set;

	/**
	 * psk - WPA pre-shared key (256 bits)
	 */
	u8 psk[32];

	/**
	 * psk_set - Whether PSK field is configured
	 */
	int psk_set;

	/**
	 * passphrase - WPA ASCII passphrase
	 *
	 * If this is set, psk will be generated using the SSID and passphrase
	 * configured for the network. ASCII passphrase must be between 8 and
	 * 63 characters (inclusive).
	 */
	char *passphrase; /* malloc should is used */
    char passphrase_len; /* length of passphrase */
} ewl_passphrase_t;


int
ewl_set_passphrase(void *ctx, ewl_passphrase_t *passphrase);


