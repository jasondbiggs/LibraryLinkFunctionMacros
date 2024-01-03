/*
   FunctionMacros.h
  
    Created on: Dec 27, 2020
        Author: jasonb
 */

#ifndef FUNCTIONMACROS_H_
#define FUNCTIONMACROS_H_

#include <LLU/ErrorLog/ErrorManager.h>
#include <LLU/LLU.h>
#include <LLU/LibraryLinkFunctionMacro.h>

#define BEGIN_LIBRARY_FUNCTION(name)                                                                                      \
	EXTERN_C DLLEXPORT int name([[maybe_unused]] WolframLibraryData libData, mint Argc, MArgument* Args, MArgument Res) { \
		auto err = LLU::ErrorCode::NoError;                                                                               \
		auto arg = 0;                                                                                                     \
		LLU::MArgumentManager mngr(libData, Argc, Args, Res);                                                             \
		try {                                                                                                             \
		try

#define BEGIN_WSTP_FUNCTION(name, argc)                                 \
	EXTERN_C DLLEXPORT int name(WolframLibraryData, WSLINK);            \
	int name([[maybe_unused]] WolframLibraryData libData, WSLINK wsl) { \
		auto err = LLU::ErrorCode::NoError;                             \
		try {                                                           \
			LLU::WSStream<LLU::WS::Encoding::UTF8> stream(wsl, argc);      \
		try

#define END_LIBRARY_FUNCTION                                                  \
	catch(...) {                                                              \
		throw;                                                                \
	}                                                                         \
	}                                                                         \
	catch(const LLU::LibraryLinkError& e) {                                   \
		err = e.which();                                                      \
	}                                                                         \
	catch(const std::exception& e) {                                          \
		try {                                                                 \
			LLU::ErrorManager::throwException("StandardException", e.what()); \
		} catch(const LLU::LibraryLinkError& e) {                             \
			err = e.which();                                                  \
		}                                                                     \
	}                                                                         \
	catch(...) {                                                              \
		err = LLU::ErrorCode::FunctionError;                                  \
	}                                                                         \
	return err;                                                               \
	}

#define LIBRARY_RETURN return err

#define MEMBER_FUNCTION(name) BEGIN_LIBRARY_FUNCTION(name)
#define CONSTRUCTOR_FUNCTION(name) BEGIN_LIBRARY_FUNCTION(name)
#define EXPORTED_FUNCTION(name) BEGIN_LIBRARY_FUNCTION(name)

#define LIBRARYLINK_ENUM(name) enum class name

#define CATCH_THROW_EXCEPTION(toCatch, toThrow)               \
	catch(const toCatch& e) {                                 \
		LLU::ErrorManager::throwException(toThrow, e.what()); \
	}

#define CHECK_THROW(check) if(!(check)) { LLU::ErrorManager::throwException("ValueErrorException", #check);}

#endif /* FUNCTIONMACROS_H_ */
