/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
@��   д������Ƽ�
@��   �䣺chiusir@163.com
@����IDE��Linux ������VSCode_1.93 �����ϰ汾��Cmake_3.16 �����ϰ汾
@ʹ��ƽ̨����о2K0300�þ��ɺͱ����������ܿƼ���о�þ�����չ��
@�����Ϣ�ο����е�ַ
    ��      վ��http://www.lqist.cn
    �� �� �� �̣�http://longqiu.taobao.com
    ����������Ƶ��https://space.bilibili.com/95313236
@����汾��V1.0 ��Ȩ���У���λʹ��������ϵ��Ȩ
@�ο���Ŀ���ӣ�https://github.com/AirFortressIlikara/ls2k0300_peripheral_library

@�޸����ڣ�2025-03-04
@�޸����ݣ�
@ע�������Ҫʹ��Ӳ��SPI�������豸���������ʹ�õ��豸�����������SPI�豸�ļ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#include "LQ_HW_SPI.hpp"

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HW_SPI::HW_SPI()
 * @����˵����Ӳ�� SPI ���޲ι��캯��
 * @����˵������
 * @�������أ���
 * @���÷�����HW_SPI MySPI;
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::HW_SPI()
{

}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HW_SPI::HW_SPI(const char *file, uint8_t mode, uint8_t bits, uint32_t speed) :
            Mode(mode), Bits(bits), Speed(speed)
 * @����˵����Ӳ�� SPI ���вι��캯��
 * @����˵����file   :  SPI �豸�ļ�����·��
 * @����˵����mode   :  SPI ģʽ��Ĭ��ģʽΪSPI_MODE_0
 * @����˵����bits   :  ���ݴ���λ����Ĭ��Ϊ8λ
 * @����˵����speed  :  ���ݴ����ٶȣ�Ĭ���ٶ�Ϊ50000000
 * @�������أ���
 * @���÷�����HW_SPI MySPI("/dev/spidev1.0", SPI_MODE_0, 8, 50000000);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::HW_SPI(const char *file, uint8_t mode, uint8_t bits, uint32_t speed) :
            Mode(mode), Bits(bits), Speed(speed)
{
    /* �� SPI �豸�ļ� */
    this->spi_fd = open(file, O_RDWR);
    if (this->spi_fd < 0)
    {
        perror("open spi file error");
        return;
    }
    /* ���� SPI ģʽ */
    this->SetMode(this->Mode);
    /* ���� SPI ÿ�η��ͻ�����ֽ��� */
    this->SetBits(this->Bits);
    /* ���� SPI ���ݴ����ٶ� */
    this->SetSpeed(this->Speed);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HW_SPI::SetMode(uint8_t mode)
 * @����˵��������Ӳ�� SPI ��ģʽ
 * @����˵����mode  :  Ӳ�� SPI ģʽ
 * @�������أ���
 * @���÷�����MySPI.SetMode(SPI_MODE_0);
 * @��ע˵����SPI �Ĵ���ģʽ������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetMode(uint8_t mode)
{
    this->Mode = mode;
    ioctl(this->spi_fd, SPI_IOC_WR_MODE, &this->Mode);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HW_SPI::SetBits(uint8_t bits)
 * @����˵��������Ӳ�� SPI ��λ��
 * @����˵����mode  :  SPI �������ݵ�λ��
 * @�������أ���
 * @���÷�����MySPI.SetBits(8);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetBits(uint8_t bits)
{
    this->Bits = bits;
    ioctl(this->spi_fd, SPI_IOC_WR_BITS_PER_WORD, &this->Bits);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HW_SPI::SetSpeed(uint32_t speed)
 * @����˵��������Ӳ�� SPI �Ĵ����ٶ�
 * @����˵����mode  :  ���ݴ����ٶ�
 * @�������أ���
 * @���÷�����MySPI.SetSpeed(50000000);
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SetSpeed(uint32_t speed)
{
    this->Speed = speed;
    ioctl(this->spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &this->Speed);
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�void HW_SPI::SPI_Write(uint8_t tx[], uint8_t tx_Num)
 * @����˵����Ӳ�� SPI �����ݷ��ͺ���
 * @����˵����tx      :  �������ݵ�����
 * @����˵����tx_Num  :  �������ݵĳ���
 * @�������أ���
 * @���÷�����MySPI.SPI_Write(tx, sizeof(tx));
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void HW_SPI::SPI_Write(uint8_t tx[], uint8_t tx_Num)
{
    this->Tr = {
        .tx_buf = (unsigned long)tx,    // ���÷��ͻ�����
        .rx_buf = 0,                    // ���������ݣ���������Ϊ0
        .len = tx_Num,                  // ���÷������ݵĳ���
    };
    /* ִ�� SPI ������� */
    int ret = ioctl(this->spi_fd, SPI_IOC_MESSAGE(1), &this->Tr);
    if (ret < 0)
    {
        perror("can't send spi message");
    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 * @�������ƣ�HW_SPI::~HW_SPI()
 * @����˵����Ӳ�� SPI ����������
 * @����˵������
 * @�������أ���
 * @���÷����������Ķ����������ڽ�����ϵͳ�Զ�����
 * @��ע˵������
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
HW_SPI::~HW_SPI()
{
    close(this->spi_fd);
}
