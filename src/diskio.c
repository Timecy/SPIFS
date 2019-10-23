#include "diskio.h"

/*
�������ʹ�ü�¼��
��¼��ռ������0�ĵ�512�ֽ�
�ܼƿɱ�ʾ4096����ʹ�����
bit0~bit3 ���ڱ�ʶ����0~����3�ļ���ʹ�����
1: ���������п����ļ���ռ�
0: ����������
�����¼������ռ��/δռ��
1: δռ��
0: ��ʹ��
@param sec_id �����߼����
*/
uint8_t sector_isempty(uint32_t sec_id) {
    uint8_t mem = 0xFF;
    uint32_t idx = (sec_id / 8) * 8, pos = sec_id % 8;
    w25q32_read(&mem, 1, idx);
    return (mem >> pos) & 0x1;
}

/*
������ʹ�ü�¼���ע������ʹ��
@param sec_id �����߼����
*/
uint8_t mark_sector_inuse(uint32_t sec_id) {
    uint8_t mem = 0xFF;
    uint8_t res = 0x00;
    uint32_t idx = (sec_id / 8) * 8, pos = sec_id % 8;
    w25q32_read(&mem, 1, idx);
    if(((mem >> pos) & 0x1) == 0) {
        return 0;
    }
    mem &= (~(0x1 << pos));
    res = w25q32_write_page(&mem, 1, idx);
    return (res == 0x2);
}

/*
д�ļ����¼
addrΪ�����ַ��Ӧ�ڣ�[����0 0x200 ~ ����3 0x3FFF]
@param addr д��������ַ
@param *fb �ļ��ṹ��ָ��
*/
void write_fileblock(uint32_t addr, FileBlock *fb) {
    uint8_t *slot_buffer = (uint8_t *)fb;
    w25q32_write_page(slot_buffer, 24, addr);
}

void write_value(uint32_t addr, uint32_t value, uint8_t bytes) {
    uint8_t buffer[bytes];
    for(uint8_t i = 0; i < bytes; i++) {
        buffer[i] = (value >> (i * 8)) & 0xFF;
    }
    w25q32_write_page(buffer, bytes, addr);
}

void write_fileblock_cluster(uint32_t fbaddr, uint32_t cluster) {
    write_value(fbaddr + 12, cluster, 4);
}

void write_fileblock_length(uint32_t fbaddr, uint32_t length) {
    write_value(fbaddr + 16, length, 4);
}

void write_fileblock_state(uint32_t fbaddr, uint8_t state) {
    write_value(fbaddr + 23, state, 1);
}
