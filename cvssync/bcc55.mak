# ---------------------------------------------------------------------------
#  CVS用 文字コード変換(EUC <-> SJIS)付きフォルダ同期ツール
#  Borland-C Ver5.5 用 makefile
#
#                                      Copyright (C) 2002 by Ryuji Fuchikami
# ---------------------------------------------------------------------------


# ツール
CC   = bcc32
LINK = bcc32


# オプション
CFLAGS = -c
LFLAGS = 


# ターゲット
TARGET = CvsCync.exe


# オブジェクトファイル
OBJS = CvsSync.obj \
       ReadConfig.obj \
       TimeStamp.obj \
       TreeSearch.obj \
       AssociationVector.obj \
       StringVector.obj


# ルールの宣言
.suffixes: .cpp


# ターゲット生成
$(TARGET): $(OBJS)
	$(LINK) $(OBJS) -o$(TARGET)


clean:
	del *.obj


.cpp.obj:
	$(CC) -P -c $<



# ---------------------------------------------------------------------------
#  Copyright (C) 2002 by Ryuji Fuchikami
# ---------------------------------------------------------------------------
