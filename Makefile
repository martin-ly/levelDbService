TAG_OBJECT=src/atomic.o src/table.o src/tagServiceHandler.o src/tagServer.o
USR_OBJECT=src/atomic.o src/table.o src/usrServiceHandler.o src/usrServer.o
THRIFT_SRC=src/levelThrift/gen-cpp
THRIFT_OBJECT=${THRIFT_SRC}/kvsys_constants.o ${THRIFT_SRC}/kvsys_types.o ${THRIFT_SRC}/tagService.o ${THRIFT_SRC}/usrService.o
CPP=g++
FLAG=-DGCC

THRIFT_ROOT=/usr/local/thrift
BOOST_ROOT=/usr/local/Cellar/boost/1.55.0_1

ITHRIFT=-I${THRIFT_ROOT}/include/ -I${THRIFT_ROOT}/include/thrift/
LTHRIFT=${THRIFT_ROOT}/lib/libthrift.a

IBOOST=-I${BOOST_ROOT}/include -I${BOOST_ROOT}/include/boost

ILEVELTHRIFT=-I./src/levelThrift/gen-cpp

ILEVELDB=-I./lib/leveldb/include
LLEVELDB=lib/leveldb/libleveldb.a

RM=rm -rf

all:levelService
$(THRIFT_OBJECT):%.o:%.cpp
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} -c $< -o $@
$(TAG_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} ${ILEVELDB} -DHAVE_NETINET_IN_H -c $< -o $@
$(USR_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} ${ILEVELDB} -DHAVE_NETINET_IN_H -c $< -o $@
levelService: $(TAG_OBJECT) $(USR_OBJECT) $(THRIFT_OBJECT)
	$(CPP) $(FLAG) -o bin/tagService ${TAG_OBJECT} ${THRIFT_OBJECT} ${LLEVELDB} ${LTHRIFT} -lpthread -ldl  
	$(CPP) $(FLAG) -o bin/usrService ${USR_OBJECT} ${THRIFT_OBJECT} ${LLEVELDB} ${LTHRIFT} -lpthread -ldl  
clean:
	$(RM) src/*.o bin/tagService bin/usrService
	$(RM) ${THRIFT_SRC}
	thrift -o src/levelThrift/ --gen cpp src/levelThrift/kvsys.thrift
 
