#define FORCE_ABT_SOFFr			(ValSft_R29(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define FORCE_ABT_SOFFw(val)	_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define FORCE_ABT_SONr			(ValSft_R28(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define FORCE_ABT_SONw(val)		_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define STX_POr					(ValSft_R27(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define STX_POw(val)			_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define FONT_WLTCr				(ValSft_R24(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_03)
#define FONT_WLTCw(val)			_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define PREG_UPTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0002<<2)))&BitMask_01)
#define PREG_UPTw(val)			_wr32(_REG_BASE_+(0x0002<<2),(UINT)((_rd32(_REG_BASE_+(0x0002<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define SYNC_UPr				(ValSft_R31(_rd32(_REG_BASE_+(0x0003<<2)))&BitMask_01)
#define SYNC_UPw(val)			_wr32(_REG_BASE_+(0x0003<<2),(UINT)((_rd32(_REG_BASE_+(0x0003<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FPCK_PDr				(ValSft_R30(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define FPCK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PPCK_PDr				(ValSft_R29(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define PPCK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PSCK_PDr				(ValSft_R28(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define PSCK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PR_CK0_PDr				(ValSft_R27(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define PR_CK0_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define FN_CK0_PDr				(ValSft_R26(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define FN_CK0_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define YCW_CK0_PDr				(ValSft_R24(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define YCW_CK0_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define YCW_CK1_PDr				(ValSft_R23(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define YCW_CK1_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define YCW_CK2_PDr				(ValSft_R22(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define YCW_CK2_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define YCW_CK3_PDr				(ValSft_R21(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define YCW_CK3_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define YCW_CK4_PDr				(ValSft_R20(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define YCW_CK4_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IMD_CK_PDr				(ValSft_R19(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IMD_CK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define JPGCK_PDr				(ValSft_R18(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define JPGCK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define CDS0_PCK_PDr			(ValSft_R17(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define CDS0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define CDS1_PCK_PDr			(ValSft_R16(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define CDS1_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define CDS2_PCK_PDr			(ValSft_R15(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define CDS2_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define CDS3_PCK_PDr			(ValSft_R14(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define CDS3_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define IBT_PCK_PDr				(ValSft_R13(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IBT_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define IBT0_PCK_PDr			(ValSft_R12(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IBT0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define IBT1_PCK_PDr			(ValSft_R11(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IBT1_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IBT2_PCK_PDr			(ValSft_R10(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IBT2_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define BT_PCK_PDr				(ValSft_R09(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT_PCK_PDw(val)			_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define BT0_PCK_PDr				(ValSft_R08(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT0_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define BT1_PCK_PDr				(ValSft_R07(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT1_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define BT2_PCK_PDr				(ValSft_R06(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT2_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define IPUIRQ0_PDr				(ValSft_R05(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IPUIRQ0_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define IPUIRQ1_PDr				(ValSft_R04(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define IPUIRQ1_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT1120_PCK_PDr			(ValSft_R03(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT1120_PCK_PDw(val)		_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define BT1120_PCK0_PDr			(ValSft_R02(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT1120_PCK0_PDw(val)	_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define BT1120_PCK1_PDr			(ValSft_R01(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT1120_PCK1_PDw(val)	_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define BT1120_PCK2_PDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0004<<2)))&BitMask_01)
#define BT1120_PCK2_PDw(val)	_wr32(_REG_BASE_+(0x0004<<2),(UINT)((_rd32(_REG_BASE_+(0x0004<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define YCR_CK0_PDr				(ValSft_R31(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define YCR_CK0_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define YCR_CK1_PDr				(ValSft_R30(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define YCR_CK1_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YCR_CK2_PDr				(ValSft_R29(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define YCR_CK2_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define YCR_CK3_PDr				(ValSft_R28(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define YCR_CK3_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define YCR_CK4_PDr				(ValSft_R27(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define YCR_CK4_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ECK_PDr					(ValSft_R26(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define ECK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define ITCK0_PDr				(ValSft_R25(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define ITCK0_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define ITCK1_PDr				(ValSft_R24(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define ITCK1_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define PIP_PPCK_PDr			(ValSft_R22(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PIP_PPCK_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define DCKO0_PDr				(ValSft_R21(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define DCKO0_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define DCKO1_PDr				(ValSft_R20(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define DCKO1_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define RYCCK1_PDr				(ValSft_R19(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define RYCCK1_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define RYCCK2_PDr				(ValSft_R18(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define RYCCK2_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define RYCCK3_PDr				(ValSft_R17(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define RYCCK3_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define I2SBCK_PDr				(ValSft_R13(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define I2SBCK_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define PCKO_PDr				(ValSft_R12(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCKO_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define PCKO0_PDr				(ValSft_R11(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCKO0_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define PCKO1_PDr				(ValSft_R10(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCKO1_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define PCKO2_PDr				(ValSft_R09(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCKO2_PDw(val)			_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define PCK_EDIV_PD0r			(ValSft_R08(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCK_EDIV_PD0w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define PCK_EDIV_PD1r			(ValSft_R07(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCK_EDIV_PD1w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define PCK_EDIV_PD2r			(ValSft_R06(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCK_EDIV_PD2w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define PCK_EDIV_PD3r			(ValSft_R05(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCK_EDIV_PD3w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define PCLK_HF_PDr				(ValSft_R03(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCLK_HF_PDw(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define PCLK_HF_PD0r			(ValSft_R02(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCLK_HF_PD0w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define PCLK_HF_PD1r			(ValSft_R01(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCLK_HF_PD1w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define PCLK_HF_PD2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0005<<2)))&BitMask_01)
#define PCLK_HF_PD2w(val)		_wr32(_REG_BASE_+(0x0005<<2),(UINT)((_rd32(_REG_BASE_+(0x0005<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define JDEC_CK_PDr				(ValSft_R31(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define JDEC_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DZPCK_PDr				(ValSft_R30(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DZPCK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMGM_CK_PDr				(ValSft_R29(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define IMGM_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define ALP_CK0_PDr				(ValSft_R28(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define ALP_CK0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PCKO_CK_PDr				(ValSft_R26(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCKO_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define OSYNCI_CK_PDr			(ValSft_R25(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define OSYNCI_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define ECKO_PDr				(ValSft_R24(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define ECKO_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define HVSO_CK_PDr				(ValSft_R23(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define HVSO_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define DCKO2_PDr				(ValSft_R22(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DCKO2_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define LCDO_CK_PDr				(ValSft_R21(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define LCDO_CK_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define OTCK1_PDr				(ValSft_R17(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define OTCK1_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define DITCK1_PDr				(ValSft_R16(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DITCK1_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define OTCK0_PDr				(ValSft_R15(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define OTCK0_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define DITCK0_PDr				(ValSft_R14(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DITCK0_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define PSE_CK_PDr				(ValSft_R13(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PSE_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define DO0_CK_PDr				(ValSft_R12(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DO0_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define DO1_CK_PDr				(ValSft_R11(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DO1_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define DO2_CK_PDr				(ValSft_R10(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DO2_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define DO3_CK_PDr				(ValSft_R09(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define DO3_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define LCD_CK_PDr				(ValSft_R08(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define LCD_CK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define PCLK_PDr				(ValSft_R07(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCLK_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define PCLK0_PDr				(ValSft_R06(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCLK0_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define PCLK1_PDr				(ValSft_R05(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCLK1_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define PCLK2_PDr				(ValSft_R04(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define PCLK2_PDw(val)			_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define TDM_PCK0_PDr			(ValSft_R03(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define TDM_PCK0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define TDM_DCK0_PDr			(ValSft_R02(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define TDM_DCK0_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define TDM_PCK1_PDr			(ValSft_R01(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define TDM_PCK1_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define TDM_DCK1_PDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0006<<2)))&BitMask_01)
#define TDM_DCK1_PDw(val)		_wr32(_REG_BASE_+(0x0006<<2),(UINT)((_rd32(_REG_BASE_+(0x0006<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define DCKO0_DLYr				(ValSft_R28(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define DCKO0_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define DCKO1_DLYr				(ValSft_R24(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define DCKO1_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define DCKO2_DLYr				(ValSft_R20(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define DCKO2_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define PCLK_DLYr				(ValSft_R16(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define PCLK0_DLYr				(ValSft_R12(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK0_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define PCLK1_DLYr				(ValSft_R08(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK1_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define PCLK2_DLYr				(ValSft_R04(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_04)
#define PCLK2_DLYw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define ECKO_INVr				(ValSft_R01(_rd32(_REG_BASE_+(0x0007<<2)))&BitMask_01)
#define ECKO_INVw(val)			_wr32(_REG_BASE_+(0x0007<<2),(UINT)((_rd32(_REG_BASE_+(0x0007<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define VIRQI_ENr				(ValSft_R31(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQI_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VIRQW_ENr				(ValSft_R30(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQW_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define VIRQO_ENr				(ValSft_R29(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQO_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define UIRQ0_ENr				(ValSft_R28(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define UIRQ0_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define UIRQ1_ENr				(ValSft_R27(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define UIRQ1_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define VIRQW2_ENr				(ValSft_R26(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQW2_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VIRQI0_ENr				(ValSft_R25(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQI0_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define VIRQI1_ENr				(ValSft_R24(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQI1_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define VIRQI2_ENr				(ValSft_R23(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define VIRQI2_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IMW0_ENr				(ValSft_R22(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define IMW0_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IMW1_ENr				(ValSft_R21(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define IMW1_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IMW2_ENr				(ValSft_R20(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define IMW2_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IMW3_ENr				(ValSft_R19(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define IMW3_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define IMW4_ENr				(ValSft_R18(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define IMW4_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define JPGIRQ_ENr				(ValSft_R17(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define JPGIRQ_ENw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define CLI_IMW0r				(ValSft_R14(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_IMW0w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define CLI_IMW1r				(ValSft_R13(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_IMW1w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define CLI_IMW2r				(ValSft_R12(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_IMW2w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define CLI_IMW3r				(ValSft_R11(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_IMW3w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define CLI_IMW4r				(ValSft_R10(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_IMW4w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define CLI_UIRQ0r				(ValSft_R09(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_UIRQ0w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define CLI_UIRQ1r				(ValSft_R08(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_UIRQ1w(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define CLI_VLOCKI0r			(ValSft_R07(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKI0w(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define CLI_VLOCKI1r			(ValSft_R06(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKI1w(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define CLI_VLOCKI2r			(ValSft_R05(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKI2w(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CLI_VLOCKW2r			(ValSft_R04(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKW2w(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define CLI_JPGr				(ValSft_R03(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_JPGw(val)			_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define CLI_VLOCKIr				(ValSft_R02(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKIw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define CLI_VLOCKWr				(ValSft_R01(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKWw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define CLI_VLOCKOr				(ValSft_R00(_rd32(_REG_BASE_+(0x0008<<2)))&BitMask_01)
#define CLI_VLOCKOw(val)		_wr32(_REG_BASE_+(0x0008<<2),(UINT)((_rd32(_REG_BASE_+(0x0008<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define BT_SMr					(ValSft_R31(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_SMw(val)				_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_OBUF_RSTr			(ValSft_R30(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_OBUF_RSTw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT1120_SMODr			(ValSft_R29(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT1120_SMODw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT1120_MODr				(ValSft_R27(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT1120_MODw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define YC_MODr					(ValSft_R25(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define YC_MODw(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BT_MODESDr				(ValSft_R24(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_MODESDw(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_16MODr			(ValSft_R23(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT1120_16MODw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define WE_FLG_SELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define WE_FLG_SELw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define BT_POL_HSIr				(ValSft_R21(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_POL_HSIw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define FLD_INVr				(ValSft_R20(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define FLD_INVw(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define YCBCR_MODr				(ValSft_R19(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define YCBCR_MODw(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define YC_FLG_INVr				(ValSft_R18(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define YC_FLG_INVw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define CBR_FLG_INVr			(ValSft_R17(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define CBR_FLG_INVw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BT_DIN_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_DIN_SELw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define CBR_INTP_ONr			(ValSft_R15(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define CBR_INTP_ONw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define CPAD_ONr				(ValSft_R14(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define CPAD_ONw(val)			_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define BT_TRG_DINr				(ValSft_R13(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_TRG_DINw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define BT_YC_HSPIr				(ValSft_R12(_rd32(_REG_BASE_+(0x0009<<2)))&BitMask_01)
#define BT_YC_HSPIw(val)		_wr32(_REG_BASE_+(0x0009<<2),(UINT)((_rd32(_REG_BASE_+(0x0009<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define BT_TRG_HSIr				(ValSft_R31(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BT_TRG_HSIw(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_TRG_VSIr				(ValSft_R30(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BT_TRG_VSIw(val)		_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YC_SMODr				(ValSft_R29(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define YC_SMODw(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define Y_MODr					(ValSft_R28(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define Y_MODw(val)				_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT656_DDRr				(ValSft_R16(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define BT656_DDRw(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define DBYP_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x000a<<2)))&BitMask_01)
#define DBYP_ONw(val)			_wr32(_REG_BASE_+(0x000a<<2),(UINT)((_rd32(_REG_BASE_+(0x000a<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define BT_SYNC_RN0r			(ValSft_R31(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_SYNC_RN0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_TRG_DIN0r			(ValSft_R29(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_TRG_DIN0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT_TRG_HSI0r			(ValSft_R28(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_TRG_HSI0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT_TRG_VSI0r			(ValSft_R27(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_TRG_VSI0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BT_POL_HSI0r			(ValSft_R26(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_POL_HSI0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BT_YC_HSPI0r			(ValSft_R25(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define BT_YC_HSPI0w(val)		_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define DBYP_ON0r				(ValSft_R24(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_01)
#define DBYP_ON0w(val)			_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_8B_DI_SELr		(ValSft_R00(_rd32(_REG_BASE_+(0x000b<<2)))&BitMask_04)
#define BT1120_8B_DI_SELw(val)	_wr32(_REG_BASE_+(0x000b<<2),(UINT)((_rd32(_REG_BASE_+(0x000b<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define BT656_DDR0r				(ValSft_R31(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT656_DDR0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FLD_INV0r				(ValSft_R30(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define FLD_INV0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YC_SMOD0r				(ValSft_R29(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define YC_SMOD0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define POL_HSI0r				(ValSft_R28(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define POL_HSI0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define POL_VSI0r				(ValSft_R27(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define POL_VSI0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ASYNC_ON0r				(ValSft_R26(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define ASYNC_ON0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSYN_NAON0r				(ValSft_R25(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define VSYN_NAON0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BT_MODESD0r				(ValSft_R24(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT_MODESD0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_MOD0r			(ValSft_R23(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT1120_MOD0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define Y_MOD0r					(ValSft_R22(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define Y_MOD0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define YC_MOD0r				(ValSft_R21(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define YC_MOD0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define YCBCR_MOD0r				(ValSft_R20(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define YCBCR_MOD0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define YC_FLG_INV0r			(ValSft_R19(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define YC_FLG_INV0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CBR_FLG_INV0r			(ValSft_R18(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define CBR_FLG_INV0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define CBR_INTP_ON0r			(ValSft_R17(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define CBR_INTP_ON0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BT_SM0r					(ValSft_R16(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_01)
#define BT_SM0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define ASYNC_VPOS0r			(ValSft_R03(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_13)
#define ASYNC_VPOS0w(val)		_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L03(BitMask_13)))|ValSft_L03((val)&BitMask_13)))

#define INSEL0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000c<<2)))&BitMask_03)
#define INSEL0w(val)			_wr32(_REG_BASE_+(0x000c<<2),(UINT)((_rd32(_REG_BASE_+(0x000c<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define ASYNC_DSYNC0r			(ValSft_R31(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_01)
#define ASYNC_DSYNC0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ASYNC_RST0r				(ValSft_R30(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_01)
#define ASYNC_RST0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT_OBUF_RST0r			(ValSft_R29(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_01)
#define BT_OBUF_RST0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT1120_SMOD0r			(ValSft_R28(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_01)
#define BT1120_SMOD0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define VLOCKI_POS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_12)
#define VLOCKI_POS0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI_POS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x000d<<2)))&BitMask_13)
#define HLOCKI_POS0w(val)		_wr32(_REG_BASE_+(0x000d<<2),(UINT)((_rd32(_REG_BASE_+(0x000d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPI0r					(ValSft_R16(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_13)
#define HSPI0w(val)				_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPI0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000e<<2)))&BitMask_12)
#define VSPI0w(val)				_wr32(_REG_BASE_+(0x000e<<2),(UINT)((_rd32(_REG_BASE_+(0x000e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_VON0r				(ValSft_R31(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define DSENS_VON0w(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_CKO_ON0r			(ValSft_R30(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_01)
#define DSENS_CKO_ON0w(val)		_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HWI0r					(ValSft_R16(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_13)
#define HWI0w(val)				_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWI0r					(ValSft_R00(_rd32(_REG_BASE_+(0x000f<<2)))&BitMask_12)
#define VWI0w(val)				_wr32(_REG_BASE_+(0x000f<<2),(UINT)((_rd32(_REG_BASE_+(0x000f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_CKI_ON0r			(ValSft_R31(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define DSENS_CKI_ON0w(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_HON0r				(ValSft_R30(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define DSENS_HON0w(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ASYNC_VST0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_13)
#define ASYNC_VST0w(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_HW0r				(ValSft_R07(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_09)
#define ASYNC_HW0w(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L07(BitMask_09)))|ValSft_L07((val)&BitMask_09)))

#define DDRFF_RS0r				(ValSft_R06(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define DDRFF_RS0w(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define WE_FLG_SEL0r			(ValSft_R05(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define WE_FLG_SEL0w(val)		_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CPAD_ON0r				(ValSft_R04(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_01)
#define CPAD_ON0w(val)			_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT1120_8B_DI_SEL0r		(ValSft_R00(_rd32(_REG_BASE_+(0x0010<<2)))&BitMask_04)
#define BT1120_8B_DI_SEL0w(val)	_wr32(_REG_BASE_+(0x0010<<2),(UINT)((_rd32(_REG_BASE_+(0x0010<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define BT_SYNC_RN1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_SYNC_RN1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_TRG_DIN1r			(ValSft_R29(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_TRG_DIN1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT_TRG_HSI1r			(ValSft_R28(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_TRG_HSI1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT_TRG_VSI1r			(ValSft_R27(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_TRG_VSI1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BT_POL_HSI1r			(ValSft_R26(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_POL_HSI1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BT_YC_HSPI1r			(ValSft_R25(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define BT_YC_HSPI1w(val)		_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define DBYP_ON1r				(ValSft_R24(_rd32(_REG_BASE_+(0x0011<<2)))&BitMask_01)
#define DBYP_ON1w(val)			_wr32(_REG_BASE_+(0x0011<<2),(UINT)((_rd32(_REG_BASE_+(0x0011<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT656_DDR1r				(ValSft_R31(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define BT656_DDR1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FLD_INV1r				(ValSft_R30(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define FLD_INV1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YC_SMOD1r				(ValSft_R29(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define YC_SMOD1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define POL_HSI1r				(ValSft_R28(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define POL_HSI1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define POL_VSI1r				(ValSft_R27(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define POL_VSI1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ASYNC_ON1r				(ValSft_R26(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define ASYNC_ON1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSYN_NAON1r				(ValSft_R25(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define VSYN_NAON1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BT_MODESD1r				(ValSft_R24(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define BT_MODESD1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_MOD1r			(ValSft_R23(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define BT1120_MOD1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define Y_MOD1r					(ValSft_R22(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define Y_MOD1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define YC_MOD1r				(ValSft_R21(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define YC_MOD1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define YCBCR_MOD1r				(ValSft_R20(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define YCBCR_MOD1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define YC_FLG_INV1r			(ValSft_R19(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define YC_FLG_INV1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CBR_FLG_INV1r			(ValSft_R18(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CBR_FLG_INV1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define CBR_INTP_ON1r			(ValSft_R17(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define CBR_INTP_ON1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BT_SM1r					(ValSft_R16(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_01)
#define BT_SM1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define ASYNC_VPOS1r			(ValSft_R03(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_13)
#define ASYNC_VPOS1w(val)		_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L03(BitMask_13)))|ValSft_L03((val)&BitMask_13)))

#define INSEL1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0012<<2)))&BitMask_03)
#define INSEL1w(val)			_wr32(_REG_BASE_+(0x0012<<2),(UINT)((_rd32(_REG_BASE_+(0x0012<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define ASYNC_DSYNC1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_01)
#define ASYNC_DSYNC1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ASYNC_RST1r				(ValSft_R30(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_01)
#define ASYNC_RST1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT_OBUF_RST1r			(ValSft_R29(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_01)
#define BT_OBUF_RST1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT1120_SMOD1r			(ValSft_R28(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_01)
#define BT1120_SMOD1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define VLOCKI_POS1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_12)
#define VLOCKI_POS1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define BT1120_16MOD1r			(ValSft_R15(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_01)
#define BT1120_16MOD1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define HLOCKI_POS1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0013<<2)))&BitMask_13)
#define HLOCKI_POS1w(val)		_wr32(_REG_BASE_+(0x0013<<2),(UINT)((_rd32(_REG_BASE_+(0x0013<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPI1r					(ValSft_R16(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_13)
#define HSPI1w(val)				_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPI1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0014<<2)))&BitMask_12)
#define VSPI1w(val)				_wr32(_REG_BASE_+(0x0014<<2),(UINT)((_rd32(_REG_BASE_+(0x0014<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_VON1r				(ValSft_R31(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_01)
#define DSENS_VON1w(val)		_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_CKO_ON1r			(ValSft_R30(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_01)
#define DSENS_CKO_ON1w(val)		_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HWI1r					(ValSft_R16(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_13)
#define HWI1w(val)				_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWI1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0015<<2)))&BitMask_12)
#define VWI1w(val)				_wr32(_REG_BASE_+(0x0015<<2),(UINT)((_rd32(_REG_BASE_+(0x0015<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_CKI_ON1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_01)
#define DSENS_CKI_ON1w(val)		_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_HON1r				(ValSft_R30(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_01)
#define DSENS_HON1w(val)		_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ASYNC_VST1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_13)
#define ASYNC_VST1w(val)		_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_HW1r				(ValSft_R07(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_09)
#define ASYNC_HW1w(val)			_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L07(BitMask_09)))|ValSft_L07((val)&BitMask_09)))

#define DDRFF_RS1r				(ValSft_R06(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_01)
#define DDRFF_RS1w(val)			_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define WE_FLG_SEL1r			(ValSft_R05(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_01)
#define WE_FLG_SEL1w(val)		_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CPAD_ON1r				(ValSft_R04(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_01)
#define CPAD_ON1w(val)			_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT1120_8B_DI_SEL1r		(ValSft_R00(_rd32(_REG_BASE_+(0x0016<<2)))&BitMask_04)
#define BT1120_8B_DI_SEL1w(val)	_wr32(_REG_BASE_+(0x0016<<2),(UINT)((_rd32(_REG_BASE_+(0x0016<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define BT_SYNC_RN2r			(ValSft_R31(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_SYNC_RN2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BT_TRG_DIN2r			(ValSft_R29(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_TRG_DIN2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT_TRG_HSI2r			(ValSft_R28(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_TRG_HSI2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BT_TRG_VSI2r			(ValSft_R27(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_TRG_VSI2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BT_POL_HSI2r			(ValSft_R26(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_POL_HSI2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BT_YC_HSPI2r			(ValSft_R25(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define BT_YC_HSPI2w(val)		_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define DBYP_ON2r				(ValSft_R24(_rd32(_REG_BASE_+(0x0017<<2)))&BitMask_01)
#define DBYP_ON2w(val)			_wr32(_REG_BASE_+(0x0017<<2),(UINT)((_rd32(_REG_BASE_+(0x0017<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT656_DDR2r				(ValSft_R31(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define BT656_DDR2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FLD_INV2r				(ValSft_R30(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define FLD_INV2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YC_SMOD2r				(ValSft_R29(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define YC_SMOD2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define POL_HSI2r				(ValSft_R28(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define POL_HSI2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define POL_VSI2r				(ValSft_R27(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define POL_VSI2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define ASYNC_ON2r				(ValSft_R26(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define ASYNC_ON2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSYN_NAON2r				(ValSft_R25(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define VSYN_NAON2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BT_MODESD2r				(ValSft_R24(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define BT_MODESD2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BT1120_MOD2r			(ValSft_R23(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define BT1120_MOD2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define Y_MOD2r					(ValSft_R22(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define Y_MOD2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define YC_MOD2r				(ValSft_R21(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define YC_MOD2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define YCBCR_MOD2r				(ValSft_R20(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define YCBCR_MOD2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define YC_FLG_INV2r			(ValSft_R19(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define YC_FLG_INV2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CBR_FLG_INV2r			(ValSft_R18(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define CBR_FLG_INV2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define CBR_INTP_ON2r			(ValSft_R17(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define CBR_INTP_ON2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BT_SM2r					(ValSft_R16(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_01)
#define BT_SM2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define ASYNC_VPOS2r			(ValSft_R03(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_13)
#define ASYNC_VPOS2w(val)		_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L03(BitMask_13)))|ValSft_L03((val)&BitMask_13)))

#define INSEL2r					(ValSft_R00(_rd32(_REG_BASE_+(0x0018<<2)))&BitMask_03)
#define INSEL2w(val)			_wr32(_REG_BASE_+(0x0018<<2),(UINT)((_rd32(_REG_BASE_+(0x0018<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define ASYNC_DSYNC2r			(ValSft_R31(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_01)
#define ASYNC_DSYNC2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ASYNC_RST2r				(ValSft_R30(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_01)
#define ASYNC_RST2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BT_OBUF_RST2r			(ValSft_R29(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_01)
#define BT_OBUF_RST2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT1120_SMOD2r			(ValSft_R28(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_01)
#define BT1120_SMOD2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define VLOCKI_POS2r			(ValSft_R16(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_12)
#define VLOCKI_POS2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI_POS2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0019<<2)))&BitMask_13)
#define HLOCKI_POS2w(val)		_wr32(_REG_BASE_+(0x0019<<2),(UINT)((_rd32(_REG_BASE_+(0x0019<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPI2r					(ValSft_R16(_rd32(_REG_BASE_+(0x001a<<2)))&BitMask_13)
#define HSPI2w(val)				_wr32(_REG_BASE_+(0x001a<<2),(UINT)((_rd32(_REG_BASE_+(0x001a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPI2r					(ValSft_R00(_rd32(_REG_BASE_+(0x001a<<2)))&BitMask_12)
#define VSPI2w(val)				_wr32(_REG_BASE_+(0x001a<<2),(UINT)((_rd32(_REG_BASE_+(0x001a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_VON2r				(ValSft_R31(_rd32(_REG_BASE_+(0x001b<<2)))&BitMask_01)
#define DSENS_VON2w(val)		_wr32(_REG_BASE_+(0x001b<<2),(UINT)((_rd32(_REG_BASE_+(0x001b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_CKO_ON2r			(ValSft_R30(_rd32(_REG_BASE_+(0x001b<<2)))&BitMask_01)
#define DSENS_CKO_ON2w(val)		_wr32(_REG_BASE_+(0x001b<<2),(UINT)((_rd32(_REG_BASE_+(0x001b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HWI2r					(ValSft_R16(_rd32(_REG_BASE_+(0x001b<<2)))&BitMask_13)
#define HWI2w(val)				_wr32(_REG_BASE_+(0x001b<<2),(UINT)((_rd32(_REG_BASE_+(0x001b<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWI2r					(ValSft_R00(_rd32(_REG_BASE_+(0x001b<<2)))&BitMask_12)
#define VWI2w(val)				_wr32(_REG_BASE_+(0x001b<<2),(UINT)((_rd32(_REG_BASE_+(0x001b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DSENS_CKI_ON2r			(ValSft_R31(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define DSENS_CKI_ON2w(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DSENS_HON2r				(ValSft_R30(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define DSENS_HON2w(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ASYNC_VST2r				(ValSft_R16(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_13)
#define ASYNC_VST2w(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_HW2r				(ValSft_R07(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_09)
#define ASYNC_HW2w(val)			_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L07(BitMask_09)))|ValSft_L07((val)&BitMask_09)))

#define DDRFF_RS2r				(ValSft_R06(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define DDRFF_RS2w(val)			_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define WE_FLG_SEL2r			(ValSft_R05(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define WE_FLG_SEL2w(val)		_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CPAD_ON2r				(ValSft_R04(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_01)
#define CPAD_ON2w(val)			_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BT1120_8B_DI_SEL2r		(ValSft_R00(_rd32(_REG_BASE_+(0x001c<<2)))&BitMask_04)
#define BT1120_8B_DI_SEL2w(val)	_wr32(_REG_BASE_+(0x001c<<2),(UINT)((_rd32(_REG_BASE_+(0x001c<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define TDM_DI_SEL0r			(ValSft_R19(_rd32(_REG_BASE_+(0x001d<<2)))&BitMask_01)
#define TDM_DI_SEL0w(val)		_wr32(_REG_BASE_+(0x001d<<2),(UINT)((_rd32(_REG_BASE_+(0x001d<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define TDM_BT656_0r			(ValSft_R17(_rd32(_REG_BASE_+(0x001d<<2)))&BitMask_01)
#define TDM_BT656_0w(val)		_wr32(_REG_BASE_+(0x001d<<2),(UINT)((_rd32(_REG_BASE_+(0x001d<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define TDM_CHIP_MON0r			(ValSft_R16(_rd32(_REG_BASE_+(0x001d<<2)))&BitMask_01)
#define TDM_CHIP_MON0w(val)		_wr32(_REG_BASE_+(0x001d<<2),(UINT)((_rd32(_REG_BASE_+(0x001d<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define TDM_CHID0_0r			(ValSft_R12(_rd32(_REG_BASE_+(0x001d<<2)))&BitMask_04)
#define TDM_CHID0_0w(val)		_wr32(_REG_BASE_+(0x001d<<2),(UINT)((_rd32(_REG_BASE_+(0x001d<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define TDM_CHID1_0r			(ValSft_R08(_rd32(_REG_BASE_+(0x001d<<2)))&BitMask_04)
#define TDM_CHID1_0w(val)		_wr32(_REG_BASE_+(0x001d<<2),(UINT)((_rd32(_REG_BASE_+(0x001d<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define TDM_DI_SEL1r			(ValSft_R19(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_01)
#define TDM_DI_SEL1w(val)		_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define TDM_BT656_1r			(ValSft_R17(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_01)
#define TDM_BT656_1w(val)		_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define TDM_CHIP_MON1r			(ValSft_R16(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_01)
#define TDM_CHIP_MON1w(val)		_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define TDM_CHID0_1r			(ValSft_R12(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_04)
#define TDM_CHID0_1w(val)		_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define TDM_CHID1_1r			(ValSft_R08(_rd32(_REG_BASE_+(0x001e<<2)))&BitMask_04)
#define TDM_CHID1_1w(val)		_wr32(_REG_BASE_+(0x001e<<2),(UINT)((_rd32(_REG_BASE_+(0x001e<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define DZPCK2_HONr				(ValSft_R29(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define DZPCK2_HONw(val)		_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BT1120_PCK_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_02)
#define BT1120_PCK_SELw(val)	_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BT1120_PCK0_SELr		(ValSft_R20(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_02)
#define BT1120_PCK0_SELw(val)	_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BT1120_PCK1_SELr		(ValSft_R18(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define BT1120_PCK1_SELw(val)	_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define BT1120_PCK2_SELr		(ValSft_R16(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define BT1120_PCK2_SELw(val)	_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define TDM_PCK0_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define TDM_PCK0_SELw(val)		_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define TDM_DCK0_SELr			(ValSft_R08(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define TDM_DCK0_SELw(val)		_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define TDM_PCK1_SELr			(ValSft_R04(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define TDM_PCK1_SELw(val)		_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define TDM_DCK1_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0020<<2)))&BitMask_01)
#define TDM_DCK1_SELw(val)		_wr32(_REG_BASE_+(0x0020<<2),(UINT)((_rd32(_REG_BASE_+(0x0020<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define PCK_EMAX0r				(ValSft_R27(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_05)
#define PCK_EMAX0w(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L27(BitMask_05)))|ValSft_L27((val)&BitMask_05)))

#define PCK_EHVL0r				(ValSft_R22(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_05)
#define PCK_EHVL0w(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L22(BitMask_05)))|ValSft_L22((val)&BitMask_05)))

#define PCK_EMAX1r				(ValSft_R17(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_05)
#define PCK_EMAX1w(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L17(BitMask_05)))|ValSft_L17((val)&BitMask_05)))

#define PCK_EHVL1r				(ValSft_R12(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_05)
#define PCK_EHVL1w(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L12(BitMask_05)))|ValSft_L12((val)&BitMask_05)))

#define PCK_EMAX2r				(ValSft_R02(_rd32(_REG_BASE_+(0x0021<<2)))&BitMask_10)
#define PCK_EMAX2w(val)			_wr32(_REG_BASE_+(0x0021<<2),(UINT)((_rd32(_REG_BASE_+(0x0021<<2))&(~ValSft_L02(BitMask_10)))|ValSft_L02((val)&BitMask_10)))

#define JDEC_CK_SELr			(ValSft_R30(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_02)
#define JDEC_CK_SELw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define PCK_EHVL2r				(ValSft_R20(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_10)
#define PCK_EHVL2w(val)			_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L20(BitMask_10)))|ValSft_L20((val)&BitMask_10)))

#define CDS0_PCK_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_04)
#define CDS0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define CDS1_PCK_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_04)
#define CDS1_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define CDS2_PCK_SELr			(ValSft_R08(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_04)
#define CDS2_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define CDS3_PCK_SELr			(ValSft_R04(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_04)
#define CDS3_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define JPGCK_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0022<<2)))&BitMask_04)
#define JPGCK_SELw(val)			_wr32(_REG_BASE_+(0x0022<<2),(UINT)((_rd32(_REG_BASE_+(0x0022<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define IBT_PCK_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define IBT_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BT_PCK_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_03)
#define BT_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IBT0_PCK_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define IBT0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BT0_PCK_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define BT0_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define IBT1_PCK_SELr			(ValSft_R10(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define IBT1_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define BT1_PCK_SELr			(ValSft_R06(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define BT1_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define IBT2_PCK_SELr			(ValSft_R03(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define IBT2_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L03(BitMask_02)))|ValSft_L03((val)&BitMask_02)))

#define BT2_PCK_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0023<<2)))&BitMask_02)
#define BT2_PCK_SELw(val)		_wr32(_REG_BASE_+(0x0023<<2),(UINT)((_rd32(_REG_BASE_+(0x0023<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define RDES_CK_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_01)
#define RDES_CK_SELw(val)		_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FPCK_SELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_03)
#define FPCK_SELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define PPCK_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_03)
#define PPCK_SELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define PSCK_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_03)
#define PSCK_SELw(val)			_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define FN_CK0_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_04)
#define FN_CK0_SELw(val)		_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define PR_CK0_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_04)
#define PR_CK0_SELw(val)		_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define IMD_CK_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0024<<2)))&BitMask_04)
#define IMD_CK_SELw(val)		_wr32(_REG_BASE_+(0x0024<<2),(UINT)((_rd32(_REG_BASE_+(0x0024<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define YCW_CK0_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_04)
#define YCW_CK0_SELw(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define YCW_CK1_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_04)
#define YCW_CK1_SELw(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define YCW_CK2_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_04)
#define YCW_CK2_SELw(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define YCW_CK3_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_04)
#define YCW_CK3_SELw(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define YCW_CK4_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0025<<2)))&BitMask_04)
#define YCW_CK4_SELw(val)		_wr32(_REG_BASE_+(0x0025<<2),(UINT)((_rd32(_REG_BASE_+(0x0025<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define YCRCK0_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_04)
#define YCRCK0_SELw(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define YCRCK1_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_04)
#define YCRCK1_SELw(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define YCRCK2_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_04)
#define YCRCK2_SELw(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define YCRCK3_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_04)
#define YCRCK3_SELw(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define YCRCK4_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0026<<2)))&BitMask_04)
#define YCRCK4_SELw(val)		_wr32(_REG_BASE_+(0x0026<<2),(UINT)((_rd32(_REG_BASE_+(0x0026<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define PCK_EMAX3r				(ValSft_R24(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_08)
#define PCK_EMAX3w(val)			_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define PCK_EHVL3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_08)
#define PCK_EHVL3w(val)			_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define RYCCK1_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_04)
#define RYCCK1_SELw(val)		_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define RYCCK2_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_04)
#define RYCCK2_SELw(val)		_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define RYCCK3_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0027<<2)))&BitMask_04)
#define RYCCK3_SELw(val)		_wr32(_REG_BASE_+(0x0027<<2),(UINT)((_rd32(_REG_BASE_+(0x0027<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define DCKO2_INVr				(ValSft_R31(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_01)
#define DCKO2_INVw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DCKO2_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_04)
#define DCKO2_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define ITCK0_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_04)
#define ITCK0_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define ITCK1_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_04)
#define ITCK1_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define PIP_PPCK_SELr			(ValSft_R09(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_04)
#define PIP_PPCK_SELw(val)		_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L09(BitMask_04)))|ValSft_L09((val)&BitMask_04)))

#define ECK_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0028<<2)))&BitMask_02)
#define ECK_SELw(val)			_wr32(_REG_BASE_+(0x0028<<2),(UINT)((_rd32(_REG_BASE_+(0x0028<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define DCKO0_INVr				(ValSft_R31(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_01)
#define DCKO0_INVw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DCKO0_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_04)
#define DCKO0_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define DCKO1_INVr				(ValSft_R23(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_01)
#define DCKO1_INVw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define DCKO1_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_04)
#define DCKO1_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define PCKO_CK_SELr			(ValSft_R15(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_01)
#define PCKO_CK_SELw(val)		_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define PCKO_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_03)
#define PCKO_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define PCKO0_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_03)
#define PCKO0_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define PCKO1_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_03)
#define PCKO1_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L04(BitMask_03)))|ValSft_L04((val)&BitMask_03)))

#define PCKO2_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0029<<2)))&BitMask_03)
#define PCKO2_SELw(val)			_wr32(_REG_BASE_+(0x0029<<2),(UINT)((_rd32(_REG_BASE_+(0x0029<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define DO0_CK_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_04)
#define DO0_CK_SELw(val)		_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define DO1_CK_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_04)
#define DO1_CK_SELw(val)		_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define DO2_CK_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_04)
#define DO2_CK_SELw(val)		_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define DO3_CK_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x002a<<2)))&BitMask_04)
#define DO3_CK_SELw(val)		_wr32(_REG_BASE_+(0x002a<<2),(UINT)((_rd32(_REG_BASE_+(0x002a<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define IMGM_CK_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_02)
#define IMGM_CK_SELw(val)		_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PSE_CK_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_04)
#define PSE_CK_SELw(val)		_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define LCD_CK_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_04)
#define LCD_CK_SELw(val)		_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define IP_UIRQ0_CK_SELr		(ValSft_R04(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_04)
#define IP_UIRQ0_CK_SELw(val)	_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define IP_UIRQ1_CK_SELr		(ValSft_R00(_rd32(_REG_BASE_+(0x002b<<2)))&BitMask_04)
#define IP_UIRQ1_CK_SELw(val)	_wr32(_REG_BASE_+(0x002b<<2),(UINT)((_rd32(_REG_BASE_+(0x002b<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define DITCK0_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_04)
#define DITCK0_SELw(val)		_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define DITCK1_SELr				(ValSft_R16(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_04)
#define DITCK1_SELw(val)		_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define OTCK0_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_04)
#define OTCK0_SELw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define OTCK1_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x002c<<2)))&BitMask_04)
#define OTCK1_SELw(val)			_wr32(_REG_BASE_+(0x002c<<2),(UINT)((_rd32(_REG_BASE_+(0x002c<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define CDS0_HONr				(ValSft_R31(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_01)
#define CDS0_HONw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CDS0_HSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_03)
#define CDS0_HSELw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define CDS1_HONr				(ValSft_R27(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_01)
#define CDS1_HONw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define CDS1_HSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_03)
#define CDS1_HSELw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define CDS2_HONr				(ValSft_R23(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_01)
#define CDS2_HONw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define CDS2_HSELr				(ValSft_R20(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_03)
#define CDS2_HSELw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define CDS3_HONr				(ValSft_R19(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_01)
#define CDS3_HONw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define CDS3_HSELr				(ValSft_R16(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_03)
#define CDS3_HSELw(val)			_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define OSYNCI_CK_SELr			(ValSft_R08(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_04)
#define OSYNCI_CK_SELw(val)		_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define HVSO_CK_SELr			(ValSft_R04(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_04)
#define HVSO_CK_SELw(val)		_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define LCDO_CK_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x002d<<2)))&BitMask_04)
#define LCDO_CK_SELw(val)		_wr32(_REG_BASE_+(0x002d<<2),(UINT)((_rd32(_REG_BASE_+(0x002d<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define YCR0_HLOCKI_SELr		(ValSft_R29(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_03)
#define YCR0_HLOCKI_SELw(val)	_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define YCR1_HLOCKI_SELr		(ValSft_R26(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_03)
#define YCR1_HLOCKI_SELw(val)	_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L26(BitMask_03)))|ValSft_L26((val)&BitMask_03)))

#define YCR2_HLOCKI_SELr		(ValSft_R23(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_03)
#define YCR2_HLOCKI_SELw(val)	_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L23(BitMask_03)))|ValSft_L23((val)&BitMask_03)))

#define YCR3_HLOCKI_SELr		(ValSft_R20(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_03)
#define YCR3_HLOCKI_SELw(val)	_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define DZ_HLOCKI_SELr			(ValSft_R17(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_03)
#define DZ_HLOCKI_SELw(val)		_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L17(BitMask_03)))|ValSft_L17((val)&BitMask_03)))

#define DZPCK_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_04)
#define DZPCK_SELw(val)			_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define ALP_CK0_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x002e<<2)))&BitMask_04)
#define ALP_CK0_SELw(val)		_wr32(_REG_BASE_+(0x002e<<2),(UINT)((_rd32(_REG_BASE_+(0x002e<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define IMD_HONr				(ValSft_R23(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define IMD_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IMD_HSELr				(ValSft_R20(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_03)
#define IMD_HSELw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define YCR0_HONr				(ValSft_R19(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define YCR0_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define YCR1_HONr				(ValSft_R18(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define YCR1_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define YCR2_HONr				(ValSft_R17(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define YCR2_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define YCR3_HONr				(ValSft_R16(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define YCR3_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define YCR4_HONr				(ValSft_R15(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define YCR4_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define YCR4_HLOCKI_SELr		(ValSft_R12(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_03)
#define YCR4_HLOCKI_SELw(val)	_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define JPG_HSELr				(ValSft_R09(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_03)
#define JPG_HSELw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L09(BitMask_03)))|ValSft_L09((val)&BitMask_03)))

#define JPG_HONr				(ValSft_R08(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define JPG_HONw(val)			_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define FN_APPCK0_HONr			(ValSft_R07(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_01)
#define FN_APPCK0_HONw(val)		_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define FN_APPCK0_HSELr			(ValSft_R04(_rd32(_REG_BASE_+(0x002f<<2)))&BitMask_03)
#define FN_APPCK0_HSELw(val)	_wr32(_REG_BASE_+(0x002f<<2),(UINT)((_rd32(_REG_BASE_+(0x002f<<2))&(~ValSft_L04(BitMask_03)))|ValSft_L04((val)&BitMask_03)))

#define PCKO_OENr				(ValSft_R31(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define PCKO_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define SDES_PCr				(ValSft_R21(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_05)
#define SDES_PCw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L21(BitMask_05)))|ValSft_L21((val)&BitMask_05)))

#define SDES_PDr				(ValSft_R12(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_05)
#define SDES_PDw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L12(BitMask_05)))|ValSft_L12((val)&BitMask_05)))

#define DCKO0_OENr				(ValSft_R11(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define DCKO0_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define DCKO1_OENr				(ValSft_R10(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define DCKO1_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define DCKO2_OENr				(ValSft_R09(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define DCKO2_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define VSO_OENr				(ValSft_R08(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define VSO_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define HSO_OENr				(ValSft_R07(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define HSO_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define DENO_OENr				(ValSft_R06(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define DENO_OENw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define DAC_PDr					(ValSft_R05(_rd32(_REG_BASE_+(0x0030<<2)))&BitMask_01)
#define DAC_PDw(val)			_wr32(_REG_BASE_+(0x0030<<2),(UINT)((_rd32(_REG_BASE_+(0x0030<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define DO_OENr					(ValSft_R00(_rd32(_REG_BASE_+(0x0031<<2)))&BitMask_32)
#define DO_OENw(val)			_wr32(_REG_BASE_+(0x0031<<2),(UINT)((_rd32(_REG_BASE_+(0x0031<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define EXTCDI_MUXr				(ValSft_R31(_rd32(_REG_BASE_+(0x0032<<2)))&BitMask_01)
#define EXTCDI_MUXw(val)		_wr32(_REG_BASE_+(0x0032<<2),(UINT)((_rd32(_REG_BASE_+(0x0032<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DO_MUX_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0032<<2)))&BitMask_01)
#define DO_MUX_ONw(val)			_wr32(_REG_BASE_+(0x0032<<2),(UINT)((_rd32(_REG_BASE_+(0x0032<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define SLVr					(ValSft_R30(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_02)
#define SLVw(val)				_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define CH_DSELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define CH_DSELw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CH_HSSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define CH_HSSELw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define CH_VSSELr				(ValSft_R27(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define CH_VSSELw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define TRG_DINr				(ValSft_R26(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define TRG_DINw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define TRG_HSIr				(ValSft_R25(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define TRG_HSIw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define TRG_VSIr				(ValSft_R24(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define TRG_VSIw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define POL_HSIr				(ValSft_R23(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define POL_HSIw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define POL_VSIr				(ValSft_R22(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define POL_VSIw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define SHSO_INVr				(ValSft_R21(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SHSO_INVw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define SVSO_INVr				(ValSft_R20(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SVSO_INVw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define SHSO_TRGr				(ValSft_R19(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SHSO_TRGw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define SVSO_TRGr				(ValSft_R18(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SVSO_TRGw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IDDR_ONr				(ValSft_R17(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define IDDR_ONw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define ISDR_OPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define ISDR_OPw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define DDRFF_RSr				(ValSft_R15(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define DDRFF_RSw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define SORDr					(ValSft_R14(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SORDw(val)				_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define CATPr					(ValSft_R13(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define CATPw(val)				_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define ASYNC_ONr				(ValSft_R12(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define ASYNC_ONw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define VSYN_NAONr				(ValSft_R11(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define VSYN_NAONw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define PDI_MODr				(ValSft_R09(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_02)
#define PDI_MODw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L09(BitMask_02)))|ValSft_L09((val)&BitMask_02)))

#define SYNC_BYSr				(ValSft_R08(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define SYNC_BYSw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define BT_SYNC_RNr				(ValSft_R07(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define BT_SYNC_RNw(val)		_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define ICSELr					(ValSft_R04(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_02)
#define ICSELw(val)				_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define INSEL_MODr				(ValSft_R03(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_01)
#define INSEL_MODw(val)			_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define INSELr					(ValSft_R00(_rd32(_REG_BASE_+(0x0033<<2)))&BitMask_03)
#define INSELw(val)				_wr32(_REG_BASE_+(0x0033<<2),(UINT)((_rd32(_REG_BASE_+(0x0033<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define HTWIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0034<<2)))&BitMask_16)
#define HTWIw(val)				_wr32(_REG_BASE_+(0x0034<<2),(UINT)((_rd32(_REG_BASE_+(0x0034<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define VTWIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0034<<2)))&BitMask_16)
#define VTWIw(val)				_wr32(_REG_BASE_+(0x0034<<2),(UINT)((_rd32(_REG_BASE_+(0x0034<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define PRS_HZr					(ValSft_R31(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define PRS_HZw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define POS_HZr					(ValSft_R30(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define POS_HZw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define FCNT_RSr				(ValSft_R29(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_01)
#define FCNT_RSw(val)			_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define VLOCKI_POSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_12)
#define VLOCKI_POSw(val)		_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0035<<2)))&BitMask_13)
#define HLOCKI_POSw(val)		_wr32(_REG_BASE_+(0x0035<<2),(UINT)((_rd32(_REG_BASE_+(0x0035<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_13)
#define HSPIw(val)				_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0036<<2)))&BitMask_12)
#define VSPIw(val)				_wr32(_REG_BASE_+(0x0036<<2),(UINT)((_rd32(_REG_BASE_+(0x0036<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWIr					(ValSft_R16(_rd32(_REG_BASE_+(0x0037<<2)))&BitMask_13)
#define HWIw(val)				_wr32(_REG_BASE_+(0x0037<<2),(UINT)((_rd32(_REG_BASE_+(0x0037<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWIr					(ValSft_R00(_rd32(_REG_BASE_+(0x0037<<2)))&BitMask_12)
#define VWIw(val)				_wr32(_REG_BASE_+(0x0037<<2),(UINT)((_rd32(_REG_BASE_+(0x0037<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HTWOr					(ValSft_R16(_rd32(_REG_BASE_+(0x0038<<2)))&BitMask_16)
#define HTWOw(val)				_wr32(_REG_BASE_+(0x0038<<2),(UINT)((_rd32(_REG_BASE_+(0x0038<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define VTWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x0038<<2)))&BitMask_16)
#define VTWOw(val)				_wr32(_REG_BASE_+(0x0038<<2),(UINT)((_rd32(_REG_BASE_+(0x0038<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define OSYNC_MODr				(ValSft_R31(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_01)
#define OSYNC_MODw(val)			_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define TRG_OVSYNCIr			(ValSft_R30(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_01)
#define TRG_OVSYNCIw(val)		_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define TRG_OHSYNCIr			(ValSft_R29(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_01)
#define TRG_OHSYNCIw(val)		_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define OSYNC_MOD_ZERO_ALr		(ValSft_R28(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_01)
#define OSYNC_MOD_ZERO_ALw(val)	_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define VLOCKI2_POSr			(ValSft_R16(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_12)
#define VLOCKI2_POSw(val)		_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKI2_POSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0039<<2)))&BitMask_13)
#define HLOCKI2_POSw(val)		_wr32(_REG_BASE_+(0x0039<<2),(UINT)((_rd32(_REG_BASE_+(0x0039<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define EXVSYNC_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_01)
#define EXVSYNC_SELw(val)		_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define EXHSYNC_SELr			(ValSft_R30(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_01)
#define EXHSYNC_SELw(val)		_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HSPOr					(ValSft_R16(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_13)
#define HSPOw(val)				_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define POL_OVSYNCIr			(ValSft_R15(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_01)
#define POL_OVSYNCIw(val)		_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define POL_OHSYNCIr			(ValSft_R14(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_01)
#define POL_OHSYNCIw(val)		_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define VSPOr					(ValSft_R00(_rd32(_REG_BASE_+(0x003a<<2)))&BitMask_12)
#define VSPOw(val)				_wr32(_REG_BASE_+(0x003a<<2),(UINT)((_rd32(_REG_BASE_+(0x003a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HVWIO_SYNr				(ValSft_R31(_rd32(_REG_BASE_+(0x003b<<2)))&BitMask_01)
#define HVWIO_SYNw(val)			_wr32(_REG_BASE_+(0x003b<<2),(UINT)((_rd32(_REG_BASE_+(0x003b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HWO_1D_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x003b<<2)))&BitMask_01)
#define HWO_1D_ONw(val)			_wr32(_REG_BASE_+(0x003b<<2),(UINT)((_rd32(_REG_BASE_+(0x003b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HWOr					(ValSft_R16(_rd32(_REG_BASE_+(0x003b<<2)))&BitMask_13)
#define HWOw(val)				_wr32(_REG_BASE_+(0x003b<<2),(UINT)((_rd32(_REG_BASE_+(0x003b<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x003b<<2)))&BitMask_12)
#define VWOw(val)				_wr32(_REG_BASE_+(0x003b<<2),(UINT)((_rd32(_REG_BASE_+(0x003b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SENSOR_WIDr				(ValSft_R08(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_08)
#define SENSOR_WIDw(val)		_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ISSPILOGICr				(ValSft_R05(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_01)
#define ISSPILOGICw(val)		_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define LSPI_RSTNr				(ValSft_R03(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_01)
#define LSPI_RSTNw(val)			_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define SSTYPE_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x003c<<2)))&BitMask_01)
#define SSTYPE_SELw(val)		_wr32(_REG_BASE_+(0x003c<<2),(UINT)((_rd32(_REG_BASE_+(0x003c<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define HSO_INV0r				(ValSft_R29(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define HSO_INV0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define VSO_INV0r				(ValSft_R28(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define VSO_INV0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define HSO_OEN0r				(ValSft_R27(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define HSO_OEN0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define VSO_OEN0r				(ValSft_R26(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define VSO_OEN0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSO_TRG0r				(ValSft_R25(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define VSO_TRG0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define VLOCKO_IT_TRG0r			(ValSft_R22(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_01)
#define VLOCKO_IT_TRG0w(val)	_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define OUT_SEL0r				(ValSft_R19(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_03)
#define OUT_SEL0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L19(BitMask_03)))|ValSft_L19((val)&BitMask_03)))

#define ITU_HTWO0r				(ValSft_R00(_rd32(_REG_BASE_+(0x003d<<2)))&BitMask_13)
#define ITU_HTWO0w(val)			_wr32(_REG_BASE_+(0x003d<<2),(UINT)((_rd32(_REG_BASE_+(0x003d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HV_RS0r					(ValSft_R31(_rd32(_REG_BASE_+(0x003e<<2)))&BitMask_01)
#define HV_RS0w(val)			_wr32(_REG_BASE_+(0x003e<<2),(UINT)((_rd32(_REG_BASE_+(0x003e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VLOCKO_IT_POS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x003e<<2)))&BitMask_12)
#define VLOCKO_IT_POS0w(val)	_wr32(_REG_BASE_+(0x003e<<2),(UINT)((_rd32(_REG_BASE_+(0x003e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKO_IT_POS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x003e<<2)))&BitMask_13)
#define HLOCKO_IT_POS0w(val)	_wr32(_REG_BASE_+(0x003e<<2),(UINT)((_rd32(_REG_BASE_+(0x003e<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPC0r					(ValSft_R16(_rd32(_REG_BASE_+(0x003f<<2)))&BitMask_13)
#define HSPC0w(val)				_wr32(_REG_BASE_+(0x003f<<2),(UINT)((_rd32(_REG_BASE_+(0x003f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPC0r					(ValSft_R00(_rd32(_REG_BASE_+(0x003f<<2)))&BitMask_12)
#define VSPC0w(val)				_wr32(_REG_BASE_+(0x003f<<2),(UINT)((_rd32(_REG_BASE_+(0x003f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWC0r					(ValSft_R16(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_13)
#define HWC0w(val)				_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWC0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0040<<2)))&BitMask_12)
#define VWC0w(val)				_wr32(_REG_BASE_+(0x0040<<2),(UINT)((_rd32(_REG_BASE_+(0x0040<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSRP0r					(ValSft_R16(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_13)
#define HSRP0w(val)				_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define HSFP0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0043<<2)))&BitMask_13)
#define HSFP0w(val)				_wr32(_REG_BASE_+(0x0043<<2),(UINT)((_rd32(_REG_BASE_+(0x0043<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define VSRP0r					(ValSft_R16(_rd32(_REG_BASE_+(0x0044<<2)))&BitMask_12)
#define VSRP0w(val)				_wr32(_REG_BASE_+(0x0044<<2),(UINT)((_rd32(_REG_BASE_+(0x0044<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSFP0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0044<<2)))&BitMask_12)
#define VSFP0w(val)				_wr32(_REG_BASE_+(0x0044<<2),(UINT)((_rd32(_REG_BASE_+(0x0044<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DERP0r					(ValSft_R16(_rd32(_REG_BASE_+(0x0045<<2)))&BitMask_13)
#define DERP0w(val)				_wr32(_REG_BASE_+(0x0045<<2),(UINT)((_rd32(_REG_BASE_+(0x0045<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DEFP0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0045<<2)))&BitMask_13)
#define DEFP0w(val)				_wr32(_REG_BASE_+(0x0045<<2),(UINT)((_rd32(_REG_BASE_+(0x0045<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DENO_HST0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0046<<2)))&BitMask_13)
#define DENO_HST0w(val)			_wr32(_REG_BASE_+(0x0046<<2),(UINT)((_rd32(_REG_BASE_+(0x0046<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSO_INV1r				(ValSft_R29(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define HSO_INV1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define VSO_INV1r				(ValSft_R28(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define VSO_INV1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define HSO_OEN1r				(ValSft_R27(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define HSO_OEN1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define VSO_OEN1r				(ValSft_R26(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define VSO_OEN1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define VSO_TRG1r				(ValSft_R25(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define VSO_TRG1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define JLDO_INV1_Sr			(ValSft_R24(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define JLDO_INV1_Sw(val)		_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define JLDO_INV1r				(ValSft_R23(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define JLDO_INV1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define VLOCKO_IT_TRG1r			(ValSft_R22(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_01)
#define VLOCKO_IT_TRG1w(val)	_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define OUT_SEL1r				(ValSft_R19(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_03)
#define OUT_SEL1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L19(BitMask_03)))|ValSft_L19((val)&BitMask_03)))

#define ITU_HTWO1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0047<<2)))&BitMask_13)
#define ITU_HTWO1w(val)			_wr32(_REG_BASE_+(0x0047<<2),(UINT)((_rd32(_REG_BASE_+(0x0047<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HV_RS1r					(ValSft_R31(_rd32(_REG_BASE_+(0x0048<<2)))&BitMask_01)
#define HV_RS1w(val)			_wr32(_REG_BASE_+(0x0048<<2),(UINT)((_rd32(_REG_BASE_+(0x0048<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VLOCKO_IT_POS1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0048<<2)))&BitMask_12)
#define VLOCKO_IT_POS1w(val)	_wr32(_REG_BASE_+(0x0048<<2),(UINT)((_rd32(_REG_BASE_+(0x0048<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HLOCKO_IT_POS1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0048<<2)))&BitMask_13)
#define HLOCKO_IT_POS1w(val)	_wr32(_REG_BASE_+(0x0048<<2),(UINT)((_rd32(_REG_BASE_+(0x0048<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HSPC1r					(ValSft_R16(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_13)
#define HSPC1w(val)				_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VSPC1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0049<<2)))&BitMask_12)
#define VSPC1w(val)				_wr32(_REG_BASE_+(0x0049<<2),(UINT)((_rd32(_REG_BASE_+(0x0049<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWC1r					(ValSft_R16(_rd32(_REG_BASE_+(0x004a<<2)))&BitMask_13)
#define HWC1w(val)				_wr32(_REG_BASE_+(0x004a<<2),(UINT)((_rd32(_REG_BASE_+(0x004a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VWC1r					(ValSft_R00(_rd32(_REG_BASE_+(0x004a<<2)))&BitMask_12)
#define VWC1w(val)				_wr32(_REG_BASE_+(0x004a<<2),(UINT)((_rd32(_REG_BASE_+(0x004a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSRP1r					(ValSft_R16(_rd32(_REG_BASE_+(0x004d<<2)))&BitMask_13)
#define HSRP1w(val)				_wr32(_REG_BASE_+(0x004d<<2),(UINT)((_rd32(_REG_BASE_+(0x004d<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define HSFP1r					(ValSft_R00(_rd32(_REG_BASE_+(0x004d<<2)))&BitMask_13)
#define HSFP1w(val)				_wr32(_REG_BASE_+(0x004d<<2),(UINT)((_rd32(_REG_BASE_+(0x004d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define VSRP1r					(ValSft_R16(_rd32(_REG_BASE_+(0x004e<<2)))&BitMask_12)
#define VSRP1w(val)				_wr32(_REG_BASE_+(0x004e<<2),(UINT)((_rd32(_REG_BASE_+(0x004e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSFP1r					(ValSft_R00(_rd32(_REG_BASE_+(0x004e<<2)))&BitMask_12)
#define VSFP1w(val)				_wr32(_REG_BASE_+(0x004e<<2),(UINT)((_rd32(_REG_BASE_+(0x004e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DERP1r					(ValSft_R16(_rd32(_REG_BASE_+(0x004f<<2)))&BitMask_13)
#define DERP1w(val)				_wr32(_REG_BASE_+(0x004f<<2),(UINT)((_rd32(_REG_BASE_+(0x004f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DEFP1r					(ValSft_R00(_rd32(_REG_BASE_+(0x004f<<2)))&BitMask_13)
#define DEFP1w(val)				_wr32(_REG_BASE_+(0x004f<<2),(UINT)((_rd32(_REG_BASE_+(0x004f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DENO_HST1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0050<<2)))&BitMask_13)
#define DENO_HST1w(val)			_wr32(_REG_BASE_+(0x0050<<2),(UINT)((_rd32(_REG_BASE_+(0x0050<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define EFT_OFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0056<<2)))&BitMask_05)
#define EFT_OFSw(val)			_wr32(_REG_BASE_+(0x0056<<2),(UINT)((_rd32(_REG_BASE_+(0x0056<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define HLOCKE_POSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0057<<2)))&BitMask_12)
#define HLOCKE_POSw(val)		_wr32(_REG_BASE_+(0x0057<<2),(UINT)((_rd32(_REG_BASE_+(0x0057<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VLOCKE_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0057<<2)))&BitMask_12)
#define VLOCKE_POSw(val)		_wr32(_REG_BASE_+(0x0057<<2),(UINT)((_rd32(_REG_BASE_+(0x0057<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HTWEr					(ValSft_R16(_rd32(_REG_BASE_+(0x0058<<2)))&BitMask_12)
#define HTWEw(val)				_wr32(_REG_BASE_+(0x0058<<2),(UINT)((_rd32(_REG_BASE_+(0x0058<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VTWEr					(ValSft_R00(_rd32(_REG_BASE_+(0x0058<<2)))&BitMask_12)
#define VTWEw(val)				_wr32(_REG_BASE_+(0x0058<<2),(UINT)((_rd32(_REG_BASE_+(0x0058<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HSPEr					(ValSft_R16(_rd32(_REG_BASE_+(0x0059<<2)))&BitMask_12)
#define HSPEw(val)				_wr32(_REG_BASE_+(0x0059<<2),(UINT)((_rd32(_REG_BASE_+(0x0059<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VSPEr					(ValSft_R00(_rd32(_REG_BASE_+(0x0059<<2)))&BitMask_12)
#define VSPEw(val)				_wr32(_REG_BASE_+(0x0059<<2),(UINT)((_rd32(_REG_BASE_+(0x0059<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HWE_AUTOr				(ValSft_R31(_rd32(_REG_BASE_+(0x005a<<2)))&BitMask_01)
#define HWE_AUTOw(val)			_wr32(_REG_BASE_+(0x005a<<2),(UINT)((_rd32(_REG_BASE_+(0x005a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HWEr					(ValSft_R16(_rd32(_REG_BASE_+(0x005a<<2)))&BitMask_12)
#define HWEw(val)				_wr32(_REG_BASE_+(0x005a<<2),(UINT)((_rd32(_REG_BASE_+(0x005a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWEr					(ValSft_R00(_rd32(_REG_BASE_+(0x005a<<2)))&BitMask_12)
#define VWEw(val)				_wr32(_REG_BASE_+(0x005a<<2),(UINT)((_rd32(_REG_BASE_+(0x005a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define FLDE_INVr				(ValSft_R31(_rd32(_REG_BASE_+(0x005b<<2)))&BitMask_01)
#define FLDE_INVw(val)			_wr32(_REG_BASE_+(0x005b<<2),(UINT)((_rd32(_REG_BASE_+(0x005b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define FLDE_INV2r				(ValSft_R30(_rd32(_REG_BASE_+(0x005b<<2)))&BitMask_01)
#define FLDE_INV2w(val)			_wr32(_REG_BASE_+(0x005b<<2),(UINT)((_rd32(_REG_BASE_+(0x005b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LHTWE_ONr				(ValSft_R29(_rd32(_REG_BASE_+(0x005b<<2)))&BitMask_01)
#define LHTWE_ONw(val)			_wr32(_REG_BASE_+(0x005b<<2),(UINT)((_rd32(_REG_BASE_+(0x005b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define OCSELr					(ValSft_R30(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_02)
#define OCSELw(val)				_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define YDLYr					(ValSft_R24(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_03)
#define YDLYw(val)				_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define HSYNC_OFS_ONr			(ValSft_R23(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_01)
#define HSYNC_OFS_ONw(val)		_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define HSYNC_OFS_VFLG_SELr		(ValSft_R22(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_01)
#define HSYNC_OFS_VFLG_SELw(val)	_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define HSYNC_OFS_INVr			(ValSft_R21(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_01)
#define HSYNC_OFS_INVw(val)		_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define HSYNC_OFSr				(ValSft_R16(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_03)
#define HSYNC_OFSw(val)			_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define ONSELr					(ValSft_R04(_rd32(_REG_BASE_+(0x005d<<2)))&BitMask_03)
#define ONSELw(val)				_wr32(_REG_BASE_+(0x005d<<2),(UINT)((_rd32(_REG_BASE_+(0x005d<<2))&(~ValSft_L04(BitMask_03)))|ValSft_L04((val)&BitMask_03)))

#define POST_MASK0r				(ValSft_R31(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_01)
#define POST_MASK0w(val)		_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PCBR_CINV0r				(ValSft_R28(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_01)
#define PCBR_CINV0w(val)		_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define YCBR2YC_SEL0r			(ValSft_R24(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_04)
#define YCBR2YC_SEL0w(val)		_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define POST_OSEL1r				(ValSft_R16(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_05)
#define POST_OSEL1w(val)		_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L16(BitMask_05)))|ValSft_L16((val)&BitMask_05)))

#define YC_OSELr				(ValSft_R10(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_05)
#define YC_OSELw(val)			_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L10(BitMask_05)))|ValSft_L10((val)&BitMask_05)))

#define POST_OSEL0r				(ValSft_R05(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_05)
#define POST_OSEL0w(val)		_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L05(BitMask_05)))|ValSft_L05((val)&BitMask_05)))

#define PRE_OSELr				(ValSft_R00(_rd32(_REG_BASE_+(0x005e<<2)))&BitMask_05)
#define PRE_OSELw(val)			_wr32(_REG_BASE_+(0x005e<<2),(UINT)((_rd32(_REG_BASE_+(0x005e<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define REG_UPMODEr				(ValSft_R31(_rd32(_REG_BASE_+(0x005f<<2)))&BitMask_01)
#define REG_UPMODEw(val)		_wr32(_REG_BASE_+(0x005f<<2),(UINT)((_rd32(_REG_BASE_+(0x005f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define SHSO_FPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_16)
#define SHSO_FPw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SHSO_RPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0060<<2)))&BitMask_16)
#define SHSO_RPw(val)			_wr32(_REG_BASE_+(0x0060<<2),(UINT)((_rd32(_REG_BASE_+(0x0060<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SVSO_FPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0061<<2)))&BitMask_16)
#define SVSO_FPw(val)			_wr32(_REG_BASE_+(0x0061<<2),(UINT)((_rd32(_REG_BASE_+(0x0061<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SVSO_RPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0061<<2)))&BitMask_16)
#define SVSO_RPw(val)			_wr32(_REG_BASE_+(0x0061<<2),(UINT)((_rd32(_REG_BASE_+(0x0061<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define ASYNC_DSYNCr			(ValSft_R31(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_01)
#define ASYNC_DSYNCw(val)		_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ASYNC_RSTr				(ValSft_R30(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_01)
#define ASYNC_RSTw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ASYNC_VPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_13)
#define ASYNC_VPOSw(val)		_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ASYNC_VSTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0062<<2)))&BitMask_13)
#define ASYNC_VSTw(val)			_wr32(_REG_BASE_+(0x0062<<2),(UINT)((_rd32(_REG_BASE_+(0x0062<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ASYNC_HWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0063<<2)))&BitMask_09)
#define ASYNC_HWw(val)			_wr32(_REG_BASE_+(0x0063<<2),(UINT)((_rd32(_REG_BASE_+(0x0063<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define UIRQ_FHPOS0r			(ValSft_R16(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_13)
#define UIRQ_FHPOS0w(val)		_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define UIRQ_FVPOS0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0064<<2)))&BitMask_12)
#define UIRQ_FVPOS0w(val)		_wr32(_REG_BASE_+(0x0064<<2),(UINT)((_rd32(_REG_BASE_+(0x0064<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define UIRQ_FHPOS1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_13)
#define UIRQ_FHPOS1w(val)		_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define UIRQ_FVPOS1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0065<<2)))&BitMask_12)
#define UIRQ_FVPOS1w(val)		_wr32(_REG_BASE_+(0x0065<<2),(UINT)((_rd32(_REG_BASE_+(0x0065<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define LVDS_RXONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LVDS_RXONw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LCK_SONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LCK_SONw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RDES_BITr				(ValSft_R28(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_02)
#define RDES_BITw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define RDES_PNSELr				(ValSft_R26(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define RDES_PNSELw(val)		_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define LVDS_LSBr				(ValSft_R25(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LVDS_LSBw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define LBUF_ONr				(ValSft_R24(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LBUF_ONw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LBUF_DONr				(ValSft_R23(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LBUF_DONw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define RDES_CH_MODr			(ValSft_R20(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_02)
#define RDES_CH_MODw(val)		_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define OMOD_BITr				(ValSft_R16(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_02)
#define OMOD_BITw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define SOF_NOr					(ValSft_R14(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define SOF_NOw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define FIX_SOV_ONr				(ValSft_R13(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define FIX_SOV_ONw(val)		_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define CH2_RFLG_INVr			(ValSft_R12(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define CH2_RFLG_INVw(val)		_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define LVLE_PRS_HZr			(ValSft_R06(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_01)
#define LVLE_PRS_HZw(val)		_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define SOL_CVALr				(ValSft_R00(_rd32(_REG_BASE_+(0x0067<<2)))&BitMask_06)
#define SOL_CVALw(val)			_wr32(_REG_BASE_+(0x0067<<2),(UINT)((_rd32(_REG_BASE_+(0x0067<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define LSYNCM_SELr				(ValSft_R30(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_02)
#define LSYNCM_SELw(val)		_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define LDIDLY3r				(ValSft_R12(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_03)
#define LDIDLY3w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define LDIDLY2r				(ValSft_R09(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_03)
#define LDIDLY2w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L09(BitMask_03)))|ValSft_L09((val)&BitMask_03)))

#define LDIDLY1r				(ValSft_R06(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_03)
#define LDIDLY1w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L06(BitMask_03)))|ValSft_L06((val)&BitMask_03)))

#define LDIDLY0r				(ValSft_R03(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_03)
#define LDIDLY0w(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L03(BitMask_03)))|ValSft_L03((val)&BitMask_03)))

#define LCK_DLYr				(ValSft_R00(_rd32(_REG_BASE_+(0x0068<<2)))&BitMask_03)
#define LCK_DLYw(val)			_wr32(_REG_BASE_+(0x0068<<2),(UINT)((_rd32(_REG_BASE_+(0x0068<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define LDO3_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_02)
#define LDO3_SELw(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define LDO2_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_02)
#define LDO2_SELw(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define LDO1_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_02)
#define LDO1_SELw(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define LDO0_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x0069<<2)))&BitMask_02)
#define LDO0_SELw(val)			_wr32(_REG_BASE_+(0x0069<<2),(UINT)((_rd32(_REG_BASE_+(0x0069<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define SYNC_COD0r				(ValSft_R16(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_12)
#define SYNC_COD0w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SYNC_COD1r				(ValSft_R00(_rd32(_REG_BASE_+(0x006a<<2)))&BitMask_12)
#define SYNC_COD1w(val)			_wr32(_REG_BASE_+(0x006a<<2),(UINT)((_rd32(_REG_BASE_+(0x006a<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SYNC_COD2r				(ValSft_R16(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_12)
#define SYNC_COD2w(val)			_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SYNC_COD3_0r			(ValSft_R00(_rd32(_REG_BASE_+(0x006b<<2)))&BitMask_12)
#define SYNC_COD3_0w(val)		_wr32(_REG_BASE_+(0x006b<<2),(UINT)((_rd32(_REG_BASE_+(0x006b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SYNC_COD3_1r			(ValSft_R16(_rd32(_REG_BASE_+(0x006c<<2)))&BitMask_12)
#define SYNC_COD3_1w(val)		_wr32(_REG_BASE_+(0x006c<<2),(UINT)((_rd32(_REG_BASE_+(0x006c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SYNC_COD3_2r			(ValSft_R00(_rd32(_REG_BASE_+(0x006c<<2)))&BitMask_16)
#define SYNC_COD3_2w(val)		_wr32(_REG_BASE_+(0x006c<<2),(UINT)((_rd32(_REG_BASE_+(0x006c<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SYNC_S0COD3_0r			(ValSft_R16(_rd32(_REG_BASE_+(0x006d<<2)))&BitMask_16)
#define SYNC_S0COD3_0w(val)		_wr32(_REG_BASE_+(0x006d<<2),(UINT)((_rd32(_REG_BASE_+(0x006d<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define SYNC_S0COD3_1r			(ValSft_R00(_rd32(_REG_BASE_+(0x006d<<2)))&BitMask_16)
#define SYNC_S0COD3_1w(val)		_wr32(_REG_BASE_+(0x006d<<2),(UINT)((_rd32(_REG_BASE_+(0x006d<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define SYNC_S0COD3_2r			(ValSft_R16(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_12)
#define SYNC_S0COD3_2w(val)		_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SYNC_S1COD3_0r			(ValSft_R00(_rd32(_REG_BASE_+(0x006e<<2)))&BitMask_12)
#define SYNC_S1COD3_0w(val)		_wr32(_REG_BASE_+(0x006e<<2),(UINT)((_rd32(_REG_BASE_+(0x006e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SYNC_S1COD3_1r			(ValSft_R16(_rd32(_REG_BASE_+(0x006f<<2)))&BitMask_12)
#define SYNC_S1COD3_1w(val)		_wr32(_REG_BASE_+(0x006f<<2),(UINT)((_rd32(_REG_BASE_+(0x006f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SYNC_S1COD3_2r			(ValSft_R00(_rd32(_REG_BASE_+(0x006f<<2)))&BitMask_12)
#define SYNC_S1COD3_2w(val)		_wr32(_REG_BASE_+(0x006f<<2),(UINT)((_rd32(_REG_BASE_+(0x006f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RDES_RPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0070<<2)))&BitMask_13)
#define RDES_RPOSw(val)			_wr32(_REG_BASE_+(0x0070<<2),(UINT)((_rd32(_REG_BASE_+(0x0070<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RDES_RVPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0071<<2)))&BitMask_12)
#define RDES_RVPOSw(val)		_wr32(_REG_BASE_+(0x0071<<2),(UINT)((_rd32(_REG_BASE_+(0x0071<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define MERGE_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0072<<2)))&BitMask_10)
#define MERGE_HWw(val)			_wr32(_REG_BASE_+(0x0072<<2),(UINT)((_rd32(_REG_BASE_+(0x0072<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define RDES_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0072<<2)))&BitMask_12)
#define RDES_VWw(val)			_wr32(_REG_BASE_+(0x0072<<2),(UINT)((_rd32(_REG_BASE_+(0x0072<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SYNC_CH_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0073<<2)))&BitMask_02)
#define SYNC_CH_SELw(val)		_wr32(_REG_BASE_+(0x0073<<2),(UINT)((_rd32(_REG_BASE_+(0x0073<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define SOV_ENr					(ValSft_R18(_rd32(_REG_BASE_+(0x0073<<2)))&BitMask_01)
#define SOV_ENw(val)			_wr32(_REG_BASE_+(0x0073<<2),(UINT)((_rd32(_REG_BASE_+(0x0073<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define SYNC_MASKr				(ValSft_R00(_rd32(_REG_BASE_+(0x0073<<2)))&BitMask_12)
#define SYNC_MASKw(val)			_wr32(_REG_BASE_+(0x0073<<2),(UINT)((_rd32(_REG_BASE_+(0x0073<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RDES_POLr				(ValSft_R08(_rd32(_REG_BASE_+(0x0077<<2)))&BitMask_03)
#define RDES_POLw(val)			_wr32(_REG_BASE_+(0x0077<<2),(UINT)((_rd32(_REG_BASE_+(0x0077<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define TSELr					(ValSft_R28(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_03)
#define TSELw(val)				_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define TPAT_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_04)
#define TPAT_SELw(val)			_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define LVDS_LE_TONr			(ValSft_R14(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_01)
#define LVDS_LE_TONw(val)		_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define TLVLr					(ValSft_R00(_rd32(_REG_BASE_+(0x0079<<2)))&BitMask_12)
#define TLVLw(val)				_wr32(_REG_BASE_+(0x0079<<2),(UINT)((_rd32(_REG_BASE_+(0x0079<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RDES0_CH_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_02)
#define RDES0_CH_SELw(val)		_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define RDES1_CH_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_02)
#define RDES1_CH_SELw(val)		_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define RDES2_CH_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_02)
#define RDES2_CH_SELw(val)		_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define RDES3_CH_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x007a<<2)))&BitMask_02)
#define RDES3_CH_SELw(val)		_wr32(_REG_BASE_+(0x007a<<2),(UINT)((_rd32(_REG_BASE_+(0x007a<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define MIPI_RXONr				(ValSft_R31(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_RXONw(val)			_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define MIPI_ECC_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_ECC_ONw(val)		_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define MIPI_WCCHK_LONr			(ValSft_R29(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_WCCHK_LONw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define MIPI_WCCHK_FONr			(ValSft_R28(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_WCCHK_FONw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define MIPI_WCCHK_FEONr		(ValSft_R27(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_WCCHK_FEONw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define MIPI_LAT_AENr			(ValSft_R26(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_LAT_AENw(val)		_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define MIPI_IMG_MCNTr			(ValSft_R24(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_02)
#define MIPI_IMG_MCNTw(val)		_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define MIPI_SOL_SELr			(ValSft_R23(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_SOL_SELw(val)		_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define SYNC_SOL_LS_SELr		(ValSft_R22(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define SYNC_SOL_LS_SELw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define SYNC_EOL_LS_SELr		(ValSft_R21(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define SYNC_EOL_LS_SELw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define MIPI_LEX_POS_ONr		(ValSft_R18(_rd32(_REG_BASE_+(0x007b<<2)))&BitMask_01)
#define MIPI_LEX_POS_ONw(val)	_wr32(_REG_BASE_+(0x007b<<2),(UINT)((_rd32(_REG_BASE_+(0x007b<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define MIPI_VLOCK_POSr			(ValSft_R16(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_12)
#define MIPI_VLOCK_POSw(val)	_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define MIPI_HLOCK_POSr			(ValSft_R00(_rd32(_REG_BASE_+(0x007c<<2)))&BitMask_14)
#define MIPI_HLOCK_POSw(val)	_wr32(_REG_BASE_+(0x007c<<2),(UINT)((_rd32(_REG_BASE_+(0x007c<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define SHD_MODEr				(ValSft_R30(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_02)
#define SHD_MODEw(val)			_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define SHD_DETOSDr				(ValSft_R27(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_01)
#define SHD_DETOSDw(val)		_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define SHD_ECHKr				(ValSft_R26(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_01)
#define SHD_ECHKw(val)			_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define SHD_KHr					(ValSft_R16(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_10)
#define SHD_KHw(val)			_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define SHD_KVr					(ValSft_R00(_rd32(_REG_BASE_+(0x0080<<2)))&BitMask_10)
#define SHD_KVw(val)			_wr32(_REG_BASE_+(0x0080<<2),(UINT)((_rd32(_REG_BASE_+(0x0080<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define SHD_HLF_MODr			(ValSft_R31(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_01)
#define SHD_HLF_MODw(val)		_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define SHD_RB_INVr				(ValSft_R30(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_01)
#define SHD_RB_INVw(val)		_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define SHD_YG_INVr				(ValSft_R29(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_01)
#define SHD_YG_INVw(val)		_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define SHD_DHOFFr				(ValSft_R16(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_13)
#define SHD_DHOFFw(val)			_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SHD_DVOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0081<<2)))&BitMask_13)
#define SHD_DVOFFw(val)			_wr32(_REG_BASE_+(0x0081<<2),(UINT)((_rd32(_REG_BASE_+(0x0081<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define SHD_CHOFFr				(ValSft_R16(_rd32(_REG_BASE_+(0x0082<<2)))&BitMask_12)
#define SHD_CHOFFw(val)			_wr32(_REG_BASE_+(0x0082<<2),(UINT)((_rd32(_REG_BASE_+(0x0082<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define SHD_CVOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0082<<2)))&BitMask_12)
#define SHD_CVOFFw(val)			_wr32(_REG_BASE_+(0x0082<<2),(UINT)((_rd32(_REG_BASE_+(0x0082<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define SHD_TESTr				(ValSft_R28(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_04)
#define SHD_TESTw(val)			_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define SHD_RGAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_08)
#define SHD_RGAINw(val)			_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define SHD_GGAINr				(ValSft_R08(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_08)
#define SHD_GGAINw(val)			_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define SHD_BGAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0083<<2)))&BitMask_08)
#define SHD_BGAINw(val)			_wr32(_REG_BASE_+(0x0083<<2),(UINT)((_rd32(_REG_BASE_+(0x0083<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define RD_RGB_SELr				(ValSft_R30(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_02)
#define RD_RGB_SELw(val)		_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define SHD_DHWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_13)
#define SHD_DHWw(val)			_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SHD_DVWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0084<<2)))&BitMask_13)
#define SHD_DVWw(val)			_wr32(_REG_BASE_+(0x0084<<2),(UINT)((_rd32(_REG_BASE_+(0x0084<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define PRE_OFS_YGINVr			(ValSft_R29(_rd32(_REG_BASE_+(0x0086<<2)))&BitMask_01)
#define PRE_OFS_YGINVw(val)		_wr32(_REG_BASE_+(0x0086<<2),(UINT)((_rd32(_REG_BASE_+(0x0086<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PRE_OFS_RBINVr			(ValSft_R28(_rd32(_REG_BASE_+(0x0086<<2)))&BitMask_01)
#define PRE_OFS_RBINVw(val)		_wr32(_REG_BASE_+(0x0086<<2),(UINT)((_rd32(_REG_BASE_+(0x0086<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define PRE_GROFSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0088<<2)))&BitMask_10)
#define PRE_GROFSw(val)			_wr32(_REG_BASE_+(0x0088<<2),(UINT)((_rd32(_REG_BASE_+(0x0088<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define PRE_GBOFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0088<<2)))&BitMask_10)
#define PRE_GBOFSw(val)			_wr32(_REG_BASE_+(0x0088<<2),(UINT)((_rd32(_REG_BASE_+(0x0088<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PRE_ROFSr				(ValSft_R16(_rd32(_REG_BASE_+(0x0089<<2)))&BitMask_10)
#define PRE_ROFSw(val)			_wr32(_REG_BASE_+(0x0089<<2),(UINT)((_rd32(_REG_BASE_+(0x0089<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define PRE_BOFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0089<<2)))&BitMask_10)
#define PRE_BOFSw(val)			_wr32(_REG_BASE_+(0x0089<<2),(UINT)((_rd32(_REG_BASE_+(0x0089<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PGAIN_RBINVr			(ValSft_R31(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_01)
#define PGAIN_RBINVw(val)		_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PGAIN_YGINVr			(ValSft_R30(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_01)
#define PGAIN_YGINVw(val)		_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PRE_GAIN_OFFr			(ValSft_R29(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_01)
#define PRE_GAIN_OFFw(val)		_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PGAIN_GRr				(ValSft_R16(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_12)
#define PGAIN_GRw(val)			_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PGAIN_GBr				(ValSft_R00(_rd32(_REG_BASE_+(0x008e<<2)))&BitMask_12)
#define PGAIN_GBw(val)			_wr32(_REG_BASE_+(0x008e<<2),(UINT)((_rd32(_REG_BASE_+(0x008e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PGAIN_Rr				(ValSft_R16(_rd32(_REG_BASE_+(0x008f<<2)))&BitMask_12)
#define PGAIN_Rw(val)			_wr32(_REG_BASE_+(0x008f<<2),(UINT)((_rd32(_REG_BASE_+(0x008f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PGAIN_Br				(ValSft_R00(_rd32(_REG_BASE_+(0x008f<<2)))&BitMask_12)
#define PGAIN_Bw(val)			_wr32(_REG_BASE_+(0x008f<<2),(UINT)((_rd32(_REG_BASE_+(0x008f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DNR3D_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_01)
#define DNR3D_ONw(val)			_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DNR3D_RCH_ENr			(ValSft_R30(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_01)
#define DNR3D_RCH_ENw(val)		_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DNR3D_FKr				(ValSft_R16(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_08)
#define DNR3D_FKw(val)			_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DNR3D_THr				(ValSft_R08(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_08)
#define DNR3D_THw(val)			_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DNR3D_GAr				(ValSft_R00(_rd32(_REG_BASE_+(0x0095<<2)))&BitMask_06)
#define DNR3D_GAw(val)			_wr32(_REG_BASE_+(0x0095<<2),(UINT)((_rd32(_REG_BASE_+(0x0095<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define DNR2D_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_01)
#define DNR2D_ONw(val)			_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DNR2D_MODr				(ValSft_R30(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_01)
#define DNR2D_MODw(val)			_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DNR2D_CNTr				(ValSft_R16(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_04)
#define DNR2D_CNTw(val)			_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define DNR2D_DTHr				(ValSft_R08(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_08)
#define DNR2D_DTHw(val)			_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DNR2D_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0097<<2)))&BitMask_08)
#define DNR2D_GAINw(val)		_wr32(_REG_BASE_+(0x0097<<2),(UINT)((_rd32(_REG_BASE_+(0x0097<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DF_COR_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_01)
#define DF_COR_ONw(val)			_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DFS_ICSELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_02)
#define DFS_ICSELw(val)			_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L29(BitMask_02)))|ValSft_L29((val)&BitMask_02)))

#define DF_GSUM_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_01)
#define DF_GSUM_SELw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define DF_RBSUM_SELr			(ValSft_R27(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_01)
#define DF_RBSUM_SELw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define DF_GWGT_SELr			(ValSft_R25(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_02)
#define DF_GWGT_SELw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L25(BitMask_02)))|ValSft_L25((val)&BitMask_02)))

#define DF_RBWGT_SELr			(ValSft_R23(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_02)
#define DF_RBWGT_SELw(val)		_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L23(BitMask_02)))|ValSft_L23((val)&BitMask_02)))

#define DF_SLOPEr				(ValSft_R16(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_07)
#define DF_SLOPEw(val)			_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define DF_OFSr					(ValSft_R00(_rd32(_REG_BASE_+(0x0098<<2)))&BitMask_14)
#define DF_OFSw(val)			_wr32(_REG_BASE_+(0x0098<<2),(UINT)((_rd32(_REG_BASE_+(0x0098<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define DF_MRGr					(ValSft_R30(_rd32(_REG_BASE_+(0x0099<<2)))&BitMask_02)
#define DF_MRGw(val)			_wr32(_REG_BASE_+(0x0099<<2),(UINT)((_rd32(_REG_BASE_+(0x0099<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DF_MINr					(ValSft_R16(_rd32(_REG_BASE_+(0x0099<<2)))&BitMask_13)
#define DF_MINw(val)			_wr32(_REG_BASE_+(0x0099<<2),(UINT)((_rd32(_REG_BASE_+(0x0099<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_MAXr					(ValSft_R00(_rd32(_REG_BASE_+(0x0099<<2)))&BitMask_13)
#define DF_MAXw(val)			_wr32(_REG_BASE_+(0x0099<<2),(UINT)((_rd32(_REG_BASE_+(0x0099<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DF_SUM_GTHr				(ValSft_R16(_rd32(_REG_BASE_+(0x009a<<2)))&BitMask_13)
#define DF_SUM_GTHw(val)		_wr32(_REG_BASE_+(0x009a<<2),(UINT)((_rd32(_REG_BASE_+(0x009a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_SUM_RBTHr			(ValSft_R00(_rd32(_REG_BASE_+(0x009a<<2)))&BitMask_13)
#define DF_SUM_RBTHw(val)		_wr32(_REG_BASE_+(0x009a<<2),(UINT)((_rd32(_REG_BASE_+(0x009a<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DF_TEST_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_01)
#define DF_TEST_ONw(val)		_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DF_TEST_PATr			(ValSft_R30(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_01)
#define DF_TEST_PATw(val)		_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DF_TESTX0r				(ValSft_R16(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_13)
#define DF_TESTX0w(val)			_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_TESTY0r				(ValSft_R00(_rd32(_REG_BASE_+(0x009b<<2)))&BitMask_12)
#define DF_TESTY0w(val)			_wr32(_REG_BASE_+(0x009b<<2),(UINT)((_rd32(_REG_BASE_+(0x009b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DF_TESTX1r				(ValSft_R16(_rd32(_REG_BASE_+(0x009c<<2)))&BitMask_13)
#define DF_TESTX1w(val)			_wr32(_REG_BASE_+(0x009c<<2),(UINT)((_rd32(_REG_BASE_+(0x009c<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_TESTY1r				(ValSft_R00(_rd32(_REG_BASE_+(0x009c<<2)))&BitMask_12)
#define DF_TESTY1w(val)			_wr32(_REG_BASE_+(0x009c<<2),(UINT)((_rd32(_REG_BASE_+(0x009c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DF_TESTX2r				(ValSft_R16(_rd32(_REG_BASE_+(0x009d<<2)))&BitMask_13)
#define DF_TESTX2w(val)			_wr32(_REG_BASE_+(0x009d<<2),(UINT)((_rd32(_REG_BASE_+(0x009d<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_TESTY2r				(ValSft_R00(_rd32(_REG_BASE_+(0x009d<<2)))&BitMask_12)
#define DF_TESTY2w(val)			_wr32(_REG_BASE_+(0x009d<<2),(UINT)((_rd32(_REG_BASE_+(0x009d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DF_TESTX3r				(ValSft_R16(_rd32(_REG_BASE_+(0x009e<<2)))&BitMask_13)
#define DF_TESTX3w(val)			_wr32(_REG_BASE_+(0x009e<<2),(UINT)((_rd32(_REG_BASE_+(0x009e<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DF_TESTY3r				(ValSft_R00(_rd32(_REG_BASE_+(0x009e<<2)))&BitMask_12)
#define DF_TESTY3w(val)			_wr32(_REG_BASE_+(0x009e<<2),(UINT)((_rd32(_REG_BASE_+(0x009e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_ONw(val)			_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define WDR_SCHM_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_SCHM_ONw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define WDR_SGAMMA_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_SGAMMA_ONw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define WDR_SWGT_LMT_ONr		(ValSft_R28(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_SWGT_LMT_ONw(val)	_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define LS_FLG_INVr				(ValSft_R27(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define LS_FLG_INVw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define WDR_LS_SWCH_ONr			(ValSft_R26(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_LS_SWCH_ONw(val)	_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define WDR_G_SWCH_ONr			(ValSft_R25(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_G_SWCH_ONw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define WDR_MOTION_ONr			(ValSft_R24(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define WDR_MOTION_ONw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LS_FLG_POST_Hr			(ValSft_R23(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_01)
#define LS_FLG_POST_Hw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define WDR_LD_MSTEPr			(ValSft_R16(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_07)
#define WDR_LD_MSTEPw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define WDR_LOW_THr				(ValSft_R08(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_06)
#define WDR_LOW_THw(val)		_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define WDR_TSELr				(ValSft_R00(_rd32(_REG_BASE_+(0x00a0<<2)))&BitMask_05)
#define WDR_TSELw(val)			_wr32(_REG_BASE_+(0x00a0<<2),(UINT)((_rd32(_REG_BASE_+(0x00a0<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define WDR_SATVr				(ValSft_R16(_rd32(_REG_BASE_+(0x00a1<<2)))&BitMask_10)
#define WDR_SATVw(val)			_wr32(_REG_BASE_+(0x00a1<<2),(UINT)((_rd32(_REG_BASE_+(0x00a1<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define WDR_SWGTr				(ValSft_R00(_rd32(_REG_BASE_+(0x00a1<<2)))&BitMask_10)
#define WDR_SWGTw(val)			_wr32(_REG_BASE_+(0x00a1<<2),(UINT)((_rd32(_REG_BASE_+(0x00a1<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_LSHUTr				(ValSft_R16(_rd32(_REG_BASE_+(0x00a2<<2)))&BitMask_12)
#define WDR_LSHUTw(val)			_wr32(_REG_BASE_+(0x00a2<<2),(UINT)((_rd32(_REG_BASE_+(0x00a2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define WDR_SSHUTr				(ValSft_R00(_rd32(_REG_BASE_+(0x00a2<<2)))&BitMask_12)
#define WDR_SSHUTw(val)			_wr32(_REG_BASE_+(0x00a2<<2),(UINT)((_rd32(_REG_BASE_+(0x00a2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WDR_SGM_RANGEr			(ValSft_R16(_rd32(_REG_BASE_+(0x00a3<<2)))&BitMask_08)
#define WDR_SGM_RANGEw(val)		_wr32(_REG_BASE_+(0x00a3<<2),(UINT)((_rd32(_REG_BASE_+(0x00a3<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define WDR_SCH_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a3<<2)))&BitMask_10)
#define WDR_SCH_MAXw(val)		_wr32(_REG_BASE_+(0x00a3<<2),(UINT)((_rd32(_REG_BASE_+(0x00a3<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_LS_MSTEPr			(ValSft_R16(_rd32(_REG_BASE_+(0x00a4<<2)))&BitMask_09)
#define WDR_LS_MSTEPw(val)		_wr32(_REG_BASE_+(0x00a4<<2),(UINT)((_rd32(_REG_BASE_+(0x00a4<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define WDR_BIT_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a4<<2)))&BitMask_10)
#define WDR_BIT_MAXw(val)		_wr32(_REG_BASE_+(0x00a4<<2),(UINT)((_rd32(_REG_BASE_+(0x00a4<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define WDR_HIGH_CNT_THr		(ValSft_R00(_rd32(_REG_BASE_+(0x00a5<<2)))&BitMask_21)
#define WDR_HIGH_CNT_THw(val)	_wr32(_REG_BASE_+(0x00a5<<2),(UINT)((_rd32(_REG_BASE_+(0x00a5<<2))&(~ValSft_L00(BitMask_21)))|ValSft_L00((val)&BitMask_21)))

#define WDR_SL_R_IIRr			(ValSft_R24(_rd32(_REG_BASE_+(0x00a6<<2)))&BitMask_08)
#define WDR_SL_R_IIRw(val)		_wr32(_REG_BASE_+(0x00a6<<2),(UINT)((_rd32(_REG_BASE_+(0x00a6<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define WDR_LOW_CNT_THr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a6<<2)))&BitMask_21)
#define WDR_LOW_CNT_THw(val)	_wr32(_REG_BASE_+(0x00a6<<2),(UINT)((_rd32(_REG_BASE_+(0x00a6<<2))&(~ValSft_L00(BitMask_21)))|ValSft_L00((val)&BitMask_21)))

#define WDR_SR_GAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x00a7<<2)))&BitMask_11)
#define WDR_SR_GAINw(val)		_wr32(_REG_BASE_+(0x00a7<<2),(UINT)((_rd32(_REG_BASE_+(0x00a7<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define WDR_SB_GAINr			(ValSft_R00(_rd32(_REG_BASE_+(0x00a7<<2)))&BitMask_11)
#define WDR_SB_GAINw(val)		_wr32(_REG_BASE_+(0x00a7<<2),(UINT)((_rd32(_REG_BASE_+(0x00a7<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define OZONE_SELr				(ValSft_R28(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_03)
#define OZONE_SELw(val)			_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define AF_YSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_03)
#define AF_YSELw(val)			_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define AF_YLPFr				(ValSft_R23(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_01)
#define AF_YLPFw(val)			_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define AE_LOCK_SELr			(ValSft_R22(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_01)
#define AE_LOCK_SELw(val)		_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define AFODMFF_RSr				(ValSft_R15(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_01)
#define AFODMFF_RSw(val)		_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define ODMFF_HONr				(ValSft_R14(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_01)
#define ODMFF_HONw(val)			_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define AE_WIN_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b0<<2)))&BitMask_06)
#define AE_WIN_SELw(val)		_wr32(_REG_BASE_+(0x00b0<<2),(UINT)((_rd32(_REG_BASE_+(0x00b0<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AE1_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b1<<2)))&BitMask_12)
#define AE1_HSPw(val)			_wr32(_REG_BASE_+(0x00b1<<2),(UINT)((_rd32(_REG_BASE_+(0x00b1<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE1_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b1<<2)))&BitMask_12)
#define AE1_HEPw(val)			_wr32(_REG_BASE_+(0x00b1<<2),(UINT)((_rd32(_REG_BASE_+(0x00b1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE1_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b2<<2)))&BitMask_12)
#define AE1_VSPw(val)			_wr32(_REG_BASE_+(0x00b2<<2),(UINT)((_rd32(_REG_BASE_+(0x00b2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE1_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b2<<2)))&BitMask_12)
#define AE1_VEPw(val)			_wr32(_REG_BASE_+(0x00b2<<2),(UINT)((_rd32(_REG_BASE_+(0x00b2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE2_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b3<<2)))&BitMask_12)
#define AE2_HSPw(val)			_wr32(_REG_BASE_+(0x00b3<<2),(UINT)((_rd32(_REG_BASE_+(0x00b3<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE2_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b3<<2)))&BitMask_12)
#define AE2_HEPw(val)			_wr32(_REG_BASE_+(0x00b3<<2),(UINT)((_rd32(_REG_BASE_+(0x00b3<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE2_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b4<<2)))&BitMask_12)
#define AE2_VSPw(val)			_wr32(_REG_BASE_+(0x00b4<<2),(UINT)((_rd32(_REG_BASE_+(0x00b4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE2_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b4<<2)))&BitMask_12)
#define AE2_VEPw(val)			_wr32(_REG_BASE_+(0x00b4<<2),(UINT)((_rd32(_REG_BASE_+(0x00b4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE3_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b5<<2)))&BitMask_12)
#define AE3_HSPw(val)			_wr32(_REG_BASE_+(0x00b5<<2),(UINT)((_rd32(_REG_BASE_+(0x00b5<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE3_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b5<<2)))&BitMask_12)
#define AE3_HEPw(val)			_wr32(_REG_BASE_+(0x00b5<<2),(UINT)((_rd32(_REG_BASE_+(0x00b5<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE3_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b6<<2)))&BitMask_12)
#define AE3_VSPw(val)			_wr32(_REG_BASE_+(0x00b6<<2),(UINT)((_rd32(_REG_BASE_+(0x00b6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE3_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b6<<2)))&BitMask_12)
#define AE3_VEPw(val)			_wr32(_REG_BASE_+(0x00b6<<2),(UINT)((_rd32(_REG_BASE_+(0x00b6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE4_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b7<<2)))&BitMask_12)
#define AE4_HSPw(val)			_wr32(_REG_BASE_+(0x00b7<<2),(UINT)((_rd32(_REG_BASE_+(0x00b7<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE4_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b7<<2)))&BitMask_12)
#define AE4_HEPw(val)			_wr32(_REG_BASE_+(0x00b7<<2),(UINT)((_rd32(_REG_BASE_+(0x00b7<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE4_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b8<<2)))&BitMask_12)
#define AE4_VSPw(val)			_wr32(_REG_BASE_+(0x00b8<<2),(UINT)((_rd32(_REG_BASE_+(0x00b8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE4_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b8<<2)))&BitMask_12)
#define AE4_VEPw(val)			_wr32(_REG_BASE_+(0x00b8<<2),(UINT)((_rd32(_REG_BASE_+(0x00b8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE5_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00b9<<2)))&BitMask_12)
#define AE5_HSPw(val)			_wr32(_REG_BASE_+(0x00b9<<2),(UINT)((_rd32(_REG_BASE_+(0x00b9<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE5_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00b9<<2)))&BitMask_12)
#define AE5_HEPw(val)			_wr32(_REG_BASE_+(0x00b9<<2),(UINT)((_rd32(_REG_BASE_+(0x00b9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE5_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ba<<2)))&BitMask_12)
#define AE5_VSPw(val)			_wr32(_REG_BASE_+(0x00ba<<2),(UINT)((_rd32(_REG_BASE_+(0x00ba<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE5_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ba<<2)))&BitMask_12)
#define AE5_VEPw(val)			_wr32(_REG_BASE_+(0x00ba<<2),(UINT)((_rd32(_REG_BASE_+(0x00ba<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE6_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00bb<<2)))&BitMask_12)
#define AE6_HSPw(val)			_wr32(_REG_BASE_+(0x00bb<<2),(UINT)((_rd32(_REG_BASE_+(0x00bb<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE6_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00bb<<2)))&BitMask_12)
#define AE6_HEPw(val)			_wr32(_REG_BASE_+(0x00bb<<2),(UINT)((_rd32(_REG_BASE_+(0x00bb<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE6_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00bc<<2)))&BitMask_12)
#define AE6_VSPw(val)			_wr32(_REG_BASE_+(0x00bc<<2),(UINT)((_rd32(_REG_BASE_+(0x00bc<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AE6_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00bc<<2)))&BitMask_12)
#define AE6_VEPw(val)			_wr32(_REG_BASE_+(0x00bc<<2),(UINT)((_rd32(_REG_BASE_+(0x00bc<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AE1_SLICEr				(ValSft_R24(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_08)
#define AE1_SLICEw(val)			_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define AE1_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_08)
#define AE1_CLIPw(val)			_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AE2_SLICEr				(ValSft_R08(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_08)
#define AE2_SLICEw(val)			_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define AE2_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00bd<<2)))&BitMask_08)
#define AE2_CLIPw(val)			_wr32(_REG_BASE_+(0x00bd<<2),(UINT)((_rd32(_REG_BASE_+(0x00bd<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AE3_SLICEr				(ValSft_R24(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_08)
#define AE3_SLICEw(val)			_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define AE3_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_08)
#define AE3_CLIPw(val)			_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AE4_SLICEr				(ValSft_R08(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_08)
#define AE4_SLICEw(val)			_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define AE4_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00be<<2)))&BitMask_08)
#define AE4_CLIPw(val)			_wr32(_REG_BASE_+(0x00be<<2),(UINT)((_rd32(_REG_BASE_+(0x00be<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AE5_SLICEr				(ValSft_R24(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_08)
#define AE5_SLICEw(val)			_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define AE5_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_08)
#define AE5_CLIPw(val)			_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AE6_SLICEr				(ValSft_R08(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_08)
#define AE6_SLICEw(val)			_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define AE6_CLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00bf<<2)))&BitMask_08)
#define AE6_CLIPw(val)			_wr32(_REG_BASE_+(0x00bf<<2),(UINT)((_rd32(_REG_BASE_+(0x00bf<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define HISTO0_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_07)
#define HISTO0_MAXw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO1_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_07)
#define HISTO1_MAXw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO2_MAXr				(ValSft_R08(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_07)
#define HISTO2_MAXw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO3_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x00c0<<2)))&BitMask_07)
#define HISTO3_MAXw(val)		_wr32(_REG_BASE_+(0x00c0<<2),(UINT)((_rd32(_REG_BASE_+(0x00c0<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO4_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_07)
#define HISTO4_MAXw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO5_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_07)
#define HISTO5_MAXw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO6_MAXr				(ValSft_R08(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_07)
#define HISTO6_MAXw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO7_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x00c1<<2)))&BitMask_07)
#define HISTO7_MAXw(val)		_wr32(_REG_BASE_+(0x00c1<<2),(UINT)((_rd32(_REG_BASE_+(0x00c1<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO8_MAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_07)
#define HISTO8_MAXw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO9_MAXr				(ValSft_R16(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_07)
#define HISTO9_MAXw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO10_MAXr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_07)
#define HISTO10_MAXw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO11_MAXr			(ValSft_R00(_rd32(_REG_BASE_+(0x00c2<<2)))&BitMask_07)
#define HISTO11_MAXw(val)		_wr32(_REG_BASE_+(0x00c2<<2),(UINT)((_rd32(_REG_BASE_+(0x00c2<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO12_MAXr			(ValSft_R24(_rd32(_REG_BASE_+(0x00c3<<2)))&BitMask_07)
#define HISTO12_MAXw(val)		_wr32(_REG_BASE_+(0x00c3<<2),(UINT)((_rd32(_REG_BASE_+(0x00c3<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO13_MAXr			(ValSft_R16(_rd32(_REG_BASE_+(0x00c3<<2)))&BitMask_07)
#define HISTO13_MAXw(val)		_wr32(_REG_BASE_+(0x00c3<<2),(UINT)((_rd32(_REG_BASE_+(0x00c3<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO14_MAXr			(ValSft_R08(_rd32(_REG_BASE_+(0x00c3<<2)))&BitMask_07)
#define HISTO14_MAXw(val)		_wr32(_REG_BASE_+(0x00c3<<2),(UINT)((_rd32(_REG_BASE_+(0x00c3<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO0_MAX2r			(ValSft_R24(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_07)
#define HISTO0_MAX2w(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO1_MAX2r			(ValSft_R16(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_07)
#define HISTO1_MAX2w(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO2_MAX2r			(ValSft_R08(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_07)
#define HISTO2_MAX2w(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO3_MAX2r			(ValSft_R00(_rd32(_REG_BASE_+(0x00c5<<2)))&BitMask_07)
#define HISTO3_MAX2w(val)		_wr32(_REG_BASE_+(0x00c5<<2),(UINT)((_rd32(_REG_BASE_+(0x00c5<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO4_MAX2r			(ValSft_R24(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_07)
#define HISTO4_MAX2w(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO5_MAX2r			(ValSft_R16(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_07)
#define HISTO5_MAX2w(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO6_MAX2r			(ValSft_R08(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_07)
#define HISTO6_MAX2w(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO7_MAX2r			(ValSft_R00(_rd32(_REG_BASE_+(0x00c6<<2)))&BitMask_07)
#define HISTO7_MAX2w(val)		_wr32(_REG_BASE_+(0x00c6<<2),(UINT)((_rd32(_REG_BASE_+(0x00c6<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO8_MAX2r			(ValSft_R24(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_07)
#define HISTO8_MAX2w(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO9_MAX2r			(ValSft_R16(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_07)
#define HISTO9_MAX2w(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO10_MAX2r			(ValSft_R08(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_07)
#define HISTO10_MAX2w(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define HISTO11_MAX2r			(ValSft_R00(_rd32(_REG_BASE_+(0x00c7<<2)))&BitMask_07)
#define HISTO11_MAX2w(val)		_wr32(_REG_BASE_+(0x00c7<<2),(UINT)((_rd32(_REG_BASE_+(0x00c7<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define HISTO12_MAX2r			(ValSft_R24(_rd32(_REG_BASE_+(0x00c8<<2)))&BitMask_07)
#define HISTO12_MAX2w(val)		_wr32(_REG_BASE_+(0x00c8<<2),(UINT)((_rd32(_REG_BASE_+(0x00c8<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define HISTO13_MAX2r			(ValSft_R16(_rd32(_REG_BASE_+(0x00c8<<2)))&BitMask_07)
#define HISTO13_MAX2w(val)		_wr32(_REG_BASE_+(0x00c8<<2),(UINT)((_rd32(_REG_BASE_+(0x00c8<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define HISTO14_MAX2r			(ValSft_R08(_rd32(_REG_BASE_+(0x00c8<<2)))&BitMask_07)
#define HISTO14_MAX2w(val)		_wr32(_REG_BASE_+(0x00c8<<2),(UINT)((_rd32(_REG_BASE_+(0x00c8<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define POGAIN_GRr				(ValSft_R16(_rd32(_REG_BASE_+(0x00ce<<2)))&BitMask_14)
#define POGAIN_GRw(val)			_wr32(_REG_BASE_+(0x00ce<<2),(UINT)((_rd32(_REG_BASE_+(0x00ce<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define POGAIN_GBr				(ValSft_R00(_rd32(_REG_BASE_+(0x00ce<<2)))&BitMask_14)
#define POGAIN_GBw(val)			_wr32(_REG_BASE_+(0x00ce<<2),(UINT)((_rd32(_REG_BASE_+(0x00ce<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define POGAIN_Rr				(ValSft_R16(_rd32(_REG_BASE_+(0x00cf<<2)))&BitMask_14)
#define POGAIN_Rw(val)			_wr32(_REG_BASE_+(0x00cf<<2),(UINT)((_rd32(_REG_BASE_+(0x00cf<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define POGAIN_Br				(ValSft_R00(_rd32(_REG_BASE_+(0x00cf<<2)))&BitMask_14)
#define POGAIN_Bw(val)			_wr32(_REG_BASE_+(0x00cf<<2),(UINT)((_rd32(_REG_BASE_+(0x00cf<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define AF_VLPF_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_01)
#define AF_VLPF_SELw(val)		_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AF_CHSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_02)
#define AF_CHSELw(val)			_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define AF1_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_12)
#define AF1_HSPw(val)			_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF1_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d0<<2)))&BitMask_12)
#define AF1_HEPw(val)			_wr32(_REG_BASE_+(0x00d0<<2),(UINT)((_rd32(_REG_BASE_+(0x00d0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF1_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d1<<2)))&BitMask_12)
#define AF1_VSPw(val)			_wr32(_REG_BASE_+(0x00d1<<2),(UINT)((_rd32(_REG_BASE_+(0x00d1<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF1_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d1<<2)))&BitMask_12)
#define AF1_VEPw(val)			_wr32(_REG_BASE_+(0x00d1<<2),(UINT)((_rd32(_REG_BASE_+(0x00d1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF2_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d2<<2)))&BitMask_12)
#define AF2_HSPw(val)			_wr32(_REG_BASE_+(0x00d2<<2),(UINT)((_rd32(_REG_BASE_+(0x00d2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF2_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d2<<2)))&BitMask_12)
#define AF2_HEPw(val)			_wr32(_REG_BASE_+(0x00d2<<2),(UINT)((_rd32(_REG_BASE_+(0x00d2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF2_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d3<<2)))&BitMask_12)
#define AF2_VSPw(val)			_wr32(_REG_BASE_+(0x00d3<<2),(UINT)((_rd32(_REG_BASE_+(0x00d3<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AF2_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d3<<2)))&BitMask_12)
#define AF2_VEPw(val)			_wr32(_REG_BASE_+(0x00d3<<2),(UINT)((_rd32(_REG_BASE_+(0x00d3<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AF_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d4<<2)))&BitMask_08)
#define AF_CLIPw(val)			_wr32(_REG_BASE_+(0x00d4<<2),(UINT)((_rd32(_REG_BASE_+(0x00d4<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AF_SLICEr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d4<<2)))&BitMask_08)
#define AF_SLICEw(val)			_wr32(_REG_BASE_+(0x00d4<<2),(UINT)((_rd32(_REG_BASE_+(0x00d4<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AF_HONr					(ValSft_R31(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define AF_HONw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AF_CLIP_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define AF_CLIP_ONw(val)		_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define AE_HONr					(ValSft_R29(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define AE_HONw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define AF2_HTABr				(ValSft_R12(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_03)
#define AF2_HTABw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define AF1_HTABr				(ValSft_R08(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_03)
#define AF1_HTABw(val)			_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define AF_WIN_SELr				(ValSft_R04(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_02)
#define AF_WIN_SELw(val)		_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define AF_LOCK_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x00d5<<2)))&BitMask_01)
#define AF_LOCK_SELw(val)		_wr32(_REG_BASE_+(0x00d5<<2),(UINT)((_rd32(_REG_BASE_+(0x00d5<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define PRR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d8<<2)))&BitMask_11)
#define PRR_GAINw(val)			_wr32(_REG_BASE_+(0x00d8<<2),(UINT)((_rd32(_REG_BASE_+(0x00d8<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PRG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d8<<2)))&BitMask_12)
#define PRG_GAINw(val)			_wr32(_REG_BASE_+(0x00d8<<2),(UINT)((_rd32(_REG_BASE_+(0x00d8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PRB_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00d9<<2)))&BitMask_12)
#define PRB_GAINw(val)			_wr32(_REG_BASE_+(0x00d9<<2),(UINT)((_rd32(_REG_BASE_+(0x00d9<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PGR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00d9<<2)))&BitMask_12)
#define PGR_GAINw(val)			_wr32(_REG_BASE_+(0x00d9<<2),(UINT)((_rd32(_REG_BASE_+(0x00d9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PGG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_11)
#define PGG_GAINw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PGB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00da<<2)))&BitMask_12)
#define PGB_GAINw(val)			_wr32(_REG_BASE_+(0x00da<<2),(UINT)((_rd32(_REG_BASE_+(0x00da<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PBR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00db<<2)))&BitMask_12)
#define PBR_GAINw(val)			_wr32(_REG_BASE_+(0x00db<<2),(UINT)((_rd32(_REG_BASE_+(0x00db<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PBG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00db<<2)))&BitMask_12)
#define PBG_GAINw(val)			_wr32(_REG_BASE_+(0x00db<<2),(UINT)((_rd32(_REG_BASE_+(0x00db<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PBB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00dc<<2)))&BitMask_11)
#define PBB_GAINw(val)			_wr32(_REG_BASE_+(0x00dc<<2),(UINT)((_rd32(_REG_BASE_+(0x00dc<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define PYR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x00dd<<2)))&BitMask_08)
#define PYR_GAINw(val)			_wr32(_REG_BASE_+(0x00dd<<2),(UINT)((_rd32(_REG_BASE_+(0x00dd<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PYG_GAINr				(ValSft_R08(_rd32(_REG_BASE_+(0x00dd<<2)))&BitMask_08)
#define PYG_GAINw(val)			_wr32(_REG_BASE_+(0x00dd<<2),(UINT)((_rd32(_REG_BASE_+(0x00dd<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PYB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x00dd<<2)))&BitMask_08)
#define PYB_GAINw(val)			_wr32(_REG_BASE_+(0x00dd<<2),(UINT)((_rd32(_REG_BASE_+(0x00dd<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PAWB_OFFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x00de<<2)))&BitMask_09)
#define PAWB_OFFSw(val)			_wr32(_REG_BASE_+(0x00de<<2),(UINT)((_rd32(_REG_BASE_+(0x00de<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define ODMFF_RSr				(ValSft_R28(_rd32(_REG_BASE_+(0x00df<<2)))&BitMask_01)
#define ODMFF_RSw(val)			_wr32(_REG_BASE_+(0x00df<<2),(UINT)((_rd32(_REG_BASE_+(0x00df<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define ODM_M1r					(ValSft_R16(_rd32(_REG_BASE_+(0x00e0<<2)))&BitMask_10)
#define ODM_M1w(val)			_wr32(_REG_BASE_+(0x00e0<<2),(UINT)((_rd32(_REG_BASE_+(0x00e0<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M2r					(ValSft_R00(_rd32(_REG_BASE_+(0x00e0<<2)))&BitMask_10)
#define ODM_M2w(val)			_wr32(_REG_BASE_+(0x00e0<<2),(UINT)((_rd32(_REG_BASE_+(0x00e0<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M3r					(ValSft_R16(_rd32(_REG_BASE_+(0x00e1<<2)))&BitMask_10)
#define ODM_M3w(val)			_wr32(_REG_BASE_+(0x00e1<<2),(UINT)((_rd32(_REG_BASE_+(0x00e1<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M4r					(ValSft_R00(_rd32(_REG_BASE_+(0x00e1<<2)))&BitMask_10)
#define ODM_M4w(val)			_wr32(_REG_BASE_+(0x00e1<<2),(UINT)((_rd32(_REG_BASE_+(0x00e1<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M5r					(ValSft_R16(_rd32(_REG_BASE_+(0x00e2<<2)))&BitMask_10)
#define ODM_M5w(val)			_wr32(_REG_BASE_+(0x00e2<<2),(UINT)((_rd32(_REG_BASE_+(0x00e2<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M6r					(ValSft_R00(_rd32(_REG_BASE_+(0x00e2<<2)))&BitMask_10)
#define ODM_M6w(val)			_wr32(_REG_BASE_+(0x00e2<<2),(UINT)((_rd32(_REG_BASE_+(0x00e2<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M7r					(ValSft_R16(_rd32(_REG_BASE_+(0x00e3<<2)))&BitMask_10)
#define ODM_M7w(val)			_wr32(_REG_BASE_+(0x00e3<<2),(UINT)((_rd32(_REG_BASE_+(0x00e3<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define ODM_M8r					(ValSft_R00(_rd32(_REG_BASE_+(0x00e3<<2)))&BitMask_10)
#define ODM_M8w(val)			_wr32(_REG_BASE_+(0x00e3<<2),(UINT)((_rd32(_REG_BASE_+(0x00e3<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define ODM_M9r					(ValSft_R00(_rd32(_REG_BASE_+(0x00e4<<2)))&BitMask_10)
#define ODM_M9w(val)			_wr32(_REG_BASE_+(0x00e4<<2),(UINT)((_rd32(_REG_BASE_+(0x00e4<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define IRIS_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_01)
#define IRIS_ONw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IRIS_INVr				(ValSft_R30(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_01)
#define IRIS_INVw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IRIS_MODr				(ValSft_R28(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_02)
#define IRIS_MODw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define IRIS_WSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_03)
#define IRIS_WSELw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IRIS_FBKr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_08)
#define IRIS_FBKw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IRIS_MUXr				(ValSft_R15(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_01)
#define IRIS_MUXw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define IRIS_TLVr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e5<<2)))&BitMask_10)
#define IRIS_TLVw(val)			_wr32(_REG_BASE_+(0x00e5<<2),(UINT)((_rd32(_REG_BASE_+(0x00e5<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define IRIS_TGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e6<<2)))&BitMask_10)
#define IRIS_TGAw(val)			_wr32(_REG_BASE_+(0x00e6<<2),(UINT)((_rd32(_REG_BASE_+(0x00e6<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define IRIS_OFSr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e6<<2)))&BitMask_14)
#define IRIS_OFSw(val)			_wr32(_REG_BASE_+(0x00e6<<2),(UINT)((_rd32(_REG_BASE_+(0x00e6<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define IRIS_CLPLr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e7<<2)))&BitMask_14)
#define IRIS_CLPLw(val)			_wr32(_REG_BASE_+(0x00e7<<2),(UINT)((_rd32(_REG_BASE_+(0x00e7<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define IRIS_CLPHr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e7<<2)))&BitMask_14)
#define IRIS_CLPHw(val)			_wr32(_REG_BASE_+(0x00e7<<2),(UINT)((_rd32(_REG_BASE_+(0x00e7<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define IRIS_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_14)
#define IRIS_VSPw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define IRIS_VEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e8<<2)))&BitMask_14)
#define IRIS_VEPw(val)			_wr32(_REG_BASE_+(0x00e8<<2),(UINT)((_rd32(_REG_BASE_+(0x00e8<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define IRIS_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_14)
#define IRIS_HSPw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define IRIS_HEPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00e9<<2)))&BitMask_14)
#define IRIS_HEPw(val)			_wr32(_REG_BASE_+(0x00e9<<2),(UINT)((_rd32(_REG_BASE_+(0x00e9<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define THM_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_ONw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define THM_DFIRSTr				(ValSft_R30(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_DFIRSTw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DHBITr					(ValSft_R29(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define DHBITw(val)				_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define DONLYr					(ValSft_R28(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define DONLYw(val)				_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define THM_POL_HSIr			(ValSft_R27(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_POL_HSIw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define THM_POL_VSIr			(ValSft_R26(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_POL_VSIw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define THM_VSYN_NAONr			(ValSft_R25(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_VSYN_NAONw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define THM_ASYNC_ONr			(ValSft_R24(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_ASYNC_ONw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define THM_SYNC_RNr			(ValSft_R23(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_SYNC_RNw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define THM_DDRFF_RSr			(ValSft_R22(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_DDRFF_RSw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define THM_DBYP_ONr			(ValSft_R21(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define THM_DBYP_ONw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define THM_WE_FLG_SELr			(ValSft_R19(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_02)
#define THM_WE_FLG_SELw(val)	_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L19(BitMask_02)))|ValSft_L19((val)&BitMask_02)))

#define TONLYr					(ValSft_R16(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define TONLYw(val)				_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define T_MERGEr				(ValSft_R15(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define T_MERGEw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define T_MERGE_MSBFr			(ValSft_R14(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_01)
#define T_MERGE_MSBFw(val)		_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define T_BIT_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_03)
#define T_BIT_SELw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define THM_INSELr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f0<<2)))&BitMask_03)
#define THM_INSELw(val)			_wr32(_REG_BASE_+(0x00f0<<2),(UINT)((_rd32(_REG_BASE_+(0x00f0<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define THM_VLOCKI_POSr			(ValSft_R16(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_11)
#define THM_VLOCKI_POSw(val)	_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define THM_HLOCKI_POSr			(ValSft_R00(_rd32(_REG_BASE_+(0x00f1<<2)))&BitMask_12)
#define THM_HLOCKI_POSw(val)	_wr32(_REG_BASE_+(0x00f1<<2),(UINT)((_rd32(_REG_BASE_+(0x00f1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define THM_ASYNC_VPOSr			(ValSft_R16(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_11)
#define THM_ASYNC_VPOSw(val)	_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define THM_ASYNC_VSTr			(ValSft_R00(_rd32(_REG_BASE_+(0x00f2<<2)))&BitMask_11)
#define THM_ASYNC_VSTw(val)		_wr32(_REG_BASE_+(0x00f2<<2),(UINT)((_rd32(_REG_BASE_+(0x00f2<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define THM_ASYNC_HWr			(ValSft_R00(_rd32(_REG_BASE_+(0x00f3<<2)))&BitMask_09)
#define THM_ASYNC_HWw(val)		_wr32(_REG_BASE_+(0x00f3<<2),(UINT)((_rd32(_REG_BASE_+(0x00f3<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define THM_HSPIr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_12)
#define THM_HSPIw(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define THM_VSPIr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f4<<2)))&BitMask_11)
#define THM_VSPIw(val)			_wr32(_REG_BASE_+(0x00f4<<2),(UINT)((_rd32(_REG_BASE_+(0x00f4<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define THM_HWIr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_12)
#define THM_HWIw(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define THM_VWIr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f5<<2)))&BitMask_11)
#define THM_VWIw(val)			_wr32(_REG_BASE_+(0x00f5<<2),(UINT)((_rd32(_REG_BASE_+(0x00f5<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define PSEUDO_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_01)
#define PSEUDO_ONw(val)			_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PSEUDO_SELr				(ValSft_R30(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_01)
#define PSEUDO_SELw(val)		_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PSEUDO_WONr				(ValSft_R29(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_01)
#define PSEUDO_WONw(val)		_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PSEUSO_IVSELr			(ValSft_R05(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_03)
#define PSEUSO_IVSELw(val)		_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L05(BitMask_03)))|ValSft_L05((val)&BitMask_03)))

#define PSEUSO_ISELr			(ValSft_R00(_rd32(_REG_BASE_+(0x00f7<<2)))&BitMask_05)
#define PSEUSO_ISELw(val)		_wr32(_REG_BASE_+(0x00f7<<2),(UINT)((_rd32(_REG_BASE_+(0x00f7<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define MAX_TEMPr				(ValSft_R16(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_16)
#define MAX_TEMPw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define MIN_TEMPr				(ValSft_R00(_rd32(_REG_BASE_+(0x00f8<<2)))&BitMask_16)
#define MIN_TEMPw(val)			_wr32(_REG_BASE_+(0x00f8<<2),(UINT)((_rd32(_REG_BASE_+(0x00f8<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define THM_TMP_HSPIr			(ValSft_R16(_rd32(_REG_BASE_+(0x00fa<<2)))&BitMask_12)
#define THM_TMP_HSPIw(val)		_wr32(_REG_BASE_+(0x00fa<<2),(UINT)((_rd32(_REG_BASE_+(0x00fa<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define THM_TMP_VSPIr			(ValSft_R00(_rd32(_REG_BASE_+(0x00fa<<2)))&BitMask_11)
#define THM_TMP_VSPIw(val)		_wr32(_REG_BASE_+(0x00fa<<2),(UINT)((_rd32(_REG_BASE_+(0x00fa<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define THM_TMP_HWIr			(ValSft_R16(_rd32(_REG_BASE_+(0x00fb<<2)))&BitMask_12)
#define THM_TMP_HWIw(val)		_wr32(_REG_BASE_+(0x00fb<<2),(UINT)((_rd32(_REG_BASE_+(0x00fb<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define THM_TMP_VWIr			(ValSft_R00(_rd32(_REG_BASE_+(0x00fb<<2)))&BitMask_11)
#define THM_TMP_VWIw(val)		_wr32(_REG_BASE_+(0x00fb<<2),(UINT)((_rd32(_REG_BASE_+(0x00fb<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define AWB_WIN1_HSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_12)
#define AWB_WIN1_HSPw(val)		_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWB_WIN1_HWr			(ValSft_R00(_rd32(_REG_BASE_+(0x0100<<2)))&BitMask_12)
#define AWB_WIN1_HWw(val)		_wr32(_REG_BASE_+(0x0100<<2),(UINT)((_rd32(_REG_BASE_+(0x0100<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWB_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0101<<2)))&BitMask_01)
#define AWB_ONw(val)			_wr32(_REG_BASE_+(0x0101<<2),(UINT)((_rd32(_REG_BASE_+(0x0101<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AWB_WIN1_VSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0101<<2)))&BitMask_12)
#define AWB_WIN1_VSPw(val)		_wr32(_REG_BASE_+(0x0101<<2),(UINT)((_rd32(_REG_BASE_+(0x0101<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWB_WIN1_VWr			(ValSft_R00(_rd32(_REG_BASE_+(0x0101<<2)))&BitMask_12)
#define AWB_WIN1_VWw(val)		_wr32(_REG_BASE_+(0x0101<<2),(UINT)((_rd32(_REG_BASE_+(0x0101<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWBL_DBGAIN0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define AWBL_DBGAIN0w(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBL_DRGAIN0r			(ValSft_R08(_rd32(_REG_BASE_+(0x0102<<2)))&BitMask_08)
#define AWBL_DRGAIN0w(val)		_wr32(_REG_BASE_+(0x0102<<2),(UINT)((_rd32(_REG_BASE_+(0x0102<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define AWBL0_DBOFFSETr			(ValSft_R16(_rd32(_REG_BASE_+(0x0103<<2)))&BitMask_08)
#define AWBL0_DBOFFSETw(val)	_wr32(_REG_BASE_+(0x0103<<2),(UINT)((_rd32(_REG_BASE_+(0x0103<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AWBL0_DROFFSETr			(ValSft_R00(_rd32(_REG_BASE_+(0x0103<<2)))&BitMask_08)
#define AWBL0_DROFFSETw(val)	_wr32(_REG_BASE_+(0x0103<<2),(UINT)((_rd32(_REG_BASE_+(0x0103<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBLCBCRO_SELr			(ValSft_R30(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_01)
#define AWBLCBCRO_SELw(val)		_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define AWBLNOR_SELr			(ValSft_R29(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_01)
#define AWBLNOR_SELw(val)		_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define AWB_LOCK_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_01)
#define AWB_LOCK_SELw(val)		_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define AWBL_HTAB_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_02)
#define AWBL_HTAB_SELw(val)		_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define AWBLYEDGE_THr			(ValSft_R00(_rd32(_REG_BASE_+(0x0104<<2)))&BitMask_08)
#define AWBLYEDGE_THw(val)		_wr32(_REG_BASE_+(0x0104<<2),(UINT)((_rd32(_REG_BASE_+(0x0104<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBLOSD_ONr				(ValSft_R08(_rd32(_REG_BASE_+(0x0105<<2)))&BitMask_01)
#define AWBLOSD_ONw(val)		_wr32(_REG_BASE_+(0x0105<<2),(UINT)((_rd32(_REG_BASE_+(0x0105<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define AWBL_OSD_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0105<<2)))&BitMask_08)
#define AWBL_OSD_SELw(val)		_wr32(_REG_BASE_+(0x0105<<2),(UINT)((_rd32(_REG_BASE_+(0x0105<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBL_CYCLIPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_08)
#define AWBL_CYCLIPw(val)		_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define AWBL_CYSLICEr			(ValSft_R00(_rd32(_REG_BASE_+(0x0106<<2)))&BitMask_08)
#define AWBL_CYSLICEw(val)		_wr32(_REG_BASE_+(0x0106<<2),(UINT)((_rd32(_REG_BASE_+(0x0106<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define AWBL_CM_HSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0107<<2)))&BitMask_12)
#define AWBL_CM_HSPw(val)		_wr32(_REG_BASE_+(0x0107<<2),(UINT)((_rd32(_REG_BASE_+(0x0107<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWBL_CM_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0107<<2)))&BitMask_12)
#define AWBL_CM_VSPw(val)		_wr32(_REG_BASE_+(0x0107<<2),(UINT)((_rd32(_REG_BASE_+(0x0107<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AWBL_CM00_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0108<<2)))&BitMask_06)
#define AWBL_CM00_HSPw(val)		_wr32(_REG_BASE_+(0x0108<<2),(UINT)((_rd32(_REG_BASE_+(0x0108<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM00_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0108<<2)))&BitMask_06)
#define AWBL_CM00_HEPw(val)		_wr32(_REG_BASE_+(0x0108<<2),(UINT)((_rd32(_REG_BASE_+(0x0108<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM01_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0109<<2)))&BitMask_06)
#define AWBL_CM01_HSPw(val)		_wr32(_REG_BASE_+(0x0109<<2),(UINT)((_rd32(_REG_BASE_+(0x0109<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM01_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0109<<2)))&BitMask_06)
#define AWBL_CM01_HEPw(val)		_wr32(_REG_BASE_+(0x0109<<2),(UINT)((_rd32(_REG_BASE_+(0x0109<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM02_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010a<<2)))&BitMask_06)
#define AWBL_CM02_HSPw(val)		_wr32(_REG_BASE_+(0x010a<<2),(UINT)((_rd32(_REG_BASE_+(0x010a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM02_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010a<<2)))&BitMask_06)
#define AWBL_CM02_HEPw(val)		_wr32(_REG_BASE_+(0x010a<<2),(UINT)((_rd32(_REG_BASE_+(0x010a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM03_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010b<<2)))&BitMask_06)
#define AWBL_CM03_HSPw(val)		_wr32(_REG_BASE_+(0x010b<<2),(UINT)((_rd32(_REG_BASE_+(0x010b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM03_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010b<<2)))&BitMask_06)
#define AWBL_CM03_HEPw(val)		_wr32(_REG_BASE_+(0x010b<<2),(UINT)((_rd32(_REG_BASE_+(0x010b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM04_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010c<<2)))&BitMask_06)
#define AWBL_CM04_HSPw(val)		_wr32(_REG_BASE_+(0x010c<<2),(UINT)((_rd32(_REG_BASE_+(0x010c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM04_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010c<<2)))&BitMask_06)
#define AWBL_CM04_HEPw(val)		_wr32(_REG_BASE_+(0x010c<<2),(UINT)((_rd32(_REG_BASE_+(0x010c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM05_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010d<<2)))&BitMask_06)
#define AWBL_CM05_HSPw(val)		_wr32(_REG_BASE_+(0x010d<<2),(UINT)((_rd32(_REG_BASE_+(0x010d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM05_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010d<<2)))&BitMask_06)
#define AWBL_CM05_HEPw(val)		_wr32(_REG_BASE_+(0x010d<<2),(UINT)((_rd32(_REG_BASE_+(0x010d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM06_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010e<<2)))&BitMask_06)
#define AWBL_CM06_HSPw(val)		_wr32(_REG_BASE_+(0x010e<<2),(UINT)((_rd32(_REG_BASE_+(0x010e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM06_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010e<<2)))&BitMask_06)
#define AWBL_CM06_HEPw(val)		_wr32(_REG_BASE_+(0x010e<<2),(UINT)((_rd32(_REG_BASE_+(0x010e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM07_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x010f<<2)))&BitMask_06)
#define AWBL_CM07_HSPw(val)		_wr32(_REG_BASE_+(0x010f<<2),(UINT)((_rd32(_REG_BASE_+(0x010f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM07_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x010f<<2)))&BitMask_06)
#define AWBL_CM07_HEPw(val)		_wr32(_REG_BASE_+(0x010f<<2),(UINT)((_rd32(_REG_BASE_+(0x010f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM08_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0110<<2)))&BitMask_06)
#define AWBL_CM08_HSPw(val)		_wr32(_REG_BASE_+(0x0110<<2),(UINT)((_rd32(_REG_BASE_+(0x0110<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM08_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0110<<2)))&BitMask_06)
#define AWBL_CM08_HEPw(val)		_wr32(_REG_BASE_+(0x0110<<2),(UINT)((_rd32(_REG_BASE_+(0x0110<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM09_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0111<<2)))&BitMask_06)
#define AWBL_CM09_HSPw(val)		_wr32(_REG_BASE_+(0x0111<<2),(UINT)((_rd32(_REG_BASE_+(0x0111<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM09_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0111<<2)))&BitMask_06)
#define AWBL_CM09_HEPw(val)		_wr32(_REG_BASE_+(0x0111<<2),(UINT)((_rd32(_REG_BASE_+(0x0111<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM10_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0112<<2)))&BitMask_06)
#define AWBL_CM10_HSPw(val)		_wr32(_REG_BASE_+(0x0112<<2),(UINT)((_rd32(_REG_BASE_+(0x0112<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM10_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0112<<2)))&BitMask_06)
#define AWBL_CM10_HEPw(val)		_wr32(_REG_BASE_+(0x0112<<2),(UINT)((_rd32(_REG_BASE_+(0x0112<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM11_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0113<<2)))&BitMask_06)
#define AWBL_CM11_HSPw(val)		_wr32(_REG_BASE_+(0x0113<<2),(UINT)((_rd32(_REG_BASE_+(0x0113<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM11_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0113<<2)))&BitMask_06)
#define AWBL_CM11_HEPw(val)		_wr32(_REG_BASE_+(0x0113<<2),(UINT)((_rd32(_REG_BASE_+(0x0113<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM12_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0114<<2)))&BitMask_06)
#define AWBL_CM12_HSPw(val)		_wr32(_REG_BASE_+(0x0114<<2),(UINT)((_rd32(_REG_BASE_+(0x0114<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM12_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0114<<2)))&BitMask_06)
#define AWBL_CM12_HEPw(val)		_wr32(_REG_BASE_+(0x0114<<2),(UINT)((_rd32(_REG_BASE_+(0x0114<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM13_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0115<<2)))&BitMask_06)
#define AWBL_CM13_HSPw(val)		_wr32(_REG_BASE_+(0x0115<<2),(UINT)((_rd32(_REG_BASE_+(0x0115<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM13_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0115<<2)))&BitMask_06)
#define AWBL_CM13_HEPw(val)		_wr32(_REG_BASE_+(0x0115<<2),(UINT)((_rd32(_REG_BASE_+(0x0115<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM14_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0116<<2)))&BitMask_06)
#define AWBL_CM14_HSPw(val)		_wr32(_REG_BASE_+(0x0116<<2),(UINT)((_rd32(_REG_BASE_+(0x0116<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM14_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0116<<2)))&BitMask_06)
#define AWBL_CM14_HEPw(val)		_wr32(_REG_BASE_+(0x0116<<2),(UINT)((_rd32(_REG_BASE_+(0x0116<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM15_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0117<<2)))&BitMask_06)
#define AWBL_CM15_HSPw(val)		_wr32(_REG_BASE_+(0x0117<<2),(UINT)((_rd32(_REG_BASE_+(0x0117<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM15_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0117<<2)))&BitMask_06)
#define AWBL_CM15_HEPw(val)		_wr32(_REG_BASE_+(0x0117<<2),(UINT)((_rd32(_REG_BASE_+(0x0117<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM16_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0118<<2)))&BitMask_06)
#define AWBL_CM16_HSPw(val)		_wr32(_REG_BASE_+(0x0118<<2),(UINT)((_rd32(_REG_BASE_+(0x0118<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM16_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0118<<2)))&BitMask_06)
#define AWBL_CM16_HEPw(val)		_wr32(_REG_BASE_+(0x0118<<2),(UINT)((_rd32(_REG_BASE_+(0x0118<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM17_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_06)
#define AWBL_CM17_HSPw(val)		_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM17_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0119<<2)))&BitMask_06)
#define AWBL_CM17_HEPw(val)		_wr32(_REG_BASE_+(0x0119<<2),(UINT)((_rd32(_REG_BASE_+(0x0119<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM18_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011a<<2)))&BitMask_06)
#define AWBL_CM18_HSPw(val)		_wr32(_REG_BASE_+(0x011a<<2),(UINT)((_rd32(_REG_BASE_+(0x011a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM18_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011a<<2)))&BitMask_06)
#define AWBL_CM18_HEPw(val)		_wr32(_REG_BASE_+(0x011a<<2),(UINT)((_rd32(_REG_BASE_+(0x011a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM19_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011b<<2)))&BitMask_06)
#define AWBL_CM19_HSPw(val)		_wr32(_REG_BASE_+(0x011b<<2),(UINT)((_rd32(_REG_BASE_+(0x011b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM19_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011b<<2)))&BitMask_06)
#define AWBL_CM19_HEPw(val)		_wr32(_REG_BASE_+(0x011b<<2),(UINT)((_rd32(_REG_BASE_+(0x011b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM20_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011c<<2)))&BitMask_06)
#define AWBL_CM20_HSPw(val)		_wr32(_REG_BASE_+(0x011c<<2),(UINT)((_rd32(_REG_BASE_+(0x011c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM20_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011c<<2)))&BitMask_06)
#define AWBL_CM20_HEPw(val)		_wr32(_REG_BASE_+(0x011c<<2),(UINT)((_rd32(_REG_BASE_+(0x011c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM21_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011d<<2)))&BitMask_06)
#define AWBL_CM21_HSPw(val)		_wr32(_REG_BASE_+(0x011d<<2),(UINT)((_rd32(_REG_BASE_+(0x011d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM21_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011d<<2)))&BitMask_06)
#define AWBL_CM21_HEPw(val)		_wr32(_REG_BASE_+(0x011d<<2),(UINT)((_rd32(_REG_BASE_+(0x011d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM22_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011e<<2)))&BitMask_06)
#define AWBL_CM22_HSPw(val)		_wr32(_REG_BASE_+(0x011e<<2),(UINT)((_rd32(_REG_BASE_+(0x011e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM22_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011e<<2)))&BitMask_06)
#define AWBL_CM22_HEPw(val)		_wr32(_REG_BASE_+(0x011e<<2),(UINT)((_rd32(_REG_BASE_+(0x011e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM23_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_06)
#define AWBL_CM23_HSPw(val)		_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM23_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x011f<<2)))&BitMask_06)
#define AWBL_CM23_HEPw(val)		_wr32(_REG_BASE_+(0x011f<<2),(UINT)((_rd32(_REG_BASE_+(0x011f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM24_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0120<<2)))&BitMask_06)
#define AWBL_CM24_HSPw(val)		_wr32(_REG_BASE_+(0x0120<<2),(UINT)((_rd32(_REG_BASE_+(0x0120<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM24_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0120<<2)))&BitMask_06)
#define AWBL_CM24_HEPw(val)		_wr32(_REG_BASE_+(0x0120<<2),(UINT)((_rd32(_REG_BASE_+(0x0120<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM25_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_06)
#define AWBL_CM25_HSPw(val)		_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM25_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0121<<2)))&BitMask_06)
#define AWBL_CM25_HEPw(val)		_wr32(_REG_BASE_+(0x0121<<2),(UINT)((_rd32(_REG_BASE_+(0x0121<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM26_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0122<<2)))&BitMask_06)
#define AWBL_CM26_HSPw(val)		_wr32(_REG_BASE_+(0x0122<<2),(UINT)((_rd32(_REG_BASE_+(0x0122<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM26_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0122<<2)))&BitMask_06)
#define AWBL_CM26_HEPw(val)		_wr32(_REG_BASE_+(0x0122<<2),(UINT)((_rd32(_REG_BASE_+(0x0122<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM27_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_06)
#define AWBL_CM27_HSPw(val)		_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM27_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0123<<2)))&BitMask_06)
#define AWBL_CM27_HEPw(val)		_wr32(_REG_BASE_+(0x0123<<2),(UINT)((_rd32(_REG_BASE_+(0x0123<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM28_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_06)
#define AWBL_CM28_HSPw(val)		_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM28_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0124<<2)))&BitMask_06)
#define AWBL_CM28_HEPw(val)		_wr32(_REG_BASE_+(0x0124<<2),(UINT)((_rd32(_REG_BASE_+(0x0124<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM29_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_06)
#define AWBL_CM29_HSPw(val)		_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM29_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0125<<2)))&BitMask_06)
#define AWBL_CM29_HEPw(val)		_wr32(_REG_BASE_+(0x0125<<2),(UINT)((_rd32(_REG_BASE_+(0x0125<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM30_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0126<<2)))&BitMask_06)
#define AWBL_CM30_HSPw(val)		_wr32(_REG_BASE_+(0x0126<<2),(UINT)((_rd32(_REG_BASE_+(0x0126<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM30_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0126<<2)))&BitMask_06)
#define AWBL_CM30_HEPw(val)		_wr32(_REG_BASE_+(0x0126<<2),(UINT)((_rd32(_REG_BASE_+(0x0126<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM31_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0127<<2)))&BitMask_06)
#define AWBL_CM31_HSPw(val)		_wr32(_REG_BASE_+(0x0127<<2),(UINT)((_rd32(_REG_BASE_+(0x0127<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM31_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0127<<2)))&BitMask_06)
#define AWBL_CM31_HEPw(val)		_wr32(_REG_BASE_+(0x0127<<2),(UINT)((_rd32(_REG_BASE_+(0x0127<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM32_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0128<<2)))&BitMask_06)
#define AWBL_CM32_HSPw(val)		_wr32(_REG_BASE_+(0x0128<<2),(UINT)((_rd32(_REG_BASE_+(0x0128<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM32_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0128<<2)))&BitMask_06)
#define AWBL_CM32_HEPw(val)		_wr32(_REG_BASE_+(0x0128<<2),(UINT)((_rd32(_REG_BASE_+(0x0128<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM33_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0129<<2)))&BitMask_06)
#define AWBL_CM33_HSPw(val)		_wr32(_REG_BASE_+(0x0129<<2),(UINT)((_rd32(_REG_BASE_+(0x0129<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM33_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0129<<2)))&BitMask_06)
#define AWBL_CM33_HEPw(val)		_wr32(_REG_BASE_+(0x0129<<2),(UINT)((_rd32(_REG_BASE_+(0x0129<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM34_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012a<<2)))&BitMask_06)
#define AWBL_CM34_HSPw(val)		_wr32(_REG_BASE_+(0x012a<<2),(UINT)((_rd32(_REG_BASE_+(0x012a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM34_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012a<<2)))&BitMask_06)
#define AWBL_CM34_HEPw(val)		_wr32(_REG_BASE_+(0x012a<<2),(UINT)((_rd32(_REG_BASE_+(0x012a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM35_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012b<<2)))&BitMask_06)
#define AWBL_CM35_HSPw(val)		_wr32(_REG_BASE_+(0x012b<<2),(UINT)((_rd32(_REG_BASE_+(0x012b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM35_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012b<<2)))&BitMask_06)
#define AWBL_CM35_HEPw(val)		_wr32(_REG_BASE_+(0x012b<<2),(UINT)((_rd32(_REG_BASE_+(0x012b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM36_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012c<<2)))&BitMask_06)
#define AWBL_CM36_HSPw(val)		_wr32(_REG_BASE_+(0x012c<<2),(UINT)((_rd32(_REG_BASE_+(0x012c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM36_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012c<<2)))&BitMask_06)
#define AWBL_CM36_HEPw(val)		_wr32(_REG_BASE_+(0x012c<<2),(UINT)((_rd32(_REG_BASE_+(0x012c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM37_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012d<<2)))&BitMask_06)
#define AWBL_CM37_HSPw(val)		_wr32(_REG_BASE_+(0x012d<<2),(UINT)((_rd32(_REG_BASE_+(0x012d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM37_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012d<<2)))&BitMask_06)
#define AWBL_CM37_HEPw(val)		_wr32(_REG_BASE_+(0x012d<<2),(UINT)((_rd32(_REG_BASE_+(0x012d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM38_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012e<<2)))&BitMask_06)
#define AWBL_CM38_HSPw(val)		_wr32(_REG_BASE_+(0x012e<<2),(UINT)((_rd32(_REG_BASE_+(0x012e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM38_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012e<<2)))&BitMask_06)
#define AWBL_CM38_HEPw(val)		_wr32(_REG_BASE_+(0x012e<<2),(UINT)((_rd32(_REG_BASE_+(0x012e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM39_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x012f<<2)))&BitMask_06)
#define AWBL_CM39_HSPw(val)		_wr32(_REG_BASE_+(0x012f<<2),(UINT)((_rd32(_REG_BASE_+(0x012f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM39_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x012f<<2)))&BitMask_06)
#define AWBL_CM39_HEPw(val)		_wr32(_REG_BASE_+(0x012f<<2),(UINT)((_rd32(_REG_BASE_+(0x012f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM40_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0130<<2)))&BitMask_06)
#define AWBL_CM40_HSPw(val)		_wr32(_REG_BASE_+(0x0130<<2),(UINT)((_rd32(_REG_BASE_+(0x0130<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM40_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0130<<2)))&BitMask_06)
#define AWBL_CM40_HEPw(val)		_wr32(_REG_BASE_+(0x0130<<2),(UINT)((_rd32(_REG_BASE_+(0x0130<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM41_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0131<<2)))&BitMask_06)
#define AWBL_CM41_HSPw(val)		_wr32(_REG_BASE_+(0x0131<<2),(UINT)((_rd32(_REG_BASE_+(0x0131<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM41_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0131<<2)))&BitMask_06)
#define AWBL_CM41_HEPw(val)		_wr32(_REG_BASE_+(0x0131<<2),(UINT)((_rd32(_REG_BASE_+(0x0131<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM42_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0132<<2)))&BitMask_06)
#define AWBL_CM42_HSPw(val)		_wr32(_REG_BASE_+(0x0132<<2),(UINT)((_rd32(_REG_BASE_+(0x0132<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM42_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0132<<2)))&BitMask_06)
#define AWBL_CM42_HEPw(val)		_wr32(_REG_BASE_+(0x0132<<2),(UINT)((_rd32(_REG_BASE_+(0x0132<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM43_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0133<<2)))&BitMask_06)
#define AWBL_CM43_HSPw(val)		_wr32(_REG_BASE_+(0x0133<<2),(UINT)((_rd32(_REG_BASE_+(0x0133<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM43_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0133<<2)))&BitMask_06)
#define AWBL_CM43_HEPw(val)		_wr32(_REG_BASE_+(0x0133<<2),(UINT)((_rd32(_REG_BASE_+(0x0133<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM44_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0134<<2)))&BitMask_06)
#define AWBL_CM44_HSPw(val)		_wr32(_REG_BASE_+(0x0134<<2),(UINT)((_rd32(_REG_BASE_+(0x0134<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM44_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0134<<2)))&BitMask_06)
#define AWBL_CM44_HEPw(val)		_wr32(_REG_BASE_+(0x0134<<2),(UINT)((_rd32(_REG_BASE_+(0x0134<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM45_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0135<<2)))&BitMask_06)
#define AWBL_CM45_HSPw(val)		_wr32(_REG_BASE_+(0x0135<<2),(UINT)((_rd32(_REG_BASE_+(0x0135<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM45_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0135<<2)))&BitMask_06)
#define AWBL_CM45_HEPw(val)		_wr32(_REG_BASE_+(0x0135<<2),(UINT)((_rd32(_REG_BASE_+(0x0135<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM46_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0136<<2)))&BitMask_06)
#define AWBL_CM46_HSPw(val)		_wr32(_REG_BASE_+(0x0136<<2),(UINT)((_rd32(_REG_BASE_+(0x0136<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM46_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0136<<2)))&BitMask_06)
#define AWBL_CM46_HEPw(val)		_wr32(_REG_BASE_+(0x0136<<2),(UINT)((_rd32(_REG_BASE_+(0x0136<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM47_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0137<<2)))&BitMask_06)
#define AWBL_CM47_HSPw(val)		_wr32(_REG_BASE_+(0x0137<<2),(UINT)((_rd32(_REG_BASE_+(0x0137<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM47_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0137<<2)))&BitMask_06)
#define AWBL_CM47_HEPw(val)		_wr32(_REG_BASE_+(0x0137<<2),(UINT)((_rd32(_REG_BASE_+(0x0137<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM48_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0138<<2)))&BitMask_06)
#define AWBL_CM48_HSPw(val)		_wr32(_REG_BASE_+(0x0138<<2),(UINT)((_rd32(_REG_BASE_+(0x0138<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM48_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0138<<2)))&BitMask_06)
#define AWBL_CM48_HEPw(val)		_wr32(_REG_BASE_+(0x0138<<2),(UINT)((_rd32(_REG_BASE_+(0x0138<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM49_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0139<<2)))&BitMask_06)
#define AWBL_CM49_HSPw(val)		_wr32(_REG_BASE_+(0x0139<<2),(UINT)((_rd32(_REG_BASE_+(0x0139<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM49_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0139<<2)))&BitMask_06)
#define AWBL_CM49_HEPw(val)		_wr32(_REG_BASE_+(0x0139<<2),(UINT)((_rd32(_REG_BASE_+(0x0139<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM50_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013a<<2)))&BitMask_06)
#define AWBL_CM50_HSPw(val)		_wr32(_REG_BASE_+(0x013a<<2),(UINT)((_rd32(_REG_BASE_+(0x013a<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM50_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013a<<2)))&BitMask_06)
#define AWBL_CM50_HEPw(val)		_wr32(_REG_BASE_+(0x013a<<2),(UINT)((_rd32(_REG_BASE_+(0x013a<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM51_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013b<<2)))&BitMask_06)
#define AWBL_CM51_HSPw(val)		_wr32(_REG_BASE_+(0x013b<<2),(UINT)((_rd32(_REG_BASE_+(0x013b<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM51_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013b<<2)))&BitMask_06)
#define AWBL_CM51_HEPw(val)		_wr32(_REG_BASE_+(0x013b<<2),(UINT)((_rd32(_REG_BASE_+(0x013b<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM52_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013c<<2)))&BitMask_06)
#define AWBL_CM52_HSPw(val)		_wr32(_REG_BASE_+(0x013c<<2),(UINT)((_rd32(_REG_BASE_+(0x013c<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM52_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013c<<2)))&BitMask_06)
#define AWBL_CM52_HEPw(val)		_wr32(_REG_BASE_+(0x013c<<2),(UINT)((_rd32(_REG_BASE_+(0x013c<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM53_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013d<<2)))&BitMask_06)
#define AWBL_CM53_HSPw(val)		_wr32(_REG_BASE_+(0x013d<<2),(UINT)((_rd32(_REG_BASE_+(0x013d<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM53_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013d<<2)))&BitMask_06)
#define AWBL_CM53_HEPw(val)		_wr32(_REG_BASE_+(0x013d<<2),(UINT)((_rd32(_REG_BASE_+(0x013d<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM54_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013e<<2)))&BitMask_06)
#define AWBL_CM54_HSPw(val)		_wr32(_REG_BASE_+(0x013e<<2),(UINT)((_rd32(_REG_BASE_+(0x013e<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM54_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013e<<2)))&BitMask_06)
#define AWBL_CM54_HEPw(val)		_wr32(_REG_BASE_+(0x013e<<2),(UINT)((_rd32(_REG_BASE_+(0x013e<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM55_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x013f<<2)))&BitMask_06)
#define AWBL_CM55_HSPw(val)		_wr32(_REG_BASE_+(0x013f<<2),(UINT)((_rd32(_REG_BASE_+(0x013f<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM55_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x013f<<2)))&BitMask_06)
#define AWBL_CM55_HEPw(val)		_wr32(_REG_BASE_+(0x013f<<2),(UINT)((_rd32(_REG_BASE_+(0x013f<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM56_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0140<<2)))&BitMask_06)
#define AWBL_CM56_HSPw(val)		_wr32(_REG_BASE_+(0x0140<<2),(UINT)((_rd32(_REG_BASE_+(0x0140<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM56_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0140<<2)))&BitMask_06)
#define AWBL_CM56_HEPw(val)		_wr32(_REG_BASE_+(0x0140<<2),(UINT)((_rd32(_REG_BASE_+(0x0140<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM57_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0141<<2)))&BitMask_06)
#define AWBL_CM57_HSPw(val)		_wr32(_REG_BASE_+(0x0141<<2),(UINT)((_rd32(_REG_BASE_+(0x0141<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM57_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0141<<2)))&BitMask_06)
#define AWBL_CM57_HEPw(val)		_wr32(_REG_BASE_+(0x0141<<2),(UINT)((_rd32(_REG_BASE_+(0x0141<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM58_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0142<<2)))&BitMask_06)
#define AWBL_CM58_HSPw(val)		_wr32(_REG_BASE_+(0x0142<<2),(UINT)((_rd32(_REG_BASE_+(0x0142<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM58_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0142<<2)))&BitMask_06)
#define AWBL_CM58_HEPw(val)		_wr32(_REG_BASE_+(0x0142<<2),(UINT)((_rd32(_REG_BASE_+(0x0142<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM59_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0143<<2)))&BitMask_06)
#define AWBL_CM59_HSPw(val)		_wr32(_REG_BASE_+(0x0143<<2),(UINT)((_rd32(_REG_BASE_+(0x0143<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM59_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0143<<2)))&BitMask_06)
#define AWBL_CM59_HEPw(val)		_wr32(_REG_BASE_+(0x0143<<2),(UINT)((_rd32(_REG_BASE_+(0x0143<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM60_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0144<<2)))&BitMask_06)
#define AWBL_CM60_HSPw(val)		_wr32(_REG_BASE_+(0x0144<<2),(UINT)((_rd32(_REG_BASE_+(0x0144<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM60_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0144<<2)))&BitMask_06)
#define AWBL_CM60_HEPw(val)		_wr32(_REG_BASE_+(0x0144<<2),(UINT)((_rd32(_REG_BASE_+(0x0144<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM61_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0145<<2)))&BitMask_06)
#define AWBL_CM61_HSPw(val)		_wr32(_REG_BASE_+(0x0145<<2),(UINT)((_rd32(_REG_BASE_+(0x0145<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM61_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0145<<2)))&BitMask_06)
#define AWBL_CM61_HEPw(val)		_wr32(_REG_BASE_+(0x0145<<2),(UINT)((_rd32(_REG_BASE_+(0x0145<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM62_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0146<<2)))&BitMask_06)
#define AWBL_CM62_HSPw(val)		_wr32(_REG_BASE_+(0x0146<<2),(UINT)((_rd32(_REG_BASE_+(0x0146<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM62_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0146<<2)))&BitMask_06)
#define AWBL_CM62_HEPw(val)		_wr32(_REG_BASE_+(0x0146<<2),(UINT)((_rd32(_REG_BASE_+(0x0146<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWBL_CM63_HSPr			(ValSft_R08(_rd32(_REG_BASE_+(0x0147<<2)))&BitMask_06)
#define AWBL_CM63_HSPw(val)		_wr32(_REG_BASE_+(0x0147<<2),(UINT)((_rd32(_REG_BASE_+(0x0147<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define AWBL_CM63_HEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0147<<2)))&BitMask_06)
#define AWBL_CM63_HEPw(val)		_wr32(_REG_BASE_+(0x0147<<2),(UINT)((_rd32(_REG_BASE_+(0x0147<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define AWB_SUBWIN_SPCr			(ValSft_R00(_rd32(_REG_BASE_+(0x0148<<2)))&BitMask_10)
#define AWB_SUBWIN_SPCw(val)	_wr32(_REG_BASE_+(0x0148<<2),(UINT)((_rd32(_REG_BASE_+(0x0148<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define AWB_SUBWIN_HSPr			(ValSft_R16(_rd32(_REG_BASE_+(0x0149<<2)))&BitMask_12)
#define AWB_SUBWIN_HSPw(val)	_wr32(_REG_BASE_+(0x0149<<2),(UINT)((_rd32(_REG_BASE_+(0x0149<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AWB_SUBWIN_VSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x0149<<2)))&BitMask_12)
#define AWB_SUBWIN_VSPw(val)	_wr32(_REG_BASE_+(0x0149<<2),(UINT)((_rd32(_REG_BASE_+(0x0149<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define IMD_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_01)
#define IMD_ONw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_FLTr				(ValSft_R30(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_01)
#define IMD_FLTw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMD_TESTr				(ValSft_R28(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_02)
#define IMD_TESTw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define IMD_DUALr				(ValSft_R27(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_01)
#define IMD_DUALw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IMD_GMAr				(ValSft_R24(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_01)
#define IMD_GMAw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define IMD_SELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_02)
#define IMD_SELw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define IMD_THr					(ValSft_R08(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_08)
#define IMD_THw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define IMD_BUPCr				(ValSft_R00(_rd32(_REG_BASE_+(0x0150<<2)))&BitMask_08)
#define IMD_BUPCw(val)			_wr32(_REG_BASE_+(0x0150<<2),(UINT)((_rd32(_REG_BASE_+(0x0150<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define IMD_OSDONr				(ValSft_R16(_rd32(_REG_BASE_+(0x0151<<2)))&BitMask_06)
#define IMD_OSDONw(val)			_wr32(_REG_BASE_+(0x0151<<2),(UINT)((_rd32(_REG_BASE_+(0x0151<<2))&(~ValSft_L16(BitMask_06)))|ValSft_L16((val)&BitMask_06)))

#define IMD_OSDBDWr				(ValSft_R23(_rd32(_REG_BASE_+(0x0151<<2)))&BitMask_01)
#define IMD_OSDBDWw(val)		_wr32(_REG_BASE_+(0x0151<<2),(UINT)((_rd32(_REG_BASE_+(0x0151<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IMD_OSDLVr				(ValSft_R00(_rd32(_REG_BASE_+(0x0151<<2)))&BitMask_10)
#define IMD_OSDLVw(val)			_wr32(_REG_BASE_+(0x0151<<2),(UINT)((_rd32(_REG_BASE_+(0x0151<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define IMD_HWr					(ValSft_R08(_rd32(_REG_BASE_+(0x0152<<2)))&BitMask_07)
#define IMD_HWw(val)			_wr32(_REG_BASE_+(0x0152<<2),(UINT)((_rd32(_REG_BASE_+(0x0152<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x0152<<2)))&BitMask_07)
#define IMD_VWw(val)			_wr32(_REG_BASE_+(0x0152<<2),(UINT)((_rd32(_REG_BASE_+(0x0152<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0153<<2)))&BitMask_11)
#define IMD_HSPw(val)			_wr32(_REG_BASE_+(0x0153<<2),(UINT)((_rd32(_REG_BASE_+(0x0153<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define IMD_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0153<<2)))&BitMask_11)
#define IMD_VSPw(val)			_wr32(_REG_BASE_+(0x0153<<2),(UINT)((_rd32(_REG_BASE_+(0x0153<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define IMD_KMAXr				(ValSft_R24(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_08)
#define IMD_KMAXw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define IMD_KMINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_08)
#define IMD_KMINw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define IMD_KGAr				(ValSft_R08(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_08)
#define IMD_KGAw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define IMD_KTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x0154<<2)))&BitMask_08)
#define IMD_KTHw(val)			_wr32(_REG_BASE_+(0x0154<<2),(UINT)((_rd32(_REG_BASE_+(0x0154<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define IMD_WON1r				(ValSft_R31(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_01)
#define IMD_WON1w(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WVSP1r				(ValSft_R24(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_07)
#define IMD_WVSP1w(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WVEP1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_07)
#define IMD_WVEP1w(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WHSP1r				(ValSft_R08(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_07)
#define IMD_WHSP1w(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_WHEP1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0155<<2)))&BitMask_07)
#define IMD_WHEP1w(val)			_wr32(_REG_BASE_+(0x0155<<2),(UINT)((_rd32(_REG_BASE_+(0x0155<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_WON2r				(ValSft_R31(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_01)
#define IMD_WON2w(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WVSP2r				(ValSft_R24(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_07)
#define IMD_WVSP2w(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WVEP2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_07)
#define IMD_WVEP2w(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WHSP2r				(ValSft_R08(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_07)
#define IMD_WHSP2w(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_WHEP2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0156<<2)))&BitMask_07)
#define IMD_WHEP2w(val)			_wr32(_REG_BASE_+(0x0156<<2),(UINT)((_rd32(_REG_BASE_+(0x0156<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_WON3r				(ValSft_R31(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_01)
#define IMD_WON3w(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WVSP3r				(ValSft_R24(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_07)
#define IMD_WVSP3w(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WVEP3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_07)
#define IMD_WVEP3w(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WHSP3r				(ValSft_R08(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_07)
#define IMD_WHSP3w(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_WHEP3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0157<<2)))&BitMask_07)
#define IMD_WHEP3w(val)			_wr32(_REG_BASE_+(0x0157<<2),(UINT)((_rd32(_REG_BASE_+(0x0157<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_WON4r				(ValSft_R31(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_01)
#define IMD_WON4w(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMD_WVSP4r				(ValSft_R24(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_07)
#define IMD_WVSP4w(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L24(BitMask_07)))|ValSft_L24((val)&BitMask_07)))

#define IMD_WVEP4r				(ValSft_R16(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_07)
#define IMD_WVEP4w(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L16(BitMask_07)))|ValSft_L16((val)&BitMask_07)))

#define IMD_WHSP4r				(ValSft_R08(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_07)
#define IMD_WHSP4w(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L08(BitMask_07)))|ValSft_L08((val)&BitMask_07)))

#define IMD_WHEP4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0158<<2)))&BitMask_07)
#define IMD_WHEP4w(val)			_wr32(_REG_BASE_+(0x0158<<2),(UINT)((_rd32(_REG_BASE_+(0x0158<<2))&(~ValSft_L00(BitMask_07)))|ValSft_L00((val)&BitMask_07)))

#define IMD_ISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0159<<2)))&BitMask_04)
#define IMD_ISELw(val)			_wr32(_REG_BASE_+(0x0159<<2),(UINT)((_rd32(_REG_BASE_+(0x0159<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define IMD_IVSELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0159<<2)))&BitMask_04)
#define IMD_IVSELw(val)			_wr32(_REG_BASE_+(0x0159<<2),(UINT)((_rd32(_REG_BASE_+(0x0159<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define IMD_VMODr				(ValSft_R06(_rd32(_REG_BASE_+(0x0159<<2)))&BitMask_01)
#define IMD_VMODw(val)			_wr32(_REG_BASE_+(0x0159<<2),(UINT)((_rd32(_REG_BASE_+(0x0159<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define POST_YBGAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x0160<<2)))&BitMask_08)
#define POST_YBGAINw(val)		_wr32(_REG_BASE_+(0x0160<<2),(UINT)((_rd32(_REG_BASE_+(0x0160<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define POST_YGGAINr			(ValSft_R08(_rd32(_REG_BASE_+(0x0160<<2)))&BitMask_08)
#define POST_YGGAINw(val)		_wr32(_REG_BASE_+(0x0160<<2),(UINT)((_rd32(_REG_BASE_+(0x0160<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define POST_YRGAINr			(ValSft_R00(_rd32(_REG_BASE_+(0x0160<<2)))&BitMask_08)
#define POST_YRGAINw(val)		_wr32(_REG_BASE_+(0x0160<<2),(UINT)((_rd32(_REG_BASE_+(0x0160<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define WPRG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0161<<2)))&BitMask_13)
#define WPRG_GAINw(val)			_wr32(_REG_BASE_+(0x0161<<2),(UINT)((_rd32(_REG_BASE_+(0x0161<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPRR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0161<<2)))&BitMask_13)
#define WPRR_GAINw(val)			_wr32(_REG_BASE_+(0x0161<<2),(UINT)((_rd32(_REG_BASE_+(0x0161<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPGR_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0162<<2)))&BitMask_13)
#define WPGR_GAINw(val)			_wr32(_REG_BASE_+(0x0162<<2),(UINT)((_rd32(_REG_BASE_+(0x0162<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPRB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0162<<2)))&BitMask_13)
#define WPRB_GAINw(val)			_wr32(_REG_BASE_+(0x0162<<2),(UINT)((_rd32(_REG_BASE_+(0x0162<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPGB_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0163<<2)))&BitMask_13)
#define WPGB_GAINw(val)			_wr32(_REG_BASE_+(0x0163<<2),(UINT)((_rd32(_REG_BASE_+(0x0163<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPGG_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0163<<2)))&BitMask_13)
#define WPGG_GAINw(val)			_wr32(_REG_BASE_+(0x0163<<2),(UINT)((_rd32(_REG_BASE_+(0x0163<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPBG_GAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x0164<<2)))&BitMask_13)
#define WPBG_GAINw(val)			_wr32(_REG_BASE_+(0x0164<<2),(UINT)((_rd32(_REG_BASE_+(0x0164<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define WPBR_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0164<<2)))&BitMask_13)
#define WPBR_GAINw(val)			_wr32(_REG_BASE_+(0x0164<<2),(UINT)((_rd32(_REG_BASE_+(0x0164<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define WPBB_GAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x0165<<2)))&BitMask_13)
#define WPBB_GAINw(val)			_wr32(_REG_BASE_+(0x0165<<2),(UINT)((_rd32(_REG_BASE_+(0x0165<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define PR_OFSr					(ValSft_R16(_rd32(_REG_BASE_+(0x0166<<2)))&BitMask_11)
#define PR_OFSw(val)			_wr32(_REG_BASE_+(0x0166<<2),(UINT)((_rd32(_REG_BASE_+(0x0166<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define PG_OFSr					(ValSft_R00(_rd32(_REG_BASE_+(0x0166<<2)))&BitMask_11)
#define PG_OFSw(val)			_wr32(_REG_BASE_+(0x0166<<2),(UINT)((_rd32(_REG_BASE_+(0x0166<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define PB_OFSr					(ValSft_R00(_rd32(_REG_BASE_+(0x0167<<2)))&BitMask_11)
#define PB_OFSw(val)			_wr32(_REG_BASE_+(0x0167<<2),(UINT)((_rd32(_REG_BASE_+(0x0167<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define INTP_MODr				(ValSft_R17(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_01)
#define INTP_MODw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define RGB_BYPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_01)
#define RGB_BYPw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define HSUB_SELr				(ValSft_R14(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_02)
#define HSUB_SELw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define VSUB_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_02)
#define VSUB_SELw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define HVSUB_SELr				(ValSft_R10(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_02)
#define HVSUB_SELw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define DSUB_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_02)
#define DSUB_SELw(val)			_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define INTP_DIR_THr			(ValSft_R00(_rd32(_REG_BASE_+(0x0168<<2)))&BitMask_08)
#define INTP_DIR_THw(val)		_wr32(_REG_BASE_+(0x0168<<2),(UINT)((_rd32(_REG_BASE_+(0x0168<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define RMYGr					(ValSft_R16(_rd32(_REG_BASE_+(0x0169<<2)))&BitMask_10)
#define RMYGw(val)				_wr32(_REG_BASE_+(0x0169<<2),(UINT)((_rd32(_REG_BASE_+(0x0169<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define RMYRr					(ValSft_R00(_rd32(_REG_BASE_+(0x0169<<2)))&BitMask_10)
#define RMYRw(val)				_wr32(_REG_BASE_+(0x0169<<2),(UINT)((_rd32(_REG_BASE_+(0x0169<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BMYRr					(ValSft_R16(_rd32(_REG_BASE_+(0x016a<<2)))&BitMask_10)
#define BMYRw(val)				_wr32(_REG_BASE_+(0x016a<<2),(UINT)((_rd32(_REG_BASE_+(0x016a<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define RMYBr					(ValSft_R00(_rd32(_REG_BASE_+(0x016a<<2)))&BitMask_10)
#define RMYBw(val)				_wr32(_REG_BASE_+(0x016a<<2),(UINT)((_rd32(_REG_BASE_+(0x016a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BMYBr					(ValSft_R16(_rd32(_REG_BASE_+(0x016b<<2)))&BitMask_10)
#define BMYBw(val)				_wr32(_REG_BASE_+(0x016b<<2),(UINT)((_rd32(_REG_BASE_+(0x016b<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define BMYGr					(ValSft_R00(_rd32(_REG_BASE_+(0x016b<<2)))&BitMask_10)
#define BMYGw(val)				_wr32(_REG_BASE_+(0x016b<<2),(UINT)((_rd32(_REG_BASE_+(0x016b<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define BYGAIN_LUr				(ValSft_R24(_rd32(_REG_BASE_+(0x016c<<2)))&BitMask_08)
#define BYGAIN_LUw(val)			_wr32(_REG_BASE_+(0x016c<<2),(UINT)((_rd32(_REG_BASE_+(0x016c<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define BYGAIN_LDr				(ValSft_R16(_rd32(_REG_BASE_+(0x016c<<2)))&BitMask_08)
#define BYGAIN_LDw(val)			_wr32(_REG_BASE_+(0x016c<<2),(UINT)((_rd32(_REG_BASE_+(0x016c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define BYGAIN_RUr				(ValSft_R08(_rd32(_REG_BASE_+(0x016c<<2)))&BitMask_08)
#define BYGAIN_RUw(val)			_wr32(_REG_BASE_+(0x016c<<2),(UINT)((_rd32(_REG_BASE_+(0x016c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define BYGAIN_RDr				(ValSft_R00(_rd32(_REG_BASE_+(0x016c<<2)))&BitMask_08)
#define BYGAIN_RDw(val)			_wr32(_REG_BASE_+(0x016c<<2),(UINT)((_rd32(_REG_BASE_+(0x016c<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define RYGAIN_LUr				(ValSft_R24(_rd32(_REG_BASE_+(0x016d<<2)))&BitMask_08)
#define RYGAIN_LUw(val)			_wr32(_REG_BASE_+(0x016d<<2),(UINT)((_rd32(_REG_BASE_+(0x016d<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define RYGAIN_LDr				(ValSft_R16(_rd32(_REG_BASE_+(0x016d<<2)))&BitMask_08)
#define RYGAIN_LDw(val)			_wr32(_REG_BASE_+(0x016d<<2),(UINT)((_rd32(_REG_BASE_+(0x016d<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define RYGAIN_RUr				(ValSft_R08(_rd32(_REG_BASE_+(0x016d<<2)))&BitMask_08)
#define RYGAIN_RUw(val)			_wr32(_REG_BASE_+(0x016d<<2),(UINT)((_rd32(_REG_BASE_+(0x016d<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define RYGAIN_RDr				(ValSft_R00(_rd32(_REG_BASE_+(0x016d<<2)))&BitMask_08)
#define RYGAIN_RDw(val)			_wr32(_REG_BASE_+(0x016d<<2),(UINT)((_rd32(_REG_BASE_+(0x016d<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define BYHUE_LUr				(ValSft_R24(_rd32(_REG_BASE_+(0x016e<<2)))&BitMask_08)
#define BYHUE_LUw(val)			_wr32(_REG_BASE_+(0x016e<<2),(UINT)((_rd32(_REG_BASE_+(0x016e<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define BYHUE_LDr				(ValSft_R16(_rd32(_REG_BASE_+(0x016e<<2)))&BitMask_08)
#define BYHUE_LDw(val)			_wr32(_REG_BASE_+(0x016e<<2),(UINT)((_rd32(_REG_BASE_+(0x016e<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define BYHUE_RUr				(ValSft_R08(_rd32(_REG_BASE_+(0x016e<<2)))&BitMask_08)
#define BYHUE_RUw(val)			_wr32(_REG_BASE_+(0x016e<<2),(UINT)((_rd32(_REG_BASE_+(0x016e<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define BYHUE_RDr				(ValSft_R00(_rd32(_REG_BASE_+(0x016e<<2)))&BitMask_08)
#define BYHUE_RDw(val)			_wr32(_REG_BASE_+(0x016e<<2),(UINT)((_rd32(_REG_BASE_+(0x016e<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define RYHUE_LUr				(ValSft_R24(_rd32(_REG_BASE_+(0x016f<<2)))&BitMask_08)
#define RYHUE_LUw(val)			_wr32(_REG_BASE_+(0x016f<<2),(UINT)((_rd32(_REG_BASE_+(0x016f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define RYHUE_LDr				(ValSft_R16(_rd32(_REG_BASE_+(0x016f<<2)))&BitMask_08)
#define RYHUE_LDw(val)			_wr32(_REG_BASE_+(0x016f<<2),(UINT)((_rd32(_REG_BASE_+(0x016f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define RYHUE_RUr				(ValSft_R08(_rd32(_REG_BASE_+(0x016f<<2)))&BitMask_08)
#define RYHUE_RUw(val)			_wr32(_REG_BASE_+(0x016f<<2),(UINT)((_rd32(_REG_BASE_+(0x016f<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define RYHUE_RDr				(ValSft_R00(_rd32(_REG_BASE_+(0x016f<<2)))&BitMask_08)
#define RYHUE_RDw(val)			_wr32(_REG_BASE_+(0x016f<<2),(UINT)((_rd32(_REG_BASE_+(0x016f<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define HSUP_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_01)
#define HSUP_ONw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HSUP_GAr				(ValSft_R24(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_06)
#define HSUP_GAw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define HSUP_THr				(ValSft_R16(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_08)
#define HSUP_THw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define CES_DLYr				(ValSft_R08(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_04)
#define CES_DLYw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define CVLPF_SELr				(ValSft_R07(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_01)
#define CVLPF_SELw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define CVLPF_ONr				(ValSft_R06(_rd32(_REG_BASE_+(0x0170<<2)))&BitMask_01)
#define CVLPF_ONw(val)			_wr32(_REG_BASE_+(0x0170<<2),(UINT)((_rd32(_REG_BASE_+(0x0170<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define CHLPF_SELr				(ValSft_R05(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_01)
#define CHLPF_SELw(val)			_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define CHLPF_ONr				(ValSft_R04(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_01)
#define CHLPF_ONw(val)			_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define CHLPF_CINVr				(ValSft_R03(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_01)
#define CHLPF_CINVw(val)		_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define NEGA_ONr				(ValSft_R01(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_01)
#define NEGA_ONw(val)			_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define BW_ONr					(ValSft_R00(_rd32(_REG_BASE_+(0x0171<<2)))&BitMask_01)
#define BW_ONw(val)				_wr32(_REG_BASE_+(0x0171<<2),(UINT)((_rd32(_REG_BASE_+(0x0171<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define LSUP_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_01)
#define LSUP_ONw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LSUP_GAr				(ValSft_R25(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_06)
#define LSUP_GAw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L25(BitMask_06)))|ValSft_L25((val)&BitMask_06)))

#define LSUP_THr				(ValSft_R17(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_08)
#define LSUP_THw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L17(BitMask_08)))|ValSft_L17((val)&BitMask_08)))

#define CES_ONr					(ValSft_R16(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_01)
#define CES_ONw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define LCES_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_01)
#define LCES_ONw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define HCES_ONr				(ValSft_R14(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_01)
#define HCES_ONw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define CES_GAINr				(ValSft_R08(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_06)
#define CES_GAINw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L08(BitMask_06)))|ValSft_L08((val)&BitMask_06)))

#define CES_LCLIPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0172<<2)))&BitMask_08)
#define CES_LCLIPw(val)			_wr32(_REG_BASE_+(0x0172<<2),(UINT)((_rd32(_REG_BASE_+(0x0172<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define HEQ_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0174<<2)))&BitMask_01)
#define HEQ_ONw(val)			_wr32(_REG_BASE_+(0x0174<<2),(UINT)((_rd32(_REG_BASE_+(0x0174<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HEQ_OSDr				(ValSft_R30(_rd32(_REG_BASE_+(0x0174<<2)))&BitMask_01)
#define HEQ_OSDw(val)			_wr32(_REG_BASE_+(0x0174<<2),(UINT)((_rd32(_REG_BASE_+(0x0174<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HEQ_WGr					(ValSft_R16(_rd32(_REG_BASE_+(0x0174<<2)))&BitMask_09)
#define HEQ_WGw(val)			_wr32(_REG_BASE_+(0x0174<<2),(UINT)((_rd32(_REG_BASE_+(0x0174<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define HEQ_THr					(ValSft_R00(_rd32(_REG_BASE_+(0x0174<<2)))&BitMask_15)
#define HEQ_THw(val)			_wr32(_REG_BASE_+(0x0174<<2),(UINT)((_rd32(_REG_BASE_+(0x0174<<2))&(~ValSft_L00(BitMask_15)))|ValSft_L00((val)&BitMask_15)))

#define HEQ_HWr					(ValSft_R28(_rd32(_REG_BASE_+(0x0175<<2)))&BitMask_04)
#define HEQ_HWw(val)			_wr32(_REG_BASE_+(0x0175<<2),(UINT)((_rd32(_REG_BASE_+(0x0175<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define HEQ_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0175<<2)))&BitMask_12)
#define HEQ_HSPw(val)			_wr32(_REG_BASE_+(0x0175<<2),(UINT)((_rd32(_REG_BASE_+(0x0175<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HEQ_VWr					(ValSft_R12(_rd32(_REG_BASE_+(0x0175<<2)))&BitMask_04)
#define HEQ_VWw(val)			_wr32(_REG_BASE_+(0x0175<<2),(UINT)((_rd32(_REG_BASE_+(0x0175<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define HEQ_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0175<<2)))&BitMask_12)
#define HEQ_VSPw(val)			_wr32(_REG_BASE_+(0x0175<<2),(UINT)((_rd32(_REG_BASE_+(0x0175<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define HEQ_IIRKr				(ValSft_R00(_rd32(_REG_BASE_+(0x0176<<2)))&BitMask_08)
#define HEQ_IIRKw(val)			_wr32(_REG_BASE_+(0x0176<<2),(UINT)((_rd32(_REG_BASE_+(0x0176<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define FLP_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_01)
#define FLP_ONw(val)			_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define OCFSEL_INVr				(ValSft_R30(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_01)
#define OCFSEL_INVw(val)		_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define FLP_OFSr				(ValSft_R24(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_05)
#define FLP_OFSw(val)			_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define STLL_ONr				(ValSft_R23(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_01)
#define STLL_ONw(val)			_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define YCBCR2YC_CINVr			(ValSft_R11(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_01)
#define YCBCR2YC_CINVw(val)		_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define YC2YCBCR_CINVr			(ValSft_R10(_rd32(_REG_BASE_+(0x0178<<2)))&BitMask_01)
#define YC2YCBCR_CINVw(val)		_wr32(_REG_BASE_+(0x0178<<2),(UINT)((_rd32(_REG_BASE_+(0x0178<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define DCBAR_ON0r				(ValSft_R31(_rd32(_REG_BASE_+(0x0179<<2)))&BitMask_01)
#define DCBAR_ON0w(val)			_wr32(_REG_BASE_+(0x0179<<2),(UINT)((_rd32(_REG_BASE_+(0x0179<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DCBAR_ST0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0179<<2)))&BitMask_12)
#define DCBAR_ST0w(val)			_wr32(_REG_BASE_+(0x0179<<2),(UINT)((_rd32(_REG_BASE_+(0x0179<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DCBAR_HW0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0179<<2)))&BitMask_11)
#define DCBAR_HW0w(val)			_wr32(_REG_BASE_+(0x0179<<2),(UINT)((_rd32(_REG_BASE_+(0x0179<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_01)
#define APT_ONw(val)			_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_GAIN_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_01)
#define APT_GAIN_ONw(val)		_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_FIL_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_02)
#define APT_FIL_SELw(val)		_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define APT_TESTr				(ValSft_R16(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_02)
#define APT_TESTw(val)			_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define APT_TH3r				(ValSft_R00(_rd32(_REG_BASE_+(0x017a<<2)))&BitMask_10)
#define APT_TH3w(val)			_wr32(_REG_BASE_+(0x017a<<2),(UINT)((_rd32(_REG_BASE_+(0x017a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_TH1r				(ValSft_R16(_rd32(_REG_BASE_+(0x017b<<2)))&BitMask_16)
#define APT_TH1w(val)			_wr32(_REG_BASE_+(0x017b<<2),(UINT)((_rd32(_REG_BASE_+(0x017b<<2))&(~ValSft_L16(BitMask_16)))|ValSft_L16((val)&BitMask_16)))

#define APT_TH2r				(ValSft_R00(_rd32(_REG_BASE_+(0x017b<<2)))&BitMask_16)
#define APT_TH2w(val)			_wr32(_REG_BASE_+(0x017b<<2),(UINT)((_rd32(_REG_BASE_+(0x017b<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define APT_GAIN1_POSr			(ValSft_R24(_rd32(_REG_BASE_+(0x017c<<2)))&BitMask_08)
#define APT_GAIN1_POSw(val)		_wr32(_REG_BASE_+(0x017c<<2),(UINT)((_rd32(_REG_BASE_+(0x017c<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_GAIN2_POSr			(ValSft_R16(_rd32(_REG_BASE_+(0x017c<<2)))&BitMask_08)
#define APT_GAIN2_POSw(val)		_wr32(_REG_BASE_+(0x017c<<2),(UINT)((_rd32(_REG_BASE_+(0x017c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_GAIN1_NEGr			(ValSft_R08(_rd32(_REG_BASE_+(0x017c<<2)))&BitMask_08)
#define APT_GAIN1_NEGw(val)		_wr32(_REG_BASE_+(0x017c<<2),(UINT)((_rd32(_REG_BASE_+(0x017c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_GAIN2_NEGr			(ValSft_R00(_rd32(_REG_BASE_+(0x017c<<2)))&BitMask_08)
#define APT_GAIN2_NEGw(val)		_wr32(_REG_BASE_+(0x017c<<2),(UINT)((_rd32(_REG_BASE_+(0x017c<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_SLIC3r				(ValSft_R16(_rd32(_REG_BASE_+(0x017d<<2)))&BitMask_14)
#define APT_SLIC3w(val)			_wr32(_REG_BASE_+(0x017d<<2),(UINT)((_rd32(_REG_BASE_+(0x017d<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define APT_CLIP3r				(ValSft_R00(_rd32(_REG_BASE_+(0x017d<<2)))&BitMask_10)
#define APT_CLIP3w(val)			_wr32(_REG_BASE_+(0x017d<<2),(UINT)((_rd32(_REG_BASE_+(0x017d<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_GAIN3r				(ValSft_R00(_rd32(_REG_BASE_+(0x017e<<2)))&BitMask_08)
#define APT_GAIN3w(val)			_wr32(_REG_BASE_+(0x017e<<2),(UINT)((_rd32(_REG_BASE_+(0x017e<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_LPFKr				(ValSft_R24(_rd32(_REG_BASE_+(0x017f<<2)))&BitMask_08)
#define APT_LPFKw(val)			_wr32(_REG_BASE_+(0x017f<<2),(UINT)((_rd32(_REG_BASE_+(0x017f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_LPFGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x017f<<2)))&BitMask_08)
#define APT_LPFGAw(val)			_wr32(_REG_BASE_+(0x017f<<2),(UINT)((_rd32(_REG_BASE_+(0x017f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_LPFTHr				(ValSft_R00(_rd32(_REG_BASE_+(0x017f<<2)))&BitMask_10)
#define APT_LPFTHw(val)			_wr32(_REG_BASE_+(0x017f<<2),(UINT)((_rd32(_REG_BASE_+(0x017f<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMA_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0180<<2)))&BitMask_01)
#define YGMA_ONw(val)			_wr32(_REG_BASE_+(0x0180<<2),(UINT)((_rd32(_REG_BASE_+(0x0180<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CGMA_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0180<<2)))&BitMask_01)
#define CGMA_ONw(val)			_wr32(_REG_BASE_+(0x0180<<2),(UINT)((_rd32(_REG_BASE_+(0x0180<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YGMY_00r				(ValSft_R16(_rd32(_REG_BASE_+(0x0180<<2)))&BitMask_10)
#define YGMY_00w(val)			_wr32(_REG_BASE_+(0x0180<<2),(UINT)((_rd32(_REG_BASE_+(0x0180<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_01r				(ValSft_R00(_rd32(_REG_BASE_+(0x0180<<2)))&BitMask_10)
#define YGMY_01w(val)			_wr32(_REG_BASE_+(0x0180<<2),(UINT)((_rd32(_REG_BASE_+(0x0180<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_02r				(ValSft_R16(_rd32(_REG_BASE_+(0x0181<<2)))&BitMask_10)
#define YGMY_02w(val)			_wr32(_REG_BASE_+(0x0181<<2),(UINT)((_rd32(_REG_BASE_+(0x0181<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_03r				(ValSft_R00(_rd32(_REG_BASE_+(0x0181<<2)))&BitMask_10)
#define YGMY_03w(val)			_wr32(_REG_BASE_+(0x0181<<2),(UINT)((_rd32(_REG_BASE_+(0x0181<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_04r				(ValSft_R16(_rd32(_REG_BASE_+(0x0182<<2)))&BitMask_10)
#define YGMY_04w(val)			_wr32(_REG_BASE_+(0x0182<<2),(UINT)((_rd32(_REG_BASE_+(0x0182<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_05r				(ValSft_R00(_rd32(_REG_BASE_+(0x0182<<2)))&BitMask_10)
#define YGMY_05w(val)			_wr32(_REG_BASE_+(0x0182<<2),(UINT)((_rd32(_REG_BASE_+(0x0182<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_06r				(ValSft_R16(_rd32(_REG_BASE_+(0x0183<<2)))&BitMask_10)
#define YGMY_06w(val)			_wr32(_REG_BASE_+(0x0183<<2),(UINT)((_rd32(_REG_BASE_+(0x0183<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_07r				(ValSft_R00(_rd32(_REG_BASE_+(0x0183<<2)))&BitMask_10)
#define YGMY_07w(val)			_wr32(_REG_BASE_+(0x0183<<2),(UINT)((_rd32(_REG_BASE_+(0x0183<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_08r				(ValSft_R16(_rd32(_REG_BASE_+(0x0184<<2)))&BitMask_10)
#define YGMY_08w(val)			_wr32(_REG_BASE_+(0x0184<<2),(UINT)((_rd32(_REG_BASE_+(0x0184<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_09r				(ValSft_R00(_rd32(_REG_BASE_+(0x0184<<2)))&BitMask_10)
#define YGMY_09w(val)			_wr32(_REG_BASE_+(0x0184<<2),(UINT)((_rd32(_REG_BASE_+(0x0184<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_10r				(ValSft_R16(_rd32(_REG_BASE_+(0x0185<<2)))&BitMask_10)
#define YGMY_10w(val)			_wr32(_REG_BASE_+(0x0185<<2),(UINT)((_rd32(_REG_BASE_+(0x0185<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_11r				(ValSft_R00(_rd32(_REG_BASE_+(0x0185<<2)))&BitMask_10)
#define YGMY_11w(val)			_wr32(_REG_BASE_+(0x0185<<2),(UINT)((_rd32(_REG_BASE_+(0x0185<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_12r				(ValSft_R16(_rd32(_REG_BASE_+(0x0186<<2)))&BitMask_10)
#define YGMY_12w(val)			_wr32(_REG_BASE_+(0x0186<<2),(UINT)((_rd32(_REG_BASE_+(0x0186<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_13r				(ValSft_R00(_rd32(_REG_BASE_+(0x0186<<2)))&BitMask_10)
#define YGMY_13w(val)			_wr32(_REG_BASE_+(0x0186<<2),(UINT)((_rd32(_REG_BASE_+(0x0186<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_14r				(ValSft_R16(_rd32(_REG_BASE_+(0x0187<<2)))&BitMask_10)
#define YGMY_14w(val)			_wr32(_REG_BASE_+(0x0187<<2),(UINT)((_rd32(_REG_BASE_+(0x0187<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define YGMY_15r				(ValSft_R00(_rd32(_REG_BASE_+(0x0187<<2)))&BitMask_10)
#define YGMY_15w(val)			_wr32(_REG_BASE_+(0x0187<<2),(UINT)((_rd32(_REG_BASE_+(0x0187<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YGMY_16r				(ValSft_R16(_rd32(_REG_BASE_+(0x0188<<2)))&BitMask_10)
#define YGMY_16w(val)			_wr32(_REG_BASE_+(0x0188<<2),(UINT)((_rd32(_REG_BASE_+(0x0188<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_00r				(ValSft_R16(_rd32(_REG_BASE_+(0x0189<<2)))&BitMask_10)
#define CGMY_00w(val)			_wr32(_REG_BASE_+(0x0189<<2),(UINT)((_rd32(_REG_BASE_+(0x0189<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_01r				(ValSft_R00(_rd32(_REG_BASE_+(0x0189<<2)))&BitMask_10)
#define CGMY_01w(val)			_wr32(_REG_BASE_+(0x0189<<2),(UINT)((_rd32(_REG_BASE_+(0x0189<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_02r				(ValSft_R16(_rd32(_REG_BASE_+(0x018a<<2)))&BitMask_10)
#define CGMY_02w(val)			_wr32(_REG_BASE_+(0x018a<<2),(UINT)((_rd32(_REG_BASE_+(0x018a<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_03r				(ValSft_R00(_rd32(_REG_BASE_+(0x018a<<2)))&BitMask_10)
#define CGMY_03w(val)			_wr32(_REG_BASE_+(0x018a<<2),(UINT)((_rd32(_REG_BASE_+(0x018a<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_04r				(ValSft_R16(_rd32(_REG_BASE_+(0x018b<<2)))&BitMask_10)
#define CGMY_04w(val)			_wr32(_REG_BASE_+(0x018b<<2),(UINT)((_rd32(_REG_BASE_+(0x018b<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_05r				(ValSft_R00(_rd32(_REG_BASE_+(0x018b<<2)))&BitMask_10)
#define CGMY_05w(val)			_wr32(_REG_BASE_+(0x018b<<2),(UINT)((_rd32(_REG_BASE_+(0x018b<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_06r				(ValSft_R16(_rd32(_REG_BASE_+(0x018c<<2)))&BitMask_10)
#define CGMY_06w(val)			_wr32(_REG_BASE_+(0x018c<<2),(UINT)((_rd32(_REG_BASE_+(0x018c<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_07r				(ValSft_R00(_rd32(_REG_BASE_+(0x018c<<2)))&BitMask_10)
#define CGMY_07w(val)			_wr32(_REG_BASE_+(0x018c<<2),(UINT)((_rd32(_REG_BASE_+(0x018c<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_08r				(ValSft_R16(_rd32(_REG_BASE_+(0x018d<<2)))&BitMask_10)
#define CGMY_08w(val)			_wr32(_REG_BASE_+(0x018d<<2),(UINT)((_rd32(_REG_BASE_+(0x018d<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_09r				(ValSft_R00(_rd32(_REG_BASE_+(0x018d<<2)))&BitMask_10)
#define CGMY_09w(val)			_wr32(_REG_BASE_+(0x018d<<2),(UINT)((_rd32(_REG_BASE_+(0x018d<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_10r				(ValSft_R16(_rd32(_REG_BASE_+(0x018e<<2)))&BitMask_10)
#define CGMY_10w(val)			_wr32(_REG_BASE_+(0x018e<<2),(UINT)((_rd32(_REG_BASE_+(0x018e<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_11r				(ValSft_R00(_rd32(_REG_BASE_+(0x018e<<2)))&BitMask_10)
#define CGMY_11w(val)			_wr32(_REG_BASE_+(0x018e<<2),(UINT)((_rd32(_REG_BASE_+(0x018e<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_12r				(ValSft_R16(_rd32(_REG_BASE_+(0x018f<<2)))&BitMask_10)
#define CGMY_12w(val)			_wr32(_REG_BASE_+(0x018f<<2),(UINT)((_rd32(_REG_BASE_+(0x018f<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_13r				(ValSft_R00(_rd32(_REG_BASE_+(0x018f<<2)))&BitMask_10)
#define CGMY_13w(val)			_wr32(_REG_BASE_+(0x018f<<2),(UINT)((_rd32(_REG_BASE_+(0x018f<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_14r				(ValSft_R16(_rd32(_REG_BASE_+(0x0190<<2)))&BitMask_10)
#define CGMY_14w(val)			_wr32(_REG_BASE_+(0x0190<<2),(UINT)((_rd32(_REG_BASE_+(0x0190<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CGMY_15r				(ValSft_R00(_rd32(_REG_BASE_+(0x0190<<2)))&BitMask_10)
#define CGMY_15w(val)			_wr32(_REG_BASE_+(0x0190<<2),(UINT)((_rd32(_REG_BASE_+(0x0190<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CGMY_16r				(ValSft_R16(_rd32(_REG_BASE_+(0x0191<<2)))&BitMask_10)
#define CGMY_16w(val)			_wr32(_REG_BASE_+(0x0191<<2),(UINT)((_rd32(_REG_BASE_+(0x0191<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define EDGE_OSD_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0193<<2)))&BitMask_01)
#define EDGE_OSD_ONw(val)		_wr32(_REG_BASE_+(0x0193<<2),(UINT)((_rd32(_REG_BASE_+(0x0193<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define EDGE_THr				(ValSft_R00(_rd32(_REG_BASE_+(0x0193<<2)))&BitMask_16)
#define EDGE_THw(val)			_wr32(_REG_BASE_+(0x0193<<2),(UINT)((_rd32(_REG_BASE_+(0x0193<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define EDGE_YLVr				(ValSft_R20(_rd32(_REG_BASE_+(0x0194<<2)))&BitMask_10)
#define EDGE_YLVw(val)			_wr32(_REG_BASE_+(0x0194<<2),(UINT)((_rd32(_REG_BASE_+(0x0194<<2))&(~ValSft_L20(BitMask_10)))|ValSft_L20((val)&BitMask_10)))

#define EDGE_CBLVr				(ValSft_R10(_rd32(_REG_BASE_+(0x0194<<2)))&BitMask_10)
#define EDGE_CBLVw(val)			_wr32(_REG_BASE_+(0x0194<<2),(UINT)((_rd32(_REG_BASE_+(0x0194<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define EDGE_CRLVr				(ValSft_R00(_rd32(_REG_BASE_+(0x0194<<2)))&BitMask_10)
#define EDGE_CRLVw(val)			_wr32(_REG_BASE_+(0x0194<<2),(UINT)((_rd32(_REG_BASE_+(0x0194<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define FONT_ON0r				(ValSft_R31(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_01)
#define FONT_ON0w(val)			_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BOAD_ON0r				(ValSft_R30(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_01)
#define BOAD_ON0w(val)			_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BOAD_MODE0r				(ValSft_R29(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_01)
#define BOAD_MODE0w(val)		_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define FONT_MUTE0r				(ValSft_R28(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_01)
#define FONT_MUTE0w(val)		_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define FONT_OFY0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_12)
#define FONT_OFY0w(val)			_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CHAR_WMOD0r				(ValSft_R15(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_01)
#define CHAR_WMOD0w(val)		_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define FONT_OFX0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0196<<2)))&BitMask_12)
#define FONT_OFX0w(val)			_wr32(_REG_BASE_+(0x0196<<2),(UINT)((_rd32(_REG_BASE_+(0x0196<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define FONT_LV0_0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0197<<2)))&BitMask_24)
#define FONT_LV0_0w(val)		_wr32(_REG_BASE_+(0x0197<<2),(UINT)((_rd32(_REG_BASE_+(0x0197<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV1_0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0198<<2)))&BitMask_24)
#define FONT_LV1_0w(val)		_wr32(_REG_BASE_+(0x0198<<2),(UINT)((_rd32(_REG_BASE_+(0x0198<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV2_0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0199<<2)))&BitMask_24)
#define FONT_LV2_0w(val)		_wr32(_REG_BASE_+(0x0199<<2),(UINT)((_rd32(_REG_BASE_+(0x0199<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_LV3_0r				(ValSft_R00(_rd32(_REG_BASE_+(0x019a<<2)))&BitMask_24)
#define FONT_LV3_0w(val)		_wr32(_REG_BASE_+(0x019a<<2),(UINT)((_rd32(_REG_BASE_+(0x019a<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOAD_LV0r				(ValSft_R00(_rd32(_REG_BASE_+(0x019b<<2)))&BitMask_24)
#define BOAD_LV0w(val)			_wr32(_REG_BASE_+(0x019b<<2),(UINT)((_rd32(_REG_BASE_+(0x019b<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define FONT_VBLK0r				(ValSft_R16(_rd32(_REG_BASE_+(0x019c<<2)))&BitMask_08)
#define FONT_VBLK0w(val)		_wr32(_REG_BASE_+(0x019c<<2),(UINT)((_rd32(_REG_BASE_+(0x019c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define FONT_HBLK0r				(ValSft_R08(_rd32(_REG_BASE_+(0x019c<<2)))&BitMask_08)
#define FONT_HBLK0w(val)		_wr32(_REG_BASE_+(0x019c<<2),(UINT)((_rd32(_REG_BASE_+(0x019c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define FONT_VSIZ0r				(ValSft_R04(_rd32(_REG_BASE_+(0x019c<<2)))&BitMask_04)
#define FONT_VSIZ0w(val)		_wr32(_REG_BASE_+(0x019c<<2),(UINT)((_rd32(_REG_BASE_+(0x019c<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define FONT_HSIZ0r				(ValSft_R00(_rd32(_REG_BASE_+(0x019c<<2)))&BitMask_04)
#define FONT_HSIZ0w(val)		_wr32(_REG_BASE_+(0x019c<<2),(UINT)((_rd32(_REG_BASE_+(0x019c<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define FONT_HW0r				(ValSft_R00(_rd32(_REG_BASE_+(0x019d<<2)))&BitMask_12)
#define FONT_HW0w(val)			_wr32(_REG_BASE_+(0x019d<<2),(UINT)((_rd32(_REG_BASE_+(0x019d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define OSD_MUTE0r				(ValSft_R31(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_01)
#define OSD_MUTE0w(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define OSD_MUTE1r				(ValSft_R30(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_01)
#define OSD_MUTE1w(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define OSD_ISEL0r				(ValSft_R24(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_04)
#define OSD_ISEL0w(val)			_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define OSD_IVSEL0r				(ValSft_R20(_rd32(_REG_BASE_+(0x01a0<<2)))&BitMask_04)
#define OSD_IVSEL0w(val)		_wr32(_REG_BASE_+(0x01a0<<2),(UINT)((_rd32(_REG_BASE_+(0x01a0<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define BOX_HSP0r				(ValSft_R16(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_13)
#define BOX_HSP0w(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define BOX_VSP0r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a1<<2)))&BitMask_12)
#define BOX_VSP0w(val)			_wr32(_REG_BASE_+(0x01a1<<2),(UINT)((_rd32(_REG_BASE_+(0x01a1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define BOSD_ON0r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a2<<2)))&BitMask_32)
#define BOSD_ON0w(val)			_wr32(_REG_BASE_+(0x01a2<<2),(UINT)((_rd32(_REG_BASE_+(0x01a2<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define BTONE31r				(ValSft_R30(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE31w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define BTONE30r				(ValSft_R28(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE30w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BTONE29r				(ValSft_R26(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE29w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define BTONE28r				(ValSft_R24(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE28w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BTONE27r				(ValSft_R22(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE27w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define BTONE26r				(ValSft_R20(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE26w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BTONE25r				(ValSft_R18(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE25w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define BTONE24r				(ValSft_R16(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE24w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define BTONE23r				(ValSft_R14(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE23w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define BTONE22r				(ValSft_R12(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE22w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define BTONE21r				(ValSft_R10(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE21w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define BTONE20r				(ValSft_R08(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE20w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define BTONE19r				(ValSft_R06(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE19w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define BTONE18r				(ValSft_R04(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE18w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define BTONE17r				(ValSft_R02(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE17w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L02(BitMask_02)))|ValSft_L02((val)&BitMask_02)))

#define BTONE16r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a3<<2)))&BitMask_02)
#define BTONE16w(val)			_wr32(_REG_BASE_+(0x01a3<<2),(UINT)((_rd32(_REG_BASE_+(0x01a3<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define BTONE15r				(ValSft_R30(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE15w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define BTONE14r				(ValSft_R28(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE14w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define BTONE13r				(ValSft_R26(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE13w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L26(BitMask_02)))|ValSft_L26((val)&BitMask_02)))

#define BTONE12r				(ValSft_R24(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE12w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define BTONE11r				(ValSft_R22(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE11w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L22(BitMask_02)))|ValSft_L22((val)&BitMask_02)))

#define BTONE10r				(ValSft_R20(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE10w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define BTONE9r					(ValSft_R18(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE9w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define BTONE8r					(ValSft_R16(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE8w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define BTONE7r					(ValSft_R14(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE7w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define BTONE6r					(ValSft_R12(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE6w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define BTONE5r					(ValSft_R10(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE5w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define BTONE4r					(ValSft_R08(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE4w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define BTONE3r					(ValSft_R06(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE3w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define BTONE2r					(ValSft_R04(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE2w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define BTONE1r					(ValSft_R02(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE1w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L02(BitMask_02)))|ValSft_L02((val)&BitMask_02)))

#define BTONE0r					(ValSft_R00(_rd32(_REG_BASE_+(0x01a4<<2)))&BitMask_02)
#define BTONE0w(val)			_wr32(_REG_BASE_+(0x01a4<<2),(UINT)((_rd32(_REG_BASE_+(0x01a4<<2))&(~ValSft_L00(BitMask_02)))|ValSft_L00((val)&BitMask_02)))

#define BFL_ON31r				(ValSft_R31(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON31w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define BFL_ON30r				(ValSft_R30(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON30w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define BFL_ON29r				(ValSft_R29(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON29w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define BFL_ON28r				(ValSft_R28(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON28w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BFL_ON27r				(ValSft_R27(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON27w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define BFL_ON26r				(ValSft_R26(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON26w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define BFL_ON25r				(ValSft_R25(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON25w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define BFL_ON24r				(ValSft_R24(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON24w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BFL_ON23r				(ValSft_R23(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON23w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define BFL_ON22r				(ValSft_R22(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON22w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define BFL_ON21r				(ValSft_R21(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON21w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define BFL_ON20r				(ValSft_R20(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON20w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define BFL_ON19r				(ValSft_R19(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON19w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define BFL_ON18r				(ValSft_R18(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON18w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define BFL_ON17r				(ValSft_R17(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON17w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define BFL_ON16r				(ValSft_R16(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON16w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define BFL_ON15r				(ValSft_R15(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON15w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define BFL_ON14r				(ValSft_R14(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON14w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define BFL_ON13r				(ValSft_R13(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON13w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define BFL_ON12r				(ValSft_R12(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON12w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define BFL_ON11r				(ValSft_R11(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON11w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define BFL_ON10r				(ValSft_R10(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON10w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define BFL_ON9r				(ValSft_R09(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON9w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define BFL_ON8r				(ValSft_R08(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON8w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define BFL_ON7r				(ValSft_R07(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON7w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define BFL_ON6r				(ValSft_R06(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON6w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define BFL_ON5r				(ValSft_R05(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON5w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define BFL_ON4r				(ValSft_R04(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON4w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define BFL_ON3r				(ValSft_R03(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON3w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define BFL_ON2r				(ValSft_R02(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON2w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define BFL_ON1r				(ValSft_R01(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON1w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define BFL_ON0r				(ValSft_R00(_rd32(_REG_BASE_+(0x01a5<<2)))&BitMask_01)
#define BFL_ON0w(val)			_wr32(_REG_BASE_+(0x01a5<<2),(UINT)((_rd32(_REG_BASE_+(0x01a5<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define B0_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01a6<<2)))&BitMask_12)
#define B0_VSPw(val)			_wr32(_REG_BASE_+(0x01a6<<2),(UINT)((_rd32(_REG_BASE_+(0x01a6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B0_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01a6<<2)))&BitMask_12)
#define B0_VEDw(val)			_wr32(_REG_BASE_+(0x01a6<<2),(UINT)((_rd32(_REG_BASE_+(0x01a6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B0_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_13)
#define B0_HSPw(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B0_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01a7<<2)))&BitMask_13)
#define B0_HEDw(val)			_wr32(_REG_BASE_+(0x01a7<<2),(UINT)((_rd32(_REG_BASE_+(0x01a7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B1_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_12)
#define B1_VSPw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B1_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01a8<<2)))&BitMask_12)
#define B1_VEDw(val)			_wr32(_REG_BASE_+(0x01a8<<2),(UINT)((_rd32(_REG_BASE_+(0x01a8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B1_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01a9<<2)))&BitMask_13)
#define B1_HSPw(val)			_wr32(_REG_BASE_+(0x01a9<<2),(UINT)((_rd32(_REG_BASE_+(0x01a9<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B1_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01a9<<2)))&BitMask_13)
#define B1_HEDw(val)			_wr32(_REG_BASE_+(0x01a9<<2),(UINT)((_rd32(_REG_BASE_+(0x01a9<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B2_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_12)
#define B2_VSPw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B2_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01aa<<2)))&BitMask_12)
#define B2_VEDw(val)			_wr32(_REG_BASE_+(0x01aa<<2),(UINT)((_rd32(_REG_BASE_+(0x01aa<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B2_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ab<<2)))&BitMask_13)
#define B2_HSPw(val)			_wr32(_REG_BASE_+(0x01ab<<2),(UINT)((_rd32(_REG_BASE_+(0x01ab<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B2_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ab<<2)))&BitMask_13)
#define B2_HEDw(val)			_wr32(_REG_BASE_+(0x01ab<<2),(UINT)((_rd32(_REG_BASE_+(0x01ab<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B3_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ac<<2)))&BitMask_12)
#define B3_VSPw(val)			_wr32(_REG_BASE_+(0x01ac<<2),(UINT)((_rd32(_REG_BASE_+(0x01ac<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B3_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ac<<2)))&BitMask_12)
#define B3_VEDw(val)			_wr32(_REG_BASE_+(0x01ac<<2),(UINT)((_rd32(_REG_BASE_+(0x01ac<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B3_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ad<<2)))&BitMask_13)
#define B3_HSPw(val)			_wr32(_REG_BASE_+(0x01ad<<2),(UINT)((_rd32(_REG_BASE_+(0x01ad<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B3_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ad<<2)))&BitMask_13)
#define B3_HEDw(val)			_wr32(_REG_BASE_+(0x01ad<<2),(UINT)((_rd32(_REG_BASE_+(0x01ad<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B4_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01ae<<2)))&BitMask_12)
#define B4_VSPw(val)			_wr32(_REG_BASE_+(0x01ae<<2),(UINT)((_rd32(_REG_BASE_+(0x01ae<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B4_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01ae<<2)))&BitMask_12)
#define B4_VEDw(val)			_wr32(_REG_BASE_+(0x01ae<<2),(UINT)((_rd32(_REG_BASE_+(0x01ae<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B4_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01af<<2)))&BitMask_13)
#define B4_HSPw(val)			_wr32(_REG_BASE_+(0x01af<<2),(UINT)((_rd32(_REG_BASE_+(0x01af<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B4_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01af<<2)))&BitMask_13)
#define B4_HEDw(val)			_wr32(_REG_BASE_+(0x01af<<2),(UINT)((_rd32(_REG_BASE_+(0x01af<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B5_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_12)
#define B5_VSPw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B5_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b0<<2)))&BitMask_12)
#define B5_VEDw(val)			_wr32(_REG_BASE_+(0x01b0<<2),(UINT)((_rd32(_REG_BASE_+(0x01b0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B5_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b1<<2)))&BitMask_13)
#define B5_HSPw(val)			_wr32(_REG_BASE_+(0x01b1<<2),(UINT)((_rd32(_REG_BASE_+(0x01b1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B5_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b1<<2)))&BitMask_13)
#define B5_HEDw(val)			_wr32(_REG_BASE_+(0x01b1<<2),(UINT)((_rd32(_REG_BASE_+(0x01b1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B6_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_12)
#define B6_VSPw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B6_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b2<<2)))&BitMask_12)
#define B6_VEDw(val)			_wr32(_REG_BASE_+(0x01b2<<2),(UINT)((_rd32(_REG_BASE_+(0x01b2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B6_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b3<<2)))&BitMask_13)
#define B6_HSPw(val)			_wr32(_REG_BASE_+(0x01b3<<2),(UINT)((_rd32(_REG_BASE_+(0x01b3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B6_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b3<<2)))&BitMask_13)
#define B6_HEDw(val)			_wr32(_REG_BASE_+(0x01b3<<2),(UINT)((_rd32(_REG_BASE_+(0x01b3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B7_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b4<<2)))&BitMask_12)
#define B7_VSPw(val)			_wr32(_REG_BASE_+(0x01b4<<2),(UINT)((_rd32(_REG_BASE_+(0x01b4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B7_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b4<<2)))&BitMask_12)
#define B7_VEDw(val)			_wr32(_REG_BASE_+(0x01b4<<2),(UINT)((_rd32(_REG_BASE_+(0x01b4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B7_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_13)
#define B7_HSPw(val)			_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B7_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b5<<2)))&BitMask_13)
#define B7_HEDw(val)			_wr32(_REG_BASE_+(0x01b5<<2),(UINT)((_rd32(_REG_BASE_+(0x01b5<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B8_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_12)
#define B8_VSPw(val)			_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B8_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b6<<2)))&BitMask_12)
#define B8_VEDw(val)			_wr32(_REG_BASE_+(0x01b6<<2),(UINT)((_rd32(_REG_BASE_+(0x01b6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B8_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b7<<2)))&BitMask_13)
#define B8_HSPw(val)			_wr32(_REG_BASE_+(0x01b7<<2),(UINT)((_rd32(_REG_BASE_+(0x01b7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B8_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b7<<2)))&BitMask_13)
#define B8_HEDw(val)			_wr32(_REG_BASE_+(0x01b7<<2),(UINT)((_rd32(_REG_BASE_+(0x01b7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B9_VSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b8<<2)))&BitMask_12)
#define B9_VSPw(val)			_wr32(_REG_BASE_+(0x01b8<<2),(UINT)((_rd32(_REG_BASE_+(0x01b8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B9_VEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b8<<2)))&BitMask_12)
#define B9_VEDw(val)			_wr32(_REG_BASE_+(0x01b8<<2),(UINT)((_rd32(_REG_BASE_+(0x01b8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B9_HSPr					(ValSft_R16(_rd32(_REG_BASE_+(0x01b9<<2)))&BitMask_13)
#define B9_HSPw(val)			_wr32(_REG_BASE_+(0x01b9<<2),(UINT)((_rd32(_REG_BASE_+(0x01b9<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B9_HEDr					(ValSft_R00(_rd32(_REG_BASE_+(0x01b9<<2)))&BitMask_13)
#define B9_HEDw(val)			_wr32(_REG_BASE_+(0x01b9<<2),(UINT)((_rd32(_REG_BASE_+(0x01b9<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B10_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_12)
#define B10_VSPw(val)			_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B10_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ba<<2)))&BitMask_12)
#define B10_VEDw(val)			_wr32(_REG_BASE_+(0x01ba<<2),(UINT)((_rd32(_REG_BASE_+(0x01ba<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B10_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01bb<<2)))&BitMask_13)
#define B10_HSPw(val)			_wr32(_REG_BASE_+(0x01bb<<2),(UINT)((_rd32(_REG_BASE_+(0x01bb<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B10_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bb<<2)))&BitMask_13)
#define B10_HEDw(val)			_wr32(_REG_BASE_+(0x01bb<<2),(UINT)((_rd32(_REG_BASE_+(0x01bb<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B11_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01bc<<2)))&BitMask_12)
#define B11_VSPw(val)			_wr32(_REG_BASE_+(0x01bc<<2),(UINT)((_rd32(_REG_BASE_+(0x01bc<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B11_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bc<<2)))&BitMask_12)
#define B11_VEDw(val)			_wr32(_REG_BASE_+(0x01bc<<2),(UINT)((_rd32(_REG_BASE_+(0x01bc<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B11_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01bd<<2)))&BitMask_13)
#define B11_HSPw(val)			_wr32(_REG_BASE_+(0x01bd<<2),(UINT)((_rd32(_REG_BASE_+(0x01bd<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B11_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bd<<2)))&BitMask_13)
#define B11_HEDw(val)			_wr32(_REG_BASE_+(0x01bd<<2),(UINT)((_rd32(_REG_BASE_+(0x01bd<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B12_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01be<<2)))&BitMask_12)
#define B12_VSPw(val)			_wr32(_REG_BASE_+(0x01be<<2),(UINT)((_rd32(_REG_BASE_+(0x01be<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B12_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01be<<2)))&BitMask_12)
#define B12_VEDw(val)			_wr32(_REG_BASE_+(0x01be<<2),(UINT)((_rd32(_REG_BASE_+(0x01be<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B12_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01bf<<2)))&BitMask_13)
#define B12_HSPw(val)			_wr32(_REG_BASE_+(0x01bf<<2),(UINT)((_rd32(_REG_BASE_+(0x01bf<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B12_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01bf<<2)))&BitMask_13)
#define B12_HEDw(val)			_wr32(_REG_BASE_+(0x01bf<<2),(UINT)((_rd32(_REG_BASE_+(0x01bf<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B13_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c0<<2)))&BitMask_12)
#define B13_VSPw(val)			_wr32(_REG_BASE_+(0x01c0<<2),(UINT)((_rd32(_REG_BASE_+(0x01c0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B13_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c0<<2)))&BitMask_12)
#define B13_VEDw(val)			_wr32(_REG_BASE_+(0x01c0<<2),(UINT)((_rd32(_REG_BASE_+(0x01c0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B13_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_13)
#define B13_HSPw(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B13_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c1<<2)))&BitMask_13)
#define B13_HEDw(val)			_wr32(_REG_BASE_+(0x01c1<<2),(UINT)((_rd32(_REG_BASE_+(0x01c1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B14_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c2<<2)))&BitMask_12)
#define B14_VSPw(val)			_wr32(_REG_BASE_+(0x01c2<<2),(UINT)((_rd32(_REG_BASE_+(0x01c2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B14_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c2<<2)))&BitMask_12)
#define B14_VEDw(val)			_wr32(_REG_BASE_+(0x01c2<<2),(UINT)((_rd32(_REG_BASE_+(0x01c2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B14_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_13)
#define B14_HSPw(val)			_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B14_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c3<<2)))&BitMask_13)
#define B14_HEDw(val)			_wr32(_REG_BASE_+(0x01c3<<2),(UINT)((_rd32(_REG_BASE_+(0x01c3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B15_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_12)
#define B15_VSPw(val)			_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B15_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c4<<2)))&BitMask_12)
#define B15_VEDw(val)			_wr32(_REG_BASE_+(0x01c4<<2),(UINT)((_rd32(_REG_BASE_+(0x01c4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B15_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c5<<2)))&BitMask_13)
#define B15_HSPw(val)			_wr32(_REG_BASE_+(0x01c5<<2),(UINT)((_rd32(_REG_BASE_+(0x01c5<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B15_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c5<<2)))&BitMask_13)
#define B15_HEDw(val)			_wr32(_REG_BASE_+(0x01c5<<2),(UINT)((_rd32(_REG_BASE_+(0x01c5<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B16_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_12)
#define B16_VSPw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B16_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c6<<2)))&BitMask_12)
#define B16_VEDw(val)			_wr32(_REG_BASE_+(0x01c6<<2),(UINT)((_rd32(_REG_BASE_+(0x01c6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B16_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c7<<2)))&BitMask_13)
#define B16_HSPw(val)			_wr32(_REG_BASE_+(0x01c7<<2),(UINT)((_rd32(_REG_BASE_+(0x01c7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B16_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c7<<2)))&BitMask_13)
#define B16_HEDw(val)			_wr32(_REG_BASE_+(0x01c7<<2),(UINT)((_rd32(_REG_BASE_+(0x01c7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B17_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_12)
#define B17_VSPw(val)			_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B17_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c8<<2)))&BitMask_12)
#define B17_VEDw(val)			_wr32(_REG_BASE_+(0x01c8<<2),(UINT)((_rd32(_REG_BASE_+(0x01c8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B17_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_13)
#define B17_HSPw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B17_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01c9<<2)))&BitMask_13)
#define B17_HEDw(val)			_wr32(_REG_BASE_+(0x01c9<<2),(UINT)((_rd32(_REG_BASE_+(0x01c9<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B18_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01ca<<2)))&BitMask_12)
#define B18_VSPw(val)			_wr32(_REG_BASE_+(0x01ca<<2),(UINT)((_rd32(_REG_BASE_+(0x01ca<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B18_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ca<<2)))&BitMask_12)
#define B18_VEDw(val)			_wr32(_REG_BASE_+(0x01ca<<2),(UINT)((_rd32(_REG_BASE_+(0x01ca<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B18_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01cb<<2)))&BitMask_13)
#define B18_HSPw(val)			_wr32(_REG_BASE_+(0x01cb<<2),(UINT)((_rd32(_REG_BASE_+(0x01cb<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B18_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01cb<<2)))&BitMask_13)
#define B18_HEDw(val)			_wr32(_REG_BASE_+(0x01cb<<2),(UINT)((_rd32(_REG_BASE_+(0x01cb<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B19_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01cc<<2)))&BitMask_12)
#define B19_VSPw(val)			_wr32(_REG_BASE_+(0x01cc<<2),(UINT)((_rd32(_REG_BASE_+(0x01cc<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B19_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01cc<<2)))&BitMask_12)
#define B19_VEDw(val)			_wr32(_REG_BASE_+(0x01cc<<2),(UINT)((_rd32(_REG_BASE_+(0x01cc<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B19_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01cd<<2)))&BitMask_13)
#define B19_HSPw(val)			_wr32(_REG_BASE_+(0x01cd<<2),(UINT)((_rd32(_REG_BASE_+(0x01cd<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B19_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01cd<<2)))&BitMask_13)
#define B19_HEDw(val)			_wr32(_REG_BASE_+(0x01cd<<2),(UINT)((_rd32(_REG_BASE_+(0x01cd<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B20_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01ce<<2)))&BitMask_12)
#define B20_VSPw(val)			_wr32(_REG_BASE_+(0x01ce<<2),(UINT)((_rd32(_REG_BASE_+(0x01ce<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B20_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ce<<2)))&BitMask_12)
#define B20_VEDw(val)			_wr32(_REG_BASE_+(0x01ce<<2),(UINT)((_rd32(_REG_BASE_+(0x01ce<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B20_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_13)
#define B20_HSPw(val)			_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B20_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01cf<<2)))&BitMask_13)
#define B20_HEDw(val)			_wr32(_REG_BASE_+(0x01cf<<2),(UINT)((_rd32(_REG_BASE_+(0x01cf<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B21_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d0<<2)))&BitMask_12)
#define B21_VSPw(val)			_wr32(_REG_BASE_+(0x01d0<<2),(UINT)((_rd32(_REG_BASE_+(0x01d0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B21_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d0<<2)))&BitMask_12)
#define B21_VEDw(val)			_wr32(_REG_BASE_+(0x01d0<<2),(UINT)((_rd32(_REG_BASE_+(0x01d0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B21_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d1<<2)))&BitMask_13)
#define B21_HSPw(val)			_wr32(_REG_BASE_+(0x01d1<<2),(UINT)((_rd32(_REG_BASE_+(0x01d1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B21_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d1<<2)))&BitMask_13)
#define B21_HEDw(val)			_wr32(_REG_BASE_+(0x01d1<<2),(UINT)((_rd32(_REG_BASE_+(0x01d1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B22_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d2<<2)))&BitMask_12)
#define B22_VSPw(val)			_wr32(_REG_BASE_+(0x01d2<<2),(UINT)((_rd32(_REG_BASE_+(0x01d2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B22_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d2<<2)))&BitMask_12)
#define B22_VEDw(val)			_wr32(_REG_BASE_+(0x01d2<<2),(UINT)((_rd32(_REG_BASE_+(0x01d2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B22_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d3<<2)))&BitMask_13)
#define B22_HSPw(val)			_wr32(_REG_BASE_+(0x01d3<<2),(UINT)((_rd32(_REG_BASE_+(0x01d3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B22_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d3<<2)))&BitMask_13)
#define B22_HEDw(val)			_wr32(_REG_BASE_+(0x01d3<<2),(UINT)((_rd32(_REG_BASE_+(0x01d3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B23_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d4<<2)))&BitMask_12)
#define B23_VSPw(val)			_wr32(_REG_BASE_+(0x01d4<<2),(UINT)((_rd32(_REG_BASE_+(0x01d4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B23_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d4<<2)))&BitMask_12)
#define B23_VEDw(val)			_wr32(_REG_BASE_+(0x01d4<<2),(UINT)((_rd32(_REG_BASE_+(0x01d4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B23_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d5<<2)))&BitMask_13)
#define B23_HSPw(val)			_wr32(_REG_BASE_+(0x01d5<<2),(UINT)((_rd32(_REG_BASE_+(0x01d5<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B23_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d5<<2)))&BitMask_13)
#define B23_HEDw(val)			_wr32(_REG_BASE_+(0x01d5<<2),(UINT)((_rd32(_REG_BASE_+(0x01d5<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B24_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d6<<2)))&BitMask_12)
#define B24_VSPw(val)			_wr32(_REG_BASE_+(0x01d6<<2),(UINT)((_rd32(_REG_BASE_+(0x01d6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B24_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d6<<2)))&BitMask_12)
#define B24_VEDw(val)			_wr32(_REG_BASE_+(0x01d6<<2),(UINT)((_rd32(_REG_BASE_+(0x01d6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B24_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d7<<2)))&BitMask_13)
#define B24_HSPw(val)			_wr32(_REG_BASE_+(0x01d7<<2),(UINT)((_rd32(_REG_BASE_+(0x01d7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B24_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d7<<2)))&BitMask_13)
#define B24_HEDw(val)			_wr32(_REG_BASE_+(0x01d7<<2),(UINT)((_rd32(_REG_BASE_+(0x01d7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B25_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d8<<2)))&BitMask_12)
#define B25_VSPw(val)			_wr32(_REG_BASE_+(0x01d8<<2),(UINT)((_rd32(_REG_BASE_+(0x01d8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B25_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d8<<2)))&BitMask_12)
#define B25_VEDw(val)			_wr32(_REG_BASE_+(0x01d8<<2),(UINT)((_rd32(_REG_BASE_+(0x01d8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B25_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01d9<<2)))&BitMask_13)
#define B25_HSPw(val)			_wr32(_REG_BASE_+(0x01d9<<2),(UINT)((_rd32(_REG_BASE_+(0x01d9<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B25_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01d9<<2)))&BitMask_13)
#define B25_HEDw(val)			_wr32(_REG_BASE_+(0x01d9<<2),(UINT)((_rd32(_REG_BASE_+(0x01d9<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B26_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01da<<2)))&BitMask_12)
#define B26_VSPw(val)			_wr32(_REG_BASE_+(0x01da<<2),(UINT)((_rd32(_REG_BASE_+(0x01da<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B26_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01da<<2)))&BitMask_12)
#define B26_VEDw(val)			_wr32(_REG_BASE_+(0x01da<<2),(UINT)((_rd32(_REG_BASE_+(0x01da<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B26_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01db<<2)))&BitMask_13)
#define B26_HSPw(val)			_wr32(_REG_BASE_+(0x01db<<2),(UINT)((_rd32(_REG_BASE_+(0x01db<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B26_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01db<<2)))&BitMask_13)
#define B26_HEDw(val)			_wr32(_REG_BASE_+(0x01db<<2),(UINT)((_rd32(_REG_BASE_+(0x01db<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B27_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01dc<<2)))&BitMask_12)
#define B27_VSPw(val)			_wr32(_REG_BASE_+(0x01dc<<2),(UINT)((_rd32(_REG_BASE_+(0x01dc<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B27_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01dc<<2)))&BitMask_12)
#define B27_VEDw(val)			_wr32(_REG_BASE_+(0x01dc<<2),(UINT)((_rd32(_REG_BASE_+(0x01dc<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B27_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01dd<<2)))&BitMask_13)
#define B27_HSPw(val)			_wr32(_REG_BASE_+(0x01dd<<2),(UINT)((_rd32(_REG_BASE_+(0x01dd<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B27_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01dd<<2)))&BitMask_13)
#define B27_HEDw(val)			_wr32(_REG_BASE_+(0x01dd<<2),(UINT)((_rd32(_REG_BASE_+(0x01dd<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B28_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01de<<2)))&BitMask_12)
#define B28_VSPw(val)			_wr32(_REG_BASE_+(0x01de<<2),(UINT)((_rd32(_REG_BASE_+(0x01de<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B28_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01de<<2)))&BitMask_12)
#define B28_VEDw(val)			_wr32(_REG_BASE_+(0x01de<<2),(UINT)((_rd32(_REG_BASE_+(0x01de<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B28_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01df<<2)))&BitMask_13)
#define B28_HSPw(val)			_wr32(_REG_BASE_+(0x01df<<2),(UINT)((_rd32(_REG_BASE_+(0x01df<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B28_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01df<<2)))&BitMask_13)
#define B28_HEDw(val)			_wr32(_REG_BASE_+(0x01df<<2),(UINT)((_rd32(_REG_BASE_+(0x01df<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B29_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e0<<2)))&BitMask_12)
#define B29_VSPw(val)			_wr32(_REG_BASE_+(0x01e0<<2),(UINT)((_rd32(_REG_BASE_+(0x01e0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B29_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e0<<2)))&BitMask_12)
#define B29_VEDw(val)			_wr32(_REG_BASE_+(0x01e0<<2),(UINT)((_rd32(_REG_BASE_+(0x01e0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B29_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e1<<2)))&BitMask_13)
#define B29_HSPw(val)			_wr32(_REG_BASE_+(0x01e1<<2),(UINT)((_rd32(_REG_BASE_+(0x01e1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B29_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e1<<2)))&BitMask_13)
#define B29_HEDw(val)			_wr32(_REG_BASE_+(0x01e1<<2),(UINT)((_rd32(_REG_BASE_+(0x01e1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B30_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e2<<2)))&BitMask_12)
#define B30_VSPw(val)			_wr32(_REG_BASE_+(0x01e2<<2),(UINT)((_rd32(_REG_BASE_+(0x01e2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B30_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e2<<2)))&BitMask_12)
#define B30_VEDw(val)			_wr32(_REG_BASE_+(0x01e2<<2),(UINT)((_rd32(_REG_BASE_+(0x01e2<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B30_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e3<<2)))&BitMask_13)
#define B30_HSPw(val)			_wr32(_REG_BASE_+(0x01e3<<2),(UINT)((_rd32(_REG_BASE_+(0x01e3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B30_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e3<<2)))&BitMask_13)
#define B30_HEDw(val)			_wr32(_REG_BASE_+(0x01e3<<2),(UINT)((_rd32(_REG_BASE_+(0x01e3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define B31_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e4<<2)))&BitMask_12)
#define B31_VSPw(val)			_wr32(_REG_BASE_+(0x01e4<<2),(UINT)((_rd32(_REG_BASE_+(0x01e4<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define B31_VEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e4<<2)))&BitMask_12)
#define B31_VEDw(val)			_wr32(_REG_BASE_+(0x01e4<<2),(UINT)((_rd32(_REG_BASE_+(0x01e4<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define B31_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x01e5<<2)))&BitMask_13)
#define B31_HSPw(val)			_wr32(_REG_BASE_+(0x01e5<<2),(UINT)((_rd32(_REG_BASE_+(0x01e5<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define B31_HEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e5<<2)))&BitMask_13)
#define B31_HEDw(val)			_wr32(_REG_BASE_+(0x01e5<<2),(UINT)((_rd32(_REG_BASE_+(0x01e5<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define BOX0_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e6<<2)))&BitMask_24)
#define BOX0_PLTw(val)			_wr32(_REG_BASE_+(0x01e6<<2),(UINT)((_rd32(_REG_BASE_+(0x01e6<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX1_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e7<<2)))&BitMask_24)
#define BOX1_PLTw(val)			_wr32(_REG_BASE_+(0x01e7<<2),(UINT)((_rd32(_REG_BASE_+(0x01e7<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX2_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e8<<2)))&BitMask_24)
#define BOX2_PLTw(val)			_wr32(_REG_BASE_+(0x01e8<<2),(UINT)((_rd32(_REG_BASE_+(0x01e8<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX3_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01e9<<2)))&BitMask_24)
#define BOX3_PLTw(val)			_wr32(_REG_BASE_+(0x01e9<<2),(UINT)((_rd32(_REG_BASE_+(0x01e9<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX4_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ea<<2)))&BitMask_24)
#define BOX4_PLTw(val)			_wr32(_REG_BASE_+(0x01ea<<2),(UINT)((_rd32(_REG_BASE_+(0x01ea<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX5_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01eb<<2)))&BitMask_24)
#define BOX5_PLTw(val)			_wr32(_REG_BASE_+(0x01eb<<2),(UINT)((_rd32(_REG_BASE_+(0x01eb<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX6_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ec<<2)))&BitMask_24)
#define BOX6_PLTw(val)			_wr32(_REG_BASE_+(0x01ec<<2),(UINT)((_rd32(_REG_BASE_+(0x01ec<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX7_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ed<<2)))&BitMask_24)
#define BOX7_PLTw(val)			_wr32(_REG_BASE_+(0x01ed<<2),(UINT)((_rd32(_REG_BASE_+(0x01ed<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX8_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ee<<2)))&BitMask_24)
#define BOX8_PLTw(val)			_wr32(_REG_BASE_+(0x01ee<<2),(UINT)((_rd32(_REG_BASE_+(0x01ee<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX9_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ef<<2)))&BitMask_24)
#define BOX9_PLTw(val)			_wr32(_REG_BASE_+(0x01ef<<2),(UINT)((_rd32(_REG_BASE_+(0x01ef<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX10_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f0<<2)))&BitMask_24)
#define BOX10_PLTw(val)			_wr32(_REG_BASE_+(0x01f0<<2),(UINT)((_rd32(_REG_BASE_+(0x01f0<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX11_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f1<<2)))&BitMask_24)
#define BOX11_PLTw(val)			_wr32(_REG_BASE_+(0x01f1<<2),(UINT)((_rd32(_REG_BASE_+(0x01f1<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX12_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f2<<2)))&BitMask_24)
#define BOX12_PLTw(val)			_wr32(_REG_BASE_+(0x01f2<<2),(UINT)((_rd32(_REG_BASE_+(0x01f2<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX13_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f3<<2)))&BitMask_24)
#define BOX13_PLTw(val)			_wr32(_REG_BASE_+(0x01f3<<2),(UINT)((_rd32(_REG_BASE_+(0x01f3<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX14_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f4<<2)))&BitMask_24)
#define BOX14_PLTw(val)			_wr32(_REG_BASE_+(0x01f4<<2),(UINT)((_rd32(_REG_BASE_+(0x01f4<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX15_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f5<<2)))&BitMask_24)
#define BOX15_PLTw(val)			_wr32(_REG_BASE_+(0x01f5<<2),(UINT)((_rd32(_REG_BASE_+(0x01f5<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX16_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f6<<2)))&BitMask_24)
#define BOX16_PLTw(val)			_wr32(_REG_BASE_+(0x01f6<<2),(UINT)((_rd32(_REG_BASE_+(0x01f6<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX17_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f7<<2)))&BitMask_24)
#define BOX17_PLTw(val)			_wr32(_REG_BASE_+(0x01f7<<2),(UINT)((_rd32(_REG_BASE_+(0x01f7<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX18_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f8<<2)))&BitMask_24)
#define BOX18_PLTw(val)			_wr32(_REG_BASE_+(0x01f8<<2),(UINT)((_rd32(_REG_BASE_+(0x01f8<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX19_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01f9<<2)))&BitMask_24)
#define BOX19_PLTw(val)			_wr32(_REG_BASE_+(0x01f9<<2),(UINT)((_rd32(_REG_BASE_+(0x01f9<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX20_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01fa<<2)))&BitMask_24)
#define BOX20_PLTw(val)			_wr32(_REG_BASE_+(0x01fa<<2),(UINT)((_rd32(_REG_BASE_+(0x01fa<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX21_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01fb<<2)))&BitMask_24)
#define BOX21_PLTw(val)			_wr32(_REG_BASE_+(0x01fb<<2),(UINT)((_rd32(_REG_BASE_+(0x01fb<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX22_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01fc<<2)))&BitMask_24)
#define BOX22_PLTw(val)			_wr32(_REG_BASE_+(0x01fc<<2),(UINT)((_rd32(_REG_BASE_+(0x01fc<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX23_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01fd<<2)))&BitMask_24)
#define BOX23_PLTw(val)			_wr32(_REG_BASE_+(0x01fd<<2),(UINT)((_rd32(_REG_BASE_+(0x01fd<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX24_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01fe<<2)))&BitMask_24)
#define BOX24_PLTw(val)			_wr32(_REG_BASE_+(0x01fe<<2),(UINT)((_rd32(_REG_BASE_+(0x01fe<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX25_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x01ff<<2)))&BitMask_24)
#define BOX25_PLTw(val)			_wr32(_REG_BASE_+(0x01ff<<2),(UINT)((_rd32(_REG_BASE_+(0x01ff<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX26_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0200<<2)))&BitMask_24)
#define BOX26_PLTw(val)			_wr32(_REG_BASE_+(0x0200<<2),(UINT)((_rd32(_REG_BASE_+(0x0200<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX27_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0201<<2)))&BitMask_24)
#define BOX27_PLTw(val)			_wr32(_REG_BASE_+(0x0201<<2),(UINT)((_rd32(_REG_BASE_+(0x0201<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX28_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0202<<2)))&BitMask_24)
#define BOX28_PLTw(val)			_wr32(_REG_BASE_+(0x0202<<2),(UINT)((_rd32(_REG_BASE_+(0x0202<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX29_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0203<<2)))&BitMask_24)
#define BOX29_PLTw(val)			_wr32(_REG_BASE_+(0x0203<<2),(UINT)((_rd32(_REG_BASE_+(0x0203<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX30_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0204<<2)))&BitMask_24)
#define BOX30_PLTw(val)			_wr32(_REG_BASE_+(0x0204<<2),(UINT)((_rd32(_REG_BASE_+(0x0204<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BOX31_PLTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0205<<2)))&BitMask_24)
#define BOX31_PLTw(val)			_wr32(_REG_BASE_+(0x0205<<2),(UINT)((_rd32(_REG_BASE_+(0x0205<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define BITBOX0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0206<<2)))&BitMask_32)
#define BITBOX0w(val)			_wr32(_REG_BASE_+(0x0206<<2),(UINT)((_rd32(_REG_BASE_+(0x0206<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define BIT_CB0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0207<<2)))&BitMask_08)
#define BIT_CB0w(val)			_wr32(_REG_BASE_+(0x0207<<2),(UINT)((_rd32(_REG_BASE_+(0x0207<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define BIT_CR0r				(ValSft_R08(_rd32(_REG_BASE_+(0x0207<<2)))&BitMask_08)
#define BIT_CR0w(val)			_wr32(_REG_BASE_+(0x0207<<2),(UINT)((_rd32(_REG_BASE_+(0x0207<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define BITMAP_THRES0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0207<<2)))&BitMask_08)
#define BITMAP_THRES0w(val)		_wr32(_REG_BASE_+(0x0207<<2),(UINT)((_rd32(_REG_BASE_+(0x0207<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define RCH0_HATr				(ValSft_R31(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_01)
#define RCH0_HATw(val)			_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RCH1_HATr				(ValSft_R30(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_01)
#define RCH1_HATw(val)			_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RCH2_HATr				(ValSft_R29(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_01)
#define RCH2_HATw(val)			_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RCH3_HATr				(ValSft_R28(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_01)
#define RCH3_HATw(val)			_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define RCH4_HATr				(ValSft_R27(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_01)
#define RCH4_HATw(val)			_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IM_WCH1_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_03)
#define IM_WCH1_SELw(val)		_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define IM_WCH2_SELr			(ValSft_R08(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_03)
#define IM_WCH2_SELw(val)		_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define IM_WCH3_SELr			(ValSft_R04(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_03)
#define IM_WCH3_SELw(val)		_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L04(BitMask_03)))|ValSft_L04((val)&BitMask_03)))

#define IM_WCH4_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x0210<<2)))&BitMask_03)
#define IM_WCH4_SELw(val)		_wr32(_REG_BASE_+(0x0210<<2),(UINT)((_rd32(_REG_BASE_+(0x0210<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define IM_IVSEL1r				(ValSft_R29(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_03)
#define IM_IVSEL1w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define IM_ISEL1r				(ValSft_R24(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_05)
#define IM_ISEL1w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define IM_IVSEL2r				(ValSft_R21(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_03)
#define IM_IVSEL2w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L21(BitMask_03)))|ValSft_L21((val)&BitMask_03)))

#define IM_ISEL2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_05)
#define IM_ISEL2w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L16(BitMask_05)))|ValSft_L16((val)&BitMask_05)))

#define IM_IVSEL3r				(ValSft_R13(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_03)
#define IM_IVSEL3w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L13(BitMask_03)))|ValSft_L13((val)&BitMask_03)))

#define IM_ISEL3r				(ValSft_R08(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_05)
#define IM_ISEL3w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L08(BitMask_05)))|ValSft_L08((val)&BitMask_05)))

#define IM_IVSEL4r				(ValSft_R05(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_03)
#define IM_IVSEL4w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L05(BitMask_03)))|ValSft_L05((val)&BitMask_03)))

#define IM_ISEL4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0211<<2)))&BitMask_05)
#define IM_ISEL4w(val)			_wr32(_REG_BASE_+(0x0211<<2),(UINT)((_rd32(_REG_BASE_+(0x0211<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define IM_IVSEL0r				(ValSft_R29(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_03)
#define IM_IVSEL0w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define IM_ISEL0r				(ValSft_R24(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_05)
#define IM_ISEL0w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define IM_WTEST1r				(ValSft_R23(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WTEST1w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IM_WTEST2r				(ValSft_R22(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WTEST2w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IM_WTEST3r				(ValSft_R21(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WTEST3w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IM_WTEST4r				(ValSft_R20(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WTEST4w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IM_CWEN0r				(ValSft_R14(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_CWEN0w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define IM_WCINV0r				(ValSft_R13(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCINV0w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define IM_WCLINE_INV0r			(ValSft_R12(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCLINE_INV0w(val)	_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define IM_CWEN1r				(ValSft_R11(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_CWEN1w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IM_WCINV1r				(ValSft_R10(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCINV1w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define IM_WCLINE_INV1r			(ValSft_R09(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCLINE_INV1w(val)	_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define IM_CWEN2r				(ValSft_R08(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_CWEN2w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define IM_WCINV2r				(ValSft_R07(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCINV2w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define IM_WCLINE_INV2r			(ValSft_R06(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCLINE_INV2w(val)	_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define IM_CWEN3r				(ValSft_R05(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_CWEN3w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define IM_WCINV3r				(ValSft_R04(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCINV3w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define IM_WCLINE_INV3r			(ValSft_R03(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCLINE_INV3w(val)	_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define IM_CWEN4r				(ValSft_R02(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_CWEN4w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define IM_WCINV4r				(ValSft_R01(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCINV4w(val)			_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define IM_WCLINE_INV4r			(ValSft_R00(_rd32(_REG_BASE_+(0x0212<<2)))&BitMask_01)
#define IM_WCLINE_INV4w(val)	_wr32(_REG_BASE_+(0x0212<<2),(UINT)((_rd32(_REG_BASE_+(0x0212<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define IM_HWI0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0213<<2)))&BitMask_13)
#define IM_HWI0w(val)			_wr32(_REG_BASE_+(0x0213<<2),(UINT)((_rd32(_REG_BASE_+(0x0213<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_HWI1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0213<<2)))&BitMask_13)
#define IM_HWI1w(val)			_wr32(_REG_BASE_+(0x0213<<2),(UINT)((_rd32(_REG_BASE_+(0x0213<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HWI2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0214<<2)))&BitMask_13)
#define IM_HWI2w(val)			_wr32(_REG_BASE_+(0x0214<<2),(UINT)((_rd32(_REG_BASE_+(0x0214<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_HWI3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0214<<2)))&BitMask_13)
#define IM_HWI3w(val)			_wr32(_REG_BASE_+(0x0214<<2),(UINT)((_rd32(_REG_BASE_+(0x0214<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_RCH1_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_03)
#define IM_RCH1_SELw(val)		_wr32(_REG_BASE_+(0x0215<<2),(UINT)((_rd32(_REG_BASE_+(0x0215<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define IM_RCH2_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_03)
#define IM_RCH2_SELw(val)		_wr32(_REG_BASE_+(0x0215<<2),(UINT)((_rd32(_REG_BASE_+(0x0215<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IM_RCH3_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_03)
#define IM_RCH3_SELw(val)		_wr32(_REG_BASE_+(0x0215<<2),(UINT)((_rd32(_REG_BASE_+(0x0215<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define IM_RCH4_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_03)
#define IM_RCH4_SELw(val)		_wr32(_REG_BASE_+(0x0215<<2),(UINT)((_rd32(_REG_BASE_+(0x0215<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define IM_HWI4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0215<<2)))&BitMask_13)
#define IM_HWI4w(val)			_wr32(_REG_BASE_+(0x0215<<2),(UINT)((_rd32(_REG_BASE_+(0x0215<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_FRC1_RLOCKONr		(ValSft_R30(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC1_RLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_FRC2_RLOCKONr		(ValSft_R29(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC2_RLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IM_FRC3_RLOCKONr		(ValSft_R28(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC3_RLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IM_FRC4_RLOCKONr		(ValSft_R27(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC4_RLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IM_2PAGE1r				(ValSft_R26(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_2PAGE1w(val)			_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define IM_2PAGE2r				(ValSft_R25(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_2PAGE2w(val)			_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define IM_2PAGE3r				(ValSft_R24(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_2PAGE3w(val)			_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define IM_2PAGE4r				(ValSft_R23(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_2PAGE4w(val)			_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IM_RCINTP0r				(ValSft_R21(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_RCINTP0w(val)		_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IM_RCINTP1r				(ValSft_R20(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_RCINTP1w(val)		_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IM_RCINTP2r				(ValSft_R19(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_RCINTP2w(val)		_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define IM_RCINTP3r				(ValSft_R18(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_RCINTP3w(val)		_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IM_RCINTP4r				(ValSft_R17(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_RCINTP4w(val)		_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define IM_FRC1_WLOCKONr		(ValSft_R16(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC1_WLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define IM_FRC2_WLOCKONr		(ValSft_R15(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC2_WLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define IM_FRC3_WLOCKONr		(ValSft_R14(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC3_WLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define IM_FRC4_WLOCKONr		(ValSft_R13(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_01)
#define IM_FRC4_WLOCKONw(val)	_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define MYCW_HWOr				(ValSft_R00(_rd32(_REG_BASE_+(0x0216<<2)))&BitMask_13)
#define MYCW_HWOw(val)			_wr32(_REG_BASE_+(0x0216<<2),(UINT)((_rd32(_REG_BASE_+(0x0216<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_GO0r					(ValSft_R31(_rd32(_REG_BASE_+(0x0217<<2)))&BitMask_01)
#define IM_GO0w(val)			_wr32(_REG_BASE_+(0x0217<<2),(UINT)((_rd32(_REG_BASE_+(0x0217<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_CGO0r				(ValSft_R30(_rd32(_REG_BASE_+(0x0217<<2)))&BitMask_01)
#define IM_CGO0w(val)			_wr32(_REG_BASE_+(0x0217<<2),(UINT)((_rd32(_REG_BASE_+(0x0217<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_GO1r					(ValSft_R31(_rd32(_REG_BASE_+(0x0218<<2)))&BitMask_01)
#define IM_GO1w(val)			_wr32(_REG_BASE_+(0x0218<<2),(UINT)((_rd32(_REG_BASE_+(0x0218<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_CGO1r				(ValSft_R30(_rd32(_REG_BASE_+(0x0218<<2)))&BitMask_01)
#define IM_CGO1w(val)			_wr32(_REG_BASE_+(0x0218<<2),(UINT)((_rd32(_REG_BASE_+(0x0218<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_GO2r					(ValSft_R31(_rd32(_REG_BASE_+(0x0219<<2)))&BitMask_01)
#define IM_GO2w(val)			_wr32(_REG_BASE_+(0x0219<<2),(UINT)((_rd32(_REG_BASE_+(0x0219<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_CGO2r				(ValSft_R30(_rd32(_REG_BASE_+(0x0219<<2)))&BitMask_01)
#define IM_CGO2w(val)			_wr32(_REG_BASE_+(0x0219<<2),(UINT)((_rd32(_REG_BASE_+(0x0219<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_GO3r					(ValSft_R31(_rd32(_REG_BASE_+(0x021a<<2)))&BitMask_01)
#define IM_GO3w(val)			_wr32(_REG_BASE_+(0x021a<<2),(UINT)((_rd32(_REG_BASE_+(0x021a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_CGO3r				(ValSft_R30(_rd32(_REG_BASE_+(0x021a<<2)))&BitMask_01)
#define IM_CGO3w(val)			_wr32(_REG_BASE_+(0x021a<<2),(UINT)((_rd32(_REG_BASE_+(0x021a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_GO4r					(ValSft_R31(_rd32(_REG_BASE_+(0x021b<<2)))&BitMask_01)
#define IM_GO4w(val)			_wr32(_REG_BASE_+(0x021b<<2),(UINT)((_rd32(_REG_BASE_+(0x021b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_CGO4r				(ValSft_R30(_rd32(_REG_BASE_+(0x021b<<2)))&BitMask_01)
#define IM_CGO4w(val)			_wr32(_REG_BASE_+(0x021b<<2),(UINT)((_rd32(_REG_BASE_+(0x021b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_WFRC1_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_WFRC1_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IM_WFRC2_ONr			(ValSft_R28(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_WFRC2_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IM_WFRC3_ONr			(ValSft_R27(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_WFRC3_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IM_WFRC4_ONr			(ValSft_R26(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_WFRC4_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define IM_STLL1_ONr			(ValSft_R25(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_STLL1_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define IM_STLL2_ONr			(ValSft_R24(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_STLL2_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define IM_STLL3_ONr			(ValSft_R23(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_STLL3_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IM_STLL4_ONr			(ValSft_R22(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_STLL4_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IM_INTL0_ONr			(ValSft_R21(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_INTL0_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IM_INTL1_ONr			(ValSft_R20(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_INTL1_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IM_INTL2_ONr			(ValSft_R19(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_INTL2_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define IM_INTL3_ONr			(ValSft_R18(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_INTL3_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IM_INTL4_ONr			(ValSft_R17(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_INTL4_ONw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define IM_NPFONT0r				(ValSft_R11(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_NPFONT0w(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IM_NPFONT1r				(ValSft_R10(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_NPFONT1w(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define IM_NPFONT2r				(ValSft_R09(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_NPFONT2w(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define IM_NPFONT3r				(ValSft_R08(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_NPFONT3w(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define IM_NPFONT4r				(ValSft_R07(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_NPFONT4w(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define FRC_INTLACEr			(ValSft_R06(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define FRC_INTLACEw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define FRC_FLD_INVr			(ValSft_R05(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define FRC_FLD_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define IM_FLDI0_INVr			(ValSft_R04(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_FLDI0_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define IM_FLDI1_INVr			(ValSft_R03(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_FLDI1_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define IM_FLDI2_INVr			(ValSft_R02(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_FLDI2_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define IM_FLDI3_INVr			(ValSft_R01(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_FLDI3_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define IM_FLDI4_INVr			(ValSft_R00(_rd32(_REG_BASE_+(0x021c<<2)))&BitMask_01)
#define IM_FLDI4_INVw(val)		_wr32(_REG_BASE_+(0x021c<<2),(UINT)((_rd32(_REG_BASE_+(0x021c<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define IM_RON0r				(ValSft_R30(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RON0w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_RCINV0r				(ValSft_R29(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RCINV0w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IM_CLRREN0r				(ValSft_R28(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_CLRREN0w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IM_RFRC1_ONr			(ValSft_R27(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RFRC1_ONw(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IM_RON1r				(ValSft_R26(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RON1w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define IM_RCINV1r				(ValSft_R25(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RCINV1w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define IM_CLRREN1r				(ValSft_R24(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_CLRREN1w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define IM_RFRC2_ONr			(ValSft_R23(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RFRC2_ONw(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IM_RON2r				(ValSft_R22(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RON2w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IM_RCINV2r				(ValSft_R21(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RCINV2w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IM_CLRREN2r				(ValSft_R20(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_CLRREN2w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IM_RFRC3_ONr			(ValSft_R19(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RFRC3_ONw(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define IM_RON3r				(ValSft_R18(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RON3w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IM_RCINV3r				(ValSft_R17(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RCINV3w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define IM_CLRREN3r				(ValSft_R16(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_CLRREN3w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define IM_RFRC4_ONr			(ValSft_R15(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RFRC4_ONw(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define IM_RON4r				(ValSft_R14(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RON4w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define IM_RCINV4r				(ValSft_R13(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RCINV4w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define IM_CLRREN4r				(ValSft_R12(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_CLRREN4w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define IM_RTEST0r				(ValSft_R11(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RTEST0w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IM_RTEST1r				(ValSft_R10(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RTEST1w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define IM_RTEST2r				(ValSft_R09(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RTEST2w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define IM_RTEST3r				(ValSft_R08(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RTEST3w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define IM_RTEST4r				(ValSft_R07(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RTEST4w(val)			_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define IM_RYCB_MOD0r			(ValSft_R04(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RYCB_MOD0w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define IM_RYCB_MOD1r			(ValSft_R03(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RYCB_MOD1w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define IM_RYCB_MOD2r			(ValSft_R02(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RYCB_MOD2w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define IM_RYCB_MOD3r			(ValSft_R01(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RYCB_MOD3w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define IM_RYCB_MOD4r			(ValSft_R00(_rd32(_REG_BASE_+(0x021d<<2)))&BitMask_01)
#define IM_RYCB_MOD4w(val)		_wr32(_REG_BASE_+(0x021d<<2),(UINT)((_rd32(_REG_BASE_+(0x021d<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define RI1_MR_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_01)
#define RI1_MR_ONw(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RI2_MR_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_01)
#define RI2_MR_ONw(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_RVSEL0r				(ValSft_R27(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RVSEL0w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L27(BitMask_03)))|ValSft_L27((val)&BitMask_03)))

#define IM_RISEL0r				(ValSft_R24(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RISEL0w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define IM_RVSEL1r				(ValSft_R21(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RVSEL1w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L21(BitMask_03)))|ValSft_L21((val)&BitMask_03)))

#define IM_RISEL1r				(ValSft_R18(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RISEL1w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L18(BitMask_03)))|ValSft_L18((val)&BitMask_03)))

#define IM_RVSEL2r				(ValSft_R15(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RVSEL2w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L15(BitMask_03)))|ValSft_L15((val)&BitMask_03)))

#define IM_RISEL2r				(ValSft_R12(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RISEL2w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define IM_RVSEL3r				(ValSft_R09(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RVSEL3w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L09(BitMask_03)))|ValSft_L09((val)&BitMask_03)))

#define IM_RISEL3r				(ValSft_R06(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RISEL3w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L06(BitMask_03)))|ValSft_L06((val)&BitMask_03)))

#define IM_RVSEL4r				(ValSft_R03(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RVSEL4w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L03(BitMask_03)))|ValSft_L03((val)&BitMask_03)))

#define IM_RISEL4r				(ValSft_R00(_rd32(_REG_BASE_+(0x021e<<2)))&BitMask_03)
#define IM_RISEL4w(val)			_wr32(_REG_BASE_+(0x021e<<2),(UINT)((_rd32(_REG_BASE_+(0x021e<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define RI3_MR_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x021f<<2)))&BitMask_01)
#define RI3_MR_ONw(val)			_wr32(_REG_BASE_+(0x021f<<2),(UINT)((_rd32(_REG_BASE_+(0x021f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RI4_MR_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x021f<<2)))&BitMask_01)
#define RI4_MR_ONw(val)			_wr32(_REG_BASE_+(0x021f<<2),(UINT)((_rd32(_REG_BASE_+(0x021f<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PIP0_DDR_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x021f<<2)))&BitMask_13)
#define PIP0_DDR_LTCw(val)		_wr32(_REG_BASE_+(0x021f<<2),(UINT)((_rd32(_REG_BASE_+(0x021f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP1_DDR_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x021f<<2)))&BitMask_13)
#define PIP1_DDR_LTCw(val)		_wr32(_REG_BASE_+(0x021f<<2),(UINT)((_rd32(_REG_BASE_+(0x021f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define FONT0_PIP_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_01)
#define FONT0_PIP_ONw(val)		_wr32(_REG_BASE_+(0x0220<<2),(UINT)((_rd32(_REG_BASE_+(0x0220<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP_MMASKr				(ValSft_R24(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_01)
#define PIP_MMASKw(val)			_wr32(_REG_BASE_+(0x0220<<2),(UINT)((_rd32(_REG_BASE_+(0x0220<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define PIP_IVSELr				(ValSft_R21(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_03)
#define PIP_IVSELw(val)			_wr32(_REG_BASE_+(0x0220<<2),(UINT)((_rd32(_REG_BASE_+(0x0220<<2))&(~ValSft_L21(BitMask_03)))|ValSft_L21((val)&BitMask_03)))

#define PIP_CTOG_INVr			(ValSft_R20(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_01)
#define PIP_CTOG_INVw(val)		_wr32(_REG_BASE_+(0x0220<<2),(UINT)((_rd32(_REG_BASE_+(0x0220<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define PIP_MAIN_ISELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0220<<2)))&BitMask_04)
#define PIP_MAIN_ISELw(val)		_wr32(_REG_BASE_+(0x0220<<2),(UINT)((_rd32(_REG_BASE_+(0x0220<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define PIP0_MASKr				(ValSft_R31(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_01)
#define PIP0_MASKw(val)			_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP0_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_01)
#define PIP0_ONw(val)			_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PIP0_BOX_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_01)
#define PIP0_BOX_ONw(val)		_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PIP0_MIX_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_02)
#define PIP0_MIX_SELw(val)		_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PIP0_BOX_Yr				(ValSft_R16(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_08)
#define PIP0_BOX_Yw(val)		_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PIP0_BOX_CBr			(ValSft_R08(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_08)
#define PIP0_BOX_CBw(val)		_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PIP0_BOX_CRr			(ValSft_R00(_rd32(_REG_BASE_+(0x0221<<2)))&BitMask_08)
#define PIP0_BOX_CRw(val)		_wr32(_REG_BASE_+(0x0221<<2),(UINT)((_rd32(_REG_BASE_+(0x0221<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PIP0_ISELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0222<<2)))&BitMask_03)
#define PIP0_ISELw(val)			_wr32(_REG_BASE_+(0x0222<<2),(UINT)((_rd32(_REG_BASE_+(0x0222<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define PIP0_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0222<<2)))&BitMask_13)
#define PIP0_HSPw(val)			_wr32(_REG_BASE_+(0x0222<<2),(UINT)((_rd32(_REG_BASE_+(0x0222<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP0_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0222<<2)))&BitMask_12)
#define PIP0_VSPw(val)			_wr32(_REG_BASE_+(0x0222<<2),(UINT)((_rd32(_REG_BASE_+(0x0222<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP0_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0223<<2)))&BitMask_13)
#define PIP0_HWw(val)			_wr32(_REG_BASE_+(0x0223<<2),(UINT)((_rd32(_REG_BASE_+(0x0223<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP0_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0223<<2)))&BitMask_12)
#define PIP0_VWw(val)			_wr32(_REG_BASE_+(0x0223<<2),(UINT)((_rd32(_REG_BASE_+(0x0223<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP1_MASKr				(ValSft_R31(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_01)
#define PIP1_MASKw(val)			_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP1_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_01)
#define PIP1_ONw(val)			_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PIP1_BOX_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_01)
#define PIP1_BOX_ONw(val)		_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PIP1_MIX_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_02)
#define PIP1_MIX_SELw(val)		_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PIP1_BOX_Yr				(ValSft_R16(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_08)
#define PIP1_BOX_Yw(val)		_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PIP1_BOX_CBr			(ValSft_R08(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_08)
#define PIP1_BOX_CBw(val)		_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PIP1_BOX_CRr			(ValSft_R00(_rd32(_REG_BASE_+(0x0224<<2)))&BitMask_08)
#define PIP1_BOX_CRw(val)		_wr32(_REG_BASE_+(0x0224<<2),(UINT)((_rd32(_REG_BASE_+(0x0224<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PIP1_ISELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0225<<2)))&BitMask_03)
#define PIP1_ISELw(val)			_wr32(_REG_BASE_+(0x0225<<2),(UINT)((_rd32(_REG_BASE_+(0x0225<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define PIP1_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0225<<2)))&BitMask_13)
#define PIP1_HSPw(val)			_wr32(_REG_BASE_+(0x0225<<2),(UINT)((_rd32(_REG_BASE_+(0x0225<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP1_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0225<<2)))&BitMask_12)
#define PIP1_VSPw(val)			_wr32(_REG_BASE_+(0x0225<<2),(UINT)((_rd32(_REG_BASE_+(0x0225<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP1_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0226<<2)))&BitMask_13)
#define PIP1_HWw(val)			_wr32(_REG_BASE_+(0x0226<<2),(UINT)((_rd32(_REG_BASE_+(0x0226<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP1_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0226<<2)))&BitMask_12)
#define PIP1_VWw(val)			_wr32(_REG_BASE_+(0x0226<<2),(UINT)((_rd32(_REG_BASE_+(0x0226<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP2_MASKr				(ValSft_R31(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_01)
#define PIP2_MASKw(val)			_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP2_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_01)
#define PIP2_ONw(val)			_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PIP2_BOX_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_01)
#define PIP2_BOX_ONw(val)		_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PIP2_MIX_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_02)
#define PIP2_MIX_SELw(val)		_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PIP2_BOX_Yr				(ValSft_R16(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_08)
#define PIP2_BOX_Yw(val)		_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PIP2_BOX_CBr			(ValSft_R08(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_08)
#define PIP2_BOX_CBw(val)		_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PIP2_BOX_CRr			(ValSft_R00(_rd32(_REG_BASE_+(0x0227<<2)))&BitMask_08)
#define PIP2_BOX_CRw(val)		_wr32(_REG_BASE_+(0x0227<<2),(UINT)((_rd32(_REG_BASE_+(0x0227<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PIP2_ISELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0228<<2)))&BitMask_03)
#define PIP2_ISELw(val)			_wr32(_REG_BASE_+(0x0228<<2),(UINT)((_rd32(_REG_BASE_+(0x0228<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define PIP2_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0228<<2)))&BitMask_13)
#define PIP2_HSPw(val)			_wr32(_REG_BASE_+(0x0228<<2),(UINT)((_rd32(_REG_BASE_+(0x0228<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP2_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0228<<2)))&BitMask_12)
#define PIP2_VSPw(val)			_wr32(_REG_BASE_+(0x0228<<2),(UINT)((_rd32(_REG_BASE_+(0x0228<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP2_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0229<<2)))&BitMask_13)
#define PIP2_HWw(val)			_wr32(_REG_BASE_+(0x0229<<2),(UINT)((_rd32(_REG_BASE_+(0x0229<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP2_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0229<<2)))&BitMask_12)
#define PIP2_VWw(val)			_wr32(_REG_BASE_+(0x0229<<2),(UINT)((_rd32(_REG_BASE_+(0x0229<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP3_MASKr				(ValSft_R31(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_01)
#define PIP3_MASKw(val)			_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP3_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_01)
#define PIP3_ONw(val)			_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define PIP3_BOX_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_01)
#define PIP3_BOX_ONw(val)		_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define PIP3_MIX_SELr			(ValSft_R24(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_02)
#define PIP3_MIX_SELw(val)		_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L24(BitMask_02)))|ValSft_L24((val)&BitMask_02)))

#define PIP3_BOX_Yr				(ValSft_R16(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_08)
#define PIP3_BOX_Yw(val)		_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define PIP3_BOX_CBr			(ValSft_R08(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_08)
#define PIP3_BOX_CBw(val)		_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define PIP3_BOX_CRr			(ValSft_R00(_rd32(_REG_BASE_+(0x022a<<2)))&BitMask_08)
#define PIP3_BOX_CRw(val)		_wr32(_REG_BASE_+(0x022a<<2),(UINT)((_rd32(_REG_BASE_+(0x022a<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define PIP3_ISELr				(ValSft_R29(_rd32(_REG_BASE_+(0x022b<<2)))&BitMask_03)
#define PIP3_ISELw(val)			_wr32(_REG_BASE_+(0x022b<<2),(UINT)((_rd32(_REG_BASE_+(0x022b<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define PIP3_HSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x022b<<2)))&BitMask_13)
#define PIP3_HSPw(val)			_wr32(_REG_BASE_+(0x022b<<2),(UINT)((_rd32(_REG_BASE_+(0x022b<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP3_VSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x022b<<2)))&BitMask_12)
#define PIP3_VSPw(val)			_wr32(_REG_BASE_+(0x022b<<2),(UINT)((_rd32(_REG_BASE_+(0x022b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP3_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x022c<<2)))&BitMask_13)
#define PIP3_HWw(val)			_wr32(_REG_BASE_+(0x022c<<2),(UINT)((_rd32(_REG_BASE_+(0x022c<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP3_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x022c<<2)))&BitMask_12)
#define PIP3_VWw(val)			_wr32(_REG_BASE_+(0x022c<<2),(UINT)((_rd32(_REG_BASE_+(0x022c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP_MACT_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x022d<<2)))&BitMask_01)
#define PIP_MACT_SELw(val)		_wr32(_REG_BASE_+(0x022d<<2),(UINT)((_rd32(_REG_BASE_+(0x022d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define PIP_MHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x022d<<2)))&BitMask_13)
#define PIP_MHSPw(val)			_wr32(_REG_BASE_+(0x022d<<2),(UINT)((_rd32(_REG_BASE_+(0x022d<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP_MHWOr				(ValSft_R00(_rd32(_REG_BASE_+(0x022d<<2)))&BitMask_13)
#define PIP_MHWOw(val)			_wr32(_REG_BASE_+(0x022d<<2),(UINT)((_rd32(_REG_BASE_+(0x022d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define PIP_MVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x022e<<2)))&BitMask_12)
#define PIP_MVSPw(val)			_wr32(_REG_BASE_+(0x022e<<2),(UINT)((_rd32(_REG_BASE_+(0x022e<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define PIP_MVWOr				(ValSft_R00(_rd32(_REG_BASE_+(0x022e<<2)))&BitMask_12)
#define PIP_MVWOw(val)			_wr32(_REG_BASE_+(0x022e<<2),(UINT)((_rd32(_REG_BASE_+(0x022e<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define PIP2_DDR_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x022f<<2)))&BitMask_13)
#define PIP2_DDR_LTCw(val)		_wr32(_REG_BASE_+(0x022f<<2),(UINT)((_rd32(_REG_BASE_+(0x022f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define PIP3_DDR_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x022f<<2)))&BitMask_13)
#define PIP3_DDR_LTCw(val)		_wr32(_REG_BASE_+(0x022f<<2),(UINT)((_rd32(_REG_BASE_+(0x022f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define JPG_GOr					(ValSft_R31(_rd32(_REG_BASE_+(0x0230<<2)))&BitMask_01)
#define JPG_GOw(val)			_wr32(_REG_BASE_+(0x0230<<2),(UINT)((_rd32(_REG_BASE_+(0x0230<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_ZRL_CUTr			(ValSft_R30(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_01)
#define JPG_ZRL_CUTw(val)		_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_RST_IVLr			(ValSft_R28(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_02)
#define JPG_RST_IVLw(val)		_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define JPG_QP0r				(ValSft_R20(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_08)
#define JPG_QP0w(val)			_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L20(BitMask_08)))|ValSft_L20((val)&BitMask_08)))

#define JPG_QP1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_03)
#define JPG_QP1w(val)			_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define JPG_YDC_QTBLr			(ValSft_R06(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_06)
#define JPG_YDC_QTBLw(val)		_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L06(BitMask_06)))|ValSft_L06((val)&BitMask_06)))

#define JPG_CDC_QTBLr			(ValSft_R00(_rd32(_REG_BASE_+(0x0231<<2)))&BitMask_06)
#define JPG_CDC_QTBLw(val)		_wr32(_REG_BASE_+(0x0231<<2),(UINT)((_rd32(_REG_BASE_+(0x0231<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define JPG_IVSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0232<<2)))&BitMask_04)
#define JPG_IVSELw(val)			_wr32(_REG_BASE_+(0x0232<<2),(UINT)((_rd32(_REG_BASE_+(0x0232<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define JPG_ADRr				(ValSft_R00(_rd32(_REG_BASE_+(0x0232<<2)))&BitMask_28)
#define JPG_ADRw(val)			_wr32(_REG_BASE_+(0x0232<<2),(UINT)((_rd32(_REG_BASE_+(0x0232<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define JPG_MEM_PDr				(ValSft_R31(_rd32(_REG_BASE_+(0x0233<<2)))&BitMask_01)
#define JPG_MEM_PDw(val)		_wr32(_REG_BASE_+(0x0233<<2),(UINT)((_rd32(_REG_BASE_+(0x0233<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_NPFONTr				(ValSft_R30(_rd32(_REG_BASE_+(0x0233<<2)))&BitMask_01)
#define JPG_NPFONTw(val)		_wr32(_REG_BASE_+(0x0233<<2),(UINT)((_rd32(_REG_BASE_+(0x0233<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_MAXr				(ValSft_R00(_rd32(_REG_BASE_+(0x0233<<2)))&BitMask_28)
#define JPG_MAXw(val)			_wr32(_REG_BASE_+(0x0233<<2),(UINT)((_rd32(_REG_BASE_+(0x0233<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define JPG_HWr					(ValSft_R16(_rd32(_REG_BASE_+(0x0234<<2)))&BitMask_13)
#define JPG_HWw(val)			_wr32(_REG_BASE_+(0x0234<<2),(UINT)((_rd32(_REG_BASE_+(0x0234<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define JPG_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x0234<<2)))&BitMask_12)
#define JPG_VWw(val)			_wr32(_REG_BASE_+(0x0234<<2),(UINT)((_rd32(_REG_BASE_+(0x0234<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define JPG_HBLK_SELr			(ValSft_R31(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_01)
#define JPG_HBLK_SELw(val)		_wr32(_REG_BASE_+(0x0235<<2),(UINT)((_rd32(_REG_BASE_+(0x0235<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JVLOCK_ADJ_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_01)
#define JVLOCK_ADJ_ONw(val)		_wr32(_REG_BASE_+(0x0235<<2),(UINT)((_rd32(_REG_BASE_+(0x0235<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_HBLK_CYCr			(ValSft_R16(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_14)
#define JPG_HBLK_CYCw(val)		_wr32(_REG_BASE_+(0x0235<<2),(UINT)((_rd32(_REG_BASE_+(0x0235<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define JPG_ISELr				(ValSft_R11(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_05)
#define JPG_ISELw(val)			_wr32(_REG_BASE_+(0x0235<<2),(UINT)((_rd32(_REG_BASE_+(0x0235<<2))&(~ValSft_L11(BitMask_05)))|ValSft_L11((val)&BitMask_05)))

#define JPG_VOFSr				(ValSft_R04(_rd32(_REG_BASE_+(0x0235<<2)))&BitMask_06)
#define JPG_VOFSw(val)			_wr32(_REG_BASE_+(0x0235<<2),(UINT)((_rd32(_REG_BASE_+(0x0235<<2))&(~ValSft_L04(BitMask_06)))|ValSft_L04((val)&BitMask_06)))

#define JVLOCK_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0236<<2)))&BitMask_12)
#define JVLOCK_POSw(val)		_wr32(_REG_BASE_+(0x0236<<2),(UINT)((_rd32(_REG_BASE_+(0x0236<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define JPG_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_01)
#define JPG_ONw(val)			_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_DC_FIXr				(ValSft_R30(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_01)
#define JPG_DC_FIXw(val)		_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_YQT_GAINr			(ValSft_R24(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_06)
#define JPG_YQT_GAINw(val)		_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L24(BitMask_06)))|ValSft_L24((val)&BitMask_06)))

#define JPG_CQT_GAINr			(ValSft_R16(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_06)
#define JPG_CQT_GAINw(val)		_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L16(BitMask_06)))|ValSft_L16((val)&BitMask_06)))

#define JPG_QP_MODEr			(ValSft_R14(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_02)
#define JPG_QP_MODEw(val)		_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define JPG_C_QUALITYr			(ValSft_R08(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_05)
#define JPG_C_QUALITYw(val)		_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L08(BitMask_05)))|ValSft_L08((val)&BitMask_05)))

#define JPG_C_DCT_CUT_ENr		(ValSft_R07(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_01)
#define JPG_C_DCT_CUT_ENw(val)	_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define JPG_C_DCT_CUT_ONr		(ValSft_R06(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_01)
#define JPG_C_DCT_CUT_ONw(val)	_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define JPG_C_DCT_CUT_NUMr		(ValSft_R00(_rd32(_REG_BASE_+(0x0237<<2)))&BitMask_06)
#define JPG_C_DCT_CUT_NUMw(val)	_wr32(_REG_BASE_+(0x0237<<2),(UINT)((_rd32(_REG_BASE_+(0x0237<<2))&(~ValSft_L00(BitMask_06)))|ValSft_L00((val)&BitMask_06)))

#define JPG_MODEr				(ValSft_R31(_rd32(_REG_BASE_+(0x0238<<2)))&BitMask_01)
#define JPG_MODEw(val)			_wr32(_REG_BASE_+(0x0238<<2),(UINT)((_rd32(_REG_BASE_+(0x0238<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define JPG_DEC_GOr				(ValSft_R30(_rd32(_REG_BASE_+(0x0238<<2)))&BitMask_01)
#define JPG_DEC_GOw(val)		_wr32(_REG_BASE_+(0x0238<<2),(UINT)((_rd32(_REG_BASE_+(0x0238<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define JPG_DEC_TONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0238<<2)))&BitMask_01)
#define JPG_DEC_TONw(val)		_wr32(_REG_BASE_+(0x0238<<2),(UINT)((_rd32(_REG_BASE_+(0x0238<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define JDEC_CINVr				(ValSft_R28(_rd32(_REG_BASE_+(0x0238<<2)))&BitMask_01)
#define JDEC_CINVw(val)			_wr32(_REG_BASE_+(0x0238<<2),(UINT)((_rd32(_REG_BASE_+(0x0238<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IM_IVSEL0_DSr			(ValSft_R29(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_03)
#define IM_IVSEL0_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define IM_ISEL0_DSr			(ValSft_R24(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_05)
#define IM_ISEL0_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define IM_IVSEL1_DSr			(ValSft_R21(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_03)
#define IM_IVSEL1_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L21(BitMask_03)))|ValSft_L21((val)&BitMask_03)))

#define IM_ISEL1_DSr			(ValSft_R16(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_05)
#define IM_ISEL1_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L16(BitMask_05)))|ValSft_L16((val)&BitMask_05)))

#define IM_IVSEL2_DSr			(ValSft_R13(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_03)
#define IM_IVSEL2_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L13(BitMask_03)))|ValSft_L13((val)&BitMask_03)))

#define IM_ISEL2_DSr			(ValSft_R08(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_05)
#define IM_ISEL2_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L08(BitMask_05)))|ValSft_L08((val)&BitMask_05)))

#define IM_IVSEL3_DSr			(ValSft_R05(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_03)
#define IM_IVSEL3_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L05(BitMask_03)))|ValSft_L05((val)&BitMask_03)))

#define IM_ISEL3_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x023a<<2)))&BitMask_05)
#define IM_ISEL3_DSw(val)		_wr32(_REG_BASE_+(0x023a<<2),(UINT)((_rd32(_REG_BASE_+(0x023a<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define IM_NPFONT0_DSr			(ValSft_R31(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_NPFONT0_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_FLDI0_INV_DSr		(ValSft_R30(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_FLDI0_INV_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_WCLINE_INV0_DSr		(ValSft_R29(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCLINE_INV0_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IM_WCINV0_DSr			(ValSft_R28(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCINV0_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define IM_CWEN0_DSr			(ValSft_R27(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_CWEN0_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IM_INTL0_ON_DSr			(ValSft_R26(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_INTL0_ON_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define IM_HALF0_DSr			(ValSft_R25(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_HALF0_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define IM_NPFONT1_DSr			(ValSft_R23(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_NPFONT1_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define IM_FLDI1_INV_DSr		(ValSft_R22(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_FLDI1_INV_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define IM_WCLINE_INV1_DSr		(ValSft_R21(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCLINE_INV1_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define IM_WCINV1_DSr			(ValSft_R20(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCINV1_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define IM_CWEN1_DSr			(ValSft_R19(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_CWEN1_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define IM_INTL1_ON_DSr			(ValSft_R18(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_INTL1_ON_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define IM_HALF1_DSr			(ValSft_R17(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_HALF1_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define DS_WCH2_ENr				(ValSft_R15(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define DS_WCH2_ENw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define IM_NPFONT2_DSr			(ValSft_R14(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_NPFONT2_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define IM_FLDI2_INV_DSr		(ValSft_R13(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_FLDI2_INV_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define IM_WCLINE_INV2_DSr		(ValSft_R12(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCLINE_INV2_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L12(BitMask_01)))|ValSft_L12((val)&BitMask_01)))

#define IM_WCINV2_DSr			(ValSft_R11(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCINV2_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define IM_CWEN2_DSr			(ValSft_R10(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_CWEN2_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L10(BitMask_01)))|ValSft_L10((val)&BitMask_01)))

#define IM_INTL2_ON_DSr			(ValSft_R09(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_INTL2_ON_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define IM_HALF2_DSr			(ValSft_R08(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_HALF2_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define DS_WCH3_ENr				(ValSft_R07(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define DS_WCH3_ENw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define IM_NPFONT3_DSr			(ValSft_R06(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_NPFONT3_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define IM_FLDI3_INV_DSr		(ValSft_R05(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_FLDI3_INV_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define IM_WCLINE_INV3_DSr		(ValSft_R04(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCLINE_INV3_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define IM_WCINV3_DSr			(ValSft_R03(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_WCINV3_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define IM_CWEN3_DSr			(ValSft_R02(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_CWEN3_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define IM_INTL3_ON_DSr			(ValSft_R01(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_INTL3_ON_DSw(val)	_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define IM_HALF3_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x023b<<2)))&BitMask_01)
#define IM_HALF3_DSw(val)		_wr32(_REG_BASE_+(0x023b<<2),(UINT)((_rd32(_REG_BASE_+(0x023b<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define IM_CGO0_DSr				(ValSft_R31(_rd32(_REG_BASE_+(0x023c<<2)))&BitMask_01)
#define IM_CGO0_DSw(val)		_wr32(_REG_BASE_+(0x023c<<2),(UINT)((_rd32(_REG_BASE_+(0x023c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_GO0_DSr				(ValSft_R30(_rd32(_REG_BASE_+(0x023c<<2)))&BitMask_01)
#define IM_GO0_DSw(val)			_wr32(_REG_BASE_+(0x023c<<2),(UINT)((_rd32(_REG_BASE_+(0x023c<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_CGO1_DSr				(ValSft_R31(_rd32(_REG_BASE_+(0x023d<<2)))&BitMask_01)
#define IM_CGO1_DSw(val)		_wr32(_REG_BASE_+(0x023d<<2),(UINT)((_rd32(_REG_BASE_+(0x023d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_GO1_DSr				(ValSft_R30(_rd32(_REG_BASE_+(0x023d<<2)))&BitMask_01)
#define IM_GO1_DSw(val)			_wr32(_REG_BASE_+(0x023d<<2),(UINT)((_rd32(_REG_BASE_+(0x023d<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_CGO2_DSr				(ValSft_R31(_rd32(_REG_BASE_+(0x023e<<2)))&BitMask_01)
#define IM_CGO2_DSw(val)		_wr32(_REG_BASE_+(0x023e<<2),(UINT)((_rd32(_REG_BASE_+(0x023e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_GO2_DSr				(ValSft_R30(_rd32(_REG_BASE_+(0x023e<<2)))&BitMask_01)
#define IM_GO2_DSw(val)			_wr32(_REG_BASE_+(0x023e<<2),(UINT)((_rd32(_REG_BASE_+(0x023e<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_CGO3_DSr				(ValSft_R31(_rd32(_REG_BASE_+(0x023f<<2)))&BitMask_01)
#define IM_CGO3_DSw(val)		_wr32(_REG_BASE_+(0x023f<<2),(UINT)((_rd32(_REG_BASE_+(0x023f<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IM_GO3_DSr				(ValSft_R30(_rd32(_REG_BASE_+(0x023f<<2)))&BitMask_01)
#define IM_GO3_DSw(val)			_wr32(_REG_BASE_+(0x023f<<2),(UINT)((_rd32(_REG_BASE_+(0x023f<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IM_HWI0_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x024c<<2)))&BitMask_13)
#define IM_HWI0_DSw(val)		_wr32(_REG_BASE_+(0x024c<<2),(UINT)((_rd32(_REG_BASE_+(0x024c<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_WHOFS0_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x024c<<2)))&BitMask_13)
#define IM_WHOFS0_DSw(val)		_wr32(_REG_BASE_+(0x024c<<2),(UINT)((_rd32(_REG_BASE_+(0x024c<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HWI1_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x024d<<2)))&BitMask_13)
#define IM_HWI1_DSw(val)		_wr32(_REG_BASE_+(0x024d<<2),(UINT)((_rd32(_REG_BASE_+(0x024d<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_WHOFS1_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x024d<<2)))&BitMask_13)
#define IM_WHOFS1_DSw(val)		_wr32(_REG_BASE_+(0x024d<<2),(UINT)((_rd32(_REG_BASE_+(0x024d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HWI2_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x024e<<2)))&BitMask_13)
#define IM_HWI2_DSw(val)		_wr32(_REG_BASE_+(0x024e<<2),(UINT)((_rd32(_REG_BASE_+(0x024e<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_WHOFS2_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x024e<<2)))&BitMask_13)
#define IM_WHOFS2_DSw(val)		_wr32(_REG_BASE_+(0x024e<<2),(UINT)((_rd32(_REG_BASE_+(0x024e<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HWI3_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x024f<<2)))&BitMask_13)
#define IM_HWI3_DSw(val)		_wr32(_REG_BASE_+(0x024f<<2),(UINT)((_rd32(_REG_BASE_+(0x024f<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_WHOFS3_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x024f<<2)))&BitMask_13)
#define IM_WHOFS3_DSw(val)		_wr32(_REG_BASE_+(0x024f<<2),(UINT)((_rd32(_REG_BASE_+(0x024f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DZ_DAONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_01)
#define DZ_DAONw(val)			_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DZ_DKXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_01)
#define DZ_DKXY_ONw(val)		_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DZ_FLP_ONr				(ValSft_R29(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_01)
#define DZ_FLP_ONw(val)			_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define DZ_FLP_OFSr				(ValSft_R24(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_05)
#define DZ_FLP_OFSw(val)		_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define DZ_YC2YCBCR_CINVr		(ValSft_R23(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_01)
#define DZ_YC2YCBCR_CINVw(val)	_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define DZ_CH_SELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_03)
#define DZ_CH_SELw(val)			_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define DZ_ZHLOCK_SELr			(ValSft_R16(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_03)
#define DZ_ZHLOCK_SELw(val)		_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define DZ_YCBCR2YC_CINVr		(ValSft_R15(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_01)
#define DZ_YCBCR2YC_CINVw(val)	_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define DZ_BUF_ASELr			(ValSft_R12(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_03)
#define DZ_BUF_ASELw(val)		_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L12(BitMask_03)))|ValSft_L12((val)&BitMask_03)))

#define DZ_VWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x0240<<2)))&BitMask_12)
#define DZ_VWOw(val)			_wr32(_REG_BASE_+(0x0240<<2),(UINT)((_rd32(_REG_BASE_+(0x0240<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DZ_MIRR_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0241<<2)))&BitMask_01)
#define DZ_MIRR_ONw(val)		_wr32(_REG_BASE_+(0x0241<<2),(UINT)((_rd32(_REG_BASE_+(0x0241<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DZ_MIRR_OFSr			(ValSft_R26(_rd32(_REG_BASE_+(0x0241<<2)))&BitMask_05)
#define DZ_MIRR_OFSw(val)		_wr32(_REG_BASE_+(0x0241<<2),(UINT)((_rd32(_REG_BASE_+(0x0241<<2))&(~ValSft_L26(BitMask_05)))|ValSft_L26((val)&BitMask_05)))

#define DZ_HSP_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0241<<2)))&BitMask_24)
#define DZ_HSP_POSw(val)		_wr32(_REG_BASE_+(0x0241<<2),(UINT)((_rd32(_REG_BASE_+(0x0241<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define DZ_VSP_POSr				(ValSft_R00(_rd32(_REG_BASE_+(0x0242<<2)))&BitMask_24)
#define DZ_VSP_POSw(val)		_wr32(_REG_BASE_+(0x0242<<2),(UINT)((_rd32(_REG_BASE_+(0x0242<<2))&(~ValSft_L00(BitMask_24)))|ValSft_L00((val)&BitMask_24)))

#define DZ_DZOOM_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_01)
#define DZ_DZOOM_ONw(val)		_wr32(_REG_BASE_+(0x0243<<2),(UINT)((_rd32(_REG_BASE_+(0x0243<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DZ_DALPHA0_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_01)
#define DZ_DALPHA0_ONw(val)		_wr32(_REG_BASE_+(0x0243<<2),(UINT)((_rd32(_REG_BASE_+(0x0243<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DZ_DALPHA1_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_01)
#define DZ_DALPHA1_ONw(val)		_wr32(_REG_BASE_+(0x0243<<2),(UINT)((_rd32(_REG_BASE_+(0x0243<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define DZ_VSP_KYr				(ValSft_R16(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_13)
#define DZ_VSP_KYw(val)			_wr32(_REG_BASE_+(0x0243<<2),(UINT)((_rd32(_REG_BASE_+(0x0243<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DZ_HSP_KXr				(ValSft_R00(_rd32(_REG_BASE_+(0x0243<<2)))&BitMask_13)
#define DZ_HSP_KXw(val)			_wr32(_REG_BASE_+(0x0243<<2),(UINT)((_rd32(_REG_BASE_+(0x0243<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DZOOM_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_12)
#define DZOOM_VSPw(val)			_wr32(_REG_BASE_+(0x0244<<2),(UINT)((_rd32(_REG_BASE_+(0x0244<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DZOOM_HSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x0244<<2)))&BitMask_13)
#define DZOOM_HSPw(val)			_wr32(_REG_BASE_+(0x0244<<2),(UINT)((_rd32(_REG_BASE_+(0x0244<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DZ_HWOr					(ValSft_R00(_rd32(_REG_BASE_+(0x0245<<2)))&BitMask_13)
#define DZ_HWOw(val)			_wr32(_REG_BASE_+(0x0245<<2),(UINT)((_rd32(_REG_BASE_+(0x0245<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_RHWI0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0246<<2)))&BitMask_13)
#define IM_RHWI0w(val)			_wr32(_REG_BASE_+(0x0246<<2),(UINT)((_rd32(_REG_BASE_+(0x0246<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_RHWI1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0246<<2)))&BitMask_13)
#define IM_RHWI1w(val)			_wr32(_REG_BASE_+(0x0246<<2),(UINT)((_rd32(_REG_BASE_+(0x0246<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_RHWI2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0247<<2)))&BitMask_13)
#define IM_RHWI2w(val)			_wr32(_REG_BASE_+(0x0247<<2),(UINT)((_rd32(_REG_BASE_+(0x0247<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_RHWI3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0247<<2)))&BitMask_13)
#define IM_RHWI3w(val)			_wr32(_REG_BASE_+(0x0247<<2),(UINT)((_rd32(_REG_BASE_+(0x0247<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_RHWI4r				(ValSft_R16(_rd32(_REG_BASE_+(0x0248<<2)))&BitMask_13)
#define IM_RHWI4w(val)			_wr32(_REG_BASE_+(0x0248<<2),(UINT)((_rd32(_REG_BASE_+(0x0248<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_HOFS0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0249<<2)))&BitMask_13)
#define IM_HOFS0w(val)			_wr32(_REG_BASE_+(0x0249<<2),(UINT)((_rd32(_REG_BASE_+(0x0249<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_HOFS1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0249<<2)))&BitMask_13)
#define IM_HOFS1w(val)			_wr32(_REG_BASE_+(0x0249<<2),(UINT)((_rd32(_REG_BASE_+(0x0249<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HOFS2r				(ValSft_R16(_rd32(_REG_BASE_+(0x024a<<2)))&BitMask_13)
#define IM_HOFS2w(val)			_wr32(_REG_BASE_+(0x024a<<2),(UINT)((_rd32(_REG_BASE_+(0x024a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IM_HOFS3r				(ValSft_R00(_rd32(_REG_BASE_+(0x024a<<2)))&BitMask_13)
#define IM_HOFS3w(val)			_wr32(_REG_BASE_+(0x024a<<2),(UINT)((_rd32(_REG_BASE_+(0x024a<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IM_HOFS4r				(ValSft_R16(_rd32(_REG_BASE_+(0x024b<<2)))&BitMask_13)
#define IM_HOFS4w(val)			_wr32(_REG_BASE_+(0x024b<<2),(UINT)((_rd32(_REG_BASE_+(0x024b<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SD_MODr					(ValSft_R29(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_03)
#define SD_MODw(val)			_wr32(_REG_BASE_+(0x0250<<2),(UINT)((_rd32(_REG_BASE_+(0x0250<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define CPU_FRC_ENr				(ValSft_R28(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_01)
#define CPU_FRC_ENw(val)		_wr32(_REG_BASE_+(0x0250<<2),(UINT)((_rd32(_REG_BASE_+(0x0250<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define BUS_RD_RSTr				(ValSft_R27(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_01)
#define BUS_RD_RSTw(val)		_wr32(_REG_BASE_+(0x0250<<2),(UINT)((_rd32(_REG_BASE_+(0x0250<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define WREADY_SELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_01)
#define WREADY_SELw(val)		_wr32(_REG_BASE_+(0x0250<<2),(UINT)((_rd32(_REG_BASE_+(0x0250<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define BUS_WR_RSTr				(ValSft_R00(_rd32(_REG_BASE_+(0x0250<<2)))&BitMask_18)
#define BUS_WR_RSTw(val)		_wr32(_REG_BASE_+(0x0250<<2),(UINT)((_rd32(_REG_BASE_+(0x0250<<2))&(~ValSft_L00(BitMask_18)))|ValSft_L00((val)&BitMask_18)))

#define WDR_ADR_LEr				(ValSft_R00(_rd32(_REG_BASE_+(0x0251<<2)))&BitMask_28)
#define WDR_ADR_LEw(val)		_wr32(_REG_BASE_+(0x0251<<2),(UINT)((_rd32(_REG_BASE_+(0x0251<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define FRC_ADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0252<<2)))&BitMask_28)
#define FRC_ADR0w(val)			_wr32(_REG_BASE_+(0x0252<<2),(UINT)((_rd32(_REG_BASE_+(0x0252<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define FRC_ADR1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0253<<2)))&BitMask_28)
#define FRC_ADR1w(val)			_wr32(_REG_BASE_+(0x0253<<2),(UINT)((_rd32(_REG_BASE_+(0x0253<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define FRC_ADR2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0254<<2)))&BitMask_28)
#define FRC_ADR2w(val)			_wr32(_REG_BASE_+(0x0254<<2),(UINT)((_rd32(_REG_BASE_+(0x0254<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define FRC_ADR3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0255<<2)))&BitMask_28)
#define FRC_ADR3w(val)			_wr32(_REG_BASE_+(0x0255<<2),(UINT)((_rd32(_REG_BASE_+(0x0255<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define FRC_ADR4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0256<<2)))&BitMask_28)
#define FRC_ADR4w(val)			_wr32(_REG_BASE_+(0x0256<<2),(UINT)((_rd32(_REG_BASE_+(0x0256<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define ENC_ADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0257<<2)))&BitMask_28)
#define ENC_ADR0w(val)			_wr32(_REG_BASE_+(0x0257<<2),(UINT)((_rd32(_REG_BASE_+(0x0257<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define ENC_ADR1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0258<<2)))&BitMask_28)
#define ENC_ADR1w(val)			_wr32(_REG_BASE_+(0x0258<<2),(UINT)((_rd32(_REG_BASE_+(0x0258<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define ENC_ADR2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0259<<2)))&BitMask_28)
#define ENC_ADR2w(val)			_wr32(_REG_BASE_+(0x0259<<2),(UINT)((_rd32(_REG_BASE_+(0x0259<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define ENC_ADR3r				(ValSft_R00(_rd32(_REG_BASE_+(0x025a<<2)))&BitMask_28)
#define ENC_ADR3w(val)			_wr32(_REG_BASE_+(0x025a<<2),(UINT)((_rd32(_REG_BASE_+(0x025a<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x025b<<2)))&BitMask_28)
#define IM_YADR0w(val)			_wr32(_REG_BASE_+(0x025b<<2),(UINT)((_rd32(_REG_BASE_+(0x025b<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR0r				(ValSft_R00(_rd32(_REG_BASE_+(0x025c<<2)))&BitMask_28)
#define IM_CADR0w(val)			_wr32(_REG_BASE_+(0x025c<<2),(UINT)((_rd32(_REG_BASE_+(0x025c<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR1_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x025d<<2)))&BitMask_28)
#define IM_YADR1_P0w(val)		_wr32(_REG_BASE_+(0x025d<<2),(UINT)((_rd32(_REG_BASE_+(0x025d<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR1_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x025e<<2)))&BitMask_28)
#define IM_CADR1_P0w(val)		_wr32(_REG_BASE_+(0x025e<<2),(UINT)((_rd32(_REG_BASE_+(0x025e<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR1_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x025f<<2)))&BitMask_28)
#define IM_YADR1_P1w(val)		_wr32(_REG_BASE_+(0x025f<<2),(UINT)((_rd32(_REG_BASE_+(0x025f<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR1_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0260<<2)))&BitMask_28)
#define IM_CADR1_P1w(val)		_wr32(_REG_BASE_+(0x0260<<2),(UINT)((_rd32(_REG_BASE_+(0x0260<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR1_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0261<<2)))&BitMask_28)
#define IM_YADR1_P2w(val)		_wr32(_REG_BASE_+(0x0261<<2),(UINT)((_rd32(_REG_BASE_+(0x0261<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR1_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0262<<2)))&BitMask_28)
#define IM_CADR1_P2w(val)		_wr32(_REG_BASE_+(0x0262<<2),(UINT)((_rd32(_REG_BASE_+(0x0262<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR2_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0263<<2)))&BitMask_28)
#define IM_YADR2_P0w(val)		_wr32(_REG_BASE_+(0x0263<<2),(UINT)((_rd32(_REG_BASE_+(0x0263<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR2_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0264<<2)))&BitMask_28)
#define IM_CADR2_P0w(val)		_wr32(_REG_BASE_+(0x0264<<2),(UINT)((_rd32(_REG_BASE_+(0x0264<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR2_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0265<<2)))&BitMask_28)
#define IM_YADR2_P1w(val)		_wr32(_REG_BASE_+(0x0265<<2),(UINT)((_rd32(_REG_BASE_+(0x0265<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR2_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0266<<2)))&BitMask_28)
#define IM_CADR2_P1w(val)		_wr32(_REG_BASE_+(0x0266<<2),(UINT)((_rd32(_REG_BASE_+(0x0266<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR2_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0267<<2)))&BitMask_28)
#define IM_YADR2_P2w(val)		_wr32(_REG_BASE_+(0x0267<<2),(UINT)((_rd32(_REG_BASE_+(0x0267<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR2_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0268<<2)))&BitMask_28)
#define IM_CADR2_P2w(val)		_wr32(_REG_BASE_+(0x0268<<2),(UINT)((_rd32(_REG_BASE_+(0x0268<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR3_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0269<<2)))&BitMask_28)
#define IM_YADR3_P0w(val)		_wr32(_REG_BASE_+(0x0269<<2),(UINT)((_rd32(_REG_BASE_+(0x0269<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR3_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x026a<<2)))&BitMask_28)
#define IM_CADR3_P0w(val)		_wr32(_REG_BASE_+(0x026a<<2),(UINT)((_rd32(_REG_BASE_+(0x026a<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR3_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x026b<<2)))&BitMask_28)
#define IM_YADR3_P1w(val)		_wr32(_REG_BASE_+(0x026b<<2),(UINT)((_rd32(_REG_BASE_+(0x026b<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR3_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x026c<<2)))&BitMask_28)
#define IM_CADR3_P1w(val)		_wr32(_REG_BASE_+(0x026c<<2),(UINT)((_rd32(_REG_BASE_+(0x026c<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR3_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x026d<<2)))&BitMask_28)
#define IM_YADR3_P2w(val)		_wr32(_REG_BASE_+(0x026d<<2),(UINT)((_rd32(_REG_BASE_+(0x026d<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR3_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x026e<<2)))&BitMask_28)
#define IM_CADR3_P2w(val)		_wr32(_REG_BASE_+(0x026e<<2),(UINT)((_rd32(_REG_BASE_+(0x026e<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR4_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x026f<<2)))&BitMask_28)
#define IM_YADR4_P0w(val)		_wr32(_REG_BASE_+(0x026f<<2),(UINT)((_rd32(_REG_BASE_+(0x026f<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR4_P0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0270<<2)))&BitMask_28)
#define IM_CADR4_P0w(val)		_wr32(_REG_BASE_+(0x0270<<2),(UINT)((_rd32(_REG_BASE_+(0x0270<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR4_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0271<<2)))&BitMask_28)
#define IM_YADR4_P1w(val)		_wr32(_REG_BASE_+(0x0271<<2),(UINT)((_rd32(_REG_BASE_+(0x0271<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR4_P1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0272<<2)))&BitMask_28)
#define IM_CADR4_P1w(val)		_wr32(_REG_BASE_+(0x0272<<2),(UINT)((_rd32(_REG_BASE_+(0x0272<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR4_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0273<<2)))&BitMask_28)
#define IM_YADR4_P2w(val)		_wr32(_REG_BASE_+(0x0273<<2),(UINT)((_rd32(_REG_BASE_+(0x0273<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR4_P2r			(ValSft_R00(_rd32(_REG_BASE_+(0x0274<<2)))&BitMask_28)
#define IM_CADR4_P2w(val)		_wr32(_REG_BASE_+(0x0274<<2),(UINT)((_rd32(_REG_BASE_+(0x0274<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define DDR_RDNR_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x0276<<2)))&BitMask_13)
#define DDR_RDNR_LTCw(val)		_wr32(_REG_BASE_+(0x0276<<2),(UINT)((_rd32(_REG_BASE_+(0x0276<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_RWDR_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x0276<<2)))&BitMask_13)
#define DDR_RWDR_LTCw(val)		_wr32(_REG_BASE_+(0x0276<<2),(UINT)((_rd32(_REG_BASE_+(0x0276<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DDR_RFRC_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x0277<<2)))&BitMask_13)
#define DDR_RFRC_LTCw(val)		_wr32(_REG_BASE_+(0x0277<<2),(UINT)((_rd32(_REG_BASE_+(0x0277<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_RENC_LTCr			(ValSft_R00(_rd32(_REG_BASE_+(0x0277<<2)))&BitMask_12)
#define DDR_RENC_LTCw(val)		_wr32(_REG_BASE_+(0x0277<<2),(UINT)((_rd32(_REG_BASE_+(0x0277<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DDR_RYC_LTCr			(ValSft_R16(_rd32(_REG_BASE_+(0x0278<<2)))&BitMask_13)
#define DDR_RYC_LTCw(val)		_wr32(_REG_BASE_+(0x0278<<2),(UINT)((_rd32(_REG_BASE_+(0x0278<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_RYC_LTC1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0278<<2)))&BitMask_13)
#define DDR_RYC_LTC1w(val)		_wr32(_REG_BASE_+(0x0278<<2),(UINT)((_rd32(_REG_BASE_+(0x0278<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DDR_RYC_LTC2r			(ValSft_R16(_rd32(_REG_BASE_+(0x0279<<2)))&BitMask_13)
#define DDR_RYC_LTC2w(val)		_wr32(_REG_BASE_+(0x0279<<2),(UINT)((_rd32(_REG_BASE_+(0x0279<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_RYC_LTC3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0279<<2)))&BitMask_13)
#define DDR_RYC_LTC3w(val)		_wr32(_REG_BASE_+(0x0279<<2),(UINT)((_rd32(_REG_BASE_+(0x0279<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define THM_TMP_WONr			(ValSft_R31(_rd32(_REG_BASE_+(0x027b<<2)))&BitMask_01)
#define THM_TMP_WONw(val)		_wr32(_REG_BASE_+(0x027b<<2),(UINT)((_rd32(_REG_BASE_+(0x027b<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define THM_HW_HALFr			(ValSft_R30(_rd32(_REG_BASE_+(0x027b<<2)))&BitMask_01)
#define THM_HW_HALFw(val)		_wr32(_REG_BASE_+(0x027b<<2),(UINT)((_rd32(_REG_BASE_+(0x027b<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define THM_DDR_WENr			(ValSft_R29(_rd32(_REG_BASE_+(0x027b<<2)))&BitMask_01)
#define THM_DDR_WENw(val)		_wr32(_REG_BASE_+(0x027b<<2),(UINT)((_rd32(_REG_BASE_+(0x027b<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define THM_ADRr				(ValSft_R00(_rd32(_REG_BASE_+(0x027b<<2)))&BitMask_28)
#define THM_ADRw(val)			_wr32(_REG_BASE_+(0x027b<<2),(UINT)((_rd32(_REG_BASE_+(0x027b<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define RD_LTC7r				(ValSft_R16(_rd32(_REG_BASE_+(0x027c<<2)))&BitMask_09)
#define RD_LTC7w(val)			_wr32(_REG_BASE_+(0x027c<<2),(UINT)((_rd32(_REG_BASE_+(0x027c<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define RD_LTC8r				(ValSft_R00(_rd32(_REG_BASE_+(0x027c<<2)))&BitMask_09)
#define RD_LTC8w(val)			_wr32(_REG_BASE_+(0x027c<<2),(UINT)((_rd32(_REG_BASE_+(0x027c<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define RD_LTC9r				(ValSft_R16(_rd32(_REG_BASE_+(0x027d<<2)))&BitMask_09)
#define RD_LTC9w(val)			_wr32(_REG_BASE_+(0x027d<<2),(UINT)((_rd32(_REG_BASE_+(0x027d<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define RD_LTC10r				(ValSft_R00(_rd32(_REG_BASE_+(0x027d<<2)))&BitMask_09)
#define RD_LTC10w(val)			_wr32(_REG_BASE_+(0x027d<<2),(UINT)((_rd32(_REG_BASE_+(0x027d<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define RD_LTC11r				(ValSft_R16(_rd32(_REG_BASE_+(0x027e<<2)))&BitMask_09)
#define RD_LTC11w(val)			_wr32(_REG_BASE_+(0x027e<<2),(UINT)((_rd32(_REG_BASE_+(0x027e<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define RD_LTC12r				(ValSft_R00(_rd32(_REG_BASE_+(0x027e<<2)))&BitMask_09)
#define RD_LTC12w(val)			_wr32(_REG_BASE_+(0x027e<<2),(UINT)((_rd32(_REG_BASE_+(0x027e<<2))&(~ValSft_L00(BitMask_09)))|ValSft_L00((val)&BitMask_09)))

#define LAYER_RONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_01)
#define LAYER_RONw(val)			_wr32(_REG_BASE_+(0x0280<<2),(UINT)((_rd32(_REG_BASE_+(0x0280<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LAYER_ALPHA_ONr			(ValSft_R30(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_01)
#define LAYER_ALPHA_ONw(val)	_wr32(_REG_BASE_+(0x0280<<2),(UINT)((_rd32(_REG_BASE_+(0x0280<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LAYER_RD_ENr			(ValSft_R29(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_01)
#define LAYER_RD_ENw(val)		_wr32(_REG_BASE_+(0x0280<<2),(UINT)((_rd32(_REG_BASE_+(0x0280<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define LAYER_BIGENDIANr		(ValSft_R28(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_01)
#define LAYER_BIGENDIANw(val)	_wr32(_REG_BASE_+(0x0280<<2),(UINT)((_rd32(_REG_BASE_+(0x0280<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define LAYER_ADRr				(ValSft_R00(_rd32(_REG_BASE_+(0x0280<<2)))&BitMask_28)
#define LAYER_ADRw(val)			_wr32(_REG_BASE_+(0x0280<<2),(UINT)((_rd32(_REG_BASE_+(0x0280<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define ACT_LAYER_RSELr			(ValSft_R24(_rd32(_REG_BASE_+(0x0281<<2)))&BitMask_03)
#define ACT_LAYER_RSELw(val)	_wr32(_REG_BASE_+(0x0281<<2),(UINT)((_rd32(_REG_BASE_+(0x0281<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define LAYER_RWHIr				(ValSft_R00(_rd32(_REG_BASE_+(0x0281<<2)))&BitMask_13)
#define LAYER_RWHIw(val)		_wr32(_REG_BASE_+(0x0281<<2),(UINT)((_rd32(_REG_BASE_+(0x0281<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define LAYER_COLORKEY_ONr		(ValSft_R31(_rd32(_REG_BASE_+(0x0282<<2)))&BitMask_01)
#define LAYER_COLORKEY_ONw(val)	_wr32(_REG_BASE_+(0x0282<<2),(UINT)((_rd32(_REG_BASE_+(0x0282<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LAYER_COLORKEYr			(ValSft_R00(_rd32(_REG_BASE_+(0x0282<<2)))&BitMask_16)
#define LAYER_COLORKEYw(val)	_wr32(_REG_BASE_+(0x0282<<2),(UINT)((_rd32(_REG_BASE_+(0x0282<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define LAYER_CINVr				(ValSft_R08(_rd32(_REG_BASE_+(0x0283<<2)))&BitMask_01)
#define LAYER_CINVw(val)		_wr32(_REG_BASE_+(0x0283<<2),(UINT)((_rd32(_REG_BASE_+(0x0283<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define LAYER_ALPHAr			(ValSft_R00(_rd32(_REG_BASE_+(0x0283<<2)))&BitMask_08)
#define LAYER_ALPHAw(val)		_wr32(_REG_BASE_+(0x0283<<2),(UINT)((_rd32(_REG_BASE_+(0x0283<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LAYER_LENDKEY_ENr		(ValSft_R31(_rd32(_REG_BASE_+(0x0283<<2)))&BitMask_00)
#define LAYER_LENDKEY_ENw(val)	_wr32(_REG_BASE_+(0x0283<<2),(UINT)((_rd32(_REG_BASE_+(0x0283<<2))&(~ValSft_L31(BitMask_00)))|ValSft_L31((val)&BitMask_00)))

#define LAYER_LENDKEYr			(ValSft_R00(_rd32(_REG_BASE_+(0x0283<<2)))&BitMask_00)
#define LAYER_LENDKEYw(val)		_wr32(_REG_BASE_+(0x0283<<2),(UINT)((_rd32(_REG_BASE_+(0x0283<<2))&(~ValSft_L00(BitMask_00)))|ValSft_L00((val)&BitMask_00)))

#define LAYER_PENDKEY_ENr		(ValSft_R31(_rd32(_REG_BASE_+(0x0284<<2)))&BitMask_00)
#define LAYER_PENDKEY_ENw(val)	_wr32(_REG_BASE_+(0x0284<<2),(UINT)((_rd32(_REG_BASE_+(0x0284<<2))&(~ValSft_L31(BitMask_00)))|ValSft_L31((val)&BitMask_00)))

#define LAYER_PENDKEYr			(ValSft_R00(_rd32(_REG_BASE_+(0x0284<<2)))&BitMask_00)
#define LAYER_PENDKEYw(val)		_wr32(_REG_BASE_+(0x0284<<2),(UINT)((_rd32(_REG_BASE_+(0x0284<<2))&(~ValSft_L00(BitMask_00)))|ValSft_L00((val)&BitMask_00)))

#define LAYER_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x0284<<2)))&BitMask_12)
#define LAYER_VSPw(val)			_wr32(_REG_BASE_+(0x0284<<2),(UINT)((_rd32(_REG_BASE_+(0x0284<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define LAYER_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0284<<2)))&BitMask_12)
#define LAYER_VWw(val)			_wr32(_REG_BASE_+(0x0284<<2),(UINT)((_rd32(_REG_BASE_+(0x0284<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define LAYER_ADR_OFSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0285<<2)))&BitMask_15)
#define LAYER_ADR_OFSw(val)		_wr32(_REG_BASE_+(0x0285<<2),(UINT)((_rd32(_REG_BASE_+(0x0285<<2))&(~ValSft_L00(BitMask_15)))|ValSft_L00((val)&BitMask_15)))

#define IM_YADR0_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0287<<2)))&BitMask_28)
#define IM_YADR0_DSw(val)		_wr32(_REG_BASE_+(0x0287<<2),(UINT)((_rd32(_REG_BASE_+(0x0287<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR0_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0288<<2)))&BitMask_28)
#define IM_CADR0_DSw(val)		_wr32(_REG_BASE_+(0x0288<<2),(UINT)((_rd32(_REG_BASE_+(0x0288<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR1_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0289<<2)))&BitMask_28)
#define IM_YADR1_DSw(val)		_wr32(_REG_BASE_+(0x0289<<2),(UINT)((_rd32(_REG_BASE_+(0x0289<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR1_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x028a<<2)))&BitMask_28)
#define IM_CADR1_DSw(val)		_wr32(_REG_BASE_+(0x028a<<2),(UINT)((_rd32(_REG_BASE_+(0x028a<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR2_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x028b<<2)))&BitMask_28)
#define IM_YADR2_DSw(val)		_wr32(_REG_BASE_+(0x028b<<2),(UINT)((_rd32(_REG_BASE_+(0x028b<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR2_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x028c<<2)))&BitMask_28)
#define IM_CADR2_DSw(val)		_wr32(_REG_BASE_+(0x028c<<2),(UINT)((_rd32(_REG_BASE_+(0x028c<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_YADR3_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x028d<<2)))&BitMask_28)
#define IM_YADR3_DSw(val)		_wr32(_REG_BASE_+(0x028d<<2),(UINT)((_rd32(_REG_BASE_+(0x028d<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define IM_CADR3_DSr			(ValSft_R00(_rd32(_REG_BASE_+(0x028e<<2)))&BitMask_28)
#define IM_CADR3_DSw(val)		_wr32(_REG_BASE_+(0x028e<<2),(UINT)((_rd32(_REG_BASE_+(0x028e<<2))&(~ValSft_L00(BitMask_28)))|ValSft_L00((val)&BitMask_28)))

#define CODEC_DS_STEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x028f<<2)))&BitMask_13)
#define CODEC_DS_STEPw(val)		_wr32(_REG_BASE_+(0x028f<<2),(UINT)((_rd32(_REG_BASE_+(0x028f<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DS0_HLPFr				(ValSft_R30(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_02)
#define DS0_HLPFw(val)			_wr32(_REG_BASE_+(0x0290<<2),(UINT)((_rd32(_REG_BASE_+(0x0290<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DS0_VLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_02)
#define DS0_VLPFw(val)			_wr32(_REG_BASE_+(0x0290<<2),(UINT)((_rd32(_REG_BASE_+(0x0290<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define CDS0_ISELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_05)
#define CDS0_ISELw(val)			_wr32(_REG_BASE_+(0x0290<<2),(UINT)((_rd32(_REG_BASE_+(0x0290<<2))&(~ValSft_L22(BitMask_05)))|ValSft_L22((val)&BitMask_05)))

#define DS0_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_10)
#define DS0_DKXw(val)			_wr32(_REG_BASE_+(0x0290<<2),(UINT)((_rd32(_REG_BASE_+(0x0290<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS0_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x0290<<2)))&BitMask_10)
#define DS0_DKYw(val)			_wr32(_REG_BASE_+(0x0290<<2),(UINT)((_rd32(_REG_BASE_+(0x0290<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS0_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0291<<2)))&BitMask_01)
#define DS0_ONw(val)			_wr32(_REG_BASE_+(0x0291<<2),(UINT)((_rd32(_REG_BASE_+(0x0291<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS0_DKY_STr				(ValSft_R25(_rd32(_REG_BASE_+(0x0291<<2)))&BitMask_06)
#define DS0_DKY_STw(val)		_wr32(_REG_BASE_+(0x0291<<2),(UINT)((_rd32(_REG_BASE_+(0x0291<<2))&(~ValSft_L25(BitMask_06)))|ValSft_L25((val)&BitMask_06)))

#define DS0_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x0291<<2)))&BitMask_13)
#define DS0_HOFFw(val)			_wr32(_REG_BASE_+(0x0291<<2),(UINT)((_rd32(_REG_BASE_+(0x0291<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS0_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0291<<2)))&BitMask_12)
#define DS0_VOFFw(val)			_wr32(_REG_BASE_+(0x0291<<2),(UINT)((_rd32(_REG_BASE_+(0x0291<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS0_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_01)
#define DS0_APTONw(val)			_wr32(_REG_BASE_+(0x0292<<2),(UINT)((_rd32(_REG_BASE_+(0x0292<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS0_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_01)
#define DS0_KXY_ONw(val)		_wr32(_REG_BASE_+(0x0292<<2),(UINT)((_rd32(_REG_BASE_+(0x0292<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS0_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_08)
#define DS0_APTGAw(val)			_wr32(_REG_BASE_+(0x0292<<2),(UINT)((_rd32(_REG_BASE_+(0x0292<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS0_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_08)
#define DS0_APTSLw(val)			_wr32(_REG_BASE_+(0x0292<<2),(UINT)((_rd32(_REG_BASE_+(0x0292<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS0_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x0292<<2)))&BitMask_08)
#define DS0_APTCLw(val)			_wr32(_REG_BASE_+(0x0292<<2),(UINT)((_rd32(_REG_BASE_+(0x0292<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS0_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0293<<2)))&BitMask_04)
#define CDS0_VISELw(val)		_wr32(_REG_BASE_+(0x0293<<2),(UINT)((_rd32(_REG_BASE_+(0x0293<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define CDS0_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0293<<2)))&BitMask_12)
#define CDS0_HWw(val)			_wr32(_REG_BASE_+(0x0293<<2),(UINT)((_rd32(_REG_BASE_+(0x0293<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CDS0_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0293<<2)))&BitMask_12)
#define CDS0_VWw(val)			_wr32(_REG_BASE_+(0x0293<<2),(UINT)((_rd32(_REG_BASE_+(0x0293<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_HLPFr				(ValSft_R30(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_02)
#define DS1_HLPFw(val)			_wr32(_REG_BASE_+(0x0294<<2),(UINT)((_rd32(_REG_BASE_+(0x0294<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DS1_VLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_02)
#define DS1_VLPFw(val)			_wr32(_REG_BASE_+(0x0294<<2),(UINT)((_rd32(_REG_BASE_+(0x0294<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define CDS1_ISELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_05)
#define CDS1_ISELw(val)			_wr32(_REG_BASE_+(0x0294<<2),(UINT)((_rd32(_REG_BASE_+(0x0294<<2))&(~ValSft_L22(BitMask_05)))|ValSft_L22((val)&BitMask_05)))

#define DS1_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_10)
#define DS1_DKXw(val)			_wr32(_REG_BASE_+(0x0294<<2),(UINT)((_rd32(_REG_BASE_+(0x0294<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS1_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x0294<<2)))&BitMask_10)
#define DS1_DKYw(val)			_wr32(_REG_BASE_+(0x0294<<2),(UINT)((_rd32(_REG_BASE_+(0x0294<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS1_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0295<<2)))&BitMask_01)
#define DS1_ONw(val)			_wr32(_REG_BASE_+(0x0295<<2),(UINT)((_rd32(_REG_BASE_+(0x0295<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS1_DKY_STr				(ValSft_R25(_rd32(_REG_BASE_+(0x0295<<2)))&BitMask_06)
#define DS1_DKY_STw(val)		_wr32(_REG_BASE_+(0x0295<<2),(UINT)((_rd32(_REG_BASE_+(0x0295<<2))&(~ValSft_L25(BitMask_06)))|ValSft_L25((val)&BitMask_06)))

#define DS1_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x0295<<2)))&BitMask_13)
#define DS1_HOFFw(val)			_wr32(_REG_BASE_+(0x0295<<2),(UINT)((_rd32(_REG_BASE_+(0x0295<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS1_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0295<<2)))&BitMask_12)
#define DS1_VOFFw(val)			_wr32(_REG_BASE_+(0x0295<<2),(UINT)((_rd32(_REG_BASE_+(0x0295<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_01)
#define DS1_APTONw(val)			_wr32(_REG_BASE_+(0x0296<<2),(UINT)((_rd32(_REG_BASE_+(0x0296<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS1_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_01)
#define DS1_KXY_ONw(val)		_wr32(_REG_BASE_+(0x0296<<2),(UINT)((_rd32(_REG_BASE_+(0x0296<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS1_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_08)
#define DS1_APTGAw(val)			_wr32(_REG_BASE_+(0x0296<<2),(UINT)((_rd32(_REG_BASE_+(0x0296<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS1_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_08)
#define DS1_APTSLw(val)			_wr32(_REG_BASE_+(0x0296<<2),(UINT)((_rd32(_REG_BASE_+(0x0296<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS1_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x0296<<2)))&BitMask_08)
#define DS1_APTCLw(val)			_wr32(_REG_BASE_+(0x0296<<2),(UINT)((_rd32(_REG_BASE_+(0x0296<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS1_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x0297<<2)))&BitMask_04)
#define CDS1_VISELw(val)		_wr32(_REG_BASE_+(0x0297<<2),(UINT)((_rd32(_REG_BASE_+(0x0297<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define CDS1_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x0297<<2)))&BitMask_12)
#define CDS1_HWw(val)			_wr32(_REG_BASE_+(0x0297<<2),(UINT)((_rd32(_REG_BASE_+(0x0297<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CDS1_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x0297<<2)))&BitMask_12)
#define CDS1_VWw(val)			_wr32(_REG_BASE_+(0x0297<<2),(UINT)((_rd32(_REG_BASE_+(0x0297<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS2_HLPFr				(ValSft_R30(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_02)
#define DS2_HLPFw(val)			_wr32(_REG_BASE_+(0x0298<<2),(UINT)((_rd32(_REG_BASE_+(0x0298<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DS2_VLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_02)
#define DS2_VLPFw(val)			_wr32(_REG_BASE_+(0x0298<<2),(UINT)((_rd32(_REG_BASE_+(0x0298<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define CDS2_ISELr				(ValSft_R22(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_05)
#define CDS2_ISELw(val)			_wr32(_REG_BASE_+(0x0298<<2),(UINT)((_rd32(_REG_BASE_+(0x0298<<2))&(~ValSft_L22(BitMask_05)))|ValSft_L22((val)&BitMask_05)))

#define DS2_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_10)
#define DS2_DKXw(val)			_wr32(_REG_BASE_+(0x0298<<2),(UINT)((_rd32(_REG_BASE_+(0x0298<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS2_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x0298<<2)))&BitMask_10)
#define DS2_DKYw(val)			_wr32(_REG_BASE_+(0x0298<<2),(UINT)((_rd32(_REG_BASE_+(0x0298<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS2_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x0299<<2)))&BitMask_01)
#define DS2_ONw(val)			_wr32(_REG_BASE_+(0x0299<<2),(UINT)((_rd32(_REG_BASE_+(0x0299<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS2_DKY_STr				(ValSft_R25(_rd32(_REG_BASE_+(0x0299<<2)))&BitMask_06)
#define DS2_DKY_STw(val)		_wr32(_REG_BASE_+(0x0299<<2),(UINT)((_rd32(_REG_BASE_+(0x0299<<2))&(~ValSft_L25(BitMask_06)))|ValSft_L25((val)&BitMask_06)))

#define DS2_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x0299<<2)))&BitMask_13)
#define DS2_HOFFw(val)			_wr32(_REG_BASE_+(0x0299<<2),(UINT)((_rd32(_REG_BASE_+(0x0299<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS2_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x0299<<2)))&BitMask_12)
#define DS2_VOFFw(val)			_wr32(_REG_BASE_+(0x0299<<2),(UINT)((_rd32(_REG_BASE_+(0x0299<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS2_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_01)
#define DS2_APTONw(val)			_wr32(_REG_BASE_+(0x029a<<2),(UINT)((_rd32(_REG_BASE_+(0x029a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS2_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_01)
#define DS2_KXY_ONw(val)		_wr32(_REG_BASE_+(0x029a<<2),(UINT)((_rd32(_REG_BASE_+(0x029a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS2_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_08)
#define DS2_APTGAw(val)			_wr32(_REG_BASE_+(0x029a<<2),(UINT)((_rd32(_REG_BASE_+(0x029a<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS2_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_08)
#define DS2_APTSLw(val)			_wr32(_REG_BASE_+(0x029a<<2),(UINT)((_rd32(_REG_BASE_+(0x029a<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS2_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x029a<<2)))&BitMask_08)
#define DS2_APTCLw(val)			_wr32(_REG_BASE_+(0x029a<<2),(UINT)((_rd32(_REG_BASE_+(0x029a<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS2_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x029b<<2)))&BitMask_04)
#define CDS2_VISELw(val)		_wr32(_REG_BASE_+(0x029b<<2),(UINT)((_rd32(_REG_BASE_+(0x029b<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define CDS2_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x029b<<2)))&BitMask_12)
#define CDS2_HWw(val)			_wr32(_REG_BASE_+(0x029b<<2),(UINT)((_rd32(_REG_BASE_+(0x029b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CDS2_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x029b<<2)))&BitMask_12)
#define CDS2_VWw(val)			_wr32(_REG_BASE_+(0x029b<<2),(UINT)((_rd32(_REG_BASE_+(0x029b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS3_HLPFr				(ValSft_R30(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_02)
#define DS3_HLPFw(val)			_wr32(_REG_BASE_+(0x029c<<2),(UINT)((_rd32(_REG_BASE_+(0x029c<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DS3_VLPFr				(ValSft_R28(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_02)
#define DS3_VLPFw(val)			_wr32(_REG_BASE_+(0x029c<<2),(UINT)((_rd32(_REG_BASE_+(0x029c<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define CDS3_ISELr				(ValSft_R22(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_05)
#define CDS3_ISELw(val)			_wr32(_REG_BASE_+(0x029c<<2),(UINT)((_rd32(_REG_BASE_+(0x029c<<2))&(~ValSft_L22(BitMask_05)))|ValSft_L22((val)&BitMask_05)))

#define DS3_DKXr				(ValSft_R10(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_10)
#define DS3_DKXw(val)			_wr32(_REG_BASE_+(0x029c<<2),(UINT)((_rd32(_REG_BASE_+(0x029c<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define DS3_DKYr				(ValSft_R00(_rd32(_REG_BASE_+(0x029c<<2)))&BitMask_10)
#define DS3_DKYw(val)			_wr32(_REG_BASE_+(0x029c<<2),(UINT)((_rd32(_REG_BASE_+(0x029c<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define DS3_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x029d<<2)))&BitMask_01)
#define DS3_ONw(val)			_wr32(_REG_BASE_+(0x029d<<2),(UINT)((_rd32(_REG_BASE_+(0x029d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS3_DKY_STr				(ValSft_R25(_rd32(_REG_BASE_+(0x029d<<2)))&BitMask_06)
#define DS3_DKY_STw(val)		_wr32(_REG_BASE_+(0x029d<<2),(UINT)((_rd32(_REG_BASE_+(0x029d<<2))&(~ValSft_L25(BitMask_06)))|ValSft_L25((val)&BitMask_06)))

#define DS3_HOFFr				(ValSft_R12(_rd32(_REG_BASE_+(0x029d<<2)))&BitMask_13)
#define DS3_HOFFw(val)			_wr32(_REG_BASE_+(0x029d<<2),(UINT)((_rd32(_REG_BASE_+(0x029d<<2))&(~ValSft_L12(BitMask_13)))|ValSft_L12((val)&BitMask_13)))

#define DS3_VOFFr				(ValSft_R00(_rd32(_REG_BASE_+(0x029d<<2)))&BitMask_12)
#define DS3_VOFFw(val)			_wr32(_REG_BASE_+(0x029d<<2),(UINT)((_rd32(_REG_BASE_+(0x029d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS3_APTONr				(ValSft_R31(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_01)
#define DS3_APTONw(val)			_wr32(_REG_BASE_+(0x029e<<2),(UINT)((_rd32(_REG_BASE_+(0x029e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS3_KXY_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_01)
#define DS3_KXY_ONw(val)		_wr32(_REG_BASE_+(0x029e<<2),(UINT)((_rd32(_REG_BASE_+(0x029e<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS3_APTGAr				(ValSft_R16(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_08)
#define DS3_APTGAw(val)			_wr32(_REG_BASE_+(0x029e<<2),(UINT)((_rd32(_REG_BASE_+(0x029e<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define DS3_APTSLr				(ValSft_R08(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_08)
#define DS3_APTSLw(val)			_wr32(_REG_BASE_+(0x029e<<2),(UINT)((_rd32(_REG_BASE_+(0x029e<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define DS3_APTCLr				(ValSft_R00(_rd32(_REG_BASE_+(0x029e<<2)))&BitMask_08)
#define DS3_APTCLw(val)			_wr32(_REG_BASE_+(0x029e<<2),(UINT)((_rd32(_REG_BASE_+(0x029e<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CDS3_VISELr				(ValSft_R28(_rd32(_REG_BASE_+(0x029f<<2)))&BitMask_04)
#define CDS3_VISELw(val)		_wr32(_REG_BASE_+(0x029f<<2),(UINT)((_rd32(_REG_BASE_+(0x029f<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define CDS3_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x029f<<2)))&BitMask_12)
#define CDS3_HWw(val)			_wr32(_REG_BASE_+(0x029f<<2),(UINT)((_rd32(_REG_BASE_+(0x029f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CDS3_VWr				(ValSft_R00(_rd32(_REG_BASE_+(0x029f<<2)))&BitMask_12)
#define CDS3_VWw(val)			_wr32(_REG_BASE_+(0x029f<<2),(UINT)((_rd32(_REG_BASE_+(0x029f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define VWO_DS_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_01)
#define VWO_DS_ONw(val)			_wr32(_REG_BASE_+(0x02a0<<2),(UINT)((_rd32(_REG_BASE_+(0x02a0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VWO_DSr					(ValSft_R16(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_12)
#define VWO_DSw(val)			_wr32(_REG_BASE_+(0x02a0<<2),(UINT)((_rd32(_REG_BASE_+(0x02a0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWI_DS_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_01)
#define VWI_DS_ONw(val)			_wr32(_REG_BASE_+(0x02a0<<2),(UINT)((_rd32(_REG_BASE_+(0x02a0<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define VWI_DSr					(ValSft_R00(_rd32(_REG_BASE_+(0x02a0<<2)))&BitMask_12)
#define VWI_DSw(val)			_wr32(_REG_BASE_+(0x02a0<<2),(UINT)((_rd32(_REG_BASE_+(0x02a0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define VWI0_DS_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x02a1<<2)))&BitMask_01)
#define VWI0_DS_ONw(val)		_wr32(_REG_BASE_+(0x02a1<<2),(UINT)((_rd32(_REG_BASE_+(0x02a1<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VWI0_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a1<<2)))&BitMask_12)
#define VWI0_DSw(val)			_wr32(_REG_BASE_+(0x02a1<<2),(UINT)((_rd32(_REG_BASE_+(0x02a1<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define VWI1_DS_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x02a1<<2)))&BitMask_01)
#define VWI1_DS_ONw(val)		_wr32(_REG_BASE_+(0x02a1<<2),(UINT)((_rd32(_REG_BASE_+(0x02a1<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define VWI1_DSr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a1<<2)))&BitMask_12)
#define VWI1_DSw(val)			_wr32(_REG_BASE_+(0x02a1<<2),(UINT)((_rd32(_REG_BASE_+(0x02a1<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define VWI2_DS_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x02a2<<2)))&BitMask_01)
#define VWI2_DS_ONw(val)		_wr32(_REG_BASE_+(0x02a2<<2),(UINT)((_rd32(_REG_BASE_+(0x02a2<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VWI2_DSr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a2<<2)))&BitMask_12)
#define VWI2_DSw(val)			_wr32(_REG_BASE_+(0x02a2<<2),(UINT)((_rd32(_REG_BASE_+(0x02a2<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS0_RAPT_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x02a4<<2)))&BitMask_01)
#define DS0_RAPT_ONw(val)		_wr32(_REG_BASE_+(0x02a4<<2),(UINT)((_rd32(_REG_BASE_+(0x02a4<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS0_RAPT_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x02a4<<2)))&BitMask_01)
#define DS0_RAPT_OSD_ONw(val)	_wr32(_REG_BASE_+(0x02a4<<2),(UINT)((_rd32(_REG_BASE_+(0x02a4<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS0_APTGA_Rr			(ValSft_R00(_rd32(_REG_BASE_+(0x02a4<<2)))&BitMask_08)
#define DS0_APTGA_Rw(val)		_wr32(_REG_BASE_+(0x02a4<<2),(UINT)((_rd32(_REG_BASE_+(0x02a4<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DS0_APTHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a5<<2)))&BitMask_12)
#define DS0_APTHSPw(val)		_wr32(_REG_BASE_+(0x02a5<<2),(UINT)((_rd32(_REG_BASE_+(0x02a5<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS0_APTHEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a5<<2)))&BitMask_12)
#define DS0_APTHEDw(val)		_wr32(_REG_BASE_+(0x02a5<<2),(UINT)((_rd32(_REG_BASE_+(0x02a5<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS0_APTVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a6<<2)))&BitMask_12)
#define DS0_APTVSPw(val)		_wr32(_REG_BASE_+(0x02a6<<2),(UINT)((_rd32(_REG_BASE_+(0x02a6<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS0_APTVEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a6<<2)))&BitMask_12)
#define DS0_APTVEDw(val)		_wr32(_REG_BASE_+(0x02a6<<2),(UINT)((_rd32(_REG_BASE_+(0x02a6<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_RAPT_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x02a7<<2)))&BitMask_01)
#define DS1_RAPT_ONw(val)		_wr32(_REG_BASE_+(0x02a7<<2),(UINT)((_rd32(_REG_BASE_+(0x02a7<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS1_RAPT_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x02a7<<2)))&BitMask_01)
#define DS1_RAPT_OSD_ONw(val)	_wr32(_REG_BASE_+(0x02a7<<2),(UINT)((_rd32(_REG_BASE_+(0x02a7<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS1_APTGA_Rr			(ValSft_R00(_rd32(_REG_BASE_+(0x02a7<<2)))&BitMask_08)
#define DS1_APTGA_Rw(val)		_wr32(_REG_BASE_+(0x02a7<<2),(UINT)((_rd32(_REG_BASE_+(0x02a7<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DS1_APTHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a8<<2)))&BitMask_12)
#define DS1_APTHSPw(val)		_wr32(_REG_BASE_+(0x02a8<<2),(UINT)((_rd32(_REG_BASE_+(0x02a8<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS1_APTHEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a8<<2)))&BitMask_12)
#define DS1_APTHEDw(val)		_wr32(_REG_BASE_+(0x02a8<<2),(UINT)((_rd32(_REG_BASE_+(0x02a8<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS1_APTVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02a9<<2)))&BitMask_12)
#define DS1_APTVSPw(val)		_wr32(_REG_BASE_+(0x02a9<<2),(UINT)((_rd32(_REG_BASE_+(0x02a9<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS1_APTVEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02a9<<2)))&BitMask_12)
#define DS1_APTVEDw(val)		_wr32(_REG_BASE_+(0x02a9<<2),(UINT)((_rd32(_REG_BASE_+(0x02a9<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS2_RAPT_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x02aa<<2)))&BitMask_01)
#define DS2_RAPT_ONw(val)		_wr32(_REG_BASE_+(0x02aa<<2),(UINT)((_rd32(_REG_BASE_+(0x02aa<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS2_RAPT_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x02aa<<2)))&BitMask_01)
#define DS2_RAPT_OSD_ONw(val)	_wr32(_REG_BASE_+(0x02aa<<2),(UINT)((_rd32(_REG_BASE_+(0x02aa<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS2_APTGA_Rr			(ValSft_R00(_rd32(_REG_BASE_+(0x02aa<<2)))&BitMask_08)
#define DS2_APTGA_Rw(val)		_wr32(_REG_BASE_+(0x02aa<<2),(UINT)((_rd32(_REG_BASE_+(0x02aa<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DS2_APTHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02ab<<2)))&BitMask_12)
#define DS2_APTHSPw(val)		_wr32(_REG_BASE_+(0x02ab<<2),(UINT)((_rd32(_REG_BASE_+(0x02ab<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS2_APTHEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02ab<<2)))&BitMask_12)
#define DS2_APTHEDw(val)		_wr32(_REG_BASE_+(0x02ab<<2),(UINT)((_rd32(_REG_BASE_+(0x02ab<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS2_APTVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02ac<<2)))&BitMask_12)
#define DS2_APTVSPw(val)		_wr32(_REG_BASE_+(0x02ac<<2),(UINT)((_rd32(_REG_BASE_+(0x02ac<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS2_APTVEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02ac<<2)))&BitMask_12)
#define DS2_APTVEDw(val)		_wr32(_REG_BASE_+(0x02ac<<2),(UINT)((_rd32(_REG_BASE_+(0x02ac<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS3_RAPT_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x02ad<<2)))&BitMask_01)
#define DS3_RAPT_ONw(val)		_wr32(_REG_BASE_+(0x02ad<<2),(UINT)((_rd32(_REG_BASE_+(0x02ad<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define DS3_RAPT_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x02ad<<2)))&BitMask_01)
#define DS3_RAPT_OSD_ONw(val)	_wr32(_REG_BASE_+(0x02ad<<2),(UINT)((_rd32(_REG_BASE_+(0x02ad<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define DS3_APTGA_Rr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ad<<2)))&BitMask_08)
#define DS3_APTGA_Rw(val)		_wr32(_REG_BASE_+(0x02ad<<2),(UINT)((_rd32(_REG_BASE_+(0x02ad<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define DS3_APTHSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02ae<<2)))&BitMask_12)
#define DS3_APTHSPw(val)		_wr32(_REG_BASE_+(0x02ae<<2),(UINT)((_rd32(_REG_BASE_+(0x02ae<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS3_APTHEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02ae<<2)))&BitMask_12)
#define DS3_APTHEDw(val)		_wr32(_REG_BASE_+(0x02ae<<2),(UINT)((_rd32(_REG_BASE_+(0x02ae<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define DS3_APTVSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02af<<2)))&BitMask_12)
#define DS3_APTVSPw(val)		_wr32(_REG_BASE_+(0x02af<<2),(UINT)((_rd32(_REG_BASE_+(0x02af<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define DS3_APTVEDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02af<<2)))&BitMask_12)
#define DS3_APTVEDw(val)		_wr32(_REG_BASE_+(0x02af<<2),(UINT)((_rd32(_REG_BASE_+(0x02af<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define IMG_IVSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x02b0<<2)))&BitMask_03)
#define IMG_IVSELw(val)			_wr32(_REG_BASE_+(0x02b0<<2),(UINT)((_rd32(_REG_BASE_+(0x02b0<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define MRG_HSP0r				(ValSft_R16(_rd32(_REG_BASE_+(0x02b1<<2)))&BitMask_13)
#define MRG_HSP0w(val)			_wr32(_REG_BASE_+(0x02b1<<2),(UINT)((_rd32(_REG_BASE_+(0x02b1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define MRG_HW0r				(ValSft_R00(_rd32(_REG_BASE_+(0x02b1<<2)))&BitMask_13)
#define MRG_HW0w(val)			_wr32(_REG_BASE_+(0x02b1<<2),(UINT)((_rd32(_REG_BASE_+(0x02b1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define MRG_HSP1r				(ValSft_R16(_rd32(_REG_BASE_+(0x02b2<<2)))&BitMask_13)
#define MRG_HSP1w(val)			_wr32(_REG_BASE_+(0x02b2<<2),(UINT)((_rd32(_REG_BASE_+(0x02b2<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define MRG_HW1r				(ValSft_R00(_rd32(_REG_BASE_+(0x02b2<<2)))&BitMask_13)
#define MRG_HW1w(val)			_wr32(_REG_BASE_+(0x02b2<<2),(UINT)((_rd32(_REG_BASE_+(0x02b2<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define MRG_HSP2r				(ValSft_R16(_rd32(_REG_BASE_+(0x02b3<<2)))&BitMask_13)
#define MRG_HSP2w(val)			_wr32(_REG_BASE_+(0x02b3<<2),(UINT)((_rd32(_REG_BASE_+(0x02b3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define MRG_HW2r				(ValSft_R00(_rd32(_REG_BASE_+(0x02b3<<2)))&BitMask_13)
#define MRG_HW2w(val)			_wr32(_REG_BASE_+(0x02b3<<2),(UINT)((_rd32(_REG_BASE_+(0x02b3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define MRG_HSP3r				(ValSft_R16(_rd32(_REG_BASE_+(0x02b4<<2)))&BitMask_13)
#define MRG_HSP3w(val)			_wr32(_REG_BASE_+(0x02b4<<2),(UINT)((_rd32(_REG_BASE_+(0x02b4<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define MRG_HW3r				(ValSft_R00(_rd32(_REG_BASE_+(0x02b4<<2)))&BitMask_13)
#define MRG_HW3w(val)			_wr32(_REG_BASE_+(0x02b4<<2),(UINT)((_rd32(_REG_BASE_+(0x02b4<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define MRG_VSPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02b5<<2)))&BitMask_12)
#define MRG_VSPw(val)			_wr32(_REG_BASE_+(0x02b5<<2),(UINT)((_rd32(_REG_BASE_+(0x02b5<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define MRG_VWr					(ValSft_R00(_rd32(_REG_BASE_+(0x02b5<<2)))&BitMask_12)
#define MRG_VWw(val)			_wr32(_REG_BASE_+(0x02b5<<2),(UINT)((_rd32(_REG_BASE_+(0x02b5<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define MRG_HWr					(ValSft_R00(_rd32(_REG_BASE_+(0x02b6<<2)))&BitMask_13)
#define MRG_HWw(val)			_wr32(_REG_BASE_+(0x02b6<<2),(UINT)((_rd32(_REG_BASE_+(0x02b6<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define MRG_CNTr				(ValSft_R30(_rd32(_REG_BASE_+(0x02b7<<2)))&BitMask_02)
#define MRG_CNTw(val)			_wr32(_REG_BASE_+(0x02b7<<2),(UINT)((_rd32(_REG_BASE_+(0x02b7<<2))&(~ValSft_L30(BitMask_02)))|ValSft_L30((val)&BitMask_02)))

#define DDR_LTC_MRG0r			(ValSft_R16(_rd32(_REG_BASE_+(0x02b7<<2)))&BitMask_13)
#define DDR_LTC_MRG0w(val)		_wr32(_REG_BASE_+(0x02b7<<2),(UINT)((_rd32(_REG_BASE_+(0x02b7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_LTC_MRG1r			(ValSft_R00(_rd32(_REG_BASE_+(0x02b7<<2)))&BitMask_13)
#define DDR_LTC_MRG1w(val)		_wr32(_REG_BASE_+(0x02b7<<2),(UINT)((_rd32(_REG_BASE_+(0x02b7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define DDR_LTC_MRG2r			(ValSft_R16(_rd32(_REG_BASE_+(0x02b8<<2)))&BitMask_13)
#define DDR_LTC_MRG2w(val)		_wr32(_REG_BASE_+(0x02b8<<2),(UINT)((_rd32(_REG_BASE_+(0x02b8<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define DDR_LTC_MRG3r			(ValSft_R00(_rd32(_REG_BASE_+(0x02b8<<2)))&BitMask_13)
#define DDR_LTC_MRG3w(val)		_wr32(_REG_BASE_+(0x02b8<<2),(UINT)((_rd32(_REG_BASE_+(0x02b8<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define CRX_BUF_CLRr			(ValSft_R31(_rd32(_REG_BASE_+(0x02be<<2)))&BitMask_01)
#define CRX_BUF_CLRw(val)		_wr32(_REG_BASE_+(0x02be<<2),(UINT)((_rd32(_REG_BASE_+(0x02be<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CRX_BITMIN_RSTr			(ValSft_R30(_rd32(_REG_BASE_+(0x02be<<2)))&BitMask_01)
#define CRX_BITMIN_RSTw(val)	_wr32(_REG_BASE_+(0x02be<<2),(UINT)((_rd32(_REG_BASE_+(0x02be<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define INT_VEP_Er				(ValSft_R16(_rd32(_REG_BASE_+(0x02be<<2)))&BitMask_13)
#define INT_VEP_Ew(val)			_wr32(_REG_BASE_+(0x02be<<2),(UINT)((_rd32(_REG_BASE_+(0x02be<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define INT_VEP_Or				(ValSft_R00(_rd32(_REG_BASE_+(0x02be<<2)))&BitMask_13)
#define INT_VEP_Ow(val)			_wr32(_REG_BASE_+(0x02be<<2),(UINT)((_rd32(_REG_BASE_+(0x02be<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define INT_BITCNT_ENr			(ValSft_R31(_rd32(_REG_BASE_+(0x02bf<<2)))&BitMask_01)
#define INT_BITCNT_ENw(val)		_wr32(_REG_BASE_+(0x02bf<<2),(UINT)((_rd32(_REG_BASE_+(0x02bf<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define INT_BITCNT_NUMr			(ValSft_R00(_rd32(_REG_BASE_+(0x02bf<<2)))&BitMask_08)
#define INT_BITCNT_NUMw(val)	_wr32(_REG_BASE_+(0x02bf<<2),(UINT)((_rd32(_REG_BASE_+(0x02bf<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ENC_BYGAIN_LUr			(ValSft_R24(_rd32(_REG_BASE_+(0x02c0<<2)))&BitMask_08)
#define ENC_BYGAIN_LUw(val)		_wr32(_REG_BASE_+(0x02c0<<2),(UINT)((_rd32(_REG_BASE_+(0x02c0<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ENC_BYGAIN_LDr			(ValSft_R16(_rd32(_REG_BASE_+(0x02c0<<2)))&BitMask_08)
#define ENC_BYGAIN_LDw(val)		_wr32(_REG_BASE_+(0x02c0<<2),(UINT)((_rd32(_REG_BASE_+(0x02c0<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ENC_BYGAIN_RUr			(ValSft_R08(_rd32(_REG_BASE_+(0x02c0<<2)))&BitMask_08)
#define ENC_BYGAIN_RUw(val)		_wr32(_REG_BASE_+(0x02c0<<2),(UINT)((_rd32(_REG_BASE_+(0x02c0<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ENC_BYGAIN_RDr			(ValSft_R00(_rd32(_REG_BASE_+(0x02c0<<2)))&BitMask_08)
#define ENC_BYGAIN_RDw(val)		_wr32(_REG_BASE_+(0x02c0<<2),(UINT)((_rd32(_REG_BASE_+(0x02c0<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ENC_RYGAIN_LUr			(ValSft_R24(_rd32(_REG_BASE_+(0x02c1<<2)))&BitMask_08)
#define ENC_RYGAIN_LUw(val)		_wr32(_REG_BASE_+(0x02c1<<2),(UINT)((_rd32(_REG_BASE_+(0x02c1<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ENC_RYGAIN_LDr			(ValSft_R16(_rd32(_REG_BASE_+(0x02c1<<2)))&BitMask_08)
#define ENC_RYGAIN_LDw(val)		_wr32(_REG_BASE_+(0x02c1<<2),(UINT)((_rd32(_REG_BASE_+(0x02c1<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ENC_RYGAIN_RUr			(ValSft_R08(_rd32(_REG_BASE_+(0x02c1<<2)))&BitMask_08)
#define ENC_RYGAIN_RUw(val)		_wr32(_REG_BASE_+(0x02c1<<2),(UINT)((_rd32(_REG_BASE_+(0x02c1<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ENC_RYGAIN_RDr			(ValSft_R00(_rd32(_REG_BASE_+(0x02c1<<2)))&BitMask_08)
#define ENC_RYGAIN_RDw(val)		_wr32(_REG_BASE_+(0x02c1<<2),(UINT)((_rd32(_REG_BASE_+(0x02c1<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ENC_BYHUE_LUr			(ValSft_R24(_rd32(_REG_BASE_+(0x02c2<<2)))&BitMask_08)
#define ENC_BYHUE_LUw(val)		_wr32(_REG_BASE_+(0x02c2<<2),(UINT)((_rd32(_REG_BASE_+(0x02c2<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ENC_BYHUE_LDr			(ValSft_R16(_rd32(_REG_BASE_+(0x02c2<<2)))&BitMask_08)
#define ENC_BYHUE_LDw(val)		_wr32(_REG_BASE_+(0x02c2<<2),(UINT)((_rd32(_REG_BASE_+(0x02c2<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ENC_BYHUE_RUr			(ValSft_R08(_rd32(_REG_BASE_+(0x02c2<<2)))&BitMask_08)
#define ENC_BYHUE_RUw(val)		_wr32(_REG_BASE_+(0x02c2<<2),(UINT)((_rd32(_REG_BASE_+(0x02c2<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ENC_BYHUE_RDr			(ValSft_R00(_rd32(_REG_BASE_+(0x02c2<<2)))&BitMask_08)
#define ENC_BYHUE_RDw(val)		_wr32(_REG_BASE_+(0x02c2<<2),(UINT)((_rd32(_REG_BASE_+(0x02c2<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ENC_RYHUE_LUr			(ValSft_R24(_rd32(_REG_BASE_+(0x02c3<<2)))&BitMask_08)
#define ENC_RYHUE_LUw(val)		_wr32(_REG_BASE_+(0x02c3<<2),(UINT)((_rd32(_REG_BASE_+(0x02c3<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define ENC_RYHUE_LDr			(ValSft_R16(_rd32(_REG_BASE_+(0x02c3<<2)))&BitMask_08)
#define ENC_RYHUE_LDw(val)		_wr32(_REG_BASE_+(0x02c3<<2),(UINT)((_rd32(_REG_BASE_+(0x02c3<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define ENC_RYHUE_RUr			(ValSft_R08(_rd32(_REG_BASE_+(0x02c3<<2)))&BitMask_08)
#define ENC_RYHUE_RUw(val)		_wr32(_REG_BASE_+(0x02c3<<2),(UINT)((_rd32(_REG_BASE_+(0x02c3<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define ENC_RYHUE_RDr			(ValSft_R00(_rd32(_REG_BASE_+(0x02c3<<2)))&BitMask_08)
#define ENC_RYHUE_RDw(val)		_wr32(_REG_BASE_+(0x02c3<<2),(UINT)((_rd32(_REG_BASE_+(0x02c3<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define CRX_AUTOr				(ValSft_R31(_rd32(_REG_BASE_+(0x02c4<<2)))&BitMask_01)
#define CRX_AUTOw(val)			_wr32(_REG_BASE_+(0x02c4<<2),(UINT)((_rd32(_REG_BASE_+(0x02c4<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CRX_AUTO_CATr			(ValSft_R16(_rd32(_REG_BASE_+(0x02c4<<2)))&BitMask_10)
#define CRX_AUTO_CATw(val)		_wr32(_REG_BASE_+(0x02c4<<2),(UINT)((_rd32(_REG_BASE_+(0x02c4<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CXIN_FILDr				(ValSft_R00(_rd32(_REG_BASE_+(0x02c4<<2)))&BitMask_16)
#define CXIN_FILDw(val)			_wr32(_REG_BASE_+(0x02c4<<2),(UINT)((_rd32(_REG_BASE_+(0x02c4<<2))&(~ValSft_L00(BitMask_16)))|ValSft_L00((val)&BitMask_16)))

#define BURST_VRSTr				(ValSft_R16(_rd32(_REG_BASE_+(0x02c5<<2)))&BitMask_11)
#define BURST_VRSTw(val)		_wr32(_REG_BASE_+(0x02c5<<2),(UINT)((_rd32(_REG_BASE_+(0x02c5<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define ENC_HWOr				(ValSft_R00(_rd32(_REG_BASE_+(0x02c5<<2)))&BitMask_13)
#define ENC_HWOw(val)			_wr32(_REG_BASE_+(0x02c5<<2),(UINT)((_rd32(_REG_BASE_+(0x02c5<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IHDVD_ICNT_CLIPr		(ValSft_R16(_rd32(_REG_BASE_+(0x02c6<<2)))&BitMask_13)
#define IHDVD_ICNT_CLIPw(val)	_wr32(_REG_BASE_+(0x02c6<<2),(UINT)((_rd32(_REG_BASE_+(0x02c6<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IHDVD_JCNT_CLIPr		(ValSft_R00(_rd32(_REG_BASE_+(0x02c6<<2)))&BitMask_13)
#define IHDVD_JCNT_CLIPw(val)	_wr32(_REG_BASE_+(0x02c6<<2),(UINT)((_rd32(_REG_BASE_+(0x02c6<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IHDVD_HSWr				(ValSft_R16(_rd32(_REG_BASE_+(0x02c7<<2)))&BitMask_13)
#define IHDVD_HSWw(val)			_wr32(_REG_BASE_+(0x02c7<<2),(UINT)((_rd32(_REG_BASE_+(0x02c7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define IHDVD_VSWr				(ValSft_R00(_rd32(_REG_BASE_+(0x02c7<<2)))&BitMask_13)
#define IHDVD_VSWw(val)			_wr32(_REG_BASE_+(0x02c7<<2),(UINT)((_rd32(_REG_BASE_+(0x02c7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ENC_ONr					(ValSft_R31(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_ONw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ENC_CKI_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_CKI_ONw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define ENC_CBR_INVr			(ValSft_R29(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_CBR_INVw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RMODr					(ValSft_R25(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_02)
#define RMODw(val)				_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L25(BitMask_02)))|ValSft_L25((val)&BitMask_02)))

#define ENC_TIMG_SELr			(ValSft_R21(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_02)
#define ENC_TIMG_SELw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L21(BitMask_02)))|ValSft_L21((val)&BitMask_02)))

#define IHDVD_GENr				(ValSft_R20(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define IHDVD_GENw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define BURST_FREEr				(ValSft_R19(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define BURST_FREEw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define EFLD_INVr				(ValSft_R18(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define EFLD_INVw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L18(BitMask_01)))|ValSft_L18((val)&BitMask_01)))

#define NTSCr					(ValSft_R17(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define NTSCw(val)				_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L17(BitMask_01)))|ValSft_L17((val)&BitMask_01)))

#define ESYNC_POLr				(ValSft_R16(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ESYNC_POLw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L16(BitMask_01)))|ValSft_L16((val)&BitMask_01)))

#define LBURST_ONr				(ValSft_R15(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define LBURST_ONw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define SERR_TYPEr				(ValSft_R14(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define SERR_TYPEw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define Y_DLY_ADJr				(ValSft_R10(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_04)
#define Y_DLY_ADJw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L10(BitMask_04)))|ValSft_L10((val)&BitMask_04)))

#define ENC_AL_RMODr			(ValSft_R09(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_AL_RMODw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L09(BitMask_01)))|ValSft_L09((val)&BitMask_01)))

#define ENC_WFLD_INVr			(ValSft_R08(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_WFLD_INVw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L08(BitMask_01)))|ValSft_L08((val)&BitMask_01)))

#define ENC_AL_WMODr			(ValSft_R07(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_01)
#define ENC_AL_WMODw(val)		_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define SIG_SELr				(ValSft_R00(_rd32(_REG_BASE_+(0x02c8<<2)))&BitMask_05)
#define SIG_SELw(val)			_wr32(_REG_BASE_+(0x02c8<<2),(UINT)((_rd32(_REG_BASE_+(0x02c8<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define ICNT_CLIPr				(ValSft_R16(_rd32(_REG_BASE_+(0x02c9<<2)))&BitMask_13)
#define ICNT_CLIPw(val)			_wr32(_REG_BASE_+(0x02c9<<2),(UINT)((_rd32(_REG_BASE_+(0x02c9<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define HLOCK_CATr				(ValSft_R00(_rd32(_REG_BASE_+(0x02c9<<2)))&BitMask_13)
#define HLOCK_CATw(val)			_wr32(_REG_BASE_+(0x02c9<<2),(UINT)((_rd32(_REG_BASE_+(0x02c9<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define VLOCK_CAT_EVr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ca<<2)))&BitMask_13)
#define VLOCK_CAT_EVw(val)		_wr32(_REG_BASE_+(0x02ca<<2),(UINT)((_rd32(_REG_BASE_+(0x02ca<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define VLOCK_CAT_ODr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ca<<2)))&BitMask_13)
#define VLOCK_CAT_ODw(val)		_wr32(_REG_BASE_+(0x02ca<<2),(UINT)((_rd32(_REG_BASE_+(0x02ca<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define SERR_VSP_EVr			(ValSft_R16(_rd32(_REG_BASE_+(0x02cb<<2)))&BitMask_13)
#define SERR_VSP_EVw(val)		_wr32(_REG_BASE_+(0x02cb<<2),(UINT)((_rd32(_REG_BASE_+(0x02cb<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SERR_VEP_EVr			(ValSft_R00(_rd32(_REG_BASE_+(0x02cb<<2)))&BitMask_13)
#define SERR_VEP_EVw(val)		_wr32(_REG_BASE_+(0x02cb<<2),(UINT)((_rd32(_REG_BASE_+(0x02cb<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define BLANK_VEP_EVr			(ValSft_R16(_rd32(_REG_BASE_+(0x02cc<<2)))&BitMask_13)
#define BLANK_VEP_EVw(val)		_wr32(_REG_BASE_+(0x02cc<<2),(UINT)((_rd32(_REG_BASE_+(0x02cc<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define BURST_VSP_EVr			(ValSft_R00(_rd32(_REG_BASE_+(0x02cc<<2)))&BitMask_13)
#define BURST_VSP_EVw(val)		_wr32(_REG_BASE_+(0x02cc<<2),(UINT)((_rd32(_REG_BASE_+(0x02cc<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ACT_VSP_EVr				(ValSft_R16(_rd32(_REG_BASE_+(0x02cd<<2)))&BitMask_13)
#define ACT_VSP_EVw(val)		_wr32(_REG_BASE_+(0x02cd<<2),(UINT)((_rd32(_REG_BASE_+(0x02cd<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ACT_VEP_EVr				(ValSft_R00(_rd32(_REG_BASE_+(0x02cd<<2)))&BitMask_13)
#define ACT_VEP_EVw(val)		_wr32(_REG_BASE_+(0x02cd<<2),(UINT)((_rd32(_REG_BASE_+(0x02cd<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define BURST_VEP_EVr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ce<<2)))&BitMask_13)
#define BURST_VEP_EVw(val)		_wr32(_REG_BASE_+(0x02ce<<2),(UINT)((_rd32(_REG_BASE_+(0x02ce<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define BLANK_VSP_EVr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ce<<2)))&BitMask_13)
#define BLANK_VSP_EVw(val)		_wr32(_REG_BASE_+(0x02ce<<2),(UINT)((_rd32(_REG_BASE_+(0x02ce<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define SERR_VSP_ODr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d0<<2)))&BitMask_13)
#define SERR_VSP_ODw(val)		_wr32(_REG_BASE_+(0x02d0<<2),(UINT)((_rd32(_REG_BASE_+(0x02d0<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SERR_VEP_ODr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d0<<2)))&BitMask_13)
#define SERR_VEP_ODw(val)		_wr32(_REG_BASE_+(0x02d0<<2),(UINT)((_rd32(_REG_BASE_+(0x02d0<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define BLANK_VEP_ODr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d1<<2)))&BitMask_13)
#define BLANK_VEP_ODw(val)		_wr32(_REG_BASE_+(0x02d1<<2),(UINT)((_rd32(_REG_BASE_+(0x02d1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define BURST_VSP_ODr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d1<<2)))&BitMask_13)
#define BURST_VSP_ODw(val)		_wr32(_REG_BASE_+(0x02d1<<2),(UINT)((_rd32(_REG_BASE_+(0x02d1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ACT_VSP_ODr				(ValSft_R16(_rd32(_REG_BASE_+(0x02d2<<2)))&BitMask_13)
#define ACT_VSP_ODw(val)		_wr32(_REG_BASE_+(0x02d2<<2),(UINT)((_rd32(_REG_BASE_+(0x02d2<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define ACT_VEP_ODr				(ValSft_R00(_rd32(_REG_BASE_+(0x02d2<<2)))&BitMask_13)
#define ACT_VEP_ODw(val)		_wr32(_REG_BASE_+(0x02d2<<2),(UINT)((_rd32(_REG_BASE_+(0x02d2<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define BURST_VEP_ODr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d3<<2)))&BitMask_13)
#define BURST_VEP_ODw(val)		_wr32(_REG_BASE_+(0x02d3<<2),(UINT)((_rd32(_REG_BASE_+(0x02d3<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define BLANK_VSP_ODr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d3<<2)))&BitMask_13)
#define BLANK_VSP_ODw(val)		_wr32(_REG_BASE_+(0x02d3<<2),(UINT)((_rd32(_REG_BASE_+(0x02d3<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ACT_BLANK_HWr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d5<<2)))&BitMask_14)
#define ACT_BLANK_HWw(val)		_wr32(_REG_BASE_+(0x02d5<<2),(UINT)((_rd32(_REG_BASE_+(0x02d5<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define ACT_BLANK_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d5<<2)))&BitMask_14)
#define ACT_BLANK_HSPw(val)		_wr32(_REG_BASE_+(0x02d5<<2),(UINT)((_rd32(_REG_BASE_+(0x02d5<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define SERR_SYNC_HWr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d6<<2)))&BitMask_13)
#define SERR_SYNC_HWw(val)		_wr32(_REG_BASE_+(0x02d6<<2),(UINT)((_rd32(_REG_BASE_+(0x02d6<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define SERR_SYNC_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d6<<2)))&BitMask_13)
#define SERR_SYNC_HSPw(val)		_wr32(_REG_BASE_+(0x02d6<<2),(UINT)((_rd32(_REG_BASE_+(0x02d6<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define EQUAL_SYNC_HWr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d7<<2)))&BitMask_13)
#define EQUAL_SYNC_HWw(val)		_wr32(_REG_BASE_+(0x02d7<<2),(UINT)((_rd32(_REG_BASE_+(0x02d7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define EQUAL_SYNC_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d7<<2)))&BitMask_13)
#define EQUAL_SYNC_HSPw(val)	_wr32(_REG_BASE_+(0x02d7<<2),(UINT)((_rd32(_REG_BASE_+(0x02d7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define ACT_SYNC_HWr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d8<<2)))&BitMask_14)
#define ACT_SYNC_HWw(val)		_wr32(_REG_BASE_+(0x02d8<<2),(UINT)((_rd32(_REG_BASE_+(0x02d8<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define ACT_SYNC_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d8<<2)))&BitMask_14)
#define ACT_SYNC_HSPw(val)		_wr32(_REG_BASE_+(0x02d8<<2),(UINT)((_rd32(_REG_BASE_+(0x02d8<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define ACT_BURST_HWr			(ValSft_R16(_rd32(_REG_BASE_+(0x02d9<<2)))&BitMask_14)
#define ACT_BURST_HWw(val)		_wr32(_REG_BASE_+(0x02d9<<2),(UINT)((_rd32(_REG_BASE_+(0x02d9<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define ACT_BURST_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02d9<<2)))&BitMask_14)
#define ACT_BURST_HSPw(val)		_wr32(_REG_BASE_+(0x02d9<<2),(UINT)((_rd32(_REG_BASE_+(0x02d9<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define ACT_ACT_HWr				(ValSft_R16(_rd32(_REG_BASE_+(0x02da<<2)))&BitMask_14)
#define ACT_ACT_HWw(val)		_wr32(_REG_BASE_+(0x02da<<2),(UINT)((_rd32(_REG_BASE_+(0x02da<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define ACT_ACT_HSPr			(ValSft_R00(_rd32(_REG_BASE_+(0x02da<<2)))&BitMask_14)
#define ACT_ACT_HSPw(val)		_wr32(_REG_BASE_+(0x02da<<2),(UINT)((_rd32(_REG_BASE_+(0x02da<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define TEST_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x02dc<<2)))&BitMask_01)
#define TEST_ONw(val)			_wr32(_REG_BASE_+(0x02dc<<2),(UINT)((_rd32(_REG_BASE_+(0x02dc<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define TEST_SELr				(ValSft_R28(_rd32(_REG_BASE_+(0x02dc<<2)))&BitMask_03)
#define TEST_SELw(val)			_wr32(_REG_BASE_+(0x02dc<<2),(UINT)((_rd32(_REG_BASE_+(0x02dc<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define TEST_HTWr				(ValSft_R16(_rd32(_REG_BASE_+(0x02dc<<2)))&BitMask_12)
#define TEST_HTWw(val)			_wr32(_REG_BASE_+(0x02dc<<2),(UINT)((_rd32(_REG_BASE_+(0x02dc<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define TEST_HSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x02dc<<2)))&BitMask_13)
#define TEST_HSPw(val)			_wr32(_REG_BASE_+(0x02dc<<2),(UINT)((_rd32(_REG_BASE_+(0x02dc<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define Y_MGAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x02dd<<2)))&BitMask_12)
#define Y_MGAINw(val)			_wr32(_REG_BASE_+(0x02dd<<2),(UINT)((_rd32(_REG_BASE_+(0x02dd<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define WHITE_LVr				(ValSft_R16(_rd32(_REG_BASE_+(0x02de<<2)))&BitMask_10)
#define WHITE_LVw(val)			_wr32(_REG_BASE_+(0x02de<<2),(UINT)((_rd32(_REG_BASE_+(0x02de<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define BLANK_LVr				(ValSft_R00(_rd32(_REG_BASE_+(0x02de<<2)))&BitMask_10)
#define BLANK_LVw(val)			_wr32(_REG_BASE_+(0x02de<<2),(UINT)((_rd32(_REG_BASE_+(0x02de<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define PEDESTAL_LVr			(ValSft_R16(_rd32(_REG_BASE_+(0x02df<<2)))&BitMask_11)
#define PEDESTAL_LVw(val)		_wr32(_REG_BASE_+(0x02df<<2),(UINT)((_rd32(_REG_BASE_+(0x02df<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define Y_LPF_ONr				(ValSft_R14(_rd32(_REG_BASE_+(0x02df<<2)))&BitMask_02)
#define Y_LPF_ONw(val)			_wr32(_REG_BASE_+(0x02df<<2),(UINT)((_rd32(_REG_BASE_+(0x02df<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define Y_SLEW_SELr				(ValSft_R12(_rd32(_REG_BASE_+(0x02df<<2)))&BitMask_02)
#define Y_SLEW_SELw(val)		_wr32(_REG_BASE_+(0x02df<<2),(UINT)((_rd32(_REG_BASE_+(0x02df<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define SYNC_LVr				(ValSft_R00(_rd32(_REG_BASE_+(0x02df<<2)))&BitMask_10)
#define SYNC_LVw(val)			_wr32(_REG_BASE_+(0x02df<<2),(UINT)((_rd32(_REG_BASE_+(0x02df<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CB_MGAINr				(ValSft_R16(_rd32(_REG_BASE_+(0x02e0<<2)))&BitMask_12)
#define CB_MGAINw(val)			_wr32(_REG_BASE_+(0x02e0<<2),(UINT)((_rd32(_REG_BASE_+(0x02e0<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define CR_MGAINr				(ValSft_R00(_rd32(_REG_BASE_+(0x02e0<<2)))&BitMask_12)
#define CR_MGAINw(val)			_wr32(_REG_BASE_+(0x02e0<<2),(UINT)((_rd32(_REG_BASE_+(0x02e0<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define C_SLEW_SELr				(ValSft_R08(_rd32(_REG_BASE_+(0x02e1<<2)))&BitMask_02)
#define C_SLEW_SELw(val)		_wr32(_REG_BASE_+(0x02e1<<2),(UINT)((_rd32(_REG_BASE_+(0x02e1<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define C_LPF_ONr				(ValSft_R00(_rd32(_REG_BASE_+(0x02e1<<2)))&BitMask_03)
#define C_LPF_ONw(val)			_wr32(_REG_BASE_+(0x02e1<<2),(UINT)((_rd32(_REG_BASE_+(0x02e1<<2))&(~ValSft_L00(BitMask_03)))|ValSft_L00((val)&BitMask_03)))

#define FSC_TGTr				(ValSft_R00(_rd32(_REG_BASE_+(0x02e2<<2)))&BitMask_32)
#define FSC_TGTw(val)			_wr32(_REG_BASE_+(0x02e2<<2),(UINT)((_rd32(_REG_BASE_+(0x02e2<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CB_BSTr					(ValSft_R16(_rd32(_REG_BASE_+(0x02e3<<2)))&BitMask_10)
#define CB_BSTw(val)			_wr32(_REG_BASE_+(0x02e3<<2),(UINT)((_rd32(_REG_BASE_+(0x02e3<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define CR_BSTr					(ValSft_R00(_rd32(_REG_BASE_+(0x02e3<<2)))&BitMask_10)
#define CR_BSTw(val)			_wr32(_REG_BASE_+(0x02e3<<2),(UINT)((_rd32(_REG_BASE_+(0x02e3<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define YCOUT_POLr				(ValSft_R31(_rd32(_REG_BASE_+(0x02e4<<2)))&BitMask_01)
#define YCOUT_POLw(val)			_wr32(_REG_BASE_+(0x02e4<<2),(UINT)((_rd32(_REG_BASE_+(0x02e4<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AY_OSELr				(ValSft_R28(_rd32(_REG_BASE_+(0x02e4<<2)))&BitMask_03)
#define AY_OSELw(val)			_wr32(_REG_BASE_+(0x02e4<<2),(UINT)((_rd32(_REG_BASE_+(0x02e4<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define AY_IORDERr				(ValSft_R27(_rd32(_REG_BASE_+(0x02e4<<2)))&BitMask_01)
#define AY_IORDERw(val)			_wr32(_REG_BASE_+(0x02e4<<2),(UINT)((_rd32(_REG_BASE_+(0x02e4<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define YC_LEVEL_TESTr			(ValSft_R00(_rd32(_REG_BASE_+(0x02e4<<2)))&BitMask_10)
#define YC_LEVEL_TESTw(val)		_wr32(_REG_BASE_+(0x02e4<<2),(UINT)((_rd32(_REG_BASE_+(0x02e4<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CTX_ENr					(ValSft_R31(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_01)
#define CTX_ENw(val)			_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define CRX_ENr					(ValSft_R30(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_01)
#define CRX_ENw(val)			_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define CRX_DEGLCHTABr			(ValSft_R24(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_05)
#define CRX_DEGLCHTABw(val)		_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L24(BitMask_05)))|ValSft_L24((val)&BitMask_05)))

#define CRX_DEGLCHTH1r			(ValSft_R16(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_05)
#define CRX_DEGLCHTH1w(val)		_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L16(BitMask_05)))|ValSft_L16((val)&BitMask_05)))

#define CRX_DEGLCHTH2r			(ValSft_R10(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_05)
#define CRX_DEGLCHTH2w(val)		_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L10(BitMask_05)))|ValSft_L10((val)&BitMask_05)))

#define CTX_LVLr				(ValSft_R00(_rd32(_REG_BASE_+(0x02e5<<2)))&BitMask_10)
#define CTX_LVLw(val)			_wr32(_REG_BASE_+(0x02e5<<2),(UINT)((_rd32(_REG_BASE_+(0x02e5<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CRX_VPOSEVSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02e6<<2)))&BitMask_13)
#define CRX_VPOSEVSw(val)		_wr32(_REG_BASE_+(0x02e6<<2),(UINT)((_rd32(_REG_BASE_+(0x02e6<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define CRX_VPOSEVEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02e6<<2)))&BitMask_13)
#define CRX_VPOSEVEw(val)		_wr32(_REG_BASE_+(0x02e6<<2),(UINT)((_rd32(_REG_BASE_+(0x02e6<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define CRX_VPOSODSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02e7<<2)))&BitMask_13)
#define CRX_VPOSODSw(val)		_wr32(_REG_BASE_+(0x02e7<<2),(UINT)((_rd32(_REG_BASE_+(0x02e7<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define CRX_VPOSODEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02e7<<2)))&BitMask_13)
#define CRX_VPOSODEw(val)		_wr32(_REG_BASE_+(0x02e7<<2),(UINT)((_rd32(_REG_BASE_+(0x02e7<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define CRX_HPOSEVSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02e8<<2)))&BitMask_14)
#define CRX_HPOSEVSw(val)		_wr32(_REG_BASE_+(0x02e8<<2),(UINT)((_rd32(_REG_BASE_+(0x02e8<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define CRX_HPOSEVEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02e8<<2)))&BitMask_14)
#define CRX_HPOSEVEw(val)		_wr32(_REG_BASE_+(0x02e8<<2),(UINT)((_rd32(_REG_BASE_+(0x02e8<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define CRX_HPOSODSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02e9<<2)))&BitMask_14)
#define CRX_HPOSODSw(val)		_wr32(_REG_BASE_+(0x02e9<<2),(UINT)((_rd32(_REG_BASE_+(0x02e9<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define CRX_HPOSODEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02e9<<2)))&BitMask_14)
#define CRX_HPOSODEw(val)		_wr32(_REG_BASE_+(0x02e9<<2),(UINT)((_rd32(_REG_BASE_+(0x02e9<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define CRX_LBITNUMr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ea<<2)))&BitMask_06)
#define CRX_LBITNUMw(val)		_wr32(_REG_BASE_+(0x02ea<<2),(UINT)((_rd32(_REG_BASE_+(0x02ea<<2))&(~ValSft_L16(BitMask_06)))|ValSft_L16((val)&BitMask_06)))

#define CRX_BITRATEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ea<<2)))&BitMask_10)
#define CRX_BITRATEw(val)		_wr32(_REG_BASE_+(0x02ea<<2),(UINT)((_rd32(_REG_BASE_+(0x02ea<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define CTX_VPOSEVSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02eb<<2)))&BitMask_13)
#define CTX_VPOSEVSw(val)		_wr32(_REG_BASE_+(0x02eb<<2),(UINT)((_rd32(_REG_BASE_+(0x02eb<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define CTX_VPOSEVEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02eb<<2)))&BitMask_13)
#define CTX_VPOSEVEw(val)		_wr32(_REG_BASE_+(0x02eb<<2),(UINT)((_rd32(_REG_BASE_+(0x02eb<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define CTX_VPOSODSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ec<<2)))&BitMask_13)
#define CTX_VPOSODSw(val)		_wr32(_REG_BASE_+(0x02ec<<2),(UINT)((_rd32(_REG_BASE_+(0x02ec<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define CTX_VPOSODEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ec<<2)))&BitMask_13)
#define CTX_VPOSODEw(val)		_wr32(_REG_BASE_+(0x02ec<<2),(UINT)((_rd32(_REG_BASE_+(0x02ec<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define CTX_HPOSEVSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ed<<2)))&BitMask_14)
#define CTX_HPOSEVSw(val)		_wr32(_REG_BASE_+(0x02ed<<2),(UINT)((_rd32(_REG_BASE_+(0x02ed<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define CTX_HPOSEVEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ed<<2)))&BitMask_14)
#define CTX_HPOSEVEw(val)		_wr32(_REG_BASE_+(0x02ed<<2),(UINT)((_rd32(_REG_BASE_+(0x02ed<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define CTX_HPOSODSr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ee<<2)))&BitMask_14)
#define CTX_HPOSODSw(val)		_wr32(_REG_BASE_+(0x02ee<<2),(UINT)((_rd32(_REG_BASE_+(0x02ee<<2))&(~ValSft_L16(BitMask_14)))|ValSft_L16((val)&BitMask_14)))

#define CTX_HPOSODEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ee<<2)))&BitMask_14)
#define CTX_HPOSODEw(val)		_wr32(_REG_BASE_+(0x02ee<<2),(UINT)((_rd32(_REG_BASE_+(0x02ee<<2))&(~ValSft_L00(BitMask_14)))|ValSft_L00((val)&BitMask_14)))

#define CLI_CXALr				(ValSft_R27(_rd32(_REG_BASE_+(0x02ef<<2)))&BitMask_01)
#define CLI_CXALw(val)			_wr32(_REG_BASE_+(0x02ef<<2),(UINT)((_rd32(_REG_BASE_+(0x02ef<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define CTX_LBITNUMr			(ValSft_R16(_rd32(_REG_BASE_+(0x02ef<<2)))&BitMask_06)
#define CTX_LBITNUMw(val)		_wr32(_REG_BASE_+(0x02ef<<2),(UINT)((_rd32(_REG_BASE_+(0x02ef<<2))&(~ValSft_L16(BitMask_06)))|ValSft_L16((val)&BitMask_06)))

#define CTX_BITRATEr			(ValSft_R00(_rd32(_REG_BASE_+(0x02ef<<2)))&BitMask_10)
#define CTX_BITRATEw(val)		_wr32(_REG_BASE_+(0x02ef<<2),(UINT)((_rd32(_REG_BASE_+(0x02ef<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define INT_ENr					(ValSft_R31(_rd32(_REG_BASE_+(0x02f0<<2)))&BitMask_01)
#define INT_ENw(val)			_wr32(_REG_BASE_+(0x02f0<<2),(UINT)((_rd32(_REG_BASE_+(0x02f0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define INT_VALIDr				(ValSft_R30(_rd32(_REG_BASE_+(0x02f0<<2)))&BitMask_01)
#define INT_VALIDw(val)			_wr32(_REG_BASE_+(0x02f0<<2),(UINT)((_rd32(_REG_BASE_+(0x02f0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define CTX_CONTINUEr			(ValSft_R29(_rd32(_REG_BASE_+(0x02f0<<2)))&BitMask_01)
#define CTX_CONTINUEw(val)		_wr32(_REG_BASE_+(0x02f0<<2),(UINT)((_rd32(_REG_BASE_+(0x02f0<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CTX_LSBKEEPr			(ValSft_R28(_rd32(_REG_BASE_+(0x02f0<<2)))&BitMask_01)
#define CTX_LSBKEEPw(val)		_wr32(_REG_BASE_+(0x02f0<<2),(UINT)((_rd32(_REG_BASE_+(0x02f0<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define INT_HSPr				(ValSft_R00(_rd32(_REG_BASE_+(0x02f0<<2)))&BitMask_13)
#define INT_HSPw(val)			_wr32(_REG_BASE_+(0x02f0<<2),(UINT)((_rd32(_REG_BASE_+(0x02f0<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define INT_VSP_Er				(ValSft_R16(_rd32(_REG_BASE_+(0x02f1<<2)))&BitMask_13)
#define INT_VSP_Ew(val)			_wr32(_REG_BASE_+(0x02f1<<2),(UINT)((_rd32(_REG_BASE_+(0x02f1<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define INT_VSP_Or				(ValSft_R00(_rd32(_REG_BASE_+(0x02f1<<2)))&BitMask_13)
#define INT_VSP_Ow(val)			_wr32(_REG_BASE_+(0x02f1<<2),(UINT)((_rd32(_REG_BASE_+(0x02f1<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define TSET_FSCr				(ValSft_R00(_rd32(_REG_BASE_+(0x02f2<<2)))&BitMask_32)
#define TSET_FSCw(val)			_wr32(_REG_BASE_+(0x02f2<<2),(UINT)((_rd32(_REG_BASE_+(0x02f2<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX00r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f3<<2)))&BitMask_32)
#define CTX00w(val)				_wr32(_REG_BASE_+(0x02f3<<2),(UINT)((_rd32(_REG_BASE_+(0x02f3<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX01r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f4<<2)))&BitMask_32)
#define CTX01w(val)				_wr32(_REG_BASE_+(0x02f4<<2),(UINT)((_rd32(_REG_BASE_+(0x02f4<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX02r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f5<<2)))&BitMask_32)
#define CTX02w(val)				_wr32(_REG_BASE_+(0x02f5<<2),(UINT)((_rd32(_REG_BASE_+(0x02f5<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX03r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f6<<2)))&BitMask_32)
#define CTX03w(val)				_wr32(_REG_BASE_+(0x02f6<<2),(UINT)((_rd32(_REG_BASE_+(0x02f6<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX04r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f7<<2)))&BitMask_32)
#define CTX04w(val)				_wr32(_REG_BASE_+(0x02f7<<2),(UINT)((_rd32(_REG_BASE_+(0x02f7<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX05r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f8<<2)))&BitMask_32)
#define CTX05w(val)				_wr32(_REG_BASE_+(0x02f8<<2),(UINT)((_rd32(_REG_BASE_+(0x02f8<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX06r					(ValSft_R00(_rd32(_REG_BASE_+(0x02f9<<2)))&BitMask_32)
#define CTX06w(val)				_wr32(_REG_BASE_+(0x02f9<<2),(UINT)((_rd32(_REG_BASE_+(0x02f9<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CTX07r					(ValSft_R00(_rd32(_REG_BASE_+(0x02fa<<2)))&BitMask_32)
#define CTX07w(val)				_wr32(_REG_BASE_+(0x02fa<<2),(UINT)((_rd32(_REG_BASE_+(0x02fa<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define CXAL_WOPr				(ValSft_R31(_rd32(_REG_BASE_+(0x02fb<<2)))&BitMask_01)
#define CXAL_WOPw(val)			_wr32(_REG_BASE_+(0x02fb<<2),(UINT)((_rd32(_REG_BASE_+(0x02fb<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define STACK0r					(ValSft_R00(_rd32(_REG_BASE_+(0x0300<<2)))&BitMask_32)
#define STACK0w(val)			_wr32(_REG_BASE_+(0x0300<<2),(UINT)((_rd32(_REG_BASE_+(0x0300<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0301<<2)))&BitMask_32)
#define STACK1w(val)			_wr32(_REG_BASE_+(0x0301<<2),(UINT)((_rd32(_REG_BASE_+(0x0301<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK2r					(ValSft_R00(_rd32(_REG_BASE_+(0x0302<<2)))&BitMask_32)
#define STACK2w(val)			_wr32(_REG_BASE_+(0x0302<<2),(UINT)((_rd32(_REG_BASE_+(0x0302<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK3r					(ValSft_R00(_rd32(_REG_BASE_+(0x0303<<2)))&BitMask_32)
#define STACK3w(val)			_wr32(_REG_BASE_+(0x0303<<2),(UINT)((_rd32(_REG_BASE_+(0x0303<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK4r					(ValSft_R00(_rd32(_REG_BASE_+(0x0304<<2)))&BitMask_32)
#define STACK4w(val)			_wr32(_REG_BASE_+(0x0304<<2),(UINT)((_rd32(_REG_BASE_+(0x0304<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK5r					(ValSft_R00(_rd32(_REG_BASE_+(0x0305<<2)))&BitMask_32)
#define STACK5w(val)			_wr32(_REG_BASE_+(0x0305<<2),(UINT)((_rd32(_REG_BASE_+(0x0305<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK6r					(ValSft_R00(_rd32(_REG_BASE_+(0x0306<<2)))&BitMask_32)
#define STACK6w(val)			_wr32(_REG_BASE_+(0x0306<<2),(UINT)((_rd32(_REG_BASE_+(0x0306<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK7r					(ValSft_R00(_rd32(_REG_BASE_+(0x0307<<2)))&BitMask_32)
#define STACK7w(val)			_wr32(_REG_BASE_+(0x0307<<2),(UINT)((_rd32(_REG_BASE_+(0x0307<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK8r					(ValSft_R00(_rd32(_REG_BASE_+(0x0308<<2)))&BitMask_32)
#define STACK8w(val)			_wr32(_REG_BASE_+(0x0308<<2),(UINT)((_rd32(_REG_BASE_+(0x0308<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACK9r					(ValSft_R00(_rd32(_REG_BASE_+(0x0309<<2)))&BitMask_32)
#define STACK9w(val)			_wr32(_REG_BASE_+(0x0309<<2),(UINT)((_rd32(_REG_BASE_+(0x0309<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define STACKAr					(ValSft_R00(_rd32(_REG_BASE_+(0x030a<<2)))&BitMask_32)
#define STACKAw(val)			_wr32(_REG_BASE_+(0x030a<<2),(UINT)((_rd32(_REG_BASE_+(0x030a<<2))&(~ValSft_L00(BitMask_32)))|ValSft_L00((val)&BitMask_32)))

#define RYC_OSYNC_MOD1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_01)
#define RYC_OSYNC_MOD1w(val)	_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_POS_HZ1r			(ValSft_R30(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_01)
#define RYC_POS_HZ1w(val)		_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SVSO_TRG1r			(ValSft_R28(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_01)
#define RYC_SVSO_TRG1w(val)		_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define RYC_SHSO_INV1r			(ValSft_R27(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_01)
#define RYC_SHSO_INV1w(val)		_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define RYC_SVSO_INV1r			(ValSft_R26(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_01)
#define RYC_SVSO_INV1w(val)		_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define RYC_VWI1r				(ValSft_R13(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_12)
#define RYC_VWI1w(val)			_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L13(BitMask_12)))|ValSft_L13((val)&BitMask_12)))

#define RYC_HWI1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0320<<2)))&BitMask_13)
#define RYC_HWI1w(val)			_wr32(_REG_BASE_+(0x0320<<2),(UINT)((_rd32(_REG_BASE_+(0x0320<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_EXSYNC_SEL1r		(ValSft_R29(_rd32(_REG_BASE_+(0x0321<<2)))&BitMask_03)
#define RYC_EXSYNC_SEL1w(val)	_wr32(_REG_BASE_+(0x0321<<2),(UINT)((_rd32(_REG_BASE_+(0x0321<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define RYC_HTWO1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0321<<2)))&BitMask_13)
#define RYC_HTWO1w(val)			_wr32(_REG_BASE_+(0x0321<<2),(UINT)((_rd32(_REG_BASE_+(0x0321<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VTWO1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0321<<2)))&BitMask_12)
#define RYC_VTWO1w(val)			_wr32(_REG_BASE_+(0x0321<<2),(UINT)((_rd32(_REG_BASE_+(0x0321<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_ITLC1r				(ValSft_R31(_rd32(_REG_BASE_+(0x0322<<2)))&BitMask_01)
#define RYC_ITLC1w(val)			_wr32(_REG_BASE_+(0x0322<<2),(UINT)((_rd32(_REG_BASE_+(0x0322<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_FLD_INV1r			(ValSft_R30(_rd32(_REG_BASE_+(0x0322<<2)))&BitMask_01)
#define RYC_FLD_INV1w(val)		_wr32(_REG_BASE_+(0x0322<<2),(UINT)((_rd32(_REG_BASE_+(0x0322<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_FLD_IMG_INV1r		(ValSft_R29(_rd32(_REG_BASE_+(0x0322<<2)))&BitMask_01)
#define RYC_FLD_IMG_INV1w(val)	_wr32(_REG_BASE_+(0x0322<<2),(UINT)((_rd32(_REG_BASE_+(0x0322<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RYC_HSPI1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0322<<2)))&BitMask_13)
#define RYC_HSPI1w(val)			_wr32(_REG_BASE_+(0x0322<<2),(UINT)((_rd32(_REG_BASE_+(0x0322<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VSPI1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0322<<2)))&BitMask_12)
#define RYC_VSPI1w(val)			_wr32(_REG_BASE_+(0x0322<<2),(UINT)((_rd32(_REG_BASE_+(0x0322<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_SHSO_EDGE1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0323<<2)))&BitMask_01)
#define RYC_SHSO_EDGE1w(val)	_wr32(_REG_BASE_+(0x0323<<2),(UINT)((_rd32(_REG_BASE_+(0x0323<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_SVSO_EDGE1r			(ValSft_R30(_rd32(_REG_BASE_+(0x0323<<2)))&BitMask_01)
#define RYC_SVSO_EDGE1w(val)	_wr32(_REG_BASE_+(0x0323<<2),(UINT)((_rd32(_REG_BASE_+(0x0323<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SHSO_FP1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0323<<2)))&BitMask_13)
#define RYC_SHSO_FP1w(val)		_wr32(_REG_BASE_+(0x0323<<2),(UINT)((_rd32(_REG_BASE_+(0x0323<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_SHSO_RP1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0323<<2)))&BitMask_13)
#define RYC_SHSO_RP1w(val)		_wr32(_REG_BASE_+(0x0323<<2),(UINT)((_rd32(_REG_BASE_+(0x0323<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_SVSO_FP1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0324<<2)))&BitMask_12)
#define RYC_SVSO_FP1w(val)		_wr32(_REG_BASE_+(0x0324<<2),(UINT)((_rd32(_REG_BASE_+(0x0324<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_SVSO_RP1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0324<<2)))&BitMask_12)
#define RYC_SVSO_RP1w(val)		_wr32(_REG_BASE_+(0x0324<<2),(UINT)((_rd32(_REG_BASE_+(0x0324<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_DENO_EDGE1r			(ValSft_R31(_rd32(_REG_BASE_+(0x0325<<2)))&BitMask_01)
#define RYC_DENO_EDGE1w(val)	_wr32(_REG_BASE_+(0x0325<<2),(UINT)((_rd32(_REG_BASE_+(0x0325<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_DENO_VSPI1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0325<<2)))&BitMask_12)
#define RYC_DENO_VSPI1w(val)	_wr32(_REG_BASE_+(0x0325<<2),(UINT)((_rd32(_REG_BASE_+(0x0325<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_DENO_VEDI1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0325<<2)))&BitMask_12)
#define RYC_DENO_VEDI1w(val)	_wr32(_REG_BASE_+(0x0325<<2),(UINT)((_rd32(_REG_BASE_+(0x0325<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_TRG_OVSYNCI1r		(ValSft_R31(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_01)
#define RYC_TRG_OVSYNCI1w(val)	_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_TRG_OHSYNCI1r		(ValSft_R30(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_01)
#define RYC_TRG_OHSYNCI1w(val)	_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_DENO_HRP1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_13)
#define RYC_DENO_HRP1w(val)		_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_POL_OVSYNCI1r		(ValSft_R15(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_01)
#define RYC_POL_OVSYNCI1w(val)	_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define RYC_POL_OHSYNCI1r		(ValSft_R14(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_01)
#define RYC_POL_OHSYNCI1w(val)	_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define RYC_DENO_HFP1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0326<<2)))&BitMask_13)
#define RYC_DENO_HFP1w(val)		_wr32(_REG_BASE_+(0x0326<<2),(UINT)((_rd32(_REG_BASE_+(0x0326<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_OSYNC_MOD2r			(ValSft_R31(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_01)
#define RYC_OSYNC_MOD2w(val)	_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_POS_HZ2r			(ValSft_R30(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_01)
#define RYC_POS_HZ2w(val)		_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SVSO_TRG2r			(ValSft_R28(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_01)
#define RYC_SVSO_TRG2w(val)		_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define RYC_SHSO_INV2r			(ValSft_R27(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_01)
#define RYC_SHSO_INV2w(val)		_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define RYC_SVSO_INV2r			(ValSft_R26(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_01)
#define RYC_SVSO_INV2w(val)		_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define RYC_VWI2r				(ValSft_R13(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_12)
#define RYC_VWI2w(val)			_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L13(BitMask_12)))|ValSft_L13((val)&BitMask_12)))

#define RYC_HWI2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0327<<2)))&BitMask_13)
#define RYC_HWI2w(val)			_wr32(_REG_BASE_+(0x0327<<2),(UINT)((_rd32(_REG_BASE_+(0x0327<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_EXSYNC_SEL2r		(ValSft_R29(_rd32(_REG_BASE_+(0x0328<<2)))&BitMask_03)
#define RYC_EXSYNC_SEL2w(val)	_wr32(_REG_BASE_+(0x0328<<2),(UINT)((_rd32(_REG_BASE_+(0x0328<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define RYC_HTWO2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0328<<2)))&BitMask_13)
#define RYC_HTWO2w(val)			_wr32(_REG_BASE_+(0x0328<<2),(UINT)((_rd32(_REG_BASE_+(0x0328<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VTWO2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0328<<2)))&BitMask_12)
#define RYC_VTWO2w(val)			_wr32(_REG_BASE_+(0x0328<<2),(UINT)((_rd32(_REG_BASE_+(0x0328<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_ITLC2r				(ValSft_R31(_rd32(_REG_BASE_+(0x0329<<2)))&BitMask_01)
#define RYC_ITLC2w(val)			_wr32(_REG_BASE_+(0x0329<<2),(UINT)((_rd32(_REG_BASE_+(0x0329<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_FLD_INV2r			(ValSft_R30(_rd32(_REG_BASE_+(0x0329<<2)))&BitMask_01)
#define RYC_FLD_INV2w(val)		_wr32(_REG_BASE_+(0x0329<<2),(UINT)((_rd32(_REG_BASE_+(0x0329<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_FLD_IMG_INV2r		(ValSft_R29(_rd32(_REG_BASE_+(0x0329<<2)))&BitMask_01)
#define RYC_FLD_IMG_INV2w(val)	_wr32(_REG_BASE_+(0x0329<<2),(UINT)((_rd32(_REG_BASE_+(0x0329<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RYC_HSPI2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0329<<2)))&BitMask_13)
#define RYC_HSPI2w(val)			_wr32(_REG_BASE_+(0x0329<<2),(UINT)((_rd32(_REG_BASE_+(0x0329<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VSPI2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0329<<2)))&BitMask_12)
#define RYC_VSPI2w(val)			_wr32(_REG_BASE_+(0x0329<<2),(UINT)((_rd32(_REG_BASE_+(0x0329<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_SHSO_EDGE2r			(ValSft_R31(_rd32(_REG_BASE_+(0x032a<<2)))&BitMask_01)
#define RYC_SHSO_EDGE2w(val)	_wr32(_REG_BASE_+(0x032a<<2),(UINT)((_rd32(_REG_BASE_+(0x032a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_SVSO_EDGE2r			(ValSft_R30(_rd32(_REG_BASE_+(0x032a<<2)))&BitMask_01)
#define RYC_SVSO_EDGE2w(val)	_wr32(_REG_BASE_+(0x032a<<2),(UINT)((_rd32(_REG_BASE_+(0x032a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SHSO_FP2r			(ValSft_R16(_rd32(_REG_BASE_+(0x032a<<2)))&BitMask_13)
#define RYC_SHSO_FP2w(val)		_wr32(_REG_BASE_+(0x032a<<2),(UINT)((_rd32(_REG_BASE_+(0x032a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_SHSO_RP2r			(ValSft_R00(_rd32(_REG_BASE_+(0x032a<<2)))&BitMask_13)
#define RYC_SHSO_RP2w(val)		_wr32(_REG_BASE_+(0x032a<<2),(UINT)((_rd32(_REG_BASE_+(0x032a<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_SVSO_FP2r			(ValSft_R16(_rd32(_REG_BASE_+(0x032b<<2)))&BitMask_12)
#define RYC_SVSO_FP2w(val)		_wr32(_REG_BASE_+(0x032b<<2),(UINT)((_rd32(_REG_BASE_+(0x032b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_SVSO_RP2r			(ValSft_R00(_rd32(_REG_BASE_+(0x032b<<2)))&BitMask_12)
#define RYC_SVSO_RP2w(val)		_wr32(_REG_BASE_+(0x032b<<2),(UINT)((_rd32(_REG_BASE_+(0x032b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_DENO_EDGE2r			(ValSft_R31(_rd32(_REG_BASE_+(0x032c<<2)))&BitMask_01)
#define RYC_DENO_EDGE2w(val)	_wr32(_REG_BASE_+(0x032c<<2),(UINT)((_rd32(_REG_BASE_+(0x032c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_DENO_VSPI2r			(ValSft_R16(_rd32(_REG_BASE_+(0x032c<<2)))&BitMask_12)
#define RYC_DENO_VSPI2w(val)	_wr32(_REG_BASE_+(0x032c<<2),(UINT)((_rd32(_REG_BASE_+(0x032c<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_DENO_VEDI2r			(ValSft_R00(_rd32(_REG_BASE_+(0x032c<<2)))&BitMask_12)
#define RYC_DENO_VEDI2w(val)	_wr32(_REG_BASE_+(0x032c<<2),(UINT)((_rd32(_REG_BASE_+(0x032c<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_TRG_OVSYNCI2r		(ValSft_R31(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_01)
#define RYC_TRG_OVSYNCI2w(val)	_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_TRG_OHSYNCI2r		(ValSft_R30(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_01)
#define RYC_TRG_OHSYNCI2w(val)	_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_DENO_HRP2r			(ValSft_R16(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_13)
#define RYC_DENO_HRP2w(val)		_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_POL_OVSYNCI2r		(ValSft_R15(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_01)
#define RYC_POL_OVSYNCI2w(val)	_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define RYC_POL_OHSYNCI2r		(ValSft_R14(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_01)
#define RYC_POL_OHSYNCI2w(val)	_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define RYC_DENO_HFP2r			(ValSft_R00(_rd32(_REG_BASE_+(0x032d<<2)))&BitMask_13)
#define RYC_DENO_HFP2w(val)		_wr32(_REG_BASE_+(0x032d<<2),(UINT)((_rd32(_REG_BASE_+(0x032d<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_OSYNC_MOD3r			(ValSft_R31(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_01)
#define RYC_OSYNC_MOD3w(val)	_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_POS_HZ3r			(ValSft_R30(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_01)
#define RYC_POS_HZ3w(val)		_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SVSO_TRG3r			(ValSft_R28(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_01)
#define RYC_SVSO_TRG3w(val)		_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define RYC_SHSO_INV3r			(ValSft_R27(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_01)
#define RYC_SHSO_INV3w(val)		_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define RYC_SVSO_INV3r			(ValSft_R26(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_01)
#define RYC_SVSO_INV3w(val)		_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define RYC_VWI3r				(ValSft_R13(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_12)
#define RYC_VWI3w(val)			_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L13(BitMask_12)))|ValSft_L13((val)&BitMask_12)))

#define RYC_HWI3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0330<<2)))&BitMask_13)
#define RYC_HWI3w(val)			_wr32(_REG_BASE_+(0x0330<<2),(UINT)((_rd32(_REG_BASE_+(0x0330<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_EXSYNC_SEL3r		(ValSft_R29(_rd32(_REG_BASE_+(0x0331<<2)))&BitMask_03)
#define RYC_EXSYNC_SEL3w(val)	_wr32(_REG_BASE_+(0x0331<<2),(UINT)((_rd32(_REG_BASE_+(0x0331<<2))&(~ValSft_L29(BitMask_03)))|ValSft_L29((val)&BitMask_03)))

#define RYC_HTWO3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0331<<2)))&BitMask_13)
#define RYC_HTWO3w(val)			_wr32(_REG_BASE_+(0x0331<<2),(UINT)((_rd32(_REG_BASE_+(0x0331<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VTWO3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0331<<2)))&BitMask_12)
#define RYC_VTWO3w(val)			_wr32(_REG_BASE_+(0x0331<<2),(UINT)((_rd32(_REG_BASE_+(0x0331<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_ITLC3r				(ValSft_R31(_rd32(_REG_BASE_+(0x0332<<2)))&BitMask_01)
#define RYC_ITLC3w(val)			_wr32(_REG_BASE_+(0x0332<<2),(UINT)((_rd32(_REG_BASE_+(0x0332<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_FLD_INV3r			(ValSft_R30(_rd32(_REG_BASE_+(0x0332<<2)))&BitMask_01)
#define RYC_FLD_INV3w(val)		_wr32(_REG_BASE_+(0x0332<<2),(UINT)((_rd32(_REG_BASE_+(0x0332<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_FLD_IMG_INV3r		(ValSft_R29(_rd32(_REG_BASE_+(0x0332<<2)))&BitMask_01)
#define RYC_FLD_IMG_INV3w(val)	_wr32(_REG_BASE_+(0x0332<<2),(UINT)((_rd32(_REG_BASE_+(0x0332<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define RYC_HSPI3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0332<<2)))&BitMask_13)
#define RYC_HSPI3w(val)			_wr32(_REG_BASE_+(0x0332<<2),(UINT)((_rd32(_REG_BASE_+(0x0332<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_VSPI3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0332<<2)))&BitMask_12)
#define RYC_VSPI3w(val)			_wr32(_REG_BASE_+(0x0332<<2),(UINT)((_rd32(_REG_BASE_+(0x0332<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_SHSO_EDGE3r			(ValSft_R31(_rd32(_REG_BASE_+(0x0333<<2)))&BitMask_01)
#define RYC_SHSO_EDGE3w(val)	_wr32(_REG_BASE_+(0x0333<<2),(UINT)((_rd32(_REG_BASE_+(0x0333<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_SVSO_EDGE3r			(ValSft_R30(_rd32(_REG_BASE_+(0x0333<<2)))&BitMask_01)
#define RYC_SVSO_EDGE3w(val)	_wr32(_REG_BASE_+(0x0333<<2),(UINT)((_rd32(_REG_BASE_+(0x0333<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_SHSO_FP3r			(ValSft_R16(_rd32(_REG_BASE_+(0x0333<<2)))&BitMask_13)
#define RYC_SHSO_FP3w(val)		_wr32(_REG_BASE_+(0x0333<<2),(UINT)((_rd32(_REG_BASE_+(0x0333<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_SHSO_RP3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0333<<2)))&BitMask_13)
#define RYC_SHSO_RP3w(val)		_wr32(_REG_BASE_+(0x0333<<2),(UINT)((_rd32(_REG_BASE_+(0x0333<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define RYC_SVSO_FP3r			(ValSft_R16(_rd32(_REG_BASE_+(0x0334<<2)))&BitMask_12)
#define RYC_SVSO_FP3w(val)		_wr32(_REG_BASE_+(0x0334<<2),(UINT)((_rd32(_REG_BASE_+(0x0334<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_SVSO_RP3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0334<<2)))&BitMask_12)
#define RYC_SVSO_RP3w(val)		_wr32(_REG_BASE_+(0x0334<<2),(UINT)((_rd32(_REG_BASE_+(0x0334<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_DENO_EDGE3r			(ValSft_R31(_rd32(_REG_BASE_+(0x0335<<2)))&BitMask_01)
#define RYC_DENO_EDGE3w(val)	_wr32(_REG_BASE_+(0x0335<<2),(UINT)((_rd32(_REG_BASE_+(0x0335<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_DENO_VSPI3r			(ValSft_R16(_rd32(_REG_BASE_+(0x0335<<2)))&BitMask_12)
#define RYC_DENO_VSPI3w(val)	_wr32(_REG_BASE_+(0x0335<<2),(UINT)((_rd32(_REG_BASE_+(0x0335<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define RYC_DENO_VEDI3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0335<<2)))&BitMask_12)
#define RYC_DENO_VEDI3w(val)	_wr32(_REG_BASE_+(0x0335<<2),(UINT)((_rd32(_REG_BASE_+(0x0335<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define RYC_TRG_OVSYNCI3r		(ValSft_R31(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_01)
#define RYC_TRG_OVSYNCI3w(val)	_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define RYC_TRG_OHSYNCI3r		(ValSft_R30(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_01)
#define RYC_TRG_OHSYNCI3w(val)	_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define RYC_DENO_HRP3r			(ValSft_R16(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_13)
#define RYC_DENO_HRP3w(val)		_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define RYC_POL_OVSYNCI3r		(ValSft_R15(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_01)
#define RYC_POL_OVSYNCI3w(val)	_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define RYC_POL_OHSYNCI3r		(ValSft_R14(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_01)
#define RYC_POL_OHSYNCI3w(val)	_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define RYC_DENO_HFP3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0336<<2)))&BitMask_13)
#define RYC_DENO_HFP3w(val)		_wr32(_REG_BASE_+(0x0336<<2),(UINT)((_rd32(_REG_BASE_+(0x0336<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define LAST_YCLIP0r			(ValSft_R24(_rd32(_REG_BASE_+(0x0340<<2)))&BitMask_08)
#define LAST_YCLIP0w(val)		_wr32(_REG_BASE_+(0x0340<<2),(UINT)((_rd32(_REG_BASE_+(0x0340<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LAST_YSLIC0r			(ValSft_R16(_rd32(_REG_BASE_+(0x0340<<2)))&BitMask_08)
#define LAST_YSLIC0w(val)		_wr32(_REG_BASE_+(0x0340<<2),(UINT)((_rd32(_REG_BASE_+(0x0340<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LAST_YOFFS0r			(ValSft_R08(_rd32(_REG_BASE_+(0x0340<<2)))&BitMask_08)
#define LAST_YOFFS0w(val)		_wr32(_REG_BASE_+(0x0340<<2),(UINT)((_rd32(_REG_BASE_+(0x0340<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LAST_CCLIP0r			(ValSft_R08(_rd32(_REG_BASE_+(0x0341<<2)))&BitMask_08)
#define LAST_CCLIP0w(val)		_wr32(_REG_BASE_+(0x0341<<2),(UINT)((_rd32(_REG_BASE_+(0x0341<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LAST_CSLIC0r			(ValSft_R00(_rd32(_REG_BASE_+(0x0341<<2)))&BitMask_08)
#define LAST_CSLIC0w(val)		_wr32(_REG_BASE_+(0x0341<<2),(UINT)((_rd32(_REG_BASE_+(0x0341<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LAST_YCLIP1r			(ValSft_R24(_rd32(_REG_BASE_+(0x0342<<2)))&BitMask_08)
#define LAST_YCLIP1w(val)		_wr32(_REG_BASE_+(0x0342<<2),(UINT)((_rd32(_REG_BASE_+(0x0342<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LAST_YSLIC1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0342<<2)))&BitMask_08)
#define LAST_YSLIC1w(val)		_wr32(_REG_BASE_+(0x0342<<2),(UINT)((_rd32(_REG_BASE_+(0x0342<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LAST_YOFFS1r			(ValSft_R08(_rd32(_REG_BASE_+(0x0342<<2)))&BitMask_08)
#define LAST_YOFFS1w(val)		_wr32(_REG_BASE_+(0x0342<<2),(UINT)((_rd32(_REG_BASE_+(0x0342<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LAST_CCLIP1r			(ValSft_R08(_rd32(_REG_BASE_+(0x0343<<2)))&BitMask_08)
#define LAST_CCLIP1w(val)		_wr32(_REG_BASE_+(0x0343<<2),(UINT)((_rd32(_REG_BASE_+(0x0343<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LAST_CSLIC1r			(ValSft_R00(_rd32(_REG_BASE_+(0x0343<<2)))&BitMask_08)
#define LAST_CSLIC1w(val)		_wr32(_REG_BASE_+(0x0343<<2),(UINT)((_rd32(_REG_BASE_+(0x0343<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ITU_SSEL0r				(ValSft_R24(_rd32(_REG_BASE_+(0x0344<<2)))&BitMask_04)
#define ITU_SSEL0w(val)			_wr32(_REG_BASE_+(0x0344<<2),(UINT)((_rd32(_REG_BASE_+(0x0344<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define ITU_ASEL0r				(ValSft_R16(_rd32(_REG_BASE_+(0x0344<<2)))&BitMask_05)
#define ITU_ASEL0w(val)			_wr32(_REG_BASE_+(0x0344<<2),(UINT)((_rd32(_REG_BASE_+(0x0344<<2))&(~ValSft_L16(BitMask_05)))|ValSft_L16((val)&BitMask_05)))

#define ITU_SSEL1r				(ValSft_R08(_rd32(_REG_BASE_+(0x0344<<2)))&BitMask_04)
#define ITU_SSEL1w(val)			_wr32(_REG_BASE_+(0x0344<<2),(UINT)((_rd32(_REG_BASE_+(0x0344<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define ITU_ASEL1r				(ValSft_R00(_rd32(_REG_BASE_+(0x0344<<2)))&BitMask_05)
#define ITU_ASEL1w(val)			_wr32(_REG_BASE_+(0x0344<<2),(UINT)((_rd32(_REG_BASE_+(0x0344<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define D_LNUM_EXT_ON0r			(ValSft_R29(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_LNUM_EXT_ON0w(val)	_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define D_ITU_LCON0r			(ValSft_R28(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_LCON0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define D_ITU_VID0r				(ValSft_R27(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_VID0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define D_ITU_FID0r				(ValSft_R26(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_FID0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define D_ITU_YCCH0r			(ValSft_R25(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_YCCH0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define D_BT20_ISEL0r			(ValSft_R20(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_03)
#define D_BT20_ISEL0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define D_BT20_OSEL0r			(ValSft_R16(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_03)
#define D_BT20_OSEL0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define D_ITU_DO_BT656_ON0r		(ValSft_R15(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_DO_BT656_ON0w(val)	_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define D_ITU_CTOG_INV0r		(ValSft_R14(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_CTOG_INV0w(val)	_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define D_ITU_OMOD0r			(ValSft_R11(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_OMOD0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define D_ITU_OYSEL0r			(ValSft_R09(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_02)
#define D_ITU_OYSEL0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L09(BitMask_02)))|ValSft_L09((val)&BitMask_02)))

#define D_ITU_OCSEL0r			(ValSft_R07(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_02)
#define D_ITU_OCSEL0w(val)		_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L07(BitMask_02)))|ValSft_L07((val)&BitMask_02)))

#define D_ITU_DOFF_RS0r			(ValSft_R06(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_DOFF_RS0w(val)	_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define D_ITU_FIFO_ON0r			(ValSft_R05(_rd32(_REG_BASE_+(0x0345<<2)))&BitMask_01)
#define D_ITU_FIFO_ON0w(val)	_wr32(_REG_BASE_+(0x0345<<2),(UINT)((_rd32(_REG_BASE_+(0x0345<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define D_LNUM_EXT_ON1r			(ValSft_R29(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_LNUM_EXT_ON1w(val)	_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define D_ITU_LCON1r			(ValSft_R28(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_LCON1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define D_ITU_VID1r				(ValSft_R27(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_VID1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define D_ITU_FID1r				(ValSft_R26(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_FID1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define D_ITU_YCCH1r			(ValSft_R25(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_YCCH1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define D_BT20_ISEL1r			(ValSft_R20(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_03)
#define D_BT20_ISEL1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L20(BitMask_03)))|ValSft_L20((val)&BitMask_03)))

#define D_BT20_OSEL1r			(ValSft_R16(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_03)
#define D_BT20_OSEL1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L16(BitMask_03)))|ValSft_L16((val)&BitMask_03)))

#define D_ITU_DO_BT656_ON1r		(ValSft_R15(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_DO_BT656_ON1w(val)	_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define D_ITU_CTOG_INV1r		(ValSft_R14(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_CTOG_INV1w(val)	_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define D_ITU_OMOD1r			(ValSft_R11(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_OMOD1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L11(BitMask_01)))|ValSft_L11((val)&BitMask_01)))

#define D_ITU_OYSEL1r			(ValSft_R09(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_02)
#define D_ITU_OYSEL1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L09(BitMask_02)))|ValSft_L09((val)&BitMask_02)))

#define D_ITU_OCSEL1r			(ValSft_R07(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_02)
#define D_ITU_OCSEL1w(val)		_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L07(BitMask_02)))|ValSft_L07((val)&BitMask_02)))

#define D_ITU_DOFF_RS1r			(ValSft_R06(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_DOFF_RS1w(val)	_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define D_ITU_FIFO_ON1r			(ValSft_R05(_rd32(_REG_BASE_+(0x0346<<2)))&BitMask_01)
#define D_ITU_FIFO_ON1w(val)	_wr32(_REG_BASE_+(0x0346<<2),(UINT)((_rd32(_REG_BASE_+(0x0346<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define VSO_OMODr				(ValSft_R17(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_03)
#define VSO_OMODw(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L17(BitMask_03)))|ValSft_L17((val)&BitMask_03)))

#define HSO_OMODr				(ValSft_R14(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_03)
#define HSO_OMODw(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L14(BitMask_03)))|ValSft_L14((val)&BitMask_03)))

#define DENO_OMODr				(ValSft_R11(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_03)
#define DENO_OMODw(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L11(BitMask_03)))|ValSft_L11((val)&BitMask_03)))

#define DOMODEr					(ValSft_R08(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_03)
#define DOMODEw(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L08(BitMask_03)))|ValSft_L08((val)&BitMask_03)))

#define DENO_MAN_ON1r			(ValSft_R05(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_MAN_ON1w(val)		_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define DENO_INV1r				(ValSft_R04(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_INV1w(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L04(BitMask_01)))|ValSft_L04((val)&BitMask_01)))

#define DENO_OEN1r				(ValSft_R03(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_OEN1w(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define DENO_MAN_ON0r			(ValSft_R02(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_MAN_ON0w(val)		_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define DENO_INV0r				(ValSft_R01(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_INV0w(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define DENO_OEN0r				(ValSft_R00(_rd32(_REG_BASE_+(0x0347<<2)))&BitMask_01)
#define DENO_OEN0w(val)			_wr32(_REG_BASE_+(0x0347<<2),(UINT)((_rd32(_REG_BASE_+(0x0347<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define LCD0_NFSELr				(ValSft_R31(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_01)
#define LCD0_NFSELw(val)		_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LCD0_TPAT_ONr			(ValSft_R27(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_03)
#define LCD0_TPAT_ONw(val)		_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L27(BitMask_03)))|ValSft_L27((val)&BitMask_03)))

#define LCD0_DSELr				(ValSft_R24(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_03)
#define LCD0_DSELw(val)			_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define LCD0_VSELr				(ValSft_R20(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_04)
#define LCD0_VSELw(val)			_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define LRR0_VALr				(ValSft_R10(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_10)
#define LRR0_VALw(val)			_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define LGR0_VALr				(ValSft_R00(_rd32(_REG_BASE_+(0x0348<<2)))&BitMask_10)
#define LGR0_VALw(val)			_wr32(_REG_BASE_+(0x0348<<2),(UINT)((_rd32(_REG_BASE_+(0x0348<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define LGB0_VALr				(ValSft_R10(_rd32(_REG_BASE_+(0x0349<<2)))&BitMask_10)
#define LGB0_VALw(val)			_wr32(_REG_BASE_+(0x0349<<2),(UINT)((_rd32(_REG_BASE_+(0x0349<<2))&(~ValSft_L10(BitMask_10)))|ValSft_L10((val)&BitMask_10)))

#define LBB0_VALr				(ValSft_R00(_rd32(_REG_BASE_+(0x0349<<2)))&BitMask_10)
#define LBB0_VALw(val)			_wr32(_REG_BASE_+(0x0349<<2),(UINT)((_rd32(_REG_BASE_+(0x0349<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define LCD0_PMODr				(ValSft_R28(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_04)
#define LCD0_PMODw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define LCD0_ICr				(ValSft_R27(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_ICw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define LCD0FF_RSr				(ValSft_R26(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0FF_RSw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define LCD0_WIMGr				(ValSft_R25(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_WIMGw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define LCD0_WRXr				(ValSft_R24(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_WRXw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LCD0_DCXr				(ValSft_R23(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_DCXw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define LCD0_CSXr				(ValSft_R22(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_CSXw(val)			_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define LCD0_INST_CONr			(ValSft_R21(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_INST_CONw(val)		_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define LCD0_DCX_SELr			(ValSft_R20(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_DCX_SELw(val)		_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define LCD0_SPI_LSBr			(ValSft_R19(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_01)
#define LCD0_SPI_LSBw(val)		_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L19(BitMask_01)))|ValSft_L19((val)&BitMask_01)))

#define LCD0_INST_STEPr			(ValSft_R00(_rd32(_REG_BASE_+(0x034b<<2)))&BitMask_05)
#define LCD0_INST_STEPw(val)	_wr32(_REG_BASE_+(0x034b<<2),(UINT)((_rd32(_REG_BASE_+(0x034b<<2))&(~ValSft_L00(BitMask_05)))|ValSft_L00((val)&BitMask_05)))

#define LCD0_INSTPARA0r			(ValSft_R24(_rd32(_REG_BASE_+(0x034c<<2)))&BitMask_08)
#define LCD0_INSTPARA0w(val)	_wr32(_REG_BASE_+(0x034c<<2),(UINT)((_rd32(_REG_BASE_+(0x034c<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LCD0_INSTPARA1r			(ValSft_R16(_rd32(_REG_BASE_+(0x034c<<2)))&BitMask_08)
#define LCD0_INSTPARA1w(val)	_wr32(_REG_BASE_+(0x034c<<2),(UINT)((_rd32(_REG_BASE_+(0x034c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LCD0_INSTPARA2r			(ValSft_R08(_rd32(_REG_BASE_+(0x034c<<2)))&BitMask_08)
#define LCD0_INSTPARA2w(val)	_wr32(_REG_BASE_+(0x034c<<2),(UINT)((_rd32(_REG_BASE_+(0x034c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LCD0_INSTPARA3r			(ValSft_R00(_rd32(_REG_BASE_+(0x034c<<2)))&BitMask_08)
#define LCD0_INSTPARA3w(val)	_wr32(_REG_BASE_+(0x034c<<2),(UINT)((_rd32(_REG_BASE_+(0x034c<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LCD0_INSTPARA4r			(ValSft_R24(_rd32(_REG_BASE_+(0x034d<<2)))&BitMask_08)
#define LCD0_INSTPARA4w(val)	_wr32(_REG_BASE_+(0x034d<<2),(UINT)((_rd32(_REG_BASE_+(0x034d<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LCD0_INSTPARA5r			(ValSft_R16(_rd32(_REG_BASE_+(0x034d<<2)))&BitMask_08)
#define LCD0_INSTPARA5w(val)	_wr32(_REG_BASE_+(0x034d<<2),(UINT)((_rd32(_REG_BASE_+(0x034d<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LCD0_INSTPARA6r			(ValSft_R08(_rd32(_REG_BASE_+(0x034d<<2)))&BitMask_08)
#define LCD0_INSTPARA6w(val)	_wr32(_REG_BASE_+(0x034d<<2),(UINT)((_rd32(_REG_BASE_+(0x034d<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LCD0_INSTPARA7r			(ValSft_R00(_rd32(_REG_BASE_+(0x034d<<2)))&BitMask_08)
#define LCD0_INSTPARA7w(val)	_wr32(_REG_BASE_+(0x034d<<2),(UINT)((_rd32(_REG_BASE_+(0x034d<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LCD0_INSTPARA8r			(ValSft_R24(_rd32(_REG_BASE_+(0x034e<<2)))&BitMask_08)
#define LCD0_INSTPARA8w(val)	_wr32(_REG_BASE_+(0x034e<<2),(UINT)((_rd32(_REG_BASE_+(0x034e<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LCD0_INSTPARA9r			(ValSft_R16(_rd32(_REG_BASE_+(0x034e<<2)))&BitMask_08)
#define LCD0_INSTPARA9w(val)	_wr32(_REG_BASE_+(0x034e<<2),(UINT)((_rd32(_REG_BASE_+(0x034e<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LCD0_INSTPARA10r		(ValSft_R08(_rd32(_REG_BASE_+(0x034e<<2)))&BitMask_08)
#define LCD0_INSTPARA10w(val)	_wr32(_REG_BASE_+(0x034e<<2),(UINT)((_rd32(_REG_BASE_+(0x034e<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LCD0_INSTPARA11r		(ValSft_R00(_rd32(_REG_BASE_+(0x034e<<2)))&BitMask_08)
#define LCD0_INSTPARA11w(val)	_wr32(_REG_BASE_+(0x034e<<2),(UINT)((_rd32(_REG_BASE_+(0x034e<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LCD0_INSTPARA12r		(ValSft_R24(_rd32(_REG_BASE_+(0x034f<<2)))&BitMask_08)
#define LCD0_INSTPARA12w(val)	_wr32(_REG_BASE_+(0x034f<<2),(UINT)((_rd32(_REG_BASE_+(0x034f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LCD0_INSTPARA13r		(ValSft_R16(_rd32(_REG_BASE_+(0x034f<<2)))&BitMask_08)
#define LCD0_INSTPARA13w(val)	_wr32(_REG_BASE_+(0x034f<<2),(UINT)((_rd32(_REG_BASE_+(0x034f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LCD0_INSTPARA14r		(ValSft_R08(_rd32(_REG_BASE_+(0x034f<<2)))&BitMask_08)
#define LCD0_INSTPARA14w(val)	_wr32(_REG_BASE_+(0x034f<<2),(UINT)((_rd32(_REG_BASE_+(0x034f<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LCD0_INSTPARA15r		(ValSft_R00(_rd32(_REG_BASE_+(0x034f<<2)))&BitMask_08)
#define LCD0_INSTPARA15w(val)	_wr32(_REG_BASE_+(0x034f<<2),(UINT)((_rd32(_REG_BASE_+(0x034f<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC0r				(ValSft_R24(_rd32(_REG_BASE_+(0x0350<<2)))&BitMask_08)
#define LC_PVC0w(val)			_wr32(_REG_BASE_+(0x0350<<2),(UINT)((_rd32(_REG_BASE_+(0x0350<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0350<<2)))&BitMask_08)
#define LC_PVC1w(val)			_wr32(_REG_BASE_+(0x0350<<2),(UINT)((_rd32(_REG_BASE_+(0x0350<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC2r				(ValSft_R08(_rd32(_REG_BASE_+(0x0350<<2)))&BitMask_08)
#define LC_PVC2w(val)			_wr32(_REG_BASE_+(0x0350<<2),(UINT)((_rd32(_REG_BASE_+(0x0350<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC3r				(ValSft_R00(_rd32(_REG_BASE_+(0x0350<<2)))&BitMask_08)
#define LC_PVC3w(val)			_wr32(_REG_BASE_+(0x0350<<2),(UINT)((_rd32(_REG_BASE_+(0x0350<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC4r				(ValSft_R24(_rd32(_REG_BASE_+(0x0351<<2)))&BitMask_08)
#define LC_PVC4w(val)			_wr32(_REG_BASE_+(0x0351<<2),(UINT)((_rd32(_REG_BASE_+(0x0351<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC5r				(ValSft_R16(_rd32(_REG_BASE_+(0x0351<<2)))&BitMask_08)
#define LC_PVC5w(val)			_wr32(_REG_BASE_+(0x0351<<2),(UINT)((_rd32(_REG_BASE_+(0x0351<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC6r				(ValSft_R08(_rd32(_REG_BASE_+(0x0351<<2)))&BitMask_08)
#define LC_PVC6w(val)			_wr32(_REG_BASE_+(0x0351<<2),(UINT)((_rd32(_REG_BASE_+(0x0351<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC7r				(ValSft_R00(_rd32(_REG_BASE_+(0x0351<<2)))&BitMask_08)
#define LC_PVC7w(val)			_wr32(_REG_BASE_+(0x0351<<2),(UINT)((_rd32(_REG_BASE_+(0x0351<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC8r				(ValSft_R24(_rd32(_REG_BASE_+(0x0352<<2)))&BitMask_08)
#define LC_PVC8w(val)			_wr32(_REG_BASE_+(0x0352<<2),(UINT)((_rd32(_REG_BASE_+(0x0352<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC9r				(ValSft_R16(_rd32(_REG_BASE_+(0x0352<<2)))&BitMask_08)
#define LC_PVC9w(val)			_wr32(_REG_BASE_+(0x0352<<2),(UINT)((_rd32(_REG_BASE_+(0x0352<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC10r				(ValSft_R08(_rd32(_REG_BASE_+(0x0352<<2)))&BitMask_08)
#define LC_PVC10w(val)			_wr32(_REG_BASE_+(0x0352<<2),(UINT)((_rd32(_REG_BASE_+(0x0352<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC11r				(ValSft_R00(_rd32(_REG_BASE_+(0x0352<<2)))&BitMask_08)
#define LC_PVC11w(val)			_wr32(_REG_BASE_+(0x0352<<2),(UINT)((_rd32(_REG_BASE_+(0x0352<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC12r				(ValSft_R24(_rd32(_REG_BASE_+(0x0353<<2)))&BitMask_08)
#define LC_PVC12w(val)			_wr32(_REG_BASE_+(0x0353<<2),(UINT)((_rd32(_REG_BASE_+(0x0353<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC13r				(ValSft_R16(_rd32(_REG_BASE_+(0x0353<<2)))&BitMask_08)
#define LC_PVC13w(val)			_wr32(_REG_BASE_+(0x0353<<2),(UINT)((_rd32(_REG_BASE_+(0x0353<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC14r				(ValSft_R08(_rd32(_REG_BASE_+(0x0353<<2)))&BitMask_08)
#define LC_PVC14w(val)			_wr32(_REG_BASE_+(0x0353<<2),(UINT)((_rd32(_REG_BASE_+(0x0353<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC15r				(ValSft_R00(_rd32(_REG_BASE_+(0x0353<<2)))&BitMask_08)
#define LC_PVC15w(val)			_wr32(_REG_BASE_+(0x0353<<2),(UINT)((_rd32(_REG_BASE_+(0x0353<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC16r				(ValSft_R24(_rd32(_REG_BASE_+(0x0354<<2)))&BitMask_08)
#define LC_PVC16w(val)			_wr32(_REG_BASE_+(0x0354<<2),(UINT)((_rd32(_REG_BASE_+(0x0354<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC17r				(ValSft_R16(_rd32(_REG_BASE_+(0x0354<<2)))&BitMask_08)
#define LC_PVC17w(val)			_wr32(_REG_BASE_+(0x0354<<2),(UINT)((_rd32(_REG_BASE_+(0x0354<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC18r				(ValSft_R08(_rd32(_REG_BASE_+(0x0354<<2)))&BitMask_08)
#define LC_PVC18w(val)			_wr32(_REG_BASE_+(0x0354<<2),(UINT)((_rd32(_REG_BASE_+(0x0354<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC19r				(ValSft_R00(_rd32(_REG_BASE_+(0x0354<<2)))&BitMask_08)
#define LC_PVC19w(val)			_wr32(_REG_BASE_+(0x0354<<2),(UINT)((_rd32(_REG_BASE_+(0x0354<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC20r				(ValSft_R24(_rd32(_REG_BASE_+(0x0355<<2)))&BitMask_08)
#define LC_PVC20w(val)			_wr32(_REG_BASE_+(0x0355<<2),(UINT)((_rd32(_REG_BASE_+(0x0355<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC21r				(ValSft_R16(_rd32(_REG_BASE_+(0x0355<<2)))&BitMask_08)
#define LC_PVC21w(val)			_wr32(_REG_BASE_+(0x0355<<2),(UINT)((_rd32(_REG_BASE_+(0x0355<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC22r				(ValSft_R08(_rd32(_REG_BASE_+(0x0355<<2)))&BitMask_08)
#define LC_PVC22w(val)			_wr32(_REG_BASE_+(0x0355<<2),(UINT)((_rd32(_REG_BASE_+(0x0355<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC23r				(ValSft_R00(_rd32(_REG_BASE_+(0x0355<<2)))&BitMask_08)
#define LC_PVC23w(val)			_wr32(_REG_BASE_+(0x0355<<2),(UINT)((_rd32(_REG_BASE_+(0x0355<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC24r				(ValSft_R24(_rd32(_REG_BASE_+(0x0356<<2)))&BitMask_08)
#define LC_PVC24w(val)			_wr32(_REG_BASE_+(0x0356<<2),(UINT)((_rd32(_REG_BASE_+(0x0356<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC25r				(ValSft_R16(_rd32(_REG_BASE_+(0x0356<<2)))&BitMask_08)
#define LC_PVC25w(val)			_wr32(_REG_BASE_+(0x0356<<2),(UINT)((_rd32(_REG_BASE_+(0x0356<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC26r				(ValSft_R08(_rd32(_REG_BASE_+(0x0356<<2)))&BitMask_08)
#define LC_PVC26w(val)			_wr32(_REG_BASE_+(0x0356<<2),(UINT)((_rd32(_REG_BASE_+(0x0356<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC27r				(ValSft_R00(_rd32(_REG_BASE_+(0x0356<<2)))&BitMask_08)
#define LC_PVC27w(val)			_wr32(_REG_BASE_+(0x0356<<2),(UINT)((_rd32(_REG_BASE_+(0x0356<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PVC28r				(ValSft_R24(_rd32(_REG_BASE_+(0x0357<<2)))&BitMask_08)
#define LC_PVC28w(val)			_wr32(_REG_BASE_+(0x0357<<2),(UINT)((_rd32(_REG_BASE_+(0x0357<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PVC29r				(ValSft_R16(_rd32(_REG_BASE_+(0x0357<<2)))&BitMask_08)
#define LC_PVC29w(val)			_wr32(_REG_BASE_+(0x0357<<2),(UINT)((_rd32(_REG_BASE_+(0x0357<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PVC30r				(ValSft_R08(_rd32(_REG_BASE_+(0x0357<<2)))&BitMask_08)
#define LC_PVC30w(val)			_wr32(_REG_BASE_+(0x0357<<2),(UINT)((_rd32(_REG_BASE_+(0x0357<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PVC31r				(ValSft_R00(_rd32(_REG_BASE_+(0x0357<<2)))&BitMask_08)
#define LC_PVC31w(val)			_wr32(_REG_BASE_+(0x0357<<2),(UINT)((_rd32(_REG_BASE_+(0x0357<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV0r					(ValSft_R24(_rd32(_REG_BASE_+(0x0358<<2)))&BitMask_08)
#define LC_PV0w(val)			_wr32(_REG_BASE_+(0x0358<<2),(UINT)((_rd32(_REG_BASE_+(0x0358<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV1r					(ValSft_R16(_rd32(_REG_BASE_+(0x0358<<2)))&BitMask_08)
#define LC_PV1w(val)			_wr32(_REG_BASE_+(0x0358<<2),(UINT)((_rd32(_REG_BASE_+(0x0358<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV2r					(ValSft_R08(_rd32(_REG_BASE_+(0x0358<<2)))&BitMask_08)
#define LC_PV2w(val)			_wr32(_REG_BASE_+(0x0358<<2),(UINT)((_rd32(_REG_BASE_+(0x0358<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV3r					(ValSft_R00(_rd32(_REG_BASE_+(0x0358<<2)))&BitMask_08)
#define LC_PV3w(val)			_wr32(_REG_BASE_+(0x0358<<2),(UINT)((_rd32(_REG_BASE_+(0x0358<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV4r					(ValSft_R24(_rd32(_REG_BASE_+(0x0359<<2)))&BitMask_08)
#define LC_PV4w(val)			_wr32(_REG_BASE_+(0x0359<<2),(UINT)((_rd32(_REG_BASE_+(0x0359<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV5r					(ValSft_R16(_rd32(_REG_BASE_+(0x0359<<2)))&BitMask_08)
#define LC_PV5w(val)			_wr32(_REG_BASE_+(0x0359<<2),(UINT)((_rd32(_REG_BASE_+(0x0359<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV6r					(ValSft_R08(_rd32(_REG_BASE_+(0x0359<<2)))&BitMask_08)
#define LC_PV6w(val)			_wr32(_REG_BASE_+(0x0359<<2),(UINT)((_rd32(_REG_BASE_+(0x0359<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV7r					(ValSft_R00(_rd32(_REG_BASE_+(0x0359<<2)))&BitMask_08)
#define LC_PV7w(val)			_wr32(_REG_BASE_+(0x0359<<2),(UINT)((_rd32(_REG_BASE_+(0x0359<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV8r					(ValSft_R24(_rd32(_REG_BASE_+(0x035a<<2)))&BitMask_08)
#define LC_PV8w(val)			_wr32(_REG_BASE_+(0x035a<<2),(UINT)((_rd32(_REG_BASE_+(0x035a<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV9r					(ValSft_R16(_rd32(_REG_BASE_+(0x035a<<2)))&BitMask_08)
#define LC_PV9w(val)			_wr32(_REG_BASE_+(0x035a<<2),(UINT)((_rd32(_REG_BASE_+(0x035a<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV10r				(ValSft_R08(_rd32(_REG_BASE_+(0x035a<<2)))&BitMask_08)
#define LC_PV10w(val)			_wr32(_REG_BASE_+(0x035a<<2),(UINT)((_rd32(_REG_BASE_+(0x035a<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV11r				(ValSft_R00(_rd32(_REG_BASE_+(0x035a<<2)))&BitMask_08)
#define LC_PV11w(val)			_wr32(_REG_BASE_+(0x035a<<2),(UINT)((_rd32(_REG_BASE_+(0x035a<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV12r				(ValSft_R24(_rd32(_REG_BASE_+(0x035b<<2)))&BitMask_08)
#define LC_PV12w(val)			_wr32(_REG_BASE_+(0x035b<<2),(UINT)((_rd32(_REG_BASE_+(0x035b<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV13r				(ValSft_R16(_rd32(_REG_BASE_+(0x035b<<2)))&BitMask_08)
#define LC_PV13w(val)			_wr32(_REG_BASE_+(0x035b<<2),(UINT)((_rd32(_REG_BASE_+(0x035b<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV14r				(ValSft_R08(_rd32(_REG_BASE_+(0x035b<<2)))&BitMask_08)
#define LC_PV14w(val)			_wr32(_REG_BASE_+(0x035b<<2),(UINT)((_rd32(_REG_BASE_+(0x035b<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV15r				(ValSft_R00(_rd32(_REG_BASE_+(0x035b<<2)))&BitMask_08)
#define LC_PV15w(val)			_wr32(_REG_BASE_+(0x035b<<2),(UINT)((_rd32(_REG_BASE_+(0x035b<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV16r				(ValSft_R24(_rd32(_REG_BASE_+(0x035c<<2)))&BitMask_08)
#define LC_PV16w(val)			_wr32(_REG_BASE_+(0x035c<<2),(UINT)((_rd32(_REG_BASE_+(0x035c<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV17r				(ValSft_R16(_rd32(_REG_BASE_+(0x035c<<2)))&BitMask_08)
#define LC_PV17w(val)			_wr32(_REG_BASE_+(0x035c<<2),(UINT)((_rd32(_REG_BASE_+(0x035c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV18r				(ValSft_R08(_rd32(_REG_BASE_+(0x035c<<2)))&BitMask_08)
#define LC_PV18w(val)			_wr32(_REG_BASE_+(0x035c<<2),(UINT)((_rd32(_REG_BASE_+(0x035c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV19r				(ValSft_R00(_rd32(_REG_BASE_+(0x035c<<2)))&BitMask_08)
#define LC_PV19w(val)			_wr32(_REG_BASE_+(0x035c<<2),(UINT)((_rd32(_REG_BASE_+(0x035c<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV20r				(ValSft_R24(_rd32(_REG_BASE_+(0x035d<<2)))&BitMask_08)
#define LC_PV20w(val)			_wr32(_REG_BASE_+(0x035d<<2),(UINT)((_rd32(_REG_BASE_+(0x035d<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV21r				(ValSft_R16(_rd32(_REG_BASE_+(0x035d<<2)))&BitMask_08)
#define LC_PV21w(val)			_wr32(_REG_BASE_+(0x035d<<2),(UINT)((_rd32(_REG_BASE_+(0x035d<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV22r				(ValSft_R08(_rd32(_REG_BASE_+(0x035d<<2)))&BitMask_08)
#define LC_PV22w(val)			_wr32(_REG_BASE_+(0x035d<<2),(UINT)((_rd32(_REG_BASE_+(0x035d<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV23r				(ValSft_R00(_rd32(_REG_BASE_+(0x035d<<2)))&BitMask_08)
#define LC_PV23w(val)			_wr32(_REG_BASE_+(0x035d<<2),(UINT)((_rd32(_REG_BASE_+(0x035d<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV24r				(ValSft_R24(_rd32(_REG_BASE_+(0x035e<<2)))&BitMask_08)
#define LC_PV24w(val)			_wr32(_REG_BASE_+(0x035e<<2),(UINT)((_rd32(_REG_BASE_+(0x035e<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV25r				(ValSft_R16(_rd32(_REG_BASE_+(0x035e<<2)))&BitMask_08)
#define LC_PV25w(val)			_wr32(_REG_BASE_+(0x035e<<2),(UINT)((_rd32(_REG_BASE_+(0x035e<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV26r				(ValSft_R08(_rd32(_REG_BASE_+(0x035e<<2)))&BitMask_08)
#define LC_PV26w(val)			_wr32(_REG_BASE_+(0x035e<<2),(UINT)((_rd32(_REG_BASE_+(0x035e<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV27r				(ValSft_R00(_rd32(_REG_BASE_+(0x035e<<2)))&BitMask_08)
#define LC_PV27w(val)			_wr32(_REG_BASE_+(0x035e<<2),(UINT)((_rd32(_REG_BASE_+(0x035e<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_PV28r				(ValSft_R24(_rd32(_REG_BASE_+(0x035f<<2)))&BitMask_08)
#define LC_PV28w(val)			_wr32(_REG_BASE_+(0x035f<<2),(UINT)((_rd32(_REG_BASE_+(0x035f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define LC_PV29r				(ValSft_R16(_rd32(_REG_BASE_+(0x035f<<2)))&BitMask_08)
#define LC_PV29w(val)			_wr32(_REG_BASE_+(0x035f<<2),(UINT)((_rd32(_REG_BASE_+(0x035f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define LC_PV30r				(ValSft_R08(_rd32(_REG_BASE_+(0x035f<<2)))&BitMask_08)
#define LC_PV30w(val)			_wr32(_REG_BASE_+(0x035f<<2),(UINT)((_rd32(_REG_BASE_+(0x035f<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define LC_PV31r				(ValSft_R00(_rd32(_REG_BASE_+(0x035f<<2)))&BitMask_08)
#define LC_PV31w(val)			_wr32(_REG_BASE_+(0x035f<<2),(UINT)((_rd32(_REG_BASE_+(0x035f<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define LC_WENC_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0360<<2)))&BitMask_01)
#define LC_WENC_ONw(val)		_wr32(_REG_BASE_+(0x0360<<2),(UINT)((_rd32(_REG_BASE_+(0x0360<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LC_RENC_ONr				(ValSft_R30(_rd32(_REG_BASE_+(0x0360<<2)))&BitMask_01)
#define LC_RENC_ONw(val)		_wr32(_REG_BASE_+(0x0360<<2),(UINT)((_rd32(_REG_BASE_+(0x0360<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LC_ENC_BMODEr			(ValSft_R28(_rd32(_REG_BASE_+(0x0360<<2)))&BitMask_02)
#define LC_ENC_BMODEw(val)		_wr32(_REG_BASE_+(0x0360<<2),(UINT)((_rd32(_REG_BASE_+(0x0360<<2))&(~ValSft_L28(BitMask_02)))|ValSft_L28((val)&BitMask_02)))

#define LC_WON0r				(ValSft_R31(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_WON0w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define LC_WON1r				(ValSft_R30(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_WON1w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define LC_WON2r				(ValSft_R29(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_WON2w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define LC_WON3r				(ValSft_R28(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_WON3w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define LC_WON4r				(ValSft_R27(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_WON4w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define LC_RON0r				(ValSft_R26(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_RON0w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define LC_RON1r				(ValSft_R25(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_RON1w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define LC_RON2r				(ValSft_R24(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_RON2w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L24(BitMask_01)))|ValSft_L24((val)&BitMask_01)))

#define LC_RON3r				(ValSft_R23(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_RON3w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define LC_RON4r				(ValSft_R22(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_01)
#define LC_RON4w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define LC_MODEY0r				(ValSft_R20(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEY0w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L20(BitMask_02)))|ValSft_L20((val)&BitMask_02)))

#define LC_MODEY1r				(ValSft_R18(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEY1w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L18(BitMask_02)))|ValSft_L18((val)&BitMask_02)))

#define LC_MODEY2r				(ValSft_R16(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEY2w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L16(BitMask_02)))|ValSft_L16((val)&BitMask_02)))

#define LC_MODEY3r				(ValSft_R14(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEY3w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L14(BitMask_02)))|ValSft_L14((val)&BitMask_02)))

#define LC_MODEY4r				(ValSft_R12(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEY4w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L12(BitMask_02)))|ValSft_L12((val)&BitMask_02)))

#define LC_MODEC0r				(ValSft_R10(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEC0w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L10(BitMask_02)))|ValSft_L10((val)&BitMask_02)))

#define LC_MODEC1r				(ValSft_R08(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEC1w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L08(BitMask_02)))|ValSft_L08((val)&BitMask_02)))

#define LC_MODEC2r				(ValSft_R06(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEC2w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L06(BitMask_02)))|ValSft_L06((val)&BitMask_02)))

#define LC_MODEC3r				(ValSft_R04(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEC3w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L04(BitMask_02)))|ValSft_L04((val)&BitMask_02)))

#define LC_MODEC4r				(ValSft_R02(_rd32(_REG_BASE_+(0x0361<<2)))&BitMask_02)
#define LC_MODEC4w(val)			_wr32(_REG_BASE_+(0x0361<<2),(UINT)((_rd32(_REG_BASE_+(0x0361<<2))&(~ValSft_L02(BitMask_02)))|ValSft_L02((val)&BitMask_02)))

#define APT_ROI0_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0365<<2)))&BitMask_01)
#define APT_ROI0_ONw(val)		_wr32(_REG_BASE_+(0x0365<<2),(UINT)((_rd32(_REG_BASE_+(0x0365<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_ROI0_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x0365<<2)))&BitMask_01)
#define APT_ROI0_OSD_ONw(val)	_wr32(_REG_BASE_+(0x0365<<2),(UINT)((_rd32(_REG_BASE_+(0x0365<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_ROI0_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0365<<2)))&BitMask_11)
#define APT_ROI0_HSTw(val)		_wr32(_REG_BASE_+(0x0365<<2),(UINT)((_rd32(_REG_BASE_+(0x0365<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define APT_ROI0_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0365<<2)))&BitMask_11)
#define APT_ROI0_HEDw(val)		_wr32(_REG_BASE_+(0x0365<<2),(UINT)((_rd32(_REG_BASE_+(0x0365<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ROI0_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0366<<2)))&BitMask_10)
#define APT_ROI0_VSTw(val)		_wr32(_REG_BASE_+(0x0366<<2),(UINT)((_rd32(_REG_BASE_+(0x0366<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define APT_ROI0_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0366<<2)))&BitMask_10)
#define APT_ROI0_VEDw(val)		_wr32(_REG_BASE_+(0x0366<<2),(UINT)((_rd32(_REG_BASE_+(0x0366<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_ROI1_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0367<<2)))&BitMask_01)
#define APT_ROI1_ONw(val)		_wr32(_REG_BASE_+(0x0367<<2),(UINT)((_rd32(_REG_BASE_+(0x0367<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_ROI1_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x0367<<2)))&BitMask_01)
#define APT_ROI1_OSD_ONw(val)	_wr32(_REG_BASE_+(0x0367<<2),(UINT)((_rd32(_REG_BASE_+(0x0367<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_ROI1_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0367<<2)))&BitMask_11)
#define APT_ROI1_HSTw(val)		_wr32(_REG_BASE_+(0x0367<<2),(UINT)((_rd32(_REG_BASE_+(0x0367<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define APT_ROI1_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0367<<2)))&BitMask_11)
#define APT_ROI1_HEDw(val)		_wr32(_REG_BASE_+(0x0367<<2),(UINT)((_rd32(_REG_BASE_+(0x0367<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ROI1_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0368<<2)))&BitMask_10)
#define APT_ROI1_VSTw(val)		_wr32(_REG_BASE_+(0x0368<<2),(UINT)((_rd32(_REG_BASE_+(0x0368<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define APT_ROI1_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0368<<2)))&BitMask_10)
#define APT_ROI1_VEDw(val)		_wr32(_REG_BASE_+(0x0368<<2),(UINT)((_rd32(_REG_BASE_+(0x0368<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_ROI1_GAIN1_POSr		(ValSft_R24(_rd32(_REG_BASE_+(0x0369<<2)))&BitMask_08)
#define APT_ROI1_GAIN1_POSw(val)	_wr32(_REG_BASE_+(0x0369<<2),(UINT)((_rd32(_REG_BASE_+(0x0369<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_ROI1_GAIN2_POSr		(ValSft_R16(_rd32(_REG_BASE_+(0x0369<<2)))&BitMask_08)
#define APT_ROI1_GAIN2_POSw(val)	_wr32(_REG_BASE_+(0x0369<<2),(UINT)((_rd32(_REG_BASE_+(0x0369<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_ROI1_GAIN1_NEGr		(ValSft_R08(_rd32(_REG_BASE_+(0x0369<<2)))&BitMask_08)
#define APT_ROI1_GAIN1_NEGw(val)	_wr32(_REG_BASE_+(0x0369<<2),(UINT)((_rd32(_REG_BASE_+(0x0369<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_ROI1_GAIN2_NEGr		(ValSft_R00(_rd32(_REG_BASE_+(0x0369<<2)))&BitMask_08)
#define APT_ROI1_GAIN2_NEGw(val)	_wr32(_REG_BASE_+(0x0369<<2),(UINT)((_rd32(_REG_BASE_+(0x0369<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_ROI2_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x036a<<2)))&BitMask_01)
#define APT_ROI2_ONw(val)		_wr32(_REG_BASE_+(0x036a<<2),(UINT)((_rd32(_REG_BASE_+(0x036a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_ROI2_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x036a<<2)))&BitMask_01)
#define APT_ROI2_OSD_ONw(val)	_wr32(_REG_BASE_+(0x036a<<2),(UINT)((_rd32(_REG_BASE_+(0x036a<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_ROI2_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x036a<<2)))&BitMask_11)
#define APT_ROI2_HSTw(val)		_wr32(_REG_BASE_+(0x036a<<2),(UINT)((_rd32(_REG_BASE_+(0x036a<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define APT_ROI2_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x036a<<2)))&BitMask_11)
#define APT_ROI2_HEDw(val)		_wr32(_REG_BASE_+(0x036a<<2),(UINT)((_rd32(_REG_BASE_+(0x036a<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ROI2_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x036b<<2)))&BitMask_10)
#define APT_ROI2_VSTw(val)		_wr32(_REG_BASE_+(0x036b<<2),(UINT)((_rd32(_REG_BASE_+(0x036b<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define APT_ROI2_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x036b<<2)))&BitMask_10)
#define APT_ROI2_VEDw(val)		_wr32(_REG_BASE_+(0x036b<<2),(UINT)((_rd32(_REG_BASE_+(0x036b<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_ROI2_GAIN1_POSr		(ValSft_R24(_rd32(_REG_BASE_+(0x036c<<2)))&BitMask_08)
#define APT_ROI2_GAIN1_POSw(val)	_wr32(_REG_BASE_+(0x036c<<2),(UINT)((_rd32(_REG_BASE_+(0x036c<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_ROI2_GAIN2_POSr		(ValSft_R16(_rd32(_REG_BASE_+(0x036c<<2)))&BitMask_08)
#define APT_ROI2_GAIN2_POSw(val)	_wr32(_REG_BASE_+(0x036c<<2),(UINT)((_rd32(_REG_BASE_+(0x036c<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_ROI2_GAIN1_NEGr		(ValSft_R08(_rd32(_REG_BASE_+(0x036c<<2)))&BitMask_08)
#define APT_ROI2_GAIN1_NEGw(val)	_wr32(_REG_BASE_+(0x036c<<2),(UINT)((_rd32(_REG_BASE_+(0x036c<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_ROI2_GAIN2_NEGr		(ValSft_R00(_rd32(_REG_BASE_+(0x036c<<2)))&BitMask_08)
#define APT_ROI2_GAIN2_NEGw(val)	_wr32(_REG_BASE_+(0x036c<<2),(UINT)((_rd32(_REG_BASE_+(0x036c<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_ROI3_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x036d<<2)))&BitMask_01)
#define APT_ROI3_ONw(val)		_wr32(_REG_BASE_+(0x036d<<2),(UINT)((_rd32(_REG_BASE_+(0x036d<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_ROI3_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x036d<<2)))&BitMask_01)
#define APT_ROI3_OSD_ONw(val)	_wr32(_REG_BASE_+(0x036d<<2),(UINT)((_rd32(_REG_BASE_+(0x036d<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_ROI3_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x036d<<2)))&BitMask_11)
#define APT_ROI3_HSTw(val)		_wr32(_REG_BASE_+(0x036d<<2),(UINT)((_rd32(_REG_BASE_+(0x036d<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define APT_ROI3_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x036d<<2)))&BitMask_11)
#define APT_ROI3_HEDw(val)		_wr32(_REG_BASE_+(0x036d<<2),(UINT)((_rd32(_REG_BASE_+(0x036d<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ROI3_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x036e<<2)))&BitMask_10)
#define APT_ROI3_VSTw(val)		_wr32(_REG_BASE_+(0x036e<<2),(UINT)((_rd32(_REG_BASE_+(0x036e<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define APT_ROI3_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x036e<<2)))&BitMask_10)
#define APT_ROI3_VEDw(val)		_wr32(_REG_BASE_+(0x036e<<2),(UINT)((_rd32(_REG_BASE_+(0x036e<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_ROI3_GAIN1_POSr		(ValSft_R24(_rd32(_REG_BASE_+(0x036f<<2)))&BitMask_08)
#define APT_ROI3_GAIN1_POSw(val)	_wr32(_REG_BASE_+(0x036f<<2),(UINT)((_rd32(_REG_BASE_+(0x036f<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_ROI3_GAIN2_POSr		(ValSft_R16(_rd32(_REG_BASE_+(0x036f<<2)))&BitMask_08)
#define APT_ROI3_GAIN2_POSw(val)	_wr32(_REG_BASE_+(0x036f<<2),(UINT)((_rd32(_REG_BASE_+(0x036f<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_ROI3_GAIN1_NEGr		(ValSft_R08(_rd32(_REG_BASE_+(0x036f<<2)))&BitMask_08)
#define APT_ROI3_GAIN1_NEGw(val)	_wr32(_REG_BASE_+(0x036f<<2),(UINT)((_rd32(_REG_BASE_+(0x036f<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_ROI3_GAIN2_NEGr		(ValSft_R00(_rd32(_REG_BASE_+(0x036f<<2)))&BitMask_08)
#define APT_ROI3_GAIN2_NEGw(val)	_wr32(_REG_BASE_+(0x036f<<2),(UINT)((_rd32(_REG_BASE_+(0x036f<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_ROI4_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0370<<2)))&BitMask_01)
#define APT_ROI4_ONw(val)		_wr32(_REG_BASE_+(0x0370<<2),(UINT)((_rd32(_REG_BASE_+(0x0370<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define APT_ROI4_OSD_ONr		(ValSft_R30(_rd32(_REG_BASE_+(0x0370<<2)))&BitMask_01)
#define APT_ROI4_OSD_ONw(val)	_wr32(_REG_BASE_+(0x0370<<2),(UINT)((_rd32(_REG_BASE_+(0x0370<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define APT_ROI4_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0370<<2)))&BitMask_11)
#define APT_ROI4_HSTw(val)		_wr32(_REG_BASE_+(0x0370<<2),(UINT)((_rd32(_REG_BASE_+(0x0370<<2))&(~ValSft_L16(BitMask_11)))|ValSft_L16((val)&BitMask_11)))

#define APT_ROI4_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0370<<2)))&BitMask_11)
#define APT_ROI4_HEDw(val)		_wr32(_REG_BASE_+(0x0370<<2),(UINT)((_rd32(_REG_BASE_+(0x0370<<2))&(~ValSft_L00(BitMask_11)))|ValSft_L00((val)&BitMask_11)))

#define APT_ROI4_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0371<<2)))&BitMask_10)
#define APT_ROI4_VSTw(val)		_wr32(_REG_BASE_+(0x0371<<2),(UINT)((_rd32(_REG_BASE_+(0x0371<<2))&(~ValSft_L16(BitMask_10)))|ValSft_L16((val)&BitMask_10)))

#define APT_ROI4_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0371<<2)))&BitMask_10)
#define APT_ROI4_VEDw(val)		_wr32(_REG_BASE_+(0x0371<<2),(UINT)((_rd32(_REG_BASE_+(0x0371<<2))&(~ValSft_L00(BitMask_10)))|ValSft_L00((val)&BitMask_10)))

#define APT_ROI4_GAIN1_POSr		(ValSft_R24(_rd32(_REG_BASE_+(0x0372<<2)))&BitMask_08)
#define APT_ROI4_GAIN1_POSw(val)	_wr32(_REG_BASE_+(0x0372<<2),(UINT)((_rd32(_REG_BASE_+(0x0372<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_ROI4_GAIN2_POSr		(ValSft_R16(_rd32(_REG_BASE_+(0x0372<<2)))&BitMask_08)
#define APT_ROI4_GAIN2_POSw(val)	_wr32(_REG_BASE_+(0x0372<<2),(UINT)((_rd32(_REG_BASE_+(0x0372<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_ROI4_GAIN1_NEGr		(ValSft_R08(_rd32(_REG_BASE_+(0x0372<<2)))&BitMask_08)
#define APT_ROI4_GAIN1_NEGw(val)	_wr32(_REG_BASE_+(0x0372<<2),(UINT)((_rd32(_REG_BASE_+(0x0372<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_ROI4_GAIN2_NEGr		(ValSft_R00(_rd32(_REG_BASE_+(0x0372<<2)))&BitMask_08)
#define APT_ROI4_GAIN2_NEGw(val)	_wr32(_REG_BASE_+(0x0372<<2),(UINT)((_rd32(_REG_BASE_+(0x0372<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define APT_ROI1_GAIN3r			(ValSft_R24(_rd32(_REG_BASE_+(0x0373<<2)))&BitMask_08)
#define APT_ROI1_GAIN3w(val)	_wr32(_REG_BASE_+(0x0373<<2),(UINT)((_rd32(_REG_BASE_+(0x0373<<2))&(~ValSft_L24(BitMask_08)))|ValSft_L24((val)&BitMask_08)))

#define APT_ROI2_GAIN3r			(ValSft_R16(_rd32(_REG_BASE_+(0x0373<<2)))&BitMask_08)
#define APT_ROI2_GAIN3w(val)	_wr32(_REG_BASE_+(0x0373<<2),(UINT)((_rd32(_REG_BASE_+(0x0373<<2))&(~ValSft_L16(BitMask_08)))|ValSft_L16((val)&BitMask_08)))

#define APT_ROI3_GAIN3r			(ValSft_R08(_rd32(_REG_BASE_+(0x0373<<2)))&BitMask_08)
#define APT_ROI3_GAIN3w(val)	_wr32(_REG_BASE_+(0x0373<<2),(UINT)((_rd32(_REG_BASE_+(0x0373<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define APT_ROI4_GAIN3r			(ValSft_R00(_rd32(_REG_BASE_+(0x0373<<2)))&BitMask_08)
#define APT_ROI4_GAIN3w(val)	_wr32(_REG_BASE_+(0x0373<<2),(UINT)((_rd32(_REG_BASE_+(0x0373<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ALPHA_CH0_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_01)
#define ALPHA_CH0_ONw(val)		_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define ALPHA_CH0_CINVr			(ValSft_R30(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_01)
#define ALPHA_CH0_CINVw(val)	_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define CH0_THRES_SELr			(ValSft_R28(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_01)
#define CH0_THRES_SELw(val)		_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define CH0_MIMG_SELr			(ValSft_R25(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_01)
#define CH0_MIMG_SELw(val)		_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L25(BitMask_01)))|ValSft_L25((val)&BitMask_01)))

#define CH0_ALPHAr				(ValSft_R16(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_09)
#define CH0_ALPHAw(val)			_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L16(BitMask_09)))|ValSft_L16((val)&BitMask_09)))

#define CH0_THRESHr				(ValSft_R08(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_08)
#define CH0_THRESHw(val)		_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L08(BitMask_08)))|ValSft_L08((val)&BitMask_08)))

#define CH0_THRESLr				(ValSft_R00(_rd32(_REG_BASE_+(0x0375<<2)))&BitMask_08)
#define CH0_THRESLw(val)		_wr32(_REG_BASE_+(0x0375<<2),(UINT)((_rd32(_REG_BASE_+(0x0375<<2))&(~ValSft_L00(BitMask_08)))|ValSft_L00((val)&BitMask_08)))

#define ALPHA0_MONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0377<<2)))&BitMask_01)
#define ALPHA0_MONw(val)		_wr32(_REG_BASE_+(0x0377<<2),(UINT)((_rd32(_REG_BASE_+(0x0377<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BBLEND_ONr			(ValSft_R29(_rd32(_REG_BASE_+(0x0377<<2)))&BitMask_01)
#define AP0_BBLEND_ONw(val)		_wr32(_REG_BASE_+(0x0377<<2),(UINT)((_rd32(_REG_BASE_+(0x0377<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CH0_TH_MODr				(ValSft_R24(_rd32(_REG_BASE_+(0x0377<<2)))&BitMask_03)
#define CH0_TH_MODw(val)		_wr32(_REG_BASE_+(0x0377<<2),(UINT)((_rd32(_REG_BASE_+(0x0377<<2))&(~ValSft_L24(BitMask_03)))|ValSft_L24((val)&BitMask_03)))

#define YC_ACT_CH1_SELr			(ValSft_R03(_rd32(_REG_BASE_+(0x0377<<2)))&BitMask_01)
#define YC_ACT_CH1_SELw(val)	_wr32(_REG_BASE_+(0x0377<<2),(UINT)((_rd32(_REG_BASE_+(0x0377<<2))&(~ValSft_L03(BitMask_01)))|ValSft_L03((val)&BitMask_01)))

#define YC_ACT_CH2_SELr			(ValSft_R02(_rd32(_REG_BASE_+(0x0377<<2)))&BitMask_01)
#define YC_ACT_CH2_SELw(val)	_wr32(_REG_BASE_+(0x0377<<2),(UINT)((_rd32(_REG_BASE_+(0x0377<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define AP0_BOX0_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0378<<2)))&BitMask_01)
#define AP0_BOX0_ONw(val)		_wr32(_REG_BASE_+(0x0378<<2),(UINT)((_rd32(_REG_BASE_+(0x0378<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX0_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0378<<2)))&BitMask_13)
#define AP0_BOX0_HSTw(val)		_wr32(_REG_BASE_+(0x0378<<2),(UINT)((_rd32(_REG_BASE_+(0x0378<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX0_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0378<<2)))&BitMask_13)
#define AP0_BOX0_HEDw(val)		_wr32(_REG_BASE_+(0x0378<<2),(UINT)((_rd32(_REG_BASE_+(0x0378<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX0_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0379<<2)))&BitMask_12)
#define AP0_BOX0_VSTw(val)		_wr32(_REG_BASE_+(0x0379<<2),(UINT)((_rd32(_REG_BASE_+(0x0379<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX0_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0379<<2)))&BitMask_12)
#define AP0_BOX0_VEDw(val)		_wr32(_REG_BASE_+(0x0379<<2),(UINT)((_rd32(_REG_BASE_+(0x0379<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX1_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x037a<<2)))&BitMask_01)
#define AP0_BOX1_ONw(val)		_wr32(_REG_BASE_+(0x037a<<2),(UINT)((_rd32(_REG_BASE_+(0x037a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX1_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037a<<2)))&BitMask_13)
#define AP0_BOX1_HSTw(val)		_wr32(_REG_BASE_+(0x037a<<2),(UINT)((_rd32(_REG_BASE_+(0x037a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX1_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037a<<2)))&BitMask_13)
#define AP0_BOX1_HEDw(val)		_wr32(_REG_BASE_+(0x037a<<2),(UINT)((_rd32(_REG_BASE_+(0x037a<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX1_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037b<<2)))&BitMask_12)
#define AP0_BOX1_VSTw(val)		_wr32(_REG_BASE_+(0x037b<<2),(UINT)((_rd32(_REG_BASE_+(0x037b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX1_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037b<<2)))&BitMask_12)
#define AP0_BOX1_VEDw(val)		_wr32(_REG_BASE_+(0x037b<<2),(UINT)((_rd32(_REG_BASE_+(0x037b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX2_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x037c<<2)))&BitMask_01)
#define AP0_BOX2_ONw(val)		_wr32(_REG_BASE_+(0x037c<<2),(UINT)((_rd32(_REG_BASE_+(0x037c<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX2_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037c<<2)))&BitMask_13)
#define AP0_BOX2_HSTw(val)		_wr32(_REG_BASE_+(0x037c<<2),(UINT)((_rd32(_REG_BASE_+(0x037c<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX2_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037c<<2)))&BitMask_13)
#define AP0_BOX2_HEDw(val)		_wr32(_REG_BASE_+(0x037c<<2),(UINT)((_rd32(_REG_BASE_+(0x037c<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX2_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037d<<2)))&BitMask_12)
#define AP0_BOX2_VSTw(val)		_wr32(_REG_BASE_+(0x037d<<2),(UINT)((_rd32(_REG_BASE_+(0x037d<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX2_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037d<<2)))&BitMask_12)
#define AP0_BOX2_VEDw(val)		_wr32(_REG_BASE_+(0x037d<<2),(UINT)((_rd32(_REG_BASE_+(0x037d<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX3_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x037e<<2)))&BitMask_01)
#define AP0_BOX3_ONw(val)		_wr32(_REG_BASE_+(0x037e<<2),(UINT)((_rd32(_REG_BASE_+(0x037e<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX3_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037e<<2)))&BitMask_13)
#define AP0_BOX3_HSTw(val)		_wr32(_REG_BASE_+(0x037e<<2),(UINT)((_rd32(_REG_BASE_+(0x037e<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX3_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037e<<2)))&BitMask_13)
#define AP0_BOX3_HEDw(val)		_wr32(_REG_BASE_+(0x037e<<2),(UINT)((_rd32(_REG_BASE_+(0x037e<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX3_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x037f<<2)))&BitMask_12)
#define AP0_BOX3_VSTw(val)		_wr32(_REG_BASE_+(0x037f<<2),(UINT)((_rd32(_REG_BASE_+(0x037f<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX3_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x037f<<2)))&BitMask_12)
#define AP0_BOX3_VEDw(val)		_wr32(_REG_BASE_+(0x037f<<2),(UINT)((_rd32(_REG_BASE_+(0x037f<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX4_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0380<<2)))&BitMask_01)
#define AP0_BOX4_ONw(val)		_wr32(_REG_BASE_+(0x0380<<2),(UINT)((_rd32(_REG_BASE_+(0x0380<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX4_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0380<<2)))&BitMask_13)
#define AP0_BOX4_HSTw(val)		_wr32(_REG_BASE_+(0x0380<<2),(UINT)((_rd32(_REG_BASE_+(0x0380<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX4_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0380<<2)))&BitMask_13)
#define AP0_BOX4_HEDw(val)		_wr32(_REG_BASE_+(0x0380<<2),(UINT)((_rd32(_REG_BASE_+(0x0380<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX4_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0381<<2)))&BitMask_12)
#define AP0_BOX4_VSTw(val)		_wr32(_REG_BASE_+(0x0381<<2),(UINT)((_rd32(_REG_BASE_+(0x0381<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX4_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0381<<2)))&BitMask_12)
#define AP0_BOX4_VEDw(val)		_wr32(_REG_BASE_+(0x0381<<2),(UINT)((_rd32(_REG_BASE_+(0x0381<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX5_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0382<<2)))&BitMask_01)
#define AP0_BOX5_ONw(val)		_wr32(_REG_BASE_+(0x0382<<2),(UINT)((_rd32(_REG_BASE_+(0x0382<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX5_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0382<<2)))&BitMask_13)
#define AP0_BOX5_HSTw(val)		_wr32(_REG_BASE_+(0x0382<<2),(UINT)((_rd32(_REG_BASE_+(0x0382<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX5_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0382<<2)))&BitMask_13)
#define AP0_BOX5_HEDw(val)		_wr32(_REG_BASE_+(0x0382<<2),(UINT)((_rd32(_REG_BASE_+(0x0382<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX5_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0383<<2)))&BitMask_12)
#define AP0_BOX5_VSTw(val)		_wr32(_REG_BASE_+(0x0383<<2),(UINT)((_rd32(_REG_BASE_+(0x0383<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX5_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0383<<2)))&BitMask_12)
#define AP0_BOX5_VEDw(val)		_wr32(_REG_BASE_+(0x0383<<2),(UINT)((_rd32(_REG_BASE_+(0x0383<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX6_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0384<<2)))&BitMask_01)
#define AP0_BOX6_ONw(val)		_wr32(_REG_BASE_+(0x0384<<2),(UINT)((_rd32(_REG_BASE_+(0x0384<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX6_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0384<<2)))&BitMask_13)
#define AP0_BOX6_HSTw(val)		_wr32(_REG_BASE_+(0x0384<<2),(UINT)((_rd32(_REG_BASE_+(0x0384<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX6_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0384<<2)))&BitMask_13)
#define AP0_BOX6_HEDw(val)		_wr32(_REG_BASE_+(0x0384<<2),(UINT)((_rd32(_REG_BASE_+(0x0384<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX6_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0385<<2)))&BitMask_12)
#define AP0_BOX6_VSTw(val)		_wr32(_REG_BASE_+(0x0385<<2),(UINT)((_rd32(_REG_BASE_+(0x0385<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX6_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0385<<2)))&BitMask_12)
#define AP0_BOX6_VEDw(val)		_wr32(_REG_BASE_+(0x0385<<2),(UINT)((_rd32(_REG_BASE_+(0x0385<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX7_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0386<<2)))&BitMask_01)
#define AP0_BOX7_ONw(val)		_wr32(_REG_BASE_+(0x0386<<2),(UINT)((_rd32(_REG_BASE_+(0x0386<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX7_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0386<<2)))&BitMask_13)
#define AP0_BOX7_HSTw(val)		_wr32(_REG_BASE_+(0x0386<<2),(UINT)((_rd32(_REG_BASE_+(0x0386<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX7_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0386<<2)))&BitMask_13)
#define AP0_BOX7_HEDw(val)		_wr32(_REG_BASE_+(0x0386<<2),(UINT)((_rd32(_REG_BASE_+(0x0386<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX7_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0387<<2)))&BitMask_12)
#define AP0_BOX7_VSTw(val)		_wr32(_REG_BASE_+(0x0387<<2),(UINT)((_rd32(_REG_BASE_+(0x0387<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX7_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0387<<2)))&BitMask_12)
#define AP0_BOX7_VEDw(val)		_wr32(_REG_BASE_+(0x0387<<2),(UINT)((_rd32(_REG_BASE_+(0x0387<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX8_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x0388<<2)))&BitMask_01)
#define AP0_BOX8_ONw(val)		_wr32(_REG_BASE_+(0x0388<<2),(UINT)((_rd32(_REG_BASE_+(0x0388<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX8_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0388<<2)))&BitMask_13)
#define AP0_BOX8_HSTw(val)		_wr32(_REG_BASE_+(0x0388<<2),(UINT)((_rd32(_REG_BASE_+(0x0388<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX8_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0388<<2)))&BitMask_13)
#define AP0_BOX8_HEDw(val)		_wr32(_REG_BASE_+(0x0388<<2),(UINT)((_rd32(_REG_BASE_+(0x0388<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX8_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x0389<<2)))&BitMask_12)
#define AP0_BOX8_VSTw(val)		_wr32(_REG_BASE_+(0x0389<<2),(UINT)((_rd32(_REG_BASE_+(0x0389<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX8_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x0389<<2)))&BitMask_12)
#define AP0_BOX8_VEDw(val)		_wr32(_REG_BASE_+(0x0389<<2),(UINT)((_rd32(_REG_BASE_+(0x0389<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define AP0_BOX9_ONr			(ValSft_R31(_rd32(_REG_BASE_+(0x038a<<2)))&BitMask_01)
#define AP0_BOX9_ONw(val)		_wr32(_REG_BASE_+(0x038a<<2),(UINT)((_rd32(_REG_BASE_+(0x038a<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define AP0_BOX9_HSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x038a<<2)))&BitMask_13)
#define AP0_BOX9_HSTw(val)		_wr32(_REG_BASE_+(0x038a<<2),(UINT)((_rd32(_REG_BASE_+(0x038a<<2))&(~ValSft_L16(BitMask_13)))|ValSft_L16((val)&BitMask_13)))

#define AP0_BOX9_HEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x038a<<2)))&BitMask_13)
#define AP0_BOX9_HEDw(val)		_wr32(_REG_BASE_+(0x038a<<2),(UINT)((_rd32(_REG_BASE_+(0x038a<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define AP0_BOX9_VSTr			(ValSft_R16(_rd32(_REG_BASE_+(0x038b<<2)))&BitMask_12)
#define AP0_BOX9_VSTw(val)		_wr32(_REG_BASE_+(0x038b<<2),(UINT)((_rd32(_REG_BASE_+(0x038b<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define AP0_BOX9_VEDr			(ValSft_R00(_rd32(_REG_BASE_+(0x038b<<2)))&BitMask_12)
#define AP0_BOX9_VEDw(val)		_wr32(_REG_BASE_+(0x038b<<2),(UINT)((_rd32(_REG_BASE_+(0x038b<<2))&(~ValSft_L00(BitMask_12)))|ValSft_L00((val)&BitMask_12)))

#define VMAX_FCr				(ValSft_R31(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_01)
#define VMAX_FCw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HVTWI_SELr				(ValSft_R30(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_01)
#define HVTWI_SELw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HVTWO_SELr				(ValSft_R29(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_01)
#define HVTWO_SELw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define VMAX_VALr				(ValSft_R16(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_12)
#define VMAX_VALw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HMAX_FCr				(ValSft_R15(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_01)
#define HMAX_FCw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define HMAX_VALr				(ValSft_R00(_rd32(_REG_BASE_+(0x0390<<2)))&BitMask_13)
#define HMAX_VALw(val)			_wr32(_REG_BASE_+(0x0390<<2),(UINT)((_rd32(_REG_BASE_+(0x0390<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define VMAX2_FCr				(ValSft_R31(_rd32(_REG_BASE_+(0x0391<<2)))&BitMask_01)
#define VMAX2_FCw(val)			_wr32(_REG_BASE_+(0x0391<<2),(UINT)((_rd32(_REG_BASE_+(0x0391<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VMAX2_FC_VALr			(ValSft_R16(_rd32(_REG_BASE_+(0x0391<<2)))&BitMask_12)
#define VMAX2_FC_VALw(val)		_wr32(_REG_BASE_+(0x0391<<2),(UINT)((_rd32(_REG_BASE_+(0x0391<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define HMAX2_FCr				(ValSft_R15(_rd32(_REG_BASE_+(0x0391<<2)))&BitMask_01)
#define HMAX2_FCw(val)			_wr32(_REG_BASE_+(0x0391<<2),(UINT)((_rd32(_REG_BASE_+(0x0391<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define HMAX2_FC_VALr			(ValSft_R00(_rd32(_REG_BASE_+(0x0391<<2)))&BitMask_13)
#define HMAX2_FC_VALw(val)		_wr32(_REG_BASE_+(0x0391<<2),(UINT)((_rd32(_REG_BASE_+(0x0391<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define IP_UIRQ_CNT_SELr		(ValSft_R28(_rd32(_REG_BASE_+(0x0392<<2)))&BitMask_03)
#define IP_UIRQ_CNT_SELw(val)	_wr32(_REG_BASE_+(0x0392<<2),(UINT)((_rd32(_REG_BASE_+(0x0392<<2))&(~ValSft_L28(BitMask_03)))|ValSft_L28((val)&BitMask_03)))

#define IP_CLIUIRQr				(ValSft_R27(_rd32(_REG_BASE_+(0x0392<<2)))&BitMask_01)
#define IP_CLIUIRQw(val)		_wr32(_REG_BASE_+(0x0392<<2),(UINT)((_rd32(_REG_BASE_+(0x0392<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define IP_UIRQ_ENr				(ValSft_R26(_rd32(_REG_BASE_+(0x0392<<2)))&BitMask_01)
#define IP_UIRQ_ENw(val)		_wr32(_REG_BASE_+(0x0392<<2),(UINT)((_rd32(_REG_BASE_+(0x0392<<2))&(~ValSft_L26(BitMask_01)))|ValSft_L26((val)&BitMask_01)))

#define IP_UIRQ_VPOSr			(ValSft_R16(_rd32(_REG_BASE_+(0x0393<<2)))&BitMask_12)
#define IP_UIRQ_VPOSw(val)		_wr32(_REG_BASE_+(0x0393<<2),(UINT)((_rd32(_REG_BASE_+(0x0393<<2))&(~ValSft_L16(BitMask_12)))|ValSft_L16((val)&BitMask_12)))

#define IP_UIRQ_HPOSr			(ValSft_R00(_rd32(_REG_BASE_+(0x0393<<2)))&BitMask_13)
#define IP_UIRQ_HPOSw(val)		_wr32(_REG_BASE_+(0x0393<<2),(UINT)((_rd32(_REG_BASE_+(0x0393<<2))&(~ValSft_L00(BitMask_13)))|ValSft_L00((val)&BitMask_13)))

#define HDEC_ONr				(ValSft_R31(_rd32(_REG_BASE_+(0x0395<<2)))&BitMask_01)
#define HDEC_ONw(val)			_wr32(_REG_BASE_+(0x0395<<2),(UINT)((_rd32(_REG_BASE_+(0x0395<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define HDEC_CWENr				(ValSft_R30(_rd32(_REG_BASE_+(0x0395<<2)))&BitMask_01)
#define HDEC_CWENw(val)			_wr32(_REG_BASE_+(0x0395<<2),(UINT)((_rd32(_REG_BASE_+(0x0395<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define HDEC_VSYNC_POLr			(ValSft_R29(_rd32(_REG_BASE_+(0x0395<<2)))&BitMask_01)
#define HDEC_VSYNC_POLw(val)	_wr32(_REG_BASE_+(0x0395<<2),(UINT)((_rd32(_REG_BASE_+(0x0395<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define HDEC_HSYNC_POLr			(ValSft_R28(_rd32(_REG_BASE_+(0x0395<<2)))&BitMask_01)
#define HDEC_HSYNC_POLw(val)	_wr32(_REG_BASE_+(0x0395<<2),(UINT)((_rd32(_REG_BASE_+(0x0395<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define HDEC_ST_SELr			(ValSft_R27(_rd32(_REG_BASE_+(0x0395<<2)))&BitMask_01)
#define HDEC_ST_SELw(val)		_wr32(_REG_BASE_+(0x0395<<2),(UINT)((_rd32(_REG_BASE_+(0x0395<<2))&(~ValSft_L27(BitMask_01)))|ValSft_L27((val)&BitMask_01)))

#define YCW_DCK0_PDr			(ValSft_R31(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_01)
#define YCW_DCK0_PDw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define YCW_DCK1_PDr			(ValSft_R30(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_01)
#define YCW_DCK1_PDw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define YCW_DCK2_PDr			(ValSft_R29(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_01)
#define YCW_DCK2_PDw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define YCW_DCK3_PDr			(ValSft_R28(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_01)
#define YCW_DCK3_PDw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define YCW_DCK0_SELr			(ValSft_R12(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_04)
#define YCW_DCK0_SELw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define YCW_DCK1_SELr			(ValSft_R08(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_04)
#define YCW_DCK1_SELw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define YCW_DCK2_SELr			(ValSft_R04(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_04)
#define YCW_DCK2_SELw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define YCW_DCK3_SELr			(ValSft_R00(_rd32(_REG_BASE_+(0x03a0<<2)))&BitMask_04)
#define YCW_DCK3_SELw(val)		_wr32(_REG_BASE_+(0x03a0<<2),(UINT)((_rd32(_REG_BASE_+(0x03a0<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define QOS_WCH0r				(ValSft_R28(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH0w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define QOS_WCH1r				(ValSft_R24(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH1w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define QOS_WCH2r				(ValSft_R20(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH2w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define QOS_WCH3r				(ValSft_R16(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH3w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define QOS_WCH4r				(ValSft_R12(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH4w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define QOS_WCH5r				(ValSft_R08(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH5w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define QOS_WCH6r				(ValSft_R04(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH6w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define QOS_WCH7r				(ValSft_R00(_rd32(_REG_BASE_+(0x03a2<<2)))&BitMask_04)
#define QOS_WCH7w(val)			_wr32(_REG_BASE_+(0x03a2<<2),(UINT)((_rd32(_REG_BASE_+(0x03a2<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define QOS_WCH8r				(ValSft_R28(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH8w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define QOS_WCH9r				(ValSft_R24(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH9w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define QOS_WCH10r				(ValSft_R20(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH10w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define QOS_WCH11r				(ValSft_R16(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH11w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define QOS_WCH12r				(ValSft_R12(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH12w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define QOS_WCH13r				(ValSft_R08(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH13w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define QOS_WCH14r				(ValSft_R04(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH14w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define QOS_WCH15r				(ValSft_R00(_rd32(_REG_BASE_+(0x03a3<<2)))&BitMask_04)
#define QOS_WCH15w(val)			_wr32(_REG_BASE_+(0x03a3<<2),(UINT)((_rd32(_REG_BASE_+(0x03a3<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define QOS_WCH16r				(ValSft_R28(_rd32(_REG_BASE_+(0x03a4<<2)))&BitMask_04)
#define QOS_WCH16w(val)			_wr32(_REG_BASE_+(0x03a4<<2),(UINT)((_rd32(_REG_BASE_+(0x03a4<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define QOS_WCH17r				(ValSft_R24(_rd32(_REG_BASE_+(0x03a4<<2)))&BitMask_04)
#define QOS_WCH17w(val)			_wr32(_REG_BASE_+(0x03a4<<2),(UINT)((_rd32(_REG_BASE_+(0x03a4<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define QOS_RCH0r				(ValSft_R28(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH0w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define QOS_RCH1r				(ValSft_R24(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH1w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define QOS_RCH2r				(ValSft_R20(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH2w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define QOS_RCH3r				(ValSft_R16(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH3w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define QOS_RCH4r				(ValSft_R12(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH4w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define QOS_RCH5r				(ValSft_R08(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH5w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define QOS_RCH6r				(ValSft_R04(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH6w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L04(BitMask_04)))|ValSft_L04((val)&BitMask_04)))

#define QOS_RCH7r				(ValSft_R00(_rd32(_REG_BASE_+(0x03a7<<2)))&BitMask_04)
#define QOS_RCH7w(val)			_wr32(_REG_BASE_+(0x03a7<<2),(UINT)((_rd32(_REG_BASE_+(0x03a7<<2))&(~ValSft_L00(BitMask_04)))|ValSft_L00((val)&BitMask_04)))

#define QOS_RCH8r				(ValSft_R28(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH8w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L28(BitMask_04)))|ValSft_L28((val)&BitMask_04)))

#define QOS_RCH9r				(ValSft_R24(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH9w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L24(BitMask_04)))|ValSft_L24((val)&BitMask_04)))

#define QOS_RCH10r				(ValSft_R20(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH10w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L20(BitMask_04)))|ValSft_L20((val)&BitMask_04)))

#define QOS_RCH11r				(ValSft_R16(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH11w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L16(BitMask_04)))|ValSft_L16((val)&BitMask_04)))

#define QOS_RCH12r				(ValSft_R12(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH12w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L12(BitMask_04)))|ValSft_L12((val)&BitMask_04)))

#define QOS_RCH13r				(ValSft_R08(_rd32(_REG_BASE_+(0x03a8<<2)))&BitMask_04)
#define QOS_RCH13w(val)			_wr32(_REG_BASE_+(0x03a8<<2),(UINT)((_rd32(_REG_BASE_+(0x03a8<<2))&(~ValSft_L08(BitMask_04)))|ValSft_L08((val)&BitMask_04)))

#define IMW0_DS_ENr				(ValSft_R31(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define IMW0_DS_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define IMW1_DS_ENr				(ValSft_R30(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define IMW1_DS_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define IMW2_DS_ENr				(ValSft_R29(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define IMW2_DS_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define IMW3_DS_ENr				(ValSft_R28(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define IMW3_DS_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L28(BitMask_01)))|ValSft_L28((val)&BitMask_01)))

#define CLI_IMW3_DSr			(ValSft_R23(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_IMW3_DSw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L23(BitMask_01)))|ValSft_L23((val)&BitMask_01)))

#define CLI_IMW2_DSr			(ValSft_R22(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_IMW2_DSw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L22(BitMask_01)))|ValSft_L22((val)&BitMask_01)))

#define CLI_IMW1_DSr			(ValSft_R21(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_IMW1_DSw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L21(BitMask_01)))|ValSft_L21((val)&BitMask_01)))

#define CLI_IMW0_DSr			(ValSft_R20(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_IMW0_DSw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L20(BitMask_01)))|ValSft_L20((val)&BitMask_01)))

#define VIT1_IRQ_ENr			(ValSft_R15(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define VIT1_IRQ_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L15(BitMask_01)))|ValSft_L15((val)&BitMask_01)))

#define VIT2_IRQ_ENr			(ValSft_R14(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define VIT2_IRQ_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L14(BitMask_01)))|ValSft_L14((val)&BitMask_01)))

#define VIT3_IRQ_ENr			(ValSft_R13(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define VIT3_IRQ_ENw(val)		_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L13(BitMask_01)))|ValSft_L13((val)&BitMask_01)))

#define CLI_VLOCKO_IT1r			(ValSft_R07(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_VLOCKO_IT1w(val)	_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L07(BitMask_01)))|ValSft_L07((val)&BitMask_01)))

#define CLI_VLOCKO_IT2r			(ValSft_R06(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_VLOCKO_IT2w(val)	_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L06(BitMask_01)))|ValSft_L06((val)&BitMask_01)))

#define CLI_VLOCKO_IT3r			(ValSft_R05(_rd32(_REG_BASE_+(0x03ae<<2)))&BitMask_01)
#define CLI_VLOCKO_IT3w(val)	_wr32(_REG_BASE_+(0x03ae<<2),(UINT)((_rd32(_REG_BASE_+(0x03ae<<2))&(~ValSft_L05(BitMask_01)))|ValSft_L05((val)&BitMask_01)))

#define VIRQO_EN_Tr				(ValSft_R31(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define VIRQO_EN_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L31(BitMask_01)))|ValSft_L31((val)&BitMask_01)))

#define VIRQW_EN_Tr				(ValSft_R30(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define VIRQW_EN_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L30(BitMask_01)))|ValSft_L30((val)&BitMask_01)))

#define VIRQI_EN_Tr				(ValSft_R29(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define VIRQI_EN_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L29(BitMask_01)))|ValSft_L29((val)&BitMask_01)))

#define CLI_VLOCKO_Tr			(ValSft_R02(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define CLI_VLOCKO_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L02(BitMask_01)))|ValSft_L02((val)&BitMask_01)))

#define CLI_VLOCKW_Tr			(ValSft_R01(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define CLI_VLOCKW_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L01(BitMask_01)))|ValSft_L01((val)&BitMask_01)))

#define CLI_VLOCKI_Tr			(ValSft_R00(_rd32(_REG_BASE_+(0x03af<<2)))&BitMask_01)
#define CLI_VLOCKI_Tw(val)		_wr32(_REG_BASE_+(0x03af<<2),(UINT)((_rd32(_REG_BASE_+(0x03af<<2))&(~ValSft_L00(BitMask_01)))|ValSft_L00((val)&BitMask_01)))

#define LCD_CMD_BUSYr			(ValSft_R00(_rd32(_REG_BASE_+(0x034a<<2)))&BitMask_01)
#define AE1_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0500<<2)))&BitMask_32)
#define AE1_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0501<<2)))&BitMask_32)
#define AE1_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0502<<2)))&BitMask_32)
#define AE2_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0503<<2)))&BitMask_32)
#define AE2_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0504<<2)))&BitMask_32)
#define AE2_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0505<<2)))&BitMask_32)
#define AE3_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0506<<2)))&BitMask_32)
#define AE3_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0507<<2)))&BitMask_32)
#define AE3_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0508<<2)))&BitMask_32)
#define AE4_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0509<<2)))&BitMask_32)
#define AE4_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050a<<2)))&BitMask_32)
#define AE4_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050b<<2)))&BitMask_32)
#define AE5_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050c<<2)))&BitMask_32)
#define AE5_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050d<<2)))&BitMask_32)
#define AE5_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050e<<2)))&BitMask_32)
#define AE6_SUM0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x050f<<2)))&BitMask_32)
#define AE6_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0510<<2)))&BitMask_32)
#define AE6_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0511<<2)))&BitMask_32)
#define AE1_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0512<<2)))&BitMask_24)
#define AE2_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0513<<2)))&BitMask_24)
#define AE3_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0514<<2)))&BitMask_24)
#define AE4_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0515<<2)))&BitMask_24)
#define AE5_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0516<<2)))&BitMask_24)
#define AE6_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0517<<2)))&BitMask_24)
#define AE1_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0518<<2)))&BitMask_24)
#define AE2_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0519<<2)))&BitMask_24)
#define AE3_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051a<<2)))&BitMask_24)
#define AE4_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051b<<2)))&BitMask_24)
#define AE5_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051c<<2)))&BitMask_24)
#define AE6_SLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051d<<2)))&BitMask_24)
#define HISTO0_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051e<<2)))&BitMask_24)
#define HISTO1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x051f<<2)))&BitMask_24)
#define HISTO2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0520<<2)))&BitMask_24)
#define HISTO3_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0521<<2)))&BitMask_24)
#define HISTO4_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0522<<2)))&BitMask_24)
#define HISTO5_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0523<<2)))&BitMask_24)
#define HISTO6_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0524<<2)))&BitMask_24)
#define HISTO7_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0525<<2)))&BitMask_24)
#define HISTO8_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0526<<2)))&BitMask_24)
#define HISTO9_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0527<<2)))&BitMask_24)
#define HISTO10_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0528<<2)))&BitMask_24)
#define HISTO11_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0529<<2)))&BitMask_24)
#define HISTO12_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052a<<2)))&BitMask_24)
#define HISTO13_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052b<<2)))&BitMask_24)
#define HISTO14_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052c<<2)))&BitMask_24)
#define HISTO15_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052d<<2)))&BitMask_24)
#define HISTO0_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052e<<2)))&BitMask_24)
#define HISTO1_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x052f<<2)))&BitMask_24)
#define HISTO2_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0530<<2)))&BitMask_24)
#define HISTO3_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0531<<2)))&BitMask_24)
#define HISTO4_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0532<<2)))&BitMask_24)
#define HISTO5_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0533<<2)))&BitMask_24)
#define HISTO6_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0534<<2)))&BitMask_24)
#define HISTO7_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0535<<2)))&BitMask_24)
#define HISTO8_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0536<<2)))&BitMask_24)
#define HISTO9_W2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0537<<2)))&BitMask_24)
#define HISTO10_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0538<<2)))&BitMask_24)
#define HISTO11_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0539<<2)))&BitMask_24)
#define HISTO12_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x053a<<2)))&BitMask_24)
#define HISTO13_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x053b<<2)))&BitMask_24)
#define HISTO14_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x053c<<2)))&BitMask_24)
#define HISTO15_W2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x053d<<2)))&BitMask_24)
#define AWBL_CCR_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0540<<2)))&BitMask_32)
#define AWBL_CCG_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0541<<2)))&BitMask_32)
#define AWBL_CCB_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0542<<2)))&BitMask_32)
#define AWBL_CCT_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0543<<2)))&BitMask_24)
#define AWBL_CY_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0544<<2)))&BitMask_32)
#define AWBL_CBDB_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0545<<2)))&BitMask_32)
#define AWBL_CRDR_SUM_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0546<<2)))&BitMask_32)
#define AF1_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0550<<2)))&BitMask_32)
#define AF1_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0551<<2)))&BitMask_32)
#define AF2_SUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0552<<2)))&BitMask_32)
#define AF2_SUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0553<<2)))&BitMask_32)
#define AF1_CLP_SUM1_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0554<<2)))&BitMask_32)
#define AF1_CLP_SUM2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0555<<2)))&BitMask_32)
#define AF2_CLP_SUM1_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0556<<2)))&BitMask_32)
#define AF2_CLP_SUM2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0557<<2)))&BitMask_32)
#define AF1_YSUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0558<<2)))&BitMask_32)
#define AF1_YSUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x0559<<2)))&BitMask_32)
#define AF2_YSUM1_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055a<<2)))&BitMask_32)
#define AF2_YSUM2_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055b<<2)))&BitMask_32)
#define AF1_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055c<<2)))&BitMask_24)
#define AF2_CLCNT_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055d<<2)))&BitMask_24)
#define VAF1_SUM_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055e<<2)))&BitMask_32)
#define VAF2_SUM_LOCKr			(ValSft_R00(_rd32(_REG_BASE_+(0x055f<<2)))&BitMask_32)
#define AWBL_CCT_SUM0_LOCKr		(ValSft_R16(_rd32(_REG_BASE_+(0x058c<<2)))&BitMask_16)
#define AWBL_CCT_SUM1_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x058c<<2)))&BitMask_16)
#define AWBL_CCT_SUM2_LOCKr		(ValSft_R16(_rd32(_REG_BASE_+(0x058d<<2)))&BitMask_16)
#define AWBL_CCT_SUM3_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x058d<<2)))&BitMask_16)
#define AWBL_CCT_SUM4_LOCKr		(ValSft_R16(_rd32(_REG_BASE_+(0x058e<<2)))&BitMask_16)
#define AWBL_CCT_SUM5_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x058e<<2)))&BitMask_16)
#define AWBL_CCT_SUM6_LOCKr		(ValSft_R16(_rd32(_REG_BASE_+(0x058f<<2)))&BitMask_16)
#define AWBL_CCT_SUM7_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x058f<<2)))&BitMask_16)
#define AWBL_DB_SUM0_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0590<<2)))&BitMask_23)
#define AWBL_DB_SUM1_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0591<<2)))&BitMask_23)
#define AWBL_DB_SUM2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0592<<2)))&BitMask_23)
#define AWBL_DB_SUM3_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0593<<2)))&BitMask_23)
#define AWBL_DB_SUM4_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0594<<2)))&BitMask_23)
#define AWBL_DB_SUM5_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0595<<2)))&BitMask_23)
#define AWBL_DB_SUM6_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0596<<2)))&BitMask_23)
#define AWBL_DB_SUM7_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0597<<2)))&BitMask_23)
#define AWBL_DR_SUM0_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0598<<2)))&BitMask_23)
#define AWBL_DR_SUM1_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x0599<<2)))&BitMask_23)
#define AWBL_DR_SUM2_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059a<<2)))&BitMask_23)
#define AWBL_DR_SUM3_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059b<<2)))&BitMask_23)
#define AWBL_DR_SUM4_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059c<<2)))&BitMask_23)
#define AWBL_DR_SUM5_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059d<<2)))&BitMask_23)
#define AWBL_DR_SUM6_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059e<<2)))&BitMask_23)
#define AWBL_DR_SUM7_LOCKr		(ValSft_R00(_rd32(_REG_BASE_+(0x059f<<2)))&BitMask_23)
#define LS_FLGr					(ValSft_R31(_rd32(_REG_BASE_+(0x0570<<2)))&BitMask_01)
#define LS_FLG_POSTr			(ValSft_R30(_rd32(_REG_BASE_+(0x0570<<2)))&BitMask_01)
#define WDR_Gr					(ValSft_R16(_rd32(_REG_BASE_+(0x0570<<2)))&BitMask_12)
#define WDR_S1r					(ValSft_R00(_rd32(_REG_BASE_+(0x0570<<2)))&BitMask_10)
#define WDR_S2r					(ValSft_R16(_rd32(_REG_BASE_+(0x0571<<2)))&BitMask_10)
#define WDR_SHr					(ValSft_R00(_rd32(_REG_BASE_+(0x0571<<2)))&BitMask_10)
#define WDR_SH_ADJr				(ValSft_R16(_rd32(_REG_BASE_+(0x0572<<2)))&BitMask_10)
#define WDR_SWGT_ADJr			(ValSft_R00(_rd32(_REG_BASE_+(0x0572<<2)))&BitMask_10)
#define WDR_SCr					(ValSft_R16(_rd32(_REG_BASE_+(0x0573<<2)))&BitMask_10)
#define WDR_SMr					(ValSft_R00(_rd32(_REG_BASE_+(0x0573<<2)))&BitMask_10)
#define WDR_SGF_SLOPEr			(ValSft_R00(_rd32(_REG_BASE_+(0x0574<<2)))&BitMask_08)
#define WDR_LY_SUMr				(ValSft_R00(_rd32(_REG_BASE_+(0x0575<<2)))&BitMask_31)
#define WDR_SY_SUMr				(ValSft_R00(_rd32(_REG_BASE_+(0x0576<<2)))&BitMask_31)
#define WDR_HIGH_CNTr			(ValSft_R00(_rd32(_REG_BASE_+(0x0577<<2)))&BitMask_21)
#define WDR_LOW_CNTr			(ValSft_R00(_rd32(_REG_BASE_+(0x0578<<2)))&BitMask_21)
#define WDR_SL_Rr				(ValSft_R00(_rd32(_REG_BASE_+(0x0579<<2)))&BitMask_12)
#define WDR_G_INVr				(ValSft_R00(_rd32(_REG_BASE_+(0x057a<<2)))&BitMask_18)
#define IMD_DENDr				(ValSft_R31(_rd32(_REG_BASE_+(0x0580<<2)))&BitMask_01)
#define IMD_WCNT1r				(ValSft_R16(_rd32(_REG_BASE_+(0x0580<<2)))&BitMask_12)
#define IMD_WCNT2r				(ValSft_R00(_rd32(_REG_BASE_+(0x0580<<2)))&BitMask_12)
#define IMD_WCNT3r				(ValSft_R16(_rd32(_REG_BASE_+(0x0581<<2)))&BitMask_12)
#define IMD_WCNT4r				(ValSft_R00(_rd32(_REG_BASE_+(0x0581<<2)))&BitMask_12)
#define THM_HCNTI_SVr			(ValSft_R16(_rd32(_REG_BASE_+(0x05b0<<2)))&BitMask_12)
#define THM_VCNTI_SVr			(ValSft_R00(_rd32(_REG_BASE_+(0x05b0<<2)))&BitMask_11)
#define THM_HCNTIr				(ValSft_R16(_rd32(_REG_BASE_+(0x05b1<<2)))&BitMask_12)
#define THM_VCNTIr				(ValSft_R00(_rd32(_REG_BASE_+(0x05b1<<2)))&BitMask_11)
#define THM_TEMP_MAXr			(ValSft_R16(_rd32(_REG_BASE_+(0x05b2<<2)))&BitMask_16)
#define THM_TEMP_MINr			(ValSft_R00(_rd32(_REG_BASE_+(0x05b2<<2)))&BitMask_16)
#define THM_ARCNT_SV0r			(ValSft_R00(_rd32(_REG_BASE_+(0x05b3<<2)))&BitMask_11)
#define MAX_HPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x05b4<<2)))&BitMask_12)
#define MAX_VPOSr				(ValSft_R00(_rd32(_REG_BASE_+(0x05b4<<2)))&BitMask_11)
#define MIN_HPOSr				(ValSft_R16(_rd32(_REG_BASE_+(0x05b5<<2)))&BitMask_12)
#define MIN_VPOSr				(ValSft_R00(_rd32(_REG_BASE_+(0x05b5<<2)))&BitMask_11)
#define FCNTr					(ValSft_R00(_rd32(_REG_BASE_+(0x05bf<<2)))&BitMask_08)
#define SYNC1_STATUSr			(ValSft_R31(_rd32(_REG_BASE_+(0x05c0<<2)))&BitMask_01)
#define CH1_HDETr				(ValSft_R16(_rd32(_REG_BASE_+(0x05c0<<2)))&BitMask_12)
#define LE2SE0_VDIFr			(ValSft_R16(_rd32(_REG_BASE_+(0x05c1<<2)))&BitMask_12)
#define SE0SE1_VDIFr			(ValSft_R00(_rd32(_REG_BASE_+(0x05c1<<2)))&BitMask_12)
#define LE2SE0_HDIFr			(ValSft_R16(_rd32(_REG_BASE_+(0x05c2<<2)))&BitMask_13)
#define SE0SE1_HDIFr			(ValSft_R00(_rd32(_REG_BASE_+(0x05c2<<2)))&BitMask_13)
#define FRC_RPAGEr				(ValSft_R16(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_03)
#define FRC_WPAGEr				(ValSft_R12(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_03)
#define WPAGE_CH4r				(ValSft_R10(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_02)
#define WPAGE_CH3r				(ValSft_R08(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_02)
#define WPAGE_CH2r				(ValSft_R04(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_03)
#define WPAGE_CH1r				(ValSft_R00(_rd32(_REG_BASE_+(0x05c4<<2)))&BitMask_03)
#define HCNTI_SVr				(ValSft_R16(_rd32(_REG_BASE_+(0x05c5<<2)))&BitMask_13)
#define VCNTI_SVr				(ValSft_R00(_rd32(_REG_BASE_+(0x05c5<<2)))&BitMask_12)
#define HCNTIr					(ValSft_R16(_rd32(_REG_BASE_+(0x05c6<<2)))&BitMask_13)
#define VCNTIr					(ValSft_R00(_rd32(_REG_BASE_+(0x05c6<<2)))&BitMask_12)
#define FLDO0_ITr				(ValSft_R31(_rd32(_REG_BASE_+(0x05c7<<2)))&BitMask_01)
#define FLDO1_ITr				(ValSft_R30(_rd32(_REG_BASE_+(0x05c7<<2)))&BitMask_01)
#define HCNTI2_SVr				(ValSft_R16(_rd32(_REG_BASE_+(0x05c7<<2)))&BitMask_13)
#define VCNTI2_SVr				(ValSft_R00(_rd32(_REG_BASE_+(0x05c7<<2)))&BitMask_12)
#define HCNTI2r					(ValSft_R16(_rd32(_REG_BASE_+(0x05c8<<2)))&BitMask_13)
#define VCNTI2r					(ValSft_R00(_rd32(_REG_BASE_+(0x05c8<<2)))&BitMask_12)
#define HCNTI0_BT_SVr			(ValSft_R16(_rd32(_REG_BASE_+(0x05c9<<2)))&BitMask_13)
#define VCNTI0_BT_SVr			(ValSft_R00(_rd32(_REG_BASE_+(0x05c9<<2)))&BitMask_12)
#define HCNTI0_BTr				(ValSft_R16(_rd32(_REG_BASE_+(0x05ca<<2)))&BitMask_13)
#define VCNTI0_BTr				(ValSft_R00(_rd32(_REG_BASE_+(0x05ca<<2)))&BitMask_12)
#define HCNTI1_BT_SVr			(ValSft_R16(_rd32(_REG_BASE_+(0x05cb<<2)))&BitMask_13)
#define VCNTI1_BT_SVr			(ValSft_R00(_rd32(_REG_BASE_+(0x05cb<<2)))&BitMask_12)
#define HCNTI1_BTr				(ValSft_R16(_rd32(_REG_BASE_+(0x05cc<<2)))&BitMask_13)
#define VCNTI1_BTr				(ValSft_R00(_rd32(_REG_BASE_+(0x05cc<<2)))&BitMask_12)
#define HCNTI2_BT_SVr			(ValSft_R16(_rd32(_REG_BASE_+(0x05cd<<2)))&BitMask_13)
#define VCNTI2_BT_SVr			(ValSft_R00(_rd32(_REG_BASE_+(0x05cd<<2)))&BitMask_12)
#define HCNTI2_BTr				(ValSft_R16(_rd32(_REG_BASE_+(0x05ce<<2)))&BitMask_13)
#define VCNTI2_BTr				(ValSft_R00(_rd32(_REG_BASE_+(0x05ce<<2)))&BitMask_12)
#define ARCNT_SV0r				(ValSft_R16(_rd32(_REG_BASE_+(0x05cf<<2)))&BitMask_13)
#define ARCNT_SV1r				(ValSft_R00(_rd32(_REG_BASE_+(0x05cf<<2)))&BitMask_13)
#define JPG_OVFr				(ValSft_R31(_rd32(_REG_BASE_+(0x05d2<<2)))&BitMask_01)
#define JPG_BIGr				(ValSft_R30(_rd32(_REG_BASE_+(0x05d2<<2)))&BitMask_01)
#define JPDEC_OVFr				(ValSft_R29(_rd32(_REG_BASE_+(0x05d2<<2)))&BitMask_01)
#define JPDEC_UDFr				(ValSft_R28(_rd32(_REG_BASE_+(0x05d2<<2)))&BitMask_01)
#define JPG_QP_REQr				(ValSft_R27(_rd32(_REG_BASE_+(0x05d2<<2)))&BitMask_01)
#define JPG_FSIZEr				(ValSft_R00(_rd32(_REG_BASE_+(0x05d3<<2)))&BitMask_28)
#define JPG_FSIZE_HOLDr			(ValSft_R00(_rd32(_REG_BASE_+(0x05d4<<2)))&BitMask_28)
#define JPG_LENr				(ValSft_R00(_rd32(_REG_BASE_+(0x05d5<<2)))&BitMask_28)
#define CXAL_WOP_Rr				(ValSft_R31(_rd32(_REG_BASE_+(0x05d7<<2)))&BitMask_01)
#define CRX_BITMINr				(ValSft_R00(_rd32(_REG_BASE_+(0x05d7<<2)))&BitMask_10)
#define CRX00r					(ValSft_R00(_rd32(_REG_BASE_+(0x05d8<<2)))&BitMask_32)
#define CRX01r					(ValSft_R00(_rd32(_REG_BASE_+(0x05d9<<2)))&BitMask_32)
#define CRX02r					(ValSft_R00(_rd32(_REG_BASE_+(0x05da<<2)))&BitMask_32)
#define CRX03r					(ValSft_R00(_rd32(_REG_BASE_+(0x05db<<2)))&BitMask_32)
#define CRX04r					(ValSft_R00(_rd32(_REG_BASE_+(0x05dc<<2)))&BitMask_32)
#define CRX05r					(ValSft_R00(_rd32(_REG_BASE_+(0x05dd<<2)))&BitMask_32)
#define CRX06r					(ValSft_R00(_rd32(_REG_BASE_+(0x05de<<2)))&BitMask_32)
#define CRX07r					(ValSft_R00(_rd32(_REG_BASE_+(0x05df<<2)))&BitMask_32)
#define ISP_RIRQr				(ValSft_R00(_rd32(_REG_BASE_+(0x05ee<<2)))&BitMask_31)
#define ISP_RIRQ_VOr			(ValSft_R02(_rd32(_REG_BASE_+(0x05ef<<2)))&BitMask_01)
#define ISP_RIRQ_VWr			(ValSft_R01(_rd32(_REG_BASE_+(0x05ef<<2)))&BitMask_01)
#define ISP_RIRQ_VIr			(ValSft_R00(_rd32(_REG_BASE_+(0x05ef<<2)))&BitMask_01)
#define SOL_CNTr				(ValSft_R16(_rd32(_REG_BASE_+(0x05f1<<2)))&BitMask_06)
#define CH124_DI_SVr			(ValSft_R24(_rd32(_REG_BASE_+(0x05f2<<2)))&BitMask_08)
#define CH124_WCH_SVr			(ValSft_R16(_rd32(_REG_BASE_+(0x05f2<<2)))&BitMask_08)
#define CH124_WCL_SVr			(ValSft_R08(_rd32(_REG_BASE_+(0x05f2<<2)))&BitMask_08)
#define ECCr					(ValSft_R08(_rd32(_REG_BASE_+(0x05f3<<2)))&BitMask_08)
#define PRTr					(ValSft_R00(_rd32(_REG_BASE_+(0x05f3<<2)))&BitMask_08)
#define SOF_WCr					(ValSft_R16(_rd32(_REG_BASE_+(0x05f4<<2)))&BitMask_16)
#define SOL_WC_Lr				(ValSft_R00(_rd32(_REG_BASE_+(0x05f4<<2)))&BitMask_16)
#define SOL_WC_Sr				(ValSft_R16(_rd32(_REG_BASE_+(0x05f5<<2)))&BitMask_16)
#define EOF_WCr					(ValSft_R00(_rd32(_REG_BASE_+(0x05f5<<2)))&BitMask_16)
#define HCNT_M_SVr				(ValSft_R16(_rd32(_REG_BASE_+(0x05f6<<2)))&BitMask_14)
#define VCNT_M_SVr				(ValSft_R00(_rd32(_REG_BASE_+(0x05f6<<2)))&BitMask_12)
#define LVCNTr					(ValSft_R16(_rd32(_REG_BASE_+(0x05f7<<2)))&BitMask_12)
#define LHCNTr					(ValSft_R00(_rd32(_REG_BASE_+(0x05f7<<2)))&BitMask_13)
#define RACNTr					(ValSft_R00(_rd32(_REG_BASE_+(0x05f8<<2)))&BitMask_12)
