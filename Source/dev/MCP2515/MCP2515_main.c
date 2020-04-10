
#include "dev.h"
#include "MCP2515_main.h"


void MCP2515_SPI_WRITE(UCHAR awAddr, UCHAR awData)
{
    Cmd_gpio=0x02;
    Dat_gpio0=awAddr;
    Dat_gpio1=awData;

    SpiSetCs(MCP2515_SPI_CH0, 0); // CS Low

    SpiWrite(MCP2515_SPI_CH0, &Cmd_gpio);
    SpiWrite(MCP2515_SPI_CH0, &Dat_gpio0);
    SpiWrite(MCP2515_SPI_CH0, &Dat_gpio1);

    SpiSetCs(MCP2515_SPI_CH0, 1); // CS High
}

UCHAR MCP2515_SPI_READ(UCHAR awAddr)
{
    BYTE bRbuf=0;
    Cmd_gpio=0x03;
    Dat_gpio0=awAddr;

    SpiSetCs(MCP2515_SPI_CH0, 0); // CS Low

    SpiWrite(MCP2515_SPI_CH0, &Cmd_gpio);
    SpiWrite(MCP2515_SPI_CH0, &Dat_gpio0);
    SpiRead(MCP2515_SPI_CH0, &bRbuf);

    SpiSetCs(MCP2515_SPI_CH0, 1); // CS High

    return (bRbuf);
}

//------------------------------------------------------------
// Set the controller in loopback mode
void can_set_loopback_mode_GPIO(void) {

    MCP2515_SPI_WRITE(MCP_CANCTRL, 0x47);
    //while ((can_read_reg(MCP_CANSTAT) & 0xE0 != 0x40) != 0);

    return;
}

//------------------------------------------------------------
// Set the controller in configuration mode
void can_set_config_mode_GPIO(void) {

    MCP2515_SPI_WRITE(MCP_CANCTRL, 0x87);
    printf("MCP2515_SPI_WRITE : 0x87\r\n");
    //while ((can_read_reg(MCP_CANSTAT) & 0xE0) != 0);

    printf("MCP2515_SPI_READ : 0x%2x\r\n",MCP2515_SPI_READ(MCP_CANCTRL));

    return;
}

//------------------------------------------------------------
// Set the controller in normal mode
void can_set_normal_mode_GPIO(void) {

    MCP2515_SPI_WRITE(MCP_CANCTRL, 0x07);
    //while ((can_read_reg(MCP_CANSTAT) & 0xE0) != 0);

    return;
}

// CAN reset
void can_reset_GPIO(void) {
    Cmd_gpio = MCP_RESET;

	SpiSetCs(MCP2515_SPI_CH0, 0); // CS Low

    SpiWrite(MCP2515_SPI_CH0, &Cmd_gpio);

    SpiSetCs(MCP2515_SPI_CH0, 1); // CS High
}

////////////////////////////////////////////////////////////
// can_peek_messages
//
// Checks for message in Receive Buf 1.  If there is already
// a message pending, return with a value 1.
//
// If message pending:
//     Load 'can_msg_id_gpio' with ID.
//     Load 'can_rx_count_gpio' with number of bytes of data received.
//     Load 'can_buffer_gpio' with data
//     Clear 2510 Receive Buffer 1 interrupt flag
//     Set rx_message_pending flag
//
// If message is already pending, the function doesn't
// check for new messages since it would overwrite the
// current data.
//
short can_peek_message_GPIO(void) {

    //// Check if there are already messages pending
    //if (rx_message_pending){
    //    return 1;
    //}

    // Check for pending messages in RX buffer 0
    if ((MCP2515_SPI_READ(MCP_CANINTF) & 0x01) == 0) {
        return 0;
    }

    //// If we're here, there is something waiting
    //rx_message_pending = 1;
    //can_rx_pointer     = 0;

    // Strip the ID of the message source
    //can_msg_id_gpio = (SPI_RD(MCP_RXB0SIDH) << 8);
    //can_msg_id_gpio |= (SPI_RD(MCP_RXB0SIDL) & 0xE0);
    can_msg_id_gpio = (MCP2515_SPI_READ(MCP_RXB0SIDH) << 3);

    // Get number of databytes
    can_rx_count_gpio = (MCP2515_SPI_READ(MCP_RXB0DLC) & 0x0F);

    // Read the data from the MCP2515 rx buffer
    unsigned char lcount = 0;
 //   char data = 0;

    while (lcount < can_rx_count_gpio) {
        // Get the data byte and store in the local buffer
        can_buffer_gpio[lcount] = (UCHAR) MCP2515_SPI_READ((UCHAR)(MCP_RXB0D0 + lcount));
        lcount++;
    }

    // Clear RX buffer 0 interrupt signal
    MCP2515_SPI_WRITE(MCP_CANINTF, 0x00);

    return can_rx_count_gpio;
}

