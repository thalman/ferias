AUTOMAKE_OPTIONS = foreign
AM_CXXFLAGS = ${my_CXXFLAGS}
bin_PROGRAMS = ferias
ferias_SOURCES = src/milter/configuration.cc \
                 src/milter/feriasmilter.cc \
                 src/milter/httpclient.cc \
                 src/milter/mailer.cc \
                 src/milter/main.cc \
                 src/milter/vsjson.c
ferias_LDFLAGS = -lpthread ${LIBCURL_LIBS}
ferias_LDADD = -lmilter
