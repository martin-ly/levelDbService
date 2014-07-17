COMMON_OBJECT=src/atomic.o src/table.o
TAG_OBJECT=src/tagServiceHandler.o src/tagServer.o
USR_OBJECT=src/usrServiceHandler.o src/usrServer.o
THRIFT_SRC=src/levelThrift/gen-cpp
THRIFT_OBJECT=${THRIFT_SRC}/kvsys_constants.o ${THRIFT_SRC}/kvsys_types.o ${THRIFT_SRC}/tagService.o ${THRIFT_SRC}/usrService.o
CPP=g++ -std=c++11
FLAG=-DGCC

THRIFT_ROOT=/usr/local/Cellar/thrift/0.9.1
BOOST_ROOT=/usr/local/Cellar/boost/1.55.0_1

ITHRIFT=-I${THRIFT_ROOT}/include/ -I${THRIFT_ROOT}/include/thrift/
LTHRIFT=${THRIFT_ROOT}/lib/libthrift.a

IBOOST=-I${BOOST_ROOT}/include -I${BOOST_ROOT}/include/boost

ILEVELTHRIFT=-I./src/levelThrift/gen-cpp

ILEVELDB=-I./lib/leveldb/include
LLEVELDB=lib/leveldb/libleveldb.a

all:levelService
$(THRIFT_OBJECT):%.o:%.cpp
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} -c $< -o $@
$(COMMON_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} ${ILEVELDB} -DHAVE_NETINET_IN_H -c $< -o $@
$(TAG_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} ${ILEVELDB} -DHAVE_NETINET_IN_H -c $< -o $@
$(USR_OBJECT):%.o:%.cc
	$(CPP) $(FLAG) -I. ${ILEVELTHRIFT} ${ITHRIFT} ${IBOOST} ${ILEVELDB} -DHAVE_NETINET_IN_H -c $< -o $@
levelService: $(COMMON_OBJECT) $(TAG_OBJECT) $(USR_OBJECT) $(THRIFT_OBJECT)
	$(CPP) $(FLAG) -o bin/tagService ${COMMON_OBJECT} ${TAG_OBJECT} ${THRIFT_OBJECT} ${LLEVELDB} ${LTHRIFT} -lpthread -ldl
	$(CPP) $(FLAG) -o bin/usrService ${COMMON_OBJECT} ${USR_OBJECT} ${THRIFT_OBJECT} ${LLEVELDB} ${LTHRIFT} -lpthread -ldl

clean:
	rm -rf src/*.o bin/*
	rm -rf ${THRIFT_SRC}
	thrift -o src/levelThrift/ --gen cpp src/levelThrift/kvsys.thrift
 
