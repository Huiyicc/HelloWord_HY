# HelloWord-Dynamic

---

## ����
[HelloWord-Dynamic](https://github.com/Huiyicc/HelloWord_HY/tree/master/HelloWord-Dynamic-fw)�ǻ������;�([@peng-zhihui](https://github.com/peng-zhihui))ԭ�� [Dynamic](https://github.com/peng-zhihui/HelloWord-Keyboard/tree/main/2.Firmware/HelloWord-Dynamic-fw) �����Ͻ��д�������,�޸�֮��İ汾

## ˵��

## ���¼ƻ�

- [x] �������
  - [x] ������ģʽ
    - [x] ���ɱ�����ģʽ
    - [x] ������ģʽ
    - [x] ����ģʽ
    - [x] ҡ��ģʽ
    - [x] ��λģʽ
    - [ ] ����ģʽ(���������Ϊ������ģʽ,���������Ϊ����ģʽ)
  - [ ] ���˫��ͨ��(��λ������ָ��������/�������λ�ø���λ��)
  - [ ] ���λ�÷���
  - [ ] �޸�ż�����Ѻ�Ƕȿ���ģʽ���е���������
  - [ ] �޸�����ڵ���ʱ���ж���������
- [x] RGB��Ч
- [x] �͹�������ģʽ
- [x] ����APP�ܹ�
  - [x] ���ȵ���
  - [x] ��������
  - [x] ����/�������ģʽ
  - [x] ����/��Ƶ���ȵ���
  - [x] �Ӵ�ģʽ
  - [ ] APPӦ���г�
- [x] ��λ��(��ҳ/APP)����
- [x] ˫��ť�̰�/�����¼�
  - [ ] ˫��ť˫��/����¼�
- [x] īˮ������
  - [x] ȫˢ
  - [ ] ��ˢ
  - [ ] ��ˢ
  - [ ] ��ɫ��
- [ ] �޸�UART(���ڸ�����HIDͨ��ģʽ,����UARTͨ��Ҫ����,���ڽ���ڴ����������)

# �Զ���ͼ���ֿ�
- ���Ȱ�װ [fontforge](https://github.com/fontforge/fontforge)
> fontforge ��������:  
> - ����fontforge��װĿ¼,�༭fontforge.bat  
> - �ҵ�set LANGUAGE=en,ɾ��ǰ���::  
> - �޸�Ϊset LANGUAGE=zh_CN  
> - ����

- ʹ��fontforge��`hwd_hy.sfd`
- ����ttf
- ��guitool,ʹ�øոյ�ttf����bpf�ļ�(��������24,����00-FF�ַ�)
- ��������map
- �����н���scripts�ļ���
- `"./u8g2/tools/font/bdfconv/bdfconv.exe"  -v -b 0 -f 1 <bdf�ļ�>  -M "../font/hwd_hy.map" -n hwd_hy_U24 -o  -o "../font/hwd_hy_U24.c" -d <bdf�ļ�>`

# �������

---

- [peng-zhihui/HelloWord-Keyboard](https://github.com/peng-zhihui/HelloWord-Keyboard)  
- [fontforge](https://github.com/fontforge/fontforge)

# Э��

---

MIT License