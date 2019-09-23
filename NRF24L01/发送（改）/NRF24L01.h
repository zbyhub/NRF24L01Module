#ifndef _NREF24L01_H
#define _NREF24L01_H
/*********************************************************************************************************/
typedef unsigned int uint;
typedef unsigned char uchar;
#define TX_PLOAD_WIDTH 32   
#define RX_PLOAD_WIDTH 32

/*********************************************************************************************************/
void delayus(uint t);
void delayms(uint t);
/*********************************************************************************************************/
uchar SPI_RW(uchar byte);//SPI����������Ҫ��
uchar SPI_write(uchar add,daa);//��daa����д��add�Ĵ���
uchar SPI_read(uchar add);//��add�Ĵ���������
uchar SPI_read_buf(uchar add, uchar *pbuf, uchar num);
uchar SPI_write_buf(uchar add, uchar *pbuf, uchar num);
/*********************************************************************************************************/
void SendInit_NRF24L01(void);//���ͳ�ʼ��(�Ѱ���Ƶ���趨)
void SetTx_Mode(void);
void NRF24L01_Tx_ADDR(uchar *tx_addr);
void NRF24L01_TxPacket(uchar *tx_buf);//�ò��迪�����䣨��CE = 1���������CE = 0�رշ�����䣬���������ж�����SPI_write(WRITE_REG + STATUS, 0xff)���͡��������SPI_write(FLUSH_TX, 0xff)��
/*********************************************************************************************************/
void RecInit_NRF24L01(void);//���ճ�ʼ��(�Ѱ���Ƶ���趨)
void NRF24L01_Rx_Channel(uchar channel);
void NRF24L01_Rx_ADDR(uchar *rx_addr,uchar channel,uchar Addr_Num);
void SetRx_Mode(void);//�ò��迪�����գ���CE = 1��
void NRF24L01_RxPacket(uchar *rx_buf);//���ڲ������ж����㣩
/*********************************************************************************************************/
void NRF24L01_Freq(uint fre);
void first();
/*********************************************************************************************************/
sbit CE   = P1^0;
sbit CSN  = P1^1;
sbit IRQ  = P1^2;
sbit MISO = P1^3;
sbit MOSI = P1^4;
sbit SCK  = P1^5;

/***********************************************************************/


//NRF24L01�Ĵ���ָ��
#define READ_REG        0x00   // ���Ĵ���ָ��
#define WRITE_REG       0x20  // д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61   // ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0   // д��������ָ��
#define FLUSH_TX        0xE1  // ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2   // ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3   // �����ظ�װ������ָ��
#define NOP             0xFF   // ����
//SPI(nRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define STATUS          0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��0�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��0�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��0�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��0�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��0�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
#endif