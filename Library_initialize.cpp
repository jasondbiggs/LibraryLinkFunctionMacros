#include <LLU/ErrorLog/LibraryLinkError.h>
#include <LLU/ErrorLog/Logger.h>
#include <LLU/LibraryLinkFunctionMacro.h>

#include "../ManagedTypes.h"

#define MANAGED_CLASS(className, storeName, wlType)                                        \
	LLU::ManagedExpressionStore<className> LIBRARY_NAMESPACE::storeName;                   \
	template<>                                                                             \
	void LLU::manageInstanceCallback<className>(WolframLibraryData, mbool mode, mint id) { \
		LIBRARY_NAMESPACE::storeName.manageInstance(mode, id);                             \
	};

MANAGED_CLASS_LIST

#undef MANAGED_CLASS

namespace LIBRARY_NAMESPACE {

extern "C" DLLEXPORT int WolframLibrary_initialize(WolframLibraryData libData) {
	auto err = LLU::ErrorCode::NoError;
	try {
		LLU::LibraryData::setLibraryData(libData);

#define MANAGED_CLASS(className, storeName, wlType) storeName.registerType(wlType);
		MANAGED_CLASS_LIST
#undef MANAGED_CLASS

		LLU::ErrorManager::registerPacletErrors(libraryExceptionList);
	} catch(const LLU::LibraryLinkError& e) {
		err = e.which();
	}
	return err;
}

extern "C" DLLEXPORT void WolframLibrary_uninitialize(WolframLibraryData libData) {
#define MANAGED_CLASS(className, storeName, wlType) storeName.unregisterType(libData);
	MANAGED_CLASS_LIST
#undef MANAGED_CLASS
}

LLU_LIBRARY_FUNCTION(clearManagedInstances) {
	auto type = mngr.getString(0);

#define MANAGED_CLASS(className, storeName, wlType) \
	if(type == wlType) {                            \
		std::vector<mint> instances;                \
		for(auto instance : storeName) {            \
			instances.push_back(instance.first);    \
		}                                           \
		for(auto id : instances) {                  \
			storeName.releaseInstance(id);          \
		}                                           \
	} else
	MANAGED_CLASS_LIST
#undef MANAGED_CLASS
	if(type == "All") {

#define MANAGED_CLASS(className, storeName, wlType) \
	{                                               \
		std::vector<mint> instances;                \
		for(auto instance : storeName) {            \
			instances.push_back(instance.first);    \
		}                                           \
		for(auto id : instances) {                  \
			storeName.releaseInstance(id);          \
		}                                           \
	}
		MANAGED_CLASS_LIST
	} else {
		LLU::ErrorManager::throwException("UnknownType", type);
	}
#undef MANAGED_CLASS
}

LLU_LIBRARY_FUNCTION(managedInstanceIDList) {
	auto type = mngr.getString(0);

#define MANAGED_CLASS(className, storeName, wlType)             \
	if(type == wlType) {                                        \
		LLU::Tensor<mint> result(0, {(mint) storeName.size()}); \
		auto i = 0;                                             \
		for(auto instance : storeName) {                        \
			result[i++] = instance.first;                       \
		}                                                       \
		mngr.set(result);                                       \
	} else
	MANAGED_CLASS_LIST
#undef MANAGED_CLASS
	{
		LLU::ErrorManager::throwException("UnknownType", type);
	}
}

LLU_LIBRARY_FUNCTION(deleteInstance) {
	auto type = mngr.getString(0);
	auto idx = mngr.getInteger<mint>(1);

#define MANAGED_CLASS(className, storeName, wlType) \
	if(type == wlType) {                            \
		storeName.releaseInstance(idx);             \
	} else
	MANAGED_CLASS_LIST
#undef MANAGED_CLASS
	{
		LLU::ErrorManager::throwException("UnknownType", type);
	}
}

}	 // namespace LIBRARY_NAMESPACE