//void can_init_buffer(){
//    can_tx_pointer_gpio = 0;
//    return;
//}
//
//void can_load_byte(char input){
//    SPI_WR_BYTE(MCP_TXB0D0+can_tx_pointer_gpio, input);
//    can_tx_pointer_gpio++;
//}
//
//void can_tx_buffer(){
//    // Wait for pending messages to be sent
//    while(SPI_RD(MCP_TXB0CTRL)& 0x08){
//        //nop();
//    }
//
//    // Load the number of bytes to transmit
//    can_write_bits(TXB0DLC, can_tx_pointer_gpio, 0x0F); // modify
//
//    // Request transmission
//    can_write_cmd(0x81);
//
//    return;
//
//}

void can_tx_GPIO(short id, UCHAR data_len, UCHAR data[8]){
    int i = 0;
    register unsigned int reg_data = 0;

    // Wait for pending messages to be sent
    reg_data = MCP2515_SPI_READ(MCP_TXB0CTRL);
    while(reg_data & 0x08){
        reg_data = MCP2515_SPI_READ(MCP_TXB0CTRL);
    //printf("0x%02x",reg_data);
        //nop();
    }

    // Load CAN ID
    UCHAR idh = (id & 0x07F8) >> 3;
    UCHAR idl = (id & 0x0007);
    MCP2515_SPI_WRITE(MCP_TXB0SIDH, idh);
    MCP2515_SPI_WRITE(MCP_TXB0SIDL, idl<<5);

    // Load the number of bytes to transmit
    MCP2515_SPI_WRITE(MCP_TXB0DLC, data_len);

    // Load data to transmit buffer
    for(i=0; i<8; i++)
        MCP2515_SPI_WRITE(MCP_TXB0D0+i, data[i]);

    // Request transmission
    MCP2515_SPI_WRITE(MCP_TXB0CTRL, 0x08 | 0x3); // TX request, Priority(Highest)

    return;
}

#if 0
void can_set_id(short id){
    UCHAR idh = (id & 0x03FF) >> 3;
    UCHAR idl = (id & 0x0007);
    SPI_WR_BYTE(MCP_TXB0SIDH, idh);
    SPI_WR_BYTE(MCP_TXB0SIDL, idl<<5);
}

void can_rd_reg(unsigned int addr, char *name) {
    register unsigned int reg_data = 0;

    reg_data = SPI_RD(addr);
    printf("%s[%02x]: %02x \r\n", name, addr, reg_data);
}

void test_reg_read() {

    can_rd_reg(MCP_CNF1, "MCP_CNF1    ");
    can_rd_reg(MCP_CNF2, "MCP_CNF2    ");
    can_rd_reg(MCP_CNF3, "MCP_CNF3    ");
    can_rd_reg(MCP_RXM0SIDH, "MCP_RXM0SIDH");
    can_rd_reg(MCP_RXM0SIDL, "MCP_RXM0SIDL");
    can_rd_reg(MCP_RXM0EID8, "MCP_RXM0EID8");
    can_rd_reg(MCP_RXM0EID0, "MCP_RXM0EID0");
    can_rd_reg(MCP_RXM1SIDH, "MCP_RXM1SIDH");
    can_rd_reg(MCP_RXM1SIDL, "MCP_RXM1SIDL");
    can_rd_reg(MCP_RXM1EID8, "MCP_RXM1EID8");
    can_rd_reg(MCP_RXM1EID0, "MCP_RXM1EID0");

    can_rd_reg(MCP_RXF0SIDH, "MCP_RXF0SIDH");
    can_rd_reg(MCP_RXF0SIDL, "MCP_RXF0SIDL");
    can_rd_reg(MCP_RXF1SIDH, "MCP_RXF1SIDH");
    can_rd_reg(MCP_RXF1SIDL, "MCP_RXF1SIDL");
    can_rd_reg(MCP_RXF2SIDL, "MCP_RXF2SIDL");
    can_rd_reg(MCP_RXF2EID8, "MCP_RXF2EID8");
    can_rd_reg(MCP_RXF3SIDL, "MCP_RXF3SIDL");
    can_rd_reg(MCP_RXF3EID8, "MCP_RXF3EID8");
    can_rd_reg(MCP_RXF4SIDL, "MCP_RXF4SIDL");
    can_rd_reg(MCP_RXF4EID8, "MCP_RXF4EID8");
    can_rd_reg(MCP_RXF5SIDL, "MCP_RXF5SIDL");
    can_rd_reg(MCP_RXF5EID8, "MCP_RXF5EID8");
}
#endif

