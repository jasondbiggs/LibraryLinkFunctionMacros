/*
 * PrintStream.h
 *
 *  Created on: Dec 28, 2022
 *      Author: jasonb
 */

#ifndef LLUU_PRINTSTREAM_H_
#define LLUU_PRINTSTREAM_H_

#include <LLU/LLU.h>

namespace WLPrint {

template<typename T>
inline void print(T msg) {
	auto libData = LLU::LibraryData::API();
	if(libData->AbortQ())
		return;	   // trying to use the MathLink connection during an abort appears to break it
	LLU::WSStream<LLU::WS::Encoding::UTF8> mls {libData->getWSLINK(libData)};
	mls << LLU::WS::Function("EvaluatePacket", 1);
	mls << LLU::WS::Function("Print", 1) << msg;
	libData->processWSLINK(mls.get());
	auto pkt = WSNextPacket(mls.get());
  if (pkt == RETURNPKT) {
	WSNewPacket(mls.get());
  }
}

template<typename T, typename T2>
inline void print(T msg, T2 val) {
	auto libData = LLU::LibraryData::API();
	if(libData->AbortQ())
		return;	   // trying to use the MathLink connection during an abort appears to break it
	LLU::WSStream<LLU::WS::Encoding::UTF8> mls {libData->getWSLINK(libData)};
	mls << LLU::WS::Function("EvaluatePacket", 1);
	mls << LLU::WS::Function("Print", 2) << msg << val;
	libData->processWSLINK(mls.get());
	auto pkt = WSNextPacket(mls.get());
	if (pkt == RETURNPKT) {
		WSNewPacket(mls.get());
	  }

}

}


#endif /* LLUU_PRINTSTREAM_H_ */
