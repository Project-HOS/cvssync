# ---------------------------------------------------------------------------
#  CVS�� ʸ���������Ѵ�(EUC <-> SJIS)�դ��ե����Ʊ���ġ���
#  Borland-C Ver5.5 �� makefile
#
#                                      Copyright (C) 2002 by Ryuji Fuchikami
# ---------------------------------------------------------------------------


# �ġ���
CC   = bcc32
LINK = bcc32


# ���ץ����
CFLAGS = -c
LFLAGS = 


# �������å�
TARGET = CvsCync.exe


# ���֥������ȥե�����
OBJS = CvsSync.obj \
       ReadConfig.obj \
       TimeStamp.obj \
       TreeSearch.obj \
       AssociationVector.obj \
       StringVector.obj


# �롼������
.suffixes: .cpp


# �������å�����
$(TARGET): $(OBJS)
	$(LINK) $(OBJS) -o$(TARGET)


clean:
	del *.obj


.cpp.obj:
	$(CC) -P -c $<



# ---------------------------------------------------------------------------
#  Copyright (C) 2002 by Ryuji Fuchikami
# ---------------------------------------------------------------------------
