## Process this file with automake to produce Makefile.in

INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-Wall \
	@PACKAGE_CFLAGS@

bin_PROGRAMS = project2

project2_SOURCES = \
	main.c \
	support.c support.h \
	interface.c interface.h \
	callbacks.c callbacks.h \
	splash.c  copyright_png.h \
	myfuncs.c myfuncs.h \
	editor.c editor.h \
       	errors.h defines.h \
	includes.h \
	college.txt subcode.txt \
	sub_common_all.txt \
	sub_common_agri.txt \
	sub_agri.txt \
	sub_4th_agri.txt \
	sub_common_arts.txt \
	sub_arts.txt \
	sub_4th_arts.txt \
	sub_common_busi.txt \
	sub_busi.txt \
	sub_4th_busi.txt \
	sub_common_com.txt \
	sub_com.txt \
	sub_4th_com.txt \
	sub_common_home_eco.txt \
	sub_home_eco.txt \
	sub_4th_home_eco.txt \
	sub_common_is_st.txt \
	sub_is_st.txt \
	sub_4th_is_st.txt \
	sub_common_music.txt \
	sub_music.txt \
	sub_4th_music.txt \
	sub_common_sc.txt \
	sub_sc.txt \
	sub_4th_sc.txt \
	sub_common_so_sc.txt \
	sub_so_sc.txt \
	sub_4th_so_sc.txt \
	pair.txt \
	website.txt \
	special.txt


project2_LDADD = @PACKAGE_LIBS@ $(INTLLIBS) -lcrypto

