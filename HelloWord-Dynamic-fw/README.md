# HelloWord-Dynamic

---

## ����
[HelloWord-Dynamic](https://github.com/Huiyicc/HelloWord_HY/tree/master/HelloWord-Dynamic-fw)�ǻ������;�([@peng-zhihui](https://github.com/peng-zhihui))ԭ�� [Dynamic](https://github.com/peng-zhihui/HelloWord-Keyboard/tree/main/2.Firmware/HelloWord-Dynamic-fw) �����Ͻ��д�������,�޸�֮��İ汾

## ˵��

�����ο����;���[clion����](https://zhuanlan.zhihu.com/p/145801160)

## ���¼ƻ�

- [x] �������
  - [x] ������ģʽ
    - [x] ���ɱ�����ģʽ
    - [x] ������ģʽ
    - [x] ����ģʽ
    - [x] ҡ��ģʽ
    - [x] ��λģʽ
    - [x] ����ģʽ
      - > ���������Ϊ������ģʽ,���������Ϊ����ģʽ
  - [x] ���˫��ͨ��
    - > ��λ������ָ��������/�������λ�ø���λ��
  - [x] ���λ�÷���
  - [ ] �޸�ż�����Ѻ�Ƕȿ���ģʽ���е���������
    - > �Ѿ�����ѹ����С��,����������û���豸��������,�޷�����������.
  - [x] �޸�����ڵ���ʱ���ж���������
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
  - [x] ˫��ť˫��/����¼�
- [x] īˮ������
  - [x] ȫˢ
  - [x] ��ˢ
  - [ ] ��ˢ
  - [ ] ��ɫ��
- [ ] �޸�UART
  - > ���ڸ�����HIDͨ��ģʽ,��Ҫ��Ϊ�����豸,emmmm,˭����˭��,̫�鷳��

# �Զ���ͼ���ֿ�
- ���Ȱ�װ [fontforge](https://github.com/fontforge/fontforge)
> fontforge ��������:  
> - ����fontforge��װĿ¼,�༭fontforge.bat  
> - �ҵ�set LANGUAGE=en,ɾ��ǰ���::  
> - �޸�Ϊset LANGUAGE=zh_CN  
> - ����

- ʹ��fontforge��`hwd_hy.sfd`
- ����ttf
- ��[guitool](./scripts/guitooldswda.zip),ʹ�øոյ�ttf����bpf�ļ�(��������24,����00-FF�ַ�)
- ��������map
- �����н���scripts�ļ���
- `"./u8g2/tools/font/bdfconv/bdfconv.exe"  -v -b 0 -f 1 <bdf�ļ�>  -M "../font/hwd_hy.map" -n hwd_hy_U24 -o  -o "../font/hwd_hy_U24.c" -d <bdf�ļ�>`
- �����ɵ�`hwd_hy_U24.c`����Ҫ���ݸ��Ƶ�`BSP/u8g2/u8g2_fonts_custom.c`���滻

# �������

---

- [peng-zhihui/HelloWord-Keyboard](https://github.com/peng-zhihui/HelloWord-Keyboard)  
- [fontforge](https://github.com/fontforge/fontforge)

# Э��

---

MIT License