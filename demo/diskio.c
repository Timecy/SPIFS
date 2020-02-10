#include "diskio.h"

/**
 * ��ȡ
 * @param address ��ַ
 * @param buffer ���뻺����
 * @param size ��ȡ��С(�ֽ�)
 * @return ʵ�ʶ�ȡ��С(�ֽ�)
 **/
uint32_t disk_read(uint32_t address, uint8_t *buffer, uint32_t size) {
    return w25q32_read(address, buffer, size);
}

/**
 * д��
 * @param address ��ַ
 * @param buffer ���뻺����
 * @param size д���С(�ֽ�)
 * @return 0x2: д��ɹ�
 * */
uint8_t disk_write(uint32_t address, uint8_t *buffer, uint32_t size) {
    return w25q32_write_page(address, buffer, size);
}

/**
 * ��������
 * @param address �����׵�ַ
 * @return 0x2: �����ɹ�
 * */
uint8_t chip_erase() {
    return w25q32_chip_erase();
}

/**
 * ��������
 * @param address �����׵�ַ
 * @return 0x2: �����ɹ�
 * */
uint8_t sector_erase(uint32_t address) {
    return w25q32_sector_erase(address);
}

/**
 * д�ļ����¼
 * @param addr �����ַ
 * @param *fb �ļ��ṹ��ָ��
 * */
void write_fileblock(uint32_t addr, FileBlock *fb) {
    uint8_t *slot_buffer = (uint8_t *)fb;
    w25q32_write_page(addr, slot_buffer, 24);
}

/**
 * �����ļ����¼
 * @param baseAddr ��ַ
 * @param offset ƫ����
 * */
void clear_fileblock(uint8_t *baseAddr, uint32_t offset) {
    for(uint32_t i = 0; i < FILEBLOCK_SIZE; i++) {
        *(baseAddr + offset + i) = 0xFF;
    }
}

/**
 * ��ָ����ַдֵ
 * @param addr �����ַ
 * @param value д������
 * @param bytes �ֽ���
 * */
void write_value(uint32_t addr, uint32_t value, uint8_t bytes) {
    uint8_t buffer[4];
    if(bytes > 4) return;
    for(uint8_t i = 0; i < bytes; i++) {
        buffer[i] = (value >> (i << 3)) & 0xFF;
    }
    w25q32_write_page(addr, buffer, bytes);
}

/**
 * д�ļ����������״ص�ַ
 * @param fbaddr �ļ����ַ
 * @param cluster �״ص�ַ
 * */
void write_fileblock_cluster(uint32_t fbaddr, uint32_t cluster) {
    write_value(fbaddr + 12, cluster, 4);
}

/**
 * д�ļ����ļ�����
 * @param fbaddr �ļ����ַ
 * @param length �ļ�����
 * */
void write_fileblock_length(uint32_t fbaddr, uint32_t length) {
    write_value(fbaddr + 16, length, 4);
}

/**
 * д�ļ����ļ�״̬�ֶ�
 * @param fbaddr �ļ����ַ
 * @param state �ļ�״̬�ֶ�
 * */
void write_fileblock_state(uint32_t fbaddr, uint8_t state) {
    write_value(fbaddr + 23, state, 1);
}
