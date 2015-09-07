
/* UART Register set */
typedef struct {

	//0x00
	union {
		volatile uint32_t REVNB_LO;

		volatile struct{
			unsigned MINOR : 6; //5-0
			unsigned CUSTOM : 2; //7-6
			unsigned MAJOR : 3; //10-8
			unsigned RTL : 5; //15-11
			unsigned rsvd8 : 16;		// 31-16
		} REVNB_LO_bit;
	}; //0x04

	union {
		volatile uint32_t REVNB_HI;

		volatile struct{
			unsigned FUNC : 12; //11-0
			unsigned rsvd1 : 2; //13-12
			unsigned SCHEME : 2; //15-14
			unsigned rsvd2 : 16;		// 31-16
		} REVNB_HI_bit;
	}; //0x08

	uint32_t rsvd1[2];

	//0x10
	union {
		volatile uint32_t SYSC;

		volatile struct{
			unsigned AUTOIDLE : 1;
			unsigned SRST : 1;
			unsigned ENAWAKEUP : 1;
			unsigned IDLEMODE : 2;
			unsigned rsvd1 : 3;
			unsigned CLKACTIVITY : 2;
			unsigned rsvd2 : 22;
		} SYSC_bit;
	}; //0x14

	uint32_t rsvd2[3];

	//0x20
	union {
		volatile uint32_t EOI;

		volatile struct{
			unsigned LINE_NUMBER : 1;
			unsigned rsvd8 : 31;
		} EOI_bit;
	}; //0x24

	union {
		volatile uint32_t IRQSTATUS_RAW;

		volatile struct{
			unsigned AL : 1;
			unsigned NACK : 1;
			unsigned ARDY : 1;
			unsigned RRDY : 1;
			unsigned XRDY : 1;
			unsigned GC : 1;
			unsigned STC : 1;
			unsigned AERR : 1;
			unsigned BF : 1;
			unsigned AAS : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned BB : 1;
			unsigned RDR : 1;
			unsigned XDR : 1;
			unsigned rsvd8 : 17;
		} IRQSTATUS_RAW_bit;
	}; //0x28

	union {
		volatile uint32_t IRQSTATUS;

		volatile struct{
			unsigned AL : 1;
			unsigned NACK : 1;
			unsigned ARDY : 1;
			unsigned RRDY : 1;
			unsigned XRDY : 1;
			unsigned GC : 1;
			unsigned STC : 1;
			unsigned AERR : 1;
			unsigned BF : 1;
			unsigned AAS : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned BB : 1;
			unsigned RDR : 1;
			unsigned XDR : 1;
			unsigned rsvd8 : 17;
		} IRQSTATUS_bit;
	}; //0x2C

	//IRQENABLE_SET
	union {
		volatile uint32_t IRQENABLE_SET;

		volatile struct{
			unsigned AL_IE : 1;
			unsigned NACK_IE : 1;
			unsigned ARDY_IE : 1;
			unsigned RRDY_IE : 1;
			unsigned XRDY_IE : 1;
			unsigned GC_IE : 1;
			unsigned STC_IE : 1;
			unsigned AERR_IE : 1;
			unsigned BF_IE : 1;
			unsigned AAS_IE : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned rsvd2 : 1;
			unsigned RDR_IE : 1;
			unsigned XDR_IE : 1;
			unsigned rsvd8 : 17;
		} IRQENABLE_SET_bit;
	}; //0x30

	//IRQENABLE_CLR
	union {
		volatile uint32_t IRQENABLE_CLR;

		volatile struct{
			unsigned AL_IE : 1;
			unsigned NACK_IE : 1;
			unsigned ARDY_IE : 1;
			unsigned RRDY_IE : 1;
			unsigned XRDY_IE : 1;
			unsigned GC_IE : 1;
			unsigned STC_IE : 1;
			unsigned AERR_IE : 1;
			unsigned BF_IE : 1;
			unsigned AAS_IE : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned rsvd2 : 1;
			unsigned RDR_IE : 1;
			unsigned XDR_IE : 1;
			unsigned rsvd8 : 17;
		} IRQENABLE_CLR_bit;
	}; //0x34

	//WE
	union {
		volatile uint32_t WE;

		volatile struct{
			unsigned AL_WE : 1;
			unsigned NACK_WE : 1;
			unsigned ARDY_WE : 1;
			unsigned DRDY_WE : 1;
			unsigned rsvd0 : 1;
			unsigned GC_WE : 1;
			unsigned STC_WE : 1;
			unsigned rsvd1 : 1;
			unsigned BF_WE : 1;
			unsigned AAS_WE : 1;
			unsigned XUDF_WE : 1;
			unsigned ROVR_WE : 1;
			unsigned rsvd2 : 1;
			unsigned RDR_WE : 1;
			unsigned XDR_WE : 1;
			unsigned rsvd8 : 17;
		} WE_bit;
	}; //0x38

	//DMARXENABLE_SET
	union {
		volatile uint32_t DMARXENABLE_SET;

		volatile struct{
			unsigned DMARX_ENABLE_SET : 1;
			unsigned rsvd8 : 31;
		} DMARXENABLE_SET_bit;
	}; //0x3C

	//DMATXENABLE_SET
	union {
		volatile uint32_t DMATXENABLE_SET;

		volatile struct{
			unsigned DMATX_TRANSMIT_SET : 1;
			unsigned rsvd8 : 31;
		} DMATXENABLE_SET_bit;
	}; //0x40

	//DMARXENABLE_CLR
	union {
		volatile uint32_t DMARXENABLE_CLR;

		volatile struct{
			unsigned DMARX_ENABLE_CLEAR : 1;
			unsigned rsvd8 : 31;
		} DMARXENABLE_CLR_bit;
	}; //0x44

	//DMATXENABLE_CLR
	union {
		volatile uint32_t DMATXENABLE_CLR;

		volatile struct{
			unsigned DMATX_ENABLE_CLEAR : 1;
			unsigned rsvd8 : 31;
		} DMATXENABLE_CLR_bit;
	}; //0x48

	//DMARXWAKE_EN
	union {
		volatile uint32_t DMARXWAKE_EN;

		volatile struct{
			unsigned AL : 1;
			unsigned NACK : 1;
			unsigned ARDY : 1;
			unsigned DRDY : 1;
			unsigned rsvd1 : 1;
			unsigned GC : 1;
			unsigned STC : 1;
			unsigned rsvd2 : 1;
			unsigned BF : 1;
			unsigned AAS : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned rsvd3 : 1;
			unsigned RDR : 1;
			unsigned XDR : 1;
			unsigned rsvd4 : 17;
		} DMARXWAKE_EN_bit;
	}; //0x4C

	//DMATXWAKE_EN
	union {
		volatile uint32_t DMATXWAKE_EN;

		volatile struct{
			unsigned AL : 1;
			unsigned NACK : 1;
			unsigned ARDY : 1;
			unsigned DRDY : 1;
			unsigned rsvd1 : 1;
			unsigned GC : 1;
			unsigned STC : 1;
			unsigned rsvd2 : 1;
			unsigned BF : 1;
			unsigned AAS : 1;
			unsigned XUDF : 1;
			unsigned ROVR : 1;
			unsigned rsvd3 : 1;
			unsigned RDR : 1;
			unsigned XDR : 1;
			unsigned rsvd4 : 17;
		} DMATXWAKE_EN_bit;
	}; //0x50

	uint32_t rsvd3[0x10];

	//0x90
	//SYSS
	union {
		volatile uint32_t SYSS;

		volatile struct{
			unsigned RDONE : 1;
			unsigned rsvd8 : 31;
		} SYSS_bit;
	}; //0x94

	//BUF
	union {
		volatile uint32_t BUF;

		volatile struct{
			unsigned TXTRSH : 6;
			unsigned TXFIFO_CLR : 1;
			unsigned XDMA_EN : 1;
			unsigned RXTRSH : 6;
			unsigned RXFIFO_CLR : 1;
			unsigned RDMA_EN : 1;
			unsigned rsvd8 : 16;
		} BUF_bit;
	}; //0x98

	//CNT
	union {
		volatile uint32_t CNT;

		volatile struct{
			unsigned CNT : 16;
			unsigned rsvd8 : 16;
		} CNT_bit;
	}; //0x9C

	//DATA
	union {
		volatile uint32_t DATA;

		volatile struct{
			unsigned DATA : 8;
			unsigned rsvd8 : 24;
		} DATA_bit;
	}; //0xA0

	uint32_t rsvd4;

	//0xA4
	//CON
	union {
		volatile uint32_t CON;

		volatile struct{
			unsigned STT : 1;
			unsigned STP : 1;
			unsigned rsvd1 : 2;
			unsigned XOA3 : 1;
			unsigned XOA02 : 1;
			unsigned XOA01 : 1;
			unsigned XOA0 : 1;
			unsigned XSA : 1;
			unsigned TRX : 1;
			unsigned MST : 1;
			unsigned STB : 1;
			unsigned OPMODE : 2;
			unsigned rsvd2 : 1;
			unsigned I2C_EN : 1;
			unsigned rsvd3 : 16;
		} CON_bit;
	}; //0xA8

	//OA
	union {
		volatile uint32_t OA;

		volatile struct{
			unsigned OA : 10;
			unsigned rsvd8 : 22;
		} OA_bit;
	}; //0xAC

	//SA
	union {
		volatile uint32_t SA;

		volatile struct{
			unsigned SA : 10;
			unsigned rsvd8 : 22;
		} SA_bit;
	}; //0xB0

	//PSC
	union {
		volatile uint32_t PSC;

		volatile struct{
			unsigned PSC : 8;
			unsigned rsvd8 : 24;
		} PSC_bit;
	}; //0xB4

	//SCLL
	union {
		volatile uint32_t SCLL;

		volatile struct{
			unsigned SCLL : 8;
			unsigned rsvd8 : 24;
		} SCLL_bit;
	}; //0xB8

	//SCLH
	union {
		volatile uint32_t SCLH;

		volatile struct{
			unsigned SCLH : 8;
			unsigned rsvd8 : 24;
		} SCLH_bit;
	}; //0xBC

	//SYSTEST
	union {
		volatile uint32_t SYSTEST;

		volatile struct{
			unsigned SDA_O : 1;
			unsigned SDA_I : 1;
			unsigned SCL_O : 1;
			unsigned SCL_I : 1;
			unsigned rsvd1 : 1;
			unsigned SDA_O_FUNC : 1;
			unsigned SDA_I_FUNC : 1;
			unsigned SCL_O_FUNC : 1;
			unsigned SCL_I_FUNC : 1;
			unsigned SSB : 1;
			unsigned TMODE : 2;
			unsigned FREE : 1;
			unsigned ST_EN : 1;
			unsigned rsvd2 : 16;
		} SYSTEST_bit;
	}; //0xC0

	//BUFSTAT
	union {
		volatile uint32_t BUFSTAT;

		volatile struct{
			unsigned TXSTAT : 6;
			unsigned rsvd1 : 2;
			unsigned RXSTAT : 6;
			unsigned FIFODEPTH : 2;
			unsigned rsvd2 : 16;
		} BUFSTAT_bit;
	}; //0xC4

	//OA1
	union {
		volatile uint32_t OA1;

		volatile struct{
			unsigned OA1 : 10;
			unsigned rsvd8 : 22;
		} OA1_bit;
	}; //0xC8

	//OA2
	union {
		volatile uint32_t OA2;

		volatile struct{
			unsigned OA2 : 10;
			unsigned rsvd8 : 22;
		} OA2_bit;
	}; //0xCC

	//OA3
	union {
		volatile uint32_t OA3;

		volatile struct{
			unsigned OA3 : 10;
			unsigned rsvd8 : 22;
		} OA3_bit;
	}; //0xD0

	//ACTOA
	union {
		volatile uint32_t ACTOA;

		volatile struct{
			unsigned OA0_ACT : 1;
			unsigned OA1_ACT : 1;
			unsigned OA2_ACT : 1;
			unsigned OA3_ACT : 1;
			unsigned rsvd8 : 28;
		} ACTOA_bit;
	}; //0xD4

	//SBLOCK
	union {
		volatile uint32_t SBLOCK;

		volatile struct{
			unsigned OA0_EN : 1;
			unsigned OA1_EN : 1;
			unsigned OA2_EN : 1;
			unsigned OA3_EN : 1;
			unsigned rsvd8 : 28;
		} SBLOCK_bit;
	};

} pruI2C;

typedef struct {

	union {
		volatile uint32_t CLK_CTRL;

		volatile struct{
			unsigned MODULEMODE : 2;
			unsigned rsvd1 : 14;
			unsigned IDLEST : 2;
			unsigned rsvd8 : 14;
		} CLK_CTRL_bit;
	};

} pruCMPerI2CClkCtrl;
