# nxtOSEK���[�g�f�B���N�g��
NXTOSEK_ROOT = ../nxtOSEK

# �^�[�Q�b�g���s�`���t�@�C����
#TARGET = model_impl
TARGET = maimai

# �C���N���[�h�p�X
USER_INC_PATH= $(NXTOSEK_ROOT)/ecrobot/nxtway_gs_balancer

# ���C�u����
USER_LIB = nxtway_gs_balancer

# C�\�[�X�t�@�C��
TARGET_SOURCES = balancer_param.c LineTracer.c  UI.c BalanceRunner.c TouchSensor.c GyroSensor.c LightSensor.c Motor.c model_impl.c SonarSensor.c Maimai.c Basic.c DistMeasure.c

# TOPPERS/ATK1(OSEK)�ݒ�t�@�C��
TOPPERS_OSEK_OIL_SOURCE = model_impl.oil

# ���L�̃}�N���͕ύX���Ȃ��ł�������
O_PATH ?= build

include $(NXTOSEK_ROOT)/ecrobot/ecrobot.mak
