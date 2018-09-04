bin_PROGRAMS=astaire
astaire_SOURCES=Astaire.cpp \
                connection/ConnectionManager.cpp \
                connection/Packet.cpp \
                handler/Handler.cpp \
                handler/JavaHandler.cpp \
                utils/JsonUtils.cpp \
                utils/HandlerUtils.cpp
astaire_LDADD=$(astaire_LIBS)

if ENABLE_JAVA
if JAVA_HOME_SET
astaire_CXXFLAGS="$(astaire_CXXFLAGS) -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/linux" 
endif
endif

#if JSONCPP_INCLUDE_PATH_FIX
#astaire_CXXFLAGS="$(astaire_CXXFLAGS) -I/usr/include/jsoncpp/"
#endif