///////////////////////////////////////////////////////////////
void MCP2515_Init(void)
{
    register unsigned int reg_data = 0;

    // Initailize the MCP2515
    can_reset_GPIO();

    WaitXus(10000); // for Stable Time??

    can_set_config_mode_GPIO();

    reg_data = MCP2515_SPI_READ(MCP_CANSTAT);

    while ((reg_data & 0xE0) != 0x80) {
        can_set_config_mode_GPIO();
        reg_data = MCP2515_SPI_READ(MCP_CANSTAT);
        printf("Debuging REG Data - 0x%x\r\n", reg_data);
    }

    // Set physical layer configuration
    //
    //     Fosc = 16MHz
    //     BRP        =   7  (divide by 8)
    //     Sync Seg   = 1TQ
    //     Prop Seg   = 1TQ
    //     Phase Seg1 = 3TQ
    //     Phase Seg2 = 3TQ
    //
    //    TQ = 2 * (1/Fosc) * (BRP+1)
    //    Bus speed = 1/(Total # of TQ) * TQ

    // set BRP to div by 8 // not tested @16MHz
    //SPI_WR_BYTE(MCP_CNF1, 0x07);
    //SPI_WR_BYTE(MCP_CNF2, 0x90);
    //SPI_WR_BYTE(MCP_CNF3, 0x02);
    //SPI_WR_BYTE(MCP_CNF1, 0x04);
    //SPI_WR_BYTE(MCP_CNF2, 0xb1);
    //SPI_WR_BYTE(MCP_CNF3, 0x05);

    // 500Kbps baudrate @20Mhz
    //SPI_WR_BYTE(MCP_CNF1, 0x01);
    //SPI_WR_BYTE(MCP_CNF2, 0x92);
    //SPI_WR_BYTE(MCP_CNF3, 0x82);

    // 500Kbps baudrate @8Mhz
    //MCP2515_SPI_WRITE(MCP_CNF1, 0x00);  // FPGA (External OSC)
    //MCP2515_SPI_WRITE(MCP_CNF2, 0x90);  // FPGA (External OSC)
    //MCP2515_SPI_WRITE(MCP_CNF3, 0x82);  // FPGA (External OSC)
    MCP2515_SPI_WRITE(MCP_CNF1, 0x00); // SoC (16MHz OSC)
    MCP2515_SPI_WRITE(MCP_CNF2, 0x9a); // SoC (16MHz OSC)
    MCP2515_SPI_WRITE(MCP_CNF3, 0x07); // SoC (16MHz OSC)

    //// Set up the MCP receive buffer 0 mask to receive broadcast messages.
    //// Set up receive buffer 1 mask to receive messages sent to the
    //// global address (or eventually us).  Set up bit timing as defined by
    //// the CA, and set up interrupts on either receive buffer full.
    //SPI_WR_BYTE(MCP_RXM0SIDH, 0x00);
    //SPI_WR_BYTE(MCP_RXM0SIDL, 0x03);
    //SPI_WR_BYTE(MCP_RXM0EID8, 0xFF);
    //SPI_WR_BYTE(MCP_RXM0EID0, 0xFF);
    //SPI_WR_BYTE(MCP_RXM1SIDH, 0x00);
    //SPI_WR_BYTE(MCP_RXM1SIDL, 0x03);
    //SPI_WR_BYTE(MCP_RXM1EID8, 0xFF);
    //SPI_WR_BYTE(MCP_RXM1EID0, 0xFF);

    //// Set all the RXB0 filters to accept only broadcast messages
    //// (PF = 240-255).  Set all the RXB1 filters to accept only the
    //// global address.  Once we get an address for the CA, we'll change
    //// filter 2 to accept that address.
    //SPI_WR_BYTE(MCP_RXF0SIDH, 0x07 );
    //SPI_WR_BYTE(MCP_RXF0SIDL, 0x88 );
    //SPI_WR_BYTE(MCP_RXF1SIDH, 0x07 );
    //SPI_WR_BYTE(MCP_RXF1SIDL, 0x88 );
    //SPI_WR_BYTE(MCP_RXF2SIDL, 0x08 );
    //SPI_WR_BYTE(MCP_RXF2EID8, 0xFF );
    //SPI_WR_BYTE(MCP_RXF3SIDL, 0x08 );
    //SPI_WR_BYTE(MCP_RXF3EID8, 0xFF );
    //SPI_WR_BYTE(MCP_RXF4SIDL, 0x08 );
    //SPI_WR_BYTE(MCP_RXF4EID8, 0xFF );
    //SPI_WR_BYTE(MCP_RXF5SIDL, 0x08 );
    //SPI_WR_BYTE(MCP_RXF5EID8, 0xFF );

    // Disable all MCP2510 Interrupts
    //SPI_WR_BYTE(MCP_CANINTE, 0x00 );
    MCP2515_SPI_WRITE(MCP_CANINTE, 0x3);

    //reg_data = SPI_RD(MCP_CNF1);
    //printf("CNF1: 0x",reg_data);
    //reg_data = SPI_RD(MCP_CNF2);
    //printf("CNF2: 0x",reg_data);
    //reg_data = SPI_RD(MCP_CNF3);
    //printf("CNF3: 0x",reg_data);
    //reg_data = SPI_RD(MCP_CANINTE);
    //printf("CANINTE: 0x",reg_data);

    // Set normal mode (not loopback)
    //can_set_loopback_mode();
    can_set_normal_mode_GPIO();

    while ((MCP2515_SPI_READ(MCP_CANSTAT) & 0xE0) != 0x00)
        can_set_normal_mode_GPIO();
}

/* EOF */
